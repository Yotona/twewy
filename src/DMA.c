#include "DMA.h"
#include "Memory.h"
#include "common_data.h"
#include "game.h"
#include <NitroSDK/os/cache.h>

static void DMA_ResetState(void);

DmaMgr data_02066a5c = {0};

void DMA_Init(s32 size) {
    static const char* funcName = "DMA_Init()";

    if (size == 0) {
        size = 0x100;
    }
    if (Mem_GetSequence(&gMainHeap, data_02066a5c.active.data) == funcName) {
        DMA_Flush();
        if (data_02066a5c.active.size != size) {
            Mem_Free(&gMainHeap, data_02066a5c.active.data);
            data_02066a5c.active.data = NULL;
        }
    }
    data_02066a5c.active.size = size;

    if (data_02066a5c.active.data == NULL) {
        char* sequence = funcName;
        void* buffer   = Mem_AllocHeapTail(&gMainHeap, size * sizeof(DmaRequest));
        Mem_SetSequence(&gMainHeap, buffer, sequence);
        data_02066a5c.active.data = buffer;
    }
    DMA_ResetState();
}

// Nonmatching
// ScratcH: DRYhu
static void DMA_ResetState(void) {
    data_02066a5c.active.unk_00 = 0;
    data_02066a5c.active.addr   = NULL;
    for (s32 i = 0; i < ARRAY_COUNT(data_02066a5c.queue); i++) {
        data_02066a5c.queue[i] = NULL;
    }
}

static void func_02001848(DmaRequest* request) {
    do {
        func_02037d88();
        func_02037de4(request->addr, request->unk_00, request->size);
        func_02037f24();
        request = request->data;
    } while (request != NULL);
}

static void func_02001874(DmaRequest* data) {
    do {
        func_02037f70();
        func_02037fa4(data->addr, data->unk_00, data->size);
        func_02038010();
        data = data->data;
    } while (data != NULL);
}

static void func_020018a0(DmaRequest* data) {
    do {
        func_020379c0();
        func_02037a60(data->addr, data->unk_00 & 0x7fff, data->size);
        func_02037ad0();
        data = data->data;
    } while (data != NULL);
}

static void func_020018d8(DmaRequest* data) {
    do {
        func_02037b18();
        func_02037b60(data->addr, data->unk_00 & 0x1fff, data->size);
        func_02037bcc();
        data = data->data;
    } while (data != NULL);
}

static void func_02001910(DmaRequest* data) {
    do {
        func_02037c10();
        func_02037c28(data->addr, data->unk_00 & 0x7fff, data->size);
        func_02037c8c();
        data = data->data;
    } while (data != NULL);
}

static void func_02001948(DmaRequest* data) {
    do {
        func_02037ccc();
        func_02037ce4(data->addr, data->unk_00 & 0x1fff, data->size);
        func_02037d48();
        data = data->data;
    } while (data != NULL);
}

static void func_02001980(DmaRequest* data) {
    do {
        func_0203ab7c(3, data->addr, data->unk_00, data->size);
        data = data->data;
    } while (data != NULL);
}

static DMAFunc DMA_OpHandlers[16] = {
    func_02001980, func_02001848, func_02001874, func_020018a0, func_020018d8, func_02001910, func_02001948, func_02001980,
    func_02001980, func_02001980, func_02001980, func_02001980, func_02001980, func_02001980, func_02001980, func_02001980,
};

void DMA_Flush(void) {
    DC_PurgeAll();
    for (s32 i = 0; i < 16; i++) {
        if (data_02066a5c.queue[i] != 0) {
            DMA_OpHandlers[i](data_02066a5c.queue[i]);
        }
    }
    DMA_ResetState();
}

static void DMA_Decode(s32 funcIdx, u32 param_2, DmaRequest* data) {
    DmaRequest dmaRequest;

    if (funcIdx == 7) {
        func_02004e0c(param_2, data);
        return;
    }

    dmaRequest.data = NULL;
    u32 size        = (u32)(data->unk_00 & ~0xFF) >> 8;

    if ((data->unk_00_08 & 0xf0) == 0) {
        size -= 4;
    }

    dmaRequest.size   = size;
    dmaRequest.unk_00 = param_2;

    if ((data->unk_00_08 & 0xf0) == 0) {
        dmaRequest.addr = &data->addr;
        DC_PurgeRange(dmaRequest.addr, dmaRequest.size);
        DMA_OpHandlers[funcIdx](&dmaRequest);
        return;
    }

    void* buf = Mem_AllocHeapHead(&gMainHeap, dmaRequest.size);
    Mem_SetSequence(&gMainHeap, buf, "DMA_Decode()");
    dmaRequest.addr = buf;
    func_02004d60(buf, &data->unk_00);
    DC_PurgeRange(dmaRequest.addr, dmaRequest.size);
    DMA_OpHandlers[funcIdx](&dmaRequest);
    Mem_Free(&gMainHeap, dmaRequest.addr);
}

void func_02001ae8(s32 funcIdx, u32* param_2, void* addr, u32 count) {
    DmaRequest data;

    if (count == 0) {
        DMA_Decode(funcIdx, param_2, addr);
        return;
    }
    data.unk_00 = param_2;
    data.addr   = addr;
    data.size   = count;
    data.data   = NULL;
    DC_PurgeRange(addr, count);
    DMA_OpHandlers[funcIdx](&data);
}

BOOL func_02001b44(s32 idx, s32 arg1, DmaRequest* req, s32 arg3) {
    DmaRequest* temp_r0;

    if ((Interrupts_IsVBlankInterrupted() == 0) || ((u32)data_02066a5c.active.addr >= data_02066a5c.active.size)) {
        func_02001ae8(idx, arg1, req, arg3);
        return FALSE;
    }
    if (arg3 == 0) {
        DMA_Decode(idx, arg1, req);
        return FALSE;
    }
    temp_r0         = (DmaRequest*)data_02066a5c.active.data + ((s32)data_02066a5c.active.addr);
    temp_r0->unk_00 = arg1;
    temp_r0->addr   = req;
    temp_r0->size   = arg3;

    data_02066a5c.active.unk_00 += arg3;
    data_02066a5c.active.addr += 1;

    if (data_02066a5c.queue[idx] == 0) {
        temp_r0->data            = 0;
        data_02066a5c.queue[idx] = temp_r0;
        data_02066aac[idx]       = (s32)temp_r0;
    } else {
        temp_r0->data            = 0;
        data_02066aac[idx]->data = temp_r0;
        data_02066aac[idx]       = (s32)temp_r0;
    }
    return TRUE;
}
