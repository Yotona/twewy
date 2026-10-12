#include "Engine/Core/OamMgr.h"
#include "Engine/Resources/ObjResMgr.h"
#include "SpriteMgr.h"

// Nonmatching: register allocation of the cell-piece pointer and char data load (r2/r12)
void func_ov030_020c5f70(Sprite* sprite) {
    void*         loaded      = sprite->charData->loadedCharData;
    OamCellPiece* cellPieces  = (OamCellPiece*)((u16*)sprite->cellTable + sprite->cellTable[sprite->cellIndex].pieceOffset);
    BOOL          isAnimating = FALSE;

    if (sprite->charData->bitmapIndex != sprite->charData->unk_12) {
        isAnimating = TRUE;
    }
    if (isAnimating != TRUE) {
        if (loaded == sprite->unk34 && cellPieces == sprite->lastCellPieces) {
            return;
        }
    }

    sprite->lastCellPieces = cellPieces;
    if (sprite->bits_3_4 == 1 || sprite->bits_3_4 == 3) {
        ObjResMgr_LoadToVram(g_ObjResourceManagers[2], sprite->charData, sprite->unk34, sprite->lastCellPieces);
    }
}

SpriteFrameInfo* func_ov030_020c6000(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = 0x1000);
}
