#include "Field/TaskHelpers.h"
// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020b7dd8-0x020b87bc; 8 functions.
// Research draft: docs/decomp/ov030/Field_020b7dd8.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Save.h"

extern s32 func_ov030_020af618(void);
extern s32 func_ov030_020b3128(s32*);

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

// One entry of the click-region table: the in-world bounds of a spot on the
// current map, its centre point, and the facing index the resulting menu uses.
// Table coordinates are integer map units, converted to fx32 by shifting 12, and
// records are 0xE bytes.  The entry is picked by (arg5 - 0xC) below; arg5 is the
// FieldExit::region kind, and kinds under 0xC are derived at runtime by the
// switch further down instead of being tabulated here.
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
extern s32              func_ov030_020b7ba8(void);

// Nonmatching: register allocation only — the record reads are in field order
// and every operand matches the target, but retail pins the 6 param pointers one
// reg higher (arg0->r8 descending, ours arg0->r9) and keeps {x1,x2,y2} live in
// r9-r11 while our build keeps {y2,dx,dy} and spills {x1,x2}; param block
// r8-r4 vs r9-r4.  Swept decl-order/precompute-order permutations; split is
// allocation-invariant (USA 79.16%; JP unvalidated).
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

    tbl = data_ov030_020e1650[gSaveData.unk_2458];
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

    tbl = data_ov030_020e1650[gSaveData.unk_2458];
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
