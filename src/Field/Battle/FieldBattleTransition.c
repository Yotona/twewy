#include "Engine/Core/System.h"
#include "Engine/IO/Str.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Events.h"
#include "Field/TaskHelpers.h"
#include "Save.h"

// USA: interaction/mode helpers at 0x020aed54-0x020af320.
// The provisional file name groups helpers also called by battle setup.
extern s32       func_ov030_020ae2c8(FieldEventManager*);
extern const s32 data_ov030_020da778[44];

extern s32  func_ov030_02084788(FieldEventManager*);
extern s32  func_ov030_020847c8(FieldEventManager*);
extern s8*  func_ov030_020b0534(s16 area);
extern void func_02011e94(void* dst, s32 arg1, s32 arg2, void* src);

extern char data_ov030_021002ec[];
extern char data_ov030_02100300[];
extern char data_ov030_02100314[];
extern char data_ov030_02100328[];

void func_ov030_020aed54(s32 id) {
    FieldEventManager* field = data_ov030_02109d60;
    s32                i     = 0;
    do {
        s32 existing = field->unk_21B3C[i];
        if (existing == -1) {
            field->unk_21B3C[i] = id;
            return;
        }
        if (id == existing) {
            return;
        }
        i++;
    } while (i < 32);
}

BOOL func_ov030_020aed9c(s32 id) {
    FieldEventManager* field = data_ov030_02109d60;
    s32                i;

    if (id == -1) {
        for (i = 0; i < 32; i++) {
            if (field->unk_21B3C[i] != -1) {
                return 1;
            }
        }
        return 0;
    }
    for (i = 0; i < 32; i++) {
        if (id == field->unk_21B3C[i]) {
            return 1;
        }
    }
    return 0;
}

BOOL func_ov030_020aee10(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (TouchInput_IsTouchActive() == 0) {
        return 0;
    }
    if (func_ov030_02084788(field) != 0) {
        return 0;
    }
    return field->unk_21B14 == 3;
}

BOOL func_ov030_020aee5c(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (TouchInput_IsTouchActive() == 0) {
        return 0;
    }
    if (func_ov030_020b3058(EasyTask_GetTaskById(&field->taskPool, field->unk_216D4)) != 0) {
        return 0;
    }
    if (func_ov030_020847c8(field) != 0) {
        return 0;
    }
    return field->unk_21B14 == 5;
}

BOOL func_ov030_020aeecc(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (SysControl.buttonState.currButtons & 0xCF3) {
        if (func_ov030_020847c8(field) != 0) {
            return 0;
        }
        return field->unk_21B14 == 5;
    }
    return func_ov030_020aee5c();
}

s32 func_ov030_020aef2c(void) {
    return func_ov030_020ae2c8(data_ov030_02109d60);
}
BOOL func_ov030_020aef44(void) {
    s32 mode = data_ov030_02109d60->unk_21B14;
    if (mode == 6) {
        return 1;
    }
    if (mode == 7) {
        return 1;
    }
    return 0;
}
BOOL func_ov030_020aef74(void) {
    return data_ov030_02109d60->unk_21B14 == 7;
}
BOOL func_ov030_020aef98(void) {
    return data_ov030_02109d60->unk_21630 == 2;
}
BOOL func_ov030_020aefbc(void) {
    return data_ov030_02109d60->unk_21B14 != 5;
}
s32 func_ov030_020aefe0(void) {
    return data_ov030_02109d60->unk_21CF4;
}
BOOL func_ov030_020aeff8(void) {
    return data_ov030_02109d60->unk_21B14 == 4;
}

BOOL func_ov030_020af01c(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (func_ov030_020b484c(EasyTask_GetTaskById(&field->taskPool, field->unk_216B4)) != 0) {
        return 0;
    }
    if (field->unk_21A5C == 1) {
        return 1;
    }
    return gSaveData.unk_2498 == 0;
}

BOOL func_ov030_020af080(void) {
    FieldEventManager* field = data_ov030_02109d60;

    if (field->unk_21AC8 != 0) {
        return 0;
    }
    if (field->unk_21AAC != 0) {
        return 0;
    }
    if (func_ov030_020b484c(EasyTask_GetTaskById(&field->taskPool, field->unk_216B4)) != 0) {
        return 0;
    }
    if (gSaveData.unk_2498 != 0) {
        return 0;
    }
    return field->unk_21630 == 0;
}

void func_ov030_020af10c(void) {
    FieldEventManager* field = data_ov030_02109d60;
    s8*                src;
    u8*                data;
    s32                i;

    if (field->unk_22E20 == -1) {
        return;
    }
    data = EasyTask_GetTaskById(&field->taskPool, field->unk_22E20)->data;
    src  = func_ov030_020b0534(gSaveData.unk_2458);
    i    = 0;
    do {
        field->unk_22E24[i] = src[i];
        i++;
    } while (i < 5);
    field->unk_22E24[i] = 0;
    func_02011e94(data + 0x10, 0, 0, field->unk_22E24);
}

void func_ov030_020af198(void) {
    FieldEventManager* field = data_ov030_02109d60;
    u8*                data;
    s32                value;

    if (field->unk_22E20 == -1) {
        return;
    }
    data = EasyTask_GetTaskById(&field->taskPool, field->unk_22E20)->data;
    func_02011e94(data + 0x10, 0, 0x18,
                  Str_SPrintf(data_ov030_021002ec, gSaveData.fieldPos.x >> 0xC, gSaveData.fieldPos.y >> 0xC));
    value = func_ov030_020aeca4(gSaveData.fieldPos.y);
    func_02011e94(data + 0x10, 0, 0x20,
                  Str_SPrintf(data_ov030_02100300, value >> 0xC, ((value * 0x64) >> 0xC) - ((value >> 0xC) * 0x64)));
    func_02011e94(data + 0x10, 0, 0x28,
                  Str_SPrintf(data_ov030_02100314, (field->origin.x >> 0xC) + 0x80, (field->origin.y >> 0xC) + 0x60));
    func_02011e94(data + 0x10, 0, 0x38, Str_SPrintf(data_ov030_02100328, field->unk_22E38, field->unk_21918));
}

s32 func_ov030_020af2b8(void) {
    FieldEventManager* field = data_ov030_02109d60;

    return func_ov030_020b4858(EasyTask_GetTaskById(&field->taskPool, field->unk_216B4));
}

s32 func_ov030_020af2e4(void) {
    return data_ov030_02109d60->unk_21B10;
}
s32 func_ov030_020af2fc(void) {
    s16 area = gSaveData.unk_2458;
    if ((u32)area >= 44) {
        return 25;
    }
    return data_ov030_020da778[area];
}
