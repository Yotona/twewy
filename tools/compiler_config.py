"""Compiler versions and flags for every source file, shared by configure.py and the other tools."""

import os
from dataclasses import dataclass
from functools import cache
from pathlib import Path

ROOT_PATH = Path(__file__).resolve().parent.parent
MWCC_ROOT = Path("tools") / "mwccarm"

MWCC_DEFAULT_VERSION = "2.0/sp1p5"

# decomp.me compiler names for each mwcc version
DECOMP_ME_COMPILERS = {
    "1.2/sp4": "mwcc_20_87",
    "2.0/sp1p5": "mwcc_30_131",
}

# Passed to every compile
COMMON_CC_FLAGS = (
    "-O4,p",  # Optimize heavily, prioritize speed
    "-enum int",  # Use int-sized enums
    "-proc arm946e",  # Target processor
    "-gccext,on",  # Enable GCC extensions
    "-fp soft",  # Compute float operations in software
    "-inline noauto",  # Inline only functions marked with 'inline'
    "-interworking",  # Enable ARM/Thumb interworking
    "-w off",  # Disable warnings
    "-sym on",  # Debug info, including line numbers
    "-gccinc",  # Interpret #include "..." and #include <...> equally
    "-msgstyle gcc",  # Use GCC-like messages (some IDEs will make file names clickable)
    "-enc SJIS",  # Use Shift-JIS encoding
)

# Language each source file suffix is compiled as
SOURCE_LANGUAGES = {
    ".c": "c99",
    ".cpp": "c++",
}


@dataclass(frozen=True)
class CompilerConfig:
    """The compiler settings that differ between sources."""

    version: str = MWCC_DEFAULT_VERSION
    char: str = "signed"  # Signedness of plain char
    ipa: str | None = "file"  # Interprocedural analysis scope, None if unsupported
    strings: str = "reuse"  # Whether identical string literals share storage
    exceptions: bool = False  # Exception tables, which are emitted for C functions too
    rtti: bool = False  # Runtime type information (type_info objects referenced by vtables)

    def flags(self) -> list[str]:
        flags = [f"-char {self.char}"]
        if self.ipa is not None:
            flags.append(f"-ipa {self.ipa}")
        flags.append(f"-str {self.strings}")
        flags.append(f"-Cpp_exceptions {'on' if self.exceptions else 'off'}")
        flags.append(f"-RTTI {'on' if self.rtti else 'off'}")
        return flags


DEFAULT_COMPILER_CONFIG = CompilerConfig()

# Files and directories that need different settings than the project default, the most specific path wins
COMPILER_CONFIGS: dict[Path, CompilerConfig] = {
    Path("libs/c"): CompilerConfig(exceptions=True),
    Path("libs/cri"): CompilerConfig(ipa="function", strings="noreuse"),
    Path("libs/runtime"): CompilerConfig(
        char="unsigned", ipa="function", exceptions=True
    ),
    Path("libs/runtime/src/cxxabi_rtti.cpp"): CompilerConfig(
        char="unsigned", ipa="function", exceptions=True, rtti=True
    ),
    Path("src/Debug/Abe/Mini108.c"): CompilerConfig(version="1.2/sp4", ipa=None),
    Path("src/Field/Flash"): CompilerConfig(ipa="function", strings="noreuse"),
    Path("src/Field/Flash/SeqFlash.c"): DEFAULT_COMPILER_CONFIG,
}


def is_source(path: Path) -> bool:
    return path.suffix in SOURCE_LANGUAGES


def repo_relative(path: Path) -> Path:
    """`path` relative to the repository root, given either relative to the root or absolute."""
    if path.is_absolute():
        path = path.resolve().relative_to(ROOT_PATH)
    return Path(os.path.normpath(path))


def config_path_for_source(source: Path) -> Path | None:
    """The most specific COMPILER_CONFIGS path `source` falls under."""
    source = repo_relative(source)
    for path in (source, *source.parents):
        if path in COMPILER_CONFIGS:
            return path
    return None


@cache
def compiler_config_for_source(source: Path) -> CompilerConfig:
    path = config_path_for_source(source)
    return DEFAULT_COMPILER_CONFIG if path is None else COMPILER_CONFIGS[path]


def source_cc_flags(source: Path) -> list[str]:
    """Flags that vary between sources: the compiler config and the language."""
    return [
        *compiler_config_for_source(source).flags(),
        f"-lang={SOURCE_LANGUAGES[source.suffix]}",
    ]


def cc_flags(source: Path) -> str:
    """Every flag `source` is compiled with, except includes and defines."""
    return " ".join((*COMMON_CC_FLAGS, *source_cc_flags(source)))


def compiler_versions_in_use() -> list[str]:
    configs = [DEFAULT_COMPILER_CONFIG, *COMPILER_CONFIGS.values()]
    return list(dict.fromkeys(config.version for config in configs))


def unused_compiler_configs(sources: list[Path]) -> list[Path]:
    """Config paths that no source falls under, such as after a rename."""
    used = {config_path_for_source(source) for source in sources}
    return [path for path in COMPILER_CONFIGS if path not in used]


def compiler_executable(version: str, mwcc_root: Path = MWCC_ROOT) -> Path:
    return mwcc_root / version / "mwccarm.exe"


def linker_executable(version: str, mwcc_root: Path = MWCC_ROOT) -> Path:
    return mwcc_root / version / "mwldarm.exe"


def include_dirs() -> list[Path]:
    """Include directories relative to the repository root: include/ and each library's include/."""
    libs = ROOT_PATH / "libs"
    return [
        Path("include"),
        *sorted(
            path.relative_to(ROOT_PATH)
            for path in libs.glob("*/include")
            if path.is_dir()
        ),
    ]


def mwcc_include_args() -> list[str]:
    return [arg for include in include_dirs() for arg in ("-i", str(include))]


def gcc_include_args() -> list[str]:
    """Include flags for gcc and clang, which must not see the host's system headers."""
    return ["-nostdinc", *(f"-I{include}" for include in include_dirs())]


def region_define(game_version: str) -> str:
    return f"REGION_{game_version.upper()}"
