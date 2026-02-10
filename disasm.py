#!/usr/bin/env python3
"""
Disassemble compiled object files and compare with target assembly.

This script extracts and disassembles functions from compiled .o files,
allowing you to compare your compiled output against the target assembly.

Usage:
    python disasm.py src/ov031.o func_ov031_0210a2d8              # Show compiled
    python disasm.py src/ov031.o func_ov031_0210a2d8 --target    # Side-by-side
    python disasm.py src/ov031.o --list                          # List functions
"""

import sys
import argparse
import re
from pathlib import Path
from typing import Optional, Dict, List, Tuple

try:
    from elftools.elf.elffile import ELFFile
    from elftools.elf.sections import SymbolTableSection
    from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB
except ImportError as e:
    print(f"Error: Missing required library: {e}")
    print("Install with: python -m pip install capstone pyelftools")
    sys.exit(1)


def find_function_in_elf(elf_file: ELFFile, func_name: str) -> Optional[Tuple[int, int, bool, int]]:
    """Find function symbol and return (offset, size, is_thumb, section_index)."""
    for section in elf_file.iter_sections():
        if not isinstance(section, SymbolTableSection):
            continue

        for symbol in section.iter_symbols():
            if symbol.name == func_name and symbol['st_info']['type'] == 'STT_FUNC':
                offset = symbol['st_value']
                size = symbol['st_size']
                is_thumb = (offset & 1) == 1  # Thumb functions have LSB set
                offset &= ~1  # Clear LSB for actual offset
                section_index = symbol['st_shndx']

                return (offset, size, is_thumb, section_index)

    return None


def disassemble_function(data: bytes, address: int, size: int, is_thumb: bool) -> List[str]:
    """Disassemble ARM/Thumb code and return formatted lines."""
    mode = CS_MODE_THUMB if is_thumb else CS_MODE_ARM
    cs = Cs(CS_ARCH_ARM, mode)
    cs.detail = True

    lines = []
    for insn in cs.disasm(data[:size], address):
        # Format similar to objdump: address  bytes  mnemonic operands
        bytes_str = ' '.join(f'{b:02x}' for b in insn.bytes)
        line = f"  {insn.address:08x}:  {bytes_str:12s}  {insn.mnemonic:8s} {insn.op_str}"
        lines.append(line)

    return lines


def parse_target_assembly(asm_file: Path, func_name: str) -> Optional[List[str]]:
    """Extract function assembly from target .s file."""
    if not asm_file.exists():
        return None

    lines = []
    in_function = False

    with open(asm_file, 'r') as f:
        for line in f:
            line = line.rstrip()

            # Check for function start
            if func_name in line and ('arm_func_start' in line or f'{func_name}:' in line):
                in_function = True
                continue

            # Check for function end
            if in_function and 'arm_func_end' in line and func_name in line:
                break

            # Collect assembly lines
            if in_function:
                # Skip labels and directives, keep instructions
                stripped = line.strip()
                if stripped and not stripped.startswith('.') and not stripped.endswith(':'):
                    lines.append(line)

    return lines if lines else None


def list_functions(elf_path: Path) -> List[Tuple[str, int, str]]:
    """List all functions in the object file."""
    functions = []

    with open(elf_path, 'rb') as f:
        elf = ELFFile(f)

        for section in elf.iter_sections():
            if not isinstance(section, SymbolTableSection):
                continue

            for symbol in section.iter_symbols():
                if symbol['st_info']['type'] == 'STT_FUNC' and symbol['st_size'] > 0:
                    name = symbol.name
                    size = symbol['st_size']
                    is_thumb = (symbol['st_value'] & 1) == 1
                    mode = 'thumb' if is_thumb else 'arm'
                    functions.append((name, size, mode))

    return sorted(functions, key=lambda x: x[0])


def show_side_by_side(compiled: List[str], target: List[str], func_name: str):
    """Show side-by-side comparison of compiled vs target assembly."""
    print("=" * 80)
    print(f"Function: {func_name}")
    print("=" * 80)
    print(f"{'COMPILED':40s} | {'TARGET':37s}")
    print("-" * 80)

    max_lines = max(len(compiled), len(target))
    for i in range(max_lines):
        left = compiled[i] if i < len(compiled) else ""
        right = target[i] if i < len(target) else ""

        # Truncate if too long
        if len(left) > 39:
            left = left[:36] + "..."
        if len(right) > 37:
            right = right[:34] + "..."

        print(f"{left:40s} | {right}")


def main():
    parser = argparse.ArgumentParser(
        description='Disassemble compiled object files and compare with target',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  %(prog)s build/usa/src/ov031.o func_ov031_0210a2d8
  %(prog)s build/usa/src/ov031.o func_ov031_0210a2d8 -t
  %(prog)s build/usa/src/ov031.o -l
  %(prog)s build/usa/src/main.o MainLoop --target build/usa/asm/main_4.s
        '''
    )

    parser.add_argument('object_file', help='Path to compiled .o file')
    parser.add_argument('function', nargs='?', help='Function name to disassemble')
    parser.add_argument('-l', '--list', action='store_true',
                        help='List all functions in the object file')
    parser.add_argument('-t', '--target', nargs='?', const='auto', metavar='ASM_FILE',
                        help='Show side-by-side with target assembly (auto-detect or specify path)')

    args = parser.parse_args()

    obj_path = Path(args.object_file)
    if not obj_path.exists():
        print(f"Error: Object file not found: {args.object_file}")
        return 1

    # List mode
    if args.list:
        functions = list_functions(obj_path)
        if not functions:
            print("No functions found in object file")
            return 1

        print(f"Functions in {obj_path.name}:")
        print(f"{'Name':50s} {'Size':>6s} {'Mode':>7s}")
        print("-" * 65)
        for name, size, mode in functions:
            print(f"{name:50s} {size:6d}b {mode:>7s}")
        return 0

    # Need function name for disassembly
    if not args.function:
        parser.print_help()
        return 1

    func_name = args.function

    # Disassemble compiled function
    with open(obj_path, 'rb') as f:
        elf = ELFFile(f)
        func_info = find_function_in_elf(elf, func_name)

        if not func_info:
            print(f"Error: Function '{func_name}' not found in {obj_path}")
            print("Use --list to see available functions")
            return 1

        offset, size, is_thumb, section_idx = func_info

        # Get the function's section by index
        if not isinstance(section_idx, int):
            print(f"Error: Invalid section index for function")
            return 1

        func_section = elf.get_section(section_idx)
        section_data = func_section.data()
        func_data = section_data[offset:offset + size]

        compiled_asm = disassemble_function(func_data, offset, size, is_thumb)

    # Show compiled only or side-by-side?
    if not args.target:
        print(f"Function: {func_name} ({size} bytes, {'thumb' if is_thumb else 'arm'} mode)")
        print("-" * 60)
        for line in compiled_asm:
            print(line)
        return 0

    # Side-by-side comparison
    target_path = None
    if args.target == 'auto':
        # Auto-detect: build/usa/src/ov031.o -> build/usa/asm/ov031_4.s
        # Extract unit name (e.g., ov031 from ov031.o)
        match = re.search(r'(ov\d+|main|[a-zA-Z_]+)\.o$', obj_path.name)
        if match:
            unit_name = match.group(1)
            # Try common patterns
            asm_dir = obj_path.parent.parent / 'asm'
            for pattern in [f'{unit_name}_4.s', f'{unit_name}.s', f'{unit_name}_0.s']:
                potential = asm_dir / pattern
                if potential.exists():
                    target_path = potential
                    break
    else:
        target_path = Path(args.target)

    if not target_path or not target_path.exists():
        print(f"Warning: Target assembly file not found")
        if args.target == 'auto':
            print(f"Specify manually with: --target path/to/file.s")
        print("\nShowing compiled assembly only:")
        print("-" * 60)
        for line in compiled_asm:
            print(line)
        return 0

    target_asm = parse_target_assembly(target_path, func_name)
    if not target_asm:
        print(f"Warning: Function {func_name} not found in {target_path}")
        print("\nShowing compiled assembly only:")
        print("-" * 60)
        for line in compiled_asm:
            print(line)
        return 0

    show_side_by_side(compiled_asm, target_asm, func_name)
    return 0


if __name__ == '__main__':
    sys.exit(main())
