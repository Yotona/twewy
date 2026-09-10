#!/usr/bin/env python3
"""
Decodes Apl_Fuk/mestxt.bin into readable text.

Container format
----------------
  mestable.bin : MessageTableEntry { s32 offset; s32 size; } [n]
  mestxt.bin   : concatenated messages, each an array of SysCodes terminated
                 by SYSFONT_CODE_STR_END (0xFFFF).

A SysCode is a glyph index in the font's own code space, or a control code.
See include/Util/SysFont.h and src/Util/SysFont.c.

Text escapes
------------
  \\n                        SYSFONT_CODE_LINEBREAK
  \\\\                       a literal backslash
  <c1>..<cF>                 set text colour (SYSFONT_CODE_COLOR_FIRST + n)
  <u32> <s32> <str> <chr>    format specifiers, one vararg each
  <fx0>..<fx4>               fixed-point, n fractional digits
  <w1x> <w2x>                single/double width glyph cells
  <msgend>                   SYSFONT_CODE_MSG_END
  {XXXX}                     raw SysCode, for any glyph or control code with no known mapping
"""

import argparse
import collections
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]  # tools/asset/mesdump.py -> repo root
VERSIONS = ("usa", "jp")


def data_dir(version):
    return REPO / "extract" / version / "files/Apl_Fuk"


def out_dir(version):
    return REPO / "assets" / version / "text"


# Terminator for messages
STR_END = 0xFFFF

# Control codes for the message system
CONTROL = {
    0xFFFD: "<msgend>",
    0xFFFE: "\\n",
    0xFFE0: "<w1x>",
    0xFFE1: "<w2x>",
    0xFFD0: "<u32>",
    0xFFD1: "<s32>",
    0xFFD2: "<str>",
    0xFFD3: "<chr>",
}
CONTROL.update({0xFFC0 + n: "<fx%d>" % n for n in range(5)})
CONTROL.update({0xFFB1 + n: "<c%X>" % (n + 1) for n in range(0xF)})

# Inline icon glyphs. These are pictograms with no Unicode equivalent,
# though the devs left a legend embedded in the message table
ICONS = [
    "po",
    "ne",
    "ab",
    "to",
    "sl",
    "dr",
    "ko",
    "ci",
    "mi",
    "re",
    "pu",
    "bt",
    "at",
    "mu",
]
CONTROL.update({0x0226 + n: f"<ic_{name}>" for n, name in enumerate(ICONS)})

# The font's code space (first glyph index, characters in glyph order)
CHARMAP_RUNS = [
    (0, "".join(chr(0x20 + n) for n in range(95))),  # ASCII 0x20..0x7E
    (
        95,
        "あいうえおかがきぎくぐけげこごさざしじすずせぜそぞただちぢつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもやゆよらりるれろわをん",
    ),  # hiragana
    (
        178,
        "ァィゥェォカガキギクグケゲコゴサザシジスズセゼソゾタダチヂツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモヤユヨラリルレロワヲン",
    ),  # katakana
    (
        264,
        "、。，．・：；？！゛゜´｀¨＾～‾＿ヽヾゝゞ〃仝々〆〇ゑゐゔ㐀㐁㐂㐃㐄㐅㐆㐇㐈㐉㐊㐋㐌㐍㐎㐏㐐㐑㐒㐓㐔㐕㐖㐗㐘㐙㐚㐛㐜㐝㐞㐟㐠㐡㐢㐣",
    ),  # JP punctuation and symbols
    (341, "ａｂｃ"),
    (345, "ＡＢ"),
    (350, "「ＡＢ」ａ｡ａ｢ａ｣"),
    (359, "ａｩ｡"),
    (364, "ａ｢ａ｣ａ､｡｢｡｣｡ａ｡ａ｢ａ｣ａ､｡"),
    (390, "ａ｡｡｢‐"),
    (421, "ａ｢ａ｣ａ､｡｢ａ｣ａ､"),
    (427, "".join(chr(0xE000 + n) for n in range(29))),  # DS firmware pictographs
    (456, ""),
    (457, "　"),  # SYSFONT_GLYPH_WIDE_SPACE
    # Past 457 SysFontDsCodeMap simply stops because it only has to cover what a DS
    # owner name can contain. What follows after is fullwidth alphanumerics,
    # e.g. ｱｲｳｴ, ｻﾀ, ﾗﾘﾞ and ﾔﾐﾁﾝ.
    (458, "".join(chr(0xFF10 + n) for n in range(10))),  # ０..９
    (468, "".join(chr(0xFF21 + n) for n in range(26))),  # Ａ..Ｚ
    (494, "".join(chr(0xFF41 + n) for n in range(26))),  # ａ..ｚ
    # The katakana prolonged sound mark, and what SysFont_WriteFx32 emits as the minus
    # sign for a negative value (SYSFONT_GLYPH_MINUS). The sheet's other ー at 283
    # occurs zero times in any ROM version, favoring this one in all instances.
    (520, "ー"),  # ー
]


def load_charmap():
    """{glyph index: unicode char} for the whole known code space."""
    charmap = {}
    for start, chars in CHARMAP_RUNS:
        for n, ch in enumerate(chars):
            charmap[start + n] = ch
    return charmap


# Only the backslash needs escaping, to allow it to represent an escape in text.
_ESCAPED = {"\\": "\\\\"}


def decode(codes, charmap):
    """SysCode[] -> escaped text. Stops at the first STR_END."""
    out = []
    for code in codes:
        if code == STR_END:
            break
        if code in CONTROL:
            out.append(CONTROL[code])
        elif code in charmap:
            ch = charmap[code]
            out.append(_ESCAPED.get(ch, ch))
        else:
            out.append(f"{{{code:04X}}}")
    return "".join(out)


def read_messages(version):
    """Yield (index, codes) for every entry of mestable.bin."""
    d = data_dir(version)
    table = (d / "mestable.bin").read_bytes()
    text = (d / "mestxt.bin").read_bytes()
    for i in range(len(table) // 8):
        offset, size = struct.unpack_from("<ii", table, i * 8)
        yield i, struct.unpack_from("<%dH" % (size // 2), text, offset)


MESTXT_COLUMNS = ("index", "text")
UNKNOWN_COLUMNS = ("glyph", "hex", "count", "first_msg", "first_msg_text")


def write_dump(path, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", encoding="utf-8", newline="\n") as f:
        f.write("\t".join(MESTXT_COLUMNS) + "\n")
        for index, text in rows:
            f.write(f"{index}\t{text}\n")


def cmd_dump(args):
    charmap = load_charmap()
    for version in args.versions:
        rows = []
        unknown = collections.Counter()
        samples = {}
        for index, codes in read_messages(version):
            text = decode(codes, charmap)
            rows.append((index, text))
            for code in codes:
                if code >= 0xFF00:  # control codes, STR_END included
                    continue
                # CONTROL also names the icon glyphs
                if code not in charmap and code not in CONTROL:
                    unknown[code] += 1
                    samples.setdefault(code, (index, text))

        d = out_dir(version)
        write_dump(d / "mestxt.tsv", rows)

        with (d / "unknown_glyphs.tsv").open("w", encoding="utf-8", newline="\n") as f:
            f.write("\t".join(UNKNOWN_COLUMNS) + "\n")
            for code, count in sorted(unknown.items()):
                index, text = samples[code]
                f.write(f"{code}\t0x{code:04X}\t{count}\t{index}\t{text}\n")

        print(f"{d.relative_to(REPO)}/mestxt.tsv: {len(rows)} messages")


def main():
    p = argparse.ArgumentParser(
        description=__doc__.split("\n")[1],
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    sub = p.add_subparsers(required=True)

    sp = sub.add_parser("dump", help="decode mestxt.bin into assets/<version>/text/")
    sp.add_argument(
        "-v",
        "--version",
        dest="versions",
        action="append",
        choices=VERSIONS,
        help="default: both",
    )
    sp.set_defaults(func=cmd_dump)

    args = p.parse_args()
    args.versions = args.versions or list(VERSIONS)
    sys.exit(args.func(args) or 0)


if __name__ == "__main__":
    main()
