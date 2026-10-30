#!/usr/bin/env python3

import argparse
import json
import os
import subprocess
import sys
from collections.abc import Generator
from pathlib import Path
from typing import Any

import ninja_syntax
from compiler_config import (
    ALL_WARNINGS_CC_FLAG,
    COMMON_CC_FLAGS,
    DEFAULT_COMPILER_CONFIG,
    ROOT_PATH,
    compiler_config_for_source,
    compiler_executable,
    compiler_versions_in_use,
    gcc_include_args,
    is_source,
    linker_executable,
    mwcc_include_args,
    region_define,
    source_cc_flags,
    unused_compiler_configs,
)
from get_platform import Platform, get_platform

# Game Versions
VERSIONS = [
    "usa",
    "jp",
]
ALL_VERSIONS = "all"

DEFAULT_WIBO_PATH = "./wibo"


parser = argparse.ArgumentParser(description="Generates build.ninja")
parser.add_argument(
    "-w",
    type=str,
    default=DEFAULT_WIBO_PATH,
    dest="wine",
    required=False,
    help="Path to Wine/Wibo (Linux only)",
)
parser.add_argument(
    "--compiler",
    type=Path,
    required=False,
    help="Path to pre-installed compiler root directory",
)
parser.add_argument("--no-extract", action="store_true", help="Skip extraction step")
parser.add_argument(
    "--dsd", type=Path, required=False, help="Path to pre-installed dsd executable"
)
parser.add_argument(
    "-v",
    "--version",
    help=f"Game version to build, or '{ALL_VERSIONS}' for every version whose ROM is supplied",
    choices=[*VERSIONS, ALL_VERSIONS],
    default=ALL_VERSIONS,
)
parser.add_argument(
    "--report",
    action="store_true",
    help="Also generate each version's objdiff progress report when running ninja",
)
parser.add_argument(
    "--shift-check",
    action="store_true",
    help="Also relink each version with every module padded when running ninja, to find addresses that would not move",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="Print everything, including all warnings. Implies --report and --shift-check",
)
parser.add_argument(
    "--skip-check",
    action="store_true",
    help="Build the ROM without verifying it against the base ROM (for non-matching or shifted builds)",
)
args = parser.parse_args()
if args.verbose:
    args.report = True
    args.shift_check = True

# Config
GAME = "twewy"
DSD_VERSION = "v0.12.1"
WIBO_VERSION = "1.2.0"
OBJDIFF_VERSION = "v3.8.3"

# Common linker flags passed to all modules and final arm9.o link
LD_FLAGS = "-proc arm946e -interworking -map closure,unused -msgstyle gcc -nodead -nostdlib"  # TODO: Remove -nodead

# Link as a static library, only passed to the module links
MODULE_LD_FLAGS = "-library"

# Set entry function, only passed to the final arm9.o link
ARM9_LD_FLAGS = "-m Entry"

# Force color output in terminal
DSD_BASE_FLAGS = "--force-color"


# Paths given on the command line are relative to where configure.py was run from
if args.compiler is not None:
    args.compiler = Path(os.path.abspath(args.compiler))
if args.dsd is not None:
    args.dsd = Path(os.path.abspath(args.dsd))
if args.wine != DEFAULT_WIBO_PATH and os.path.dirname(args.wine):
    args.wine = os.path.abspath(args.wine)

# Everything below is relative to the repository root, where ninja runs
os.chdir(ROOT_PATH)
this_file = Path(__file__).resolve().relative_to(ROOT_PATH)
build_ninja_path = Path("build.ninja")
arm7_bios_path = Path("arm7_bios.bin")
config_path = Path("config")
build_path = Path("build")
src_path = Path("src")
libs_path = Path("libs")
extract_path = Path("extract")
tools_path = Path("tools")
mwcc_root = args.compiler or tools_path / "mwccarm"


def compiler_executable_path(version: str) -> str:
    return os.path.join(".", str(compiler_executable(version, mwcc_root)))


def linker_executable_path(version: str) -> str:
    return os.path.join(".", str(linker_executable(version, mwcc_root)))


CC_INCLUDES = " ".join(mwcc_include_args())


# Platform info
platform = get_platform()
if platform is None:
    sys.exit(1)

EXE = platform.exe
WINE = args.wine if platform.system != "windows" else ""
WINE_PREFIX = f"{WINE} " if WINE else ""
DSD = str(args.dsd or os.path.join(".", f"dsd{EXE}"))
OBJDIFF = os.path.join(".", f"objdiff-cli{EXE}")
LD = linker_executable_path(DEFAULT_COMPILER_CONFIG.version)
PYTHON = sys.executable
OBJDIFF_SCRATCH = str(tools_path / "objdiff_scratch.py")
SHIFT_CHECK = str(tools_path / "shift_check.py")
COMPILER_CONFIG = str(tools_path / "compiler_config.py")


def baserom(game_version: str) -> Path:
    return extract_path / f"baserom_{GAME}_{game_version}.nds"


def baserom_config(game_version: str) -> Path:
    return extract_path / game_version / "config.yaml"


def arm9_config_yaml(game_version: str) -> Path:
    return config_path / game_version / "arm9" / "config.yaml"


def rom_input(game_version: str) -> Path:
    if args.no_extract:
        return baserom_config(game_version)
    return baserom(game_version)


def is_rom_supplied(game_version: str) -> bool:
    return rom_input(game_version).is_file()


def configure_args() -> list[str]:
    """Arguments that rerun configure.py from the repository root with the same settings."""
    argv = ["--version", args.version]
    if args.wine != DEFAULT_WIBO_PATH:
        argv += ["-w", args.wine]
    if args.compiler is not None:
        argv += ["--compiler", str(args.compiler)]
    if args.dsd is not None:
        argv += ["--dsd", str(args.dsd)]
    if args.no_extract:
        argv.append("--no-extract")
    if args.report:
        argv.append("--report")
    if args.shift_check:
        argv.append("--shift-check")
    if args.verbose:
        argv.append("--verbose")
    if args.skip_check:
        argv.append("--skip-check")
    return argv


def configured_versions() -> list[str]:
    if args.version != ALL_VERSIONS:
        return [args.version]
    versions = [version for version in VERSIONS if is_rom_supplied(version)]
    if not versions:
        print(
            f"No ROM found in {extract_path}/, configuring {VERSIONS[0]} only",
            file=sys.stderr,
        )
        return VERSIONS[:1]
    return versions


class Project:
    def __init__(
        self,
        game_version: str,
        *,
        platform: Platform,
        delinks_json: Any,
        source_files: list[Path],
        primary: bool,
    ):
        self.game_version = game_version
        """Version of the game"""

        self.game_config = config_path / game_version
        """Root directory for dsd configs"""

        self.platform = platform
        """Host platform information"""

        self.delinks_json = delinks_json
        """Delinks JSON data from dsd"""

        self.source_files = source_files
        """Every C/C++ source file"""

        self.primary = primary
        """Whether this is the version objdiff.json in the repository root shows"""

        self.game_build = build_path / game_version
        """Path to build directory"""

        self.game_extract = extract_path / game_version
        """Path to extract directory"""

        self.delinks_files = get_config_files(self.game_config, "delinks.txt")
        """Paths to every delinks.txt file"""

        self.relocs_files = get_config_files(self.game_config, "relocs.txt")
        """Paths to every relocs.txt file"""

        self.symbols_files = get_config_files(self.game_config, "symbols.txt")
        """Paths to every symbols.txt file"""

        self.linked_objects = {Path(file["object_to_link"]) for file in self.files()}
        """Objects the arm9 link consumes"""

    def target(self, name: str) -> str:
        return f"{self.game_version}/{name}"

    def dsd_configs(self) -> list[str]:
        return self.delinks_files + self.relocs_files + self.symbols_files

    def build_rom(self) -> Path:
        return self.game_build / f"{GAME}_{self.game_version}.nds"

    def build_rom_config(self) -> Path:
        return self.game_build / "build" / "rom_config.yaml"

    def source_object_file(self, source_file: Path) -> Path:
        return (self.game_build / source_file).with_suffix(".o")

    def source_object_files(self) -> list[str]:
        return [
            str(self.source_object_file(source_file))
            for source_file in self.source_files
        ]

    def arm9_o(self) -> Path:
        return self.game_build / "arm9.o"

    def shift_build(self) -> Path:
        """Relink with every module padded, to find addresses that would not move"""
        return self.game_build / "shift"

    def arm9_disassembly_dir(self) -> Path:
        return self.game_build / "asm"

    def objdiff_json(self) -> Path:
        return self.game_build / "objdiff.json"

    def objdiff_report(self) -> Path:
        return self.game_build / "report.json"

    def files(self) -> list[dict[str, str]]:
        return self.delinks_json["files"]

    def delink_files(self) -> list[str]:
        return sorted({file["delink_file"] for file in self.files()})

    def arm9_lcf_file(self) -> str:
        return self.delinks_json["arm9_lcf_file"]

    def arm9_objects_file(self) -> str:
        return self.delinks_json["arm9_objects_file"]


# Targets each version provides, which are also built for every version under the same name
VERSION_TARGETS = [
    "delink",
    "dis",
    "arm9",
    "rom",
    "sha1",
    "check_modules",
    "check_symbols",
    "check_shift",
    "check",
    "objdiff",
    "objects",
    "report",
    "apply",
]


def check_can_run_dsd() -> bool:
    try:
        output = subprocess.run(
            [DSD, "--version"], capture_output=True, text=True, check=True
        )
        version = output.stdout.strip().split(" ")[-1]
        if not version.startswith("v"):
            version = "v" + version

        # If it's not the correct version, Ninja will download it and then rerun this script
        return version == DSD_VERSION or args.dsd is not None
    except (subprocess.CalledProcessError, OSError):
        return False


def load_delinks_json(game_version: str) -> Any:
    out = subprocess.run(
        [
            DSD,
            "--force-color",
            "json",
            "delinks",
            "--config-path",
            arm9_config_yaml(game_version),
        ],
        capture_output=True,
        text=True,
        check=False,
    )
    if out.returncode != 0:
        sys.exit(f"Error running dsd:\n{out.stderr.strip()}")
    return json.loads(out.stdout)


def main():
    versions = configured_versions()
    for version in versions:
        if not (config_path / version).is_dir():
            sys.exit(f"Version '{version}' not recognized")

    if not args.no_extract:
        # Extracting into an existing directory leaves the extract directory configure watches untouched
        for version in versions:
            if is_rom_supplied(version):
                (extract_path / version).mkdir(exist_ok=True)

    source_files = sorted(get_source_files([src_path, libs_path]))
    unused_configs = unused_compiler_configs(source_files)
    if unused_configs:
        sys.exit(
            f"No source files under compiler config paths in {COMPILER_CONFIG}: "
            + ", ".join(map(str, unused_configs))
        )

    projects: list[Project] = []
    can_run_dsd = check_can_run_dsd()
    if can_run_dsd:
        projects = [
            Project(
                version,
                platform=platform,
                delinks_json=load_delinks_json(version),
                source_files=source_files,
                primary=version == versions[0],
            )
            for version in versions
        ]

    with build_ninja_path.open("w") as file:
        n = ninja_syntax.Writer(file)

        n.rule(
            name="download_tool",
            command=f"{PYTHON} tools/download_tool.py $tool $tag --path $path",
        )
        n.newline()

        if arm7_bios_path.is_file():
            n.variable("arm7_bios_flag", f"--arm7-bios {arm7_bios_path}")
        else:
            n.variable("arm7_bios_flag", "")
        n.newline()

        n.rule(
            name="extract",
            command=f"{DSD} {DSD_BASE_FLAGS} rom extract --rom $in --output-path $output_path $arm7_bios_flag",
        )
        n.newline()

        n.rule(
            name="delink",
            command=f"{DSD} {DSD_BASE_FLAGS} delink --config-path $config_path",
        )
        n.newline()

        n.rule(
            name="disassemble",
            command=f"{DSD} {DSD_BASE_FLAGS} dis --config-path $config_path --asm-path $output_path --ual",
        )
        n.newline()

        # -MMD excludes all includes instead of just system includes for some reason, so use -MD instead.
        common_cc_flags = [*COMMON_CC_FLAGS]
        if args.verbose:
            common_cc_flags.append(ALL_WARNINGS_CC_FLAG)
        mwcc_cmd = (
            f'{WINE_PREFIX}"$compiler" {" ".join(common_cc_flags)} {CC_INCLUDES} '
            "$cc_flags -d $region_define -MD -c $in -o $basedir"
        )
        mwcc_common_implicit: list[str] = []
        if platform.system != "windows":
            transform_dep = "tools/transform_dep.py"
            mwcc_cmd += f" && {PYTHON} {transform_dep} $basefile.d $basefile.d"
            mwcc_common_implicit.append(transform_dep)
            if WINE == DEFAULT_WIBO_PATH:
                mwcc_common_implicit.append(WINE)
        n.rule(
            name="mwcc",
            command=mwcc_cmd,
            depfile="$basefile.d",
        )
        n.newline()

        n.rule(name="lcf", command=f"{DSD} {DSD_BASE_FLAGS} lcf -c $config_path")
        n.newline()

        n.rule(
            name="mwld",
            command=f'{WINE_PREFIX}"{LD}" {LD_FLAGS} $extra_ld_flags @$objects_file $lcf_file -o $out',
        )
        n.newline()

        n.rule(
            name="rom_config",
            command=f"{DSD} {DSD_BASE_FLAGS} rom config --elf $in --config $config_path",
        )
        n.newline()

        n.rule(
            name="rom_build",
            command=f"{DSD} {DSD_BASE_FLAGS} rom build --config $in --rom $out $arm7_bios_flag",
        )
        n.newline()

        n.rule(
            name="objdiff",
            command=f"{PYTHON} {OBJDIFF_SCRATCH} --dsd {DSD} --config-path $config_path --region-define $region_define $output_path_args",
        )
        n.newline()

        n.rule(
            name="objdiff_report",
            command=f"{OBJDIFF} report generate -p $project_dir -o $out",
        )
        n.newline()

        n.rule(
            name="m2ctx",
            command=f"{PYTHON} tools/m2ctx.py -D $region_define -f $out --depfile $out.d $in",
            depfile="$out.d",
        )
        n.newline()

        n.rule(
            name="check_modules",
            command=f"{DSD} {DSD_BASE_FLAGS} check modules --config-path $config_path --fail",
        )
        n.newline()

        n.rule(
            name="check_symbols",
            command=f"{DSD} {DSD_BASE_FLAGS} check symbols --config-path $config_path --elf-path $elf_path --fail --max-lines 20",
        )
        n.newline()

        n.rule(
            name="apply",
            command=f"{DSD} {DSD_BASE_FLAGS} apply --config-path $config_path --elf-path $elf_path",
        )
        n.newline()

        n.rule(
            name="shift_lcf",
            command=f"{PYTHON} {SHIFT_CHECK} lcf $in $out",
        )
        n.newline()

        n.rule(
            name="check_shift",
            command=f"{PYTHON} {SHIFT_CHECK} check --lcf $lcf_file --elf $elf_path --shifted-elf $in --allowlist $allowlist --stamp $out",
        )
        n.newline()

        n.rule(name="sha1", command=f"{PYTHON} tools/sha1.py $in -c $sha1_file")
        n.newline()

        configure_cmdline = subprocess.list2cmdline(configure_args())
        n.rule(
            name="configure",
            command=f"{PYTHON} {this_file} {configure_cmdline}",
            generator=True,
        )
        n.newline()

        create_compilation_database(source_files, versions[0])
        add_download_tool_builds(n)
        add_configure_build(n, versions)

        if can_run_dsd:
            for project in projects:
                add_extract_build(n, project)
                add_delink_and_lcf_builds(n, project)
                add_disassemble_builds(n, project)
            for project in projects:
                add_mwcc_builds(
                    n, project, mwcc_common_implicit, link_inputs=True
                )  # Emit the compiles the links need first to unblock them early
            for project in projects:
                add_mwld_and_rom_builds(
                    n, project
                )  # then everything downstream of the links
                add_check_builds(n, project)
            for project in projects:
                add_mwcc_builds(
                    n, project, mwcc_common_implicit, link_inputs=False
                )  # then the other compiles (incomplete units)
                add_objdiff_builds(n, project)
                add_apply_build(n, project)
            add_version_target_builds(n, projects)

            if args.skip_check:
                defaults = ["objdiff", "objects", "rom"]
            else:
                defaults = ["objdiff", "objects", "check", "sha1"]
            if args.report:
                defaults.append("report")
            if args.shift_check:
                defaults.append("check_shift")
            n.default(defaults)
        else:
            n.default(["download_tools"])


def add_download_tool_builds(n: ninja_syntax.Writer):
    downloads: list[str] = []

    if args.dsd is None:
        downloads.append(DSD)
        n.build(
            rule="download_tool",
            outputs=DSD,
            variables={
                "tool": "dsd",
                "tag": DSD_VERSION,
                "path": DSD,
            },
        )
        n.newline()

    downloads.append(OBJDIFF)
    n.build(
        rule="download_tool",
        outputs=OBJDIFF,
        variables={
            "tool": "objdiff",
            "tag": OBJDIFF_VERSION,
            "path": OBJDIFF,
        },
    )
    n.newline()

    if args.compiler is None:
        compiler_outputs = [
            compiler_executable_path(version) for version in compiler_versions_in_use()
        ]
        downloads.extend(compiler_outputs)
        downloads.append(LD)
        n.build(
            rule="download_tool",
            outputs=[*compiler_outputs, LD],
            variables={
                "tool": "mwccarm",
                "tag": "latest",
                "path": str(tools_path),
            },
        )
        n.newline()

    if platform.system != "windows" and WINE == DEFAULT_WIBO_PATH:
        downloads.append(WINE)
        n.build(
            rule="download_tool",
            outputs=WINE,
            variables={
                "tool": "wibo",
                "tag": WIBO_VERSION,
                "path": WINE,
            },
        )
        n.newline()

    n.build(
        inputs=downloads,
        rule="phony",
        outputs="download_tools",
    )
    n.newline()


def add_action_build(
    n: ninja_syntax.Writer, project: Project, name: str, **build_args: Any
):
    output = str(project.game_build / name)
    n.build(outputs=output, **build_args)
    n.build(inputs=output, rule="phony", outputs=project.target(name))
    n.newline()


def add_extract_build(n: ninja_syntax.Writer, project: Project):
    if not args.no_extract:
        n.build(
            inputs=str(baserom(project.game_version)),
            implicit=DSD,
            rule="extract",
            outputs=str(baserom_config(project.game_version)),
            variables={"output_path": str(project.game_extract)},
        )
        n.newline()


def add_mwld_and_rom_builds(n: ninja_syntax.Writer, project: Project):
    n.comment("Run linker")
    objects_to_link = [file["object_to_link"] for file in project.files()]
    elf_file = str(project.arm9_o())
    lcf_file = project.arm9_lcf_file()
    objects_file = project.arm9_objects_file()

    n.build(
        inputs=[*objects_to_link, lcf_file, objects_file],
        implicit=LD,
        rule="mwld",
        outputs=elf_file,
        variables={
            "extra_ld_flags": ARM9_LD_FLAGS,
            "lcf_file": str(lcf_file),
            "objects_file": str(objects_file),
        },
    )
    n.newline()

    n.build(
        inputs=elf_file,
        rule="phony",
        outputs=project.target("arm9"),
    )
    n.newline()

    rom_config_file = str(project.build_rom_config())
    n.build(
        inputs=elf_file,
        implicit=DSD,
        rule="rom_config",
        outputs=rom_config_file,
        variables={
            "config_path": str(arm9_config_yaml(project.game_version)),
        },
    )
    n.newline()

    rom_file = str(project.build_rom())
    n.build(
        inputs=rom_config_file,
        implicit=DSD,
        rule="rom_build",
        outputs=rom_file,
    )
    n.newline()

    n.build(
        inputs=rom_file,
        rule="phony",
        outputs=project.target("rom"),
    )
    n.newline()

    add_action_build(
        n,
        project,
        "sha1",
        inputs=rom_file,
        rule="sha1",
        variables={"sha1_file": project.game_config / "build.sha1"},
    )


def add_mwcc_builds(
    n: ninja_syntax.Writer,
    project: Project,
    mwcc_common_implicit: list[str],
    link_inputs: bool | None = None,
):
    """Emit the compile edges. `link_inputs` selects the objects the arm9 link
    consumes (True), the rest (False), or everything (None)."""
    for source_file in project.source_files:
        src_obj_path = project.source_object_file(source_file)
        if (
            link_inputs is not None
            and (src_obj_path in project.linked_objects) != link_inputs
        ):
            continue

        compiler_path = compiler_executable_path(
            compiler_config_for_source(source_file).version
        )
        n.build(
            inputs=str(source_file),
            implicit=[*mwcc_common_implicit, compiler_path],
            rule="mwcc",
            outputs=str(src_obj_path),
            variables={
                "compiler": compiler_path,
                "cc_flags": " ".join(source_cc_flags(source_file)),
                "region_define": region_define(project.game_version),
                "basedir": os.path.dirname(src_obj_path),
                "basefile": str(src_obj_path.with_suffix("")),
            },
        )
        n.newline()

        ctx_file = str(src_obj_path.with_suffix(f".ctx{source_file.suffix}"))
        n.build(
            inputs=str(source_file),
            rule="m2ctx",
            outputs=ctx_file,
            variables={"region_define": region_define(project.game_version)},
        )
        n.newline()


def get_source_files(dirs: list[Path]) -> Generator[Path, None, None]:
    for dir in dirs:
        for root, _, files in os.walk(dir):
            root = Path(root)
            for file in files:
                if is_source(root / file):
                    yield root / file


def add_delink_and_lcf_builds(n: ninja_syntax.Writer, project: Project):
    rom_config = str(baserom_config(project.game_version))
    delink_files = project.delink_files()
    n.comment("Delink ELF binaries when any delinks.txt file is modified")
    n.build(
        inputs=project.dsd_configs() + [rom_config],
        implicit=DSD,
        rule="delink",
        outputs=delink_files,
        variables={
            "config_path": str(arm9_config_yaml(project.game_version)),
        },
    )
    n.newline()

    n.build(inputs=delink_files, rule="phony", outputs=project.target("delink"))
    n.newline()

    lcf_file = project.arm9_lcf_file()
    objects_file = project.arm9_objects_file()
    n.build(
        inputs=project.delinks_files + [rom_config],
        implicit=DSD,
        rule="lcf",
        outputs=[lcf_file, objects_file],
        variables={
            "config_path": str(arm9_config_yaml(project.game_version)),
        },
    )
    n.newline()


def add_disassemble_builds(n: ninja_syntax.Writer, project: Project):
    add_action_build(
        n,
        project,
        "dis",
        inputs=project.dsd_configs(),
        implicit=DSD,
        rule="disassemble",
        variables={
            "config_path": str(arm9_config_yaml(project.game_version)),
            "output_path": str(project.arm9_disassembly_dir()),
        },
    )


def add_check_builds(n: ninja_syntax.Writer, project: Project):
    add_action_build(
        n,
        project,
        "check_modules",
        inputs=str(project.arm9_o()),
        rule="check_modules",
        variables={
            "config_path": str(arm9_config_yaml(project.game_version)),
        },
    )

    add_action_build(
        n,
        project,
        "check_symbols",
        inputs=str(project.arm9_o()),
        rule="check_symbols",
        variables={
            "config_path": str(arm9_config_yaml(project.game_version)),
            "elf_path": str(project.arm9_o()),
        },
    )

    lcf_file = project.arm9_lcf_file()
    shift_lcf_file = str(project.shift_build() / "arm9.lcf")
    n.build(
        inputs=lcf_file,
        implicit=SHIFT_CHECK,
        rule="shift_lcf",
        outputs=shift_lcf_file,
    )
    n.newline()

    shift_elf_file = str(project.shift_build() / "arm9.o")
    objects_file = project.arm9_objects_file()
    n.build(
        inputs=[
            *(file["object_to_link"] for file in project.files()),
            shift_lcf_file,
            objects_file,
        ],
        implicit=LD,
        rule="mwld",
        outputs=shift_elf_file,
        # The linker writes the module binaries next to the ELF but will not create the directory
        implicit_outputs=str(project.shift_build() / "build" / "arm9.bin"),
        variables={
            "extra_ld_flags": ARM9_LD_FLAGS,
            "lcf_file": shift_lcf_file,
            "objects_file": str(objects_file),
        },
    )
    n.newline()

    shift_stamp = str(project.shift_build() / "check.ok")
    allowlist = project.game_config / "shift_allowlist.txt"
    n.build(
        inputs=shift_elf_file,
        implicit=[str(project.arm9_o()), str(allowlist), SHIFT_CHECK],
        rule="check_shift",
        outputs=shift_stamp,
        variables={
            "lcf_file": str(lcf_file),
            "elf_path": str(project.arm9_o()),
            "allowlist": str(allowlist),
        },
    )
    n.newline()

    n.build(
        inputs=shift_stamp,
        rule="phony",
        outputs=project.target("check_shift"),
    )
    n.newline()

    n.build(
        inputs=[
            project.target("check_modules"),
            project.target("check_symbols"),
        ],
        rule="phony",
        outputs=project.target("check"),
    )
    n.newline()


def add_objdiff_builds(n: ninja_syntax.Writer, project: Project):
    output_paths = [project.game_build]
    if project.primary:
        output_paths.append(Path("."))
    objdiff_jsons = [str(path / "objdiff.json") for path in output_paths]
    n.build(
        inputs=project.dsd_configs(),
        implicit=[DSD, OBJDIFF_SCRATCH, COMPILER_CONFIG],
        rule="objdiff",
        outputs=objdiff_jsons,
        variables={
            "config_path": str(arm9_config_yaml(project.game_version)),
            "region_define": region_define(project.game_version),
            "output_path_args": " ".join(
                f"--output-path {path}" for path in output_paths
            ),
        },
    )
    n.newline()

    n.build(
        inputs=objdiff_jsons,
        rule="phony",
        outputs=project.target("objdiff"),
    )
    n.newline()

    # Incomplete units link their delinked object, so linking alone does not
    # compile their source. Keep those objects in the default build as well.
    source_object_files = project.source_object_files()
    n.build(
        inputs=source_object_files,
        rule="phony",
        outputs=project.target("objects"),
    )
    n.newline()

    n.build(
        inputs=str(project.objdiff_json()),
        implicit=[OBJDIFF] + project.delink_files() + source_object_files,
        rule="objdiff_report",
        outputs=str(project.objdiff_report()),
        variables={"project_dir": str(project.game_build)},
    )
    n.newline()

    n.build(
        inputs=str(project.objdiff_report()),
        rule="phony",
        outputs=project.target("report"),
    )
    n.newline()


def add_version_target_builds(n: ninja_syntax.Writer, projects: list[Project]):
    for name in VERSION_TARGETS:
        n.build(
            inputs=[project.target(name) for project in projects],
            rule="phony",
            outputs=name,
        )
    n.newline()


def add_configure_build(n: ninja_syntax.Writer, versions: list[str]):
    implicit = [
        str(this_file),
        COMPILER_CONFIG,
        # Require dsd to exist when rerunning configure.py
        DSD,
    ]
    if args.version == ALL_VERSIONS:
        # Supplying another version's ROM adds it. Ninja cannot depend on a file that
        # does not exist yet, so watch the nearest directory that does.
        for version in VERSIONS:
            if version not in versions:
                watched_dir = next(
                    path for path in rom_input(version).parents if path.is_dir()
                )
                if str(watched_dir) not in implicit:
                    implicit.append(str(watched_dir))
    for version in versions:
        # Symbols and relocations affect delinking, not graph membership.
        implicit.append(str(arm9_config_yaml(version)))
        implicit.extend(get_config_files(config_path / version, "delinks.txt"))
    n.build(
        outputs="build.ninja",
        rule="configure",
        implicit=implicit,
    )


def add_apply_build(n: ninja_syntax.Writer, project: Project):
    add_action_build(
        n,
        project,
        "apply",
        inputs=project.dsd_configs() + [str(project.arm9_o())],
        implicit=DSD,
        rule="apply",
        variables={
            "config_path": str(arm9_config_yaml(project.game_version)),
            "elf_path": str(project.arm9_o()),
        },
    )


def get_config_files(game_config: Path, name: str) -> list[str]:
    return sorted(
        f"{root}/{file}"
        for root, _, files in os.walk(game_config)
        for file in files
        if file == name
    )


def create_compilation_database(source_files: list[Path], game_version: str):
    """Describes each compile with the equivalent clang flags, for clangd."""
    db_path = Path("compile_commands.json")
    common_args = [f"-D{region_define(game_version)}", *gcc_include_args()]
    db: list[dict] = []

    for source_file in source_files:
        if compiler_config_for_source(source_file).char == "unsigned":
            char_arg = "-funsigned-char"
        else:
            char_arg = "-fsigned-char"
        db.append(
            {
                "directory": str(ROOT_PATH),
                "arguments": ["clang", char_arg, *common_args, "-c", str(source_file)],
                "file": str(source_file),
            }
        )
    with db_path.open("w") as f:
        f.write(json.dumps(db))


if __name__ == "__main__":
    main()
