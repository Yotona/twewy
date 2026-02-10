#!/usr/bin/env python3
"""
Analyze overlay assembly files to extract and display string data.
This helps identify the purpose of each overlay based on asset references.
"""

import re
import sys
from pathlib import Path
from collections import defaultdict

def extract_strings_from_bytes(byte_line):
    """Convert a .byte line with hex values to ASCII string if possible."""
    # Extract hex values from the line
    hex_pattern = r'0x([0-9a-fA-F]{2})'
    hex_values = re.findall(hex_pattern, byte_line)

    if not hex_values:
        return None

    # Convert to bytes
    try:
        bytes_data = bytes(int(h, 16) for h in hex_values)
        # Try to decode as ASCII, filter out non-printable
        text = ''.join(chr(b) if 32 <= b < 127 else '.' for b in bytes_data)
        # Only return if it looks like a meaningful string (at least 4 readable chars in a row)
        if re.search(r'[A-Za-z0-9_/]{4,}', text):
            return text.rstrip('\x00.')
    except:
        pass

    return None

def analyze_overlay(asm_file):
    """Extract meaningful strings from an overlay assembly file."""
    strings = []
    in_data_section = False
    current_bytes = []

    with open(asm_file, 'r', encoding='utf-8', errors='ignore') as f:
        for line in f:
            line = line.strip()

            # Check if we're in a data section
            if line.startswith('.data') or line.startswith('data_'):
                in_data_section = True
            elif line.startswith('.text') or line.startswith('.bss'):
                in_data_section = False

            # Look for .byte lines with potential string data
            if '.byte' in line and '0x' in line:
                # Try to extract strings from byte sequences
                string = extract_strings_from_bytes(line)
                if string and len(string) >= 4:  # At least 4 chars
                    strings.append(string)

    return strings

def categorize_string(s):
    """Categorize a string based on its content."""
    s_lower = s.lower()

    # File extensions and paths
    if any(ext in s_lower for ext in ['.pac', '.bin', '.dat', '.spr', '.anm', '.model']):
        return 'asset_file'
    if '/' in s or '\\' in s:
        return 'path'

    # UI/Menu related
    if any(word in s_lower for word in ['menu', 'button', 'icon', 'window', 'cursor', 'select']):
        return 'ui'

    # Battle related
    if any(word in s_lower for word in ['battle', 'btl', 'enemy', 'player', 'attack', 'damage', 'noise']):
        return 'battle'

    # Task/System
    if 'task' in s_lower or 'tsk_' in s_lower or 'seq_' in s_lower:
        return 'system'

    # Debug
    if 'debug' in s_lower or 'test' in s_lower:
        return 'debug'

    # Character/animation related
    if any(word in s_lower for word in ['anim', 'sprite', 'model', 'chr', 'chara']):
        return 'graphics'

    # Sound
    if any(word in s_lower for word in ['sound', 'snd', 'music', 'bgm', 'se']):
        return 'audio'

    # Game-specific
    if any(word in s_lower for word in ['shibuya', 'neku', 'pin', 'brand', 'shop']):
        return 'game_content'

    return 'other'

def main():
    build_asm_path = Path('build/usa/asm')

    if not build_asm_path.exists():
        print(f"Error: {build_asm_path} not found")
        sys.exit(1)

    # Get all overlay assembly files
    overlay_files = sorted(build_asm_path.glob('ov*.s'))

    results = {}

    for asm_file in overlay_files:
        overlay_name = asm_file.stem
        overlay_id = overlay_name.split('_')[0]  # e.g., 'ov002' from 'ov002_4'

        strings = analyze_overlay(asm_file)

        # Categorize strings
        categorized = defaultdict(list)
        for s in strings:
            cat = categorize_string(s)
            if s not in categorized[cat]:  # Avoid duplicates
                categorized[cat].append(s)

        results[overlay_id] = {
            'file': asm_file.name,
            'total_strings': len(strings),
            'categories': dict(categorized),
            'unique_strings': list(set(strings))[:20]  # Show first 20 unique strings
        }

    # Print results
    print("=" * 80)
    print("OVERLAY STRING ANALYSIS")
    print("=" * 80)
    print()

    for overlay_id in sorted(results.keys()):
        data = results[overlay_id]
        if data['total_strings'] == 0:
            continue

        print(f"\n{'='*80}")
        print(f"? {overlay_id.upper()} ({data['file']})")
        print(f"{'='*80}")
        print(f"Total strings found: {data['total_strings']}")
        print()

        # Show categories
        if data['categories']:
            print("Categories:")
            for cat, items in sorted(data['categories'].items()):
                print(f"  ? {cat}: {len(items)} strings")
                # Show a few examples
                for item in items[:3]:
                    print(f"    - {item}")
                if len(items) > 3:
                    print(f"    ... and {len(items) - 3} more")

        print()

if __name__ == '__main__':
    main()
