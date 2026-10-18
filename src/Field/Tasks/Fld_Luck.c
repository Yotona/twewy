#include "Field/Tasks/Fld_Luck.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Battle/FieldNoiseEncounter.h"
#include "Field/Battle/FieldNoiseQuota.h"
#include "Field/Event/EventFlags.h"
#include "Field/FieldTypes.h"
#include "Field/Scene/FieldArea.h"
#include "Field/Tasks/Fld_Badge.h"
#include "Field/Tasks/Fld_NSymbolUG.h"
#include "Field/Tasks/Fld_NSymbolUGMgr.h"
#include "Field/Tasks/Fld_Piranha.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

// Fixed symbol placement of an area (FldLuckEntry::spawns).
typedef struct FldLuckSpawn {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s32 gate;      // FldGate that retires the symbol, or -1
    /* 0x08 */ s16 encounter; // -1 picks a random encounter
    /* 0x0A */ s16 pad;
} FldLuckSpawn;               // Size: 0xC

// Per-area placement list, reached through MainData::unk_310C (terminated by unk_0 == 0x46).
typedef struct FldLuckEntry {
    /* 0x00 */ s16           unk_0;
    /* 0x02 */ s16           unk_2;
    /* 0x04 */ s32           unk_4;
    /* 0x08 */ s32           unk_8;
    /* 0x0C */ s16           count;
    /* 0x0E */ s16           pad;
    /* 0x10 */ FldLuckSpawn* spawns;
} FldLuckEntry; // Size: 0x14

typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s32 value;
} FldLuckDigitArgs;

// Luck-number sprite digit object.
typedef struct FldLuckDigit {
    /* 0x000 */ Sprite sprites[5];
    /* 0x140 */ Sprite extra;
    /* 0x180 */ s32    unk_180;
    /* 0x184 */ s32    unk_184;
    /* 0x188 */ s32    unk_188;
    /* 0x18C */ s32    unk_18C;
    /* 0x190 */ s32    unk_190;
    /* 0x194 */ s16    unk_194;
    /* 0x196 */ s16    pad_196;
} FldLuckDigit; // Size: 0x198

extern s32              data_0205c968;
s32                     func_020259e4(s32 itemId);
static SpriteFrameInfo* Fld_Luck_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_Luck_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const BinIdentifier Iden_MenuLuck = {30, "Apl_Tak/Grp_MenuLuck.bin"};

static const TaskHandle Tsk_Fld_Luck = {"Tsk_Fld_Luck", Fld_Luck_RunTask, 0x198};

static const SpriteAnimation Fld_Luck_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_Luck_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_MenuLuck,
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};
static void Fld_Luck_SetVisible(TaskPool* pool, u32 id, s32 arg2);

static s32 Fld_Luck_IsVariantCategory(s32 arg0) {
    if (arg0 >= 0x1F && arg0 < 0x27) {
        return 1;
    }
    return 0;
}

static void Fld_Luck_RemoveOtherSymbols(TaskPool* pool, NsymUGMgr* mgr, s32 mode) {
    s32 i;

    for (i = 0; i < mgr->count; i++) {
        if (mode == 8) {
            if (gSaveData.scanSymbols[i].category != 8) {
                EasyTask_DeleteTask(pool, mgr->ids[i]);
                mgr->ids[i] = -1;
            }
        } else if (Fld_Luck_IsVariantCategory(mode) != 0) {
            if (Fld_Luck_IsVariantCategory(gSaveData.scanSymbols[i].category) == 0) {
                EasyTask_DeleteTask(pool, mgr->ids[i]);
                mgr->ids[i] = -1;
            }
        } else if (gSaveData.scanSymbols[i].category == 8 ||
                   Fld_Luck_IsVariantCategory(gSaveData.scanSymbols[i].category) != 0)
        {
            EasyTask_DeleteTask(pool, mgr->ids[i]);
            mgr->ids[i] = -1;
        }
    }
}

static void Fld_Luck_SelectSymbol(TaskPool* pool, NsymUGMgr* mgr, s32 idx) {
    gSaveData.scanSymbolSelection = idx;
    Fld_NSymbolUG_Capture(pool, mgr->ids[idx]);
    if (gSaveData.scanSymbols[idx].category == 0x16) {
        return;
    }
    Fld_Luck_SetVisible(pool, mgr->luckTaskId, 1);
    Fld_Luck_RemoveOtherSymbols(pool, mgr, gSaveData.scanSymbols[idx].category);
}

s32 Fld_Luck_TrySelectSymbol(TaskPool* pool, u32 id1, u32 id2) {
    NsymUGMgr* mgr;
    s16        i;

    mgr = EasyTask_GetTaskById(pool, id1)->data;
    for (i = 0; i < mgr->count; i++) {
        if (mgr->ids[i] != -1 && gSaveData.scanSymbols[i].state == 2) {
            Fld_Luck_SelectSymbol(pool, mgr, i);
            return 1;
        }
    }
    if (Field_IsMoveTouchActive() != 0) {
        return 0;
    }
    if (Fld_Badge_IsTouched(EasyTask_GetTaskById(pool, id2)) != 0) {
        return 0;
    }
    for (i = 0; i < mgr->count; i++) {
        if (mgr->ids[i] != -1 && Fld_NSymbolUG_IsTaskTouched(pool, mgr->ids[i]) != 0) {
            Fld_Luck_SelectSymbol(pool, mgr, i);
            return 1;
        }
    }
    return 0;
}

s32 Fld_Luck_RemoveActiveSymbols(TaskPool* pool, u32 id) {
    NsymUGMgr* mgr;
    s16        i;

    mgr = EasyTask_GetTaskById(pool, id)->data;
    for (i = 0; i < mgr->count; i++) {
        if (mgr->ids[i] != -1 && gSaveData.scanSymbols[i].state == 1) {
            EasyTask_DeleteTask(pool, mgr->ids[i]);
            mgr->ids[i] = -1;
        }
    }
}

static s32 Fld_Luck_HasActiveNormalSymbol(void) {
    s16 count;
    s32 i;

    count = gSaveData.scanSymbolCount;
    for (i = 0; i < count; i++) {
        if (gSaveData.scanSymbols[i].state == 1 && gSaveData.scanSymbols[i].category != 8) {
            return 1;
        }
    }
    return 0;
}

static s32 Fld_Luck_IsSymbolHidden(s32 idx) {
    if (gSaveData.scanSymbols[idx].category != 8) {
        return 0;
    }
    if (gSaveData.fieldArea < 0x18 || gSaveData.fieldArea > 0x24) {
        return 0;
    }
    return Fld_Luck_HasActiveNormalSymbol();
}

void Fld_Luck_SpawnSymbols(TaskPool* pool, u32 id, FieldPos* anchor) {
    NsymUGMgr* mgr;
    s32        i;

    mgr = EasyTask_GetTaskById(pool, id)->data;
    for (i = 0; i < gSaveData.scanSymbolCount; i++) {
        if (Fld_Luck_IsSymbolHidden(i) != 0) {
            mgr->ids[mgr->count] = -1;
        } else if (gSaveData.scanSymbols[i].state == 1) {
            FldScreenPos pos;

            if (mgr->count >= 0x80) {
                return;
            }
            pos.x                = gSaveData.scanSymbols[i].x;
            pos.y                = gSaveData.scanSymbols[i].y;
            mgr->ids[mgr->count] = Fld_NSymbolUG_CreateTask(pool, mgr->dataType, pos, gSaveData.scanSymbols[i].category,
                                                            gSaveData.scanSymbols[i].mode, 0, anchor, i);
        } else {
            mgr->ids[mgr->count] = -1;
        }
        mgr->count += 1;
    }
}

s16 Fld_Luck_GetDropRate(void) {
    if (Fld_NSymbolUGMgr_Instance == NULL) {
        return (s16)gSaveData.playerStats.dropRate;
    }
    return Fld_NSymbolUGMgr_Instance->dropRate;
}

s32 Fld_Luck_AddSymbol(TaskPool* pool, s16 x, s16 y, s16 kind, s16 mode, s32 mask) {
    NsymUGMgr*   mgr;
    s16          k;
    s32          category;
    FldScreenPos pos;

    mgr = Fld_NSymbolUGMgr_Instance;
    k   = kind;
    if (mgr == NULL) {
        return 0;
    }
    if (mgr->count >= 0x80) {
        return 0;
    }
    if (k == 0) {
        k = FieldNoise_RollEncounter(0);
        if (k == -1) {
            k = 1;
        }
    }
    category = Field_GetEncounterCategory(k);
    if (mode == 1 && category < 8) {
        category += 0x1F;
    }
    pos.x                = x;
    pos.y                = y;
    mgr->ids[mgr->count] = Fld_NSymbolUG_CreateTask(pool, mgr->dataType, pos, category, mode, mask, NULL, -1);
    gSaveData.scanSymbols[mgr->count].encounter = k;
    gSaveData.scanSymbols[mgr->count].state     = 1;
    gSaveData.scanSymbols[mgr->count].category  = category;
    gSaveData.scanSymbols[mgr->count].mode      = mode;
    gSaveData.scanSymbols[mgr->count].x         = x;
    gSaveData.scanSymbols[mgr->count].y         = y;
    gSaveData.scanSymbols[mgr->count].unk_0C    = 0;
    gSaveData.scanSymbols[mgr->count].unk_10    = -1;
    mgr->count += 1;
    return 1;
}

// Nonmatching: register allocation only (count/v0 accumulator pair swapped:
// target count=r9, dy-base v0=r6; ours count=r6, v0=r9) and the loop-tail
// compare sits two instructions early (USA 99.41%; JP unvalidated).
static void Fld_Luck_RollRandomSymbols(s32 arg0) {
    s32 limit;
    s32 v0;
    s32 v1;
    s32 y2;
    s32 dy;
    s32 count;

    count = gSaveData.scanSymbolCount;
    if (gSaveData.unk_2488 != 0) {
        limit = gSaveData.scanSymbolCount + FieldNoise_GetQuota(arg0);
    } else {
        limit = count + gSaveData.noiseSymbolCount;
    }
    if (limit >= 0x80) {
        limit = 0x80;
    }
    y2 = func_ov030_020b7b60() + 0x60;
    if (y2 > func_ov030_020b7b20()) {
        y2 = func_ov030_020b7b20();
    }
    dy = func_ov030_020b7b20() - y2;
    v1 = func_ov030_020b7b80();
    v0 = func_ov030_020b7b00();
    v0 = v1 - v0 / 2;
    for (; count < limit; count++) {
        gSaveData.scanSymbols[count].encounter = FieldNoise_RollEncounter(arg0);
        if (gSaveData.scanSymbols[count].encounter == -1) {
            if (gSaveData.unk_2488 != 0) {
                break;
            }
            gSaveData.scanSymbols[count].encounter = 1;
        }
        gSaveData.scanSymbols[count].x        = v0 + RNG_Next(func_ov030_020b7b00());
        gSaveData.scanSymbols[count].y        = dy + RNG_Next(y2);
        gSaveData.scanSymbols[count].state    = 1;
        gSaveData.scanSymbols[count].category = Field_GetEncounterCategory(gSaveData.scanSymbols[count].encounter);
        gSaveData.scanSymbols[count].mode     = 2;
        gSaveData.scanSymbols[count].unk_10   = -1;
        gSaveData.scanSymbols[count].unk_0C   = 0;
    }
    gSaveData.scanSymbolCount = count;
}

static void Fld_Luck_AddSpecialSymbol(void) {
    s32 i = gSaveData.scanSymbolCount;

    gSaveData.scanSymbols[i].encounter = 0;
    gSaveData.scanSymbols[i].x         = 0xA0;
    gSaveData.scanSymbols[i].y         = 0x74;
    gSaveData.scanSymbols[i].state     = 1;
    gSaveData.scanSymbols[i].category  = 0x16;
    gSaveData.scanSymbols[i].mode      = 0;
    gSaveData.scanSymbols[i].unk_10    = -1;
    gSaveData.scanSymbols[i].unk_0C    = 0;
    gSaveData.scanSymbolCount++;
}

static s32 Fld_Luck_HasScanEventSymbol(void) {
    s32             i;
    s32             n;
    FieldScanEvent* ev;

    i = 0;
    n = data_0205c968;
    if (n > 0) {
        ev = data_0205c5be;
        do {
            u16 flags = ev->flags;

            if (!(flags & 2) && (!(flags & 1) || Inventory_HasRequiredQuantity(0x2AE, 1, 0) != 0) &&
                ev->area == gSaveData.fieldArea)
            {
                if (ev->firstChapter <= (s32)gSaveData.chapter && (s32)gSaveData.chapter <= ev->lastChapter) {
                    if (func_020259e4(i) == 0 && Field_GetEncounterCategory(ev->encounter) == 8) {
                        return 1;
                    }
                }
            }
            i += 1;
            ev += 1;
        } while (i < n);
    }
    return 0;
}

static void Fld_Luck_AddScanEventSymbols(void) {
    s32             i;
    s32             n;
    FieldScanEvent* ev;

    i = 0;
    n = data_0205c968;
    if (n <= 0) {
        return;
    }
    ev = data_0205c5be;
    do {
        if ((!(ev->flags & 1) || Inventory_HasRequiredQuantity(0x2AE, 1, 0) != 0) && ev->area == gSaveData.fieldArea) {
            if (ev->firstChapter <= (s32)gSaveData.chapter && (s32)gSaveData.chapter <= ev->lastChapter) {
                if (func_020259e4(i) != 0) {
                    if ((u16)ev->encounter == 0xBE) {
                        gSaveData.scanSymbols[gSaveData.scanSymbolCount].x         = ev->x;
                        gSaveData.scanSymbols[gSaveData.scanSymbolCount].y         = ev->y;
                        gSaveData.scanSymbols[gSaveData.scanSymbolCount].encounter = FieldNoise_RollEncounter(0);
                        if (gSaveData.scanSymbols[gSaveData.scanSymbolCount].encounter != -1) {
                            gSaveData.scanSymbols[gSaveData.scanSymbolCount].state = 1;
                            gSaveData.scanSymbols[gSaveData.scanSymbolCount].category =
                                (s16)Field_GetEncounterCategory(gSaveData.scanSymbols[gSaveData.scanSymbolCount].encounter);
                            gSaveData.scanSymbols[i].mode   = 2;
                            gSaveData.scanSymbols[i].unk_10 = -1;
                            gSaveData.scanSymbols[i].unk_0C = 0;
                            gSaveData.scanSymbolCount       = gSaveData.scanSymbolCount + 1;
                        }
                    }
                } else {
                    gSaveData.scanSymbols[gSaveData.scanSymbolCount].x         = ev->x;
                    gSaveData.scanSymbols[gSaveData.scanSymbolCount].y         = ev->y;
                    gSaveData.scanSymbols[gSaveData.scanSymbolCount].state     = 1;
                    gSaveData.scanSymbols[gSaveData.scanSymbolCount].encounter = ev->encounterId;
                    gSaveData.scanSymbols[gSaveData.scanSymbolCount].category = (s16)Field_GetEncounterCategory(ev->encounter);
                    gSaveData.scanSymbols[gSaveData.scanSymbolCount].mode     = 0;
                    if (gSaveData.scanSymbols[gSaveData.scanSymbolCount].category >= 9 &&
                        gSaveData.scanSymbols[gSaveData.scanSymbolCount].category < 0x16)
                    {
                        gSaveData.scanSymbols[gSaveData.scanSymbolCount].unk_10 = -1;
                        gSaveData.scanSymbols[gSaveData.scanSymbolCount].unk_0C = 0;
                    } else {
                        gSaveData.scanSymbols[gSaveData.scanSymbolCount].unk_10 = i;
                        gSaveData.scanSymbols[gSaveData.scanSymbolCount].unk_0C = 1;
                    }
                    gSaveData.scanSymbolCount = gSaveData.scanSymbolCount + 1;
                }
            }
        }
        ev += 1;
        i += 1;
    } while (i < n);
}

static void Fld_Luck_AddPlacedSymbols(void) {
    s16           i;
    FldLuckEntry* base;
    s16           n;
    FldLuckSpawn* sp;

    base = gSaveData.unk_310C;
    if (base == NULL) {
        return;
    }
    i = 0;
    for (;;) {
        s16 kind = base[i].unk_0;

        if (kind == 0x46) {
            return;
        }
        if (gSaveData.fieldArea == kind && base[i].unk_8 == func_ov030_020aec08(base[i].unk_4)) {
            break;
        }
        i += 1;
    }

    n  = base[i].count;
    sp = base[i].spawns;
    for (i = 0; i < n; sp++, i++) {
        s32 encounter = sp->encounter;

        if (encounter == -1) {
            gSaveData.scanSymbols[i].encounter = FieldNoise_RollEncounter(0);
            if (gSaveData.scanSymbols[i].encounter == -1) {
                if (gSaveData.unk_2488 != 0) {
                    break;
                }
                gSaveData.scanSymbols[i].encounter = 1;
            }
        } else {
            gSaveData.scanSymbols[i].encounter = encounter;
        }
        gSaveData.scanSymbols[i].x        = sp->x;
        gSaveData.scanSymbols[i].y        = sp->y;
        gSaveData.scanSymbols[i].unk_10   = sp->gate;
        gSaveData.scanSymbols[i].unk_0C   = 0;
        gSaveData.scanSymbols[i].state    = 1;
        gSaveData.scanSymbols[i].category = Field_GetEncounterCategory(gSaveData.scanSymbols[i].encounter);
        gSaveData.scanSymbols[i].mode     = 0;
        if (gSaveData.scanSymbols[i].category <= 0x17) {
            gSaveData.scanSymbols[i].category += 0x1F;
        }
        if (gSaveData.scanSymbols[i].unk_10 != -1 && FldGate_Get(sp->gate)) {
            gSaveData.scanSymbols[i].state = 0;
        }
    }
    gSaveData.scanSymbolCount = i;
}

void Fld_Luck_GenerateSymbols(void) {
    gSaveData.scanSymbolCount = 0;
    Fld_Luck_AddPlacedSymbols();
    Fld_Luck_RollRandomSymbols(0);
    Fld_Luck_RollRandomSymbols(1);
    Fld_Luck_AddScanEventSymbols();
    if (gSaveData.fieldArea != 0x29) {
        return;
    }
    if ((s16)gSaveData.unk_26B4 < 4) {
        return;
    }
    Fld_Luck_AddSpecialSymbol();
}

void Fld_Luck_RetireBattledSymbols(void) {
    s32 i;
    s16 changed;

    changed = 0;
    for (i = 0; i < gSaveData.scanSymbolCount; i++) {
        if (gSaveData.scanSymbols[i].state == 2) {
            gSaveData.scanSymbols[i].state = 0;
            changed += 1;
            if (gSaveData.unk_3100 == gSaveData.fieldArea) {
                if (gSaveData.unk_30FE != 0x27 && gSaveData.unk_30FE != gSaveData.scanSymbols[i].category) {
                } else {
                    gSaveData.unk_30FC += 1;
                }
            }
            if (gSaveData.scanSymbols[i].unk_0C == 1) {
                func_ov030_020aec58(gSaveData.scanSymbols[i].unk_10);
            } else if (gSaveData.scanSymbols[i].unk_10 != -1) {
                FldGate_Set(gSaveData.scanSymbols[i].unk_10);
            }
        }
    }
    if (changed >= gSaveData.unk_3102) {
        gSaveData.unk_3102 = changed;
    }
}

s32 Fld_Luck_IsScanEventAvailable(void) {
    return Fld_Luck_HasScanEventSymbol();
}

void Fld_Luck_RestoreSelectedSymbols(void) {
    s32 i;

    for (i = 0; i < gSaveData.scanSymbolCount; i++) {
        if (gSaveData.scanSymbols[i].state == 2) {
            gSaveData.scanSymbols[i].state = 1;
        }
    }
}

s16 Fld_Luck_GetNormalSymbolPercent(void) {
    s16 count;
    s32 i;
    s32 good;

    count = gSaveData.scanSymbolCount;
    if (count == 0) {
        return 0;
    }
    good = 0;
    for (i = 0; i < count; i++) {
        if (gSaveData.scanSymbols[i].state == 1) {
            s16 category = gSaveData.scanSymbols[i].category;

            if (category >= 9 && category < 0x16) {
                return -1;
            }
            good += 1;
        }
    }
    return (good * 0x64) / count;
}

static SpriteFrameInfo* Fld_Luck_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode)
}

static void Fld_Luck_LoadDigit(Sprite* sprite, s32 dataType, s16 animIndex, s16 posX, s16 posY) {
    SpriteAnimation anim = Fld_Luck_Anim;

    anim.dataType  = dataType;
    anim.animIndex = animIndex;
    anim.posX      = posX;
    anim.posY      = posY;
    _Sprite_Load(sprite, &anim);
}

static void Fld_Luck_SetDigits(FldLuckDigit* obj, s32 value, s32 big) {
    s32     val;
    s16     x;
    s32     i;
    Sprite* sp;
    s32     step;

    val = value;
    if (val > 0x1869F) {
        val = 0x1869F;
    }
    step = 1;
    if (big != 0) {
        step = 0xC;
    }
    sp = obj->sprites;
    x  = 0xEF;
    i  = 0;
    while (val / 10 != 0 || val % 10 != 0) {
        if ((s32)obj->unk_184 > i) {
            Sprite_ChangeAnimation(sp, obj->sprites[i].animData, (s16)(val % 10 + step), obj->sprites[i].cellTable);
        } else {
            Fld_Luck_LoadDigit(sp, obj->unk_180, (s16)(val % 10 + step), x, 0xB8);
        }
        val = val / 10;
        x -= 7;
        sp += 1;
        i += 1;
    }
    obj->unk_184 = i;
}

static s32 Fld_Luck_Init(TaskPool* pool, Task* task, void* args) {
    FldLuckDigit* obj = task->data;

    obj->unk_180 = ((FldLuckDigitArgs*)args)->dataType;
    obj->unk_188 = 0;
    obj->unk_18C = ((FldLuckDigitArgs*)args)->value;
    obj->unk_184 = 0;
    Fld_Luck_SetDigits(obj, obj->unk_188, 0);
    Fld_Luck_LoadDigit(&obj->extra, obj->unk_180, 0xB, (s16)(0xEF - ((obj->unk_184 + 1) * 7)), 0xB8);
    obj->unk_194 = 0;
    obj->unk_190 = 0;
    return 1;
}

static s32 Fld_Luck_Update(TaskPool* pool, Task* task, void* args) {
    FldLuckDigit* obj = task->data;

    if (obj->unk_190 != 0 && obj->unk_188 != obj->unk_18C) {
        obj->unk_194 = (s16)(obj->unk_194 + 1);
        if (Field_IsBattlePending() != 0) {
            obj->unk_188 = obj->unk_18C;
        } else if (!(obj->unk_194 & 1)) {
            s32 target = obj->unk_18C;
            s32 step   = target / 40;

            if (step == 0) {
                obj->unk_188 = obj->unk_188 + 1;
            } else {
                s32 diff = target - obj->unk_188;

                if (diff < step) {
                    obj->unk_188 = obj->unk_188 + diff;
                } else {
                    obj->unk_188 = obj->unk_188 + step;
                }
            }
        }
        Fld_Luck_SetDigits(obj, obj->unk_188, 1);
        obj->extra.posX = (s16)(0xEF - ((obj->unk_184 + 1) * 7));
    }
    {
        s32 i = 0;

        if (obj->unk_184 > 0) {
            Sprite* sp = obj->sprites;

            do {
                Sprite_Update(sp);
                i += 1;
                sp += 1;
            } while (i < obj->unk_184);
        }
    }
    Sprite_Update(&obj->extra);
    return 1;
}

static s32 Fld_Luck_Render(TaskPool* pool, Task* task, void* args) {
    FldLuckDigit* obj = task->data;
    s32           i;

    if (obj->unk_190 == 0) {
        return 1;
    }
    i = 0;
    if (obj->unk_184 > 0) {
        Sprite* sp = obj->sprites;

        do {
            Sprite_RenderFrame(sp);
            i += 1;
            sp += 1;
        } while (i < obj->unk_184);
    }
    Sprite_RenderFrame(&obj->extra);
    return 1;
}

static s32 Fld_Luck_Destroy(TaskPool* pool, Task* task, void* args) {
    FldLuckDigit* obj = task->data;
    s32           i;

    i = 0;
    if (obj->unk_184 > 0) {
        Sprite* sp = obj->sprites;

        do {
            Sprite_Release(sp);
            i += 1;
            sp += 1;
        } while (i < obj->unk_184);
    }
    Sprite_Release(&obj->extra);
    return 1;
}

static s32 Fld_Luck_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Luck_Init,
        .update     = Fld_Luck_Update,
        .render     = Fld_Luck_Render,
        .cleanup    = Fld_Luck_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Luck_CreateTask(TaskPool* pool, s32 dataType, s32 value) {
    FldLuckDigitArgs params;

    params.dataType = dataType;
    params.value    = value;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Luck, NULL, 0, NULL, &params);
}

static void Fld_Luck_SetVisible(TaskPool* pool, u32 id, s32 arg2) {
    FldLuckDigit* obj = EasyTask_GetTaskById(pool, id)->data;

    if (arg2 != 0) {
        SndMgr_StartPlayingSE(0x115);
    }
    obj->unk_190 = arg2;
}

void Fld_Luck_SetTarget(TaskPool* pool, u32 id, s32 arg2) {
    FldLuckDigit* obj = EasyTask_GetTaskById(pool, id)->data;

    obj->unk_18C = arg2;
}
