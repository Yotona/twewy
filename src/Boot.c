#include "BinMgr.h"
#include "DatMgr.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "PacMgr.h"
#include <types.h>

extern void func_ov037_0208370c(void* state);
extern void func_ov046_02083630(void* state);

// TODO: Padding suggests these structs might be slightly wrong
typedef struct {
    /* 0x000 */ BinMgr binMgr;
    /* 0x04C */ char   padding_4C[0x14];
    /* 0x060 */ PacMgr pacMgr;
    /* 0x0AC */ char   padding_AC[0x14];
    /* 0x0C0 */ DatMgr datMgr;
} BootHeap; // Size: 0x304

static const char* BootSequence = "Seq_Boot(void *)";

void Boot(void* unused) {
    if (MainOvlDisp_GetRepeatCount() == 0) {
        const char* seq = BootSequence;

        BootHeap* heap = Mem_AllocHeapTail(&gDebugHeap, sizeof(BootHeap));

        Mem_SetSequence(&gDebugHeap, heap, seq);
        func_0203b2d0(0, heap, Mem_GetBlockSize(&gDebugHeap, heap));
        MainOvlDisp_SetCbArg(heap);
        BinMgr_Init(&heap->binMgr, 8);
        PacMgr_Init(&heap->pacMgr, 32);
        DatMgr_Init(&heap->datMgr, 0x100);
        MainOvlDisp_IncrementRepeatCount();
        MainOvlDisp_Push(&OVERLAY_37_ID, func_ov037_0208370c, NULL, 0);
    } else {
        MainOvlDisp_Push(&OVERLAY_46_ID, func_ov046_02083630, NULL, 0);
    }
}
