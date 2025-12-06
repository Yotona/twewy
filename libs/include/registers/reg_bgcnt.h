#ifndef REG_BGCNT_H
#define REG_BGCNT_H

#include <types.h>

/// MARK: Main Engine

#define REG_BG0CNT (*(vu16*)0x04000008)
#define REG_BG1CNT (*(vu16*)0x0400000A)
#define REG_BG2CNT (*(vu16*)0x0400000C)
#define REG_BG3CNT (*(vu16*)0x0400000E)

#define REG_BG0HOFS (*(vu32*)0x04000010)
#define REG_BG1HOFS (*(vu32*)0x04000014)
#define REG_BG2HOFS (*(vu32*)0x04000018)
#define REG_BG3HOFS (*(vu32*)0x0400001C)

#define REG_BG2PA (*(vu16*)0x04000020)
#define REG_BG2PB (*(vu16*)0x04000022)
#define REG_BG2PC (*(vu16*)0x04000024)
#define REG_BG2PD (*(vu16*)0x04000026)
#define REG_BG2X  (*(vu32*)0x04000028)
#define REG_BG2Y  (*(vu32*)0x0400002C)

#define REG_BG3PA (*(vu16*)0x04000030)
#define REG_BG3PB (*(vu16*)0x04000032)
#define REG_BG3PC (*(vu16*)0x04000034)
#define REG_BG3PD (*(vu16*)0x04000036)
#define REG_BG3X  (*(vu32*)0x04000038)
#define REG_BG3Y  (*(vu32*)0x0400003C)

/// MARK: Sub Engine

#define REG_BG0CNT_SUB (*(vu16*)0x04001008)
#define REG_BG1CNT_SUB (*(vu16*)0x0400100A)
#define REG_BG2CNT_SUB (*(vu16*)0x0400100C)
#define REG_BG3CNT_SUB (*(vu16*)0x0400100E)

#define REG_BG0HOFS_SUB (*(vu32*)0x04001010)
#define REG_BG1HOFS_SUB (*(vu32*)0x04001014)
#define REG_BG2HOFS_SUB (*(vu32*)0x04001018)
#define REG_BG3HOFS_SUB (*(vu32*)0x0400101C)

#define REG_BG2PA_SUB (*(vu16*)0x04001020)
#define REG_BG2PB_SUB (*(vu16*)0x04001022)
#define REG_BG2PC_SUB (*(vu16*)0x04001024)
#define REG_BG2PD_SUB (*(vu16*)0x04001026)
#define REG_BG2X_SUB  (*(vu32*)0x04001028)
#define REG_BG2Y_SUB  (*(vu32*)0x0400102C)

#define REG_BG3PA_SUB (*(vu16*)0x04001030)
#define REG_BG3PB_SUB (*(vu16*)0x04001032)
#define REG_BG3PC_SUB (*(vu16*)0x04001034)
#define REG_BG3PD_SUB (*(vu16*)0x04001036)
#define REG_BG3X_SUB  (*(vu32*)0x04001038)
#define REG_BG3Y_SUB  (*(vu32*)0x0400103C)

#endif // REG_BGCNT_H