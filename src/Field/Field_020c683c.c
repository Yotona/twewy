#include "Engine/Core/OamMgr.h"

extern OamCellFrame data_02059b94[];

// Nonmatching: instruction scheduling of the attr0/attr1 shifts and index merge
OamCellFrame* func_ov030_020c683c(OamCellPiece* piece) {
    return &data_02059b94[((s32)piece->attr1 >> 14) | (((s32)piece->attr0 >> 14) * 4)];
}
