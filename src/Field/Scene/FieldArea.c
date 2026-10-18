#include "Field/Events.h"
#include "Field/Ov030Data.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include <nitro/fx/fx_matrix.h>

extern void func_02027c94(s32);
extern s32  func_ov030_020aeaf4(void);
extern s32  func_ov030_020af618(void);
extern s32  func_ov030_020b3128(s32*);
extern u16  func_ov030_020c5430(void);
extern void FO3DViewer_Update(s32, s32);
extern void FO3DViewer_SetOffset(s32, s32);
extern void FO3DViewer_Destroy(void);

extern const FieldSlope data_ov030_020e0fb4[3];
extern const FieldSlope data_ov030_020e14f8[6];

s32 func_ov030_020b7d74(s32 value);

void func_ov030_020b7a00(void* arg1, FieldPos origin) {
    FO3DViewer_SetOffset(gSaveData.fieldPos.x - origin.x, gSaveData.fieldPos.y - origin.y);
    Mtx_OrthoW(-origin.y, -0xC0000 - origin.y, origin.x, origin.x + 0x100000, -0x400000, 0x400000, 0x20000, &data_02076a4c);
    data_02076a44.unk_FC &= ~0x50;
    func_02028a44();
    FO3DViewer_Update(origin.x + 0x80000, -(origin.y + 0x60000));
}

void func_ov030_020b7aac(s32* arg0) {
    FO3DViewer_Destroy();
    func_02027c94(*arg0);
    func_0202688c();
}

void func_ov030_020b7ac8(void) {
    s32 i = 0;
    do {
        gSaveData.areaPopulation[i] = data_ov030_020e1d64[i].unk_1C;
        i++;
    } while (i < 44);
}

s16 func_ov030_020b7b00(void) {
    return data_ov030_020e1d64[gSaveData.fieldArea].unk_18;
}
s16 func_ov030_020b7b20(void) {
    return data_ov030_020e1d64[gSaveData.fieldArea].unk_02;
}
s16 func_ov030_020b7b40(void) {
    return data_ov030_020e1d64[gSaveData.fieldArea].unk_04;
}
s16 func_ov030_020b7b60(void) {
    return data_ov030_020e1d64[gSaveData.fieldArea].unk_1A;
}
s32 func_ov030_020b7b80(void) {
    return data_ov030_020e1d64[gSaveData.fieldArea].unk_00 / 2;
}
s32 func_ov030_020b7ba8(void) {
    s16 origin = func_ov030_020b7b40();
    s32 extent = func_ov030_020b7b60();
    return origin + extent / 2;
}
s16 func_ov030_020b7bc4(void) {
    s16 area   = gSaveData.fieldArea;
    s16 second = data_ov030_020e1d64[area].unk_18;
    s16 first  = data_ov030_020e1d64[area].unk_00;
    if (first <= second) {
        first = second;
    }
    return first;
}
s16 func_ov030_020b7bf8(void) {
    return data_ov030_020e1d64[gSaveData.fieldArea].unk_10;
}
s32 func_ov030_020b7c18(void) {
    return data_ov030_020e1d64[gSaveData.fieldArea].unk_14;
}
u16 func_ov030_020b7c34(s32 area) {
    if (area >= 70) {
        return func_ov030_020c5430();
    }
    return data_ov030_020e1d64[area].unk_1E;
}

void func_ov030_020b7c58(s32 unused, s32* out1, s32* out2, s32* out3) {
    *out1 = data_ov030_020e1d64[gSaveData.fieldArea].unk_08;
    *out2 = data_ov030_020e1d64[gSaveData.fieldArea].unk_0C;
    *out3 = *out2 - *out1;
}

s32 func_ov030_020b7c9c(s32 value, s32 unusedY) {
    s32 area = gSaveData.fieldArea;
    if (area == 0) {
        return func_ov030_020b7d74(value);
    }
    if (area == 1) {
        if (value < 0x118000) {
            return 0x78000;
        }
        if (value < 0x167000) {
            return 0x66000;
        }
        if (value < 0x1BE000) {
            return 0x75000;
        }
        return 0x86000;
    }
    return func_ov030_020b7b40() << 12;
}

s32 func_ov030_020b7d04(s32 value) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (value + 0x100000 < data_ov030_020e0fb4[i].limit) {
            break;
        }
    }
    return data_ov030_020e0fb4[i].base + (s32)(((s64)(value + 0x100000) * data_ov030_020e0fb4[i].slope + 0x800) >> 12) -
           0xC1000;
}

s32 func_ov030_020b7d74(s32 value) {
    u32 i;

    for (i = 0; i < 5; i++) {
        if (value < data_ov030_020e14f8[i].limit) {
            break;
        }
    }
    return data_ov030_020e14f8[i].base + (s32)(((s64)value * data_ov030_020e14f8[i].slope + 0x800) >> 12);
}

s32 func_ov030_020b7dd8(s32 arg0) {
    s32 value;

    if (func_ov030_020af618() == 0) {
        return 0;
    }
    if (func_ov030_020b3128(&value) == 0) {
        return 0;
    }
    if (arg0 == value) {
        return 1;
    }
    arg0 += 1;
    if (arg0 >= 8) {
        arg0 = 0;
    }
    if (arg0 == value) {
        return 1;
    }
    arg0 -= 2;
    if (arg0 < 0) {
        arg0 = 7;
    }
    return arg0 == value;
}

s32 func_ov030_020b7e5c(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 x = gSaveData.fieldPos.x;
    s32 y = gSaveData.fieldPos.y;

    if (x >= arg0 && y >= arg2) {
        if (x <= arg1 && y <= arg3) {
            return 1;
        }
    }
    return 0;
}

// One entry of the click-region table
typedef struct {
    /* 0x00 */ s16 x0;
    /* 0x02 */ s16 y0;
    /* 0x04 */ s16 x1;
    /* 0x06 */ s16 y1;
    /* 0x08 */ s16 cx;
    /* 0x0A */ s16 cy;
    /* 0x0C */ s16 dir;
} Ov030ClickRegion;

extern Ov030ClickRegion data_ov030_020e1878[];

// Nonmatching
void func_ov030_020b7e94(s32* arg0, s32* arg1, s32* arg2, s32* arg3, s32* arg4, s32 arg5, s32* arg6, s32* arg7) {
    s32 y1;
    s32 dx;
    s32 dy;
    s32 x1;
    s32 x2;
    s32 y2;
    s32 idx;

    if (arg5 >= 0xC) {
        idx   = arg5 - 0xC;
        *arg0 = data_ov030_020e1878[idx].x0 << 12;
        *arg2 = data_ov030_020e1878[idx].y0 << 12;
        *arg1 = data_ov030_020e1878[idx].x1 << 12;
        *arg3 = data_ov030_020e1878[idx].y1 << 12;
        *arg6 = data_ov030_020e1878[idx].cx << 12;
        *arg4 = data_ov030_020e1878[idx].dir;
        *arg7 = data_ov030_020e1878[idx].cy << 12;
        return;
    }
    x1 = (func_ov030_020b7b80() - func_ov030_020b7b00() / 2) << 12;
    x2 = (func_ov030_020b7b80() + func_ov030_020b7b00() / 2) << 12;
    y1 = func_ov030_020b7b40() << 12;
    y2 = (func_ov030_020b7b40() + func_ov030_020b7b60()) << 12;
    dx = func_ov030_020b7b00() / 3 << 12;
    dy = func_ov030_020b7b60() / 3 << 12;
    switch (arg5) {
        case 0: {
            s32 w;
            *arg0 = x1;
            w     = y1 - 0xC000;
            *arg1 = x1 + dx;
            *arg2 = w;
            *arg3 = w + 0xC000;
            *arg4 = 4;
            break;
        }
        case 1: {
            s32 v;
            s32 w;
            v     = (func_ov030_020b7b80() << 12) - dx / 2;
            *arg0 = v;
            w     = y1 - 0xC000;
            *arg1 = v + dx;
            *arg2 = w;
            *arg3 = w + 0xC000;
            *arg4 = 4;
            break;
        }
        case 2: {
            s32 w;
            *arg1 = x2;
            w     = y1 - 0xC000;
            *arg0 = x2 - dx;
            *arg2 = w;
            *arg3 = w + 0xC000;
            *arg4 = 4;
            break;
        }
        case 3:
            *arg0 = x1;
            *arg1 = x1 + dx;
            *arg3 = y2;
            *arg2 = y2 - 0x18000;
            *arg4 = 0;
            break;
        case 4: {
            s32 v;
            v     = (func_ov030_020b7b80() << 12) - dx / 2;
            *arg0 = v;
            *arg1 = v + dx;
            *arg3 = y2;
            *arg2 = y2 - 0x18000;
            *arg4 = 0;
            break;
        }
        case 5:
            *arg1 = x2;
            *arg0 = x2 - dx;
            *arg3 = y2;
            *arg2 = y2 - 0x18000;
            *arg4 = 0;
            break;
        case 6:
            *arg1 = x2;
            *arg0 = x2 - 0x18000;
            *arg2 = y1;
            *arg3 = y1 + dy;
            *arg4 = 2;
            break;
        case 7: {
            s32 v;
            *arg1 = x2;
            *arg0 = x2 - 0x18000;
            v     = (func_ov030_020b7ba8() << 12) - dy / 2;
            *arg2 = v;
            *arg3 = v + dy;
            *arg4 = 2;
            break;
        }
        case 8:
            *arg1 = x2;
            *arg0 = x2 - 0x18000;
            *arg3 = y2;
            *arg2 = y2 - dy;
            *arg4 = 2;
            break;
        case 9:
            *arg2 = y1;
            *arg3 = y1 + dy;
            *arg0 = x1;
            *arg1 = x1 + 0x18000;
            *arg4 = 6;
            break;
        case 10: {
            s32 v;
            v     = (func_ov030_020b7ba8() << 12) - dy / 2;
            *arg2 = v;
            *arg3 = v + dy;
            *arg0 = x1;
            *arg1 = x1 + 0x18000;
            *arg4 = 6;
            break;
        }
        case 11:
            *arg3 = y2;
            *arg2 = y2 - dy;
            *arg0 = x1;
            *arg1 = x1 + 0x18000;
            *arg4 = 6;
            break;
        case 0x66:
            *arg4 = 8;
            break;
    }
    *arg6 = (*arg0 + *arg1) / 2;
    *arg7 = (*arg2 + *arg3) / 2;
}

// Exit table of one area.
typedef struct {
    /* 0x00 */ s16        count;
    /* 0x02 */ s16        unk_02;
    /* 0x04 */ FieldExit* exits;
} Ov030AreaTable;

extern Ov030AreaTable data_ov030_020e1650[];

s32 func_ov030_020b82dc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    if (arg4 == 1) {
        arg2 -= 0x10000;
        arg0 -= 0x10000;
    } else if (arg4 == 0) {
        arg2 -= 0x10000;
    } else if (arg4 == 7) {
        arg2 -= 0x10000;
        arg1 += 0x10000;
    } else if (arg4 == 6) {
        arg1 += 0x10000;
    } else if (arg4 == 5) {
        arg3 += 0x10000;
        arg1 += 0x10000;
    } else if (arg4 == 4) {
        arg3 += 0x10000;
    } else if (arg4 == 3) {
        arg3 += 0x10000;
        arg0 -= 0x10000;
    } else if (arg4 == 2) {
        arg0 -= 0x10000;
    }
    return func_ov030_020b7e5c(arg0, arg1, arg2, arg3);
}

FieldExit* func_ov030_020b8358(s32 arg0, FieldPos pos, s32 arg3, s32* arg4) {
    Ov030AreaTable tbl;
    s32            bx0;
    s32            bx1;
    s32            by0;
    s32            by1;
    s32            cx;
    s32            cy;
    s32            dir;
    s16            i;

    tbl   = data_ov030_020e1650[arg0];
    i     = 0;
    *arg4 = 0;
    if (tbl.exits == NULL) {
        return NULL;
    }
    for (; i < tbl.count; i++) {
        func_ov030_020b7e94(&bx0, &bx1, &by0, &by1, &dir, tbl.exits[i].region, &cx, &cy);
        if (func_ov030_020b7e5c(bx0, bx1, by0, by1) != 0) {
            *arg4 = 1;
            if (gSaveData.exitStates[tbl.exits[i].exitId] >= 4) {
                if (arg3 != 0 || func_0202599c(7) != 0) {
                    return &tbl.exits[i];
                }
                if (func_ov030_020b7dd8(dir) != 0) {
                    return &tbl.exits[i];
                }
            }
        } else if (func_ov030_020b82dc(bx0, bx1, by0, by1, dir) != 0) {
            *arg4 = 1;
        }
    }
    return NULL;
}

FieldExit* func_ov030_020b84a4(s32 arg0, s32* arg1, s32* arg2) {
    Ov030AreaTable tbl;
    s32            bx0;
    s32            bx1;
    s32            by0;
    s32            by1;
    s32            cx;
    s32            cy;
    s32            dir;
    s16            i;

    tbl = data_ov030_020e1650[arg0];
    if (tbl.exits == NULL) {
        return NULL;
    }
    for (i = 0; i < tbl.count; i++) {
        func_ov030_020b7e94(&bx0, &bx1, &by0, &by1, &dir, tbl.exits[i].region, &cx, &cy);
        if (func_ov030_020b7e5c(bx0, bx1, by0, by1) != 0) {
            s32 id = tbl.exits[i].exitId;
            s16 v  = gSaveData.exitStates[id];

            if (v >= 4) {
                return &tbl.exits[i];
            }
            if (id < 0x38) {
                if (v > 1) {
                    *arg1 = 0;
                } else {
                    *arg1 = 1;
                    *arg2 = dir;
                }
            }
        }
    }
    return NULL;
}

FieldExit* func_ov030_020b85a8(s32 arg0, s32* arg1) {
    Ov030AreaTable tbl;
    s32            bx0;
    s32            bx1;
    s32            by0;
    s32            by1;
    s32            cx;
    s32            cy;
    s16            i;

    tbl = data_ov030_020e1650[gSaveData.fieldArea];
    if (tbl.exits == NULL) {
        return NULL;
    }
    for (i = 0; i < tbl.count; i++) {
        if (arg0 == tbl.exits[i].exitId) {
            func_ov030_020b7e94(&bx0, &bx1, &by0, &by1, arg1, tbl.exits[i].region, &cx, &cy);
            if (arg0 >= 0x38) {
                if (func_ov030_020b7e5c(bx0, bx1, by0, by1) != 0) {
                    return &tbl.exits[i];
                }
            } else if (func_ov030_020b82dc(bx0, bx1, by0, by1, *arg1) != 0) {
                return &tbl.exits[i];
            }
        }
    }
    return NULL;
}

s32 func_ov030_020b86bc(FieldPos* arg0, s16* arg1, s32* arg2, s32 arg3) {
    Ov030AreaTable tbl;
    s32            bx0;
    s32            bx1;
    s32            by0;
    s32            by1;
    s32            dir;
    s16            v;

    tbl = data_ov030_020e1650[gSaveData.fieldArea];
    if (tbl.exits == NULL) {
        return 0;
    }
    if (tbl.count <= arg3) {
        return 0;
    }
    v = gSaveData.exitStates[tbl.exits[arg3].exitId];
    if (v == 0 || v == 3) {
        return 1;
    }
    *arg2 = tbl.exits[arg3].destArea;
    func_ov030_020b7e94(&bx0, &bx1, &by0, &by1, &dir, tbl.exits[arg3].region, &arg0->x, &arg0->y);
    if (dir < 2) {
        *arg1 = 7;
    } else if (dir < 4) {
        *arg1 = 2;
    } else if (dir < 6) {
        *arg1 = 6;
    } else {
        *arg1 = 1;
    }
    return 2;
}

void func_ov030_020b87bc(FieldExit* exit) {
    s32 out0;
    s32 out1;
    s32 out2;
    s32 out3;
    s32 direction;

    if (exit->destArea == 37 && gSaveData.fieldArea == 24) {
        gSaveData.fieldArea = func_ov030_020aeaf4();
    } else {
        gSaveData.fieldArea = exit->destArea;
    }

    func_ov030_020b7e94(&out0, &out1, &out2, &out3, &direction, exit->destRegion, &gSaveData.fieldPos.x,
                        &gSaveData.fieldPos.y);

    if (direction == 2) {
        gSaveData.fieldPos.x -= 0x10000;
    } else if (direction == 6) {
        gSaveData.fieldPos.x += 0x10000;
    } else if (direction == 0) {
        gSaveData.fieldPos.y -= 0x10000;
    } else if (direction == 4) {
        gSaveData.fieldPos.y += 0x10000;
    }

    if (gSaveData.fieldArea == 0 && gSaveData.fieldPos.x < 0x80000) {
        gSaveData.fieldPos.x = 0x18000;
        gSaveData.fieldPos.y = 0x100000;
    }
}

void func_ov030_020b88e8(FieldExit* exit) {
    func_ov030_020b87bc(exit);
    func_ov030_020b7c58(gSaveData.fieldArea, &gSaveData.unk_248C, &gSaveData.unk_2490, &gSaveData.unk_2494);
}
