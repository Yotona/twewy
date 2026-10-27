import argparse
import platform
import re
import subprocess
import sys
from pathlib import Path

from compiler_config import (
    ROOT_PATH,
    cc_flags,
    compiler_config_for_source,
    compiler_executable,
    mwcc_include_args,
    region_define,
    repo_relative,
)

parser = argparse.ArgumentParser(description="Mangles function declarations")
parser.add_argument("file", help="C/C++ source file with defined function to mangle")
parser.add_argument(
    "-v",
    "--version",
    default="usa",
    help="Game version whose region is defined (default: usa)",
)
parser.add_argument(
    "-w",
    dest="wine",
    default="./wibo",
    help="Path to Wine/Wibo relative to the repository root (linux only)",
)
args = parser.parse_args()

# Compile the file the way the build does, so the listing has the same symbols
source = repo_relative(Path(args.file).absolute())
cc = [] if platform.system() == "Windows" else [args.wine]
cc.extend(
    [
        str(compiler_executable(compiler_config_for_source(source).version)),
        *cc_flags(source).split(),
        *mwcc_include_args(),
        "-d",
        region_define(args.version),
        "-nolink",
        "-dis",  # Print the disassembly listing, which names each symbol
        str(source),
    ]
)

try:
    output = subprocess.check_output(cc, cwd=ROOT_PATH)
except subprocess.CalledProcessError as e:
    print(e.stdout.decode())
    sys.exit(1)

output = output.decode()

mangled_funcs: list[str] = re.findall(r".text +([^\$ ]\S+)", output)
init_funcs: list[str] = re.findall(r".init +([^\$ ]\S+)", output)
mangled_data: list[str] = re.findall(r".data +([^\. ]\S+)", output)
mangled_bss: list[str] = re.findall(r".bss +([^\. ]\S+)", output)

if len(mangled_funcs) > 0:
    print(".text:\n")
    for func in mangled_funcs:
        print(func)
    print("\n")
if len(init_funcs) > 0:
    print(".init:\n")
    for func in init_funcs:
        print(func)
    print("\n")
if len(mangled_data) > 0:
    print(".data:\n")
    for data in mangled_data:
        print(data)
    print("\n")
if len(mangled_bss) > 0:
    print(".bss:\n")
    for bss in mangled_bss:
        print(bss)
    print("\n")
