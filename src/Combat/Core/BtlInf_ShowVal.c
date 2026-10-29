#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/File/DatMgr.h"
#include "Player/Inventory.h"
#include "Player/Inventory/Items.h"
#include <nitro/mi/cpumem.h>
#include <stdarg.h>
#include <stddef.h>

// Function recovery; the animation scripts and field resource tables allocated
// after the task descriptor remain in retail assembly until recovered.
typedef struct BtlShowValArgs {
    /* 0x00 */ const u16* mainValues[6];
    /* 0x18 */ const u16* subValues[6];
    /* 0x30 */ u16        count;
    /* 0x32 */ u16        reserved;
} BtlShowValArgs;

typedef struct BtlShowVal {
    /* 0x000 */ CombatSprite   mainSprite;
    /* 0x060 */ CombatSprite   subSprite;
    /* 0x0C0 */ BtlShowValArgs args;
    /* 0x0F4 */ s32            mainDigits[6][4];
    /* 0x154 */ u16            mainLengths[6];
    /* 0x160 */ s32            subDigits[6][4];
    /* 0x1C0 */ u16            subLengths[6];
    /* 0x1CC */ u16            state;
    /* 0x1CE */ u16            reserved;
    /* 0x1D0 */ void (*update)(struct BtlShowVal*);
    /* 0x1D4 */ s16 timer;
    /* 0x1D6 */ s16 phase;
    /* 0x1D8 */ s32 visible;
} BtlShowVal;

typedef char check_ShowValArgs[(sizeof(BtlShowValArgs) == 0x34) ? 1 : -1];
typedef char check_ShowVal[(sizeof(BtlShowVal) == 0x1DC) ? 1 : -1];
typedef char check_ShowVal_args[(offsetof(BtlShowVal, args) == 0xC0) ? 1 : -1];
typedef char check_ShowVal_mainDigits[(offsetof(BtlShowVal, mainDigits) == 0xF4) ? 1 : -1];
typedef char check_ShowVal_mainLengths[(offsetof(BtlShowVal, mainLengths) == 0x154) ? 1 : -1];
typedef char check_ShowVal_subDigits[(offsetof(BtlShowVal, subDigits) == 0x160) ? 1 : -1];
typedef char check_ShowVal_subLengths[(offsetof(BtlShowVal, subLengths) == 0x1C0) ? 1 : -1];
typedef char check_ShowVal_update[(offsetof(BtlShowVal, update) == 0x1D0) ? 1 : -1];
typedef char check_ShowVal_timer[(offsetof(BtlShowVal, timer) == 0x1D4) ? 1 : -1];
typedef char check_ShowVal_visible[(offsetof(BtlShowVal, visible) == 0x1D8) ? 1 : -1];

extern const BinIdentifier data_ov003_020d7800;
extern const BinIdentifier data_ov003_020da744;
extern char                data_ov003_020e3e50[];
extern const TaskHandle    data_ov003_020d8880;
extern const TaskStages    data_ov003_020d888c;
extern s32                 func_02023b7c(u16);
extern void                func_ov003_020a30d4(s32, s32, u32);

extern u8        data_ov003_020d9a80[];
extern u8        data_ov003_020d9a90[];
extern u8        data_ov003_020d9aa8[];
extern void      func_ov003_02083470(void);
extern void      func_ov003_02083478(void);
extern void      func_ov003_020835d4(void);
extern void      func_ov003_02083700(void);
extern const u32 data_ov003_020d88ac[];
extern const u32 data_ov003_020d88d8[];
extern const u32 data_ov003_020d88f0[];
extern const u32 data_ov003_020d8908[];
extern const u32 data_ov003_020d8920[];
extern const u32 data_ov003_020d8938[];
extern const u32 data_ov003_020d8950[];
extern const u32 data_ov003_020d8968[];
extern const u32 data_ov003_020d8980[];
extern const u32 data_ov003_020d89b0[];
extern const u32 data_ov003_020d89c8[];
extern const u32 data_ov003_020d89e0[];
extern const u32 data_ov003_020d89f8[];
extern const u16 data_ov003_020d8c18[];
extern const u16 data_ov003_020d8c42[];
extern const u16 data_ov003_020d8c6c[];
extern const u16 data_ov003_020d8c96[];
extern const u32 data_ov003_020d91d4[];
extern const u32 data_ov003_020d9210[];
extern const u32 data_ov003_020d924c[];
extern const u32 data_ov003_020d9288[];
extern const u32 data_ov003_020d92c4[];
extern const u32 data_ov003_020d9300[];
extern const u32 data_ov003_020d933c[];
extern const u32 data_ov003_020d9378[];
extern const u32 data_ov003_020d93b4[];
extern const u32 data_ov003_020d93f0[];
extern const u32 data_ov003_020d942c[];
extern const u32 data_ov003_020d9468[];
extern const u32 data_ov003_020d94a4[];
extern const u32 data_ov003_020d94e0[];
extern const u32 data_ov003_020d951c[];
extern const u32 data_ov003_020d9558[];
extern const u32 data_ov003_020d9594[];
extern const u32 data_ov003_020d95d0[];
extern const u32 data_ov003_020d960c[];
extern const u32 data_ov003_020d9648[];
extern const u32 data_ov003_020d9684[];
extern const u32 data_ov003_020d96c0[];
extern const u32 data_ov003_020d96fc[];
extern const u32 data_ov003_020d9738[];
extern const u32 data_ov003_020d9774[];
extern const u32 data_ov003_020d97b0[];
extern const u32 data_ov003_020d97ec[];
extern const u32 data_ov003_020d9828[];
extern const u32 data_ov003_020d9864[];
extern const u32 data_ov003_020d98a0[];
extern const u32 data_ov003_020d98dc[];
extern const u32 data_ov003_020d9918[];
extern const u32 data_ov003_020d9954[];
extern const u32 data_ov003_020d9990[];
extern const u32 data_ov003_020d99cc[];
extern const u32 data_ov003_020d9a08[];
extern const u32 data_ov003_020d9a44[];

void func_ov003_020a2a88(BtlShowVal* self, void (*update)(BtlShowVal*)) {
    self->update = update;
    self->timer  = 0;
    self->phase  = 0;
}

s32 func_ov003_020a2aa0(s32* digits, s32 value) {
    s32 remainder = value;
    digits[3]     = remainder / 1000;
    remainder %= 1000;
    digits[2] = remainder / 100;
    remainder %= 100;
    digits[1] = remainder / 10;
    digits[0] = remainder % 10;
    if (value >= 1000)
        return 4;
    if (value >= 100)
        return 3;
    return value >= 10 ? 2 : 1;
}

void func_ov003_020a2b54(BtlShowVal* self) {
    u16 i;
    for (i = 0; i < self->args.count; i++) {
        const u16* mainValue = self->args.mainValues[i];
        const u16* subValue  = self->args.subValues[i];
        if (mainValue)
            self->mainLengths[i] = func_ov003_020a2aa0(self->mainDigits[i], *mainValue);
        if (subValue)
            self->subLengths[i] = func_ov003_020a2aa0(self->subDigits[i], *subValue);
    }
}

s32 func_ov003_020a2bdc(TaskPool* pool, Task* task, void* args) {
    BtlShowVal* self = task->data;
    MI_CpuSet(self, 0, sizeof(*self));
    self->args                    = *(BtlShowValArgs*)args;
    data_ov003_020e71b8->unk3D8F8 = 0;
    self->visible                 = 1;
    self->phase                   = 0;
    self->timer                   = 0;
    self->state                   = 0;
    func_ov003_020a2a88(self, func_ov003_020a2b54);
    CombatSprite_LoadDirect(1, &self->mainSprite, &data_ov003_020d7800, 0x25, 0x27, 0x26, 0x3D, 0x14);
    func_ov003_02082738(&self->mainSprite, 0);
    func_ov003_02082730(&self->mainSprite, 0);
    CombatSprite_LoadDirect(0, &self->subSprite, &data_ov003_020d7800, 0x25, 0x27, 0x26, 0x3D, 0x14);
    func_ov003_02082738(&self->subSprite, 0);
    func_ov003_02082730(&self->subSprite, 0);
    return 1;
}

s32 func_ov003_020a2d08(TaskPool* pool, Task* task, void* args) {
    BtlShowVal* self = task->data;
    if (self->update)
        self->update(self);
    return 1;
}

// Nonmatching: loop and register allocation are under comparison.
s32 func_ov003_020a2d28(TaskPool* pool, Task* task, void* args) {
    BtlShowVal* self = task->data;
    if (!self->visible)
        return 1;
    s16 y = 0xAA;
    u16 i;
    for (i = 0; i < self->args.count; i++) {
        if (!self->args.mainValues[i]) {
            y -= 0x10;
            continue;
        }
        {
            s16 x = 0xEC;
            u16 digit;
            for (digit = 0; digit < self->mainLengths[i]; digit++) {
                CombatSprite_SetAnim(&self->mainSprite, (s16)(self->mainDigits[i][digit] + 0x14), 0);
                CombatSprite_SetPosition(&self->mainSprite, x, y);
                CombatSprite_Update(&self->mainSprite);
                CombatSprite_Render(&self->mainSprite);
                x -= 8;
            }
        }
        y -= 0x10;
    }
    s16 subY = 0x9A;
    u16 subIndex;
    for (subIndex = 0; subIndex < self->args.count; subIndex++) {
        if (!self->args.subValues[subIndex]) {
            subY -= 0x10;
            continue;
        }
        {
            s16 x = 0xEC;
            u16 digit;
            for (digit = 0; digit < self->subLengths[subIndex]; digit++) {
                CombatSprite_SetAnim(&self->subSprite, (s16)(self->subDigits[subIndex][digit] + 0x14), 0);
                CombatSprite_SetPosition(&self->subSprite, x, subY);
                CombatSprite_Update(&self->subSprite);
                CombatSprite_Render(&self->subSprite);
                x -= 8;
            }
        }
        subY -= 0x10;
    }
    return 1;
}

s32 func_ov003_020a2f14(TaskPool* pool, Task* task, void* args) {
    BtlShowVal* self = task->data;
    CombatSprite_Release(&self->mainSprite);
    CombatSprite_Release(&self->subSprite);
    return 1;
}

s32 func_ov003_020a2f34(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d888c;
    return stages.iter[stage](pool, task, args);
}

s32 func_ov003_020a2f7c(TaskPool* pool, ...) {
    BtlShowValArgs args;
    const u16*     mainValue;
    const u16*     subValue;
    u16            count;
    va_list        values;
    va_start(values, pool);
    mainValue = va_arg(values, const u16*);
    subValue  = va_arg(values, const u16*);
    count     = 0;
    while (mainValue || subValue) {
        args.mainValues[count] = mainValue;
        args.subValues[count]  = subValue;
        count++;
        mainValue = va_arg(values, const u16*);
        subValue  = va_arg(values, const u16*);
    }
    va_end(values);
    args.count = count;
    return EasyTask_CreateTask(pool, &data_ov003_020d8880, NULL, 0, NULL, &args);
}

void func_ov003_020a300c(void) {
    Ov003TouchEvent* events = data_ov003_020e71b8->touchEvents;
    s32              i;
    for (i = 0; i < 14; i++)
        events[i].unk_00 = 0xFF;
}

void func_ov003_020a303c(const u16* items, s32 touchIndex) {
    RawItemData item;
    s32         i;
    for (i = 0; i < 4; i++) {
        if (func_02023b7c(items[i])) {
            DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, &item, sizeof(item), &data_ov003_020da744,
                                                            Inventory_GetCategorizedIndex(items[i]) * sizeof(item)));
            if (item.unk_10 != 0xFF)
                func_ov003_020a30d4(item.unk_10, touchIndex, -1);
        }
    }
}

const TaskHandle data_ov003_020d8880 = {data_ov003_020e3e50, func_ov003_020a2f34, sizeof(BtlShowVal)};
const TaskStages data_ov003_020d888c = {
    func_ov003_020a2bdc,
    func_ov003_020a2d08,
    func_ov003_020a2d28,
    func_ov003_020a2f14,
};

// Background resource scripts and the file names they are read from (`BtlField_RG`/`BtlField_UG` resource tables).
const u32 data_ov003_020d889c[4]  = {(u32)func_ov003_02083478, 0x00030001, 0x000B0004, (u32)func_ov003_02083470};
const u32 data_ov003_020d88ac[5]  = {0x00010041, 0x00420040, 0x00400001, 0xFFFF0000, 0};
const u32 data_ov003_020d88c0[6]  = {(u32)func_ov003_02083700, (u32)data_ov003_020d9558, 1, 0x01900000, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d88d8[6]  = {0x0001002A, 0x002B0020, 0x00200001, 0x0001002C, 0x00000020, 0x0000FFFF};
const u32 data_ov003_020d88f0[6]  = {0x00010036, 0x00370040, 0x00080001, 0x00010038, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d8908[6]  = {0x0001000C, 0x000D0020, 0x00200001, 0x0001000E, 0x00000080, 0x0000FFFF};
const u32 data_ov003_020d8920[6]  = {0x0001003C, 0x003D0040, 0x00400001, 0x0001003E, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d8938[6]  = {0x00010046, 0x0047000C, 0x000C0001, 0x00010048, 0x0000000C, 0x0000FFFF};
const u32 data_ov003_020d8950[6]  = {0x0001004C, 0x004D000A, 0x000A0001, 0x0001004E, 0x0000000A, 0x0000FFFF};
const u32 data_ov003_020d8968[6]  = {0x00010012, 0x00130008, 0x00080001, 0x00010014, 8, 0x0000FFFF};
const u32 data_ov003_020d8980[6]  = {0x00010018, 0x00190060, 0x00600001, 0x0001001A, 0x00000060, 0x0000FFFF};
const u32 data_ov003_020d8998[6]  = {(u32)func_ov003_02083700, (u32)data_ov003_020d8c96, 2, 0x01800000, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d89b0[6]  = {0x00010024, 0x00250010, 0x00100001, 0x00010026, 0x00000010, 0x0000FFFF};
const u32 data_ov003_020d89c8[6]  = {0x0001001E, 0x001F0010, 0x00100001, 0x00010020, 0x00000010, 0x0000FFFF};
const u32 data_ov003_020d89e0[6]  = {0x00010004, 0x00060008, 0x00080001, 0x00010008, 8, 0x0000FFFF};
const u32 data_ov003_020d89f8[6]  = {0x00010030, 0x00310040, 0x00400001, 0x00010032, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d8a10[10] = {
    (u32)func_ov003_02083478, 0x00060003, 0x000B0004, (u32)func_ov003_020835d4, 0x00060001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x00060002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8a38[10] = {
    (u32)func_ov003_02083478, 0x00080003, 0x000B0004, (u32)func_ov003_020835d4, 0x00080001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x00080002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8a60[10] = {
    (u32)func_ov003_02083478, 0x000E0003, 0x000B0004, (u32)func_ov003_020835d4, 0x000E0001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x000E0002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8a88[10] = {
    (u32)func_ov003_02083478, 0x000D0003, 0x000B0004, (u32)func_ov003_020835d4, 0x000D0001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x000D0002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8ab0[10] = {
    (u32)func_ov003_02083478, 0x000C0003, 0x000B0004, (u32)func_ov003_020835d4, 0x000C0001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x000C0002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8ad8[10] = {
    (u32)func_ov003_02083478, 0x000F0003, 0x000B0004, (u32)func_ov003_020835d4, 0x000F0001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x000F0002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8b00[10] = {
    (u32)func_ov003_02083478, 0x000A0003, 0x000B0004, (u32)func_ov003_020835d4, 0x000A0001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x000A0002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8b28[10] = {
    (u32)func_ov003_02083478, 0x00090003, 0x000B0004, (u32)func_ov003_020835d4, 0x00090001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x00090002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8b50[10] = {
    (u32)func_ov003_02083478, 0x00070003, 0x000B0004, (u32)func_ov003_020835d4, 0x00070001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x00070002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8b78[10] = {
    (u32)func_ov003_02083478, 0x00100003, 0x000B0004, (u32)func_ov003_020835d4, 0x00100001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x00100002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8ba0[10] = {
    (u32)func_ov003_02083478, 0x00040003, 0x000B0004, (u32)func_ov003_020835d4, 0x00040001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x00040002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8bc8[10] = {
    (u32)func_ov003_02083478, 0x00050003, 0x000B0004, (u32)func_ov003_020835d4, 0x00050001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x00050002, 0x01A00002, (u32)func_ov003_02083470};
const u32 data_ov003_020d8bf0[10] = {
    (u32)func_ov003_02083478, 0x000B0003, 0x000B0004, (u32)func_ov003_020835d4, 0x000B0001, 0x01A00001,
    (u32)func_ov003_020835d4, 0x000B0002, 0x01A00002, (u32)func_ov003_02083470};
const u16 data_ov003_020d8c18[21] = {0, 0xFFFE, 0, 0x41, 2,    0, 0x41, 1, 0x40,   0, 0xFFFE,
                                     0, 0x42,   2, 0,    0x42, 1, 0x40, 0, 0xFFFF, 0};
const u16 data_ov003_020d8c42[21] = {0, 0xFFFE, 0, 0x3F, 2,    0, 0x3F, 1, 0x40,   0, 0xFFFE,
                                     0, 0x40,   2, 0,    0x40, 1, 0x40, 0, 0xFFFF, 0};
const u16 data_ov003_020d8c6c[21] = {0, 0xFFFE, 0, 0x3F, 2,    0, 0x3F, 1, 0x40,   0, 0xFFFE,
                                     0, 0x40,   2, 0,    0x40, 1, 0x40, 0, 0xFFFF, 0};
const u16 data_ov003_020d8c96[21] = {0, 0xFFFE, 0, 3, 2, 0, 3, 1, 0x40, 0, 0xFFFE, 0, 4, 2, 0, 4, 1, 0x40, 0, 0xFFFF, 0};
const u32 data_ov003_020d8cc0[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d9918, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d9954, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8cec[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d97b0, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d97ec, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8d18[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d9990, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d99cc, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8d44[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d93b4, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d93f0, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8d70[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d9a08, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d9a44, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8d9c[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d942c, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d94a4, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8dc8[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d91d4, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d9210, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8df4[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d92c4, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d9300, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8e20[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d924c, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d9288, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8e4c[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d933c, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d9378, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8e78[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d9828, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d9864, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8ea4[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d98a0, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d98dc, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8ed0[11] = {(u32)func_ov003_02083700, (u32)data_ov003_020d8c42, 1, 0x01800000, 0x000A0002,
                                     (u32)func_ov003_02083700, (u32)data_ov003_020d8c18, 2, 0x01A00001, 0x000C0003,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8efc[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d94e0,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d8908,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8f34[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d951c,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d8968,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8f6c[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d9594,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d8980,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8fa4[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d95d0,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d89c8,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d8fdc[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d960c,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d89b0,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d9014[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d9648,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d88d8,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d904c[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d9684,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d89f8,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d9084[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d96c0,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d88f0,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d90bc[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d96fc,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d8920,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d90f4[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d8c6c,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d88ac,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d912c[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d9468,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d89e0,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d9164[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d9738,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d8938,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d919c[14] = {(u32)func_ov003_02083478,
                                     0x004F0001,
                                     0x000E0001,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d9774,
                                     1,
                                     0x01800000,
                                     0x000B0003,
                                     (u32)func_ov003_02083700,
                                     (u32)data_ov003_020d8950,
                                     2,
                                     0x01800001,
                                     0,
                                     (u32)func_ov003_02083470};
const u32 data_ov003_020d91d4[15] = {0xFFFE0000, 0x00270000, 2,          0x00010027, 0x00000020,
                                     0x0000FFFE, 0x00020028, 0x00280000, 0x00200001, 0xFFFE0000,
                                     0x00290000, 2,          0x00010029, 0x00000020, 0x0000FFFF};
const u32 data_ov003_020d9210[15] = {0xFFFE0000, 0x002A0000, 2,          0x0001002A, 0x00000020,
                                     0x0000FFFE, 0x0002002B, 0x002B0000, 0x00200001, 0xFFFE0000,
                                     0x002C0000, 2,          0x0001002C, 0x00000020, 0x0000FFFF};
const u32 data_ov003_020d924c[15] = {0xFFFE0000, 0x002D0000, 2,          0x0001002D, 0x00000040,
                                     0x0000FFFE, 0x0002002E, 0x002E0000, 0x00400001, 0xFFFE0000,
                                     0x002F0000, 2,          0x0001002F, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d9288[15] = {0xFFFE0000, 0x00300000, 2,          0x00010030, 0x00000040,
                                     0x0000FFFE, 0x00020031, 0x00310000, 0x00400001, 0xFFFE0000,
                                     0x00320000, 2,          0x00010032, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d92c4[15] = {0xFFFE0000, 0x00330000, 2,          0x00010033, 0x00000040,
                                     0x0000FFFE, 0x00020034, 0x00340000, 0x00080001, 0xFFFE0000,
                                     0x00350000, 2,          0x00010035, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d9300[15] = {0xFFFE0000, 0x00360000, 2,          0x00010036, 0x00000040,
                                     0x0000FFFE, 0x00020037, 0x00370000, 0x00080001, 0xFFFE0000,
                                     0x00380000, 2,          0x00010038, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d933c[15] = {0xFFFE0000, 0x00390000, 2,          0x00010039, 0x00000040,
                                     0x0000FFFE, 0x0002003A, 0x003A0000, 0x00400001, 0xFFFE0000,
                                     0x003B0000, 2,          0x0001003B, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d9378[15] = {0xFFFE0000, 0x003C0000, 2,          0x0001003C, 0x00000040,
                                     0x0000FFFE, 0x0002003D, 0x003D0000, 0x00400001, 0xFFFE0000,
                                     0x003E0000, 2,          0x0001003E, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d93b4[15] = {0xFFFE0000, 0x00430000, 2,          0x00010043, 0x0000000C,
                                     0x0000FFFE, 0x00020044, 0x00440000, 0x000C0001, 0xFFFE0000,
                                     0x00450000, 2,          0x00010045, 0x0000000C, 0x0000FFFF};
const u32 data_ov003_020d93f0[15] = {0xFFFE0000, 0x00460000, 2,          0x00010046, 0x0000000C,
                                     0x0000FFFE, 0x00020047, 0x00470000, 0x000C0001, 0xFFFE0000,
                                     0x00480000, 2,          0x00010048, 0x0000000C, 0x0000FFFF};
const u32 data_ov003_020d942c[15] = {0xFFFE0000, 0x00490000, 2,          0x00010049, 0x0000000A,
                                     0x0000FFFE, 0x0002004A, 0x004A0000, 0x000A0001, 0xFFFE0000,
                                     0x004B0000, 2,          0x0001004B, 0x0000000A, 0x0000FFFF};
const u32 data_ov003_020d9468[15] = {0xFFFE0000, 0x00030000, 2, 0x00010003, 8, 0x0000FFFE, 0x00020005, 0x00050000, 0x00080001,
                                     0xFFFE0000, 0x00070000, 2, 0x00010007, 8, 0x0000FFFF};
const u32 data_ov003_020d94a4[15] = {0xFFFE0000, 0x004C0000, 2,          0x0001004C, 0x0000000A,
                                     0x0000FFFE, 0x0002004D, 0x004D0000, 0x000A0001, 0xFFFE0000,
                                     0x004E0000, 2,          0x0001004E, 0x0000000A, 0x0000FFFF};
const u32 data_ov003_020d94e0[15] = {0xFFFE0000, 0x00090000, 2,          0x00010009, 0x00000020,
                                     0x0000FFFE, 0x0002000A, 0x000A0000, 0x00200001, 0xFFFE0000,
                                     0x000B0000, 2,          0x0001000B, 0x00000080, 0x0000FFFF};
const u32 data_ov003_020d951c[15] = {0xFFFE0000, 0x000F0000, 2, 0x0001000F, 8, 0x0000FFFE, 0x00020010, 0x00100000, 0x00080001,
                                     0xFFFE0000, 0x00110000, 2, 0x00010011, 8, 0x0000FFFF};
const u32 data_ov003_020d9558[15] = {0xFFFE0000, 0x00030000, 2,          0x00010003, 0x0000000A,
                                     0x0000FFFE, 0x00020004, 0x00040000, 0x000A0001, 0xFFFE0000,
                                     0x00050000, 2,          0x00010005, 0x0000000A, 0x0000FFFF};
const u32 data_ov003_020d9594[15] = {0xFFFE0000, 0x00150000, 2,          0x00010015, 0x00000060,
                                     0x0000FFFE, 0x00020016, 0x00160000, 0x00600001, 0xFFFE0000,
                                     0x00170000, 2,          0x00010017, 0x00000060, 0x0000FFFF};
const u32 data_ov003_020d95d0[15] = {0xFFFE0000, 0x001B0000, 2,          0x0001001B, 0x00000010,
                                     0x0000FFFE, 0x0002001C, 0x001C0000, 0x00100001, 0xFFFE0000,
                                     0x001D0000, 2,          0x0001001D, 0x00000010, 0x0000FFFF};
const u32 data_ov003_020d960c[15] = {0xFFFE0000, 0x00210000, 2,          0x00010021, 0x00000010,
                                     0x0000FFFE, 0x00020022, 0x00220000, 0x00100001, 0xFFFE0000,
                                     0x00230000, 2,          0x00010023, 0x00000010, 0x0000FFFF};
const u32 data_ov003_020d9648[15] = {0xFFFE0000, 0x00270000, 2,          0x00010027, 0x00000020,
                                     0x0000FFFE, 0x00020028, 0x00280000, 0x00200001, 0xFFFE0000,
                                     0x00290000, 2,          0x00010029, 0x00000020, 0x0000FFFF};
const u32 data_ov003_020d9684[15] = {0xFFFE0000, 0x002D0000, 2,          0x0001002D, 0x00000040,
                                     0x0000FFFE, 0x0002002E, 0x002E0000, 0x00400001, 0xFFFE0000,
                                     0x002F0000, 2,          0x0001002F, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d96c0[15] = {0xFFFE0000, 0x00330000, 2,          0x00010033, 0x00000040,
                                     0x0000FFFE, 0x00020034, 0x00340000, 0x00080001, 0xFFFE0000,
                                     0x00350000, 2,          0x00010035, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d96fc[15] = {0xFFFE0000, 0x00390000, 2,          0x00010039, 0x00000040,
                                     0x0000FFFE, 0x0002003A, 0x003A0000, 0x00400001, 0xFFFE0000,
                                     0x003B0000, 2,          0x0001003B, 0x00000140, 0x0000FFFF};
const u32 data_ov003_020d9738[15] = {0xFFFE0000, 0x00430000, 2,          0x00010043, 0x0000000C,
                                     0x0000FFFE, 0x00020044, 0x00440000, 0x000C0001, 0xFFFE0000,
                                     0x00450000, 2,          0x00010045, 0x0000000C, 0x0000FFFF};
const u32 data_ov003_020d9774[15] = {0xFFFE0000, 0x00490000, 2,          0x00010049, 0x0000000A,
                                     0x0000FFFE, 0x0002004A, 0x004A0000, 0x000A0001, 0xFFFE0000,
                                     0x004B0000, 2,          0x0001004B, 0x0000000A, 0x0000FFFF};
const u32 data_ov003_020d97b0[15] = {0xFFFE0000, 0x00030000, 2, 0x00010003, 8, 0x0000FFFE, 0x00020004, 0x00040000, 0x00080001,
                                     0xFFFE0000, 0x00050000, 2, 0x00010005, 8, 0x0000FFFF};
const u32 data_ov003_020d97ec[15] = {0xFFFE0000, 0x00060000, 2, 0x00010006, 8, 0x0000FFFE, 0x00020007, 0x00070000, 0x00080001,
                                     0xFFFE0000, 0x00080000, 2, 0x00010008, 8, 0x0000FFFF};
const u32 data_ov003_020d9828[15] = {0xFFFE0000, 0x00090000, 2,          0x00010009, 0x00000020,
                                     0x0000FFFE, 0x0002000A, 0x000A0000, 0x00200001, 0xFFFE0000,
                                     0x000B0000, 2,          0x0001000B, 0x00000080, 0x0000FFFF};
const u32 data_ov003_020d9864[15] = {0xFFFE0000, 0x000C0000, 2,          0x0001000C, 0x00000020,
                                     0x0000FFFE, 0x0002000D, 0x000D0000, 0x00200001, 0xFFFE0000,
                                     0x000E0000, 2,          0x0001000E, 0x00000080, 0x0000FFFF};
const u32 data_ov003_020d98a0[15] = {0xFFFE0000, 0x000F0000, 2, 0x0001000F, 8, 0x0000FFFE, 0x00020010, 0x00100000, 0x00080001,
                                     0xFFFE0000, 0x00110000, 2, 0x00010011, 8, 0x0000FFFF};
const u32 data_ov003_020d98dc[15] = {0xFFFE0000, 0x00120000, 2, 0x00010012, 8, 0x0000FFFE, 0x00020013, 0x00130000, 0x00080001,
                                     0xFFFE0000, 0x00140000, 2, 0x00010014, 8, 0x0000FFFF};
const u32 data_ov003_020d9918[15] = {0xFFFE0000, 0x00150000, 2,          0x00010015, 0x00000060,
                                     0x0000FFFE, 0x00020016, 0x00160000, 0x00600001, 0xFFFE0000,
                                     0x00170000, 2,          0x00010017, 0x00000060, 0x0000FFFF};
const u32 data_ov003_020d9954[15] = {0xFFFE0000, 0x00180000, 2,          0x00010018, 0x00000060,
                                     0x0000FFFE, 0x00020019, 0x00190000, 0x00600001, 0xFFFE0000,
                                     0x001A0000, 2,          0x0001001A, 0x00000060, 0x0000FFFF};
const u32 data_ov003_020d9990[15] = {0xFFFE0000, 0x001B0000, 2,          0x0001001B, 0x00000010,
                                     0x0000FFFE, 0x0002001C, 0x001C0000, 0x00100001, 0xFFFE0000,
                                     0x001D0000, 2,          0x0001001D, 0x00000010, 0x0000FFFF};
const u32 data_ov003_020d99cc[15] = {0xFFFE0000, 0x001E0000, 2,          0x0001001E, 0x00000010,
                                     0x0000FFFE, 0x0002001F, 0x001F0000, 0x00100001, 0xFFFE0000,
                                     0x00200000, 2,          0x00010020, 0x00000010, 0x0000FFFF};
const u32 data_ov003_020d9a08[15] = {0xFFFE0000, 0x00210000, 2,          0x00010021, 0x00000010,
                                     0x0000FFFE, 0x00020022, 0x00220000, 0x00100001, 0xFFFE0000,
                                     0x00230000, 2,          0x00010023, 0x00000010, 0x0000FFFF};
const u32 data_ov003_020d9a44[15] = {0xFFFE0000, 0x00240000, 2,          0x00010024, 0x00000010,
                                     0x0000FFFE, 0x00020025, 0x00250000, 0x00100001, 0xFFFE0000,
                                     0x00260000, 2,          0x00010026, 0x00000010, 0x0000FFFF};

const void* data_ov003_020e3f0c[1]  = {(void*)data_ov003_020d9a90};
const void* data_ov003_020e3f10[2]  = {(void*)data_ov003_020d9a80, (void*)data_ov003_020d9aa8};
char        data_ov003_020e3f18[32] = "Apl_Fur/Grp_BtlFld_Ce002RG.bin";
char        data_ov003_020e3f38[32] = "Apl_Fur/Grp_BtlFld_No009RG.bin";
char        data_ov003_020e3f58[32] = "Apl_Fur/Grp_BtlFld_Ex001RG.bin";
char        data_ov003_020e3f78[32] = "Apl_Fur/Grp_BtlFld_We004RG.bin";
char        data_ov003_020e3f98[32] = "Apl_Fur/Grp_BtlFld_We003RG.bin";
char        data_ov003_020e3fb8[32] = "Apl_Fur/Grp_BtlFld_Mk014RG.bin";
char        data_ov003_020e3fd8[32] = "Apl_Fur/Grp_BtlFld_We001RG.bin";
char        data_ov003_020e3ff8[32] = "Apl_Fur/Grp_BtlFld_So002RG.bin";
char        data_ov003_020e4018[32] = "Apl_Fur/Grp_BtlFld_So001RG.bin";
char        data_ov003_020e4038[32] = "Apl_Fur/Grp_BtlFld_Ea003RG.bin";
char        data_ov003_020e4058[32] = "Apl_Fur/Grp_BtlFld_Ea002RG.bin";
char        data_ov003_020e4078[32] = "Apl_Fur/Grp_BtlFld_Ea001RG.bin";
char        data_ov003_020e4098[32] = "Apl_Fur/Grp_BtlFld_No010RG.bin";
char        data_ov003_020e40b8[32] = "Apl_Fur/Grp_BtlFld_Ce001RG.bin";
char        data_ov003_020e40d8[32] = "Apl_Fur/Grp_BtlFld_No008RG.bin";
char        data_ov003_020e40f8[32] = "Apl_Fur/Grp_BtlFld_No006RG.bin";
char        data_ov003_020e4118[32] = "Apl_Fur/Grp_BtlFld_No005RG.bin";
char        data_ov003_020e4138[32] = "Apl_Fur/Grp_BtlFld_No004RG.bin";
char        data_ov003_020e4158[32] = "Apl_Fur/Grp_BtlFld_No003RG.bin";
char        data_ov003_020e4178[32] = "Apl_Fur/Grp_BtlFld_No002RG.bin";
char        data_ov003_020e4198[32] = "Apl_Fur/Grp_BtlFld_No001RG.bin";
char        data_ov003_020e41b8[32] = "Apl_Fur/Grp_BtlFld_Ce004RG.bin";
char        data_ov003_020e41d8[32] = "Apl_Fur/Grp_BtlFld_Ce003RG.bin";
char        data_ov003_020e41f8[32] = "Apl_Fur/Grp_BtlFld_No001UG.bin";
char        data_ov003_020e4218[32] = "Apl_Fur/Grp_BtlFld_Ce001UG.bin";
char        data_ov003_020e4238[32] = "Apl_Fur/Grp_BtlFld_Ex003UG.bin";
char        data_ov003_020e4258[32] = "Apl_Fur/Grp_BtlFld_Ex001UG.bin";
char        data_ov003_020e4278[32] = "Apl_Fur/Grp_BtlFld_We004UG.bin";
char        data_ov003_020e4298[32] = "Apl_Fur/Grp_BtlFld_We003UG.bin";
char        data_ov003_020e42b8[32] = "Apl_Fur/Grp_BtlFld_Mk014UG.bin";
char        data_ov003_020e42d8[32] = "Apl_Fur/Grp_BtlFld_We001UG.bin";
char        data_ov003_020e42f8[32] = "Apl_Fur/Grp_BtlFld_So002UG.bin";
char        data_ov003_020e4318[32] = "Apl_Fur/Grp_BtlFld_So001UG.bin";
char        data_ov003_020e4338[32] = "Apl_Fur/Grp_BtlFld_Ea003UG.bin";
char        data_ov003_020e4358[32] = "Apl_Fur/Grp_BtlFld_Ea002UG.bin";
char        data_ov003_020e4378[32] = "Apl_Fur/Grp_BtlFld_Ea001UG.bin";
char        data_ov003_020e4398[32] = "Apl_Fur/Grp_BtlFld_No010UG.bin";
char        data_ov003_020e43b8[32] = "Apl_Fur/Grp_BtlFld_No009UG.bin";
char        data_ov003_020e43d8[32] = "Apl_Fur/Grp_BtlFld_No008UG.bin";
char        data_ov003_020e43f8[32] = "Apl_Fur/Grp_BtlFld_No006UG.bin";
char        data_ov003_020e4418[32] = "Apl_Fur/Grp_BtlFld_No005UG.bin";
char        data_ov003_020e4438[32] = "Apl_Fur/Grp_BtlFld_No004UG.bin";
char        data_ov003_020e4458[32] = "Apl_Fur/Grp_BtlFld_No003UG.bin";
char        data_ov003_020e4478[32] = "Apl_Fur/Grp_BtlFld_No002UG.bin";
char        data_ov003_020e4498[32] = "Apl_Fur/Grp_BtlFld_Ce004UG.bin";
char        data_ov003_020e44b8[32] = "Apl_Fur/Grp_BtlFld_Ce003UG.bin";
char        data_ov003_020e44d8[32] = "Apl_Fur/Grp_BtlFld_Ce002UG.bin";
