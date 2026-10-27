#!/usr/bin/env python3
"""Generates objdiff.json with dsd, giving each decomp.me scratch the compiler and flags its source is built with."""

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path
from typing import Any

from compiler_config import (
    DECOMP_ME_COMPILERS,
    ROOT_PATH,
    cc_flags,
    compiler_config_for_source,
)

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--dsd", required=True, help="Path to dsd")
parser.add_argument("--config-path", required=True, help="Path to config.yaml")
parser.add_argument(
    "--output-path",
    required=True,
    action="append",
    type=Path,
    help="Directory to generate objdiff.json in, can be repeated",
)
parser.add_argument(
    "--region-define", required=True, help="Region macro, e.g. REGION_USA"
)
args = parser.parse_args()

out = subprocess.run(
    [
        args.dsd,
        "objdiff",
        "--config-path",
        args.config_path,
        "--output-path",
        args.output_path[0],
        "--stdout",
        "--scratch",  # Metadata for creating decomp.me scratches
        "--custom-make",  # Command for rebuilding files
        "ninja",
    ],
    check=False,
    capture_output=True,
    text=True,
)
if out.returncode != 0:
    sys.exit(f"Error running dsd:\n{out.stderr.strip()}")
objdiff = json.loads(out.stdout)

for unit in objdiff["units"]:
    scratch = unit.get("scratch")
    if scratch is None:
        continue
    # Paths in objdiff.json are relative to the directory it is in
    source = Path(
        os.path.relpath(
            args.output_path[0] / unit["metadata"]["source_path"], ROOT_PATH
        )
    )
    if not (ROOT_PATH / source).is_file():
        sys.exit(f"Source file of objdiff unit '{unit['name']}' not found: {source}")
    version = compiler_config_for_source(source).version
    scratch["compiler"] = DECOMP_ME_COMPILERS[version]
    scratch["c_flags"] = f"{cc_flags(source)} -d {args.region_define}"

# Keys of paths in objdiff.json, which are relative to the directory it is in
PATH_KEYS = {
    "target_dir",
    "base_dir",
    "target_path",
    "base_path",
    "ctx_path",
    "source_path",
}


def rebase_paths(value: Any, from_dir: Path, to_dir: Path) -> Any:
    if isinstance(value, list):
        return [rebase_paths(item, from_dir, to_dir) for item in value]
    if isinstance(value, dict):
        return {
            key: (
                os.path.relpath(from_dir / item, to_dir)
                if key in PATH_KEYS
                else rebase_paths(item, from_dir, to_dir)
            )
            for key, item in value.items()
        }
    return value


for output_path in args.output_path:
    with (output_path / "objdiff.json").open("w") as file:
        json.dump(
            rebase_paths(objdiff, args.output_path[0], output_path), file, indent=2
        )
