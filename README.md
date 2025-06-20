# The World Ends With You (Nintendo DS)

[decompdev-url]:https://decomp.dev/Yotona/twewy
[progress-badge]:https://decomp.dev/Yotona/twewy.svg?mode=shield&label=Matched

[![Matched][progress-badge]][decompdev-url]

This repository contains a reverse-engineered codebase for *The World Ends With You* (TWEWY) for the Nintendo DS. The goal of this project is to study and understand the game's code and mechanics. All code is produced by individual contributors without the use of assets or files produced by their original creators. **No assets are included. This repository CANNOT build a functional ROM without the user supplying a legitimately obtained copy of the game.** No "ports" or modifications are planned or supported in any form by this repository, and such use cases will **never** be supported or considered.

## Prerequisites

1. This project supports the following operating systems:
    - Windows (Recommended)
    - Linux
2. Install the following:
    - Python 3.11+ and pip
    - GCC 9+
    - Ninja
3. Install Python dependencies: `python -m pip install -r tools/requirements.txt`
4. Install pre-commit hooks: `pre-commit install`

Note: For a byte-matching build, add a copy of the ARM7 bios files to the project root. See [contribution instructions](docs/CONTRIBUTING.md) for more details.

## Setup Instructions

1. **Obtain the Original Game**:
    - Ensure you have a legitimate copy of *The World Ends With You* for the Nintendo DS.
    - Extract the .nds ROM file from your cartridge. Instructions for how to extract a ROM file from the cartridge are not included in this repository.
    - Rename and place this file into the `extract` directory. Supported cartridges and expected names are detailed in the [extract](extract/README.md) directory.

2. **Initialize the Build Configuration**:
    - Run `python tools/configure.py`
    - This initializes the project for its first usage. After configuring once, the build command `ninja` will re-configure the project if needed.

3. **Build the Project**:
    - Run `ninja`
    - When first performed, any missing required executables will be downloaded, as listed in the [tools](tools/download_tool.py) directory. This includes [dsd](https://github.com/AetiasHax/ds-decomp), [wibo](https://github.com/decompals/wibo), and the compilers used to build a matching executable.

After the build completes, the final executable will be found in the [build](build) directory.

## Contributing

Contributions are welcome! Please ensure that any proposed changes or additions comply with the repository's guidelines and do not include any copyrighted material.

## Disclaimer

This project is for educational purposes only. All rights to *The World Ends With You* and its assets belong to their respective copyright holders. This project contains no copyrighted assets and **must not** be used for distribution or piracy purposes. This repository **does not contain any assets** from the original game, nor any original assets for respective software development kits (SDKs) that may be used by the title. To build and run the project, you must own a legitimate copy of *The World Ends With You*. Redistribution of copyrighted game assets is strictly prohibited.

## License

This project is licensed under the [CC0 1.0 Universal License](LICENSE). Note that this license applies only to the code in this repository and not to any assets from the original game.
