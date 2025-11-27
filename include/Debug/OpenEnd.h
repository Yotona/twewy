#ifndef OPENEND_H
#define OPENEND_H

#include "Input.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayManager.h"
#include "System.h"
#include "common_data.h"
#include "game.h"
#include "registers.h"

void func_ov037_020829f4(s32 r0, s32 r1, s32 r2, s32 r3, s32 s1);
void func_ov037_02082c58(GameState* r0);
void func_ov037_02082b30(s32* r0, s32 r1, s32 r2, s32 r3);
void func_ov037_02082adc(int r0, int r1, int r2, int r3);
void func_ov037_0208290c(s32 r0, s32 r1, s32 r2, s32 r3, s32 s1);
void func_ov037_020833a8(GameState* r0);
void func_ov037_02083a34(u32 r0);

void func_ov037_0208280c(void);

extern u32 OVERLAY_2_ID;
extern u32 OVERLAY_30_ID;
extern u32 OVERLAY_44_ID;

extern u32 data_ov037_02083a74[];
extern u32 data_ov037_02083a7c[];
extern u32 data_ov037_02083a98[];
extern u32 data_ov037_02083a94[];
extern u32 data_ov037_02083a90[];
extern u32 data_ov037_02083aa8;
extern u32 data_ov037_02083b40;
extern u32 data_ov037_02083e08;
#endif // OPENEND_H