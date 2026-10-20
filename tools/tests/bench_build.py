#!/usr/bin/env python3
"""Time default `ninja` for clean / wide-header / single-file rebuilds.

Usage: bench_build.py BATCH_SIZE...   (run from the repo root, after one build)

Reconfigures with each --batch-size, deletes the compiled .o/.d files (never
`ninja -t clean`, which would delete the downloaded compiler), then times each
scenario. Prints seconds and the last ninja step counter.
"""

import re
import subprocess
import sys
import time
from pathlib import Path

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


def main():
    for size in sys.argv[1:]:
        subprocess.run([sys.executable, "tools/configure.py", "--batch-size", size], check=True, capture_output=True)
        ninja()
        for sub in ("src", "libs"):
            for p in Path("build/usa", sub).rglob("*"):
                if p.suffix in (".o", ".d"):
                    p.unlink()
        for label, touch in [("clean", None), *SCENARIOS]:
            if touch:
                Path(touch).touch()
            rc, sec, steps = ninja()
            _, _, again = ninja()  # must settle back to the always-run steps (3)
            print(f"batch={size:>2} {label:<26} {sec:6.2f}s steps={steps} rc={rc} rerun-steps={again}", flush=True)


if __name__ == "__main__":
    main()
