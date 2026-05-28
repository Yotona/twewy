#ifndef INTERFACE_MENU_OPENEND_H
#define INTERFACE_MENU_OPENEND_H

#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/ResourceMgr.h"

typedef struct {
    /* 0x00000 */ Data*           dataList[3];
    /* 0x0000C */ s32             dataType;
    /* 0x00010 */ ResourceManager unk_10;
    /* 0x11590 */ TaskPool        taskPool;
    /* 0x11610 */ MemPool         memPool;
    /* 0x1161C */ void*           unk_1161C;
    /* 0x11620 */ char            pad_11620[0x3FC];
    /* 0x11A1C */ s16             introFrameCount;
    /* 0x11A1E */ s16             pad_11A1E;
    /* 0x11A20 */ s16             sequenceStep;
    /* 0x11A22 */ s16             pad_11A22;
    /* 0x11A24 */ s32             titleFrameCount;
    /* 0x11A28 */ s32             selectedOption;
    /* 0x11A2C */ s32             alreadySelected;
    /* 0x11A30 */ s32             fadeBrightness;
    /* 0x11A34 */ s32             fadeRate;
    /* 0x11A38 */ u16             unk_11A38;
    /* 0x11A3A */ s16             pad_11A3A;
    /* 0x11A3C */ s32             isSaveValid;
    /* 0x11A40 */ s32             unk_11A40;
    /* 0x11A44 */ s32             isThereExistingSaveData;
    /* 0x11A48 */ s32             unk_11A48;
} OpenEndState; // Size: 0x11A4C

extern OpenEndState*       g_OpenEndstate;
extern const BinIdentifier OpenEnd_FileList[];
extern s32                 flag_screenTouched;
extern TaskPool*           g_taskPool;

#endif
