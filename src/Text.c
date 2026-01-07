#include "Text.h"

s32 func_020107a4(TextObject* obj, s32 param_2, s32 param_3);

void func_02010870(TextObject* obj);

void func_0200fa28(u32*, u8*, u32*, s32, s32, u32, u32*, u32*);
void func_0200fa2c(u32*, u8*, u32*, s32, s32, u32, u32*, u32*);
void func_0200fb88(u32*, u8*, u32*, s32, s32, u32, u32*, u32*);
void func_0200fad4(u32*, u8*, u32*, s32, s32, u32, u32*, u32*);
void func_0200fc70(u32*, u8*, u32*, s32, s32, u32, u32*, u32*);
s32  func_02010808(TextObject* arg0, s32 arg1, s32 arg2);

typedef void (*FuncPtr)(u32*, u8*, u32*, s32, s32, u32, u32*, u32*);
const FuncPtr data_0205aea4[20] = {
    func_0200fa28, func_0200fa28, func_0200fa28, func_0200fa28, func_0200fa2c, func_0200fb88, func_0200fa28,
    func_0200fa28, func_0200fad4, func_0200fc70, func_0200fa28, func_0200fa28, func_0200fa28, func_0200fa28,
    func_0200fa28, func_0200fa28, func_0200fa28, func_0200fa28, func_0200fa28, func_0200fa28,
};

s32 data_0205ae64;
s32 data_0205ae24;
s32 data_0205b0f4;

const u16 data_0205aef4[256] = {
    0x0,    0x1,    0x2,    0x3,    0x4,    0x5,    0x6,    0x7,    0x8,    0x9,    0xA,    0xB,    0xC,    0xD,    0xE,
    0xF,    0x10,   0x11,   0x12,   0x13,   0x14,   0x15,   0x16,   0x17,   0x18,   0x19,   0x1A,   0x1B,   0x1C,   0x1D,
    0x1E,   0x1F,   0x8140, 0x8149, 0x8160, 0x818C, 0x8190, 0x818B, 0x8195, 0x818C, 0x8169, 0x816A, 0x8196, 0x817B, 0x8143,
    0x817C, 0x8144, 0x815E, 0x824F, 0x8250, 0x8251, 0x8252, 0x8253, 0x8254, 0x8255, 0x8256, 0x8257, 0x8258, 0x8146, 0x8147,
    0x8183, 0x8181, 0x8184, 0x8148, 0x8197, 0x8260, 0x8261, 0x8262, 0x8263, 0x8264, 0x8265, 0x8266, 0x8267, 0x8268, 0x8269,
    0x826A, 0x826B, 0x826C, 0x826D, 0x826E, 0x826F, 0x8270, 0x8271, 0x8272, 0x8273, 0x8274, 0x8275, 0x8276, 0x8277, 0x8278,
    0x8279, 0x816D, 0x818F, 0x816E, 0x814F, 0x8151, 0x824D, 0x8281, 0x8282, 0x8283, 0x8284, 0x8285, 0x8286, 0x8287, 0x8288,
    0x8289, 0x828A, 0x828B, 0x828C, 0x828D, 0x828E, 0x828F, 0x8290, 0x8291, 0x8292, 0x8293, 0x8294, 0x8295, 0x8296, 0x8297,
    0x8298, 0x8299, 0x829A, 0x8167, 0x8162, 0x8168, 0x8164, 0x8140, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159, 0x8159,
    0x8159,
};

u16 func_0200f94c(s32 idx) {
    return data_0205aef4[idx];
}

u16 func_0200f960(u32 arg0, u8* arg1) {
    u16 offset      = ((arg0 & 0x7F00) >> 6);
    u16 base_offset = arg1[offset] | (arg1[offset + 1] << 8);
    u16 width       = (arg1[offset + 3] << 8) | arg1[offset + 2];
    u16 result      = 0;

    if (base_offset != 0xFFFF) {
        u8 char_width = arg1[(u16)(base_offset + ((arg0 & 0xFF) - 0x40))];
        if (char_width != 0xFF) {
            result = char_width + width;
        }
    }

    return result;
}

u16 func_0200f9e0(s32 arg0, u8* arg1, u8* arg2, u32 arg3, u16 arg4) {
    u16 var_r4 = 0;

    if (arg2 != 0) {
        u32 idx = func_0200f960(arg0, arg1);
        if (idx < arg3) {
            var_r4 = arg2[idx];
        }
    }
    if (var_r4 == 0) {
        var_r4 = arg4;
    }
    return var_r4;
}

u32 func_0200fa10(u32 param_1, u8* param_2, u32 param_3, u32 param_4) {
    return param_4 * func_0200f960(param_1, param_2) + param_3;
}

void func_0200fa28(u32*, u8*, u32*, s32, s32, u32, u32*, u32*) {
    return;
}

void func_0200fa2c(u32*, u8*, u32*, s32, s32, u32, u32*, u32*) {
    // Not yet implemented
}

void func_0200fad4(u32*, u8*, u32*, s32, s32, u32, u32*, u32*) {
    // Not yet implemented
}

void func_0200fb88(u32*, u8*, u32*, s32, s32, u32, u32*, u32*) {
    // Not yet implemented
}

void func_0200fc70(u32*, u8*, u32*, s32, s32, u32, u32*, u32*) {
    // Not yet implemented
}

s32 func_0200fd64(UnkMedText* arg0) {
    s32 temp_r5;
    s32 var_ip;

    s32 var_r4 = arg0->unk_10;
    if (var_r4 > 8) {
        var_r4 = 8;
    }
    var_ip  = arg0->unk_14;
    temp_r5 = arg0->unk_0C;
    if (var_ip > 8) {
        var_ip = 8;
    }
    data_0205aea4[(arg0->unk_04 + (arg0->unk_02 * 4))](arg0->unk_1C, arg0->unk_18, arg0->unk_20, var_r4, var_ip, arg0->unk_08,
                                                       (temp_r5 * 0xC) + ((var_r4 * 0x60) + (s32)&data_0205b0f4),
                                                       &data_0205ae24 + (temp_r5 * 8));

    return ((var_r4 + temp_r5 + 7) & ~7) >> 3;
}

s32 func_0200fe08(TextObject* obj, s32 xOffset, s32 yOffset, u32 arg3, u32 arg4, u16 arg5) {
    // Not yet implemented
}

u16 func_02010170(TextObject* obj) {
    if (obj->unk_64 != obj->unk_66) {
        u16 val = obj->unk_68[obj->unk_66];
        obj->unk_66++;
        obj->unk_66 = obj->unk_66 & 0xff;
        return val;
    }
    return 0;
}

BOOL func_020101ac(TextObject* obj, u32 arg1) {
    u32 iVar1 = func_0200fa10(arg1, obj->unk_50, obj->unk_54, obj->unk_46);
    u32 uVar1 = func_0200f9e0(arg1, obj->unk_50, obj->unk_58, obj->unk_5C, obj->unk_60);

    func_0200fe08(obj, obj->xOffset, obj->yOffset, iVar1, uVar1, obj->unk_4E);
    if (obj->unk_00 == 0) {
        obj->xOffset += uVar1;
    } else {
        obj->yOffset += obj->unk_4E;
    }
    if ((obj->unk_10[0] != 0) && (obj->unk_70 == 0)) {
        if (obj->unk_00 == 0) {
            func_0200fe08(obj, obj->xOffset, obj->yOffset, obj->unk_2C, obj->unk_10[0], obj->unk_4E);
            obj->xOffset += obj->unk_10[0];
        } else {
            func_0200fe08(obj, obj->xOffset, obj->yOffset, obj->unk_2C, obj->unk_4C, obj->unk_10[1]);
            obj->yOffset += obj->unk_10[1];
        }
    }
    return TRUE;
}

static inline void SetOffsets(TextObject* obj, u32 x, u32 y) {
    obj->xOffset = x;
    obj->yOffset = y;
}

BOOL func_020102d8(TextObject* obj, u32 arg1, u32 arg2) {
    BOOL BVar1 = FALSE;

    switch (arg1) {
        case 0:
            break;
        case 10:
            if (obj->unk_00 == 0) {
                SetOffsets(obj, 0, obj->yOffset + (obj->unk_4E + obj->unk_10[1]));
            } else {
                SetOffsets(obj, obj->xOffset - (obj->unk_4C + obj->unk_10[0]), 0);
            }
            break;
        case 9:
            if (obj->unk_00 == 0) {
                if (arg2 == 1) {
                    func_0200fe08(obj, obj->xOffset, obj->yOffset, obj->unk_2C, 8 - (obj->xOffset & 7U), obj->unk_4E);
                }
                SetOffsets(obj, (obj->xOffset + 8) & ~7, obj->yOffset);
            } else {
                if (arg2 == 1) {
                    func_0200fe08(obj, obj->xOffset, obj->yOffset, obj->unk_2C, obj->unk_4C, 8 - (obj->yOffset & 7U));
                }
                SetOffsets(obj, obj->xOffset, (obj->yOffset + 8) & ~7);
            }
            BVar1 = TRUE;
            break;
    }

    return BVar1;
}

void func_020103f4(TextObject* arg0, s32 arg1, s32 unused) {
    if (arg1 & 0xFF00) {
        func_020101ac(arg0, arg1);
        return;
    }
    func_020102d8(arg0, arg1, 1);
}

void func_02010414(TextObject* obj) {
    if (obj->unk_18[0] == 0 && obj->unk_18[1] == 0) {
        return;
    }

    u16 i;
    s32 iVar3 = func_020107a4(obj, 0, 0);

    if (iVar3 != 0) {
        for (i = 0; i < obj->unk_18[1]; i++) {
            func_0203b258(obj->unk_34, iVar3, obj->unk_18[0] << 1);
            iVar3 += obj->unk_04 * 2;
        }
    } else {
        func_0203b2d0(obj->unk_30, func_02010808(obj, 0, 0), obj->unk_18[0] * obj->unk_18[1] * 0x20);
    }
}

void func_020104c0(TextObject* obj, UnkSmallText* param_2) {
    obj->unk_00 = param_2->unk_0C;
    obj->unk_3C = param_2->unk_08;
    obj->data   = NULL;
    obj->unk_04 = 0x20;
    obj->unk_06 = 0x20;

    obj->unk_10 = param_2->unk_18;
    obj->unk_14 = param_2->unk_10;
    obj->unk_18 = param_2->unk_14;

    obj->unk_1C = param_2->unk_10[0] << 3;
    obj->unk_1E = param_2->unk_10[1] << 3;
    obj->unk_20 = param_2->unk_14[0] << 3;
    obj->unk_22 = param_2->unk_14[1] << 3;

    obj->unk_24 = param_2->unk_1C;
    obj->unk_28 = param_2->unk_20;
    obj->unk_34 = param_2->unk_2C;
    obj->unk_36 = param_2->unk_2E;

    func_020105d4(obj, param_2->unk_24, param_2->unk_28);
    obj->unk_38 = param_2->unk_24;
    obj->unk_2C = func_0200fa10(0x8140, obj->unk_50, obj->unk_54, obj->unk_46);
    func_02010adc(obj, &param_2->unk_30);
    func_02010870(obj);
}

func_020105b8() {
    // Not yet implemented
}

func_020105d4(TextObject* obj, u32 arg1, u32 arg2) {
    // Not yet implemented
}

s32 func_020107a4(TextObject* obj, s32 param_2, s32 param_3) {
    if (param_2 >= 0 && param_2 < obj->unk_20 && param_3 >= 0 && param_3 < obj->unk_22) {
        return 0;
    }
    return obj->unk_28 + ((int)(param_3 + obj->unk_1E) >> 3) * obj->unk_04 * 2 + ((int)(param_2 + obj->unk_1C) >> 3) * 2;
}

s32 func_02010808(TextObject* arg0, s32 arg1, s32 arg2) {
    s32 var_lr = 0;
    s32 var_ip = -1;

    if ((arg1 >= 0) && (arg1 < arg0->unk_20) && (arg2 >= 0) && (arg2 < arg0->unk_22)) {
        var_ip = ((arg2 << 0x1D) >> 0x1B) + ((((arg2 >> 3) * arg0->unk_18[0]) + (arg1 >> 3)) << 5);
    }

    if (var_ip >= 0) {
        var_lr = arg0->unk_24 + (var_ip & ~0x1F);
    }

    return var_lr;
}

void func_02010870(TextObject* obj) {
    obj->unk_68 = &obj->unk_110;
    obj->unk_64 = 0;
    obj->unk_66 = 0;
}

BOOL func_02010888(TextObject* obj) {
    return obj->unk_64 == obj->unk_66;
}

void func_020108a0(TextObject* obj, u16 param_2) {
    obj->unk_68[obj->unk_64] = param_2;
    obj->unk_64++;
    obj->unk_64 &= 0xff;
}

s32 func_020108cc(TextObject* arg0) {
    u16 var_r2;
    u32 temp_r0;
    s32 temp_r5;

loop:
    s32 val = func_02010170(arg0);
    if (val == 0x40) {
        temp_r5 = func_02010170(arg0);
        temp_r0 = func_02010170(arg0);

        if ((temp_r0 >= 0x30) && (temp_r0 <= 0x39)) {
            var_r2 = temp_r0 - 0x30;
        } else if ((temp_r0 >= 0x61) && (temp_r0 <= 0x66)) {
            var_r2 = temp_r0 - 0x57;
        } else if ((temp_r0 >= 0x41) && (temp_r0 <= 0x46)) {
            var_r2 = temp_r0 - 0x37;
        } else {
            var_r2 = 0x10;
        }

        if (temp_r5 <= 0x46) {
            if (temp_r5 != 0x46) {
                func_020105d4(arg0, arg0->unk_38, var_r2);
            } else {
            }
        } else {
            switch (temp_r5 - 0x61) {
                case 1:
                    arg0->unk_90 = var_r2;
                    break;
                case 5:
                    arg0->unk_94 = var_r2;
                    break;
                case 0:
                    arg0->unk_98 = var_r2;
                    break;
                case 4:
                    arg0->unk_9C = var_r2;
                    break;
            }
        }
        goto loop;
    }
    return val;
}

s32 func_020109d0(TextObject* arg0) {
    u16 temp_r4;
    u16 temp_r5;

    temp_r4 = arg0->unk_64;
    temp_r5 = arg0->unk_66;
loop_1:
    s32 val = func_02010170(arg0);
    if (val == 0x40) {
        func_02010170(arg0);
        func_02010170(arg0);
        goto loop_1;
    }
    arg0->unk_64 = temp_r4;
    arg0->unk_66 = temp_r5;
    return val;
}

void* func_02010a10(TextObject* arg0, u8* arg1) {
    u8  var_r2;
    u8* var_r4;

    var_r4 = arg1;
    var_r2 = *var_r4;
    if (var_r2 != 0) {
        do {
            if (var_r2 & 0x80) {
                func_020108a0(arg0, (u16)(var_r4[1] | (var_r2 << 8)));
                var_r4 += 2;
            } else {
                func_020108a0(arg0, func_0200f94c(var_r2));
                var_r4 += 1;
            }
            var_r2 = *var_r4;
        } while (var_r2 != 0);
    }
    return var_r4 + 1;
}

void func_02010a7c(TextObject* arg0) {
    while (func_02010888(arg0) == FALSE) {
        s32 charCode = func_020108cc(arg0);
        s32 result   = func_020109d0(arg0);
        s32 val      = 0;

        if (result == 0 || result == 0xA) {
            val = 1;
        }

        func_020103f4(arg0, charCode, val);
    }
}

func_02010adc() {
    // Not yet implemented
}

void Text_RenderToScreen(TextObject* obj, s32 xOffset, s32 yOffset, char* string) {
    func_02010a7c(obj);
    obj->xOffset = xOffset;
    obj->yOffset = yOffset;
    func_02010a10(obj, string);
    func_02010a7c(obj);
}

void func_02010b50(TextObject* obj) {
    s32 sVar1;

    func_02010414(obj);
    if (obj->unk_00 == 0) {
        sVar1 = 0;
    } else {
        sVar1 = obj->unk_20 - obj->unk_4C;
    }
    obj->xOffset = sVar1;
    obj->yOffset = 0;
}

func_02010b84() {
    // Not yet implemented
}

func_02010fd8() {
    // Not yet implemented
}

u16 func_020111f0(TextObject* obj) {
    if (obj->unk_6C != obj->unk_6E) {
        u16 val = obj->unk_70[obj->unk_6E];
        obj->unk_6E++;
        obj->unk_6E &= 0xff;
        return val;
    }
    return 0;
}

func_0201122c(s32, s32) {
    // Not yet implemented
}

func_02011358(s32, s32, s32) {
    // Not yet implemented
}

void func_02011474(s32 arg0, u32 arg1) {
    if ((arg1 & 0xff00) != 0) {
        func_0201122c(arg0, arg1);
    } else {
        func_02011358(arg0, arg1, 1);
    }
}

func_02011494() {
    // Not yet implemented
}

func_020114d4() {
    // Not yet implemented
}

func_02011810() {
    // Not yet implemented
}

func_0201187c() {
    // Not yet implemented
}

func_02011994() {
    // Not yet implemented
}

func_020119b0() {
    // Not yet implemented
}

func_02011b7c() {
    // Not yet implemented
}

func_02011bec() {
    // Not yet implemented
}

func_02011c04() {
    // Not yet implemented
}

func_02011c1c() {
    // Not yet implemented
}

func_02011c48() {
    // Not yet implemented
}

func_02011d48() {
    // Not yet implemented
}

func_02011d4c() {
    // Not yet implemented
}

func_02011d8c() {
    // Not yet implemented
}

func_02011df8() {
    // Not yet implemented
}

func_02011e58() {
    // Not yet implemented
}

func_02011e94() {
    // Not yet implemented
}

func_02011ecc() {
    // Not yet implemented
}
