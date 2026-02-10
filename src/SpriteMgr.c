#include "SpriteMgr.h"

extern s32 data_020676ec;

s16 data_0205adb4[12];
s32 data_0206b3d8[];
s32 data_0206b3cc[];

SpriteFrameInfo data_0206b408;

static s32 AnimCmd_Start(Sprite* sprite);
static s32 AnimCmd_Nop(Sprite* sprite);
static s32 AnimCmd_PlayFrame(Sprite* sprite);
static s32 AnimCmd_Wait(Sprite* sprite);
static s32 AnimCmd_Skip(Sprite* sprite);
static s32 AnimCmd_SetLoopFrame(Sprite* sprite);
static s32 AnimCmd_Loop(Sprite* sprite);
static s32 AnimCmd_Jump(Sprite* sprite);
static s32 AnimCmd_SetPosition(Sprite* sprite);
static s32 AnimCmd_AddPosition(Sprite* sprite);
static s32 AnimCmd_SetScale(Sprite* sprite);

typedef s32 (*AnimCommand)(Sprite*);
const AnimCommand data_0205adcc[11] = {
    AnimCmd_Start,       AnimCmd_PlayFrame, AnimCmd_Nop,          AnimCmd_Skip, AnimCmd_Wait, AnimCmd_SetPosition,
    AnimCmd_AddPosition, AnimCmd_SetScale,  AnimCmd_SetLoopFrame, AnimCmd_Loop, AnimCmd_Jump,
};

static SpriteFrameInfo* Sprite_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2) {
    switch (arg2) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            data_0206b408.unk_04 = 0;
            data_0206b408.unk_08 = 0;
            data_0206b408.unk_0C = 0;
            data_0206b408.unk_10 = -1;

            if (sprite->animData != NULL) {
                if (sprite->unk1C != 0) {
                    if (sprite->unk16 >= 0) {
                        data_0206b408.unk_04 = *(u16*)(sprite->unk1C + (sprite->unk16 * 4 + 1) * 2);
                        data_0206b408.unk_08 = sprite->unk1C + *(u16*)(sprite->unk1C + (sprite->unk16 * 8)) * 2;
                    }
                }
            }
            return &data_0206b408;
        default:
            return NULL;
    }
}

s32 AnimCmd_Start(Sprite* sprite) {
    s32 var_r3        = 0;
    sprite->isPlaying = TRUE;

    switch (sprite->animationMode) {
        case ANIM_MODE_STATIC:
            sprite->animData = NULL;
            break;
        case ANIM_MODE_LOOPED:
            sprite->currentFrame = sprite->loopFrame;
            sprite->scaleX       = 0;
            sprite->scaleY       = 0;
            var_r3               = 1;
            break;
    }
    return var_r3;
}

s32 AnimCmd_Nop(Sprite* sprite) {
    return 0;
}

s32 AnimCmd_PlayFrame(Sprite* sprite) {
    s32 var_r1 = 0;

    sprite->unk16 = (s16) * (sprite->animData + (((sprite->currentFrame * 4) + 1)));
    s16 temp_r3   = *(sprite->animData + (((sprite->currentFrame * 4) + 2)));

    if (temp_r3 > 0) {
        sprite->frameTimer++;
        if (sprite->frameTimer > temp_r3) {
            sprite->bit_11     = 1;
            sprite->frameTimer = 0;
            var_r1             = 1;
            sprite->currentFrame++;
        }
    }
    return var_r1;
}

s32 AnimCmd_Wait(Sprite* sprite) {
    s32 var_r1 = 0;

    sprite->unk16 = -1;

    s16 temp_r3 = *(sprite->animData + (((sprite->currentFrame * 4) + 1)));
    if (temp_r3 > 0) {
        sprite->frameTimer++;
        if (sprite->frameTimer > temp_r3) {
            sprite->bit_11     = 1;
            sprite->frameTimer = 0;
            var_r1             = 1;
            sprite->currentFrame++;
        }
    }
    return var_r1;
}

s32 AnimCmd_Skip(Sprite* sprite) {
    s32 var_r3 = 0;

    s16 temp_r2 = *(sprite->animData + (((sprite->currentFrame * 4) + 1)));
    if (temp_r2 > 0) {
        sprite->frameTimer++;
        if (sprite->frameTimer > temp_r2) {
            sprite->frameTimer = 0;
            var_r3             = 1;
            sprite->currentFrame++;
        }
    }
    return var_r3;
}

static s32 AnimCmd_SetLoopFrame(Sprite* sprite) {
    sprite->currentFrame++;
    sprite->loopFrame = sprite->currentFrame;
    return 1;
}

static s32 AnimCmd_Loop(Sprite* sprite) {
    sprite->currentFrame = sprite->loopFrame;
    sprite->unk16        = -1;
    return 1;
}

s32 AnimCmd_Jump(Sprite* sprite) {
    sprite->currentFrame =
        ((s16) * (sprite->animData + ((*(sprite->animData + (((sprite->currentFrame * 4) + 1))) + 4))) >> 2);
    sprite->loopFrame = sprite->currentFrame;
    return 1;
}

s32 AnimCmd_SetPosition(Sprite* sprite) {
    sprite->posX = *(sprite->animData + (((sprite->currentFrame * 4) + 1)));
    sprite->posY = *(sprite->animData + (((sprite->currentFrame * 4) + 2)));
    sprite->currentFrame++;
    return 1;
}

s32 AnimCmd_AddPosition(Sprite* sprite) {
    sprite->posX += *(sprite->animData + (((sprite->currentFrame * 4) + 1)));
    sprite->posY += *(sprite->animData + (((sprite->currentFrame * 4) + 2)));
    sprite->currentFrame++;
    return 1;
}

s32 AnimCmd_SetScale(Sprite* sprite) {
    sprite->scaleX = *(sprite->animData + (((sprite->currentFrame * 4) + 1)));
    sprite->scaleY = *(sprite->animData + (((sprite->currentFrame * 4) + 2)));
    sprite->currentFrame++;
    return 1;
}

void Sprite_Init(Sprite* sprite) {
    func_0203b3c0(sprite, 0, sizeof(Sprite));
    sprite->bits_3_4      = 0;
    sprite->bits_0_1      = 0;
    sprite->animationMode = ANIM_MODE_STATIC;
    sprite->bits_7_9      = 7;
    sprite->isPlaying     = TRUE;
    sprite->posY          = 192;
    sprite->unk16         = -1;
    sprite->unk_20        = Sprite_GetFrameInfo;
}

void Sprite_Update(Sprite* sprite) {
    SpriteFrameInfo* temp_r4 = sprite->unk_20(sprite, 0, 1);

    sprite->isPlaying = FALSE;
    sprite->bit_11    = 0;
    if (sprite->animData != NULL) {
        for (s32 var_r5 = 0; var_r5 < temp_r4->unk_00; var_r5++) {
            while (data_0205adcc[(*(sprite->animData + (sprite->currentFrame * 4)))](sprite) == TRUE) {
                // Loop until condition is false
            }
        }
    }
}

static void func_0200dde8(Sprite* sprite, void* arg1, u16) {
    // Not yet implemented
}

static void func_0200e034(Sprite* sprite, void* arg1, u16) {
    // Not yet implemented
}

void Sprite_RenderFrame(Sprite* sprite) {
    SpriteFrameInfo* renderData = sprite->unk_20(sprite, 0, 2);

    u32 test = sprite->bit_2;

    if (test != 0 && sprite->charData != NULL && sprite->paletteData != NULL) {
        if (renderData->unk_08 != 0) {
            sprite->unk_0A =
                (sprite->unk_0A & ~0xF000) | (((sprite->paletteData->unk_10 + ((sprite->unk_0A >> 12) & 0xF)) & 0xF) << 12);

            switch (sprite->bits_3_4) {
                case 0:
                case 2:
                    func_0200dde8(sprite, renderData, sprite->unk_0A);
                    break;
                case 1:
                case 3:
                    func_0200e034(sprite, renderData, sprite->unk_0A);
                    break;
                default:
                    break;
            }
        }

        if (sprite->paletteData->unk_18 != sprite->unk3C) {
            func_0200bfb4(data_0206b3cc[test]);
        }
    }
}

static s32 Sprite_LoadFromData(Sprite* sprite, SpriteAnimation* arg1) {
    Sprite_Init(sprite);

    sprite->bits_0_1      = arg1->bits_0_1;
    sprite->bit_2         = arg1->bit_6;
    sprite->bits_3_4      = arg1->bits_10_11;
    sprite->animationMode = arg1->bits_12_13;

    sprite->unk_0A = arg1->unk_02;
    sprite->posX   = arg1->unk_04;
    sprite->posY   = arg1->unk_06;

    s32 sp8[5];
    sp8[0] = 4;
    sp8[1] = arg1->unk_1C;
    sp8[2] = arg1->unk_20;
    sp8[3] = arg1->unk_26;
    sp8[4] = arg1->unk_28;

    s32 dataType = arg1->dataType;

    switch (arg1->unk_18) {
        case 0: {
            BinIdentifier* iden = arg1->binIden;

            sprite->resourceData = (BinMgr_FindById(arg1->binIden) == NULL) ? DatMgr_LoadRawData(dataType, NULL, 0, iden)
                                                                            : DatMgr_LoadUncompressedBin(dataType, iden);
        } break;

        case 1: {
            BinIdentifier* iden = arg1->binIden;

            sprite->resourceData = (BinMgr_FindById(arg1->binIden) == NULL) ? DatMgr_LoadCompressedBin(dataType, NULL, 0, iden)
                                                                            : DatMgr_LoadResource(dataType, iden);
        } break;

        case 2: {
            BinIdentifier* iden = arg1->binIden;
            s16            idx  = arg1->packIndex;

            sprite->resourceData = (BinMgr_FindById(arg1->binIden) == NULL)
                                       ? DatMgr_LoadPackEntry(dataType, NULL, 0, iden, idx, 0)
                                       : DatMgr_LoadPackEntryDirect(dataType, iden, idx, 0);
        } break;

        case 3: {
            BinIdentifier* iden = arg1->binIden;
            s16            idx  = arg1->packIndex;

            sprite->resourceData = (BinMgr_FindById(iden) == NULL) ? DatMgr_LoadPackEntry(dataType, NULL, 0, iden, idx, 1)
                                                                   : DatMgr_LoadPackEntryDirect(dataType, iden, idx, 1);
        } break;

        case 4: {
            sprite->resourceData = DatMgr_GeneratePackedData(dataType, NULL, NULL, arg1->binIden, &sp8);
            if (sp8[1] > 0) {
                sp8[1] = 1;
            }
            sp8[3] = 3;
            if (sp8[2] > 0) {
                sp8[2] = 2;
            }
            sp8[4] = 4;
        } break;
    }

    void* var_r1;
    if ((sprite->resourceData == NULL) || (sp8[4] <= 0)) {
        var_r1 = NULL;
    } else {
        void* temp_r1 = sprite->resourceData->buffer + 0x20;
        var_r1        = temp_r1 + (s32) * (u32*)((u8*)temp_r1 + (sp8[4] * 8));
    }

    void* var_r4;
    if ((sprite->resourceData == NULL) || (sp8[3] <= 0)) {
        var_r4 = NULL;
    } else {
        void* temp_r2_4 = sprite->resourceData->buffer + 0x20;
        var_r4          = temp_r2_4 + (s32) * (u32*)((u8*)temp_r2_4 + (sp8[3] * 8));
    }

    Sprite_ChangeAnimation(sprite, var_r1, arg1->unk_2A, var_r4);
    void* var_r1_2 = NULL;
    void* var_r6   = NULL;
    switch (sprite->bits_3_4) {
        case 0: {
            if ((sprite->resourceData == NULL) || (sp8[1] <= 0)) {
                var_r1_2 = NULL;
            } else {
                void* temp_r1_2 = sprite->resourceData->buffer + 0x20;
                var_r1_2        = temp_r1_2 + (s32) * (u32*)((u8*)temp_r1_2 + (sp8[1] * 8));
            }
            if ((sprite->resourceData == NULL) || (sp8[2] <= 0)) {
                var_r6 = NULL;
            } else {
                void* temp_r2_5 = sprite->resourceData->buffer + 0x20;
                var_r6          = temp_r2_5 + (s32) * (u32*)((u8*)temp_r2_5 + (sp8[2] * 8));
            }
        } break;

        case 1: {
            if ((sprite->resourceData == NULL) || (sp8[2] <= 0)) {
                var_r6 = NULL;
            } else {
                void* temp_r2_6 = sprite->resourceData->buffer + 0x20;
                var_r6          = temp_r2_6 + (s32) * (u32*)((u8*)temp_r2_6 + (sp8[2] * 8));
            }
        } break;

        case 2: {
            if ((sprite->resourceData == NULL) || (sp8[1] <= 0)) {
                var_r1_2 = NULL;
            } else {
                void* temp_r1_3 = sprite->resourceData->buffer + 0x20;
                var_r1_2        = temp_r1_3 + (s32) * (u32*)((u8*)temp_r1_3 + (sp8[1] * 8));
            }
        } break;
    }

    if ((arg1->unk_1E == 0) && (var_r1_2 == NULL)) {
        arg1->unk_1E = (s16)((UnkSmallInternal*)var_r4)->unk_06;
    }

    u32 temp_r4_4 = arg1->bits_0_1;
    u32 temp_r5_6 = arg1->bits_7_9;
    u32 temp_r6_4 = arg1->bit_6;

    if (sp8[1] > 0) {
        sprite->charData = func_0200cb4c(data_0206b3d8[temp_r4_4], var_r1_2, arg1->unk_1E, temp_r6_4);

        void* var_r0_5;
        if ((sprite->resourceData == NULL) || (sp8[1] <= 0)) {
            var_r0_5 = NULL;
        } else {
            void* temp_r1_4 = sprite->resourceData->buffer + 0x20;
            var_r0_5        = temp_r1_4 + *(u32*)(temp_r1_4 + (sp8[1] * 8));
        }
        sprite->unk34  = var_r0_5;
        sprite->bit_13 = 1;
    }

    s16 var_r3 = arg1->unk_22;
    if (sp8[2] > 0) {
        if ((arg1->unk_24 == 0) || (arg1->unk_24 == 0xFFFF)) {
            if (arg1->unk_22 == 0) {
                var_r3 = ((((Sprite*)(sprite->resourceData->buffer + (sp8[2] * 8)))->unk24 + 0x1F) & ~0x1F) >> 5;
            }
            sprite->paletteData = func_0200a968(data_0206b3cc[temp_r4_4], var_r6, temp_r5_6, var_r3);
        } else {
            if (arg1->unk_22 == 0) {
                var_r3 = 1;
            }
            sprite->paletteData = func_0200abe4(data_0206b3cc[temp_r4_4], var_r6, temp_r5_6, var_r3);
        }

        UnkSmallInternal* var_r0_6;
        if ((sprite->resourceData == NULL) || (sp8[2] <= 0)) {
            var_r0_6 = NULL;
        } else {
            void* temp_r1_6 = sprite->resourceData->buffer + 0x20;
            var_r0_6        = temp_r1_6 + *(u32*)(temp_r1_6 + (sp8[2] * 8));
        }
        sprite->unk3C = var_r0_6;

        sprite->bit_14 = 1;
    }

    if (((sp8[1] > 0) && (sprite->charData == NULL)) || ((sp8[2] > 0) && (sprite->paletteData == NULL))) {
        Sprite_Release(sprite);
        return 0;
    }

    sprite->unk38 = 0;
    if (arg1->unk_08 != NULL) {
        sprite->unk_20 = arg1->unk_08;
        if (sprite->animData == NULL) {
            sprite->animData = data_0205adb4;
        }
    }
    sprite->unk24 = arg1->unk_10;
    sprite->unk_20(sprite, arg1->unk_0C, 0);
    return 1;
}

void Sprite_Load(Sprite* sprite, SpriteAnimation* anim) {
    Sprite_LoadFromData(sprite, anim);
}

void Sprite_Release(Sprite* sprite) {
    sprite->unk_20(sprite, 0, 3);

    u32 temp_r4 = sprite->bits_0_1;

    if (sprite->resourceData != 0) {
        DatMgr_ReleaseData(sprite->resourceData);
    }

    if ((sprite->bit_13 == 1) && (sprite->charData != 0)) {
        func_0200cd44(data_0206b3d8[temp_r4], sprite->charData);
    }
    if ((sprite->bit_14 == 1) && (sprite->paletteData != NULL)) {
        func_0200afec(data_0206b3cc[temp_r4], sprite->paletteData);
    }
    Sprite_Init(sprite);
}

BOOL Sprite_HasAnimation(Sprite* sprite) {
    BOOL hasAnim = TRUE;
    if (sprite->animData == NULL) {
        hasAnim = FALSE;
    }
    return hasAnim;
}

BOOL func_0200ea4c(Sprite* sprite) {
    s32 var_r3 = 0;
    if (sprite->animData != 0) {
        switch (*(sprite->animData + (sprite->currentFrame * 4))) {
            case 2:
                var_r3 = 1;
                break;
            case 0:
                if (sprite->animationMode == ANIM_MODE_ONCE) {
                    var_r3 = 1;
                }
                break;
        }
    }
    return var_r3;
}

BOOL func_0200ea98(Sprite* sprite) {
    BOOL ret = TRUE;

    if (sprite->animData != 0) {
        if (*(sprite->animData + (sprite->currentFrame * 2)) == 1) {
            s16 temp_r1 = *(sprite->animData + ((sprite->currentFrame + 2)));
            if ((temp_r1 > 0) && (sprite->frameTimer != temp_r1)) {
                ret = FALSE;
            }
        }
    }
    return ret;
}

void Sprite_SetAnimation(Sprite* sprite, s16* animData, s16 animFrame, s32 arg3) {
    if (animFrame >= *animData) {
        animFrame = 0;
    }

    sprite->isPlaying     = FALSE;
    sprite->bit_11        = 0;
    sprite->isSingleFrame = 0;
    sprite->scaleX        = 0;
    sprite->scaleY        = 0;
    sprite->currentFrame  = animData[animFrame + 4] >> 2;
    sprite->loopFrame     = sprite->currentFrame;
    sprite->frameTimer    = 0;
    sprite->animIndex     = animFrame;
    sprite->unk16         = -1;
    sprite->animData      = animData;
    sprite->unk1C         = arg3;
}

s32 Sprite_ChangeAnimation(Sprite* arg0, void* animData, s16 arg2, void* arg3) {
    if ((arg0->animData == animData) && (arg0->animIndex == arg2) && (arg0->unk1C == arg3)) {
        return 0;
    }
    Sprite_SetAnimation(arg0, animData, arg2, arg3);
    return 1;
}

static void Sprite_SetStaticCell(Sprite* arg0, s32 arg1, s16 arg2) {
    arg0->isPlaying     = FALSE;
    arg0->bit_11        = 0;
    arg0->isSingleFrame = 1;

    arg0->scaleX       = 0;
    arg0->scaleY       = 0;
    arg0->currentFrame = 0;
    arg0->loopFrame    = 0;
    arg0->frameTimer   = 0;
    arg0->animIndex    = 0;
    arg0->unk16        = arg2;
    arg0->animData     = data_0205adb4;
    arg0->unk1C        = arg1;
}

s32 Sprite_ChangePalette(Sprite* sprite, s32 arg1, void* arg2, u32 arg3, u32 arg4) {
    if (sprite->paletteData != NULL) {
        func_0200afec(data_0206b3cc[arg1], sprite->paletteData);
        sprite->bit_14 = 0;
    }

    void* palette = func_0200a968(data_0206b3cc[arg1], arg2, arg3, arg4);
    if (palette != NULL) {
        sprite->paletteData = palette;
        sprite->unk3C       = arg2;
        sprite->bit_14      = 1;
    }
    return 0;
}

void Sprite_Restart(Sprite* sprite) {
    if (sprite->isSingleFrame == FALSE) {
        Sprite_SetAnimation(sprite, sprite->animData, sprite->animIndex, sprite->unk1C);
    } else {
        Sprite_SetStaticCell(sprite, sprite->unk1C, sprite->unk16);
    }
}

void Sprite_RenderAltPalette(Sprite* arg0, UnkSmallInternal* arg1, UnkSmallInternal* arg2, s16 arg3) {
    void* temp30;
    void* temp3C;

    if ((arg1 != 0) && (arg2 != 0)) {
        temp30            = arg0->paletteData;
        temp3C            = arg0->unk3C;
        arg0->paletteData = arg1;
        arg0->unk3C       = arg2;
    }

    u32 temp                  = arg0->paletteData->unk_10 & 0xFF;
    arg0->paletteData->unk_10 = temp + arg3;

    Sprite_Render(arg0);

    arg0->paletteData->unk_10 = temp;
    if ((arg1 != 0) && (arg2 != 0)) {
        arg0->paletteData = temp30;
        arg0->unk3C       = temp3C;
    }
}

s32 _Sprite_Load(Sprite* sprite, SpriteAnimation* anim) {
    Sprite_Load(sprite, anim);
    return 1;
}

BOOL Sprite_UpdateAndCheck(Sprite* sprite) {
    BOOL hasAnim = FALSE;
    if (sprite != NULL) {
        Sprite_Update(sprite);
        hasAnim = Sprite_HasAnimation(sprite);
    }
    return hasAnim;
}

void Sprite_Render(Sprite* sprite) {
    if (sprite != NULL) {
        Sprite_RenderFrame(sprite);
    }
}

void Sprite_Destroy(Sprite* sprite) {
    if (sprite != NULL) {
        Sprite_Release(sprite);
    }
}

// TODO: Appears to only be used by data_0205ad9c/func_0200c858, possibly just type Task
typedef struct {
    /* 0x00 */ char    unk_00[0x18];
    /* 0x18 */ Sprite* unk_18;
} UnkTemp;

void func_0200ed38(s32 unused, UnkTemp* arg1, void* arg2) {
    Sprite_Load(arg1->unk_18, arg2);
}

BOOL func_0200ed4c(s32 unused, UnkTemp* arg1) {
    Sprite* arg = arg1->unk_18;

    BOOL ret = FALSE;
    if (arg != NULL) {
        Sprite_Update(arg);
        ret = Sprite_HasAnimation(arg);
    }
    return ret;
}

BOOL func_0200ed74(s32 unused, UnkTemp* arg1) {
    if (arg1->unk_18 != NULL) {
        Sprite_RenderFrame(arg1->unk_18);
    }
    return TRUE;
}

BOOL func_0200ed90(s32 unused, UnkTemp* arg1) {
    if (arg1->unk_18 != NULL) {
        Sprite_Release(arg1->unk_18);
    }
    return TRUE;
}
