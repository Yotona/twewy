#include "DMA.h"
#include "cache.h"
#include "common_data.h"
#include "game.h"

const char* data_020636e4 = "DMA_Init()";

DMAFunc data_020636e8[16] = {
    func_02001980, func_02001848, func_02001874, func_020018a0, func_020018d8, func_02001910, func_02001948, func_02001980,
    func_02001980, func_02001980, func_02001980, func_02001980, func_02001980, func_02001980, func_02001980, func_02001980,
};

void func_02001760(s32 size) {
    if (size == 0) {
        size = 0x100;
    }
    if ((char*)func_020049c0(&data_0206a9a4, data_02066a5c->unk_0C) == data_020636e4) {
        func_020019ac();
        if (data_02066a5c->size != size) {
            func_020048b4(&data_0206a9a4, data_02066a5c->unk_0C);
            data_02066a5c->unk_0C = 0;
        }
    }
    data_02066a5c->size = size;
    if (data_02066a5c->unk_0C == 0) {
        char* funcName = data_020636e4;
        u32   buffer   = func_02004618(&data_0206a9a4, size * 0x10);
        func_020049a8(&data_0206a9a4, buffer, funcName);
        data_02066a5c->unk_0C = buffer;
    }
    func_02001810();
}

// Nonmatching
// ScratcH: DRYhu
void func_02001810(void) {
    data_02066a5c->unk_00 = 0;
    data_02066a5c->addr   = NULL;
    for (int i = 0; i < 0x10; i++) {
        (&data_02066a5c[1].unk_00)[i] = 0;
    }
}

void func_02001848(DMAStruct* data) {
    do {
        func_02037d88();
        func_02037de4(data->addr, data->unk_00, data->size);
        func_02037f24();
        data = (DMAStruct*)data->unk_0C;
    } while (data != NULL);
}

void func_02001874(DMAStruct* data) {
    do {
        func_02037f70();
        func_02037fa4(data->addr, data->unk_00, data->size);
        func_02038010();
        data = (DMAStruct*)data->unk_0C;
    } while (data != NULL);
}

void func_020018a0(DMAStruct* data) {
    do {
        func_020379c0();
        func_02037a60(data->addr, data->unk_00 & 0x7fff, data->size);
        func_02037ad0();
        data = (DMAStruct*)data->unk_0C;
    } while (data != NULL);
}

void func_020018d8(DMAStruct* data) {
    do {
        func_02037b18();
        func_02037b60(data->addr, data->unk_00 & 0x1fff, data->size);
        func_02037bcc();
        data = (DMAStruct*)data->unk_0C;
    } while (data != NULL);
}

void func_02001910(DMAStruct* data) {
    do {
        func_02037c10();
        func_02037c28(data->addr, data->unk_00 & 0x7fff, data->size);
        func_02037c8c();
        data = (DMAStruct*)data->unk_0C;
    } while (data != NULL);
}

void func_02001948(DMAStruct* data) {
    do {
        func_02037ccc();
        func_02037ce4(data->addr, data->unk_00 & 0x1fff, data->size);
        func_02037d48();
        data = (DMAStruct*)data->unk_0C;
    } while (data != NULL);
}

void func_02001980(DMAStruct* data) {
    do {
        func_0203ab7c(3, data->addr, data->unk_00, data->size);
        data = (DMAStruct*)data->unk_0C;
    } while (data != NULL);
}

void func_020019ac(void) {
    int iVar1;

    DC_PurgeAll();
    iVar1 = 0;
    do {
        if ((u32*)(&data_02066a5c[1].unk_00)[iVar1] != NULL) {
            (*data_020636e8[iVar1])((u32*)(&data_02066a5c[1].unk_00)[iVar1]);
        }
        iVar1 = iVar1 + 1;
    } while (iVar1 < 0x10);
    func_02001810();
    return;
}

// Nonmatching
// Scratch: DsIKK
void func_020019f4(s32 funcIdx, u32 param_2, DMAStruct* data) {
    DMAStruct local_20;

    if (funcIdx == 7) {
        func_02004e0c(param_2, &data->unk_00);
        return;
    }

    local_20.unk_0C = 0;
    local_20.size   = (u32)(data->unk_00 & ~0xFF) >> 8;

    if ((data->unk_00 & 0xf0) == 0) {
        local_20.size -= 4;
    }

    local_20.unk_00 = param_2;

    if ((data->unk_00 & 0xf0) == 0) {
        local_20.addr = data->addr;
        DC_PurgeRange(local_20.addr, local_20.size);
        data_020636e8[funcIdx](&local_20);
        return;
    }

    GameState* state = func_020046e8(&data_0206a9a4, local_20.size);
    func_020049a8(&data_0206a9a4, state, "DMA_Decode()");
    local_20.addr = state;
    func_02004d60(state, &data->unk_00);
    DC_PurgeRange(local_20.addr, local_20.size);
    data_020636e8[funcIdx](&local_20);
    func_020048b4(&data_0206a9a4, local_20.addr);
}

void func_02001ae8(s32 funcIdx, u32* param_2, void* addr, u32 count) {
    DMAStruct data;

    if (count == 0) {
        func_020019f4(funcIdx, param_2, addr);
        return;
    }
    data.unk_00 = param_2;
    data.addr   = addr;
    data.size   = count;
    data.unk_0C = 0;
    DC_PurgeRange(addr, count);
    data_020636e8[funcIdx](&data);
}

// Nonmatching
BOOL func_02001b44(s32 param_1, u32* param_2, DMAStruct* param_3, s32 param_4) {
    /* Not yet implemented*/
}
