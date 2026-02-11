#!/usr/bin/env python3
"""
PACK Archive Tool for The World Ends with You (Nintendo DS)

Unpacks and repacks PACK archive files used in TWEWY.

PACK Format:
  Header (0x20 bytes):
    0x00: u32 magic ("pack" = 0x6B636170)
    0x04: u32 entry_count
    0x08: u32 data_size
    0x0C: u32[5] reserved (padding)

  Entry Table (starts at 0x20):
    Each entry: 8 bytes
      0x00: u32 offset (from file start)
      0x04: u32 size

  Data Section:
    Entries aligned to 32-byte boundaries
"""

import argparse
import os
import struct
import sys
from pathlib import Path
from typing import List, Tuple

PACK_MAGIC = 0x6B636170  # "pack"
PACK_HEADER_SIZE = 0x20
ENTRY_SIZE = 8
DATA_ALIGN = 32


class PackEntry:
    """Represents a single entry in a PACK archive."""

    def __init__(self, offset: int, size: int):
        self.offset = offset
        self.size = size

    def __repr__(self):
        return f"PackEntry(offset=0x{self.offset:X}, size=0x{self.size:X})"


class PackArchive:
    """Handles reading and writing PACK archives."""

    def __init__(self):
        self.entries: List[PackEntry] = []
        self.data_size = 0

    @staticmethod
    def align_to(value: int, alignment: int) -> int:
        """Align value to specified boundary."""
        return (value + alignment - 1) & ~(alignment - 1)

    def read(self, filepath: Path) -> List[bytes]:
        """
        Read a PACK archive and return entry data.

        Args:
            filepath: Path to PACK file

        Returns:
            List of bytes objects, one per entry
        """
        if not filepath.exists():
            raise FileNotFoundError(f"File not found: {filepath}")

        with open(filepath, 'rb') as f:
            # Read header
            header = f.read(PACK_HEADER_SIZE)
            if len(header) < PACK_HEADER_SIZE:
                raise ValueError(f"File too small to be a PACK archive: {filepath}")

            # Parse header
            magic, entry_count, data_size = struct.unpack('<III', header[:12])

            if magic != PACK_MAGIC:
                raise ValueError(f"Invalid PACK magic: 0x{magic:08X} (expected 0x{PACK_MAGIC:08X})")

            self.data_size = data_size

            print(f"PACK Archive: {filepath.name}")
            print(f"  Entries: {entry_count}")
            print(f"  Data Size: 0x{data_size:X} ({data_size} bytes)")

            # Read entry table
            self.entries = []
            for i in range(entry_count):
                entry_data = f.read(ENTRY_SIZE)
                if len(entry_data) < ENTRY_SIZE:
                    raise ValueError(f"Truncated entry table at entry {i}")

                offset, size = struct.unpack('<II', entry_data)
                self.entries.append(PackEntry(offset, size))

            # Read entry data
            entry_contents = []
            for i, entry in enumerate(self.entries):
                # Seek to entry offset (offset includes header size)
                f.seek(PACK_HEADER_SIZE + entry.offset)
                data = f.read(entry.size)

                if len(data) < entry.size:
                    print(f"  Warning: Entry {i} truncated (expected {entry.size}, got {len(data)} bytes)")

                entry_contents.append(data)
                print(f"  Entry {i}: offset=0x{entry.offset:X}, size=0x{entry.size:X}")

            return entry_contents

    def write(self, filepath: Path, entry_data: List[bytes]) -> None:
        """
        Write a PACK archive from entry data.

        Args:
            filepath: Output PACK file path
            entry_data: List of bytes objects to pack
        """
        entry_count = len(entry_data)

        # Calculate entry table size
        entry_table_size = entry_count * ENTRY_SIZE

        # Build entries with proper alignment
        entries = []
        current_offset = entry_table_size
        aligned_data = []

        for i, data in enumerate(entry_data):
            size = len(data)
            entry = PackEntry(current_offset, size)
            entries.append(entry)

            # Align data to 32-byte boundary
            aligned_size = self.align_to(size, DATA_ALIGN)
            padding_size = aligned_size - size

            padded_data = data + b'\x00' * padding_size
            aligned_data.append(padded_data)

            current_offset += aligned_size

        # Data size is the total aligned size
        data_size = current_offset

        print(f"Creating PACK: {filepath.name}")
        print(f"  Entries: {entry_count}")
        print(f"  Data Size: 0x{data_size:X} ({data_size} bytes)")

        with open(filepath, 'wb') as f:
            # Write header
            header = struct.pack('<III', PACK_MAGIC, entry_count, data_size)
            header += b'\x00' * 20  # Reserved bytes
            f.write(header)

            # Write entry table
            for i, entry in enumerate(entries):
                entry_bytes = struct.pack('<II', entry.offset, entry.size)
                f.write(entry_bytes)
                print(f"  Entry {i}: offset=0x{entry.offset:X}, size=0x{entry.size:X}")

            # Write data
            for data in aligned_data:
                f.write(data)

        print(f"Successfully created: {filepath}")


def unpack_archive(pack_file: Path, output_dir: Path) -> None:
    """
    Unpack a PACK archive to a directory.

    Args:
        pack_file: Path to PACK file
        output_dir: Directory to extract entries to
    """
    archive = PackArchive()
    entries = archive.read(pack_file)

    # Create output directory
    output_dir.mkdir(parents=True, exist_ok=True)

    # Write entries
    base_name = pack_file.stem
    for i, data in enumerate(entries):
        entry_file = output_dir / f"{base_name}_{i}.bin"
        with open(entry_file, 'wb') as f:
            f.write(data)
        print(f"  Extracted: {entry_file.name} ({len(data)} bytes)")

    print(f"\nExtracted {len(entries)} entries to: {output_dir}")


def pack_directory(input_dir: Path, pack_file: Path) -> None:
    """
    Pack a directory into a PACK archive.

    Args:
        input_dir: Directory containing numbered .bin files
        pack_file: Output PACK file path
    """
    if not input_dir.exists() or not input_dir.is_dir():
        raise ValueError(f"Input directory not found: {input_dir}")

    # Find all .bin files with numeric suffixes
    files = sorted(
        [f for f in input_dir.glob("*.bin")],
        key=lambda x: extract_entry_number(x.name)
    )

    if not files:
        raise ValueError(f"No .bin files found in: {input_dir}")

    print(f"Packing directory: {input_dir}")
    print(f"  Found {len(files)} files")

    # Read all entry data
    entry_data = []
    for f in files:
        with open(f, 'rb') as fp:
            data = fp.read()
        entry_data.append(data)
        print(f"    {f.name}: {len(data)} bytes")

    # Create archive
    archive = PackArchive()
    archive.write(pack_file, entry_data)


def extract_entry_number(filename: str) -> int:
    """Extract entry number from filename (e.g., 'file_3.bin' -> 3)."""
    try:
        # Try to extract number from filename_N.bin pattern
        base = filename.rsplit('.', 1)[0]  # Remove extension
        if '_' in base:
            num_str = base.rsplit('_', 1)[-1]
            return int(num_str)
        return 0
    except (ValueError, IndexError):
        return 0


def list_archive(pack_file: Path) -> None:
    """
    List contents of a PACK archive.

    Args:
        pack_file: Path to PACK file
    """
    archive = PackArchive()

    with open(pack_file, 'rb') as f:
        # Read header
        header = f.read(PACK_HEADER_SIZE)
        magic, entry_count, data_size = struct.unpack('<III', header[:12])

        if magic != PACK_MAGIC:
            raise ValueError(f"Invalid PACK magic: 0x{magic:08X}")

        print(f"PACK Archive: {pack_file.name}")
        print(f"  Magic: 0x{magic:08X} ('{magic.to_bytes(4, 'little').decode('ascii')}')")
        print(f"  Entries: {entry_count}")
        print(f"  Data Size: 0x{data_size:X} ({data_size} bytes)")
        print(f"  File Size: {pack_file.stat().st_size} bytes")
        print()

        # Read entry table
        print(f"{'Index':<6} {'Offset':<12} {'Size':<12} {'End':<12}")
        print("-" * 48)

        for i in range(entry_count):
            offset, size = struct.unpack('<II', f.read(ENTRY_SIZE))
            end = offset + size
            print(f"{i:<6} 0x{offset:08X}   0x{size:08X}   0x{end:08X}")


def main():
    parser = argparse.ArgumentParser(
        description="PACK archive tool for The World Ends with You (Nintendo DS)",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Unpack archive
  %(prog)s unpack Grp_BtlBadge.bin -o extracted/

  # Pack directory back to archive
  %(prog)s pack extracted/ -o Grp_BtlBadge_new.bin

  # List archive contents
  %(prog)s list Grp_BtlBadge.bin
        """
    )

    subparsers = parser.add_subparsers(dest='command', help='Command to execute')

    # Unpack command
    unpack_parser = subparsers.add_parser('unpack', help='Unpack a PACK archive')
    unpack_parser.add_argument('input', type=Path, help='Input PACK file')
    unpack_parser.add_argument('-o', '--output', type=Path, required=True,
                               help='Output directory for extracted files')

    # Pack command
    pack_parser = subparsers.add_parser('pack', help='Pack directory into PACK archive')
    pack_parser.add_argument('input', type=Path, help='Input directory with .bin files')
    pack_parser.add_argument('-o', '--output', type=Path, required=True,
                             help='Output PACK file')

    # List command
    list_parser = subparsers.add_parser('list', help='List PACK archive contents')
    list_parser.add_argument('input', type=Path, help='Input PACK file')

    args = parser.parse_args()

    if not args.command:
        parser.print_help()
        return 1

    try:
        if args.command == 'unpack':
            unpack_archive(args.input, args.output)
        elif args.command == 'pack':
            pack_directory(args.input, args.output)
        elif args.command == 'list':
            list_archive(args.input)

        return 0

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
