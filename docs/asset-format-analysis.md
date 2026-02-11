# Asset File Format Analysis

This document describes the various asset file formats used in The World Ends with You (Nintendo DS), based on analysis of the game code and community Python scripts from `Ega1232387/TWEWY_DS_pins` and `Ega1232387/TWEWYDS-tools`.

## Table of Contents
1. [PACK Archive Format](#pack-archive-format)
2. [Graphics/Tile Format](#graphicstile-format)
3. [Text Encoding](#text-encoding)
4. [Font Data Format](#font-data-format)
5. [Table Format](#table-format)

---

## PACK Archive Format

### Overview
PACK files are container archives used throughout TWEWY to bundle multiple related assets together. They are identified by the magic number `0x6B636170` ("pack" in ASCII).

### File Structure

#### Header (0x20 bytes)
```c
typedef struct {
    /* 0x00 */ u32 magic;         // 0x6B636170 ("pack")
    /* 0x04 */ u32 entryCount;    // Number of entries in the archive
    /* 0x08 */ u32 dataSize;      // Size of the data section
    /* 0x0C */ u32 reserved[5];   // Reserved/padding (20 bytes)
} PackHeader;
```

#### Entry Table (follows header)
Immediately after the header at offset 0x20, there is an array of entry descriptors:

```c
typedef struct {
    /* 0x00 */ u32 offset;  // Offset from start of file (add 0x20 to skip header)
    /* 0x04 */ u32 size;    // Size of this entry in bytes
} PackEntry;
```

- Number of entries: `entryCount` from header
- Each entry is 8 bytes
- Total entry table size: `entryCount * 8` bytes

#### Data Section
- Starts after the entry table
- Contains the actual file data for each entry
- Entries are aligned to 32-byte boundaries (padding with 0x00 if needed)
- To access an entry: seek to `entry.offset + 0x20` and read `entry.size` bytes

### Implementation Notes

From `PacMgr.c`:
```c
#define PAC_PACK_HEADER_MAGIC 0x6b636170 // 'pack'
#define PAC_PACK_HEADER_SIZE  0x20

Pack* PacMgr_LoadPack(BinIdentifier* iden) {
    // Loads pack header and entry table
    // Stores entries at offset 0x20 within the file
    newPac->entries = (PackEntry*)(data + 0x20);
}

void* PacMgr_LoadPackEntryData(Pack* pac, Bin* bin, u32* outSize,
                                s32 entryIndex, BOOL compressed) {
    // Entry offset includes the 0x20 header size
    return BinMgr_LoadRawData(bin, &pac->fileIden, pac->binIden,
                              pac->entries[entryIndex].offset + 0x20, outSize);
}
```

### Python Implementation

From `Ega1232387/TWEWYDS-tools/tww_pack.py`:

#### Unpacking
```python
def unpack(name, res_dir):
    file = open(name, "rb")
    header = to_hex(file.read(32))  # Read 0x20 byte header
    file_count = from_hex(header[4:8])  # Extract entry count

    file.seek(32)  # Skip to entry table
    file_data = []
    for i in range(file_count):
        offset = to_hex(file.read(4))
        size = to_hex(file.read(4))
        file_data.append([from_hex(offset), from_hex(size)])

    # Extract each entry
    for i in range(len(file_data)):
        data = file_data[i]
        file.seek(32 + data[0])  # 32 = header size
        with open(output_file, "wb") as file1:
            file1.write(file.read(data[1]))
```

#### Packing
```python
def pack(directory):
    names = sorted_file_list(directory)
    point = len(names) * 8  # Size of entry table
    index = []
    data = []

    for file in names:
        size = file_size
        fhex = to_hex(file_data)

        # Align to 32 bytes
        if size % 32 != 0:
            fhex += ["00"] * (((size // 32) + 1) * 32 - size)

        data += fhex
        offs_hex = hex(point).to_bytes(little_endian=True)
        size_hex = hex(size).to_bytes(little_endian=True)
        index += offs_hex + size_hex
        point += len(fhex)

    # Build header
    header = ["70", "61", "63", "6B"]  # "pack"
    header += len(names).to_bytes(4, little_endian=True)
    header += len(data).to_bytes(4, little_endian=True)
    header += [0x00] * 20  # Reserved

    write_file(header + index + data)
```

### Common PACK Files
- `Grp_*.bin` - Graphics packages (badges, fonts, UI elements)
- `Apl_*/` directories contain multiple PACK files
- Badge data: `Grp_BtlBadge.bin` contains pin graphics

---

## Graphics/Tile Format

### Overview
TWEWY uses standard Nintendo DS tile-based graphics with 8x8 pixel tiles, palette-indexed colors.

### Tile Format

Tiles are encoded as 8x8 pixel blocks with configurable bits-per-pixel (BPP):
- **4 BPP**: 16 colors (common for most graphics)
- **8 BPP**: 256 colors (less common)

From `Ega1232387/TWEWY_DS_pins/convert_to_img.py`:

```python
class TileCodec(object):
    """Abstract class for 8x8 tile codecs."""

    def __init__(self, bpp, stride=0):
        self.bits_per_pixel = bpp
        self.bytes_per_row = bpp  # 8 pixels per row / 8 bits per byte
        self.stride = stride * self.bytes_per_row
        self.tile_size = self.bytes_per_row * 8  # 8 rows per tile
        self.color_count = 1 << bpp
```

### Linear Codec (Most Common)

```python
class LinearCodec(TileCodec):
    """Linear palette-indexed 8x8 tile codec."""

    IN_ORDER = 1      # Pixels stored MSB first
    REVERSE_ORDER = 2 # Pixels stored LSB first

    def decode(self, bits, ofs=0, leng=8):
        """Decodes a tile."""
        pixels = []
        for i_row in range(leng):  # 8 rows
            for i_byte in range(self.bytes_per_row):
                pos = ofs + i_row*(self.bytes_per_row + self.stride) + i_byte
                byte = bits[pos]
                for i_pixel in range(self.start_pixel, self.boundary, self.step):
                    # Extract pixel (palette index)
                    pixel = (byte >> self.bits_per_pixel*i_pixel) & self.pixel_mask
                    pixels.append(pixel)
        return pixels
```

### Badge Graphics Example

Pin/badge graphics from `Grp_BtlBadge.bin`:
- Size: 32x32 pixels (16 tiles in a 4x4 grid)
- Format: 4 BPP (16 colors)
- Encoding: Linear, reverse order
- Data offset: Starts at byte 132 (0x84)
- Palette: Stored separately, indices need to be multiplied by 8 for 16-bit color values

#### Extraction Process
```python
# From separate.py - Split Grp_BtlBadge.bin by pack signatures
hexdata = binascii.hexlify(bytedata)
hexdata3 = ["00000000" + i for i in hexdata2.split("7061636b")][2::]

# From convert_to_img.py - Convert badge to image
a = LinearCodec(4, 2)  # 4 BPP, stride for 2D mode
pin = []
for j in range(132, 644, 16):  # 32 tiles of data
    b = a.decode(bytedata, j, leng=4)  # 4 rows per chunk
    pin += [j * 8 for j in b]  # Scale palette indices for grayscale

img = Image.new("L", (32, 32))
img.putdata(pin)
```

### Tile Layout Modes

**MODE_1D** (stride = 0):
- Tiles stored in linear order
- Used for sprites and simple graphics

**MODE_2D** (stride = -1 + tile_columns):
- Tiles stored in 2D grid layout
- Used for backgrounds and larger images
- Stride accounts for jumping to next row of tiles

### Palette Format
- DS uses 15-bit RGB555 color format (5 bits per channel)
- Palettes are arrays of 16-bit values (bit 15 unused)
- Structure: `0bbb_bbgg_gggr_rrrr` (little-endian)
- Palette data typically stored in separate section

---

## Text Encoding

### Overview
TWEWY uses a custom 2-4 byte character encoding system for text.

### Character Encoding

From `Ega1232387/TWEWYDS-tools/tww_text.py`:

#### Basic Format
- Most characters: 2 bytes (little-endian)
- Extended characters: 4 bytes
- Special markers: Various byte sequences

#### Character Table Structure
```python
dic = {
    # ASCII range (2 bytes)
    "0000": " ",  "0100": "!", "0200": '"', "0300": "#",
    "1000": "0",  "1100": "1", "1200": "2", "1300": "3",
    "2100": "A",  "2200": "B", "2300": "C", "2400": "D",
    "4100": "a",  "4200": "b", "4300": "c", "4400": "d",

    # Japanese Hiragana (2 bytes)
    "5F00": "ぁ", "6000": "あ", "6100": "ぃ", "6200": "い",
    "8800": "な", "8900": "に", "8A00": "ぬ", "8B00": "ね",

    # Japanese Katakana (2 bytes)
    "B200": "ァ", "B300": "ア", "B400": "ィ", "B500": "イ",
    "FF00": "ヮ", "0001": "ワ", "0101": "ヰ", "0201": "ヱ",

    # Special symbols (2-4 bytes)
    "0A01": "、", "0B01": "．", "1001": "！", "1F01": "？",

    # Control codes
    "FEFF": "\t",  # Tab
    "FFFF": "\n",  # Newline

    # Color codes
    "B6FF": "?",  # Blue text
    "BAFF": "?",  # Green text
    "BCFF": "?",  # Red text
    "BEFF": "?",  # Black text
}
```

### File Structure

Text data is stored in `mestxt.bin` (typically inside `Apl_Fuk/` directory).

#### Reading Text
```python
def unpack():
    with open("mestxt.bin", "rb") as file:
        a = to_hex(file.read())  # Convert to hex pairs

    with open("twewytext.txt", "w", encoding="utf-8") as file1:
        for hex_pair in a:
            if hex_pair.upper() in dic.keys():
                file1.write(dic[hex_pair.upper()])
            else:
                file1.write("?" + hex_pair + "?")  # Unknown char marker
```

#### Writing Text
```python
def pack():
    res = ""
    with open("twewytext.txt", encoding="utf-8") as file2:
        read1 = file2.read()
        read1 = read1.replace("…", "...")  # Normalize

        lst = list(filter(lambda x: len(x) > 0, read1.split("?")))
        for i in lst:
            if len(i) == 4 and is_hex(i):
                res += i  # Literal hex code
            else:
                for j in i:
                    # Look up character in dictionary
                    res += list(dic.keys())[list(dic.values()).index(j)]

        with open("mestxt.bin", "wb") as file3:
            file3.write(bytes.fromhex(res))
```

### Text Table (mestable.bin)

Accompanies text data to provide string offsets/lengths.

From `tww_table.py`:

```python
def table_to_nums(file1, file2):
    """Extract string length table"""
    with open(file1, 'rb') as f:
        data = f.read()
        hexdata = data.hex()

    hx = []
    for i in range(len(hexdata) // 16):
        # Each entry is 16 hex chars (8 bytes)
        hexx = hexdata[16 * i:16 * (i + 1)]
        # Extract size field (last 4 bytes, divide by 2 for char count)
        hx.append((int("".join(reverse_hex(hexx[8:])), 16) // 2))
```

#### Table Entry Format
```c
struct TextTableEntry {
    u32 offset;  // Offset in bytes (little-endian)
    u32 size;    // Size in bytes (little-endian)
};
```

- Offsets are absolute from start of text data
- Sizes are in bytes (divide by 2 for approximate character count)
- Table is same format as PACK entry table

---

## Font Data Format

### Overview
Font data contains character width information for text rendering.

Location: Inside `Grp_Font.bin` (which is itself a PACK file).

### Structure

From `Ega1232387/TWEWYDS-tools/tww_font.py`:

```python
def unpack(original, tbl):
    """Extract font width data"""
    with open(original, "rb") as file:
        filesize = os.path.getsize(original)
        with open(tbl, "w", encoding="utf8") as file1:
            for i in range(filesize):
                a = file.read(1)
                numb = int(a.hex(), 16)  # Character width in pixels

                # Calculate character code (little-endian offset)
                symb = hex(i)[2:].zfill(4).upper()
                symb = "".join(reversed([symb[i:i+2] for i in range(0, 4, 2)]))

                if symb in dic.keys():  # Known character
                    file1.write("?".join([symb, str(numb), dic[symb]]) + "\n")
                else:  # Unknown character
                    file1.write("?".join([symb, str(numb)]) + "\n")
```

### Font Entry Format
```
Character_Code?Width?Display_Character\n
```

Example:
```
2100?8?A
4100?7?a
6000?12?あ
B300?12?ア
```

### Packing Font Data
```python
def pack(tbl, final):
    """Pack font width data back"""
    with open(tbl, encoding="utf8") as file:
        with open(final, "wb") as file1:
            f = [i.rstrip("\n") for i in file.readlines()]
            for i in f:
                if "?" in i:
                    # Extract width value
                    ii = hex(int(i.split("?")[1]))[2:].zfill(2).upper()
                    file1.write(bytes.fromhex(ii))
```

### Font File Organization
- `Grp_Font.bin` - Master PACK file
  - Entry 0: Font metadata/header
  - Entry 1: Character width table
  - Entry 2+: Font bitmap graphics (tiles)

---

## Table Format

### Overview
Tables are used to index variable-length data (text strings, compressed data, etc.).

### Generic Table Structure

```c
struct TableEntry {
    u32 offset;  // Offset from table start or data start (little-endian)
    u32 size;    // Size of data in bytes (little-endian)
};
```

### Text Table Specifics

From `tww_table.py`:

#### Reading
```python
def table_to_nums(file1, file2):
    with open(file1, 'rb') as f:
        data = f.read()
        hexdata = data.hex()

    hx = []
    for i in range(len(hexdata) // 16):  # Each entry is 16 hex digits
        hexx = hexdata[16 * i:16 * (i + 1)]

        # Extract and reverse bytes (little-endian)
        offset_bytes = reverse_hex(hexx[0:8])
        size_bytes = reverse_hex(hexx[8:16])

        # Convert to integer, divide by 2 for character count
        size_value = int("".join(size_bytes), 16) // 2
        hx.append(size_value)
```

#### Writing
```python
def nums_to_table(file1, file2):
    with open(file1, "r") as f:
        hx = [int(i.rstrip("\n")) for i in f.readlines()]

    b = 0  # Current offset
    new_str = ""

    for i in hx:
        a = hex((i) * 2).lstrip("0x").zfill(8)  # Size in bytes
        bh = hex(b).lstrip("0x").zfill(8)       # Offset
        b += (i) * 2  # Advance offset

        # Write entry (little-endian)
        new_str += "".join(reverse_hex(bh))  # Offset
        new_str += "".join(reverse_hex(a))   # Size

    with open(file2, "wb") as f1:
        f1.write(bytes.fromhex(new_str))
```

### Usage Pattern

1. Table file (`*.tbl` or `mestable.bin`) contains array of entries
2. Data file (`*.bin` or `mestxt.bin`) contains actual data
3. To read string N:
   - Read entry N from table: `offset`, `size`
   - Seek to `offset` in data file
   - Read `size` bytes
   - Decode according to data type

---

## Utility Functions

### Byte Order Conversion

From `tww_utils.py`:

```python
def reverse_hex(s):
    """Reverse byte order for little-endian conversion"""
    s1 = wrap(s, 2)  # Split into 2-char pairs
    s1.reverse()
    return s1

def to_hex(byte):
    """Convert bytes to hex string array"""
    byte = byte.hex()
    return wrap(byte, 4)  # Split into 4-char pairs (2 bytes)

def from_hex(hexed):
    """Convert hex array to integer (little-endian)"""
    hexed.reverse()
    return int("".join(hexed), 16)
```

### Alignment

From `PacMgr.c`:

```c
#define ALIGN_TO_8(value) (((value) + 7U) & ~7U)

// For 32-byte alignment (common in file data):
aligned_size = ((size + 31) & ~31)
```

---

## File Locations

Common asset locations in `extract/usa/files/`:

### Graphics
- `Grp_BtlBadge.bin` - Battle badge (pin) graphics
- `Grp_Font.bin` - Font data and graphics
- `Grp_Title.bin` - Title screen graphics
- `Grp_Tutorial.bin` - Tutorial graphics
- `MenuTop_OBD01/` - Menu graphics

### Text
- `Apl_Fuk/mestxt.bin` - Main text data
- `Apl_Fuk/mestable.bin` - Text string table

### Data
- `Data/BadgeData.bin` - Badge stats and properties
- `Data/AbilityData.bin` - Ability data
- `Data/PsyData.bin` - Psychic ability data

### Font
- `Static/Font_Funakosi.bin` - Funakoshi font

---

## References

### Game Code
- `include/PacMgr.h` - PACK file management
- `src/PacMgr.c` - PACK loading/generation
- `include/DatMgr.h` - Data/resource management
- `src/DatMgr.c` - Resource loading
- `include/Text.h` - Text rendering
- `include/SpriteMgr.h` - Sprite/graphics management
- `include/BgResMgr.h` - Background resource management

### Community Tools
- [Ega1232387/TWEWY_DS_pins](https://github.com/Ega1232387/TWEWY_DS_pins) - Badge graphics extraction
  - `convert_to_img.py` - Tile decoder
  - `separate.py` - PACK splitter
- [Ega1232387/TWEWYDS-tools](https://github.com/Ega1232387/TWEWYDS-tools) - Text and general tools
  - `tww_pack.py` - PACK unpacker/packer
  - `tww_text.py` - Text encoder/decoder
  - `tww_table.py` - Table handler
  - `tww_font.py` - Font data handler
  - `tww_utils.py` - Common utilities

### Credits
- Yepoleb - Original TileCodecs library
- Ega1232387 - TWEWY DS romhacking tools and scripts
