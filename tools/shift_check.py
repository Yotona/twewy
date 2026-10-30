#!/usr/bin/env python3
"""Shiftability regression check.

`lcf` copies the arm9 LCF with distinct padding inserted at the start of every
module so a relink moves every module by a different amount.

`check` then compares the matching link against that shifted link. Every word in a
module whose value points into a module must have moved by that module's shift.
Words that keep their old value are hardcoded addresses (or missing relocs) and
break shifted builds. Some words the base ROM contains merely happen to resemble
addresses (instruction encodings, packed data, etc.) that are listed in an allowlist
keyed by module and address.
"""

import argparse
import re
import struct
import sys
from bisect import bisect_right
from pathlib import Path

SECTION_RE = re.compile(r"^\s*\.(\w+)\s*:\s*\{")
SECTION_END_RE = re.compile(r"^\s*\}\s*>\s*(\w+)")
MEMORY_RE = re.compile(r"^\s*(\w+)\s*:\s*ORIGIN\s*=.*>\s*(\S+)")
LABEL_RE = re.compile(r"^\s*(\w+)\s*=\s*\.\s*;")
OBJECT_RE = re.compile(r"^\s*\S+\.o\(")

MAIN_PAD = 0x200
ITCM_PAD = 0x20
DTCM_PAD = 0x40
OVERLAY_PAD = 0x60  # plus 0x20 per overlay, so no two overlays move alike


def module_pad(memory: str) -> int:
    if memory == "ARM9":
        return MAIN_PAD
    if memory == "ITCM":
        return ITCM_PAD
    if memory == "DTCM":
        return DTCM_PAD
    return OVERLAY_PAD + 0x20 * int(memory.removeprefix("OV"))


def padding(size: int, indent: str) -> list[str]:
    return [f"{indent}WRITEW(0);\n"] * (size // 4)


def make_lcf(src: Path, dst: Path):
    lines = src.read_text().splitlines(keepends=True)

    # Find each output section's memory region first; it comes after the body
    memories: dict[int, str] = {}
    start = None
    for i, line in enumerate(lines):
        if SECTION_RE.match(line):
            start = i
        elif start is not None and (m := SECTION_END_RE.match(line)):
            memories[start] = m.group(1)
            start = None

    out: list[str] = []
    pending: str | None = None
    for i, line in enumerate(lines):
        if i in memories:
            pending = memories[i]
        out.append(line)
        if pending is None:
            continue
        indent = line[: len(line) - len(line.lstrip())]
        if pending == "ARM9":
            # The secure area and crt0 are the first object; keep them at the load address
            if OBJECT_RE.match(line):
                out += padding(MAIN_PAD, indent)
                pending = None
        elif line.strip() == ". = ALIGN(32);":
            out += padding(module_pad(pending), indent)
            pending = None
    if pending is not None:
        sys.exit(f"{src}: could not place padding in section for {pending}")

    dst.parent.mkdir(parents=True, exist_ok=True)
    dst.write_text("".join(out))


class Elf:
    def __init__(self, path: Path):
        data = path.read_bytes()
        shoff, = struct.unpack_from("<I", data, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
        sections = [
            struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize)
            for i in range(shnum)
        ]
        shstr = sections[shstrndx]

        def cstr(table, offset: int) -> str:
            begin = table[4] + offset
            return data[begin : data.index(b"\0", begin)].decode()

        self.section_names = [cstr(shstr, s[0]) for s in sections]
        self.section_addrs = [s[3] for s in sections]

        symtab = sections[self.section_names.index(".symtab")]
        strtab = sections[symtab[6]]
        self.symbols: list[tuple[str, int, int]] = []
        for offset in range(symtab[4], symtab[4] + symtab[5], 16):
            name, value, _size, info, _other, shndx = struct.unpack_from(
                "<IIIBBH", data, offset
            )
            if name == 0 or info & 0xF == 3 or shndx == 0 or shndx >= 0xFF00:
                continue
            sym = cstr(strtab, name)
            if sym.startswith("$"):
                continue
            self.symbols.append((sym, value, shndx))

    def unique_symbols(self) -> dict[str, tuple[int, str]]:
        seen: dict[str, tuple[int, str] | None] = {}
        for name, value, shndx in self.symbols:
            seen[name] = None if name in seen else (value, self.section_names[shndx])
        return {name: entry for name, entry in seen.items() if entry is not None}


class Module:
    def __init__(self, memory: str, bin_name: str, end_label: str):
        self.memory = memory
        self.bin_name = bin_name
        self.end_label = end_label
        self.start = 0
        self.end = 0
        self.new_start = 0
        self.addrs: list[int] = []
        """Old addresses of symbols, sorted"""
        self.deltas: list[int] = []
        """How far the symbol at the same index moved"""
        self.names: list[str] = []

    @property
    def name(self) -> str:
        return {"ARM9": "main", "ITCM": "itcm", "DTCM": "dtcm"}.get(
            self.memory, self.memory.lower()
        )

    def index(self, addr: int) -> int:
        return max(bisect_right(self.addrs, addr) - 1, 0)

    def delta(self, addr: int) -> int:
        return self.deltas[self.index(addr)]

    def target_deltas(self, value: int) -> set[int]:
        """Moves that are valid for a pointer to `value`, which may also point one past the previous symbol"""
        i = self.index(value)
        deltas = {self.deltas[i]}
        if i > 0 and self.addrs[i] == value:
            deltas.add(self.deltas[i - 1])
        return deltas

    def describe(self, addr: int) -> str:
        i = self.index(addr)
        return f"{self.names[i]}+0x{addr - self.addrs[i]:x}"


def parse_modules(lcf: Path) -> list[Module]:
    bins: dict[str, str] = {}
    modules: list[Module] = []
    last_label = None
    for line in lcf.read_text().splitlines():
        if m := MEMORY_RE.match(line):
            bins[m.group(1)] = Path(m.group(2)).name
        elif SECTION_RE.match(line):
            last_label = None
        elif m := LABEL_RE.match(line):
            last_label = m.group(1)
        elif (m := SECTION_END_RE.match(line)) and last_label is not None:
            modules.append(Module(m.group(1), bins[m.group(1)], last_label))
    return modules


def load_modules(lcf: Path, orig: Elf, shifted: Elf) -> list[Module]:
    modules = parse_modules(lcf)
    by_memory = {m.memory: m for m in modules}
    old_syms = orig.unique_symbols()
    new_syms = shifted.unique_symbols()
    pairs: dict[str, list[tuple[int, int, str]]] = {m.memory: [] for m in modules}
    for name, (value, section) in old_syms.items():
        if section in pairs and name in new_syms:
            pairs[section].append((value, new_syms[name][0] - value, name))

    for module in modules:
        index = orig.section_names.index(module.memory)
        module.start = orig.section_addrs[index]
        module.new_start = shifted.section_addrs[index]
        module.end = old_syms[module.end_label][0]
        pairs[module.memory].sort()
        module.addrs = [p[0] for p in pairs[module.memory]]
        module.deltas = [p[1] for p in pairs[module.memory]]
        module.names = [p[2] for p in pairs[module.memory]]
        if not module.addrs:
            sys.exit(f"No symbols found for {module.name}")
    assert by_memory["ARM9"].delta(by_memory["ARM9"].end) == MAIN_PAD
    return modules


def read_allowlist(path: Path) -> set[tuple[str, int]]:
    entries: set[tuple[str, int]] = set()
    if path.is_file():
        for line in path.read_text().splitlines():
            line = line.split("#", 1)[0].split()
            if line:
                entries.add((line[0], int(line[1], 16)))
    return entries


def check(args):
    orig = Elf(args.elf)
    shifted = Elf(args.shifted_elf)
    modules = load_modules(args.lcf, orig, shifted)
    lo = min(m.start for m in modules)
    hi = max(m.end for m in modules)

    findings: list[tuple[str, int, str]] = []
    relocated = 0
    for module in modules:
        old = (args.elf.parent / "build" / module.bin_name).read_bytes()
        new = (args.shifted_elf.parent / "build" / module.bin_name).read_bytes()
        for offset in range(0, len(old) - 3, 4):
            value, = struct.unpack_from("<I", old, offset)
            if not lo <= value <= hi:
                continue
            targets = [t for t in modules if t.start <= value <= t.end]
            if not targets:
                continue
            addr = module.start + offset
            new_offset = addr + module.delta(addr) - module.new_start
            new_value, = struct.unpack_from("<I", new, new_offset)
            moved = (new_value - value) & 0xFFFFFFFF
            expected = set().union(*(t.target_deltas(value) for t in targets))
            if moved in expected:
                relocated += 1
                continue
            kind = "unrelocated" if moved == 0 else f"moved by 0x{moved:x}"
            names = "/".join(t.name for t in targets)
            findings.append(
                (module.name, addr, f"{module.describe(addr)}: 0x{value:08x} ({names}) {kind}")
            )

    allowlist = read_allowlist(args.allowlist)
    if args.update:
        args.allowlist.write_text(
            "# Words in the base ROM that look like pointers but are not; see tools/shift_check.py\n"
            + "".join(f"{m} 0x{a:08x}  # {text}\n" for m, a, text in findings)
        )
        print(f"Wrote {len(findings)} entries to {args.allowlist}")
        return

    new_findings = [f for f in findings if (f[0], f[1]) not in allowlist]
    stale = allowlist - {(f[0], f[1]) for f in findings}
    print(
        f"{args.elf}: {relocated} pointers moved with their targets, "
        f"{len(findings) - len(new_findings)} allowlisted, {len(new_findings)} new"
    )
    if stale:
        print(f"note: {len(stale)} allowlist entries no longer apply; rerun with --update to prune")
    if new_findings:
        print("Pointer-like words that did not move with their target in a shifted link:")
        for m, a, text in new_findings:
            print(f"  {m} 0x{a:08x}  {text}")
        print(f"If one is not a pointer, add it to {args.allowlist}")
        sys.exit(1)
    if args.stamp:
        args.stamp.touch()


def main():
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = parser.add_subparsers(dest="command", required=True)

    lcf = sub.add_parser("lcf", help="Write the padded LCF")
    lcf.add_argument("src", type=Path)
    lcf.add_argument("dst", type=Path)

    chk = sub.add_parser("check", help="Compare a matching link against the shifted link")
    chk.add_argument("--lcf", type=Path, required=True, help="Matching arm9 LCF")
    chk.add_argument("--elf", type=Path, required=True, help="Matching arm9.o")
    chk.add_argument("--shifted-elf", type=Path, required=True, help="Shifted arm9.o")
    chk.add_argument("--allowlist", type=Path, required=True)
    chk.add_argument("--stamp", type=Path, help="File to touch on success")
    chk.add_argument("--update", action="store_true", help="Rewrite the allowlist")

    args = parser.parse_args()
    if args.command == "lcf":
        make_lcf(args.src, args.dst)
    else:
        check(args)


if __name__ == "__main__":
    main()
