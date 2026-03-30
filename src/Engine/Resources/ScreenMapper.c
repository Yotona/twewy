#include "Display.h"
#include "Engine/Color.h"
#include "common_data.h"

typedef struct ScreenMapEntry {
    s32   engineId; // 0x00
    s32   bgLayer;  // 0x04
    s32   flags;    // 0x08
    s32   hOffset;  // 0x0C
    s32   vOffset;  // 0x10
    void* tilemap;  // 0x14
    void* unk_18;   // 0x18
    s32   width;    // 0x1C
    s32   height;   // 0x20
    void* tileData; // 0x24
} ScreenMapEntry;

typedef struct {
    ScreenMapEntry* entries[4];
} ScreenMapTable; // size 0x10

static ScreenMapTable data_0206b3e8[2];

static void func_0200d858(ScreenMapEntry* entry, s32 wrapH, s32 wrapV, void* defaultTile);

s32 func_0200d1d8(ScreenMapEntry* entry, s32 engineId, s32 bgLayer, void* tilemap, void* tileData, s32 width, s32 height) {
    ScreenMapEntry** slot;
    ScreenMapEntry*  prev_entry;

    slot            = &data_0206b3e8[engineId].entries[bgLayer];
    prev_entry      = *slot;
    *slot           = entry;
    entry->engineId = engineId;
    entry->bgLayer  = bgLayer;
    entry->flags |= 2;
    entry->hOffset  = 0;
    entry->vOffset  = 0;
    entry->tilemap  = tilemap;
    entry->width    = width;
    entry->height   = height;
    entry->tileData = tileData;
    func_0200d858(entry, 1, 1, &data_0205a128);
    return (s32)prev_entry;
}

s32 func_0200d244(ScreenMapEntry* entry) {
    s32                 wrapH;
    s32                 wrapV;
    s32                 prev;
    DisplayEngineState* engState;

    wrapH = ((entry->flags & 4) != 0) ? 1 : 0;
    wrapV = ((entry->flags & 8) != 0) ? 1 : 0;

    prev = func_0200d1d8(entry, entry->engineId, entry->bgLayer, entry->tilemap, entry->tileData, entry->width, entry->height);
    func_0200d858(entry, wrapH, wrapV, entry->unk_18);

    engState = &g_DisplaySettings.engineState[entry->engineId];

    switch (engState->bgSettings[entry->bgLayer].bgMode) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            engState->bgAffines[entry->bgLayer].unk_14 = 1;
            break;
    }

    engState->bgOffsets[entry->bgLayer].hOffset = entry->hOffset;
    engState->bgOffsets[entry->bgLayer].vOffset = entry->vOffset;
    return prev;
}

void func_0200d330(ScreenMapEntry* entry, u16* dest, u16* src, s32 startCol, s32 startRow, s32 numCols, s32 numRows,
                   s32 fastMode) {
    s32 row;
    s32 col;
    s32 rowIdx;

    if (numCols <= 0 || numRows <= 0)
        return;

    if (entry->tilemap == NULL && numCols >= 0x10 && numRows >= 8) {
        fastMode = 1;
    }
    if (fastMode == 1) {
        s32 off = startRow << 5;
        func_02001b44((void*)7, (void*)((u8*)dest + off * 2), (void*)((u8*)src + off * 2), numRows << 6);
        return;
    }

    row = 0;
    do {
        col    = 0;
        rowIdx = startCol + ((startRow + row) << 5);
        if (numCols > 0) {
            do {
                dest[rowIdx + col] = src[rowIdx + col];
                col++;
            } while (col < numCols);
        }
        row++;
    } while (row < numRows);
}

u32 func_0200d3e4(ScreenMapEntry* entry, u32* result, s32 x, s32 y) {
    u32   w;
    u32   h;
    u32   ix;
    u32   iy;
    u32   ix2;
    u32   iy2;
    u32** tileData;

    w  = (u32)entry->width;
    ix = (u32)(x >> 5) % w;
    h  = (u32)entry->height;
    if ((s32)ix < 0)
        ix += w;
    iy = (u32)(y >> 5) % h;
    if ((s32)iy < 0)
        iy += h;
    ix2 = ix + 1;
    if (ix2 >= w)
        ix2 -= w;
    iy2 = iy + 1;

    tileData  = (u32**)entry->tileData;
    result[0] = (u32)tileData[iy * w + ix];
    if (iy2 >= h)
        iy2 -= h;
    result[1] = (u32)tileData[iy * entry->width + ix2];
    result[2] = (u32)tileData[iy2 * entry->width + ix];
    result[3] = (u32)tileData[iy2 * entry->width + ix2];

    if (!(entry->flags & 4)) {
        if (x < 0 || (u32)x >= (u32)(entry->width << 5)) {
            result[0] = (u32)entry->unk_18;
            result[2] = (u32)entry->unk_18;
        }
        if (x + 0x20 < 0 || (u32)(x + 0x20) >= (u32)(entry->width << 5)) {
            result[1] = (u32)entry->unk_18;
            result[3] = (u32)entry->unk_18;
        }
    }
    if (!(entry->flags & 8)) {
        if (y < 0 || (u32)y >= (u32)(entry->height << 5)) {
            result[0] = (u32)entry->unk_18;
            result[1] = (u32)entry->unk_18;
        }
        if (y + 0x20 < 0 || (u32)(y + 0x20) >= (u32)(entry->height << 5)) {
            result[2] = (u32)entry->unk_18;
            result[3] = (u32)entry->unk_18;
        }
    }
    return h;
}

void func_0200d540(ScreenMapEntry* entry, u16* dest, u32 tile0, u32 tile1, u32 tile2, u32 tile3, s32 x, s32 y, s32 numCols,
                   s32 numRows, s32 fastMode) {
    s32  colX;
    s32  rowY;
    u16* destA;
    u16* destB;
    s32  splitCol;
    s32  splitRow;
    s32  leftCols;
    s32  rightCols;
    s32  topRows;
    s32  bottomRows;
    s32  xFlip;

    if (numCols <= 0 || numRows <= 0)
        return;

    colX     = x & 0x1F;
    splitCol = 0x20 - colX;
    xFlip    = (x & 0x20) << 5;
    destA    = dest + xFlip;
    destB    = dest + (xFlip ^ 0x400);

    if (splitCol < numCols) {
        rightCols = numCols - splitCol;
    } else {
        splitCol  = numCols;
        rightCols = 0;
    }

    rowY    = y & 0x1F;
    topRows = 0x20 - rowY;
    if (topRows < numRows) {
        bottomRows = numRows - topRows;
    } else {
        topRows    = numRows;
        bottomRows = 0;
    }

    func_0200d330(entry, destA, (u16*)tile0, colX, rowY, splitCol, topRows, fastMode);
    func_0200d330(entry, destB, (u16*)tile1, 0, rowY, rightCols, topRows, fastMode);
    func_0200d330(entry, destA, (u16*)tile2, colX, 0, splitCol, bottomRows, fastMode);
    func_0200d330(entry, destB, (u16*)tile3, 0, 0, rightCols, bottomRows, fastMode);
}

void func_0200d64c(ScreenMapEntry* entry) {
    s32                 prevH;
    s32                 prevV;
    s32                 curH;
    s32                 curV;
    s32                 diffH;
    s32                 diffV;
    s32                 startH;
    s32                 startV;
    s32                 fast;
    DisplayEngineState* engState;
    u16*                screenBase;
    u16*                dest;
    u32                 tiles[4];
    s32                 bgLayer;

    if (entry == NULL)
        return;

    bgLayer  = entry->bgLayer;
    engState = &g_DisplaySettings.engineState[entry->engineId];

    prevH          = entry->hOffset;
    prevV          = entry->vOffset;
    entry->hOffset = engState->bgOffsets[bgLayer].hOffset;
    entry->vOffset = engState->bgOffsets[bgLayer].vOffset;
    curH           = entry->hOffset >> 0xF;
    curV           = entry->vOffset >> 0xF;

    if (!(entry->flags & 2)) {
        s32 pH = prevH >> 0xF;
        s32 pV = prevV >> 0xF;
        diffH  = curH - pH;
        if (diffH < 0)
            diffH = -diffH;
        diffV = curV - pV;
        if (diffV < 0)
            diffV = -diffV;
    } else {
        curH  = (entry->hOffset >> 0xF) + 0x400;
        curV  = (entry->vOffset >> 0xF) + 0x400;
        diffH = 0x21;
        diffV = 0x19;
    }

    if (diffH == 0 && diffV == 0)
        return;

    if (diffH >= 0x20 || diffV >= 0x18) {
        diffH = 0x21;
        diffV = 0x19;
    }

    {
        u32 scrBase = engState->bgSettings[bgLayer].screenBase;
        s32 scrAddr = (s32)(scrBase << 0xB);
        switch (entry->engineId) {
            case 1:
                screenBase = (u16*)(scrAddr + 0x06200000);
                break;
            case 0:
            default:
                screenBase = (u16*)(scrAddr + (g_DisplaySettings.controls[0].screenBase << 0x10) + 0x06000000);
                break;
        }
    }

    dest = (u16*)entry->tilemap;
    if (dest == NULL) {
        dest = screenBase;
    }

    if (diffH >= 1 && diffV < 0x19) {
        s32 startCol;
        if (!(entry->flags & 2)) {
            s32 pH = prevH >> 0xF;
            if (pH > curH) {
                startCol = curH;
            } else if (pH < curH) {
                startCol = pH + 0x21;
            } else {
                startCol = curH;
            }
        } else {
            startCol = curH;
        }
        fast = (entry->tilemap == NULL && diffV >= 8) ? 1 : 0;
        func_0200d3e4(entry, tiles, startCol, curV);
        func_0200d540(entry, dest, tiles[0], tiles[1], tiles[2], tiles[3], startCol, curV, diffH, 0x19, fast);
    }

    if (diffV >= 1) {
        s32 startRow;
        if (!(entry->flags & 2)) {
            s32 pV = prevV >> 0xF;
            if (pV > curV) {
                startRow = curV;
            } else if (pV < curV) {
                startRow = pV + 0x19;
            } else {
                startRow = curV;
            }
        } else {
            startRow = curV;
        }
        fast = (entry->tilemap == NULL && diffV >= 8) ? 1 : 0;
        func_0200d3e4(entry, tiles, curH, startRow);
        func_0200d540(entry, dest, tiles[0], tiles[1], tiles[2], tiles[3], curH, startRow, 0x21, diffV, fast);
    }

    if (entry->tilemap != NULL) {
        func_02001b44((void*)7, screenBase, (void*)entry->tilemap, 0x1000U);
    }
    entry->flags &= ~2;
}

static void func_0200d858(ScreenMapEntry* entry, s32 wrapH, s32 wrapV, void* defaultTile) {
    if (wrapH == 1) {
        entry->flags |= 4;
    } else {
        entry->flags &= ~4;
    }
    if (wrapV == 1) {
        entry->flags |= 8;
    } else {
        entry->flags &= ~8;
    }
    if (defaultTile == NULL) {
        defaultTile = &data_0205a128;
    }
    entry->unk_18 = defaultTile;
}

void func_0200d898(u32** table, u32 base, u32 cols, u32 rows) {
    u32 row;
    u32 idx;
    u32 col;

    if (rows == 0U)
        return;

    row = 0;
    idx = 0;
    do {
        col = 0;
        if (cols > 0U) {
            u32 rowBase = base + (row << 0xB);
            do {
                table[idx] = (u32*)rowBase;
                rowBase += 0x800;
                idx++;
                col++;
            } while (col < cols);
        }
        row++;
    } while (row < rows);
}

void func_0200d8f0(void) {
    MI_CpuSet(&data_0206b3e8, 0, 0x20);
}

void func_0200d90c(void) {
    for (s32 eng = 0; eng < 2; eng++) {
        for (s32 bg = 0; bg < 4; bg++) {
            ScreenMapEntry* entry = data_0206b3e8[eng].entries[bg];
            if (entry != NULL) {
                func_0200d64c(entry);
            }
        }
    }
}

void func_0200d954(s32 eng, s32 bg) {
    data_0206b3e8[eng].entries[bg] = NULL;
}
