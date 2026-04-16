#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Friend/Private/Shiki.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern BinIdentifier data_ov003_020d7808;

typedef struct {
    /* 0x00 */ u16  unk_00;
    /* 0x02 */ char unk_02[0xCA - 0x2];
    /* 0xCA */ u16  unk_CA;
    /* 0xCC */ s16  unk_CC;
    /* 0xCE */ s16  unk_CE;
    /* 0xD0 */ s16  unk_D0;
    /* 0xD4 */ char unk_D4[0xDC - 0xD4];
    /* 0xDC */ s32  unk_DC;
} EspUnk;

typedef struct BtlCmd_Esp {
    /* 0x00 */ u16          unk_00;
    /* 0x04 */ CombatSprite sprite;
    /* 0x64 */ s32          unk_64;
    /* 0x68 */ s32          unk_68;
    /* 0x6C */ s32          unk_6C;
    /* 0x70 */ s32          unk_70;
    /* 0x74 */ void (*unk_74)(struct BtlCmd_Esp*);
    /* 0x78 */ s16 unk_78;
    /* 0x7A */ s16 unk_7A;
    /* 0x7C */ s32 unk_7C;
    /* 0x80 */ s32 unk_80;
} BtlCmd_Esp; // Size: 0x84

typedef struct {
    /* 0x0 */ u16 unk_0;
    /* 0x4 */ s32 unk_4;
    /* 0x8 */ s32 unk_8;
} BtlCmd_Esp_Args;

void func_ov004_020ea930(BtlCmd_Esp* cmd);
void func_ov004_020ea9cc(BtlCmd_Esp* cmd);
s32  BtlCmd_Esp_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlCmd_Esp = {"Tsk_BtlCmd_Esp", BtlCmd_Esp_RunTask, 0x84};

void func_ov004_020ea634(BtlCmd_Esp* cmd, void (*arg1)(BtlCmd_Esp*)) {
    cmd->unk_74 = arg1;
    cmd->unk_7A = 0;
    cmd->unk_78 = 0;
}

void func_ov004_020ea648(CombatSprite* arg0, u16 arg1) {
    SpriteAnimationEx sp0;

    CombatSprite_InitAnim(&sp0.anim, 1, &data_ov003_020d7808);
    sp0.anim.bits_10_11 = 0;
    sp0.anim.unk_18     = 2;
    sp0.anim.packIndex  = 2;
    sp0.anim.unk_1C     = 4;
    sp0.anim.unk_26     = 5;
    sp0.anim.unk_28     = 6;
    sp0.anim.unk_20     = 7;
    sp0.anim.unk_2A     = arg1 + 1;
    sp0.unk_2C          = 0;
    CombatSprite_Load(arg0, &sp0);
    CombatSprite_SetAnim(arg0, arg1, 0);
    func_ov003_02082738(arg0, 0);
    func_ov003_02082730(arg0, 2);
}

s32 func_ov004_020ea6f0(EspUnk* arg0, u16 arg1) {
    if (arg0->unk_00 != arg1) {
        arg0->unk_CA |= 4;
        return 0;
    }
    arg0->unk_CA |= 1;
    if ((arg0->unk_CA & 4) == 0) {
        arg0->unk_CA |= 8;
    }
    return 1;
}

s32 func_ov004_020ea730(u16 arg0) {
    s16*    temp_r0 = func_ov003_02086e94();
    EspUnk* var_r5  = func_ov003_0208495c(temp_r0 + 2);
    while (var_r5 != NULL) {
        if (var_r5->unk_CC == *temp_r0) {
            if (func_ov004_020ea6f0(var_r5, arg0) != 0) {
                return 1;
            }
        } else if ((var_r5->unk_CC > *temp_r0) && (var_r5->unk_00 == arg0)) {
            var_r5->unk_CA |= 0x10;
        }
        var_r5 = func_ov003_02084984(&var_r5->unk_DC);
    }
    return 0;
}

s32 func_ov004_020ea7ac(void) {
    EspUnk* var_r0 = func_ov003_0208495c(func_ov003_02086e94() + 4);
    while (var_r0 != NULL) {
        if ((var_r0->unk_CA & 2) == 0 && (var_r0->unk_CA & 1)) {
            return 0;
        }
        var_r0 = func_ov003_02084984(&var_r0->unk_DC);
    }
    return 1;
}

s32 func_ov004_020ea7f4(void) {
    s16* temp_r0 = func_ov003_02086e94();
    s32  var_r5  = 0;

    EspUnk* temp = func_ov003_0208495c(temp_r0 + 2);
    while (temp != NULL) {
        if (temp->unk_CC == *temp_r0) {
            if (temp->unk_CA & 8) {
                var_r5 = func_ov003_02086f20(2, temp->unk_CE);
            } else {
                var_r5 = func_ov003_02086f20(1, temp->unk_CE);
            }
            break;
        } else {
            temp = func_ov003_02084984(&temp->unk_DC);
        }
    }
    *temp_r0 += 1;
    return var_r5;
}

void func_ov004_020ea878(BtlCmd_Esp* cmd) {
    s16*    temp_r0;
    EspUnk* temp_r0_2;

    if (cmd->unk_78 == 0) {
        cmd->unk_78 = 0x10;
    }
    if (cmd->unk_78 > 0) {
        temp_r0   = func_ov003_02086e94();
        temp_r0_2 = func_ov004_020e742c(temp_r0, *temp_r0);
        if (temp_r0_2 != NULL) {
            func_020265d4(&cmd->unk_64, temp_r0_2->unk_CE << 0xC, (u16)cmd->unk_78);
            func_020265d4(&cmd->unk_68, temp_r0_2->unk_D0 << 0xC, (u16)cmd->unk_78);
        }
        cmd->unk_78--;
    }

    if (cmd->unk_78 <= 0) {
        if (func_ov004_020ea730(cmd->unk_00) != 0) {
            cmd->unk_7C = 0;
            func_ov004_020ea634(cmd, func_ov004_020ea930);
        } else {
            func_ov004_020ea634(cmd, func_ov004_020ea9cc);
        }
    }
}

void func_ov004_020ea930(BtlCmd_Esp* cmd) {
    switch (cmd->unk_7A) {
        case 0: {
            if (func_ov004_020ea7ac()) {
                cmd->unk_7A++;
            }
        } break;

        case 1: {
            if (func_ov004_020ea7f4() == 0) {
                cmd->unk_80 = 0;
            } else {
                EasyFade_FadeBothDisplays(FADER_SMOOTH, 0x10, 0x1000);
                cmd->unk_7A++;
                cmd->unk_78 = 0;
            }
        } break;

        case 2: {
            EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
            cmd->unk_80 = 0;
        } break;
    }
}

void func_ov004_020ea9cc(BtlCmd_Esp* cmd) {
    switch (cmd->unk_7A) {
        case 0: {
            CombatSprite_SetAnim(&cmd->sprite, cmd->unk_00 + 10, 0);
            cmd->unk_7A++;
        } break;

        case 1: {
            cmd->unk_70 += 0x800;
            cmd->unk_6C += cmd->unk_70;
            if (cmd->unk_6C >= 0x20000) {
                cmd->unk_80 = 0;
            }
        } break;
    }
}

s32 BtlCmd_Esp_Init(TaskPool* pool, Task* task, void* args) {
    BtlCmd_Esp*      cmd     = task->data;
    BtlCmd_Esp_Args* cmdArgs = args;

    MI_CpuSet(cmd, 0, sizeof(BtlCmd_Esp));
    cmd->unk_7C = 1;
    cmd->unk_80 = 1;
    cmd->unk_00 = cmdArgs->unk_0;
    cmd->unk_64 = cmdArgs->unk_4;
    cmd->unk_68 = cmdArgs->unk_8;
    cmd->unk_6C = 0;
    cmd->unk_70 = 0;
    func_ov004_020ea648(&cmd->sprite, cmdArgs->unk_0 * 2);
    func_ov004_020ea634(cmd, func_ov004_020ea878);
    return 1;
}

s32 BtlCmd_Esp_Update(TaskPool* pool, Task* task, void* args) {
    BtlCmd_Esp* cmd = task->data;

    if (data_ov003_020e71b8->unk3D878 & 6) {
        return 0;
    }
    if (cmd->unk_74 != NULL) {
        cmd->unk_74(cmd);
    }
    CombatSprite_Update(&cmd->sprite);
    return cmd->unk_80;
}

s32 BtlCmd_Esp_Render(TaskPool* pool, Task* task, void* args) {
    BtlCmd_Esp* cmd = task->data;

    if (cmd->unk_7C != 0) {
        CombatSprite_SetPosition(&cmd->sprite, F2Ix(cmd->unk_64), F2Ix(cmd->unk_68 + cmd->unk_6C));
        CombatSprite_Render(&cmd->sprite);
    }
    return 1;
}

s32 BtlCmd_Esp_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlCmd_Esp* cmd = task->data;

    CombatSprite_Release(&cmd->sprite);
    return 1;
}

s32 BtlCmd_Esp_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = BtlCmd_Esp_Init,
        .update     = BtlCmd_Esp_Update,
        .render     = BtlCmd_Esp_Render,
        .cleanup    = BtlCmd_Esp_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void BtlCmd_Esp_CreateTask(s16 arg0, s32 arg1, s32 arg2) {
    BtlCmd_Esp_Args args;
    args.unk_4 = arg1;
    args.unk_8 = arg2;
    args.unk_0 = arg0;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &Tsk_BtlCmd_Esp, NULL, 0, NULL, &args);
}
