#!/usr/bin/env python3
"""Check report merging and unit selection without running objdiff."""

import contextlib
import io
import json
import os
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import report_units  # noqa: E402

OLD_TIME = 1_600_000_000
NEW_TIME = 1_700_000_000


def unit(name, code=100, fuzzy=50.0, funcs=(), complete=False, categories=(), **paths):
    matched = [f for f in funcs if f[1] == 100.0]
    return {
        "name": name,
        "measures": {
            "fuzzy_match_percent": fuzzy,
            "total_code": str(code),
            "matched_code": str(sum(1 for _ in matched) * 10),
            "total_data": "0",
            "total_functions": len(funcs),
            "matched_functions": len(matched),
            "complete_code": str(code if complete else 0),
            "complete_units": 1 if complete else 0,
            "total_units": 1,
        },
        "functions": [{"name": n, "fuzzy_match_percent": p} for n, p in funcs],
        "metadata": {"complete": complete, "progress_categories": list(categories), "source_path": f"{name}.c"},
        **paths,
    }


class AggregateTests(unittest.TestCase):
    def test_f32_rounds_to_single_precision(self):
        self.assertNotEqual(report_units.f32(0.1), 0.1)
        self.assertEqual(report_units.f32(0.5), 0.5)
        self.assertEqual(report_units.pct(1, 3), struct.unpack("f", struct.pack("f", 100 / 3))[0])

    def test_aggregate_sums_counts_and_weights_fuzzy_by_code_size(self):
        small = unit("small", code=100, fuzzy=50.0, funcs=[("a", 100.0)], complete=True)
        big = unit("big", code=300, fuzzy=100.0, funcs=[("b", 100.0), ("c", 0.0)])
        totals = report_units.aggregate([small, big])
        self.assertEqual(totals["fuzzy_match_percent"], 87.5)
        self.assertEqual(totals["total_code"], "400")
        self.assertEqual(totals["complete_code"], "100")
        self.assertEqual(totals["total_units"], 2)
        self.assertEqual(totals["complete_units"], 1)
        self.assertEqual(totals["total_functions"], 3)
        self.assertEqual(totals["matched_functions"], 2)
        self.assertEqual(totals["matched_functions_percent"], report_units.pct(2, 3))

    def test_aggregate_of_no_units_is_zero_without_dividing(self):
        totals = report_units.aggregate([])
        self.assertEqual(totals["fuzzy_match_percent"], 0.0)
        self.assertEqual(totals["total_code"], "0")
        self.assertEqual(totals["matched_code_percent"], 0.0)

    def test_recompute_groups_categories_by_unit_metadata(self):
        report = {
            "units": [
                unit("a", categories=["Engine"]),
                unit("b", categories=["Engine", "UI"]),
                unit("c", categories=["UI"], complete=True),
            ],
            "categories": [{"id": "Engine"}, {"id": "UI"}, {"id": "Empty"}],
        }
        report_units.recompute(report)
        by_id = {c["id"]: c["measures"] for c in report["categories"]}
        self.assertEqual(report["measures"]["total_units"], 3)
        self.assertEqual(by_id["Engine"]["total_units"], 2)
        self.assertEqual(by_id["UI"]["total_units"], 2)
        self.assertEqual(by_id["UI"]["complete_units"], 1)
        self.assertEqual(by_id["Empty"]["total_units"], 0)


class ProjectTestCase(unittest.TestCase):
    """A throwaway repo root with an objdiff.json, object files and a report."""

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="report units ")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        patch = mock.patch.object(report_units, "ROOT", self.root)
        patch.start()
        self.addCleanup(patch.stop)
        self.report_path = self.root / "build/usa/report.json"

    def touch(self, rel, when):
        path = self.root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(b"obj")
        os.utime(path, (when, when))

    def write_project(self, units):
        project = {"target_dir": "build/usa", "units": units}
        (self.root / "objdiff.json").write_text(json.dumps(project))
        return project

    def write_report(self, units, categories=(), when=None):
        report = {"units": units, "categories": [dict(c) for c in categories], "measures": {}}
        report_units.recompute(report)
        self.report_path.parent.mkdir(parents=True, exist_ok=True)
        self.report_path.write_text(json.dumps(report))
        if when is not None:
            os.utime(self.report_path, (when, when))

    @staticmethod
    def project_unit(name, **kwargs):
        return {
            "name": name,
            "target_path": f"build/usa/delinks/{name}.o",
            "base_path": f"build/usa/{name}.o",
            "metadata": {"complete": False, "progress_categories": [], "source_path": f"{name}.c", **kwargs},
        }


class SelectUnitsTests(ProjectTestCase):
    def test_names_match_unit_source_and_object_paths_without_duplicates(self):
        project = self.write_project([self.project_unit("src/Engine/Text"), self.project_unit("src/Field/Events")])
        picked = report_units.select_units(
            project,
            self.report_path,
            ["Text", "src/Engine/Text.c", "src/Engine/Text", "build\\usa\\src/Engine/Text.o", "src/Field/Events.c"],
            False,
        )
        self.assertEqual([u["name"] for u in picked], ["src/Engine/Text", "src/Field/Events"])

    def test_unknown_name_exits_with_a_message(self):
        project = self.write_project([self.project_unit("src/Engine/Text")])
        with self.assertRaises(SystemExit) as raised:
            report_units.select_units(project, self.report_path, ["Nope"], False)
        self.assertIn("Nope", str(raised.exception))

    def test_changed_selects_units_with_objects_newer_than_the_report(self):
        newer_base, newer_target, untouched = (self.project_unit(n) for n in ("newer_base", "newer_target", "old"))
        project = self.write_project([newer_base, newer_target, untouched])
        self.write_report([unit(u["name"]) for u in project["units"]], when=1_650_000_000)
        self.touch("build/usa/newer_base.o", NEW_TIME)
        self.touch("build/usa/delinks/newer_target.o", NEW_TIME)
        self.touch("build/usa/old.o", OLD_TIME)
        picked = report_units.select_units(project, self.report_path, [], True)
        self.assertEqual([u["name"] for u in picked], ["newer_base", "newer_target"])

    def test_changed_selects_units_whose_complete_flag_or_categories_changed(self):
        flipped = self.project_unit("flipped", complete=True)
        recategorized = self.project_unit("recategorized", progress_categories=["UI"])
        same = self.project_unit("same")
        project = self.write_project([flipped, recategorized, same])
        self.write_report([unit("flipped"), unit("recategorized"), unit("same")], when=1_650_000_000)
        for name in ("flipped", "recategorized", "same"):
            self.touch(f"build/usa/{name}.o", OLD_TIME)
        picked = report_units.select_units(project, self.report_path, [], True)
        self.assertEqual([u["name"] for u in picked], ["flipped", "recategorized"])

    def test_changed_selects_units_missing_from_the_report(self):
        project = self.write_project([self.project_unit("brand_new"), self.project_unit("known")])
        self.write_report([unit("known")], when=1_650_000_000)
        self.touch("build/usa/known.o", OLD_TIME)
        picked = report_units.select_units(project, self.report_path, [], True)
        self.assertEqual([u["name"] for u in picked], ["brand_new"])


class MainTests(ProjectTestCase):
    def setUp(self):
        super().setUp()
        self.calls = []
        self.fresh_units = []
        self.returncode = 0

    def fake_objdiff(self, args, **kwargs):
        project_dir = Path(args[args.index("-p") + 1])
        sub = json.loads((project_dir / "objdiff.json").read_text())
        self.calls.append(sub)
        if self.returncode:
            return subprocess.CompletedProcess(args, self.returncode, "partial output\n", "Failed: boom\n")
        wanted = {u["name"] for u in sub["units"]}
        report = {"units": [u for u in self.fresh_units if u["name"] in wanted]}
        Path(args[args.index("-o") + 1]).write_text(json.dumps(report))
        return subprocess.CompletedProcess(args, 0, "", "")

    def run_main(self, *argv):
        stdout = io.StringIO()
        with (
            mock.patch.object(sys, "argv", ["report_units.py", *argv]),
            mock.patch.object(report_units.subprocess, "run", self.fake_objdiff),
            contextlib.redirect_stdout(stdout),
        ):
            report_units.main()
        return stdout.getvalue()

    def setup_text_unit(self):
        self.write_project([self.project_unit("text", progress_categories=["Engine"]), self.project_unit("other")])
        before = unit("text", code=100, fuzzy=50.0, funcs=[("fn_a", 50.0), ("fn_b", 0.0)], categories=["Engine"])
        other = unit("other", code=300, fuzzy=100.0, funcs=[("fn_c", 100.0)])
        self.write_report([before, other], [{"id": "Engine"}], when=1_650_000_000)
        self.touch("build/usa/text.o", NEW_TIME)
        self.touch("build/usa/other.o", OLD_TIME)
        self.fresh_units = [unit("text", code=100, fuzzy=90.0, funcs=[("fn_a", 100.0), ("fn_b", 0.0)], categories=["Engine"])]

    def test_changed_units_are_diffed_alone_and_spliced_into_the_report(self):
        self.setup_text_unit()
        output = self.run_main("--changed")
        self.assertEqual([u["name"] for u in self.calls[0]["units"]], ["text"])
        self.assertTrue(Path(self.calls[0]["units"][0]["base_path"]).is_absolute())
        self.assertIn("text: 50.00% -> 90.00%", output)
        self.assertIn("  fn_a: 50.00 -> 100.00  MATCH", output)
        self.assertNotIn("fn_b", output)
        report = json.loads(self.report_path.read_text())
        self.assertEqual([u["name"] for u in report["units"]], ["text", "other"])
        self.assertEqual(report["units"][0]["measures"]["fuzzy_match_percent"], 90.0)
        self.assertEqual(report["measures"]["fuzzy_match_percent"], 97.5)
        self.assertEqual(report["categories"][0]["measures"]["fuzzy_match_percent"], 90.0)

    def test_second_run_has_nothing_to_do(self):
        self.setup_text_unit()
        self.run_main("--changed")
        self.assertEqual(self.run_main("--changed").strip(), "report_units: nothing to do")
        self.assertEqual(len(self.calls), 1)

    def test_new_units_are_added_to_the_report(self):
        self.setup_text_unit()
        self.write_project([self.project_unit("text"), self.project_unit("other"), self.project_unit("added")])
        self.fresh_units.append(unit("added", code=100, fuzzy=100.0, funcs=[("fn_d", 100.0)]))
        self.run_main("added")
        report = json.loads(self.report_path.read_text())
        self.assertEqual([u["name"] for u in report["units"]], ["text", "other", "added"])
        self.assertEqual(report["measures"]["total_units"], 3)

    def test_no_merge_prints_but_leaves_the_report_alone(self):
        self.setup_text_unit()
        before = self.report_path.read_bytes()
        output = self.run_main("text", "--no-merge")
        self.assertIn("MATCH", output)
        self.assertEqual(self.report_path.read_bytes(), before)

    def test_objdiff_failure_reports_its_output_and_keeps_the_report(self):
        self.setup_text_unit()
        before = self.report_path.read_bytes()
        self.returncode = 1
        with self.assertRaises(SystemExit) as raised:
            self.run_main("text")
        self.assertIn("Failed: boom", str(raised.exception))
        self.assertEqual(self.report_path.read_bytes(), before)
        self.assertEqual([p.name for p in self.report_path.parent.glob("*.tmp")], [])

    def test_missing_report_prints_without_creating_one(self):
        self.write_project([self.project_unit("text")])
        self.fresh_units = [unit("text", fuzzy=90.0, funcs=[("fn_a", 100.0)])]
        output = self.run_main("text")
        self.assertIn("text: 0.00% -> 90.00%", output)
        self.assertFalse(self.report_path.exists())


if __name__ == "__main__":
    unittest.main()
