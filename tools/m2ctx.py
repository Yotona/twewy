#!/usr/bin/env python3

import argparse
import os
import re
import subprocess
import sys
import tempfile

import pyperclip
from compiler_config import ROOT_PATH, gcc_include_args

parser = argparse.ArgumentParser(description="Generates a context for decomp.me")
parser.add_argument("file", help="Input file to preprocess")
parser.add_argument(
    "-f", type=str, dest="out_file", required=False, help="Output context file"
)
parser.add_argument(
    "-c",
    action=argparse.BooleanOptionalAction,
    dest="clipboard",
    required=False,
    help="Copy output to clipboard",
)
parser.add_argument(
    "-e",
    type=str,
    dest="encoding",
    required=False,
    default="shift-jis",
    help="Input file encoding",
)
parser.add_argument(
    "-v",
    action=argparse.BooleanOptionalAction,
    dest="verbose",
    required=False,
    help="Verbose error output",
)
parser.add_argument(
    "-D",
    action="append",
    dest="defines",
    default=[],
    metavar="MACRO",
    help="Define a macro for the preprocessor (e.g. -D REGION_USA)",
)
parser.add_argument(
    "--depfile",
    type=str,
    required=False,
    help="Write the headers the output context file depends on (requires -f)",
)
args = parser.parse_args()

if args.depfile and not args.out_file:
    parser.error("--depfile requires -f")


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


# Finds all lines starting with #include followed by <...> or "..."
INCLUDE_REGEX = r'^\s*#\s*include\s*([<"][\S ]+[>"])\s*$'
# Finds all line comments and multiline comments
COMMENT_REGEX = r"\/\/.*$|\/\*(?:.|\r|\n)+?\*\/"

with open(args.file, "r", encoding=args.encoding) as f:
    contents = f.read()
contents = re.sub(COMMENT_REGEX, "", contents, count=0, flags=re.MULTILINE)
includes = re.findall(INCLUDE_REGEX, contents, flags=re.MULTILINE)

_, suffix = os.path.splitext(args.file)

try:
    with tempfile.NamedTemporaryFile(delete=False, suffix=suffix) as tmp_file:
        # Write includes
        for include in includes:
            tmp_file.write(f"#include {include}\n".encode())
        tmp_file.flush()

        # Ensure the file is written to disk
        os.fsync(tmp_file.fileno())

        # Close the file so GCC can access it
        tmp_file.close()

        depfile_flags = []
        if args.depfile:
            depfile_flags = [
                "-MD",
                "-MF",
                os.path.abspath(args.depfile),
                "-MT",
                args.out_file,
            ]

        # Run preprocessor
        try:
            ctx: str = subprocess.check_output(
                [
                    "gcc",
                    "-E",
                    "-P",
                    "-fworking-directory",
                    "-undef",
                    "-dD",
                    *gcc_include_args(),
                    *[f"-D{d}" for d in args.defines],
                    *depfile_flags,
                    tmp_file.name,
                ],
                cwd=ROOT_PATH,
                encoding=args.encoding,
            )
        except subprocess.CalledProcessError as e:
            eprint(f"Failed to preprocess '{args.file}'")
            if args.verbose:
                eprint(e)
            else:
                eprint("Use -v for more verbose error output")
            sys.exit(1)

finally:
    # Clean up the temporary file
    try:
        if "tmp_file" in locals():
            os.unlink(tmp_file.name)
    except OSError as e:
        eprint(f"Failed to clean up temporary file '{tmp_file.name}': {e}")

# The preprocessed file is the deleted temporary file, so depend on the real source instead
if args.depfile:
    with open(args.depfile, "r") as file:
        deps = file.read()
    with open(args.depfile, "w") as file:
        file.write(deps.replace(tmp_file.name, args.file))

lines = ctx.splitlines(True)
for i in reversed(range(len(lines))):
    if (
        lines[i].startswith("#define __cplusplus")
        or lines[i].startswith("#define __STDC_HOSTED__")
        or lines[i].startswith("#define __STDC__")
        or lines[i].startswith("#define __STDC_VERSION__")
    ):
        lines.pop(i)

ctx = "".join(lines)

if args.out_file:
    try:
        with open(args.out_file, "w", encoding="utf-8") as file:
            file.write(ctx)
    except OSError as e:
        eprint(f"Failed to write file '{args.out_file}'")
        if args.verbose:
            eprint(e)
        else:
            eprint("Use -v for more verbose error output")
        sys.exit(1)
if args.clipboard:
    pyperclip.copy(ctx)
    eprint("Copied context to clipboard")
if args.out_file is None and not args.clipboard:
    print(ctx)
