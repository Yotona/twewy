#!/usr/bin/env python3
"""Compile several sources with ONE mwcc process, skipping up-to-date ones.

Usage: mwcc_batch.py --outdir DIR --target OBJ --depfile FILE
                     [--extra-dep FILE]... --srcs SRC... -- <mwcc command prefix>

mwcc has a large fixed per-process cost, so Ninja edges built with this wrapper
cover a few sources that share flags and an output directory. The wrapper
re-implements Ninja's staleness test per source (object missing/older than the
source, the compiler or any header listed in the source's own .d file) and runs
`<prefix> -c <stale sources> -o DIR`. mwcc writes one .o and one .d per source,
byte-identical to separate invocations.

Ninja only sees the edge as a whole, so the wrapper keeps it consistent:
  * DEPFILE gets the union of every source's dependencies (target: OBJ), which
    makes the edge dirty when any member is.
  * The Ninja rule must be `restat = 1`: members that were up to date keep their
    old timestamps (so they do not trigger relinks), and restat is what lets
    Ninja record the edge as clean although those objects are older than
    headers that only other members include.
  * A hash of the command line is kept in DEPFILE.sig; if it changes (flags,
    members) everything is recompiled, as Ninja would for separate edges.
If a batch fails, the stale sources are retried one by one so every error is
reported and every compilable object is produced, like separate edges would.
"""

import argparse
import hashlib
import os
import subprocess
import sys


def read_deps(d_file):
    """Dependencies listed in a mwcc .d file, or None if unreadable."""
    try:
        with open(d_file, encoding="utf-8", errors="replace") as f:
            tokens = [t for t in f.read().split() if t != "\\"]
    except OSError:
        return None
    if not tokens or not tokens[0].endswith(":"):
        return None
    return tokens[1:]


def mtime(path):
    try:
        return os.stat(path).st_mtime_ns
    except OSError:
        return None


def newest(paths):
    """Newest mtime among paths, or None if any is missing."""
    best = 0
    for p in paths:
        m = mtime(p)
        if m is None:
            return None
        best = max(best, m)
    return best


def stem(src):
    return os.path.splitext(os.path.basename(src))[0]


def run(cmd):
    return subprocess.run(cmd).returncode


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--outdir", required=True)
    ap.add_argument("--target", required=True, help="object named in the merged depfile")
    ap.add_argument("--depfile", required=True)
    ap.add_argument("--extra-dep", action="append", default=[])
    ap.add_argument("--srcs", nargs="+", required=True)
    ap.add_argument("prefix", nargs=argparse.REMAINDER)
    args = ap.parse_args()
    prefix = args.prefix[1:] if args.prefix[:1] == ["--"] else args.prefix
    if not prefix:
        ap.error("missing compiler command after --")

    outdir = args.outdir
    sig_file = args.depfile + ".sig"
    sig = hashlib.sha1(
        "\0".join([outdir, *args.srcs, "--", *prefix]).encode()
    ).hexdigest()
    try:
        with open(sig_file) as f:
            sig_ok = f.read() == sig
    except OSError:
        sig_ok = False

    extra = newest(args.extra_dep) or 0
    objs = {s: os.path.join(outdir, stem(s) + ".o") for s in args.srcs}
    deps = {}
    stale = []
    for s in args.srcs:
        d = read_deps(os.path.join(outdir, stem(s) + ".d"))
        deps[s] = d
        obj_time = mtime(objs[s])
        if not sig_ok or obj_time is None or d is None:
            stale.append(s)
            continue
        inputs = newest([s, *d])
        if inputs is None or max(inputs, extra) > obj_time:
            stale.append(s)

    if stale:
        os.makedirs(outdir, exist_ok=True)
        if run([*prefix, "-c", *stale, "-o", outdir]) != 0:
            failed = False
            if len(stale) > 1:
                for s in stale:
                    if run([*prefix, "-c", s, "-o", outdir]) != 0:
                        failed = True
                        for ext in (".o", ".d"):
                            try:
                                os.remove(os.path.join(outdir, stem(s) + ext))
                            except OSError:
                                pass
            else:
                failed = True
            if failed:
                return 1
        if os.name != "nt":
            import transform_dep

            for s in stale:
                d = os.path.join(outdir, stem(s) + ".d")
                with open(d, "w", encoding="UTF-8") as f:
                    f.write(transform_dep.import_d_file(d))

    # Merged dependency list for Ninja.
    merged = {}
    for s in args.srcs:
        d = read_deps(os.path.join(outdir, stem(s) + ".d"))
        if d is None:
            sys.stderr.write(f"mwcc_batch: no usable dependency file for {s}\n")
            return 1
        merged.update(dict.fromkeys(d))
    tmp = args.depfile + ".tmp"
    with open(tmp, "w", encoding="UTF-8") as f:
        f.write(args.target + ": " + " ".join(merged) + "\n")
    os.replace(tmp, args.depfile)

    with open(sig_file, "w") as f:
        f.write(sig)
    return 0


if __name__ == "__main__":
    sys.exit(main())
