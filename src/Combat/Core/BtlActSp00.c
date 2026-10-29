#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/Memory.h"
#include "SndMgr.h"
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>
#include <stddef.h>

// Shared status storage still comes from retail assembly.
typedef struct BtlActSpArgs {
    union {
        struct {
            u16 character, level;
        };
        u32 selection;
    };
    s32 skipFeature;
} BtlActSpArgs;

typedef struct BtlActSp {
    /* 0x00 */ u32 taskId;
    /* 0x04 */ void (*update)(struct BtlActSp*);
    /* 0x08 */ s16 phase;
    /* 0x0A */ s16 timer;
    /* 0x0C */ union {
        struct {
            u16 character, level;
        };
        u32 selection;
    };
    /* 0x10 */ s32 skipFeature;
    /* 0x14 */ s32 active;
} BtlActSp;

typedef struct BtlActSpCutins {
    const TaskHandle* entries[3];
} BtlActSpCutins;
typedef struct BtlActSpFeatures {
    const TaskHandle* entries[3][3];
} BtlActSpFeatures;

typedef char check_ActSpArgs[(sizeof(BtlActSpArgs) == 8) ? 1 : -1];
typedef char check_ActSp[(sizeof(BtlActSp) == 0x18) ? 1 : -1];
typedef char check_ActSp_phase[(offsetof(BtlActSp, phase) == 8) ? 1 : -1];
typedef char check_ActSp_selection[(offsetof(BtlActSp, selection) == 0xC) ? 1 : -1];
typedef char check_ActSp_active[(offsetof(BtlActSp, active) == 0x14) ? 1 : -1];

extern const TaskHandle       data_ov003_020ddaac;
extern const BtlActSpCutins   data_ov003_020ddab8;
extern const TaskStages       data_ov003_020ddac4;
extern const BtlActSpFeatures data_ov003_020ddaf8;
typedef struct BtlActSpAttackTable {
    u16 entries[3][3];
} BtlActSpAttackTable;
extern const BtlActSpAttackTable data_ov003_020ddad4, data_ov003_020ddae6;
extern const TaskHandle          data_ov024_020f6b7c;
typedef struct BtlActSpStatus {
    s8  level;
    s8  character;
    u8  reserved[6];
    s32 released;
} BtlActSpStatus;
typedef char                  check_ActSpStatus[(sizeof(BtlActSpStatus) == 0xC) ? 1 : -1];
typedef char                  check_ActSpStatus_released[(offsetof(BtlActSpStatus, released) == 8) ? 1 : -1];
extern BtlActSpStatus         data_ov003_020e71d8;
extern const Ov003AttackInfo* func_ov003_0208a114(u16);
extern void                   func_ov003_0208a128(const Ov003AttackInfo*, Ov003AttackArgs*, Ov003Unit*, s32);
extern void                   func_ov003_0208a1e8(const Ov003AttackInfo*, Ov003AttackArgs*, s32);
extern void                   func_ov003_0208b3cc(s32, s16);
extern void                   func_ov003_020bd2a8(s32);
extern u32                    func_ov003_020be6d0(TaskPool*);
void                          func_ov003_020c14ec(void);
void                          func_ov003_020c17bc(void);
void                          func_ov003_020c1848(BtlActSp*);
void                          func_ov003_020c1918(BtlActSp*);

s32 func_ov003_020c1dbc(TaskPool*, Task*, void*);
s32 func_ov003_020c1e44(TaskPool*, Task*, void*);
s32 func_ov003_020c1e68(TaskPool*, Task*, void*);
s32 func_ov003_020c1e70(TaskPool*, Task*, void*);
s32 func_ov003_020c1ed0(TaskPool*, Task*, void*, s32);

extern const TaskHandle data_ov003_020dd838, data_ov003_020dd978, data_ov003_020dd984;
extern const TaskHandle Tsk_BtlFeat00, Tsk_BtlFeat01, Tsk_BtlFeat02;
extern const TaskHandle data_ov005_020f0570, data_ov005_020f0708, data_ov005_020f0764;
extern const TaskHandle data_ov006_020f0ac0, data_ov006_020f0b60, data_ov006_020f0d0c;

char                 data_ov003_020e53e4[16] = "Tsk_BtlActSp00";
const TaskHandle     data_ov003_020ddaac     = {data_ov003_020e53e4, func_ov003_020c1ed0, sizeof(BtlActSp)};
const BtlActSpCutins data_ov003_020ddab8     = {
    {&data_ov003_020dd838, &data_ov003_020dd978, &data_ov003_020dd984}
};
const TaskStages data_ov003_020ddac4 = {func_ov003_020c1dbc, func_ov003_020c1e44, func_ov003_020c1e68, func_ov003_020c1e70};
const BtlActSpAttackTable data_ov003_020ddae6 = {
    {{0x21, 0x23, 0x25}, {0x27, 0x29, 0x2B}, {0x2D, 0x2F, 0x31}}
};
const BtlActSpAttackTable data_ov003_020ddad4 = {
    {{0x22, 0x24, 0x26}, {0x28, 0x2A, 0x2C}, {0x2E, 0x30, 0x32}}
};
const BtlActSpFeatures data_ov003_020ddaf8 = {
    {{&Tsk_BtlFeat00, &Tsk_BtlFeat01, &Tsk_BtlFeat02},
     {&data_ov005_020f0570, &data_ov005_020f0708, &data_ov005_020f0764},
     {&data_ov006_020f0ac0, &data_ov006_020f0b60, &data_ov006_020f0d0c}}
};

void func_ov003_020c1224(BtlActSp* self, void (*update)(BtlActSp*)) {
    self->update = update;
    self->phase  = 0;
    self->timer  = 0;
}

// Nonmatching: attack-loop and signed healing arithmetic register allocation.
void func_ov003_020c1238(u16 character, u16 level) {
    BtlActSpAttackTable subAttacks  = data_ov003_020ddad4;
    BtlActSpAttackTable mainAttacks = data_ov003_020ddae6;
    CombatActor*        mainActor   = data_ov003_020e71b8->unk3D898;
    Ov003AttackArgs     args;
    args.unk_00     = mainActor->unk_66;
    args.unk_04     = 0x64;
    args.unk_06     = 0x64;
    args.unk_02     = 0x5A;
    Ov003Unit* unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    if (unit) {
        u16 attack = subAttacks.entries[character][level];
        do {
            CombatActor* owner = data_ov003_020e71b8->unk3D89C;
            func_ov003_0208a128(func_ov003_0208a114(attack), &owner->baseArgs, unit, owner->position.x);
            unit = func_ov003_02084984(&unit->node);
        } while (unit);
    }
    unit = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    if (unit) {
        u16 attack = mainAttacks.entries[character][level];
        do {
            CombatActor* owner = data_ov003_020e71b8->unk3D898;
            func_ov003_0208a128(func_ov003_0208a114(attack), &args, unit, owner->position.x);
            unit = func_ov003_02084984(&unit->node);
        } while (unit);
    }
    // Retail callers supply level 0..2; the tables use the same range.
    s16 heal;
    switch (level) {
        case 0:
            heal = data_ov003_020e71b8->unk3D8D2 * 20 / 100;
            break;
        case 1:
            heal = data_ov003_020e71b8->unk3D8D2 * 50 / 100;
            break;
        case 2:
            heal = data_ov003_020e71b8->unk3D8D2 * 100 / 100;
            break;
    }
    s32 missing = data_ov003_020e71b8->unk3D8D2 - data_ov003_020e71b8->unk3D8D0;
    if (missing < heal)
        heal = missing;
    s32 rounded = heal + ((u32)heal >> 31);
    func_ov003_0208b3cc(0, (heal & 1) + (rounded >> 1));
    func_ov003_0208b3cc(1, (s16)((rounded << 15) >> 16));
    CombatActor_ClearPeriodicEffect(data_ov003_020e71b8->unk3D89C);
    CombatActor_ClearPeriodicEffect(data_ov003_020e71b8->unk3D898);
}

// Nonmatching: display-setting store scheduling.
void func_ov003_020c14ec(void) {
    g_DisplaySettings.controls[0].brightness = 0x10;
    g_DisplaySettings.controls[1].brightness = 0x10;
    Display_CommitSynced();
    g_DisplaySettings.unk_000 = 0;
    REG_POWER_CNT &= ~0x8000;
    g_DisplaySettings.controls[0].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[0].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[0].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);
    g_DisplaySettings.engineState[0].bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[0].bgSettings[1].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[0].bgSettings[1].screenBase     = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].charBase       = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].extPlttSlot    = 0;
    REG_BG1CNT                                                    = (u16)((REG_BG1CNT & 0x43) | 0x4004);
    g_DisplaySettings.engineState[0].bgSettings[2].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[2].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[0].bgSettings[2].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[0].bgSettings[2].screenBase     = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].charBase       = 3;
    g_DisplaySettings.engineState[0].bgSettings[2].extPlttSlot    = 1;
    REG_BG2CNT                                                    = (u16)((REG_BG2CNT & 0x43) | 0x420C);
    g_DisplaySettings.engineState[0].bgSettings[3].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[3].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    g_DisplaySettings.engineState[0].bgSettings[3].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[0].bgSettings[3].screenBase     = 4;
    g_DisplaySettings.engineState[0].bgSettings[3].charBase       = 1;
    g_DisplaySettings.engineState[0].bgSettings[3].extPlttSlot    = 1;
    REG_BG3CNT                                                    = (u16)((REG_BG3CNT & 0x43) | 0x404);
    g_DisplaySettings.engineState[0].bgSettings[0].priority       = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].priority       = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].priority       = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority       = 3;
    g_DisplaySettings.engineState[0].bgSettings[0].mosaic         = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic         = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic         = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic         = 0;
    g_DisplaySettings.controls[0].layers                          = 0x1F;
    g_DisplaySettings.controls[1].bgMode                          = GX_BGMODE_0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    g_DisplaySettings.engineState[1].bgSettings[0].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[0].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[1].bgSettings[0].colorMode      = GX_BG_COLORS_256;
    g_DisplaySettings.engineState[1].bgSettings[0].screenBase     = 6;
    g_DisplaySettings.engineState[1].bgSettings[0].charBase       = 5;
    g_DisplaySettings.engineState[1].bgSettings[0].extPlttSlot    = 0;
    REG_BG0CNT_SUB                                                = (u16)((REG_BG0CNT_SUB & 0x43) | 0x4694);
    g_DisplaySettings.engineState[1].bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[1].bgSettings[1].colorMode      = GX_BG_COLORS_256;
    g_DisplaySettings.engineState[1].bgSettings[1].screenBase     = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].charBase       = 1;
    g_DisplaySettings.engineState[1].bgSettings[1].extPlttSlot    = 0;
    REG_BG1CNT_SUB                                                = (u16)((REG_BG1CNT_SUB & 0x43) | 0x4084);
    g_DisplaySettings.engineState[1].bgSettings[2].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[1].bgSettings[2].colorMode      = GX_BG_COLORS_256;
    g_DisplaySettings.engineState[1].bgSettings[2].screenBase     = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].charBase       = 3;
    g_DisplaySettings.engineState[1].bgSettings[2].extPlttSlot    = 1;
    REG_BG2CNT_SUB                                                = (u16)((REG_BG2CNT_SUB & 0x43) | 0x428C);
    g_DisplaySettings.engineState[1].bgSettings[3].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[3].screenSizeText = GX_BG_SIZE_TEXT_256x256;
    g_DisplaySettings.engineState[1].bgSettings[3].colorMode      = GX_BG_COLORS_256;
    g_DisplaySettings.engineState[1].bgSettings[3].screenBase     = 4;
    g_DisplaySettings.engineState[1].bgSettings[3].charBase       = 5;
    g_DisplaySettings.engineState[1].bgSettings[3].extPlttSlot    = 1;
    REG_BG3CNT_SUB                                                = (u16)((REG_BG3CNT_SUB & 0x43) | 0x494);
    g_DisplaySettings.engineState[1].bgSettings[0].priority       = 1;
    g_DisplaySettings.engineState[1].bgSettings[1].priority       = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].priority       = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority       = 3;
    g_DisplaySettings.engineState[1].bgSettings[0].mosaic         = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic         = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic         = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic         = 0;
    g_DisplaySettings.controls[1].layers                          = 0x1E;
    g_DisplaySettings.engineState[0].blendMode                    = 1;
    g_DisplaySettings.engineState[0].blendLayer0                  = 1;
    g_DisplaySettings.engineState[0].blendLayer1                  = 0x3E;
    g_DisplaySettings.engineState[0].blendCoeff0                  = 6;
    g_DisplaySettings.engineState[0].blendCoeff1                  = 0xA;
    g_DisplaySettings.engineState[1].blendMode                    = 1;
    g_DisplaySettings.engineState[1].blendLayer0                  = 1;
    g_DisplaySettings.engineState[1].blendLayer1                  = 0x3E;
    g_DisplaySettings.engineState[1].blendCoeff0                  = 6;
    g_DisplaySettings.engineState[1].blendCoeff1                  = 0xA;
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowObj            = 8;
    g_DisplaySettings.engineState[0].windowObjEffects     = 0;
    g_DisplaySettings.engineState[0].windowOutside        = 0x17;
    g_DisplaySettings.engineState[0].windowOutsideEffects = 0;
    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].windowObj            = 8;
    g_DisplaySettings.engineState[1].windowObjEffects     = 0;
    g_DisplaySettings.engineState[1].windowOutside        = 0x17;
    g_DisplaySettings.engineState[1].windowOutsideEffects = 0;
    Display_Commit();
}

void func_ov003_020c17bc(void) {
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 0x3E;
    g_DisplaySettings.engineState[0].blendCoeff0 = 6;
    g_DisplaySettings.engineState[0].blendCoeff1 = 0xA;
    g_DisplaySettings.engineState[1].blendMode   = 1;
    g_DisplaySettings.engineState[1].blendLayer0 = 1;
    g_DisplaySettings.engineState[1].blendLayer1 = 0x3E;
    g_DisplaySettings.engineState[1].blendCoeff0 = 6;
    g_DisplaySettings.engineState[1].blendCoeff1 = 0xA;
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowObj            = 0;
    g_DisplaySettings.engineState[0].windowObjEffects     = 0;
    g_DisplaySettings.engineState[0].windowOutside        = 0x1F;
    g_DisplaySettings.engineState[0].windowOutsideEffects = 0;
    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].windowObj            = 0;
    g_DisplaySettings.engineState[1].windowObjEffects     = 0;
    g_DisplaySettings.engineState[1].windowOutside        = 0x1F;
    g_DisplaySettings.engineState[1].windowOutsideEffects = 0;
    Display_Commit();
}

// Nonmatching: fade-stage register allocation.
void func_ov003_020c1848(BtlActSp* self) {
    s16 phase = self->phase;
    switch (phase) {
        case 0:
            SndMgr_PlaySEWithPan(0x1E, 0x80);
            EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
            data_ov003_020e71b8->unk3D878 |= 0x1000;
            data_ov003_020e71b8->unk3D878 |= 0x2000;
            self->phase++;
            return;
        case 1:
            if (data_ov003_020e71b8->unk3D878 & 1)
                return;
            if (data_ov003_020e71b8->unk3D7C0[0].unk_02 & 1)
                return;
            if (data_ov003_020e71b8->unk3D7C0[1].unk_02 & 1)
                return;
            func_ov003_020c14ec();
            func_ov003_020c1224(self, func_ov003_020c1918);
            return;
    }
}

void func_ov003_020c1918(BtlActSp* self) {
    BtlActSpCutins   cutins   = data_ov003_020ddab8;
    BtlActSpFeatures features = data_ov003_020ddaf8;
    switch (self->phase) {
        case 0:
            PaletteMgr_UnknownLoop(g_PaletteManagers[1]);
            data_ov003_020e71b8->unk3D878 |= 0x10000;
            data_ov003_020e71b8->unk3D884 = 1;
            data_ov003_020e71b8->unk3D8C8 = 0;
            EasyTask_CleanupAllTasks(&data_ov003_020e71b8->unk_201A4);
            Mem_ValidateSequences(&gMainHeap);
            if (self->character == 3)
                self->taskId = func_ov003_020be6d0(&data_ov003_020e71b8->unk_34230);
            else
                self->taskId =
                    EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, cutins.entries[self->level], NULL, 0, NULL, NULL);
            self->phase++;
            func_ov003_020bd2a8(1);
            func_ov003_020bd114();
            return;
        case 1:
            if (EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_34230, &self->taskId))
                return;
            data_ov003_020e71b8->unk3D884 = 0;
            data_ov003_020e71b8->unk3D888 = 1;
            data_ov003_020e71b8->unk3D878 |= 4;
            func_ov003_020bd020();
            if (self->skipFeature) {
                self->phase++;
                return;
            }
            if (self->character == 3)
                self->taskId = EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, &data_ov024_020f6b7c, NULL, 0, NULL, NULL);
            else
                self->taskId = EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230,
                                                   features.entries[self->character][self->level], NULL, 0, NULL, NULL);
            self->phase++;
            self->timer = 2;
            return;
        case 2:
            if (EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_34230, &self->taskId))
                return;
            data_ov003_020e71d8.released = 1;
            self->phase++;
            self->timer--;
            if (self->timer)
                return;
            PaletteMgr_UnknownLoop(g_PaletteManagers[1]);
            return;
        case 3:
            if (EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_34230, &self->taskId))
                return;
            data_ov003_020e71b8->unk3D878 &= ~4;
            func_ov003_020bd078();
            data_ov003_020e71b8->unk3D888 = 0;
            self->phase++;
            self->timer = 0;
            data_ov003_020e71b8->unk3D878 &= ~0x1000;
            data_ov003_020e71b8->unk3D878 &= ~0x2000;
            func_ov003_02083e50(1);
            func_ov003_020842e0();
            if (self->character == 3)
                return;
            EasyFade_FadeBothDisplays(0, 0, 0x800);
            return;
        case 4:
            if (self->character == 3) {
                if (!self->timer) {
                    Ov003Unit*   unit  = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
                    CombatActor* owner = data_ov003_020e71b8->unk3D89C;
                    func_ov003_0208a1e8(func_ov003_0208a114(0x12D), &unit->actor.baseArgs, owner->position.x);
                    unit  = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
                    owner = data_ov003_020e71b8->unk3D898;
                    func_ov003_0208a1e8(func_ov003_0208a114(0x12E), &unit->actor.baseArgs, owner->position.x);
                }
                data_ov003_020e71b8->unk3D878 &= ~0x10000;
                self->active = 0;
                EasyFade_FadeBothDisplays(0, 0, 0x800);
                self->timer++;
                return;
            }
            if (!self->skipFeature)
                func_ov003_020c1238(self->character, self->level);
            else
                func_ov003_020c17bc();
            data_ov003_020e71b8->unk3D878 &= ~0x10000;
            self->active = 0;
            return;
    }
}

// Nonmatching: argument-load scheduling and register allocation.
s32 func_ov003_020c1dbc(TaskPool* pool, Task* task, void* rawArgs) {
    BtlActSp*     self = task->data;
    BtlActSpArgs* args = rawArgs;
    data_ov003_020e71b8->unk3D878 |= 2;
    MI_CpuSet(self, 0, sizeof(*self));
    data_ov003_020e71d8.released  = 0;
    self->selection               = args->selection;
    self->skipFeature             = args->skipFeature;
    data_ov003_020e71d8.level     = args->level;
    data_ov003_020e71d8.character = self->character;
    self->active                  = 1;
    func_ov003_020c1224(self, func_ov003_020c1848);
    return 1;
}

s32 func_ov003_020c1e44(TaskPool* pool, Task* task, void* args) {
    BtlActSp* self = task->data;
    if (self->update)
        self->update(self);
    return self->active;
}
s32 func_ov003_020c1e68(TaskPool* pool, Task* task, void* args) {
    return 1;
}
s32 func_ov003_020c1e70(TaskPool* pool, Task* task, void* args) {
    EasyTask_CleanupAllTasks(&data_ov003_020e71b8->unk_34230);
    func_ov003_020bd078();
    data_ov003_020e71b8->unk3D878 &= ~2;
    data_ov003_020e71b8->unk3D878 &= ~0x20000;
    Mem_ValidateSequences(&gMainHeap);
    return 1;
}
s32 func_ov003_020c1ed0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020ddac4;
    return stages.iter[stage](pool, task, args);
}

void func_ov003_020c1f18(void* pool, u16 level) {
    BtlActSpArgs args;
    u16          character;
    switch (data_ov003_020e71b8->unk3D88D) {
        case 1:
            character = 1;
            break;
        case 2:
            character = 2;
            break;
        default:
        case 0:
            character = 0;
            break;
    }
    args.level       = level;
    args.character   = character;
    args.skipFeature = 0;
    EasyTask_CreateTask(pool, &data_ov003_020ddaac, NULL, 0, NULL, &args);
}
s32 func_ov003_020c1f94(TaskPool* pool, s32 character) {
    BtlActSpArgs args;
    switch (character) {
        case 1:
            args.character = 1;
            break;
        case 2:
            args.character = 2;
            break;
        default:
        case 0:
            args.character = 0;
            break;
    }
    args.level       = 1;
    args.skipFeature = 1;
    return EasyTask_CreateTask(pool, &data_ov003_020ddaac, NULL, 0, NULL, &args);
}
s32 func_ov003_020c2008(TaskPool* pool) {
    BtlActSpArgs args;
    args.character     = 3;
    args.level         = 0;
    args.skipFeature   = 0;
    CombatActor* owner = data_ov003_020e71b8->unk3D89C;
    owner->flags &= ~8;
    return EasyTask_CreateTask(pool, &data_ov003_020ddaac, NULL, 0, NULL, &args);
}
