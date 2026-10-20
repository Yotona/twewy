#!/usr/bin/env python3
"""Exercise mwcc_batch.py with a fake compiler that logs what it was asked to build."""

import os
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path

WRAPPER = Path(__file__).resolve().parents[1] / "mwcc_batch.py"

FAKE_CC = r'''
import os, sys
args = sys.argv[1:]
out = args[args.index("-o") + 1]
srcs = args[args.index("-c") + 1 : args.index("-o")]
with open(os.path.join(out, "..", "calls.log"), "a") as log:
    log.write(" ".join(os.path.basename(s) for s in srcs) + "\n")
for s in srcs:
    text = open(s).read()
    if "FAIL" in text:
        print(s + ": error"); sys.exit(1)
    stem = os.path.splitext(os.path.basename(s))[0]
    open(os.path.join(out, stem + ".o"), "w").write(text)
    hdr = text.split()[1] if text.startswith("hdr") else ""
    bs, nl = chr(92), chr(10)
    target = out + bs + stem + ".o: " + s
    text = target + " " + bs + nl + chr(9) + hdr + nl if hdr else target + nl
    open(os.path.join(out, stem + ".d"), "w").write(text)
'''


class MwccBatchTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="mwcc_batch_")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        (self.root / "out").mkdir()
        (self.root / "cc.py").write_text(FAKE_CC)
        self.cc = self.root / "cc.exe.stamp"
        self.cc.write_text("compiler")
        self.header = self.root / "h1.h"
        self.header.write_text("header")
        self.srcs = []
        for name, text in (("a", f"hdr {self.header}"), ("b", "plain"), ("c", "plain")):
            path = self.root / f"{name}.c"
            path.write_text(text)
            self.srcs.append(path.name)
        self.age_all()

    def age_all(self):
        old = time.time() - 1000
        for p in self.root.rglob("*"):
            if p.is_file():
                os.utime(p, (old, old))

    def build(self, flags="-O4", srcs=None, expect=0):
        r = subprocess.run(
            [sys.executable, str(WRAPPER), "--outdir", "out", "--target", "out/a.o",
             "--depfile", "out/.batch_a.d", "--extra-dep", str(self.cc),
             "--srcs", *(srcs or self.srcs), "--", sys.executable, "cc.py", flags],
            cwd=self.root, capture_output=True, text=True,
        )
        self.assertEqual(r.returncode, expect, r.stdout + r.stderr)
        return r

    def calls(self):
        log = self.root / "calls.log"
        return log.read_text().splitlines() if log.exists() else []

    def bump(self, path):
        # Age everything built so far, then make one input newer than the outputs
        # but not newer than "now", like a real edit.
        self.age_all()
        recent = time.time() - 500
        os.utime(path, (recent, recent))

    def test_first_build_compiles_everything_in_one_process(self):
        self.build()
        self.assertEqual(self.calls(), ["a.c b.c c.c"])
        dep = (self.root / "out/.batch_a.d").read_text()
        self.assertTrue(dep.startswith("out/a.o: "))
        self.assertIn("h1.h", dep)
        self.assertIn("b.c", dep)

    def test_up_to_date_build_does_nothing(self):
        self.build()
        self.build()
        self.assertEqual(self.calls(), ["a.c b.c c.c"])

    def test_only_touched_source_is_rebuilt(self):
        self.build()
        self.bump(self.root / "b.c")
        self.build()
        self.assertEqual(self.calls()[1:], ["b.c"])

    def test_header_rebuilds_only_its_includer(self):
        self.build()
        self.bump(self.header)
        self.build()
        self.assertEqual(self.calls()[1:], ["a.c"])
        # Members that were not rebuilt keep their timestamps (relinking depends on it).
        for name in ("b", "c"):
            self.assertLess((self.root / f"out/{name}.o").stat().st_mtime_ns, self.header.stat().st_mtime_ns)
        self.build()
        self.assertEqual(len(self.calls()), 2)

    def test_command_change_rebuilds_everything(self):
        self.build()
        self.build(flags="-O0")
        self.assertEqual(self.calls()[1:], ["a.c b.c c.c"])

    def test_compiler_change_rebuilds_everything(self):
        self.build()
        self.bump(self.cc)
        self.build()
        self.assertEqual(self.calls()[1:], ["a.c b.c c.c"])

    def test_missing_object_or_dep_rebuilds_that_source(self):
        self.build()
        (self.root / "out/c.o").unlink()
        self.build()
        (self.root / "out/b.d").unlink()
        self.build()
        self.assertEqual(self.calls()[1:], ["c.c", "b.c"])

    def test_failure_still_builds_the_others_and_reports_failure(self):
        (self.root / "b.c").write_text("FAIL")
        self.age_all()
        r = self.build(expect=1)
        self.assertIn("b.c: error", r.stdout)
        self.assertEqual(self.calls(), ["a.c b.c c.c", "a.c", "b.c", "c.c"])
        self.assertTrue((self.root / "out/a.o").exists())
        self.assertTrue((self.root / "out/c.o").exists())
        self.assertFalse((self.root / "out/b.o").exists())
        # No command signature was recorded for a failed first build, so the fix
        # rebuilds the chunk; the next build is then clean.
        (self.root / "b.c").write_text("fixed")
        self.build()
        self.assertEqual(self.calls()[-1], "a.c b.c c.c")
        self.build()
        self.assertEqual(self.calls()[-1], "a.c b.c c.c")
        self.assertEqual(len(self.calls()), 5)

    def test_single_source_failure(self):
        (self.root / "a.c").write_text("FAIL")
        self.build(srcs=["a.c"], expect=1)
        self.assertEqual(self.calls(), ["a.c"])


if __name__ == "__main__":
    unittest.main()
