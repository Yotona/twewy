#!/usr/bin/env python3
"""Check that objects built through mwcc_batch.py equal one-process-per-file output.

Usage: verify_mwcc_batch.py [-j N]    (repo root, after a full `ninja objects`)

Reads `ninja -t commands objects` (batched or plain mwcc edges), recompiles every
source on its own with the same flags into a scratch directory and compares each
.o byte-for-byte, and each .d after rewriting the output directory, with what is
in build/. Also reports how many (flags, output dir) groups were covered.
"""

import argparse
import re
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

PLAIN = re.compile(r'^(?P<prefix>"[^"]+mwccarm\.exe" .*) -c (?P<src>\S+) -o (?P<out>\S+)$')
BATCH = re.compile(r"--outdir (?P<out>\S+) .*? --srcs (?P<srcs>.*?) -- (?P<prefix>\"[^\"]+mwccarm\.exe\" .*)$")


def jobs():
    lines = subprocess.run(
        ["ninja", "-t", "commands", "objects"], capture_output=True, text=True, check=True
    ).stdout.splitlines()
    for line in lines:
        if m := BATCH.search(line):
            for src in m["srcs"].split():
                yield m["prefix"], src, m["out"]
        elif m := PLAIN.match(line):
            yield m["prefix"], m["src"], m["out"]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-j", type=int, default=8)
    ap.add_argument("--scratch", default="build/_batchverify")
    args = ap.parse_args()
    scratch = Path(args.scratch)
    shutil.rmtree(scratch, ignore_errors=True)
    all_jobs = list(jobs())

    def check(item):
        n, (prefix, src, out) = item
        tmp = scratch / str(n)
        tmp.mkdir(parents=True)
        r = subprocess.run(f"{prefix} -c {src} -o {tmp}", shell=True, capture_output=True, text=True)
        if r.returncode:
            return [f"FAIL compile {src}: {r.stdout[-300:]}{r.stderr[-300:]}"]
        stem = Path(src.replace("\\", "/")).stem
        problems = []
        if (Path(out) / f"{stem}.o").read_bytes() != (tmp / f"{stem}.o").read_bytes():
            problems.append(f"DIFF object {src}")
        built_d = (Path(out) / f"{stem}.d").read_text().replace(out, "@")
        fresh_d = (tmp / f"{stem}.d").read_text().replace(str(tmp), "@")
        if built_d != fresh_d:
            problems.append(f"DIFF dependency file {src}")
        shutil.rmtree(tmp)
        return problems

    with ThreadPoolExecutor(args.j) as pool:
        problems = [p for result in pool.map(check, enumerate(all_jobs)) for p in result]
    print("\n".join(problems))
    groups = {(prefix, out) for prefix, _, out in all_jobs}
    print(f"sources: {len(all_jobs)} groups: {len(groups)} problems: {len(problems)}")
    shutil.rmtree(scratch, ignore_errors=True)
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
