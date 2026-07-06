#!/usr/bin/env python3

import argparse
import re
import subprocess
import stat
import sys

from pathlib import Path


# Repository root (this script lives in <root>/tools/).
REPO_ROOT = Path(__file__).resolve().parent.parent

# Default compiler and flags, mirroring tools/configure.py (DEFAULT_COMPILER_CONFIG).
COMPILER = "./tools/mwccarm/2.0/sp1p5/mwccarm.exe"
CC_INCLUDES = (
    "-i include -i libs/include -i libs/cpp/include "
    "-i libs/c/include -i libs/nitro/include -i libs/runtime/include"
)
CC_FLAGS = (
    "-O4,p -enum int -char signed -proc arm946e -gccext,on -fp soft "
    "-inline noauto -RTTI off -interworking -w off -sym on -gccinc -nolink "
    "-msgstyle gcc -enc SJIS -ipa file -str noreuse -Cpp_exceptions off"
)


def extract_function_source(src_path: Path, func: str) -> str | None:
    """Return the verbatim definition of `func` from `src_path`, or None.

    Definitions start at column 0 (return type / storage class); the function
    body is captured by brace matching. Forward declarations (no body, i.e. a
    ';' before the first '{') are skipped so we find the real implementation.
    """
    # Source files are Shift-JIS encoded (matching the '-enc SJIS' compile flag).
    lines = src_path.read_text(encoding="shift-jis").splitlines(keepends=True)
    name_re = re.compile(rf"(?<![A-Za-z0-9_]){re.escape(func)}\s*\(")

    for i, line in enumerate(lines):
        # A definition begins at column 0 and is not a comment.
        if not line or line[0].isspace() or line.lstrip().startswith(("//", "/*", "*")):
            continue
        if not name_re.search(line):
            continue

        depth = 0
        seen_open = False
        for j in range(i, len(lines)):
            abandon = False
            for ch in lines[j]:
                if ch == "{":
                    depth += 1
                    seen_open = True
                elif ch == "}":
                    depth -= 1
                    if seen_open and depth == 0:
                        return "".join(lines[i:j + 1])
                elif ch == ";" and not seen_open:
                    abandon = True  # forward declaration, not a definition
                    break
            if abandon:
                break

    return None


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Generates build.ninja")
    parser.add_argument("function", help="Function name (as C)")
    parser.add_argument("--version", "-v", help='Game version', required=False, default="usa", type=str.lower)
    parser.add_argument("--skip", "-s", help='Skip Parsing', required=False, action="store_true")
    args = parser.parse_args()

    version: str = args.version
    func: str = args.function

    build_f = Path("build").resolve() / version / "asm"
    assert build_f.exists(), "not a real path!"

    dir = Path("nonmatchings").resolve()
    if not dir.exists():
        dir.mkdir()

    out_dir = dir / func
    if not out_dir.exists():
        out_dir.mkdir(exist_ok=True)

    target_s = out_dir / "target.s"

    if not args.skip:
        base_c = out_dir / "base.c"

        done = False
        owning_src: Path | None = None
        for dirpath, dirnames, filenames in build_f.walk():
            for filename in filenames:
                cur_path = dirpath / filename
                assert cur_path.exists(), "not a real path!"

                if not str(cur_path).endswith(".s"):
                    print("ignoring", cur_path)
                    continue

                filedata = cur_path.read_text()

                if f"_func_start {func}" not in filedata:
                    continue

                print("found in", cur_path)
                match = re.search(fr"_func_start\s{func}\n(.*)(arm|thumb)_func_end\s{func}", filedata, re.DOTALL)
                assert match is not None, "func not found!"

                func_data = match.group(1).split("\n")
                mode = f".{'thumb' if match.group(2) == "thumb" else 'arm'}"
                func_data[0] = func_data[0].split(" ; ")[0]
                target_s.write_text(f".syntax unified\n.text\n\n{mode}\n{'\n'.join(l for l in func_data)}")

                # The asm tree mirrors the source tree (build/<version>/asm/<src path>.s),
                # so the file that owns this function is the matching .c/.cpp source.
                rel = cur_path.relative_to(build_f)
                for ext in (".c", ".cpp"):
                    candidate = REPO_ROOT / rel.with_suffix(ext)
                    if candidate.exists():
                        owning_src = candidate
                        break

                done = True
                print("func parsing is done!")
                break

            if done:
                break

        assert done, "unexpected result"

        # Seed base.c with the m2ctx context of the owning source file, followed
        # by the function's existing implementation copied verbatim.
        if owning_src is not None:
            print(f"generating context from {owning_src}")
            ctx_result = subprocess.run(
                [sys.executable, str(REPO_ROOT / "tools" / "m2ctx.py"), str(owning_src), "-f", str(base_c)],
                cwd=REPO_ROOT,
            )
            context = ""
            if ctx_result.returncode == 0:
                context = base_c.read_text(encoding="utf-8")
                # Drop compiler-provided __STDC_ defines that m2ctx emits; the
                # compiler defines these natively, so keeping them causes conflicts.
                context = "".join(
                    line for line in context.splitlines(keepends=True)
                    if not line.lstrip().startswith("#define __STDC_")
                )
            else:
                print("warning: m2ctx failed; base.c will only contain the implementation")

            impl = extract_function_source(owning_src, func)
            if impl is None:
                print(f"warning: no existing implementation of {func} found in {owning_src}")
                impl = ""

            # Write as Shift-JIS to match the '-enc SJIS' compile flag and keep the
            # implementation byte-for-byte identical to the source.
            parts = [p for p in (context.rstrip(), impl.rstrip()) if p]
            base_c.write_text("\n\n".join(parts) + "\n" if parts else "", encoding="shift-jis")
        else:
            print(f"warning: no source file owns {func}; leaving base.c empty")
            base_c.touch(exist_ok=True)

        compilesh_data = f"""#!/usr/bin/env bash
INPUT="$(realpath "$1")"
OUTPUT="$(realpath "$3")"
cd {REPO_ROOT}
./wibo {COMPILER} {CC_FLAGS} {CC_INCLUDES} -lang=c99 -d {version} -c "$INPUT" -o "$OUTPUT"
"""
        compilesh = out_dir / "compile.sh"
        compilesh.write_text(compilesh_data)
        compilesh.chmod(compilesh.stat().st_mode | stat.S_IEXEC)

        settings_data = f"""func_name = "{func}"
compiler_type = "mwcc"

# uncomment lines below to customize randomization pass weights
# see --help=randomization-passes for descriptions
[weight_overrides]
# perm_temp_for_expr = 100.0
# perm_expand_expr = 20.0
# perm_reorder_stmts = 10.0
# perm_reorder_decls = 10.0
# perm_add_mask = 15.0
# perm_xor_zero = 0.5
# perm_cast_simple = 10.0
# perm_refer_to_var = 10.0
# perm_float_literal = 3.0
# perm_randomize_internal_type = 10.0
# perm_randomize_external_type = 5.0
# perm_randomize_function_type = 5.0
# perm_split_assignment = 10.0
# perm_sameline = 0.5
# perm_ins_block = 10.0
# perm_struct_ref = 0.5
# perm_empty_stmt = 0.5
# perm_condition = 10.0
# perm_mult_zero = 5.0
# perm_factor_mult = 5.0
# perm_factor_shift = 5.0
# perm_dummy_comma_expr = 5.0
# perm_add_self_assignment = 5.0
# perm_commutative = 5.0
# perm_add_sub = 5.0
# perm_inequalities = 5.0
# perm_compound_assignment = 0.5
# perm_remove_ast = 5.0
# perm_duplicate_assignment = 5.0
# perm_chain_assignment = 10.0
# perm_long_chain_assignment = 3.0
# perm_pad_var_decl = 1.0
# perm_inline = 10.0
"""
        settings = out_dir / "settings.toml"
        settings.write_text(settings_data)

    result = subprocess.run(["arm-none-eabi-as", "-mlittle-endian", "-march=armv5te", str(target_s), "-o", str(target_s.with_suffix(".o"))])
    result.check_returncode()
    print(f"permuter nonmatchings/{func} --best-only -j8")
    print("ok! don't forget to fill base.c")
