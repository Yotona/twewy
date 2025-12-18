#include "Memory.h"
#include "OverlayDispatcher.h"
#include <types.h>

extern void func_ov037_0208370c(void* state);
extern void func_ov046_02083630(void* state);

static const char* BootSequence = "Seq_Boot(void *)";

void func_02001254(void) {
    if (MainOvlDisp_GetRepeatCount() == 0) {
        const char* seq = BootSequence;

        void* binData = Mem_AllocHeapTail(&gDebugHeap, 0x304);

        Mem_SetSequence(&gDebugHeap, binData, seq);
        func_0203b2d0(0, binData, Mem_GetBlockSize(&gDebugHeap, binData));
        MainOvlDisp_SetCbArg(binData);
        BinMgr_Init(binData, 8);
        PacMgr_Init(binData + 0x60, 0x20);
        DatMgr_Init(binData + 0xC0, 0x100);
        MainOvlDisp_IncrementRepeatCount();
        MainOvlDisp_Push(&OVERLAY_37_ID, func_ov037_0208370c, NULL, 0);
    } else {
        MainOvlDisp_Push(&OVERLAY_46_ID, func_ov046_02083630, NULL, 0);
    }
}
