#!/usr/bin/env python3
"""Time default `ninja` for clean / wide-header / single-file rebuilds.

Usage: [REPS=3] bench_build.py (ref|BATCH_SIZE)...   (repo root, after one build)

Sizes are interleaved per rep (A/B); `ref` is REF_BRANCH's unbatched configure.py.

Reconfigures with each --batch-size, deletes the compiled .o/.d files (never
`ninja -t clean`, which would delete the downloaded compiler), then times each
scenario. Prints seconds and the last ninja step counter.
"""

import os
import re
import statistics
import subprocess
import sys
import time
from pathlib import Path

REF_BRANCH = "perf/fast-objdiff"

SCENARIOS = [
    ("noop", None),
    ("Memory.h", "include/Engine/Core/Memory.h"),
    ("OtosuGame.h", "include/Otosu/OtosuGame.h"),
    ("one .c (in 63-file dir)", "src/Field/Tasks/FldChara.c"),
    ("one .c (alone in dir)", "src/Debug/Abe/Mini108.c"),
]


def ninja():
    start = time.perf_counter()
    r = subprocess.run(["ninja"], capture_output=True, text=True)
    elapsed = time.perf_counter() - start
    steps = re.findall(r"^\[(\d+)/\d+\]", r.stdout, re.M)
    return r.returncode, elapsed, steps[-1] if steps else "0"


def configure(size):
    """`ref` configures with REF_BRANCH's configure.py (the unbatched baseline)."""
    if size == "ref":
        script = Path("tools/configure_ref.py")
        script.write_bytes(subprocess.run(["git", "show", f"{REF_BRANCH}:tools/configure.py"], capture_output=True, check=True).stdout)
        cmd = [sys.executable, str(script)]
    else:
        cmd = [sys.executable, "tools/configure.py", "--batch-size", size]
    subprocess.run(cmd, check=True, capture_output=True)


def run_config(size):
    configure(size)
    ninja()
    for sub in ("src", "libs"):
        for p in Path("build/usa", sub).rglob("*"):
            if p.suffix in (".o", ".d") or p.name.startswith(".batch"):
                p.unlink()
    results = {}
    for label, touch in [("clean", None), *SCENARIOS]:
        if touch:
            Path(touch).touch()
        rc, sec, steps = ninja()
        _, _, again = ninja()  # must settle back to the always-run steps (3)
        assert rc == 0 and again == "3", (size, label, rc, again)
        results[label] = sec
    return results


def main():
    reps = int(os.environ.get("REPS", "3"))
    sizes = sys.argv[1:]
    all_results = {s: [] for s in sizes}
    for rep in range(reps):
        for size in sizes:  # interleaved A/B
            r = run_config(size)
            all_results[size].append(r)
            print(f"rep{rep} {size:>3} " + " ".join(f"{k}={v:.2f}" for k, v in r.items()), flush=True)
    Path("tools/configure_ref.py").unlink(missing_ok=True)
    for size, rs in all_results.items():
        print(f"MEDIAN {size:>3} " + " ".join(f"{k}={statistics.median(r[k] for r in rs):.2f}" for k in rs[0]))


if __name__ == "__main__":
    main()
