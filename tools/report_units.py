#!/usr/bin/env python3
"""Regenerate objdiff report entries for a few units instead of the whole project.

    python tools/report_units.py --changed              # units rebuilt since the last report
    python tools/report_units.py Shop_boardU src/Engine/Text.c
    python tools/report_units.py --changed --no-merge   # just print the delta

The selected units are diffed with `objdiff-cli report generate` on a filtered
copy of objdiff.json, then spliced into build/<ver>/report.json (the top-level
and category measures are recomputed from the units). Per-function changes are
printed as they are found.

--changed selects units whose compiled or delinked object is newer than
report.json, plus units whose `complete` flag or categories changed in
delinks.txt. An existing report.json (from `ninja report`) is needed to update
it; without one this only prints.
"""

import argparse
import json
import os
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OBJDIFF = ROOT / ("objdiff-cli.exe" if os.name == "nt" else "objdiff-cli")

# Measures objdiff stores as 64-bit counters (serialized as strings) and as plain integers.
U64_KEYS = ("total_code", "matched_code", "total_data", "matched_data", "complete_code", "complete_data")
INT_KEYS = ("total_functions", "matched_functions", "total_units", "complete_units")

# Unit metadata that changes what the report contains without touching any object file.
METADATA_KEYS = ("complete", "progress_categories")


def f32(x):
    """Round to single precision; objdiff computes and stores percentages as f32."""
    return struct.unpack("f", struct.pack("f", x))[0]


def pct(part, whole):
    return f32(100.0 * part / whole) if whole else 0.0


def aggregate(units):
    """Combine per-unit measures the way objdiff does for the whole report or a category."""
    sums = dict.fromkeys(U64_KEYS + INT_KEYS, 0)
    fuzzy = 0.0
    for unit in units:
        measures = unit["measures"]
        fuzzy += measures.get("fuzzy_match_percent", 0.0) * int(measures.get("total_code", 0))
        for key in U64_KEYS + INT_KEYS:
            sums[key] += int(measures.get(key, 0))
    out = {"fuzzy_match_percent": f32(fuzzy / sums["total_code"]) if sums["total_code"] else 0.0}
    for key in U64_KEYS:
        out[key] = str(sums[key])
    out["matched_code_percent"] = pct(sums["matched_code"], sums["total_code"])
    out["matched_data_percent"] = pct(sums["matched_data"], sums["total_data"])
    out["total_functions"] = sums["total_functions"]
    out["matched_functions"] = sums["matched_functions"]
    out["matched_functions_percent"] = pct(sums["matched_functions"], sums["total_functions"])
    out["complete_code_percent"] = pct(sums["complete_code"], sums["total_code"])
    out["complete_data_percent"] = pct(sums["complete_data"], sums["total_data"])
    out["total_units"] = sums["total_units"]
    out["complete_units"] = sums["complete_units"]
    return out


def recompute(report):
    """Refresh the report's top-level and per-category measures from its units."""
    report["measures"] = aggregate(report["units"])
    for category in report.get("categories", []):
        members = [
            u for u in report["units"] if category["id"] in u.get("metadata", {}).get("progress_categories", [])
        ]
        category["measures"] = aggregate(members)


def write_json_atomic(path, data):
    """Replace `path` in one step so a concurrent reader never sees a half-written report."""
    tmp = path.with_name(f"{path.name}.{os.getpid()}.tmp")
    tmp.write_text(json.dumps(data))
    os.replace(tmp, path)


def select_units(project, report_path, names, changed):
    units = project["units"]
    picked = []
    if changed:
        since = report_path.stat().st_mtime if report_path.exists() else 0
        reported = {}
        if report_path.exists():
            reported = {u["name"]: u.get("metadata", {}) for u in json.loads(report_path.read_text())["units"]}
        for unit in units:
            old_metadata = reported.get(unit["name"])
            new_metadata = unit.get("metadata", {})
            if old_metadata is None or any(old_metadata.get(k) != new_metadata.get(k) for k in METADATA_KEYS):
                picked.append(unit)
                continue
            for key in ("base_path", "target_path"):
                path = ROOT / unit[key] if key in unit else None
                if path and path.exists() and path.stat().st_mtime > since:
                    picked.append(unit)
                    break
    for name in names:
        name = name.replace("\\", "/")
        stem = name.rsplit(".", 1)[0] if name.endswith((".c", ".cpp", ".o")) else name
        hits = [
            u
            for u in units
            if u["name"] == stem
            or u["name"].endswith("/" + stem)
            or u.get("metadata", {}).get("source_path") == name
            or u.get("base_path") == name
        ]
        if not hits:
            sys.exit(f"no objdiff unit matches '{name}'")
        picked += hits
    return list({u["name"]: u for u in picked}.values())


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("units", nargs="*", help="unit names or source/object paths")
    parser.add_argument("--changed", action="store_true", help="units with objects newer than report.json")
    parser.add_argument("--no-merge", action="store_true", help="don't update report.json")
    args = parser.parse_args()

    project = json.loads((ROOT / "objdiff.json").read_text())
    target_dir = ROOT / project["target_dir"]
    report_path = target_dir / "report.json"
    units = select_units(project, report_path, args.units, args.changed)
    if not units:
        print("report_units: nothing to do")
        return

    target_dir.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(dir=target_dir) as tmp:
        tmp = Path(tmp)
        subproject = {k: v for k, v in project.items() if k != "units"}
        subproject["units"] = [
            {**u, **{k: str(ROOT / u[k]) for k in ("target_path", "base_path") if k in u}} for u in units
        ]
        (tmp / "objdiff.json").write_text(json.dumps(subproject))
        out = tmp / "report.json"
        result = subprocess.run(
            [str(OBJDIFF), "report", "generate", "-p", str(tmp), "-o", str(out)],
            capture_output=True,
            text=True,
        )
        if result.returncode != 0:
            sys.exit(result.stdout + result.stderr)
        fresh = json.loads(out.read_text())

    old = json.loads(report_path.read_text()) if report_path.exists() else {"units": []}
    old_units = {u["name"]: u for u in old["units"]}
    for new_unit in fresh["units"]:
        old_unit = old_units.get(new_unit["name"])
        old_funcs = {f["name"]: f.get("fuzzy_match_percent", 0.0) for f in (old_unit or {}).get("functions", [])}
        new_funcs = {f["name"]: f.get("fuzzy_match_percent", 0.0) for f in new_unit.get("functions", [])}
        old_fuzzy = (old_unit or {}).get("measures", {}).get("fuzzy_match_percent", 0.0)
        new_fuzzy = new_unit["measures"].get("fuzzy_match_percent", 0.0)
        print(f"{new_unit['name']}: {old_fuzzy:.2f}% -> {new_fuzzy:.2f}%")
        for name in sorted(set(old_funcs) | set(new_funcs)):
            before, after = old_funcs.get(name), new_funcs.get(name)
            if before is None or after is None or abs(before - after) > 1e-4:
                shown_before = "-" if before is None else f"{before:.2f}"
                shown_after = "-" if after is None else f"{after:.2f}"
                print(f"  {name}: {shown_before} -> {shown_after}{'  MATCH' if after == 100.0 else ''}")

    if not args.no_merge and "measures" in old:
        fresh_by_name = {u["name"]: u for u in fresh["units"]}
        merged = [fresh_by_name.get(u["name"], u) for u in old["units"]]
        merged += [u for u in fresh["units"] if u["name"] not in old_units]
        old["units"] = merged
        recompute(old)
        write_json_atomic(report_path, old)


if __name__ == "__main__":
    main()
