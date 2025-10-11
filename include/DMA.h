#ifndef DMA_H
#define DMA_H

#include <types.h>

typedef struct {
    union {
        /* 0x00 */ u32 unk_00;
        /* 0x00 */ u8  unk_00_08;
    };
    /* 0x04 */ void* addr;
    /* 0x08 */ s32   size;
    /* 0x0C */ void* data;
} DMAStruct;

extern DMAStruct data_02066a5c[5];

void func_02001810(void);
void func_02001848(DMAStruct* data);
void func_02001874(DMAStruct* data);
void func_020018a0(DMAStruct* data);
void func_020018d8(DMAStruct* data);
void func_02001910(DMAStruct* data);
void func_02001948(DMAStruct* data);
void func_02001980(DMAStruct* data);
void func_020019ac(void);

typedef void (*DMAFunc)();

#endif // DMA_H