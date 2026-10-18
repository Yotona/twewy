#include "Field/Tasks/FldOamCell.h"
#include "Engine/Core/OamMgr.h"

extern OamCellFrame data_02059b94[];

// Nonmatching: instruction scheduling of the attr0/attr1 shifts and index merge
OamCellFrame* FldOamCell_GetFrame(OamCellPiece* piece) {
    return &data_02059b94[((s32)piece->attr1 >> 14) | (((s32)piece->attr0 >> 14) * 4)];
}

const u32 FldOamCell_AttrMasks[8] = {
    0x00000000, 0xFFFFFF00, 0xFFFFFE00, 0x00FFFF00, 0xFE00001F, 0x03E001FF, 0x03FFFC00, 0x1FFFFC00,
};
