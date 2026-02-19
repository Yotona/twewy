#!/usr/bin/env python3
import json

with open('build/usa/report.json') as f:
    data = json.load(f)

unit = next((u for u in data['units'] if u['name'] == 'src/main_36'), None)
if not unit:
    print("Unit not found")
    exit(1)

print(f"Total functions: {len(unit['functions'])}")
print(f"\nFunctions match status:")
print("-" * 70)

for i, f in enumerate(unit['functions']):
    match_pct = f.get('match_percent', 0)
    size = f.get('size', 0)
    name = f['name']
    status = "?" if match_pct == 100 else "~" if match_pct > 0 else "X"
    print(f"{status} {name:40s} {size:>5}b  {match_pct:6.2f}%")
    if i >= 50:  # Show first 50 functions
        print(f"\n... and {len(unit['functions']) - 50} more")
        break
