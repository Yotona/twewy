#!/usr/bin/env python3
"""Exercise output timestamp preservation using actual child processes."""

import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


WRAPPER = Path(__file__).resolve().parents[1] / "keep_mtime.py"
OLD_TIME = 1_600_000_000_123_456_700


class KeepMtimeTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="keep mtime ")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def output(self, name, data=b"original"):
        path = self.root / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
        os.utime(path, ns=(OLD_TIME, OLD_TIME))
        return path

    def run_command(self, paths, code, expected=0):
        result = subprocess.run(
            [
                sys.executable, str(WRAPPER), *map(str, paths), "--",
                sys.executable, "-c", code,
            ],
            cwd=self.root,
        )
        self.assertEqual(result.returncode, expected)

    def test_identical_output_keeps_nanosecond_timestamp(self):
        path = self.output("output file")
        before = path.stat().st_mtime_ns
        self.run_command(
            [path],
            "from pathlib import Path; Path('output file').write_bytes(b'original')",
        )
        self.assertEqual(path.stat().st_mtime_ns, before)

    def test_changed_output_with_same_size_keeps_new_timestamp(self):
        path = self.output("output")
        self.run_command(
            [path],
            "from pathlib import Path; Path('output').write_bytes(b'changed!')",
        )
        self.assertEqual(path.read_bytes(), b"changed!")
        self.assertNotEqual(path.stat().st_mtime_ns, OLD_TIME)

    def test_directory_handles_unchanged_changed_new_and_deleted_outputs(self):
        unchanged = self.output("nested/unchanged")
        changed = self.output("nested/changed")
        removed = self.output("nested/removed")
        self.run_command([self.root], """
from pathlib import Path
Path('nested/unchanged').write_bytes(b'original')
Path('nested/changed').write_bytes(b'changed')
Path('nested/new').write_bytes(b'new')
Path('nested/removed').unlink()
""")
        self.assertEqual(unchanged.stat().st_mtime_ns, OLD_TIME)
        self.assertNotEqual(changed.stat().st_mtime_ns, OLD_TIME)
        self.assertEqual((self.root / "nested/new").read_bytes(), b"new")
        self.assertFalse(removed.exists())

    def test_failure_propagates_without_restoring_timestamps(self):
        path = self.output("output")
        self.run_command(
            [path],
            "from pathlib import Path; Path('output').write_bytes(b'original'); raise SystemExit(7)",
            expected=7,
        )
        self.assertNotEqual(path.stat().st_mtime_ns, OLD_TIME)

    def test_multiple_outputs_and_missing_output(self):
        first = self.output("first")
        second = self.output("second")
        missing = self.root / "missing"
        self.run_command([first, second, missing], """
from pathlib import Path
Path('first').write_bytes(b'original')
Path('second').write_bytes(b'original')
Path('missing').write_bytes(b'created')
""")
        self.assertEqual(first.stat().st_mtime_ns, OLD_TIME)
        self.assertEqual(second.stat().st_mtime_ns, OLD_TIME)
        self.assertEqual(missing.read_bytes(), b"created")


if __name__ == "__main__":
    unittest.main()
