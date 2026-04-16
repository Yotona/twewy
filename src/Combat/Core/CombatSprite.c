#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/Combat.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/Resources/ResourceMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"

extern SpriteAnimation data_ov003_020e38f0;

static inline PaletteResource* Combat_GetSharedPalette(u32 displayEngine) {
    return data_ov003_020e71b8->unk3D348[displayEngine];
}

static inline UnkSmallInternal* Combat_GetSharedPaletteSource(void) {
    return (UnkSmallInternal*)Data_GetPackEntryData(data_ov003_020e71b8->unk3D350, 65);
}

s32 CombatSprite_SetAnimFromTable(CombatSprite* cSprite, u16 animTableIndex, s32 arg2) {
    Data*            temp_ip;
    void*            animData;
    void*            charSource;
    SpriteFrameData* frameData;
    SpriteAnimEntry* animEntry;

    if (animTableIndex == cSprite->animTableIndex) {
        return 1;
    }
    cSprite->animTableIndex = animTableIndex;
    if (arg2 & 1) {
        cSprite->sprite.animationMode = ANIM_MODE_ONCE;
    } else {
        cSprite->sprite.animationMode = ANIM_MODE_LOOPED;
    }

    temp_ip   = cSprite->sprite.resourceData;
    animEntry = &cSprite->animTable[animTableIndex];
    animData  = Data_GetPackEntryData(temp_ip, animEntry->paletteDataIndex);
    frameData = Data_GetPackEntryData(temp_ip, animEntry->frameDataIndex);

    Sprite_ChangeAnimation(&cSprite->sprite, animData, (s16)(animEntry->animDataIndex + 1), frameData);

    switch (cSprite->sprite.bits_3_4) {
        case 0:
        case 2:
        case 3:
            break;
        default:
            break;
        case 1:
            charSource            = Data_GetPackEntryData(cSprite->sprite.resourceData, animEntry->charDataIndex);
            cSprite->sprite.unk34 = charSource;
            break;
    }
    return 0;
}

s32 CombatSprite_SetAnim(CombatSprite* cSprite, u16 animTableIndex, s32 arg2) {
    if (animTableIndex == cSprite->animTableIndex) {
        return 1;
    }
    cSprite->animTableIndex = animTableIndex;
    if (arg2 & 1) {
        cSprite->sprite.animationMode = ANIM_MODE_ONCE;
    } else {
        cSprite->sprite.animationMode = ANIM_MODE_LOOPED;
    }
    Sprite_ChangeAnimation(&cSprite->sprite, cSprite->sprite.animData, animTableIndex + 1, cSprite->sprite.frameDataTable);
    return 0;
}

void CombatSprite_SetAffineTransform(CombatSprite* cSprite, u32 rotation, s32 scaleX, s32 scaleY, s32 arg4) {
    if (cSprite->flags46 & 1) {
        scaleX = -scaleX;
    }

    s32 flipFlags = 0;
    if (scaleX < 0) {
        flipFlags |= 1;
        scaleX = -scaleX;
    }
    if (scaleY < 0) {
        flipFlags |= 2;
        scaleY = -scaleY;
    }

    s32 temp_r0 = OamMgr_AllocAffineGroup(&g_OamMgr[cSprite->sprite.bits_0_1], rotation, scaleX, scaleY, flipFlags);

    cSprite->sprite.unk_0A.raw = (u16)((cSprite->sprite.unk_0A.raw & ~0x3E0) | ((u32)((u16)temp_r0 << 0x1B) >> 0x16));
    cSprite->sprite.unk_0A.raw = (u16)((cSprite->sprite.unk_0A.raw & ~1) | 1);

    if (arg4 != 0) {
        temp_r0 = 1;
    } else {
        temp_r0 = 0;
    }

    cSprite->sprite.unk_0A.raw = ((cSprite->sprite.unk_0A.raw & ~2) | ((u32)((u16)temp_r0 << 0x1F) >> 0x1E));
}

void CombatSprite_SetPaletteSource(CombatSprite* cSprite, s32 arg1) {
    cSprite->sprite.unk3C = Data_GetPackEntryData(cSprite->sprite.resourceData, arg1);
    PaletteMgr_SetSource(g_PaletteManagers[cSprite->sprite.bits_0_1], cSprite->sprite.paletteData, cSprite->sprite.unk3C);
}

void CombatSprite_SetPosition(CombatSprite* cSprite, s32 posX, s32 posY) {
    cSprite->sprite.posX = posX;
    cSprite->sprite.posY = posY;
}

void func_ov003_02082730(CombatSprite* cSprite, s32 arg1) {
    cSprite->unk48 = arg1;
}

void func_ov003_02082738(CombatSprite* cSprite, s32 arg1) {
    cSprite->sprite.unk_0A.raw = (u16)((cSprite->sprite.unk_0A.raw & ~0xC00) | ((u32)(arg1 << 0x1E) >> 0x14));
}

void CombatSprite_SetFlip(CombatSprite* cSprite, s32 arg1) {
    if (arg1 != 0) {
        cSprite->sprite.unk_0A.raw = ((cSprite->sprite.unk_0A.raw & ~0x3E0) |
                                      ((u32)((u16)(((cSprite->sprite.unk_0A.raw << 0x16) >> 0x1B) | 8) << 0x1B) >> 0x16));
        cSprite->flags46 |= 1;
    } else {
        cSprite->sprite.unk_0A.raw = ((cSprite->sprite.unk_0A.raw & ~0x3E0) |
                                      ((u32)((u16)(((cSprite->sprite.unk_0A.raw << 0x16) >> 0x1B) & ~8) << 0x1B) >> 0x16));
        cSprite->flags46 &= ~1;
    }
}

void CombatSprite_SetPaletteMode(CombatSprite* cSprite, s32 arg1) {
    cSprite->paletteMode = arg1;

    switch (cSprite->paletteMode) {
        case 1: {
            cSprite->sprite.paletteData = Combat_GetSharedPalette(cSprite->sprite.bits_0_1);
            cSprite->sprite.unk3C       = Combat_GetSharedPaletteSource();
        } break;

        case 0: {
            cSprite->sprite.paletteData = cSprite->basePalette;
            cSprite->sprite.unk3C       = cSprite->basePaletteSource;
        } break;

        case 2:
            break;
    }
}

void CombatSprite_RenderWithPalette(CombatSprite* cSprite, s32 arg1) {
    cSprite->sprite.unk_0A.unk_02 = 1;

    s32 prev             = data_0206a890.unk_0C;
    data_0206a890.unk_0C = (arg1 & 0x1F);

    Sprite_RenderFrame(&cSprite->sprite);
    cSprite->sprite.unk_0A.unk_02 = 0;

    data_0206a890.unk_0C = (prev & 0x1F);
}

// Nonmatching: Structure differences
SpriteFrameInfo* CombatSprite_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
    switch (arg2) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            data_0206b408.unk_04 = 0;
            data_0206b408.unk_08 = 0;

            SpriteFrameSource* source = arg0->unk24;
            data_0206b408.unk_0C      = source->unk_4C;
            data_0206b408.unk_10      = source->unk_48;
            if ((arg0->animData != NULL) && (arg0->frameDataTable != NULL)) {
                u16* temp_r3 = (u16*)(void*)arg0->frameDataTable;
                s16  temp_r2 = arg0->unk16;
                if (temp_r2 >= 0) {
                    data_0206b408.unk_04 = temp_r3[(temp_r2 * 4) + 1];
                    data_0206b408.unk_08 = (temp_r3 + temp_r3[temp_r2 * 4]);
                }
            }
            return &data_0206b408;
        default:
            return NULL;
    }
}

void CombatSprite_InitAnim(SpriteAnimation* anim, s32 arg1, const BinIdentifier* iden) {
    *anim          = data_ov003_020e38f0;
    anim->bits_0_1 = arg1;
    anim->binIden  = iden;
}

s32 CombatSprite_Load(CombatSprite* cSprite, SpriteAnimationEx* anim) {
    anim->anim.unk_10       = cSprite;
    cSprite->animTable      = (SpriteAnimEntry*)(void*)(*((s32*)((u8*)anim + 0x2C)));
    cSprite->animTableIndex = -1;
    cSprite->unk48          = -1;
    cSprite->flags46        = 0;
    cSprite->unk4C          = 0;
    cSprite->paletteMode    = 0;
    cSprite->paletteTimer   = 0;
    if (Sprite_Load(&cSprite->sprite, &anim->anim) == 0) {
        cSprite->flags46 |= 0x8000;
        return 0;
    }
    cSprite->basePaletteSource = cSprite->sprite.unk3C;
    cSprite->basePalette       = cSprite->sprite.paletteData;
    return 1;
}

void CombatSprite_LoadFromTable(s32 arg0, CombatSprite* arg1, const BinIdentifier* arg2, const SpriteAnimEntry* arg3, u16 arg4,
                                u16 arg5, u16 arg6) {
    SpriteAnimationEx      anim;
    const SpriteAnimEntry* entry;

    CombatSprite_InitAnim(&anim.anim, arg0, arg2);
    anim.anim.bits_10_11 = 1;
    entry                = &arg3[arg4];
    anim.anim.unk_1C     = entry->charDataIndex;
    anim.anim.unk_1E     = (arg6 << 5);
    anim.anim.unk_26     = entry->frameDataIndex;
    anim.anim.unk_28     = entry->paletteDataIndex;
    anim.anim.unk_20     = arg5;
    anim.anim.unk_2A     = entry->animDataIndex + 1;
    anim.unk_2C          = arg3;
    CombatSprite_Load(arg1, &anim);
}

void CombatSprite_LoadDirect(s32 arg0, CombatSprite* cSprite, const BinIdentifier* iden, s32 arg3, u16 arg4, u16 arg5,
                             u16 arg6, u16 arg7) {
    SpriteAnimationEx anim;

    CombatSprite_InitAnim(&anim.anim, arg0, iden);
    anim.anim.bits_10_11 = 0;
    anim.anim.unk_1C     = arg3;
    anim.anim.unk_26     = arg5;
    anim.anim.unk_28     = arg4;
    anim.anim.unk_20     = arg6;
    anim.anim.unk_2A     = arg7 + 1;
    anim.unk_2C          = 0;
    CombatSprite_Load(cSprite, &anim);
}

void CombatSprite_Init(CombatSprite* cSprite) {
    Sprite_Init(&cSprite->sprite);
}

void CombatSprite_Update(CombatSprite* cSprite) {
    if (cSprite->flags46 & 0x1000) {
        cSprite->paletteTimer--;
        if (cSprite->paletteTimer <= 0) {
            cSprite->sprite.unk3C       = cSprite->basePaletteSource;
            cSprite->sprite.paletteData = cSprite->basePalette;
            cSprite->paletteMode        = 0;
            cSprite->paletteTimer       = 0;
            cSprite->flags46 &= ~0x1000;
        }
    }
    Sprite_Update(&cSprite->sprite);
}

void CombatSprite_Render(CombatSprite* cSprite) {
    if (cSprite->flags46 & 4) {
        cSprite->sprite.unk_0A.unk_02 = 1;
    } else if (cSprite->flags46 & 2) {
        cSprite->sprite.unk_0A.unk_02 = 2;
    } else {
        cSprite->sprite.unk_0A.unk_02 = 0;
    }

    if (cSprite->paletteMode == 2) {
        if (SysControl.frameCount & 1) {
            cSprite->sprite.paletteData = Combat_GetSharedPalette(cSprite->sprite.bits_0_1);
            cSprite->sprite.unk3C       = Combat_GetSharedPaletteSource();
        } else {
            cSprite->sprite.paletteData = cSprite->basePalette;
            cSprite->sprite.unk3C       = cSprite->basePaletteSource;
        }
    }

    if (cSprite->sprite.unk_0A.unk_00 == 1) {
        if (cSprite->flags46 & 1) {
            cSprite->sprite.unk_0A.raw =
                (cSprite->sprite.unk_0A.raw & ~0x3E0) |
                ((u32)((u16)(((u32)(cSprite->sprite.unk_0A.raw << 0x16) >> 0x1B) & ~8) << 0x1B) >> 0x16);
        }
        Sprite_RenderFrame(&cSprite->sprite);
        if (cSprite->flags46 & 1) {
            cSprite->sprite.unk_0A.raw =
                (cSprite->sprite.unk_0A.raw & ~0x3E0) |
                ((u32)((u16)(((u32)(cSprite->sprite.unk_0A.raw << 0x16) >> 0x1B) | 8) << 0x1B) >> 0x16);
        }
    } else {
        Sprite_RenderFrame(&cSprite->sprite);
    }
}

void CombatSprite_Release(CombatSprite* cSprite) {
    if (cSprite->paletteMode != 0) {
        cSprite->sprite.unk3C       = cSprite->basePaletteSource;
        cSprite->sprite.paletteData = cSprite->basePalette;
        cSprite->paletteMode        = 0;
        cSprite->paletteTimer       = 0;
        cSprite->flags46 &= ~0x1000;
    }
    Sprite_Release(&cSprite->sprite);
}

void CombatSprite_Restart(CombatSprite* cSprite) {
    Sprite_Restart(&cSprite->sprite);
}
