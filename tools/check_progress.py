#!/usr/bin/env python3
"""
Check matching progress for decompilation units.

This script analyzes objdiff report.json to show decompilation progress for
any C source file being matched against assembly. Useful for tracking how well
decompiled code matches the original binary.

Usage:
    python check_progress.py src/ov031           # Check specific unit
    python check_progress.py --list              # List all units
    python check_progress.py src/main -r path/to/report.json

The script shows:
    - Fuzzy match %: Overall instruction similarity (not byte-exact)
    - Code/function matching: Exact byte-for-byte matches
    - Section breakdown: Matching stats per section (.text, .data, etc.)
"""
import json
import sys
import argparse
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(
        description='Check decompilation matching progress for a unit',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
Examples:
  %(prog)s src/ov031
  %(prog)s src/main
  %(prog)s src/ov002 --report build/usa/report.json
  %(prog)s --list
        '''
    )
    parser.add_argument('unit', nargs='?', help='Unit name (e.g., src/ov031, src/main)')
    parser.add_argument('-r', '--report', default='build/usa/report.json',
                        help='Path to objdiff report.json (default: build/usa/report.json)')
    parser.add_argument('-l', '--list', action='store_true',
                        help='List all available units in the report')

    args = parser.parse_args()

    # Load report
    report_path = Path(args.report)
    if not report_path.exists():
        print(f"Error: Report file not found: {args.report}")
        print("Run 'ninja' to generate the report first.")
        return 1

    with open(report_path) as f:
        data = json.load(f)

    # List mode
    if args.list:
        print("Available units:")
        for u in sorted(data['units'], key=lambda x: x['name']):
            measures = u.get('measures', {})
            fuzzy = measures.get('fuzzy_match_percent', 0)
            total_funcs = measures.get('total_functions', 0)
            matched_funcs = measures.get('matched_functions', 0)
            print(f"  {u['name']:30s}  {fuzzy:6.2f}%  ({matched_funcs}/{total_funcs} functions)")
        return 0

    # Check for unit argument
    if not args.unit:
        parser.print_help()
        return 1

    unit = next((u for u in data['units'] if u['name'] == args.unit), None)
    if not unit:
        print(f"Error: Unit '{args.unit}' not found in report")
        print(f"Use --list to see available units")
        return 1

    measures = unit['measures']

    print("=" * 70)
    print(f"Decompilation Progress: {args.unit}")
    print("=" * 70)
    print(f"Fuzzy match:       {measures['fuzzy_match_percent']:6.2f}%")
    print(f"Code matched:      {measures['matched_code']:>5s}/{measures['total_code']:<5s} bytes ({measures['matched_code_percent']:.2f}%)")
    print(f"Functions matched: {measures['matched_functions']:>3d}/{measures['total_functions']:<3d} ({measures['matched_functions_percent']:.2f}%)")
    print()

    # Count functions by match status
    perfect = [f for f in unit['functions'] if f.get('match_percent', 0) == 100.0]
    partial = [f for f in unit['functions'] if 0 < f.get('match_percent', 0) < 100.0]
    none = [f for f in unit['functions'] if f.get('match_percent', 0) == 0]

    if perfect:
        print(f"Perfectly matched functions ({len(perfect)}):")
        for func in perfect:
            print(f"  ? {func['name']:40s} {func.get('size', '0'):>5s}b")
        print()

    if partial:
        print(f"Partially matched functions ({len(partial)}):")
        for func in partial:
            match_pct = func.get('match_percent', 0)
            print(f"  ~ {func['name']:40s} {func.get('size', '0'):>5s}b  {match_pct:5.1f}%")
        print()

    print(f"Non-matching functions: {len(none)}/{len(unit['functions'])}")

    # Section breakdown
    print("\nSection breakdown:")
    for section in unit['sections']:
        name = section['name']
        size = section['size']
        fuzzy = section.get('fuzzy_match_percent', 0)
        print(f"  {name:10s} {size:>6s}b  {fuzzy:6.2f}% fuzzy")

    return 0

if __name__ == '__main__':
    sys.exit(main())
