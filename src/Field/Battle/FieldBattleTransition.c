#include "Engine/Core/System.h"
#include "Engine/IO/Str.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Events.h"
#include "Field/TaskHelpers.h"
#include "Save.h"

extern s32       func_ov030_020ae2c8(FieldEventManager*);
extern const s32 data_ov030_020da778[44];

extern s32  func_ov030_02084788(FieldEventManager*);
extern s32  func_ov030_020847c8(FieldEventManager*);
extern s8*  func_ov030_020b0534(s16 area);
extern void func_02011e94(void* dst, s32 arg1, s32 arg2, void* src);

void FieldScan_MarkSeen(s32 id) {
    FieldEventManager* field = data_ov030_02109d60;

    for (s32 i = 0; i < 32; i++) {
        if (field->scanSeenIds[i] == -1) {
            field->scanSeenIds[i] = id;
            return;
        }
        if (id == field->scanSeenIds[i]) {
            return;
        }
    }
}

BOOL FieldScan_WasSeen(s32 id) {
    FieldEventManager* field = data_ov030_02109d60;

    if (id == -1) {
        for (s32 i = 0; i < 32; i++) {
            if (field->scanSeenIds[i] != -1) {
                return TRUE;
            }
        }
        return FALSE;
    }

    for (s32 i = 0; i < 32; i++) {
        if (id == field->scanSeenIds[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL FieldScan_IsTouchActive(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (!TouchInput_IsTouchActive()) {
        return FALSE;
    }
    if (func_ov030_02084788(field)) {
        return FALSE;
    }
    return field->scanPhase == 3;
}

BOOL Field_IsMoveTouchActive(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (!TouchInput_IsTouchActive()) {
        return FALSE;
    }
    if (func_ov030_020b3058(EasyTask_GetTaskById(&field->taskPool, field->taskId_Player))) {
        return FALSE;
    }
    if (func_ov030_020847c8(field)) {
        return FALSE;
    }
    return field->scanPhase == 5;
}

BOOL Field_IsMoveInputActive(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (SysControl.buttonState.currButtons & (INPUT_DPAD | INPUT_ABXY)) {
        if (func_ov030_020847c8(field)) {
            return FALSE;
        }
        return field->scanPhase == 5;
    }
    return Field_IsMoveTouchActive();
}

s32 func_ov030_020aef2c(void) {
    return func_ov030_020ae2c8(data_ov030_02109d60);
}

BOOL FieldScan_IsEnteringBattle(void) {
    if (data_ov030_02109d60->scanPhase == 6) {
        return TRUE;
    }
    if (data_ov030_02109d60->scanPhase == 7) {
        return TRUE;
    }
    return FALSE;
}

BOOL FieldScan_IsBattleStarting(void) {
    return data_ov030_02109d60->scanPhase == 7;
}

BOOL Field_IsBattlePending(void) {
    return data_ov030_02109d60->exitRequest == 2;
}

BOOL FieldScan_IsOpen(void) {
    return data_ov030_02109d60->scanPhase != 5;
}

s32 Field_IsTalkEventActive(void) {
    return data_ov030_02109d60->talkEventActive;
}

BOOL FieldScan_IsClosing(void) {
    return data_ov030_02109d60->scanPhase == 4;
}

BOOL func_ov030_020af01c(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (func_ov030_020b484c(EasyTask_GetTaskById(&field->taskPool, field->taskId_ScanArea))) {
        return FALSE;
    }
    if (field->unk_21A5C == 1) {
        return TRUE;
    }
    return gSaveData.unk_2498 == 0;
}

BOOL Field_IsIdle(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (field->unk_21AC8 != 0) {
        return FALSE;
    }
    if (field->unk_21AAC != 0) {
        return FALSE;
    }
    if (func_ov030_020b484c(EasyTask_GetTaskById(&field->taskPool, field->taskId_ScanArea))) {
        return FALSE;
    }
    if (gSaveData.unk_2498 != 0) {
        return FALSE;
    }
    return field->exitRequest == 0;
}

void FieldDebug_DrawAreaName(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (field->taskId_DebugText != -1) {
        void* data = EasyTask_GetTaskById(&field->taskPool, field->taskId_DebugText)->data;
        char* src  = func_ov030_020b0534(gSaveData.fieldArea);

        s32 i;
        for (i = 0; i < 5; i++) {
            field->debugAreaName[i] = src[i];
        }
        field->debugAreaName[i] = 0;
        func_02011e94(data + 0x10, 0, 0, field->debugAreaName);
    }
}

void FieldDebug_DrawPlayerInfo(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (field->taskId_DebugText != -1) {
        void* data = EasyTask_GetTaskById(&field->taskPool, field->taskId_DebugText)->data;

        func_02011e94(data + 0x10, 0, 24,
                      Str_SPrintf("player:%04d,%03d", F2I(gSaveData.fieldPos.x), F2I(gSaveData.fieldPos.y)));

        s32 value = func_ov030_020aeca4(gSaveData.fieldPos.y);
        func_02011e94(data + 0x10, 0, 32, Str_SPrintf(" Scale:%03d.%02d", F2I(value), F2I(value * 100) - F2I(value) * 100));
        func_02011e94(data + 0x10, 0, 40,
                      Str_SPrintf("camera:%04d,%03d", F2I(field->origin.x) + 128, F2I(field->origin.y) + 96));
        func_02011e94(data + 0x10, 0, 56, Str_SPrintf("NPC %04d / %04d", field->drawnHumanCount, field->humanCount));
    }
}

s32 FieldScan_GetRadius(void) {
    FieldEventManager* field = data_ov030_02109d60;

    return func_ov030_020b4858(EasyTask_GetTaskById(&field->taskPool, field->taskId_ScanArea));
}

s32 FieldScan_IsActive(void) {
    return data_ov030_02109d60->scanActive;
}

s32 func_ov030_020af2fc(void) {
    s16 area = gSaveData.fieldArea;

    if ((u32)area >= 44) {
        return 25;
    }
    return data_ov030_020da778[area];
}
