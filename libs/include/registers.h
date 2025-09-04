#ifndef REGISTERS_H
#define REGISTERS_H

#include <types.h>

#define REG_BG1CNT     (*(vu16*)0x0400000A)
#define REG_POWER_CNT  (*(vu16*)0x04000304)
#define REG_BG0CNT_SUB (*(vu16*)0x04001008)

#endif // REGISTERS_H
