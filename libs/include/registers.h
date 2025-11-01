#ifndef REGISTERS_H
#define REGISTERS_H

#include <types.h>

#define REG_DISPSTAT          (*(vu16*)0x04000004)
#define REG_BG0CNT            (*(vu16*)0x04000008)
#define REG_BG1CNT            (*(vu16*)0x0400000A)
#define REG_BG2CNT            (*(vu16*)0x0400000C)
#define REG_BG3CNT            (*(vu16*)0x0400000E)
#define REG_DISP3DCNT         (*(vu16*)0x04000060)
#define REG_KEYINPUT          (*(vu16*)0x04000130)
#define REG_EXMEM_CNT         (*(vu16*)0x04000204)
#define REG_IME               (*(vu16*)0x04000208)
#define REG_IE                (*(vu32*)0x04000210)
#define REG_IF                (*(vu32*)0x04000214)
#define REG_POWER_CNT         (*(vu16*)0x04000304)
#define GFX_FIFO_MATRIX_STORE (*(vu32*)0x0400044C)
#define GFX_FIFO_SWAP_BUFFERS (*(vu32*)0x04000540)
#define GFX_FIFO_VIEWPORT     (*(vu32*)0x04000580)
#define REG_BG0CNT_SUB        (*(vu16*)0x04001008)
#define REG_BG1CNT_SUB        (*(vu16*)0x0400100A)
#define REG_BG2CNT_SUB        (*(vu16*)0x0400100C)
#define REG_BG3CNT_SUB        (*(vu16*)0x0400100E)

#endif // REGISTERS_H
