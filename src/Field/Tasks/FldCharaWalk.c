#include "Field/Tasks/FldCharaWalk.h"
#include "Field/FieldTypes.h"
#include "Field/Tasks/FldChara.h"
#include "Field/Tasks/Fld_Shinigami.h"
#include "SpriteMgr.h"

static const u16 FldCharaWalk_AngleCells[10] = {2, 1, 0, 1, 2, 3, 4, 3, 2, 0};

u16 FldCharaWalk_AngleToCell(s32 angle) {
    return FldCharaWalk_AngleCells[((angle >> 0xC) & 1) + (angle >> 0xD)];
}

u16 FldCharaWalk_GetLoopFrame(Sprite* sprite) {
    if (SpriteMgr_IsFrameFinished(sprite) != 0) {
        return (u16)(sprite->currentFrame - sprite->loopFrame + 1);
    }
    return (u16)(sprite->currentFrame - sprite->loopFrame);
}

s32 FldCharaWalk_MoveWithCollision(FieldPos* pos, s32 angle, s32 dist) {
    s32 x = pos->x;
    s32 y = pos->y;

    FldChara_MoveAlongAngle(&x, &y, angle, dist);
    return Fld_Shinigami_MoveWithCollision(pos, x, y);
}
