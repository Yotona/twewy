#!/usr/bin/env python3
"""m2cdraft.py -- generate an m2c first draft for a range of overlay functions.

m2c (github.com/matt-kempster/m2c) reads the repo's own dsd disassembly
directly, so there is no need to hand-transcribe.  This wraps the two commands
the repo already expects:

    tools/m2ctx.py <header-or-c> -D REGION_USA -f <ctx>   # preprocess context
    m2c.py -t arm --context <ctx> -f <fn>... ov030_4.s     # draft C

The context is generated once and cached, so subsequent calls are just m2c.

    python tools/m2cdraft.py --overlay ov030 --ctx-from include/Field/Events.h
    python tools/m2cdraft.py --overlay ov030 -f 02082724 -f 0208273c
    python tools/m2cdraft.py --overlay ov030 --range 0x020824a0:0x02082800 > draft.c

Note on range selection: m2c's -f takes a function *name*, not an address, so
`--range` resolves the addresses through the overlay's symbols.txt first.

Temp/work files default under E:/ (repo policy for this task); override with
M2C_WORK. m2c itself defaults to M2C_DIR (the existing opencode checkout).
"""
import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
M2C = Path(os.environ.get("M2C_DIR", r"C:\Users\COMIRAN\AppData\Local\Temp\opencode\m2c"))
DEFAULT_WORK = r"E:\claude-scratch\m2c-work"
WORK_BASE = Path(os.environ.get("M2C_WORK", DEFAULT_WORK))


def build_context(src_rel, work):
    """Run tools/m2ctx.py to preprocess a header into a decomp.me/m2c context."""
    work.mkdir(parents=True, exist_ok=True)
    ctx = work / ("ctx_" + Path(src_rel).stem + ".c")
    if not ctx.exists():
        cmd = [sys.executable, str(ROOT / "tools" / "m2ctx.py"), str(ROOT / src_rel),
               "-f", str(ctx), "-D", "REGION_USA", "-v"]
        p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
        if not ctx.exists():
            sys.stderr.write(p.stdout + p.stderr)
            sys.exit(1)
    return ctx


def load_symbols(syms_path):
    rows = []
    for line in open(syms_path):
        m = re.match(r"(\S+) kind:function\(arm,size=(0x[0-9a-f]+)\) addr:(0x[0-9a-f]+)", line)
        if m:
            rows.append((int(m.group(3), 16), int(m.group(2), 16), m.group(1)))
    rows.sort()
    return rows


def resolve(syms_path, lo, hi):
    out = []
    for addr, size, name in load_symbols(syms_path):
        if lo <= addr < hi:
            out.append(name)
    return out


def extract_slice(asm_path, names, out_path):
    """Extract only the requested function blocks into a temp asm file.

    m2c parses the whole input file, so a single dsd artifact elsewhere
    (e.g. ov030's `b #-0x100` lines) aborts an unrelated draft. Slicing
    keeps the header plus exactly the requested arm_func_start/end blocks.
    Temp file lives under E:/ per repo policy.
    """
    wanted = set(names)
    header = []
    blocks = {}
    cur = None
    cur_lines = []
    with open(asm_path, errors="replace") as f:
        for line in f:
            m = re.search(r"arm_func_start (\S+)", line)
            if m:
                cur = m.group(1)
                cur_lines = [line]
                # include the two preceding lines (.global + start) retroactively?
                continue
            if cur is not None:
                cur_lines.append(line)
                if re.search(r"arm_func_end", line):
                    blocks[cur] = cur_lines
                    cur = None
                    cur_lines = []
            elif cur is None and not blocks:
                header.append(line)
    missing = [n for n in names if n not in blocks]
    if missing:
        sys.stderr.write(f"warning: {len(missing)} function(s) not in asm: {missing[:5]}\n")
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w", errors="replace") as o:
        o.writelines(header)
        for n in names:
            if n in blocks:
                # re-emit .global + start (captured separately in source)
                o.write(f"\n    .global {n}\n")
                o.writelines(blocks[n])
    return out_path


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-f", "--function", action="append", default=[],
                    help="function name or bare hex suffix (e.g. 02082724); repeatable")
    ap.add_argument("--range", help="lo:hi address range, e.g. 0x020824a0:0x02082800")
    ap.add_argument("--overlay", default="ov030",
                    help="overlay id, e.g. ov030 (default) or ov039")
    ap.add_argument("--ctx-from", default=None,
                    help="source file to preprocess into the m2c context (default per overlay)")
    ap.add_argument("--no-context", action="store_true")
    ap.add_argument("--globals", default="used", choices=["all", "used", "none"])
    a = ap.parse_args()

    ov = a.overlay
    ovnum = re.search(r"(\d+)", ov)
    ovsuffix = ovnum.group(1) if ovnum else ov
    asm = ROOT / "build" / "usa" / "asm" / f"ov{ovsuffix}_4.s"
    syms_path = ROOT / "config" / "usa" / "arm9" / "overlays" / f"ov{ovsuffix}" / "symbols.txt"
    work = WORK_BASE / f"ov{ovsuffix}"
    if a.ctx_from is None:
        a.ctx_from = "include/Field/Events.h" if ovsuffix == "030" else "include/Debug/Sugata/TinPinSlammer.h"

    if not asm.exists():
        sys.exit(f"{asm} missing -- run `ninja dis` (about a minute)")

    prefix = f"func_ov{ovsuffix}_"
    names = []
    for f in a.function:
        f = f.strip()
        if re.fullmatch(r"[0-9a-fA-F]{8}", f):
            names.append(prefix + f)
        else:
            names.append(f)
    if a.range:
        lo, hi = (int(x, 0) for x in a.range.split(":"))
        names += resolve(syms_path, lo, hi)
    # de-dupe, keep order
    seen, uniq = set(), []
    for n in names:
        if n not in seen:
            seen.add(n)
            uniq.append(n)

    cmd = [sys.executable, str(M2C / "m2c.py"), "-t", "arm", "--globals", a.globals]
    if not a.no_context:
        cmd += ["--context", str(build_context(a.ctx_from, work))]
    for n in uniq:
        cmd += ["-f", n]
    slice_path = work / ("slice_" + ovsuffix + ".s")
    cmd.append(str(extract_slice(asm, uniq, slice_path)))

    p = subprocess.run(cmd, cwd=M2C, capture_output=True, text=True, encoding="utf-8",
                       errors="replace")
    sys.stdout.write(p.stdout)
    if p.returncode != 0:
        sys.stderr.write(p.stderr)


if __name__ == "__main__":
    main()
