#ifndef CRISS_H
#define CRISS_H

#include "game.h"
#include <acssnd.h>
#include <adxt.h>
#include <types.h>

typedef struct CRISS {
    /* 0x00 */ u8   unk_00;
    /* 0x01 */ char pad[3];
    /* 0x04 */ s8   unk_04;
} CRISS;

void criSsPly_SetVolume(CRISS* criss, s32 volume);

void criSsPly_SetFadeTime(CRISS* criss, u32 arg1, u32 arg2);

CRISS* func_02021960(GameState* (**arg0)(s32), s32 arg1, s32 arg2);

void func_02021f4c(s32*);

void func_02021f94(const char* funcName, const char* status);

#endif // CRISS_H