# Contributing

Thanks for your interest in contributing! To begin, you'll need to follow the ARM7 BIOS instructions in order for the project setup to compile a fully byte-matching executable. Please review the guidelines for submission prior to contributing.

## ARM7 BIOS

The ARM7 BIOS will be required for the build to produce a byte-matching executable. Name this file `arm7_bios.bin` and place it in the root directory of this repository. Instructions on how to dump this from your NDS are not included here. BIOS dumps matching the listed below have been confirmed to work.

| File name       | SHA1                                       |
| --------------- | ------------------------------------------ |
| `arm7_bios.bin` | `6ee830c7f552c5bf194c20a2c13d5bb44bdb5c03` |
| `arm7_bios.bin` | `24f67bdea115a2c847c8813a262502ee1607b7df` |

Once the BIOS file is placed, `configure.py` will utilize it to produce a matching ROM when next ran.

After initially building the project, running `ninja` again will reprocess any necessary configurations and rebuild the project if changes are detected. The build process will report errors should the built ROM not be identical to the original.

[Objdiff](https://github.com/encounter/objdiff) is supported and preferred for analysing reverse engineered progress. [Ghidra](https://github.com/NationalSecurityAgency/ghidra) in combination with the [dsd-ghidra](https://github.com/AetiasHax/dsd-ghidra) extension are invaluable for this process.

## Guidelines for submission

**Never** allow copyrighted content to be submitted. This includes any assets from your `extract` directory, any SDK file known to be obtained through illegitimate means, and any compiled binaries that may contain such contents.

Any and all placeholder symbols (e.g. `func_020xxxxxx`, `data_ov000_02xxxxxx`, etc.) *must* be derived from the USA release. Never use placeholder symbols from other versions of the game, as this will cause confusion and may result in a non-matching build. Note that this will cause non-USA versions to be non-matching until those symbols receive canonical names, this is perfectly acceptable and expected. For purposes of this project, the USA release is considered our primary goal and all other versions are considered as a secondary objective.

Functions that are started but not yet matched are permitted to be left in a partially complete state. In this event, include a comment denoting it as non-matching. Optionally, please include a brief desciption of the nonmatch and a [decomp.me](https://decomp.me/) scratch ID. For ease of access, please maintain this comment format:

``` C
// Nonmatching: Reason
// Scratch: abcde
```

`*.c` and `*.h` files must adhere to formatting guidelines through `clang-format`. Specific rules are listed in the [.clang-format](../.clang-format) configuration file.

Delink, symbol, and reloc `.txt` files must be kept free of unneccessary whitespace.

Where applicable in `symbol.txt`, separate sections of listings by their relevant file and include a comment banner denoting that file's path.

Pull requests must produce a matching build before being submitted by verifying that `ninja` compiles the project without errors and reports `build/<version>/twewy_<version>.nds: OK`, e.g. `build/usa/twewy_usa.nds: OK`. If a change affects more than one game version, verify each affected version by reconfiguring for it (`python tools/configure.py --version <version>`) and rebuilding. Changes will not be accepted while *any* supported version of the game fails to build successfully.

Pull requests must pass all `pre-commit` checks. You can confirm the status of your pre-commit checks prior to submitting a pull request by running `pre-commit run --all-files`.

## Completing a file

In the event that your pull request will 100% match a given translation unit (TU) in all applicable delinked sections, please add the `complete` flag to the respective delink entry and then confirm that rebuilding the project still reports "OK".
