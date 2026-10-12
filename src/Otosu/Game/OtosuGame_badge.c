#include "Otosu/OtosuGame.h"

static void OtosuGame_badge_AddScore(OtosuGame_badge* self, s32 value);
static s32  OtuVec_DistanceToLine(Vec2p* a, Vec2p* b, Vec2p* c);
static s32  OtuVec_IsNearSegment(Vec2p* a, Vec2p* b, Vec2p* c);
static void OtuVec_ClampLength(Vec2p* v);
static void OtosuGame_badge_Launch(OtosuGame_badge* self, Vec2p* dir, s32 angle);
static u8   OtuBoard_GetTileType(Vec2p* p, OtuBoardLayout* grid);
static s32  OtuBoard_IsClearSpot(Vec2p* self, OtuBoardLayout* grid, TinPinSlammer_Scene* scene);
static void OtosuGame_badge_UpdateButtons(OtosuGame_badge* self);

typedef struct {
    /* 0x00 */ s32             dataType;
    /* 0x04 */ s32             index;
    /* 0x08 */ OtuInputPacket* input;
    /* 0x0C */ OtuBoardLayout* board;
    /* 0x10 */ s32             scene;
    /* 0x14 */ u8*             params;
    /* 0x18 */ u16*            pinID;
    /* 0x1C */ s32             hasLabel;
    /* 0x20 */ s32             chanceTbl;
} OtosuGame_badge_Args; // Size: 0x24

extern u32 gFaders[];

static void OtosuGame_badge_ResetState(OtosuGame_badge* self);

#define OTU_CELL_BITS 0x1FFFF

#define OTU_CELL_SIZE 0x20000

static s32              OtosuGame_badge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_badge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              OtosuGame_badge_AI_UseHand(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_AvoidHole(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_ChaseStunned(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_DodgeNeedle(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_CounterNeedle(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_CounterHammer(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_CounterMeteo(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_NeedleNearest(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_HammerNearest(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_MeteoNearest(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_ChaseNearest(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_Hide(OtosuGame_badge* self);
static void             OtosuGame_badge_AI_SeekHide(OtosuGame_badge* self);
static void             OtosuGame_badge_AI_SeekWarp(OtosuGame_badge* self);
static void             OtosuGame_badge_AI_SeekJumpPad(OtosuGame_badge* self);
static void             OtosuGame_badge_AI_SeekConveyor(OtosuGame_badge* self);
static s32              OtosuGame_badge_AI_Pursue(OtosuGame_badge* self);

static const u8 OtuBoard_SubTiles1[2][2] = {
    {0, 0},
    {0, 1}
};
static const u8 OtuBoard_SubTiles2[2][2] = {
    {0, 0},
    {1, 0}
};
static const u8 OtuBoard_SubTiles3[2][2] = {
    {1, 0},
    {0, 0}
};
static const u8 OtuBoard_SubTiles5[2][2] = {
    {0, 1},
    {1, 1}
};
static const u8 OtuBoard_SubTiles4[2][2] = {
    {0, 1},
    {0, 0}
};
static const u8 OtuBoard_SubTiles6[2][2] = {
    {1, 0},
    {1, 1}
};
static const u8 OtuBoard_SubTiles7[2][2] = {
    {1, 1},
    {1, 0}
};
static const u8 OtuBoard_SubTiles8[2][2] = {
    {1, 1},
    {0, 1}
};
static const u8 OtuBoard_SubTiles9[2][2] = {
    {0, 0},
    {1, 1}
};
static const u8 OtuBoard_SubTiles14[2][2] = {
    {1, 0},
    {1, 0}
};
static const u8 OtuBoard_SubTiles19[2][2] = {
    {1, 1},
    {0, 0}
};
static const u8 OtuBoard_SubTiles24[2][2] = {
    {0, 1},
    {0, 1}
};

static const TaskHandle Tsk_OtosuGame_badge = {"Tsk_OtosuGame_badge", OtosuGame_badge_RunTask, sizeof(OtosuGame_badge)};

static const SpriteAnimation OtosuGame_badge_AnimSpriteB = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = OtosuGame_badge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 6,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static const SpriteAnimation OtosuGame_badge_AnimSpriteC = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = OtosuGame_badge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 5,
};

static const SpriteAnimation OtosuGame_badge_AnimSpriteA = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = OtosuGame_badge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_Badge[0],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static u32 OtosuGame_badge_AIBehaviors[17] = {
    (u32)OtosuGame_badge_AI_UseHand,      (u32)OtosuGame_badge_AI_AvoidHole,     (u32)OtosuGame_badge_AI_ChaseStunned,
    (u32)OtosuGame_badge_AI_DodgeNeedle,  (u32)OtosuGame_badge_AI_CounterNeedle, (u32)OtosuGame_badge_AI_CounterHammer,
    (u32)OtosuGame_badge_AI_CounterMeteo, (u32)OtosuGame_badge_AI_NeedleNearest, (u32)OtosuGame_badge_AI_HammerNearest,
    (u32)OtosuGame_badge_AI_MeteoNearest, (u32)OtosuGame_badge_AI_ChaseNearest,  (u32)OtosuGame_badge_AI_Hide,
    (u32)OtosuGame_badge_AI_SeekHide,     (u32)OtosuGame_badge_AI_SeekWarp,      (u32)OtosuGame_badge_AI_SeekJumpPad,
    (u32)OtosuGame_badge_AI_SeekConveyor, (u32)OtosuGame_badge_AI_Pursue};

static void OtosuGame_badge_AddScore(OtosuGame_badge* self, s32 value) {
    if (*self->pinID < PIN_COUNT) {
        self->score += value;
        if (self->score > 999) {
            self->score = 999;
        }

        OtosuGame_badgecount_SetValue(EasyTask_GetTaskData(self->pool, self->counterId), self->score);
        OtosuGame_point_Show(EasyTask_GetTaskData(self->pool, self->pointIds[self->pointCursor]), value);

        self->pointCursor++;
        if (self->pointCursor >= 2) {
            self->pointCursor = 0;
        }
        OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_BPGET, &self->pos, &self->origin);
    }
}

static s32 OtuVec_DistanceToLine(Vec2p* a, Vec2p* b, Vec2p* c) {
    if (a->x == b->x && a->y == b->y) {
        return 0;
    }

    s32 dx = a->x - b->x;
    s32 dy = a->y - b->y;
    s32 ny = -dy;

    s32 cross = (FX_Mul(dy, a->x) - FX_Mul(dx, a->y)) + (FX_Mul(ny, c->x) + FX_Mul(dx, c->y));
    if (cross < 0) {
        cross = -cross;
    }

    s32 len = FX_Mul(ny, ny) + FX_Mul(dx, dx);

    return FX_Divide(cross, FX_Sqrt(len));
}

static s32 OtuVec_IsNearSegment(Vec2p* a, Vec2p* b, Vec2p* c) {
    Vec2p ab;
    Vec2p ac;
    s32   result = 0;

    OtuVec_Sub(b, a, &ab);
    OtuVec_Sub(c, a, &ac);

    if (OtuVec_Dot(&ab, &ac) >= 0) {
        if (OtuVec_DistanceToLine(a, b, c) <= 0x24000) {
            result = 1;
        }
    } else if (OtuVec_Distance(c, a) <= 0x24000) {
        result = 1;
    } else if (OtuVec_Distance(c, b) <= 0x24000) {
        result = 1;
    }

    return result;
}

static void OtuVec_ClampLength(Vec2p* v) {
    if (OtuVec_Length(v) > 0x8000) {
        OtuVec_Normalize(v, v);
        OtuVec_Scale(0x8000, v, v);
    }
}

static void OtosuGame_badge_Launch(OtosuGame_badge* self, Vec2p* dir, s32 angle) {
    u8  tuneIndex = self->params[*self->pinID].tuneIndex;
    s32 speed     = OtuPinTunes[tuneIndex].launch;

    OtuVec_ScaleAdd(FX_Mul(speed, angle), dir, &self->vel, &self->vel);

    OtuVec_ClampLength(&self->vel);

    if (self->vel.x != 0 || self->vel.y != 0) {
        OtuVec_Normalize(&self->vel, &self->dir);
    }

    OtuBoard_PlaySEAt(self->mode == 0x10 ? SEIDX_SE_BAYBADGE_MOVE_TAME : SEIDX_SE_BAYBADGE_MOVE, &self->pos, &self->origin);
}

static u8 OtuBoard_GetTileType(Vec2p* p, OtuBoardLayout* grid) {
    s32 gx  = p->x >> 12;
    s32 gy  = p->y >> 12;
    s32 row = (gy / 32) * grid->width + (gx / 32);

    if (grid->cells[row][0] != 0) {
        return grid->cells[row][0];
    }

    s32 gx32 = (gx / 16) % 2;
    s32 gy32 = (gy / 16) % 2;

    switch (grid->cells[row][1] - 0x10) {
        case 1:
            return OtuBoard_SubTiles1[gy32][gx32];
        case 2:
            return OtuBoard_SubTiles2[gy32][gx32];
        case 3:
            return OtuBoard_SubTiles3[gy32][gx32];
        case 4:
            return OtuBoard_SubTiles4[gy32][gx32];
        case 5:
            return OtuBoard_SubTiles5[gy32][gx32];
        case 6:
            return OtuBoard_SubTiles6[gy32][gx32];
        case 7:
            return OtuBoard_SubTiles7[gy32][gx32];
        case 8:
            return OtuBoard_SubTiles8[gy32][gx32];
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return OtuBoard_SubTiles9[gy32][gx32];
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
            return OtuBoard_SubTiles14[gy32][gx32];
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            return OtuBoard_SubTiles19[gy32][gx32];
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
            return OtuBoard_SubTiles24[gy32][gx32];
        default:
            return grid->cells[row][0];
    }
}

static s32 OtuBoard_IsClearSpot(Vec2p* self, OtuBoardLayout* grid, TinPinSlammer_Scene* scene) {
    Vec2p probe;
    Vec2p childPos;
    s32   mask = 0x1FFFF;

    switch (OtuBoard_GetTileType(self, grid)) {
        case 0:
        case 2:
            return 0;
    }

    probe = *self;
    probe.x += 0xC000;
    switch (OtuBoard_GetTileType(&probe, grid)) {
        case 2:
            return 0;
    }

    probe = *self;
    probe.x -= 0xC000;
    switch (OtuBoard_GetTileType(&probe, grid)) {
        case 2:
            return 0;
    }

    probe = *self;
    probe.y += 0xC000;
    switch (OtuBoard_GetTileType(&probe, grid)) {
        case 2:
            return 0;
    }

    probe = *self;
    probe.y -= 0xC000;
    switch (OtuBoard_GetTileType(&probe, grid)) {
        case 2:
            return 0;
    }

    probe = *self;
    probe.x += 0x20000;
    probe.y -= 0x20000;
    if (OtuBoard_GetTileType(&probe, grid) == 2) {
        probe.x &= ~mask;
        probe.y |= mask;
        if (OtuVec_Distance(self, &probe) < 0xC000) {
            return 0;
        }
    }

    probe = *self;
    probe.x += 0x20000;
    probe.y += 0x20000;
    if (OtuBoard_GetTileType(&probe, grid) == 2) {
        probe.x &= ~mask;
        probe.y &= ~mask;
        if (OtuVec_Distance(self, &probe) < 0xC000) {
            return 0;
        }
    }

    probe = *self;
    probe.x -= 0x20000;
    probe.y -= 0x20000;
    if (OtuBoard_GetTileType(&probe, grid) == 2) {
        probe.x |= mask;
        probe.y |= mask;
        if (OtuVec_Distance(self, &probe) < 0xC000) {
            return 0;
        }
    }

    probe = *self;
    probe.x -= 0x20000;
    probe.y += 0x20000;
    if (OtuBoard_GetTileType(&probe, grid) == 2) {
        probe.x |= mask;
        probe.y &= mask << 17;
        if (OtuVec_Distance(self, &probe) < 0xC000) {
            return 0;
        }
    }

    s32 i;
    s32 count = OtuBoard_GetObstacleCount(scene);
    for (i = 0; i < count; i++) {
        void* child = OtuBoard_GetObstacle(scene, i);
        OtosuGame_obstacle_GetPos(child, &childPos);
        s32 radius = OtosuGame_obstacle_GetScale(child);
        if (OtuVec_Distance(self, &childPos) < radius + 0xC000) {
            return 0;
        }
    }
    return 1;
}

static void OtosuGame_badge_UpdateButtons(OtosuGame_badge* self) {
    u16 now = self->input->buttons;

    self->pressedButtons = now & (self->prevButtons ^ now);
    self->prevButtons    = self->input->buttons;
}

static void OtosuGame_badge_UpdateTouch(OtosuGame_badge* self) {
    u16 home = (self->input->touching & 1) != 0 ? 1 : 0;

    if ((self->touchFlags ^ home) & home & OTU_TOUCH_HELD) {
        self->touchFlags |= OTU_TOUCH_PRESSED;
    } else {
        self->touchFlags &= ~OTU_TOUCH_PRESSED;
    }

    if ((self->touchFlags ^ home) & self->touchFlags & OTU_TOUCH_HELD) {
        self->touchFlags |= OTU_TOUCH_RELEASED;
    } else {
        self->touchFlags &= ~OTU_TOUCH_RELEASED;
    }

    self->touchFlags &= ~OTU_TOUCH_HELD;
    self->touchFlags |= home;
}

static void OtosuGame_badge_ApplyFriction(Vec2p* vel, s32 shortfall) {
    Vec2p norm;
    Vec2p zero;
    s32   len = OtuVec_Length(vel);
    s32   scale;

    if (len <= 0) {
        return;
    }

    scale  = len - shortfall;
    zero.x = 0;
    zero.y = 0;

    if (scale < 0) {
        scale = 0;
    }

    OtuVec_Normalize(vel, &norm);
    OtuVec_ScaleAdd(scale, &norm, &zero, vel);
}

static void OtosuGame_badge_StartWarp(OtosuGame_badge* self) {
    if (self->phase == 2) {
        return;
    }

    OtosuGame_warp_Start(EasyTask_GetTaskData(self->pool, self->warpId), (void*)&self->pos, 1);
    OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_WARP, &self->pos, &self->origin);

    self->vel.x   = 0;
    self->vel.y   = 0;
    self->step    = 0;
    self->phase   = 2;
    self->subKind = 0;
}

static void OtosuGame_badge_StartFall(OtosuGame_badge* self) {
    if (self->phase == 3) {
        return;
    }

    self->step        = 0;
    self->phase       = 3;
    self->frameBudget = 0x1E;
    self->stun        = 0;
}

static void OtosuGame_badge_KnockOut(OtosuGame_badge* self) {
    if (self->phase == 4) {
        return;
    }

    if (self->partner != NULL && *self->pinID < PIN_COUNT) {
        OtosuGame_badge_AddScore(self->partner, self->stun > 0 ? 5 : 2);
    }
    self->partner = NULL;

    OtuBoard_ForgetPartner(self->scene, self);

    self->stun        = 0;
    self->step        = 0;
    self->phase       = 4;
    self->frameBudget = 0x3C;
}

static void OtosuGame_badge_Respawn(OtosuGame_badge* self) {
    if (self->hasLabel != 0) {
        EasyFade_FadeMainDisplay(FADER_SMOOTH, 0x10, 0x1000);
    }

    self->travel       = 0;
    self->velMag       = 0;
    self->respawnTimer = 0x50;
    self->step         = 0;
    self->phase        = 5;
    self->subKind      = 0;

    {
        s32 x = ((self->board->start[self->index][0] << 5) + 0x10) << 0xC;
        s32 y = ((self->board->start[self->index][1] << 5) + 0x10) << 0xC;

        self->startPos.x = x;
        self->startPos.y = y;
    }
    self->pos   = self->startPos;
    self->vel.x = 0;
    self->vel.y = 0;

    self->dir.x     = 0x1000;
    self->dir.y     = 0;
    self->stun      = 0;
    self->cameraCut = 1;
}

static void OtosuGame_badge_Jump(OtosuGame_badge* self) {
    self->vz   = OtuPhys_JumpVelocity;
    self->step = 0;

    OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_JUMP, &self->pos, &self->origin);
}

// Nonmatching: control flow differs
static void OtosuGame_badge_UpdateMotion(OtosuGame_badge* self) {
    Vec2p          probe;
    Vec2p          norm;
    Vec2p          zero;
    OtuBadgeParam* slot;
    s32            accel;
    s32            tile;
    s32            cellX;
    s32            cellY;
    s32            len;
    s32            hit;
    s32            flip = -1;

    self->vz = self->vz + OtuPhys_Gravity;

    if (self->mode <= 0) {
        s32 rate;

        if (self->height == 0) {
            tile = OtuBoard_GetTileType(&self->pos, self->board);

            switch (tile) {
                case 3:
                    rate = OtuPhys_Friction[2];
                    break;

                case 4:
                    rate = OtuPhys_Friction[1];
                    break;

                default:
                    rate = OtuPhys_Friction[0];
                    break;
            }
        } else {
            rate = OtuPhys_Friction[3];
        }

        slot  = &self->params[self->pinID[0]];
        accel = OtuPinTunes[slot->tuneIndex].accel;

        rate = (s32)(((s64)accel * rate + 0x800) >> 12);

        if (self->phase == 3) {
            rate = rate * 2;
        }

        OtosuGame_badge_ApplyFriction(&self->vel, rate);
    }

    if (self->height == 0) {
        u16 dir;
        s32 cell;

        len = OtuVec_Length(&self->vel);

        dir = FX_Atan2Idx(self->vel.y, self->vel.x) +
              ((s32)(((s64)(self->velMag / 3) * self->params[self->pinID[0]].friction + 0x800) >> 12) >> 12);
        cell = dir >> 4;

        {
            s32 x = (s32)(((s64)len * FX_SinCosTable_[cell * 2 + 1] + 0x800) >> 12);
            s32 y = (s32)(((s64)len * FX_SinCosTable_[cell * 2] + 0x800) >> 12);

            self->vel.x = x;
            self->vel.y = y;
        }
    }

    OtuVec_Add(&self->pos, &self->vel, &self->pos);

    if (self->pos.x < 0x80000) {
        self->pos.x = 0x80000;
    } else {
        s32 limit = ((s32)self->board->width << 5) - 0x80;

        if (self->pos.x >= (limit << 0xC)) {
            self->pos.x = (limit << 0xC) - 1;
        }
    }
    if (self->pos.y < 0x60000) {
        self->pos.y = 0x60000;
    } else {
        s32 limit = ((s32)self->board->height << 5) - 0x60;

        if (self->pos.y >= (limit << 0xC)) {
            self->pos.y = (limit << 0xC) - 1;
        }
    }

    self->height = self->height + self->vz;
    if (self->height > 0) {
        self->vz     = 0;
        self->height = 0;
    }

    if (self->height == 0 && self->phase != 3) {
        s32 trailLen = OtuVec_Length(&self->vel);

        if (trailLen > 0x1000) {
            s32 angle;

            self->trailTimer = self->trailTimer - 1;
            if (self->trailTimer <= 0) {
                angle = FX_Atan2Idx(self->vel.y, self->vel.x);

                OtosuGame_track_Place((void*)EasyTask_GetTaskData(self->pool, self->trackIds[self->trailIndex]), &self->pos,
                                      angle, 1, trailLen);
                self->trailIndex = self->trailIndex + 1;

                OtosuGame_track_Place((void*)EasyTask_GetTaskData(self->pool, self->trackIds[self->trailIndex]), &self->pos,
                                      angle, 0, trailLen);
                self->trailIndex = self->trailIndex + 1;

                if (self->trailIndex >= 0xC) {
                    self->trailIndex = 0;
                }
                self->trailTimer = 5;
            }
        }
    }

    if (self->height == 0) {
        hit = 0;

        if (self->vel.x > 0) {
            OtuVec_Copy(&self->pos, &probe);
            probe.x = probe.x + 0xC000;
            tile    = OtuBoard_GetTileType(&probe, self->board);
            if (tile == 2) {
                self->pos.x = (probe.x & ~OTU_CELL_BITS) - 0xC000;
                self->vel.x = self->vel.x * flip;
                hit         = 1;
            }
        } else if (self->vel.x < 0) {
            OtuVec_Copy(&self->pos, &probe);
            probe.x = probe.x - 0xC000;
            tile    = OtuBoard_GetTileType(&probe, self->board);
            if (tile == 2) {
                self->pos.x = (probe.x | OTU_CELL_BITS) + 0xC000;
                self->vel.x = self->vel.x * flip;
                hit         = 1;
            }
        }

        if (self->vel.y > 0) {
            OtuVec_Copy(&self->pos, &probe);
            probe.y = probe.y + 0xC000;
            tile    = OtuBoard_GetTileType(&probe, self->board);
            if (tile == 2) {
                self->pos.y = (probe.y & ~OTU_CELL_BITS) - 0xC000;
                self->vel.y = self->vel.y * flip;
                hit         = 1;
            }
        } else if (self->vel.y < 0) {
            OtuVec_Copy(&self->pos, &probe);
            probe.y = probe.y - 0xC000;
            tile    = OtuBoard_GetTileType(&probe, self->board);
            if (tile == 2) {
                self->pos.y = (probe.y | OTU_CELL_BITS) + 0xC000;
                self->vel.y = self->vel.y * flip;
                hit         = 1;
            }
        }

        if (hit == 0) {
            if (self->vel.x > 0 || self->vel.y < 0) {
                OtuVec_Copy(&self->pos, &probe);
                probe.x = probe.x + OTU_CELL_SIZE;
                probe.y = probe.y - OTU_CELL_SIZE;
                tile    = OtuBoard_GetTileType(&probe, self->board);
                if (tile == 2) {
                    probe.x = probe.x & ~OTU_CELL_BITS;
                    probe.y = probe.y | OTU_CELL_BITS;
                    len     = OtuVec_Distance(&self->pos, &probe);
                    if (len < 0xC000) {
                        self->pos.x = probe.x - 0xC000;
                        hit         = 1;
                        self->pos.y = probe.y + 0xC000;
                        self->vel.x = self->vel.x * flip;
                        self->vel.y = self->vel.y * flip;
                    }
                }
            }

            if (self->vel.x > 0 || self->vel.y > 0) {
                OtuVec_Copy(&self->pos, &probe);
                probe.x = probe.x + OTU_CELL_SIZE;
                probe.y = probe.y + OTU_CELL_SIZE;
                tile    = OtuBoard_GetTileType(&probe, self->board);
                if (tile == 2) {
                    probe.x = probe.x & ~OTU_CELL_BITS;
                    probe.y = probe.y & ~OTU_CELL_BITS;
                    len     = OtuVec_Distance(&self->pos, &probe);
                    if (len < 0xC000) {
                        self->pos.x = probe.x - 0xC000;
                        hit         = 1;
                        self->pos.y = probe.y - 0xC000;
                        self->vel.x = self->vel.x * flip;
                        self->vel.y = self->vel.y * flip;
                    }
                }
            }

            if (self->vel.x < 0 || self->vel.y < 0) {
                OtuVec_Copy(&self->pos, &probe);
                probe.x = probe.x - OTU_CELL_SIZE;
                probe.y = probe.y - OTU_CELL_SIZE;
                tile    = OtuBoard_GetTileType(&probe, self->board);
                if (tile == 2) {
                    probe.x = probe.x | OTU_CELL_BITS;
                    probe.y = probe.y | OTU_CELL_BITS;
                    len     = OtuVec_Distance(&self->pos, &probe);
                    if (len < 0xC000) {
                        self->pos.x = probe.x + 0xC000;
                        hit         = 1;
                        self->pos.y = probe.y + 0xC000;
                        self->vel.x = self->vel.x * flip;
                        self->vel.y = self->vel.y * flip;
                    }
                }
            }

            if (self->vel.x < 0 || self->vel.y > 0) {
                OtuVec_Copy(&self->pos, &probe);
                probe.x = probe.x - OTU_CELL_SIZE;
                probe.y = probe.y + OTU_CELL_SIZE;
                tile    = OtuBoard_GetTileType(&probe, self->board);
                if (tile == 2) {
                    probe.x = probe.x | OTU_CELL_BITS;
                    probe.y = probe.y & ~OTU_CELL_BITS;
                    len     = OtuVec_Distance(&self->pos, &probe);
                    if (len < 0xC000) {
                        self->pos.x = probe.x + 0xC000;
                        hit         = 1;
                        self->pos.y = probe.y - 0xC000;
                        self->vel.x = self->vel.x * flip;
                        self->vel.y = self->vel.y * flip;
                    }
                }
            }
        }

        if (hit != 0) {
            zero.x = 0;
            zero.y = 0;

            len = OtuVec_Length(&self->vel);
            if (len > 0) {
                OtuVec_Normalize(&self->vel, &norm);
                OtuVec_ScaleAdd((s32)(((s64)len * OtuPhys_WallBounce + 0x800) >> 12), &norm, &zero, &self->vel);
                self->dir = norm;
            }
        }
    }

    if (self->height != 0) {
        return;
    }

    tile = OtuBoard_GetTileType(&self->pos, self->board);

    if (tile == 0) {
        OtosuGame_badge_StartFall(self);
    } else if (tile == 12) {
        self->visible = 0;
    }

    cellX = (self->pos.x >> 0xC) / 32;
    cellY = (self->pos.y >> 0xC) / 32;
    if (self->lastCellX == (cellX >> 5) && self->lastCellY == (cellY >> 5)) {
        return;
    }

    switch (tile) {
        case 5:

            if (self->board->warpCount != 0) {
                OtosuGame_badge_StartWarp(self);
            }
            break;

        case 7:
            OtosuGame_badge_Jump(self);
            break;

        case 8:
            self->vel.x = 0;
            self->vel.y = -OtuPhys_ConveyorSpeed;
            OtuVec_Normalize(&self->vel, &self->dir);
            OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_ACCEL, &self->pos, &self->origin);
            break;

        case 9:
            self->vel.x = OtuPhys_ConveyorSpeed;
            self->vel.y = 0;
            OtuVec_Normalize(&self->vel, &self->dir);
            OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_ACCEL, &self->pos, &self->origin);
            break;

        case 10:
            self->vel.x = 0;
            self->vel.y = OtuPhys_ConveyorSpeed;
            OtuVec_Normalize(&self->vel, &self->dir);
            OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_ACCEL, &self->pos, &self->origin);
            break;

        case 11:
            self->vel.x = -OtuPhys_ConveyorSpeed;
            self->vel.y = 0;
            OtuVec_Normalize(&self->vel, &self->dir);
            OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_ACCEL, &self->pos, &self->origin);
            break;
    }

    self->lastTileType = tile;
    self->lastCellX    = cellX >> 5;
    self->lastCellY    = cellY >> 5;
}

static void OtosuGame_badge_StartNeedle(OtosuGame_badge* self) {
    void* needle;

    if (self->needleUses <= 0) {
        return;
    }

    self->step  = 0;
    self->phase = 6;

    needle = EasyTask_GetTaskData(self->pool, self->needleId);
    OtosuGame_needle_Start(needle, self->params[self->pinID[0]].needleCharge, self->params[self->pinID[0]].needleHold);

    self->needleUses = self->needleUses - 1;
}

static void OtosuGame_badge_StartHammer(OtosuGame_badge* self) {
    void* hammer;

    if (self->hammerUses <= 0) {
        return;
    }

    self->step  = 0;
    self->phase = 7;

    hammer = EasyTask_GetTaskData(self->pool, self->hammerId);
    OtosuGame_hammer_Start(hammer, self->params[self->pinID[0]].hammerRate, self->params[self->pinID[0]].hammerLength,
                           self->params[self->pinID[0]].hammerFrames, self->params[self->pinID[0]].hammerArc);

    self->hammerUses = self->hammerUses - 1;
}

static void OtosuGame_badge_StartMeteo(OtosuGame_badge* self) {
    if (self->meteoUses <= 0) {
        return;
    }

    self->vel.x   = 0;
    self->vel.y   = 0;
    self->phase   = 8;
    self->step    = 0;
    self->subKind = 0;

    self->meteoUses = self->meteoUses - 1;
}

static void OtosuGame_badge_StartHand(OtosuGame_badge* self) {
    Vec2p zero;

    if (self->handUses <= 0) {
        return;
    }

    zero.x = 0;
    zero.y = 0;

    OtuVec_ScaleAdd(OtuPhys_HandDashSpeed, &self->dir, &zero, &self->vel);
    OtuVec_Normalize(&self->vel, &self->dir);

    self->vz          = OtuPhys_HandJumpVelocity;
    self->step        = 0;
    self->phase       = 9;
    self->step        = 0;
    self->frameBudget = 9;

    OtosuGame_hand_Start(EasyTask_GetTaskData(self->pool, self->handId));

    if (self->stun > 0) {
        OtosuGame_badge_AddScore(self, 1);
    }

    self->handUses = self->handUses - 1;
}

static void OtosuGame_badge_HandlePlayerInput(OtosuGame_badge* self) {
    Vec2p fromHome;
    Vec2p fromPos;
    Vec2p mid;
    Vec2p legB;
    Vec2p legA;

    if (self->stun > 0) {
        if (self->touchFlags & OTU_TOUCH_PRESSED) {
            self->stun = self->stun - 2;
            if (self->stun < 0) {
                self->stun = 0;
            }
        }
        self->step = 0;
        return;
    }

    if (self->height != 0) {
        self->step = 0;
        return;
    }

    if (self->pressedButtons & (INPUT_BUTTON_RIGHT | INPUT_BUTTON_A)) {
        self->step = 0;
        OtosuGame_badge_StartNeedle(self);
        return;
    }
    if (self->pressedButtons & (INPUT_BUTTON_LEFT | INPUT_BUTTON_Y)) {
        self->step = 0;
        OtosuGame_badge_StartHammer(self);
        return;
    }
    if (self->pressedButtons & (INPUT_BUTTON_UP | INPUT_BUTTON_X)) {
        self->step = 0;
        OtosuGame_badge_StartMeteo(self);
        return;
    }

    switch (self->step) {
        case 0: {
            if (!(self->touchFlags & OTU_TOUCH_PRESSED)) {
                return;
            }

            self->aimFrames = 0;
            self->step      = 1;

            {
                s32 x = self->input->x << 0xC;
                s32 y = self->input->y << 0xC;

                self->aimStart.x = x;
                self->aimStart.y = y;
            }
            {
                s32 x = self->input->x << 0xC;
                s32 y = self->input->y << 0xC;

                self->aimCur.x = x;
                self->aimCur.y = y;
            }

            break;
        }
        case 1: {
            s32 turn;
            s32 scale;
            s32 reach;
            s32 sign;

            if (self->aimFrames < 0x1E) {
                self->aimFrames = self->aimFrames + 1;
            }

            {
                s32 x = self->input->x << 0xC;
                s32 y = self->input->y << 0xC;

                self->aimCur.x = x;
                self->aimCur.y = y;
            }

            if (!(self->touchFlags & OTU_TOUCH_RELEASED)) {
                return;
            }

            self->step = 0;

            turn = OtuVec_Distance(&self->aimStart, &self->aimCur);
            if (turn <= 0) {
                return;
            }

            OtuVec_Add(&self->aimStart, &self->homeOffset, &fromHome);
            OtuVec_Add(&self->aimCur, &self->homeOffset, &fromPos);

            reach = OtuVec_IsNearSegment(&fromHome, &fromPos, &self->pos);
            if (reach == 0) {
                return;
            }

            OtuVec_Sub(&fromPos, &fromHome, &mid);

            turn = OtuVec_Length(&mid);
            if (turn > 0x50000) {
                turn = 0x50000;
            }
            scale = FX_Divide(turn, 0x50000);

            if (self->aimFrames >= 0x1E) {
                scale      = (s32)(((s64)scale * 0x1800 + 0x800) >> 12);
                self->mode = 0x10;
            }

            OtuVec_Normalize(&mid, &mid);
            OtosuGame_badge_Launch(self, &mid, scale);

            if (self->aimFrames >= 0x1E) {
                return;
            }

            OtuVec_Sub(&fromPos, &fromHome, &legB);
            OtuVec_Sub(&self->pos, &fromHome, &legA);

            reach = OtuVec_DistanceToLine(&fromHome, &fromPos, &self->pos);
            if (reach > 0x24000) {
                reach = 0x24000;
            }
            reach = FX_Divide(reach, 0x24000);

            if (*self->pinID >= PIN_COUNT) {
                return;
            }

            sign = (OtuVec_Cross(&legB, &legA) >= 0) ? OtuPhys_SpinPower : -OtuPhys_SpinPower;

            sign         = sign * scale;
            self->velMag = (s32)(((s64)sign * reach + 0x800) >> 12) * 3;
            break;
        }
    }
}

static void OtosuGame_badge_LaunchToward(OtosuGame_badge* self, Vec2p* dir) {
    Vec2p step;

    OtuVec_Sub(dir, &self->pos, &step);

    if (step.x != 0 || step.y != 0) {
        OtuVec_Normalize(&step, &step);
    } else {
        step.x = 0x1000;
        step.y = 0;
    }

    OtosuGame_badge_Launch(self, &step, 0x1000);

    if (*self->pinID >= PIN_COUNT) {
        return;
    }

    self->velMag = (s32)((RNG_Next(OtuPhys_SpinPower * 2) - OtuPhys_SpinPower) << 0xC) * 3;
}

// Nonmatching: instruction scheduling
static s32 OtosuGame_badge_FindTileNearby(OtosuGame_badge* self, s32 rings, s32 loType, s32 hiType, Vec2p* out) {
    Vec2p at;
    s32   ring;
    s32   span;
    s32   i;
    s32   tile;
    s32   x = self->pos.x - OTU_CELL_SIZE;
    s32   y = self->pos.y - OTU_CELL_SIZE;

    at.x = x;
    at.y = y;

    span = 2;
    for (ring = 1; ring <= rings; ring++) {
        for (i = 0; i < span; i++) {
            tile = OtuBoard_GetTileType(&at, self->board);
            if (tile >= loType && tile <= hiType) {
                *out = at;
                return 1;
            }
            at.x = at.x + OTU_CELL_SIZE;
        }

        for (i = 0; i < span; i++) {
            tile = OtuBoard_GetTileType(&at, self->board);
            if (tile >= loType && tile <= hiType) {
                *out = at;
                return 1;
            }
            at.y = at.y + OTU_CELL_SIZE;
        }

        for (i = 0; i < span; i++) {
            tile = OtuBoard_GetTileType(&at, self->board);
            if (tile >= loType && tile <= hiType) {
                *out = at;
                return 1;
            }
            at.x = at.x - OTU_CELL_SIZE;
        }

        for (i = 0; i < span; i++) {
            tile = OtuBoard_GetTileType(&at, self->board);
            if (tile >= loType && tile <= hiType) {
                *out = at;
                return 1;
            }
            at.y = at.y - OTU_CELL_SIZE;
        }

        at.x = at.x - OTU_CELL_SIZE;
        at.y = at.y - OTU_CELL_SIZE;
        span = span + 2;
    }

    return 0;
}

static s32 OtuBoard_IsSameCell(Vec2p* a, Vec2p* b) {
    if ((a->x & ~OTU_CELL_BITS) != (b->x & ~OTU_CELL_BITS)) {
        return 0;
    }
    if ((a->y & ~OTU_CELL_BITS) == (b->y & ~OTU_CELL_BITS)) {
        return 1;
    }
    return 0;
}

static s32 OtosuGame_badge_AI_SeekTile(OtosuGame_badge* self, s32 which, s32 loType, s32 hiType, s32 kind) {
    Vec2p found;

    if (self->aiBehavior != which) {
        if (self->aiChances[which] < RNG_Next(0x10000)) {
            return 0;
        }

        if (OtosuGame_badge_FindTileNearby(self, 3, loType, hiType, &found) == 0) {
            return 0;
        }

        if (OtuBoard_IsSameCell(&found, &self->aiGoal) != 0) {
            return 0;
        }

        OtosuGame_badge_LaunchToward(self, &found);

        self->aiBehavior = which;
        self->aiTimer    = kind;
        self->aiGoal     = found;
    } else {
        if (OtuBoard_IsSameCell(&self->pos, &self->aiGoal) != 0) {
            self->aiBehavior = 0x11;
        } else {
            OtosuGame_badge_LaunchToward(self, &self->aiGoal);
        }
    }

    return 1;
}

static s32 OtosuGame_badge_AI_UseHand(OtosuGame_badge* self) {
    if (self->aiBehavior != 0 && self->aiChances[0] < RNG_Next(0x10000)) {
        return 0;
    }

    OtosuGame_badge_StartHand(self);
    return 1;
}

static s32 OtosuGame_badge_AI_AvoidHole(OtosuGame_badge* self) {
    Vec2p at;
    s32   tries;
    s32   tile;

    if (self->aiChances[1] < RNG_Next(0x10000)) {
        return 0;
    }

    at    = self->pos;
    tries = 0;

    do {
        OtuVec_Add(&at, &self->vel, &at);
        tile = OtuBoard_GetTileType(&at, self->board);
        if (tile == 0) {
            break;
        }
        tries++;
    } while (tries < 4);

    if (tries >= 4) {
        return 0;
    }

    OtuVec_Sub(&self->pos, &self->vel, &at);
    OtosuGame_badge_LaunchToward(self, &at);
    return 1;
}

static s32 OtosuGame_badge_AI_ChaseStunned(OtosuGame_badge* self) {
    OtosuGame_badge* target;
    s32              alive;
    s32              gap;

    if (self->aiBehavior != 2) {
        if (self->aiChances[2] < RNG_Next(0x10000)) {
            return 0;
        }

        target = OtuBoard_FindNearestStunned(self->pool, self->scene, self->index);
        if (target == NULL) {
            return 0;
        }

        gap = OtuVec_Distance(&self->pos, &target->pos);
        if (gap > 0xC8000) {
            return 0;
        }

        OtosuGame_badge_LaunchToward(self, &target->pos);

        self->aiBehavior  = 2;
        self->aiTimer     = 1;
        self->chaseTarget = target;
    } else {
        target = self->chaseTarget;
        alive  = OtosuGame_badge_CanBeTargeted(target, 0x444);
        if (alive != 0) {
            OtosuGame_badge_LaunchToward(self, &target->pos);
        }
    }

    return 1;
}

static s32 OtosuGame_badge_AI_DodgeNeedle(OtosuGame_badge* self) {
    OtosuGame_badge* target;
    Vec2p            away;
    s32              gap;

    if (self->aiChances[3] < RNG_Next(0x10000)) {
        return 0;
    }

    target = OtuBoard_FindNearestNeedle(self->pool, self->scene, self->index);
    if (target == NULL) {
        return 0;
    }

    gap = OtuVec_Distance(&self->pos, &target->pos);
    if (gap > 0x46000) {
        return 0;
    }

    OtuVec_Sub(&target->pos, &self->pos, &away);
    OtuVec_Sub(&self->pos, &away, &away);

    OtosuGame_badge_LaunchToward(self, &away);
    return 1;
}

static s32 OtosuGame_badge_AI_CounterNeedle(OtosuGame_badge* self) {
    OtosuGame_badge* target;
    s32              speed;
    s32              gap;

    if (self->aiChances[4] < RNG_Next(0x10000)) {
        return 0;
    }

    if (self->hammerUses <= 0) {
        return 0;
    }
    speed = OtuVec_Length(&self->vel);
    if (speed >= 0x3000) {
        return 0;
    }

    target = OtuBoard_FindNearestNeedle(self->pool, self->scene, self->index);
    if (target == NULL) {
        return 0;
    }

    gap = OtuVec_Distance(&self->pos, &target->pos);
    if (gap > 0x46000) {
        return 0;
    }

    OtosuGame_badge_StartHammer(self);
    return 1;
}

static s32 OtosuGame_badge_AI_CounterHammer(OtosuGame_badge* self) {
    OtosuGame_badge* other;

    if (self->aiChances[5] < RNG_Next(0x10000)) {
        return 0;
    }

    if (self->meteoUses <= 0) {
        return 0;
    }

    other = OtuBoard_FindNearestHammer(self->pool, self->scene, self->index);

    if (other == NULL) {
        return 0;
    }

    if (OtuVec_Distance(&self->pos, &other->pos) > 0x46000) {
        return 0;
    }

    OtosuGame_badge_StartMeteo(self);

    return 1;
}

static s32 OtosuGame_badge_AI_CounterMeteo(OtosuGame_badge* self) {
    OtosuGame_badge* other;

    if (self->aiChances[6] < RNG_Next(0x10000)) {
        return 0;
    }

    if (self->needleUses <= 0) {
        return 0;
    }

    if (OtuVec_Length(&self->vel) >= 0x3000) {
        return 0;
    }

    other = OtuBoard_FindNearestMeteo(self->pool, self->scene, self->index);

    if (other == NULL) {
        return 0;
    }

    if (OtuVec_Distance(&self->pos, &other->pos) > 0x46000) {
        return 0;
    }

    OtosuGame_badge_StartNeedle(self);

    return 1;
}

static s32 OtosuGame_badge_AI_NeedleNearest(OtosuGame_badge* self) {
    OtosuGame_badge* other;

    if (self->aiChances[7] < RNG_Next(0x10000)) {
        return 0;
    }

    if (self->needleUses <= 0) {
        return 0;
    }

    if (OtuVec_Length(&self->vel) >= 0x3000) {
        return 0;
    }

    other = OtuBoard_FindNearestTargetable(self->pool, self->scene, self->index);

    if (other == NULL) {
        return 0;
    }

    if (OtuVec_Distance(&self->pos, &other->pos) > 0x32000) {
        return 0;
    }

    OtosuGame_badge_StartNeedle(self);

    return 1;
}

static s32 OtosuGame_badge_AI_HammerNearest(OtosuGame_badge* self) {
    OtosuGame_badge* other;

    if (self->aiChances[8] < RNG_Next(0x10000)) {
        return 0;
    }

    if (self->hammerUses <= 0) {
        return 0;
    }

    if (OtuVec_Length(&self->vel) >= 0x3000) {
        return 0;
    }

    other = OtuBoard_FindNearestTargetable(self->pool, self->scene, self->index);

    if (other == NULL) {
        return 0;
    }

    if (OtuVec_Distance(&self->pos, &other->pos) > 0x64000) {
        return 0;
    }

    OtosuGame_badge_StartHammer(self);

    return 1;
}

static s32 OtosuGame_badge_AI_MeteoNearest(OtosuGame_badge* self) {
    OtosuGame_badge* other;

    if (self->aiChances[9] < RNG_Next(0x10000)) {
        return 0;
    }

    if (self->meteoUses <= 0) {
        return 0;
    }

    other = OtuBoard_FindNearestTargetable(self->pool, self->scene, self->index);

    if (other == NULL) {
        return 0;
    }

    if (OtuVec_Distance(&self->pos, &other->pos) > 0x64000) {
        return 0;
    }

    OtosuGame_badge_StartMeteo(self);

    return 1;
}

static s32 OtosuGame_badge_AI_ChaseNearest(OtosuGame_badge* self) {
    OtosuGame_badge* other;

    if (self->aiBehavior != 0xA) {
        if (self->aiChances[10] < RNG_Next(0x10000)) {
            return 0;
        }

        other = OtuBoard_FindNearestTargetable(self->pool, self->scene, self->index);

        if (other == NULL) {
            return 0;
        }

        if (OtuVec_Distance(&self->pos, &other->pos) > 0xC8000) {
            return 0;
        }

        OtosuGame_badge_LaunchToward(self, &other->pos);

        self->aiBehavior  = 0xA;
        self->aiTimer     = 1;
        self->chaseTarget = other;
    } else {
        other = self->chaseTarget;
        if (OtosuGame_badge_CanBeTargeted(other, 0x444) != 0) {
            OtosuGame_badge_LaunchToward(self, &other->pos);
        }
    }

    return 1;
}

static s32 OtosuGame_badge_AI_Hide(OtosuGame_badge* self) {
    if (self->aiBehavior != 0xB) {
        if (self->aiChances[11] < RNG_Next(0x10000)) {
            return 0;
        }

        if (OtuBoard_GetTileType(&self->pos, self->board) != 0xC) {
            return 0;
        }

        self->aiBehavior = 0xB;
        self->aiTimer    = 0x3C;
    }

    return 1;
}

static void OtosuGame_badge_AI_SeekHide(OtosuGame_badge* self) {
    OtosuGame_badge_AI_SeekTile(self, 0xC, 0xC, 0xC, 1);
}

static void OtosuGame_badge_AI_SeekWarp(OtosuGame_badge* self) {
    OtosuGame_badge_AI_SeekTile(self, 0xD, 5, 5, 1);
}

static void OtosuGame_badge_AI_SeekJumpPad(OtosuGame_badge* self) {
    OtosuGame_badge_AI_SeekTile(self, 0xE, 7, 7, 1);
}

static void OtosuGame_badge_AI_SeekConveyor(OtosuGame_badge* self) {
    OtosuGame_badge_AI_SeekTile(self, 0xF, 8, 0xB, 1);
}

static s32 OtosuGame_badge_AI_Pursue(OtosuGame_badge* self) {
    OtosuGame_badge* other;

    if (self->aiBehavior != 0x10) {
        if (self->aiChances[16] < RNG_Next(0x10000)) {
            return 0;
        }

        other = OtuBoard_FindNearestTargetable(self->pool, self->scene, self->index);

        if (other == NULL) {
            return 0;
        }

        OtosuGame_badge_LaunchToward(self, &other->pos);

        self->aiBehavior  = 0x10;
        self->aiTimer     = 1;
        self->chaseTarget = other;
    } else {
        other = self->chaseTarget;
        if (OtosuGame_badge_CanBeTargeted(other, 0x444) != 0) {
            OtosuGame_badge_LaunchToward(self, &other->pos);
        }
    }

    return 1;
}

static void OtosuGame_badge_RunAI(OtosuGame_badge* arg) {
    OtosuGame_badge* self = (OtosuGame_badge*)arg;
    s32 (**fn)(OtosuGame_badge*);
    s32 i;

    if (self->stun > 0) {
        return;
    }

    if (self->height != 0) {
        return;
    }

    fn = (s32(**)(OtosuGame_badge*))OtosuGame_badge_AIBehaviors;

    for (i = 1; i < 0x11; i++) {
        if (fn[i](self) != 0) {
            return;
        }
    }
}

// Nonmatching: scheduling and register allocation
static void OtosuGame_badge_UpdateWarp(OtosuGame_badge* self) {
    OtosuGame_warp* warp;
    u8              cell[2];
    s32             i;
    s32             x;
    s32             y;

    switch (self->subKind) {
        case 0:
            warp = EasyTask_GetTaskData(self->pool, self->warpId);

            if (OtosuGame_warp_IsActive(warp) != 0) {
                return;
            }

            x       = (self->pos.x >> 0xC) / 32;
            y       = (self->pos.y >> 0xC) / 32;
            cell[0] = x;
            cell[1] = y;

            for (i = 0; i < self->board->warpCount; i++) {
                if (*(u16*)&self->board->warps[i] == *(u16*)cell) {
                    break;
                }
            }

            if (i < self->board->warpCount) {
                x = self->board->warps[i].toX;
                y = self->board->warps[i].toY;

                self->pos.x     = ((x << 5) + 0x10) << 0xC;
                self->pos.y     = ((self->board->warps[i].toY << 5) + 0x10) << 0xC;
                self->cameraCut = 1;
            }

            self->lastCellX = x;
            self->lastCellY = y;

            OtosuGame_warp_Start(warp, &self->pos, 2);

            self->subKind = 1;
            break;

        case 1:
            if (OtosuGame_warp_IsActive(EasyTask_GetTaskData(self->pool, self->warpId)) != 0) {
                return;
            }

            self->phase = 1;
            self->step  = 0;
            break;
    }
}

static void OtosuGame_badge_UpdateFall(OtosuGame_badge* self) {
    if (OtuBoard_GetTileType(&self->pos, self->board) != 0) {
        self->phase         = 1;
        self->step          = 0;
        self->affine.scaleX = 0x1000;
        self->affine.scaleY = 0x1000;
        return;
    }

    if (self->stun <= 0) {
        if (self->input != NULL) {
            if ((self->pressedButtons & (INPUT_BUTTON_DOWN | INPUT_BUTTON_B)) != 0) {
                OtosuGame_badge_StartHand(self);
            }
        } else {
            OtosuGame_badge_AI_UseHand(self);
        }
    }

    self->affine.scaleX = (self->frameBudget << 11) / 30 + 0x800;
    self->affine.scaleY = self->affine.scaleX;

    self->frameBudget = self->frameBudget - 1;

    if (self->frameBudget > 0) {
        return;
    }

    OtosuGame_badge_KnockOut(self);

    OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_DISAPPEAR, &self->pos, &self->origin);

    Sprite_Release(&self->spriteA);

    OtosuGame_dead_Start(EasyTask_GetTaskData(self->pool, self->deadId));
}

static void OtosuGame_badge_UpdateKnockOut(OtosuGame_badge* self) {
    s32 i;

    self->frameBudget = self->frameBudget - 1;

    if (self->frameBudget > 0) {
        return;
    }

    OtosuGame_badge_Respawn(self);

    if (self->pinID[0] < PIN_COUNT) {
        self->pinID = self->pinID + 1;
        OtosuGame_badgecount_ShowPinCount(EasyTask_GetTaskData(self->pool, self->counterId), self->pinID);
    }

    if (self->hasLabel != 0 && self->pinID[0] < PIN_COUNT) {
        OtosuGame_entry_Show(EasyTask_GetTaskData(self->pool, self->entryId), 1,
                             (self->pinID[1] == OTU_NO_PIN) ? TRUE : FALSE);
    }

    if (self->pinID[0] < PIN_COUNT) {
        OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_ALIVE, &self->pos, &self->origin);
    } else {
        self->score = 0;
        OtosuGame_badgecount_SetValue(EasyTask_GetTaskData(self->pool, self->counterId), self->score);
    }

    OtosuGame_badge_ResetState(self);

    for (i = 0; i < 2; i++) {
        OtosuGame_point_Hide(EasyTask_GetTaskData(self->pool, self->pointIds[i]));
    }
}

static void OtosuGame_badge_UpdateRespawn(OtosuGame_badge* arg) {
    OtosuGame_badge* self = (OtosuGame_badge*)arg;

    if (self->hasLabel != 0) {
        EasyFade_FadeMainDisplay(3, 0, 0x1E);
    }

    switch (self->subKind) {
        case 0:

            self->respawnTimer = self->respawnTimer - 1;
            if (self->respawnTimer > 0) {
                return;
            }

            if (self->hasLabel != 0) {
                OtosuGame_entry_Show(EasyTask_GetTaskData(self->pool, self->entryId), 2, 0);
                *(s32*)((u8*)gFaders + 8) = 0x10000;
                EasyFade_FadeMainDisplay(FADER_SMOOTH, 0x10, 0x1000);
            }

            self->respawnTimer = 0x34;
            self->subKind      = 1;
            return;

        case 1:

            self->respawnTimer = self->respawnTimer - 1;
            if (self->respawnTimer > 0) {
                return;
            }

            self->phase = 1;
            self->step  = 0;
            return;
    }
}

static void OtosuGame_badge_UpdateNeedle(OtosuGame_badge* arg) {
    OtosuGame_badge* self = (OtosuGame_badge*)arg;
    if (OtosuGame_needle_IsActive(EasyTask_GetTaskData(self->pool, self->needleId)) != 0) {
        return;
    }

    self->phase = 1;
    self->step  = 0;
}

static void OtosuGame_badge_UpdateHammer(OtosuGame_badge* arg) {
    OtosuGame_badge* self = (OtosuGame_badge*)arg;
    if (OtosuGame_hammer_IsActive(EasyTask_GetTaskData(self->pool, self->hammerId)) != 0) {
        return;
    }

    self->phase = 1;
    self->step  = 0;
}

// Nonmatching: self and the case-4 loop constants land in different registers
static void OtosuGame_badge_UpdateMeteo(OtosuGame_badge* self) {
    switch (self->subKind) {
        default:
            break;

        case 0:
            self->frameBudget = 0x14;
            self->subKind     = self->subKind + 1;
            self->height      = 0;
            OtosuGame_meteo_Start(EasyTask_GetTaskData(self->pool, self->meteoId), 0x14);
            OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_METEOR01, &self->pos, &self->origin);
            return;

        case 1:
            self->height      = self->height - 0xA000;
            self->frameBudget = self->frameBudget - 1;
            if (self->frameBudget > 0) {
                return;
            }
            self->frameBudget = self->params[*self->pinID].meteoHold;
            self->subKind     = self->subKind + 1;
            return;

        case 2: {
            Vec2p aim;
            Vec2p dir;

            if (self->input != NULL) {
                if (self->touchFlags & OTU_TOUCH_HELD) {
                    OtuInputPacket* source = self->input;

                    aim.x = source->x << 0xC;
                    aim.y = source->y << 0xC;
                    OtuVec_Add(&self->homeOffset, &aim, &aim);
                    if (OtuBoard_IsClearSpot(&aim, self->board, self->scene) != 0) {
                        self->pos = aim;
                    }
                }
            } else {
                OtosuGame_badge* cand = OtuBoard_FindNearestTargetable(self->pool, self->scene, self->index);

                if (cand != NULL) {
                    OtuVec_Sub(&cand->pos, &self->pos, &dir);
                    if (OtuVec_Length(&dir) < 0x1E000) {
                        self->frameBudget = 1;
                    } else {
                        OtuVec_Normalize(&dir, &dir);
                        OtuVec_ScaleAdd(0x1800, &dir, &self->pos, &dir);
                        if (OtuBoard_IsClearSpot(&dir, self->board, self->scene) != 0) {
                            self->pos = dir;
                        }
                    }
                }
            }

            self->frameBudget = self->frameBudget - 1;
            if (self->frameBudget > 0 && !(self->touchFlags & OTU_TOUCH_RELEASED)) {
                return;
            }
            self->frameBudget = 0xA;
            self->subKind     = self->subKind + 1;
            return;
        }

        case 3:
            self->frameBudget = self->frameBudget - 1;
            if (self->frameBudget > 0) {
                return;
            }
            OtosuGame_meteo_Impact(EasyTask_GetTaskData(self->pool, self->meteoId));
            self->frameBudget = self->params[*self->pinID].meteoSquash;
            self->subKind     = self->subKind + 1;
            OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_METEOR02, &self->pos, &self->origin);
            return;

        case 4: {
            s32 i = 0;

            self->height      = self->height + -self->height / self->frameBudget;
            self->frameBudget = self->frameBudget - 1;
            if (self->frameBudget > 0) {
                return;
            }
            OtosuGame_meteo_Recover(EasyTask_GetTaskData(self->pool, self->meteoId));

            do {
                s32 step = i * 0x10000;

                OtosuGame_smoke_Start(EasyTask_GetTaskData(self->pool, self->smokeIds[i]), &self->pos,
                                      (u32)((step + ((u32)(step >> 2) >> 0x1D)) << 0xD) >> 0x10, 0x2000, 0xCD, 0x14);
                i = i + 1;
            } while (i < 8);

            OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_METEOR_GROUND, &self->pos, &self->origin);
            self->frameBudget = 0x28;
            self->subKind     = self->subKind + 1;
            return;
        }

        case 5:
            self->frameBudget = self->frameBudget - 1;
            if (self->frameBudget > 0) {
                return;
            }
            self->subKind = self->subKind + 1;
            return;

        case 6:
            self->phase = 1;
            self->step  = 0;
            return;
    }
}

static void OtosuGame_badge_UpdateHand(OtosuGame_badge* self) {
    s32 count;
    s32 rate;
    s32 v;

    count = self->frameBudget;
    if (count > 0) {
        --count;
        self->frameBudget = count;
        if (count != 0) {
            return;
        }

        OtuBoard_SpawnSparks(self->scene, &self->pos);
        OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_TURN02, &self->pos, &self->origin);
        return;
    }

    self->vz = self->vz + OtuPhys_Gravity;

    rate = OtuPinTunes[self->params[self->pinID[0]].tuneIndex].accel;
    OtosuGame_badge_ApplyFriction(&self->vel, FX_Mul(rate, OtuPhys_Friction[3]));
    OtuVec_Add(&self->pos, &self->vel, &self->pos);

    v            = self->height + self->vz;
    self->height = v;
    if (v > 0) {
        self->height = 0;
        self->phase  = 1;
        self->step   = 0;
    }
}

static SpriteFrameInfo* OtosuGame_badge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_badge* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affine, OtuBoard_SpriteSortKey(3, owner->pos.y, owner->height));
}

static void OtosuGame_badge_LoadSpriteA(OtosuGame_badge* self, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_badge_AnimSpriteA;

    anim.owner    = self;
    anim.dataType = (u16)self->dataType;
    anim.posX     = self->pos.x >> 12;
    anim.posY     = self->pos.y >> 12;

    anim.bits_7_9 = 6;

    if (*self->pinID < PIN_COUNT) {
        anim.binIden   = &OtuFile_Badge[0];
        anim.packIndex = *self->pinID + 1;
        anim.unk_1C    = 1;
        anim.unk_20    = 4;
        anim.unk_26    = 2;
        anim.unk_28    = 3;
    } else {
        anim.binIden   = (BinIdentifier*)&OtuFile_CommonObj;
        anim.packIndex = 0xB;
        anim.unk_1C    = 1;
        anim.unk_20    = 4;
        anim.unk_26    = 2;
        anim.unk_28    = 3;
    }

    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_badge_LoadSpriteB(OtosuGame_badge* self, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_badge_AnimSpriteB;

    anim.owner    = self;
    anim.dataType = (u16)self->dataType;
    anim.posX     = self->pos.x >> 12;
    anim.posY     = self->pos.y >> 12;

    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_badge_LoadSpriteC(OtosuGame_badge* self, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_badge_AnimSpriteC;

    anim.owner    = self;
    anim.dataType = (u16)self->dataType;
    anim.posX     = self->pos.x >> 12;
    anim.posY     = self->pos.y >> 12;

    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_badge_ResetState(OtosuGame_badge* self) {
    self->step            = 0;
    self->lastTileType    = 1;
    self->lastCellX       = 0;
    self->lastCellY       = 0;
    self->height          = 0;
    self->vel.x           = 0;
    self->vel.y           = 0;
    self->dir.x           = 0x1000;
    self->dir.y           = 0;
    self->vz              = 0;
    self->travel          = 0;
    self->velMag          = 0;
    self->affine.rotation = 0;
    self->affine.scaleX   = 0x1000;
    self->affine.scaleY   = 0x1000;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;
    self->trailTimer      = 5;
    self->stun            = 0;
    self->aimFrames       = 0;
    self->mode            = 0;
    self->flags           = 0xA2;
    self->partner         = NULL;
    self->aiBehavior      = 0x11;
    self->aiTimer         = 0;
    self->chaseTarget     = NULL;
    self->aiGoal.x        = 0;
    self->aiGoal.y        = 0;

    self->needleUses = self->params[*self->pinID].specialUses[0];
    self->meteoUses  = self->params[*self->pinID].specialUses[1];
    self->hammerUses = self->params[*self->pinID].specialUses[2];
    self->handUses   = self->params[*self->pinID].specialUses[3];

    OtosuGame_badge_LoadSpriteA(self, &self->spriteA);
}

static s32 OtosuGame_badge_Init(TaskPool* pool, Task* task, void* args) {
    OtosuGame_badge_Args* taskArgs = args;
    OtosuGame_badge*      self     = (OtosuGame_badge*)task->data;
    s32                   startX;
    s32                   startY;

    self->dataType    = taskArgs->dataType;
    self->pool        = pool;
    self->index       = taskArgs->index;
    self->input       = taskArgs->input;
    self->board       = taskArgs->board;
    self->scene       = taskArgs->scene;
    self->visible     = 1;
    self->trailIndex  = 0;
    self->pointCursor = 0;
    self->hasLabel    = taskArgs->hasLabel;
    self->score       = 0;
    self->smokeCursor = 0;
    self->params      = (OtuBadgeParam*)taskArgs->params;
    self->pinID       = taskArgs->pinID;
    self->aiChances   = taskArgs->chanceTbl;

    if (self->input != NULL) {
        self->prevButtons = self->input->buttons;
    } else {
        self->prevButtons = 0;
    }

    self->pressedButtons = 0;
    self->touchFlags     = 0;
    self->origin.x       = 0;
    self->origin.y       = 0;
    self->homeOffset.x   = 0;
    self->homeOffset.y   = 0;

    startX      = (self->board->start[self->index][0] << 5) + 0x10;
    startY      = (self->board->start[self->index][1] << 5) + 0x10;
    self->pos.x = startX << 12;
    self->pos.y = startY << 12;

    self->cameraCut = 1;

    OtosuGame_badge_ResetState(self);
    OtosuGame_badge_LoadSpriteB(self, &self->spriteB);
    OtosuGame_badge_LoadSpriteC(self, &self->spriteC);

    return 1;
}

static s32 OtosuGame_badge_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_badge* self = (OtosuGame_badge*)task->data;

    if (self->input != NULL) {
        OtosuGame_badge_UpdateButtons(self);
        OtosuGame_badge_UpdateTouch(self);
    }

    self->visible       = 1;
    self->affine.scaleX = 0x1000;
    self->affine.scaleY = 0x1000;

    if (self->mode > 0) {
        self->mode--;
    }
    if (self->stun > 0) {
        self->stun--;
    }
    if (self->flags > 0) {
        self->flags--;
    }

    switch (self->phase) {
        case 1:
        case 3:
        case 6:
        case 7:
            OtosuGame_badge_UpdateMotion(self);
            break;

        default:
            break;
    }

    self->travel = self->travel + self->velMag;

    if (self->velMag > 0) {
        self->velMag = self->velMag - ((OtuPhys_SpinDecay * 3) << 12);
        if (self->velMag < 0) {
            self->velMag = 0;
        }
    } else if (self->velMag < 0) {
        self->velMag = self->velMag + ((OtuPhys_SpinDecay * 3) << 12);
        if (self->velMag > 0) {
            self->velMag = 0;
        }
    }

    self->affine.rotation = (u32)self->travel * 0x10 / 0x10000;

    if (self->aiTimer > 0) {
        self->aiTimer = self->aiTimer - 1;
        if (self->aiTimer <= 0) {
            self->aiBehavior = 0x11;
        }
    }

    switch (self->phase) {
        case 1:
            if (self->input != NULL) {
                OtosuGame_badge_HandlePlayerInput(self);
            } else {
                OtosuGame_badge_RunAI(self);
            }
            break;

        case 2:
            OtosuGame_badge_UpdateWarp(self);
            break;

        case 3:
            OtosuGame_badge_UpdateFall(self);
            break;

        case 4:
            OtosuGame_badge_UpdateKnockOut(self);
            break;

        case 5:
            OtosuGame_badge_UpdateRespawn(self);
            break;

        case 6:
            OtosuGame_badge_UpdateNeedle(self);
            break;

        case 7:
            OtosuGame_badge_UpdateHammer(self);
            break;

        case 8:
            OtosuGame_badge_UpdateMeteo(self);
            break;

        case 9:
            OtosuGame_badge_UpdateHand(self);
            break;

        default:
            break;
    }

    Sprite_Update(&self->spriteA);
    Sprite_Update(&self->spriteB);
    Sprite_Update(&self->spriteC);

    return 1;
}

static void OtosuGame_badge_RenderSprites(OtosuGame_badge* self) {
    if (self->mode > 0) {
        self->spriteC.posX = (self->pos.x - self->origin.x) >> 12;
        self->spriteC.posY = ((self->pos.y + self->height) - self->origin.y) >> 12;

        Sprite_RenderFrame(&self->spriteC);
    }

    self->spriteA.posX = (self->pos.x - self->origin.x) >> 12;
    self->spriteA.posY = ((self->pos.y + self->height) - self->origin.y) >> 12;

    Sprite_RenderFrame(&self->spriteA);
}

static s32 OtosuGame_badge_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_badge* self = (OtosuGame_badge*)task->data;

    if (self->visible == 0) {
        if (self->hasLabel != 0) {
            self->spriteB.posX = (self->pos.x - self->origin.x) >> 12;
            self->spriteB.posY = ((self->pos.y + self->height) - self->origin.y) >> 12;

            Sprite_RenderFrame(&self->spriteB);
        }

        return 1;
    }

    if (self->flags & 2) {
        return 1;
    }

    switch (self->phase) {
        case 1:
        case 3:
        case 4:
        case 5:
        case 7:
        case 9:
            OtosuGame_badge_RenderSprites(self);
            break;

        case 8:
            if (self->subKind == 0 || self->subKind == 1 || self->subKind == 6) {
                OtosuGame_badge_RenderSprites(self);
            }
            break;

        default:
            break;
    }

    return 1;
}

static s32 OtosuGame_badge_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_badge* self = (OtosuGame_badge*)task->data;
    s32              i;

    EasyTask_DeleteTask(pool, self->warpId);

    for (i = 0; i < 8; i++) {
        EasyTask_DeleteTask(pool, self->smokeIds[i]);
    }

    EasyTask_DeleteTask(pool, self->deadId);

    if (self->hasLabel != 0) {
        EasyTask_DeleteTask(pool, self->entryId);
    }

    for (i = 0; i < 2; i++) {
        EasyTask_DeleteTask(pool, self->pointIds[i]);
    }

    for (i = 0; i < 12; i++) {
        EasyTask_DeleteTask(pool, self->trackIds[i]);
    }

    EasyTask_DeleteTask(pool, self->counterId);
    EasyTask_DeleteTask(pool, self->radarId);
    EasyTask_DeleteTask(pool, self->handId);
    EasyTask_DeleteTask(pool, self->needleId);
    EasyTask_DeleteTask(pool, self->hammerId);
    EasyTask_DeleteTask(pool, self->meteoId);
    EasyTask_DeleteTask(pool, self->markerId);
    EasyTask_DeleteTask(pool, self->piyoId);
    EasyTask_DeleteTask(pool, self->shadowId);

    Sprite_Release(&self->spriteA);
    Sprite_Release(&self->spriteB);
    Sprite_Release(&self->spriteC);

    return 1;
}

static s32 OtosuGame_badge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_badge_Init,
        .update     = OtosuGame_badge_Update,
        .render     = OtosuGame_badge_Render,
        .cleanup    = OtosuGame_badge_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_badge_CreateTask(TaskPool* pool, s32 dataType, s32 index, OtuInputPacket* input, void* board, void* scene,
                               void* params, void* deck, s32 hasLabel, void* chanceTbl) {
    OtosuGame_badge_Args args;
    s32                  id;
    OtosuGame_badge*     self;
    s32                  i;

    args.dataType  = dataType;
    args.index     = index;
    args.input     = input;
    args.board     = board;
    args.scene     = scene;
    args.params    = (u8*)params;
    args.pinID     = (u16*)deck;
    args.hasLabel  = hasLabel;
    args.chanceTbl = chanceTbl;

    id   = EasyTask_CreateTask(pool, &Tsk_OtosuGame_badge, NULL, 0, NULL, &args);
    self = (OtosuGame_badge*)EasyTask_GetTaskData(pool, id);

    self->shadowId = OtosuGame_shadow_CreateTask(pool, dataType, id);
    self->piyoId   = OtosuGame_piyo_CreateTask(pool, dataType, id);
    self->markerId = OtosuGame_marker_CreateTask(pool, dataType, id);
    self->meteoId  = OtosuGame_meteo_CreateTask(pool, dataType, id);
    self->hammerId = OtosuGame_hammer_CreateTask(pool, dataType, id);
    self->needleId = OtosuGame_needle_CreateTask(pool, dataType, id);
    self->handId   = OtosuGame_hand_CreateTask(pool, dataType, id);

    self->radarId   = OtosuGame_badgeradar_CreateTask(pool, dataType, id, index, board, hasLabel);
    self->counterId = OtosuGame_badgecount_CreateTask(pool, dataType, id, index, hasLabel);
    OtosuGame_badgecount_ShowPinCount(EasyTask_GetTaskData(pool, self->counterId), (u16*)deck);

    for (i = 0; i < 12; i++) {
        self->trackIds[i] = OtosuGame_track_CreateTask(pool, dataType, id);
    }

    for (i = 0; i < 2; i++) {
        self->pointIds[i] = OtosuGame_point_CreateTask(pool, dataType, id);
    }

    if (self->hasLabel != 0) {
        self->entryId = OtosuGame_entry_CreateTask(pool, dataType);
    }

    self->deadId = OtosuGame_dead_CreateTask(pool, dataType, id);

    for (i = 0; i < 8; i++) {
        self->smokeIds[i] = OtosuGame_smoke_CreateTask(pool, dataType, id);
    }

    self->warpId = OtosuGame_warp_CreateTask(pool, dataType, id);

    if (self->hasLabel != 0 && *self->pinID < PIN_COUNT) {
        OtosuGame_entry_Show(EasyTask_GetTaskData(self->pool, self->entryId), 1, self->pinID[1] == OTU_NO_PIN ? 1 : 0);
    }

    OtosuGame_badge_Respawn(self);

    return id;
}

static s32 OtosuGame_badge_WillCollide(Vec2p* posA, Vec2p* velA, s32 reachA, Vec2p* posB, Vec2p* velB, s32 reachB) {
    Vec2p dir;
    s32   dotA;
    s32   dotB;

    if (velA->x == 0 && velA->y == 0 && velB->x == 0 && velB->y == 0) {
        return 0;
    }

    if (OtuVec_Distance(posA, posB) <= reachA + reachB) {
        OtuVec_Sub(posB, posA, &dir);
        dotA = OtuVec_Dot(&dir, velA);

        OtuVec_Sub(posA, posB, &dir);
        dotB = OtuVec_Dot(&dir, velB);

        return (dotA > 0 || dotB > 0) ? 1 : 0;
    }

    return 0;
}

static void OtosuGame_badge_ApplyImpulse(Vec2p* dir, s32 speed, s32 scaleA, s32 scaleB, OtosuGame_badge* other) {
    s32 mag = FX_Mul(speed, scaleA);

    OtuVec_ScaleAdd(FX_Mul(mag, scaleB), dir, &other->vel, &other->vel);
    OtuVec_ClampLength(&other->vel);

    if (&other->vel != NULL || &other->vel.y != NULL) {
        OtuVec_Normalize(&other->vel, &other->dir);
    }
}

static void OtosuGame_badge_GetImpact(Vec2p* posA, Vec2p* velA, Vec2p* posB, Vec2p* velB, Vec2p* out, s32* score) {
    Vec2p dir;
    s32   dot;

    *score = OtuVec_Distance(velB, velA);
    OtuVec_Sub(velB, velA, &dir);

    if (out->x != 0 || out->y != 0) {
        OtuVec_Normalize(&dir, &dir);
    }

    OtuVec_Sub(posA, posB, out);

    if (out->x != 0 || out->y != 0) {
        OtuVec_Normalize(out, out);
    } else {
        out->x = 0x1000;
        out->y = 0;
    }

    dot = OtuVec_Dot(&dir, out);
    if (dot < 0) {
        dot = -dot;
    }

    *score = FX_Mul(*score, dot);
}

// Nonmatching: instruction selection differs
static void OtosuGame_badge_Bounce(OtosuGame_badge* self, OtosuGame_badge* other) {
    Vec2p dir;
    s32   score;
    s32   scale;
    s32   weight;

    OtosuGame_badge_GetImpact(&self->pos, &self->vel, &other->pos, &other->vel, &dir, &score);

    scale  = (self->stun > 0) ? OtuPhys_StunnedImpact : 0x1000;
    weight = OtuPinTunes[other->params[*other->pinID].tuneIndex].weight;
    if (other->mode > 0) {
        weight *= 2;
    }
    OtosuGame_badge_ApplyImpulse(&dir, FX_Mul(score, scale), OtuPinTunes[self->params[*self->pinID].tuneIndex].power, weight,
                                 self);
    self->partner = other;

    scale  = (other->stun > 0) ? OtuPhys_StunnedImpact : 0x1000;
    weight = OtuPinTunes[self->params[*self->pinID].tuneIndex].weight;
    if (self->mode > 0) {
        weight *= 2;
    }
    OtosuGame_badge_ApplyImpulse(&dir, -FX_Mul(score, scale), OtuPinTunes[other->params[*other->pinID].tuneIndex].power,
                                 weight, other);
    other->partner = self;
}

s32 OtosuGame_badge_Collide(OtosuGame_badge* self, OtosuGame_badge* other) {
    if (self->flags > 0) {
        return 0;
    }
    switch (self->phase) {
        case 1:
        case 6:
        case 7:
            if (self->height != 0) {
                return 0;
            }
            break;
        default:
            return 0;
    }
    if (other->flags > 0) {
        return 0;
    }
    switch (other->phase) {
        case 1:
        case 6:
        case 7:
            if (other->height != 0) {
                return 0;
            }
            break;
        default:
            return 0;
    }

    if (OtosuGame_badge_WillCollide(&self->pos, &self->vel, 0xC000, &other->pos, &other->vel, 0xC000) == 0) {
        return 0;
    }

    OtosuGame_badge_Bounce(self, other);

    return 1;
}

s32 OtosuGame_badge_Separate(OtosuGame_badge* self, OtosuGame_badge* other) {
    Vec2p dir;
    s32   dist;
    s32   gap;

    if (self->flags > 0) {
        return 0;
    }

    switch (self->phase) {
        case 0:
        case 2:
        case 5:
        case 8:
            return 0;

        default:
            break;
    }

    if (self->height != 0) {
        return 0;
    }

    if (other->flags > 0) {
        return 0;
    }

    switch (other->phase) {
        case 0:
        case 2:
        case 5:
        case 8:
            return 0;

        default:
            break;
    }

    if (other->height != 0) {
        return 0;
    }

    dist = OtuVec_Distance(&self->pos, &other->pos);

    if (dist <= 0x18000) {
        gap = 0x18000 - dist;

        OtuVec_Sub(&self->pos, &other->pos, &dir);

        if (dir.x == 0 && dir.y == 0) {
            dir.x = 0x1000;
            dir.y = 0;
        } else {
            OtuVec_Normalize(&dir, &dir);
        }

        OtuVec_ScaleAdd(gap / 2, &dir, &self->pos, &self->pos);
        OtuVec_ScaleAdd((-gap) / 2, &dir, &other->pos, &other->pos);

        return 1;
    }
    return 0;
}

s32 OtosuGame_badge_CollideObstacle(OtosuGame_badge* self, OtosuGame_obstacle* obstacle) {
    Vec2p other;
    Vec2p dir;
    s32   radius;
    s32   len;

    switch (self->phase) {
        case 1:
        case 6:
        case 7:
        case 9:
            break;

        default:
            return 0;
    }

    OtosuGame_obstacle_GetPos(obstacle, &other);
    radius = OtosuGame_obstacle_GetScale(obstacle);

    if (OtuVec_Distance(&self->pos, &other) < radius + 0xC000) {
        OtuVec_Sub(&self->pos, &other, &dir);

        if (dir.x != 0 || dir.y != 0) {
            OtuVec_Normalize(&dir, &dir);
        } else {
            dir.x = 0x1000;
            dir.y = 0;
        }

        OtuVec_ScaleAdd(radius + 0xC000, &dir, &other, &self->pos);

        OtuVec_Sub(&other, &self->pos, &dir);

        if (dir.x != 0 || dir.y != 0) {
            OtuVec_Normalize(&dir, &dir);
        } else {
            dir.x = 0x1000;
            dir.y = 0;
        }

        len = OtuVec_Dot(&self->vel, &dir);
        OtuVec_ScaleAdd(-(len * 2), &dir, &self->vel, &self->vel);

        if (self->vel.x != 0 || self->vel.y != 0) {
            OtuVec_Normalize(&self->vel, &dir);

            len = OtuVec_Length(&self->vel);
            OtuVec_Scale(FX_Mul(len, OtuPhys_WallBounce), &dir, &self->vel);

            self->dir = dir;
        }

        OtosuGame_obstacle_PlayHit(obstacle);

        return 1;
    }

    return 0;
}

void OtosuGame_badge_GetDir(OtosuGame_badge* self, Vec2p* out) {
    *out = self->dir;
}

void OtosuGame_badge_GetPos(OtosuGame_badge* task, Vec2p* out) {
    *out = task->pos;
}

s32 OtosuGame_badge_GetHeight(OtosuGame_badge* task) {
    return (task)->height;
}

void OtosuGame_badge_GetCameraPos(OtosuGame_badge* self, Vec2p* out) {
    if (self->phase == 8) {
        switch (self->subKind) {
            case 2: {
                s32   x       = self->pos.x - 0x80000;
                s32   y       = self->pos.y - 0x60000;
                Vec2p scratch = {x, y};

                OtuVec_Sub(&scratch, &self->homeOffset, &scratch);

                if (OtuVec_Length(&scratch) >= 0x400) {
                    OtuVec_Normalize(&scratch, &scratch);
                    OtuVec_ScaleAdd(0x400, &scratch, &self->homeOffset, out);
                    return;
                }

                out->x = self->pos.x - 0x80000;
                out->y = self->pos.y - 0x60000;
                return;
            }

            case 3: {
                s32   scale   = FX_Divide(0x1000, self->frameBudget << 12);
                s32   x       = self->pos.x - 0x80000;
                s32   y       = self->pos.y - 0x60000;
                Vec2p scratch = {x, y};
                OtuVec_Sub(&scratch, &self->homeOffset, &scratch);
                OtuVec_ScaleAdd(scale, &scratch, &self->homeOffset, out);
                return;
            }

            default:
                out->x = self->pos.x - 0x80000;
                out->y = self->pos.y - 0x60000;
                return;
        }
    }

    out->x = self->pos.x - 0x80000;
    out->y = self->pos.y - 0x60000;
}

void OtosuGame_badge_SetOrigin(OtosuGame_badge* self, Vec2p* origin) {
    self->origin = *origin;
}

void OtosuGame_badge_GetOrigin(OtosuGame_badge* self, Vec2p* out) {
    *out = self->origin;
}

void OtosuGame_badge_SetHomeOffset(OtosuGame_badge* self, s32 x, s32 y) {
    self->homeOffset.x = x;
    self->homeOffset.y = y;
}

void OtosuGame_badge_GetHomeOffset(OtosuGame_badge* self, Vec2p* out) {
    *out = self->homeOffset;
}

s32 OtosuGame_badge_IsShown(OtosuGame_badge* self) {
    switch (self->phase) {
        case 0:
        case 2:
        case 3:
        case 4:
            return 0;

        case 1:
        default:
            break;
    }

    return self->visible;
}

s32 OtosuGame_badge_GetShadowScale(OtosuGame_badge* self) {
    s32 scale = 0x1000;

    if (self->phase == 8) {
        switch (self->subKind) {
            case 1: {
                s32 cursor = self->frameBudget;

                if (cursor < 0xA) {
                    scale = FX_Divide(cursor << 12, 0xA000);
                }
                break;
            }

            case 4: {
                u32 size = self->params[*self->pinID].meteoSquash;

                if (self->frameBudget < (s32)(size >> 1)) {
                    scale = 0x1000 - FX_Divide(self->frameBudget << 12, (s32)(size >> 1) << 12);
                }
                break;
            }

            default:
                break;
        }
    }

    return scale;
}

s32 OtosuGame_badge_GetMeteoAimState(OtosuGame_badge* self) {
    s32 r = 0;

    if (self->phase == 8) {
        switch (self->subKind) {
            case 2:
                r = 2;
                break;

            case 3:
                r = 3;
                break;
        }
    }

    return r;
}

s32 OtosuGame_badge_IsUsingMeteo(OtosuGame_badge* task) {
    s32 found = task->phase;
    s32 r     = 0;

    if (found == 8) {
        r = 1;
    }

    return r;
}

s32 OtosuGame_badge_IsUsingHammer(OtosuGame_badge* task) {
    s32 found = task->phase;
    s32 r     = 0;

    if (found == 7) {
        r = 1;
    }

    return r;
}

s32 OtosuGame_badge_GetBackAngle(OtosuGame_badge* self) {
    return (u16)FX_Atan2IdxNoProto(-self->dir.y, -self->dir.x);
}

s32 OtosuGame_badge_IsUsingNeedle(OtosuGame_badge* task) {
    s32 found = task->phase;
    s32 r     = 0;

    if (found == 6) {
        r = 1;
    }

    return r;
}

s32 OtosuGame_badge_IsUsingHand(OtosuGame_badge* task) {
    s32 found = task->phase;
    s32 r     = 0;

    if (found == 9) {
        r = 1;
    }

    return r;
}

s32 OtosuGame_badge_UpdateHits(TaskPool* pool, OtosuGame_badge* self) {
    s32 claimed = 0;
    s32 ret;

    switch (self->phase) {
        case 6:
            ret            = OtosuGame_needle_GetHitRecord(EasyTask_GetTaskData(pool, self->needleId), self->hits);
            self->hitCount = ret;
            if (ret > 0) {
                claimed = 1;
            }
            break;

        case 7:
            ret            = OtosuGame_hammer_GetHitRecord(EasyTask_GetTaskData(pool, self->hammerId), self->hits);
            self->hitCount = ret;
            if (ret > 0) {
                claimed = 1;
            }
            break;

        case 8:
            switch (self->subKind) {
                case 5:
                    if (self->frameBudget == 0x27) {
                        claimed = 1;
                        OtosuGame_meteo_GetHitRecord(EasyTask_GetTaskData(pool, self->meteoId), self->hits);
                        self->hitCount = claimed;
                    }
                    break;
            }
            break;

        default:
            break;
    }
    return claimed;
}

// Nonmatching: scheduling and register naming
void OtosuGame_badge_TakeHit(OtosuGame_badge* other, OtosuGame_badge* self) {
    if (self->height != 0) {
        return;
    }
    if (self->flags > 0) {
        return;
    }

    switch (self->phase) {
        default:
        case 1:
        case 6:
        case 7:
            break;

        case 8:
            switch (self->subKind) {
                default:
                case 0:
                    break;

                case 1:
                case 2:
                case 3:
                case 4:
                    return;
            }
            break;

        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 9:
            return;
    }

    if (other->hitCount <= 0) {
        return;
    }
    {
        s32 i = 0;

        do {
            s32 limit = other->hits[i].scale + 0xC000;

            if (OtuVec_Distance(&self->pos, &other->hits[i].pos) >= limit) {
                goto next;
            }

            switch (other->phase) {
                case 6: {
                    OtosuGame_needle_EndHold(EasyTask_GetTaskData(other->pool, other->needleId));
                    OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_NEEDLE_HIT, &other->pos, &other->origin);

                    if (self->stun <= 0) {
                        if (*self->pinID < PIN_COUNT) {
                            OtosuGame_badge_AddScore(other, 1);
                        }
                    }

                    self->vel.x = self->pos.x - other->pos.x;
                    self->vel.y = self->pos.y - other->pos.y;

                    if (self->vel.x != 0 || self->vel.y != 0) {
                        OtuVec_Normalize(&self->vel, &self->vel);
                    } else {
                        self->vel.x = 0x1000;
                        self->vel.y = 0;
                    }
                    break;
                }

                case 7: {
                    void* cand   = EasyTask_GetTaskData(other->pool, other->hammerId);
                    u16   cursor = (u16)(OtosuGame_hammer_GetAngle(cand) - 0x4000);
                    s32   pair   = (cursor >> 4) * 2;

                    OtosuGame_hammer_EndSwing(cand);

                    self->vel.x = FX_SinCosTable_[pair + 1];
                    self->vel.y = FX_SinCosTable_[pair];

                    if (self->phase == 6) {
                        OtosuGame_needle_Shatter(EasyTask_GetTaskData(self->pool, self->needleId));
                    }
                    OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_HUMMER_HIT, &other->pos, &other->origin);

                    if (self->stun <= 0) {
                        if (*self->pinID < PIN_COUNT) {
                            OtosuGame_badge_AddScore(other, 1);
                        }
                    }
                    break;
                }

                case 8: {
                    if (self->phase == 7) {
                        OtosuGame_hammer_Shatter(EasyTask_GetTaskData(self->pool, self->hammerId));
                    }
                    OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_METEOR_HIT, &other->pos, &other->origin);

                    if (self->stun <= 0) {
                        if (*self->pinID < PIN_COUNT) {
                            OtosuGame_badge_AddScore(other, 1);
                        }
                    }

                    self->vel.x = self->pos.x - other->pos.x;
                    self->vel.y = self->pos.y - other->pos.y;

                    if (self->vel.x != 0 || self->vel.y != 0) {
                        OtuVec_Normalize(&self->vel, &self->vel);
                    } else {
                        self->vel.x = 0x1000;
                        self->vel.y = 0;
                    }
                    break;
                }

                default:
                    break;
            }

            if (self->stun <= 0) {
                self->stun  = self->params[*self->pinID].stunFrames;
                self->phase = 1;
                self->step  = 0;
                OtosuGame_piyo_Start(EasyTask_GetTaskData(self->pool, self->piyoId), self->stun);
            }
            {
                Vec2p  start;
                Vec2p* base   = &self->vel;
                s32    factor = OtuPinTunes[self->params[*self->pinID].tuneIndex].power;
                s32    scale;

                start.x = 0;
                scale   = (s32)(((s64)OtuPhys_Knockback * factor + 0x800) >> 0xC);
                start.y = 0;

                self->dir.x = self->vel.x;
                self->dir.y = self->vel.y;

                OtuVec_ScaleAdd(scale, base, &start, base);
                other->partner = self;
                self->partner  = other;
            }

        next:
            i = i + 1;
        } while (i < other->hitCount);
    }
}

s32 OtosuGame_badge_IsStunned(OtosuGame_badge* task) {
    return task->stun > 0;
}

s32 OtosuGame_badge_GetStun(OtosuGame_badge* task) {
    return (task)->stun;
}

s16 OtosuGame_badge_GetNeedleUses(OtosuGame_badge* task) {
    return (task)->needleUses;
}

s16 OtosuGame_badge_GetMeteoUses(OtosuGame_badge* task) {
    return (task)->meteoUses;
}

s16 OtosuGame_badge_GetHammerUses(OtosuGame_badge* task) {
    return (task)->hammerUses;
}

s16 OtosuGame_badge_GetHandUses(OtosuGame_badge* task) {
    return (task)->handUses;
}

s32 OtosuGame_badge_IsAiming(OtosuGame_badge* self) {
    s32 r = 0;

    if (self->height == 0 && self->phase == 1 && self->step == 1) {
        if (OtuVec_Distance(&self->aimStart, &self->aimCur) >= 0x10000) {
            r = 1;
        }
    }

    return r;
}

void OtosuGame_badge_GetAimLine(void* pin, Vec2p* a, Vec2p* b) {
    OtosuGame_badge* self = (OtosuGame_badge*)pin;

    *a = self->aimStart;
    *b = self->aimCur;
}

s32 OtosuGame_badge_IsCharged(OtosuGame_badge* task) {
    return (task)->aimFrames >= 0x1E;
}

s32 OtosuGame_badge_IsTargetable(void* task, u32 hiddenChance) {
    OtosuGame_badge* self = (OtosuGame_badge*)task;
    s32              kind = self->phase;
    s32              r    = 0;

    switch (kind) {
        case 1:
        default:
            if (OtuBoard_GetTileType((Vec2p*)&self->pos.x, self->board) != 0xC) {
                r = 1;
            } else if (RNG_Next(0x10000) < hiddenChance) {
                r = 1;
            }
            break;

        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
            break;
    }

    return r;
}

s32 OtosuGame_badge_CanBeTargeted(OtosuGame_badge* task, s32 which) {
    OtosuGame_badge* self = (OtosuGame_badge*)task;

    if (OtuBoard_GetTileType((Vec2p*)&self->pos.x, self->board) == 0) {
        return 0;
    }
    if (*self->pinID == OTU_NO_PIN) {
        return 0;
    }

    return OtosuGame_badge_IsTargetable(task, which);
}

s32 OtosuGame_badge_TakeCameraCut(OtosuGame_badge* self) {
    s32 value = self->cameraCut;

    self->cameraCut = 0;
    return value;
}

s32 OtosuGame_badge_HasPin(OtosuGame_badge* self) {
    return *self->pinID < PIN_COUNT;
}

void OtosuGame_badge_AddSurvivalBonus(OtosuGame_badge* self) {
    OtosuGame_badge_AddScore(self, 0xA);
}

s32 OtosuGame_badge_GetScore(OtosuGame_badge* task) {
    return (task)->score;
}

void OtosuGame_badge_ResetAI(OtosuGame_badge* task) {
    (task)->aiBehavior = 0x11;
}

void OtosuGame_badge_SpawnSmoke(OtosuGame_badge* self, Vec2p* at, s32 selector) {
    OtosuGame_smoke_Start(EasyTask_GetTaskData(self->pool, self->smokeIds[self->smokeCursor]), at, selector, 0x1000, 0x66, 6);

    self->smokeCursor = self->smokeCursor + 1;
    if (self->smokeCursor >= 8) {
        self->smokeCursor = 0;
    }
}

s32 OtosuGame_badge_IsOutOfPins(OtosuGame_badge* self) {
    return *self->pinID == OTU_NO_PIN;
}

void OtosuGame_badge_HideEntry(OtosuGame_badge* self) {
    if (self->hasLabel != 0) {
        OtosuGame_entry_Hide(EasyTask_GetTaskData(self->pool, self->entryId));
    }
}

void OtosuGame_badge_ForgetPartner(OtosuGame_badge* self, OtosuGame_badge* other) {
    if (self->partner == other) {
        self->partner = NULL;
    }
}

void OtosuGame_badge_HidePoints(OtosuGame_badge* self) {
    s32 i;

    for (i = 0; i < 2; i++) {
        OtosuGame_point_Hide(EasyTask_GetTaskData(self->pool, self->pointIds[i]));
    }
}
