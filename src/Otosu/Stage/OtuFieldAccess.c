#include "Otosu/OtosuGame.h"

extern const void* func_ov040_0209d4a4(u16);
extern BOOL        func_ov040_0209d4c4(void*);

static s32            OtuScene_InputValid[6];
static OtuInputBuffer OtuScene_PlayerInputs;
static OtuInputBuffer OtuScene_LocalInput;

s32 OtuBoard_GetObstacleCount(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    return stage->obstacleCount;
}

void* OtuBoard_GetObstacle(TinPinSlammer_Scene* scene, s32 index) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    return EasyTask_GetTaskData(&scene->pool2, stage->obstacleIds[index]);
}

s32 OtuBoard_SpriteSortKey(s32 layer, s32 y, s32 z) {
    return (((layer << 0x0B) + ((y >> 0x0C) & 0x7FF)) << 0x0C);
}

s32 OtuScene_GetOwnPlayer(s32 mode) {
    s32 r = 0;

    if (mode != 0 && mode == 1) {
        r = func_ov040_0209d728();
    }

    return r;
}

OtuInputPacket* OtuScene_GetPlayerInput(s32 slot) {
    return &OtuScene_PlayerInputs.packets[slot];
}

// Nonmatching: the packet address is strength-reduced; the original multiplies i * 6 every iteration
void OtuScene_ExchangeInputs(void) {
    s32 i;

    if (func_ov040_0209cb78() == 5) {
        if (func_ov040_0209d4c4(OtuScene_LocalInput.packets) != 0) {
            for (i = 0; i < 4; i++) {
                const void* src = func_ov040_0209d4a4(i);

                if (src != NULL) {
                    MI_CpuCopyU8(src, &OtuScene_PlayerInputs.packets[i], sizeof(OtuInputPacket));
                    OtuScene_InputValid[i] = TRUE;
                } else {
                    MI_CpuSet(&OtuScene_PlayerInputs.packets[i], 0, sizeof(OtuInputPacket));
                    OtuScene_InputValid[i] = FALSE;
                }
            }
            OtuScene_InputStalled = FALSE;
        } else {
            for (i = 0; i < 4; i++) {
                OtuScene_InputValid[i] = FALSE;
            }
            OtuScene_InputStalled = TRUE;
        }
    } else {
        for (i = 0; i < 4; i++) {
            OtuScene_InputValid[i] = FALSE;
        }
        OtuScene_InputStalled = FALSE;
    }
}

void OtuScene_ReadLocalInput(OtuInputPacket* input) {
    TouchCoord coord;

    TouchInput_GetCoord(&coord);

    input->x = coord.x;
    input->y = coord.y;

    if (TouchInput_IsTouchActive()) {
        input->touching = TRUE;
    } else {
        input->touching = FALSE;
    }

    input->buttons    = SysControl.buttonState.currButtons;
    input->syncStep   = OtuScene_SyncStep;
    OtuScene_SyncStep = 0;
}

void OtuScene_UpdateLocalInput(void) {
    if (OtuScene_InputStalled == FALSE) {
        OtuScene_ReadLocalInput(OtuScene_LocalInput.packets);
    }
}

// TODO: Type of record/self/target
s32 OtuBoard_OnGuestConnected(void* record, TinPinSlammer_Scene* scene) {
    u8* self   = (u8*)record;
    u8* target = (u8*)OtuStageDispatch_GetBlock(&scene->stage);
    s32 flag;

    if (*(u16*)(self + 0x10) >= *(s32*)(target + 0x140)) {
        return 0;
    }

    if (*(u16*)(self + 0x14) == *(u16*)(target + 6)) {
        (*(u16*)(target + 6))++;

        if (*(u16*)(target + 6) < *(s32*)(target + 0x140)) {
            flag = 1;
        } else {
            flag = 0;
        }

        func_ov040_0209d788(0, 0, target + 6, 2, flag);
        return 1;
    }

    return 0;
}

void OtuBoard_OnGuestDisconnected(s32 unused, TinPinSlammer_Scene* scene) {
    scene->linkLost = 1;
}
