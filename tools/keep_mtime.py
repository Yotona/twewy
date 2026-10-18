#!/usr/bin/env python3
"""Run a command and preserve timestamps of byte-identical outputs.

Usage: keep_mtime.py <file-or-directory>... -- <command> [args...]

Used with Ninja's restat rules for tools that rewrite all their outputs even
when only one changed. Failed commands leave their output timestamps intact.
"""

import hashlib
import os
import subprocess
import sys
from pathlib import Path


def output_files(paths):
    for path in paths:
        path = Path(path)
        if path.is_dir():
            yield from (p for p in path.rglob("*") if p.is_file())
        elif path.is_file():
            yield path


def digest(path):
    with path.open("rb") as file:
        return hashlib.file_digest(file, "blake2b").digest()


def main():
    if "--" not in sys.argv or sys.argv.index("--") < 2:
        sys.exit(__doc__)
    separator = sys.argv.index("--")
    paths, command = sys.argv[1:separator], sys.argv[separator + 1:]
    if not command:
        sys.exit(__doc__)

    before = {}
    for path in output_files(paths):
        stat = path.stat()
        before[path] = (stat, digest(path))

    result = subprocess.run(command)
    if result.returncode:
        sys.exit(result.returncode)

    for path, (old_stat, old_digest) in before.items():
        try:
            stat = path.stat()
        except FileNotFoundError:
            continue
        if (
            stat.st_mtime_ns != old_stat.st_mtime_ns
            and stat.st_size == old_stat.st_size
            and digest(path) == old_digest
        ):
            os.utime(path, ns=(old_stat.st_atime_ns, old_stat.st_mtime_ns))


if __name__ == "__main__":
    main()
