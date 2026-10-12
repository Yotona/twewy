#include "Otosu/OtosuGame.h"

#define OTU_ANGLE_INDEX(a) (((s32)((a) >> 4)) * 2)

static s32              OtosuGame_hammer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_hammer_GetFrameInfoSpriteA(Sprite* sprite, s32 arg, s32 mode);
static SpriteFrameInfo* OtosuGame_hammer_GetFrameInfoSpriteB(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_hammer = {"Tsk_OtosuGame_hammer", OtosuGame_hammer_RunTask, sizeof(OtosuGame_hammer)};

static const OtuFrame4 OtosuGame_hammer_RetractScale[4] = {
    {15, 0x4000},
    { 3, 0x3800},
    { 2, 0x2CCD},
    { 2, 0x1CCD},
};

static const OtuFrame6 OtosuGame_hammer_WindupTrail[5] = {
    {5,    0x0, 0x0},
    {5, 0x1A4F, 0x0},
    {5, 0x238E, 0x0},
    {6, 0x24FA, 0x0},
    {6, 0x238E, 0x0},
};

static const OtuFrame6 OtosuGame_hammer_RetractSpin[5] = {
    {5, 0x1000,   0x0},
    {8, 0x1333,   0x3},
    {3,  0xCCD,  -0x3},
    {3,  0x99A,  -0x7},
    {3,  0x19A, -0x10},
};

static const OtuFrame6 OtosuGame_hammer_ExtendSpin[6] = {
    {15,    0x0, 0x0},
    { 1,   0xCD, 0x0},
    { 1,  0x4CD, 0x0},
    { 1,  0x800, 0x0},
    { 3, 0x1333, 0x1},
    { 1, 0x1000, 0x0},
};

static const OtuFrame4 OtosuGame_hammer_ExtendScale[8] = {
    {2,  0x800},
    {1, 0x119A},
    {1, 0x14CD},
    {2, 0x1666},
    {2, 0x3CCD},
    {2, 0x4000},
    {3, 0x3CCD},
    {9, 0x4000},
};

static const OtuFrame6 OtosuGame_hammer_SwingTrail[7] = {
    {4, -0x5B0,     0x0},
    {3,    0x0,     0x0},
    {2, -0x16C, -0x1000},
    {1,  0x16C,  0x1000},
    {1, -0x2D8, -0x2000},
    {2,    0x0,     0x0},
    {3, -0x16C, -0x1000},
};

static const SpriteAnimation OtosuGame_hammer_AnimSpriteA = {
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
    .frameInfoCallback = OtosuGame_hammer_GetFrameInfoSpriteA,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 9,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 2,
};

static const SpriteAnimation OtosuGame_hammer_AnimSpriteB = {
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
    .frameInfoCallback = OtosuGame_hammer_GetFrameInfoSpriteB,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 9,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static void OtosuGame_hammer_InitScaleCursor(OtuCursor* c, OtuFrame4* table, s16 count) {
    c->table      = table;
    c->index      = 0;
    c->count      = count;
    c->framesLeft = c->table[c->index].duration + 1;
}

static s32 OtosuGame_hammer_StepScaleCursor(OtuCursor* c) {
    s16 index;

    if (c->index >= c->count) {
        return 0;
    }

    c->framesLeft = c->framesLeft - 1;

    if (c->framesLeft <= 0) {
        c->index = c->index + 1;
        index    = c->index;

        if (index >= c->count) {
            return 0;
        }

        c->framesLeft = c->table[index].duration;
    }
    return 1;
}

// Size: 0x4

static s16 OtosuGame_hammer_GetScaleValue(OtuCursor* c) {
    if (c->index >= c->count) {
        return 0;
    }

    return c->table[c->index].value;
}

static void OtosuGame_hammer_InitSpinCursor(OtuCursor6* c, OtuFrame6* table, s16 count) {
    c->table      = table;
    c->index      = 0;
    c->count      = count;
    c->framesLeft = c->table[c->index].duration + 1;
}

static s32 OtosuGame_hammer_StepSpinCursor(OtuCursor6* c) {
    s16 index;

    if (c->index >= c->count) {
        return 0;
    }

    c->framesLeft = c->framesLeft - 1;

    if (c->framesLeft <= 0) {
        c->index = c->index + 1;
        index    = c->index;

        if (index >= c->count) {
            return 0;
        }

        c->framesLeft = c->table[index].duration;
    }
    return 1;
}

static s16 OtosuGame_hammer_GetSpinValue(OtuCursor6* c) {
    if (c->index >= c->count) {
        return 0;
    }

    return c->table[c->index].value;
}

static s32 OtosuGame_hammer_GetSpinScale(OtuCursor6* c) {
    if (c->index >= c->count) {
        return 0;
    }

    return c->table[c->index].scale << 12;
}

static void OtosuGame_hammer_InitTrailCursor(OtuCursor6* c, OtuFrame6* table, s16 count) {
    c->table      = table;
    c->index      = 0;
    c->count      = count;
    c->framesLeft = c->table[c->index].duration + 1;
}

static s32 OtosuGame_hammer_StepTrailCursor(OtuCursor6* c) {
    s16 index;

    if (c->index >= c->count) {
        return 0;
    }

    c->framesLeft = c->framesLeft - 1;

    if (c->framesLeft <= 0) {
        c->index = c->index + 1;
        index    = c->index;

        if (index >= c->count) {
            return 0;
        }

        c->framesLeft = c->table[index].duration;
    }
    return 1;
}

static u16 OtosuGame_hammer_GetTrailValue(OtuCursor6* c) {
    if (c->index >= c->count) {
        return 0;
    }

    return c->table[c->index].value;
}

static SpriteFrameInfo* OtosuGame_hammer_GetFrameInfoSpriteA(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_hammer* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affineA, OtuBoard_SpriteSortKey(4, owner->posA.y, 0));
}

static SpriteFrameInfo* OtosuGame_hammer_GetFrameInfoSpriteB(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_hammer* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affineB, OtuBoard_SpriteSortKey(2, owner->posB.y, 0));
}

static void OtosuGame_hammer_LoadSpriteA(OtosuGame_hammer* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_hammer_AnimSpriteA;

    anim.owner    = self;
    anim.dataType = (u16)args->dataType;
    anim.posX     = self->posA.x >> 12;
    anim.posY     = self->posA.y >> 12;
    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_hammer_LoadSpriteB(OtosuGame_hammer* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_hammer_AnimSpriteB;

    anim.owner    = self;
    anim.dataType = (u16)args->dataType;
    anim.posX     = self->posB.x >> 12;
    anim.posY     = self->posB.y >> 12;
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_hammer_Init(TaskPool* pool, Task* task, void* args) {
    OtuPinSpriteArgs* taskArgs = args;
    OtosuGame_hammer* self     = task->data;

    self->pool = pool;

    self->live  = 0;
    self->pinId = taskArgs->pinId;

    self->origin.x = 0;
    self->origin.y = 0;
    self->posA.x   = 0;
    self->posA.y   = 0;
    self->posB.x   = 0;
    self->posB.y   = 0;
    self->pinPos.x = 0;
    self->pinPos.y = 0;

    self->rate0       = 0;
    self->scale0      = 0;
    self->totalFrames = 0;
    self->angleBase   = 0;

    self->affineA.rotation = 0;
    self->affineA.scaleX   = 0x1000;
    self->affineA.scaleY   = 0x1000;
    self->affineA.unk_0C   = 0;
    self->affineA.unk_0E   = 0;
    self->affineB.rotation = 0;
    self->affineB.scaleX   = 0x1000;
    self->affineB.scaleY   = 0x1000;
    self->affineB.unk_0C   = 0;
    self->affineB.unk_0E   = 0;

    self->state = 0;
    OtosuGame_hammer_LoadSpriteA(self, &self->spriteA, taskArgs);
    OtosuGame_hammer_LoadSpriteB(self, &self->spriteB, taskArgs);
    return 1;
}

// Nonmatching: control flow differs
static s32 OtosuGame_hammer_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_badge*  pin;
    OtosuGame_hammer* self = (OtosuGame_hammer*)task->data;

    pin = (OtosuGame_badge*)EasyTask_GetTaskData(pool, self->pinId);
    s32 stepped;
    s32 steppedB;
    u16 angle;
    s32 index;
    s32 headX;
    s32 headY;

    if (pin != NULL) {
        self->live = OtosuGame_badge_IsUsingHammer(pin);

        if (self->live == 0) {
            self->state = 0;
        } else {
            OtosuGame_badge_GetOrigin(pin, &self->origin);
            OtosuGame_badge_GetPos(pin, &self->pinPos);
            self->height = OtosuGame_badge_GetHeight(pin);
        }

        switch (self->state) {
            case 0:
                self->live = 0;
                break;

            case 1:
                self->showA = 0;
                self->showB = 1;
                OtosuGame_hammer_InitScaleCursor(&self->cursorScale, OtosuGame_hammer_ExtendScale, 8);
                OtosuGame_hammer_InitSpinCursor(&self->cursorSpin, OtosuGame_hammer_ExtendSpin, 6);
                self->angle0           = OtosuGame_badge_GetBackAngle(pin);
                self->angle            = self->angle0;
                self->affineB.rotation = (u16)(self->angle + 0x4000);
                self->affineA.rotation = (u16)(self->angle + 0x4000);
                self->scale            = 0;
                self->state            = (u32)(self->state + 1);
                OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_HUMMER01, &self->posA, &self->origin);
                /* fallthrough */

            case 2: {
                s32 len;

                stepped = OtosuGame_hammer_StepScaleCursor(&self->cursorScale);
                if (stepped != 0) {
                    s32 rate;

                    rate = self->rate0;
                    len  = (s32)(((s64)OtosuGame_hammer_GetScaleValue(&self->cursorScale) * rate + 0x800) >> 12);
                    self->affineB.scaleY = len;
                    self->scale          = len * 0x10;
                    index                = OTU_ANGLE_INDEX(self->angle);
                    headX                = (s32)(((s64)self->scale * FX_SinCosTable_[index + 1] + 0x800) >> 12);
                    headY                = (s32)(((s64)self->scale * FX_SinCosTable_[index] + 0x800) >> 12);
                    self->posB.x         = self->pinPos.x + headX / 2;
                    self->posB.y         = self->pinPos.y + headY / 2;
                }

                steppedB = OtosuGame_hammer_StepSpinCursor(&self->cursorSpin);
                if (steppedB != 0) {
                    s32 len2;

                    index                = OTU_ANGLE_INDEX(self->angle);
                    len2                 = self->scale + OtosuGame_hammer_GetSpinScale(&self->cursorSpin);
                    headX                = (s32)(((s64)len2 * FX_SinCosTable_[index + 1] + 0x800) >> 12);
                    headY                = (s32)(((s64)len2 * FX_SinCosTable_[index] + 0x800) >> 12);
                    self->affineA.scaleX = OtosuGame_hammer_GetSpinValue(&self->cursorSpin);
                    if (self->affineA.scaleX > 0) {
                        self->showA = 1;
                    }
                    self->posA.x = self->pinPos.x + headX;
                    self->posA.y = self->pinPos.y + headY;
                }

                if ((stepped == 0) && (steppedB == 0)) {
                    OtosuGame_hammer_InitTrailCursor(&self->cursorTrail, OtosuGame_hammer_WindupTrail, 5);
                    self->state = (u32)(self->state + 1);
                    OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_HUMMER02, &self->posA, &self->origin);
                }
                /* fallthrough */
            }

            case 3: {
                if (OtosuGame_hammer_StepTrailCursor(&self->cursorTrail) != 0) {
                    angle       = (u16)(self->angle0 + OtosuGame_hammer_GetTrailValue(&self->cursorTrail));
                    index       = OTU_ANGLE_INDEX(angle);
                    self->angle = angle;
                    headX       = (s32)(((s64)self->scale * FX_SinCosTable_[index + 1] + 0x800) >> 12);
                    headY       = (s32)(((s64)self->scale * FX_SinCosTable_[index] + 0x800) >> 12);

                    self->affineB.rotation = (u16)(angle + 0x4000);
                    self->affineA.rotation = (u16)(angle + 0x4000);
                    self->posA.x           = self->pinPos.x + headX;
                    self->posA.y           = self->pinPos.y + headY;
                    self->posB.x           = self->pinPos.x + headX / 2;
                    self->posB.y           = self->pinPos.y + headY / 2;
                } else {
                    self->framesLeft = self->totalFrames;
                    self->state      = (u32)(self->state + 1);
                        /* fallthrough */
                    case 4:

                        angle            = (u16)(self->framesLeft - 1);
                        self->framesLeft = angle;

                        if (angle > 0) {
                            s32 len;
                            s32 spin;
                            s32 index2;

                            len                  = self->affineB.scaleY + ((self->scale0 * 4) - self->affineB.scaleY) / angle;
                            self->affineB.scaleY = len;
                            self->scale          = len * 0x10;
                            spin                 = self->angle0 -
                                   (((self->angleBase << 0x10) * (self->totalFrames - self->framesLeft)) / self->totalFrames);
                            index2      = OTU_ANGLE_INDEX(spin);
                            self->angle = spin;

                            self->affineB.rotation = (u16)(spin + 0x4000);
                            self->affineA.rotation = (u16)(spin + 0x4000);
                            headX                  = (s32)(((s64)self->scale * FX_SinCosTable_[index2 + 1] + 0x800) >> 12);
                            headY                  = (s32)(((s64)self->scale * FX_SinCosTable_[index2] + 0x800) >> 12);
                            self->posA.x           = self->pinPos.x + headX;
                            self->posA.y           = self->pinPos.y + headY;
                            self->posB.x           = self->pinPos.x + headX / 2;
                            self->posB.y           = self->pinPos.y + headY / 2;

                            self->reportTimer = self->reportTimer - 1;
                            if (self->reportTimer <= 0) {
                                OtosuGame_badge_SpawnSmoke(EasyTask_GetTaskData(pool, self->pinId), &self->posA,
                                                           (u16)self->angle);
                                self->reportTimer = 4;
                            }
                        } else {
                            self->angle0 = self->angle;
                            OtosuGame_hammer_InitTrailCursor(&self->cursorTrail, OtosuGame_hammer_SwingTrail, 7);
                            self->state = (u32)(self->state + 1);
                                /* fallthrough */
                            case 5:
                                if (OtosuGame_hammer_StepTrailCursor(&self->cursorTrail) != 0) {
                                    s32 index3;

                                    angle       = (u16)(self->angle0 + OtosuGame_hammer_GetTrailValue(&self->cursorTrail));
                                    index3      = OTU_ANGLE_INDEX(angle);
                                    self->angle = angle;
                                    headX       = (s32)(((s64)self->scale * FX_SinCosTable_[index3 + 1] + 0x800) >> 12);
                                    headY       = (s32)(((s64)self->scale * FX_SinCosTable_[index3] + 0x800) >> 12);

                                    self->affineB.rotation = (u16)(angle + 0x4000);
                                    self->affineA.rotation = (u16)(angle + 0x4000);
                                    self->posA.x           = self->pinPos.x + headX;
                                    self->posA.y           = self->pinPos.y + headY;
                                    self->posB.x           = self->pinPos.x + headX / 2;
                                    self->posB.y           = self->pinPos.y + headY / 2;
                                } else {
                                    s32 len;

                                    len           = self->affineB.scaleY;
                                    self->halfLen = len / 4;
                                    OtosuGame_hammer_InitSpinCursor(&self->cursorSpin, OtosuGame_hammer_RetractSpin, 5);
                                    OtosuGame_hammer_InitScaleCursor(&self->cursorScale, OtosuGame_hammer_RetractScale, 4);
                                    self->state = (u32)(self->state + 1);
                                    OtuBoard_PlaySEAt(SEIDX_SE_BAYBADGE_HUMMER03, &self->posA, &self->origin);
                                        /* fallthrough */
                                    case 6:
                                        stepped = OtosuGame_hammer_StepScaleCursor(&self->cursorScale);
                                        if (stepped != 0) {
                                            s32 half;
                                            s32 len3;

                                            half = self->halfLen;
                                            len3 = (s32)(((s64)OtosuGame_hammer_GetScaleValue(&self->cursorScale) * half +
                                                          0x800) >>
                                                         12);
                                            self->affineB.scaleY = len3;
                                            self->scale          = len3 * 0x10;
                                            index                = OTU_ANGLE_INDEX(self->angle);
                                            headX = (s32)(((s64)self->scale * FX_SinCosTable_[index + 1] + 0x800) >> 12);
                                            headY = (s32)(((s64)self->scale * FX_SinCosTable_[index] + 0x800) >> 12);
                                            self->posB.x = self->pinPos.x + headX / 2;
                                            self->posB.y = self->pinPos.y + headY / 2;
                                        }

                                        steppedB = OtosuGame_hammer_StepSpinCursor(&self->cursorSpin);
                                        if (steppedB != 0) {
                                            s32 len4;

                                            index = OTU_ANGLE_INDEX(self->angle);
                                            len4  = self->scale + OtosuGame_hammer_GetSpinScale(&self->cursorSpin);
                                            self->affineA.scaleX = OtosuGame_hammer_GetSpinValue(&self->cursorSpin);
                                            headX        = (s32)(((s64)len4 * FX_SinCosTable_[index + 1] + 0x800) >> 12);
                                            headY        = (s32)(((s64)len4 * FX_SinCosTable_[index] + 0x800) >> 12);
                                            self->posA.x = self->pinPos.x + headX;
                                            self->posA.y = self->pinPos.y + headY;
                                        }

                                        if ((stepped == 0) && (steppedB == 0)) {
                                            self->state = 0U;
                                        }
                                }
                        }
                }
                break;
            }

            default:
                break;
        }

        if (self->live != 0) {
            if (self->showA != 0) {
                Sprite_Update(&self->spriteA);
            }
            if (self->showB != 0) {
                Sprite_Update(&self->spriteB);
            }
        }
    } else {
        self->live = 0;
    }
    return 1;
}

static s32 OtosuGame_hammer_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_hammer* self = (OtosuGame_hammer*)task->data;

    if (self->live != 0) {
        if (self->showA != 0) {
            self->spriteA.posX = (s16)((self->posA.x - self->origin.x) >> 12);
            self->spriteA.posY = (s16)(((self->posA.y + self->height) - self->origin.y) >> 12);
            Sprite_RenderFrame(&self->spriteA);
        }
        if (self->showB != 0) {
            self->spriteB.posX = (s16)((self->posB.x - self->origin.x) >> 12);
            self->spriteB.posY = (s16)(((self->posB.y + self->height) - self->origin.y) >> 12);
            Sprite_RenderFrame(&self->spriteB);
        }
    }
    return 1;
}

static s32 OtosuGame_hammer_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_hammer* self = (OtosuGame_hammer*)task->data;
    s32               i;

    Sprite_Release(&self->spriteB);
    Sprite_Release(&self->spriteA);

    for (i = 0; i < 4; i++) {
        EasyTask_DeleteTask(pool, self->children[i]);
    }
    return 1;
}

static s32 OtosuGame_hammer_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_hammer_Init,
        .update     = OtosuGame_hammer_Update,
        .render     = OtosuGame_hammer_Render,
        .cleanup    = OtosuGame_hammer_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

u32 OtosuGame_hammer_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs  params;
    u32               handle;
    OtosuGame_hammer* self;
    s32               i;

    params.dataType = dataType;
    params.pinId    = pinId;

    handle = EasyTask_CreateTask(pool, &Tsk_OtosuGame_hammer, NULL, 0, NULL, &params);

    self = (OtosuGame_hammer*)EasyTask_GetTaskData(pool, handle);

    for (i = 0; i < 4; i++) {
        self->children[i] = OtosuGame_hammerhahen_CreateTask(pool, dataType, pinId);
    }
    return handle;
}

// Size: 0x18

s32 OtosuGame_hammer_GetHitRecord(OtosuGame_hammer* self, OtuPinRecord* out) {
    s16 cosT;
    s16 sinT;
    s32 index;

    if (self->state != 4) {
        return 0;
    }

    index = OTU_ANGLE_INDEX(self->angle);
    cosT  = FX_SinCosTable_[index + 1];
    sinT  = FX_SinCosTable_[index];

    out[0].pos.x = self->pinPos.x + FX_Mul(self->scale, cosT) - FX_Mul(sinT, 16);
    out[0].pos.y = self->pinPos.y + FX_Mul(self->scale, sinT) + FX_Mul(cosT, 16);
    out[0].scale = 0x10000;

    out[1].pos.x = self->pinPos.x + FX_Mul(self->scale, cosT) - FX_Mul(sinT, -16);
    out[1].pos.y = self->pinPos.y + FX_Mul(self->scale, sinT) + FX_Mul(cosT, -16);
    out[1].scale = 0x10000;

    return 2;
}

s32 OtosuGame_hammer_IsActive(OtosuGame_hammer* self) {
    return self->state != 0;
}

void OtosuGame_hammer_Start(OtosuGame_hammer* self, s32 rate, s32 scale, s32 frames, u16 arc) {
    self->rate0       = rate;
    self->scale0      = scale;
    self->totalFrames = frames;
    self->angleBase   = arc;
    self->reportTimer = 1;
    self->state       = 1;
}

void OtosuGame_hammer_EndSwing(OtosuGame_hammer* self) {
    if (self->state == 4) {
        self->framesLeft = 0;
    }
}

u16 OtosuGame_hammer_GetAngle(OtosuGame_hammer* self) {
    return (u16)self->angle;
}

void OtosuGame_hammer_Shatter(OtosuGame_hammer* self) {
    Vec2p pt;
    void* data;
    s32   i;

    for (i = 0; i < 4; i++) {
        OamAffineParam* affine;

        data = EasyTask_GetTaskData(self->pool, self->children[i]);

        if (i == 0) {
            pt     = self->posA;
            affine = &self->affineA;
        } else {
            s32 phase = i * 0x1000;

            affine = &self->affineB;
            OtuVec_Sub(&self->posA, &self->pinPos, &pt);
            OtuVec_ScaleAdd(phase / 4, &pt, &self->pinPos, &pt);
        }

        OtosuGame_hammerhahen_Start(data, &pt, affine, i);
    }
}
