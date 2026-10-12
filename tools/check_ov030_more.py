"""Check the additional ov030 split against pinned main/field ownership."""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / 'config/usa/arm9/overlays/ov030/delinks.txt'


def owners(text):
    result = []
    owner = None
    for line in text.splitlines():
        if line and not line[0].isspace() and line.endswith(':'):
            owner = line[:-1]
        m = re.search(r'(\.\w+)\s+start:(0x\w+) end:(0x\w+)', line)
        if owner and m:
            result.append(dict(source=owner, section=m[1], start=int(m[2], 16), end=int(m[3], 16)))
    return result


def overlap(a, b):
    return a['section'] == b['section'] and max(a['start'], b['start']) < min(a['end'], b['end'])


def main():
    manifest = json.loads((ROOT / 'docs/decomp/ov030-more/manifest.json').read_text())
    current = owners(CONFIG.read_text())
    for i, a in enumerate(current):
        assert a['start'] < a['end'], a
        for b in current[:i]:
            assert not overlap(a, b), (a, b)
    for old in manifest['baseline_owners']:
        assert old in current, ('changed main ownership', old)
    seen = set()
    for unit in manifest['units']:
        assert (ROOT / unit['source']).is_file(), unit['source']
        assert [r for r in current if r['source'] == unit['source']] == unit['ranges'], unit['source']
        for r in unit['ranges']:
            for old in manifest['reference_owners']:
                assert not overlap(r, old), ('field branch overlap', r, old)
        text = unit['ranges'][0]
        for f in unit['functions']:
            assert text['start'] <= f['address'] < f['address'] + f['size'] <= text['end'], f
            assert f['address'] not in seen, ('duplicate function', f)
            seen.add(f['address'])
    # Check every declared boundary against the current symbol inventory too.
    functions = []
    for line in (ROOT / 'config/usa/arm9/overlays/ov030/symbols.txt').read_text().splitlines():
        m = re.search(r'kind:function\(\w+,size=(0x\w+)\).*addr:(0x\w+)', line)
        if m:
            functions.append((int(m[2], 16), int(m[1], 16)))
    for unit in manifest['units']:
        r = unit['ranges'][0]
        actual = [(a, s) for a, s in functions if r['start'] <= a < r['end']]
        assert actual == [(f['address'], f['size']) for f in unit['functions']], unit['source']
        assert not any(a < r['start'] < a + s or a < r['end'] < a + s for a, s in functions), unit['source']
    print(f"PASS: {len(manifest['units'])} TUs, {len(seen)} functions; no code/data overlaps with main or field branch")


if __name__ == '__main__':
    main()
