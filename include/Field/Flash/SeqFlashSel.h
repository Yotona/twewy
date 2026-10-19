#ifndef FIELD_FLASH_SEQFLASHSEL_H
#define FIELD_FLASH_SEQFLASHSEL_H

#include "Engine/Resources/ResourceMgr.h"
#include "Engine/Text.h"

typedef struct {
    /* 0x00000 */ ResourceManager  resources;
    /* 0x11580 */ ResourceManager* previousResources;
    /* 0x11584 */ s32              dataSlot;
    /* 0x11588 */ char             unk_11588[0x1000C];
    /* 0x21594 */ s32              textBg;
    /* 0x21598 */ char             unk_21598[0x8];
    /* 0x215A0 */ TextObject       text;
    /* 0x216B4 */ char             unk_216B4[0x219B0 - 0x216B4];
} FlashSelectOverlayState; // Size: 0x219B0

void ProcessOverlay_FlashSel(FlashSelectOverlayState* state);

#endif // FIELD_FLASH_SEQFLASHSEL_H
