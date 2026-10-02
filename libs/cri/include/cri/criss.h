#ifndef CRISS_H
#define CRISS_H

#include <cri/acssnd.h>
#include <cri/adxt.h>
#include <cri/cri_xpt.h>

typedef struct CRISS {
    /* 0x00 */ unsigned char unk_00;
    /* 0x01 */ char          pad[3];
    /* 0x04 */ char          unk_04;
} CRISS;

void criSsPly_SetVolume(CRISS* criss, int volume);

void criSsPly_SetFadeTime(CRISS* criss, unsigned int arg1, unsigned int arg2);

CRISS* func_02021960(void* (**arg0)(int), int arg1, int arg2);

void func_02021f4c(int*);

void func_02021f94(const char* funcName, const char* status);

#endif // CRISS_H