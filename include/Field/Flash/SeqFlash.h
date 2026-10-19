#ifndef FIELD_FLASH_SEQFLASH_H
#define FIELD_FLASH_SEQFLASH_H

#include "CriSndMgr.h"
#include "Engine/Resources/ResourceMgr.h"

typedef struct {
    /* 0x0 */ const char*    path;
    /* 0x4 */ CriSndMgrAdxId bgm;
} FlashMovie; // Size: 0x8

typedef struct {
    /* 0x0 */ s32               count;
    /* 0x4 */ const FlashMovie* movies;
    /* 0x8 */ const char*       name;
} FlashSequence; // Size: 0xC

#define FLASH_SEQUENCE_COUNT 83

typedef struct {
    /* 0x00000 */ ResourceManager resources;
} SeqFlashState; // Size: 0x11580

extern const FlashSequence FlashSequences[FLASH_SEQUENCE_COUNT];
void                       ProcessOverlay_Flash(SeqFlashState* state);

#endif // FIELD_FLASH_SEQFLASH_H
