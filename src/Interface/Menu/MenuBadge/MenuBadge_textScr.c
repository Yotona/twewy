// #include "Interface/Menu/MenuBadge.h"

// ? OS_WaitForever();                                 /* extern */
// ? func_ov031_0210a5fc(?*, u16*, u32, u32, u32, u32, u32, u32, s32); /* extern */
// ? func_ov031_0210aa94(s32);                         /* extern */
// ? func_ov031_0210aabc(s32);                         /* extern */
// ? func_ov031_0210ab28(s32, s16, s16);               /* extern */
// ? func_ov031_0210ab34(s32, ?);                      /* extern */
// ? func_ov031_0210ab3c(void**, ?, ?);                /* extern */
// ? func_ov031_0210ab48(void*, ?, s32);               /* extern */
// ? func_ov031_0210ab54(s32, ?, ?);                   /* extern */
// ? func_ov031_0210b630(void*, u16);                  /* extern */
// u16* func_ov031_0210b698(void*, s32, s32);          /* extern */
// ? func_ov031_0210bde8(void**, ?*, s32, s32, s32);   /* extern */
// ? func_ov031_0210be18(void*, s32, s32, ?);          /* extern */
// s32 func_ov043_0208fac8(void*, u16);                /* extern */
// u32 func_ov043_0208fdd0(s32, s16, u8);              /* extern */
// ? func_ov043_02092184(void*, ?, ?, ?, s32, s32);    /* extern */
// s32 func_ov043_02097348(void* arg1, void* arg2);    /* static */
// s32 func_ov043_02097370(void* arg1);                /* static */
// s32 func_ov043_020973d8();                          /* static */
// s32 func_ov043_020973e0(void* arg1);                /* static */
// void func_ov043_02097410(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3); /* static */
// extern const MenuBadgePoint data_ov043_020c8c48[2] = {
//     {-0x44, -0x30},
//     {   -1,     0},
// };
// static s8 data_ov043_020cbd94[0x18] = {
//     0x54,
//     0x73,
//     0x6B,
//     0x5F,
//     0x4D,
//     0x65,
//     0x6E,
//     0x75,
//     0x42,
//     0x61,
//     0x64,
//     0x67,
//     0x65,
//     0x5F,
//     0x74,
//     0x65,
//     0x78,
//     0x74,
//     0x53,
//     0x63,
//     0x72,
//     0,
//     0,
//     0,
// };
// static TaskHandle data_ov043_020c8c50 = {
//     data_ov043_020cbd94,
//     ((s32 (*)(TaskPool*, Task*, void*, s32)) func_ov043_02097410),
//     0x270,
// }; /* const */
// static ? data_ov043_020c8c5c;                       /* unable to generate initializer: unknown type; const */
// static ? data_ov043_020c8c6c;                       /* unable to generate initializer: unknown type; const */
// static ? data_ov043_020c8c80;                       /* unable to generate initializer: unknown type; const */
// static ? data_ov043_020c8c94;                       /* unable to generate initializer: unknown type; const */
// static ? data_ov043_020c8ca8;                       /* unable to generate initializer: unknown type; const */
// static ? data_ov043_020c8cbc;                       /* unable to generate initializer: unknown type; const */
// static ? data_ov043_020c8cd0;                       /* unable to generate initializer: unknown type; const */

// void func_ov043_02096818(void** arg0) {
//     s32 var_r7;
//     s32 var_r8;

// var_r7 = 0;
// var_r8 = arg0 + 4;
// do {
//     func_ov031_0210aa94(var_r8);
//     func_ov031_0210ab34(var_r8, 0xE);
//     func_ov031_0210ab54(var_r8, 1, 0);
//     var_r7 += 1;
//     var_r8 += 0x7C;
// } while (var_r7 < 5);
// }

// void func_ov043_02096868(void** arg0, s32 arg1, s32 arg2, u32 arg3) {
//     ? sp1C;
//     s32 sp18;
//     s32 sp14;
//     s32 temp_r0;
//     s32 temp_r0_2;
//     s32 temp_r0_3;
//     s32 temp_r0_4;
//     s32 temp_r1;
//     s32 temp_r1_2;
//     s32 temp_r1_3;
//     s32 temp_r1_4;
//     s32 temp_r7;
//     u16* var_r4;

// sp14 = arg1;
// sp18 = arg2;
// if (arg3 < 0x3E8U) {
//     var_r4 = func_ov031_0210b698(arg0 + 0x80, 0x3413);
//     func_ov031_0210a5fc(&sp1C, var_r4, arg3);
// } else if (arg3 < 0xF4240U) {
//     temp_r1 = arg3 - (0x3E8 * ((u32) MULTU_HI(arg3, 0x10624DD3) >> 6));
//     temp_r0 = temp_r1 - (0x64 * ((u32) MULTU_HI(temp_r1, 0x51EB851F) >> 5));
//     var_r4 = func_ov031_0210b698(arg0 + 0x80, 0x3414, MULTU_HI(temp_r0, 0xCCCCCCCD) / 3435973837);
//     func_ov031_0210a5fc(&sp1C, var_r4, (u32) MULTU_HI(arg3, 0x10624DD3) >> 6, (u32) MULTU_HI(temp_r1, 0x51EB851F) >> 5,
//     (u32) MULTU_HI(temp_r0, 0xCCCCCCCD) >> 3, temp_r0 - (0xA * ((u32) MULTU_HI(temp_r0, 0xCCCCCCCD) >> 3)));
// } else {
//     temp_r0_2 = arg3 - (0xF4240 * ((u32) MULTU_HI(arg3, 0x431BDE83) >> 0x12));
//     temp_r1_2 = MULTU_HI(temp_r0_2, 0x4F8B588F);
//     temp_r7 = MULTU_HI(temp_r0_2, 0x4F8B588F);
//     temp_r1_3 = temp_r0_2 - (0x186A0 * ((u32) (temp_r1_2 + ((u32) (temp_r0_2 - temp_r1_2) >> 1)) >> 0x10));
//     temp_r0_3 = temp_r1_3 - (0x2710 * ((u32) MULTU_HI(temp_r1_3, 0xD1B71759) >> 0xD));
//     temp_r1_4 = temp_r0_3 - (0x3E8 * ((u32) MULTU_HI(temp_r0_3, 0x10624DD3) >> 6));
//     temp_r0_4 = temp_r1_4 - (0x64 * ((u32) MULTU_HI(temp_r1_4, 0x51EB851F) >> 5));
//     var_r4 = func_ov031_0210b698(arg0 + 0x80, 0x3415, MULTU_HI(temp_r0_4, 0xCCCCCCCD) / 3435973837);
//     func_ov031_0210a5fc(&sp1C, var_r4, (u32) MULTU_HI(arg3, 0x431BDE83) >> 0x12, (u32) (temp_r7 + ((u32) (temp_r0_2 -
//     temp_r7) >> 1)) >> 0x10, (u32) MULTU_HI(temp_r1_3, 0xD1B71759) >> 0xD, (u32) MULTU_HI(temp_r0_3, 0x10624DD3) >> 6, (u32)
//     MULTU_HI(temp_r1_4, 0x51EB851F) >> 5, (u32) MULTU_HI(temp_r0_4, 0xCCCCCCCD) >> 3, temp_r0_4 - (0xA * ((u32)
//     MULTU_HI(temp_r0_4, 0xCCCCCCCD) >> 3)));
// }
// func_ov031_0210ab3c(arg0 + 0x80, 0, 0x100);
// func_ov031_0210bde8(arg0 + 0x80, &sp1C, sp14 + 4, sp18 + 4, 0);
// Mem_Free(&gDebugHeap, var_r4);
// }

// void func_ov043_02096acc(void** arg0, s32 arg1, s32 arg2) {
//     ? sp1E;
//     u16 sp8;
//     u16 sp6;
//     u16 sp4;
//     ?* var_r4;
//     s32 var_r2;
//     s32* var_r3;
//     u16 temp_r0;
//     u16 temp_r1;
//     u16 temp_r4;
//     u16 temp_r5;
//     u16 var_r7;
//     u32 temp_r4_2;
//     void* temp_r11;

// temp_r11 = *arg0;
// sp4 = data_ov043_020c8c48.unk0;
// sp6 = data_ov043_020c8c48.unk2;
// var_r4 = &data_ov043_020c8ca8;
// var_r3 = &spA[0];
// sp8 = data_ov043_020c8c48.unk4;
// var_r2 = 5;
// do {
//     temp_r1 = var_r4->unk0;
//     temp_r0 = var_r4->unk2;
//     var_r4 += 4;
//     var_r3->unk0 = temp_r1;
//     var_r3->unk2 = temp_r0;
//     var_r3 += 4;
//     var_r2 -= 1;
// } while (var_r2 != 0);
// var_r7 = 0;
// do {
//     func_ov031_0210ab28((var_r7 * 0x7C) + (s32) (arg0 + 4), (&spA[0])[var_r7].unk0, (&spA[0])[var_r7].unk2);
//     var_r7 += 1;
// } while ((u32) var_r7 < 5U);
// temp_r4 = temp_r11->unkADD8;
// temp_r4_2 = func_ov043_0208fdd0(temp_r11->unkADFC, temp_r11->unkAE00, temp_r11->unkADEC);
// func_ov031_0210b630(arg0 + 4, (u16) (temp_r4 + 0x27C1));
// func_ov031_0210ab3c(arg0 + 4, 0, 0x100);
// func_ov031_0210ab48(arg0 + 4, 3, 0xFFFF);
// func_ov031_0210be18(arg0 + 4, arg1 + 4, arg2 + 4, 0);
// func_ov031_0210b630(arg0 + 0xFC, 0x3417U);
// func_ov031_0210ab3c(arg0 + 0xFC, 0, 0x100);
// func_ov031_0210be18(arg0 + 0xFC, arg1 + 4, arg2 + 4, 0);
// if (func_ov043_0208fac8(temp_r11, temp_r11->unkAEEC) == 0) {
//     func_ov043_02096868(arg0, arg1, arg2, temp_r4_2);
//     return;
// }
// temp_r5 = temp_r11->unkAF10;
// func_ov043_02096868(arg0, arg1, arg2, temp_r4_2 * temp_r5);
// func_ov031_0210a5fc(&sp1E, &sp4, (u32) temp_r5);
// func_ov031_0210ab3c(arg0 + 0x178, 0, 0x1E);
// func_ov031_0210bde8(arg0 + 0x178, &sp1E, arg1 + 4, arg2 + 4, 0);
// }

// void func_ov043_02096cac(void** arg0, s32 arg1, s32 arg2) {
//     ?* var_r3;
//     ?* var_r4;
//     s32 var_r2;
//     u16 temp_r0;
//     u16 temp_r1;
//     u16 var_r7;
//     void* temp_r11;

// temp_r11 = *arg0;
// var_r4 = &data_ov043_020c8cbc;
// var_r3 = &subroutine_arg0;
// var_r2 = 5;
// do {
//     temp_r1 = var_r4->unk0;
//     temp_r0 = var_r4->unk2;
//     var_r4 += 4;
//     var_r3->unk0 = temp_r1;
//     var_r3->unk2 = temp_r0;
//     var_r3 += 4;
//     var_r2 -= 1;
// } while (var_r2 != 0);
// var_r7 = 0;
// do {
//     func_ov031_0210ab28((var_r7 * 0x7C) + (s32) (arg0 + 4), (&subroutine_arg0)[var_r7].unk0,
//     (&subroutine_arg0)[var_r7].unk2); var_r7 += 1;
// } while ((u32) var_r7 < 5U);
// func_ov031_0210b630(arg0 + 4, 0x3418U);
// func_ov031_0210ab3c(arg0 + 4, 0, 0xE0);
// func_ov031_0210ab48(arg0 + 4, 3, 0xFFFF);
// func_ov031_0210be18(arg0 + 4, arg1 + 4, arg2 + 4, 0);
// func_ov031_0210b630(arg0 + 0x80, (u16) (temp_r11->unkAF2B + 0x3419));
// func_ov031_0210ab3c(arg0 + 0x80, 2, 0x88);
// func_ov031_0210ab48(arg0 + 0x80, 3, 0xFFFF);
// func_ov031_0210be18(arg0 + 0x80, arg1 + 4, arg2 + 4, 0);
// func_ov031_0210b630(arg0 + 0xFC, (u16) (temp_r11->unkAF2C + 0x341B));
// func_ov031_0210ab3c(arg0 + 0xFC, 2, 0x88);
// func_ov031_0210ab48(arg0 + 0xFC, 3, 0xFFFF);
// func_ov031_0210be18(arg0 + 0xFC, arg1 + 4, arg2 + 4, 0);
// func_ov031_0210b630(arg0 + 0x178, 0x341DU);
// func_ov031_0210ab3c(arg0 + 0x178, 2, 0x88);
// func_ov031_0210ab48(arg0 + 0x178, 3, 0xFFFF);
// func_ov031_0210be18(arg0 + 0x178, arg1 + 4, arg2 + 4, 0);
// }

// void func_ov043_02096e6c(void** arg0, s32 arg1, s32 arg2) {
//     ?* var_r3;
//     ?* var_r4;
//     s32 var_r2;
//     u16 temp_r0;
//     u16 temp_r1;
//     u16 var_r6;

// var_r4 = &data_ov043_020c8c94;
// var_r3 = &subroutine_arg0;
// var_r2 = 5;
// do {
//     temp_r1 = var_r4->unk0;
//     temp_r0 = var_r4->unk2;
//     var_r4 += 4;
//     var_r3->unk0 = temp_r1;
//     var_r3->unk2 = temp_r0;
//     var_r3 += 4;
//     var_r2 -= 1;
// } while (var_r2 != 0);
// var_r6 = 0;
// do {
//     func_ov031_0210ab28((var_r6 * 0x7C) + (arg0 + 4), (&subroutine_arg0)[var_r6].unk0, (&subroutine_arg0)[var_r6].unk2);
//     var_r6 += 1;
// } while ((u32) var_r6 < 5U);
// func_ov031_0210b630(arg0 + 4, 0x3402U);
// func_ov031_0210ab3c(arg0 + 4, 0, 0x100);
// func_ov031_0210ab48(arg0 + 4, 0, 0x50);
// func_ov031_0210be18(arg0 + 4, arg1 + 4, arg2 + 4, 0);
// }

// void func_ov043_02096f38(void** arg0, s32 arg1, s32 arg2, s32 arg3) {
//     ?* var_r4;
//     ?* var_r5;
//     s32 var_r2;
//     u16 temp_r0;
//     u16 temp_r1;
//     u16 var_r7;

// var_r5 = &data_ov043_020c8c6c;
// var_r4 = &subroutine_arg0;
// var_r2 = 5;
// do {
//     temp_r1 = var_r5->unk0;
//     temp_r0 = var_r5->unk2;
//     var_r5 += 4;
//     var_r4->unk0 = temp_r1;
//     var_r4->unk2 = temp_r0;
//     var_r4 += 4;
//     var_r2 -= 1;
// } while (var_r2 != 0);
// var_r7 = 0;
// do {
//     func_ov031_0210ab28((var_r7 * 0x7C) + (arg0 + 4), (&subroutine_arg0)[var_r7].unk0, (&subroutine_arg0)[var_r7].unk2);
//     var_r7 += 1;
// } while ((u32) var_r7 < 5U);
// func_ov031_0210b630(arg0 + 4, (u16) (arg3 + 0x3405));
// func_ov031_0210ab3c(arg0 + 4, 0, 0x100);
// func_ov031_0210ab48(arg0 + 4, 0, 0x50);
// func_ov031_0210be18(arg0 + 4, arg1 + 4, arg2 + 4, 0);
// }

// void func_ov043_02097010(void** arg0, s32 arg1, s32 arg2) {
//     ? sp18;
//     ?* var_r4;
//     s32 var_r2;
//     s32* var_r3;
//     u16 temp_r0;
//     u16 temp_r1;
//     u16 var_r8;
//     u16* var_r0;
//     void* temp_r7;

// temp_r7 = *arg0;
// var_r4 = &data_ov043_020c8c80;
// var_r3 = &sp4[0];
// var_r2 = 5;
// do {
//     temp_r1 = var_r4->unk0;
//     temp_r0 = var_r4->unk2;
//     var_r4 += 4;
//     var_r3->unk0 = temp_r1;
//     var_r3->unk2 = temp_r0;
//     var_r3 += 4;
//     var_r2 -= 1;
// } while (var_r2 != 0);
// var_r8 = 0;
// do {
//     func_ov031_0210ab28((var_r8 * 0x7C) + (s32) (arg0 + 4), (&sp4[0])[var_r8].unk0, (&sp4[0])[var_r8].unk2);
//     var_r8 += 1;
// } while ((u32) var_r8 < 5U);
// if (temp_r7->unkAF30 == 1) {
//     var_r0 = func_ov031_0210b698(arg0 + 4, 0x3404);
// } else {
//     var_r0 = func_ov031_0210b698(arg0 + 4, 0x3403);
// }
// func_ov031_0210a5fc(&sp18, var_r0, (u32) temp_r7->unkAF30);
// func_ov031_0210ab3c(arg0 + 4, 0, 0x100);
// func_ov031_0210ab48(arg0 + 4, 0, 0x50);
// func_ov031_0210bde8(arg0 + 4, &sp18, arg1 + 4, arg2 + 4, 0);
// Mem_Free(&gDebugHeap, var_r0);
// }

// void func_ov043_02097134(void** arg0, s32 arg1, s32 arg2) {
//     ?* var_r3;
//     ?* var_r4;
//     s32 var_r2;
//     u16 temp_r0;
//     u16 temp_r1;
//     u16 var_r6;

// var_r4 = &data_ov043_020c8cd0;
// var_r3 = &subroutine_arg0;
// var_r2 = 5;
// do {
//     temp_r1 = var_r4->unk0;
//     temp_r0 = var_r4->unk2;
//     var_r4 += 4;
//     var_r3->unk0 = temp_r1;
//     var_r3->unk2 = temp_r0;
//     var_r3 += 4;
//     var_r2 -= 1;
// } while (var_r2 != 0);
// var_r6 = 0;
// do {
//     func_ov031_0210ab28((var_r6 * 0x7C) + (arg0 + 4), (&subroutine_arg0)[var_r6].unk0, (&subroutine_arg0)[var_r6].unk2);
//     var_r6 += 1;
// } while ((u32) var_r6 < 5U);
// func_ov031_0210b630(arg0 + 4, 0x340AU);
// func_ov031_0210ab3c(arg0 + 4, 0, 0x100);
// func_ov031_0210ab48(arg0 + 4, 0, 0x50);
// func_ov031_0210be18(arg0 + 4, arg1 + 4, arg2 + 4, 0);
// }

// void func_ov043_02097200(void** arg0) {
//     s32 temp_r6;
//     s32 temp_r7;
//     u16 temp_r0;
//     void* temp_r5;

// temp_r5 = *arg0;
// temp_r6 = temp_r5->unkB234;
// temp_r7 = temp_r5->unkB230;
// if ((temp_r6 != 0) && (temp_r7 != 0)) {

// } else {
//     OS_WaitForever();
// }
// temp_r0 = temp_r5->unkAF20;
// switch (temp_r0) {
//     default:
//         return;
//     case 2:
//         func_ov043_02096acc(arg0, temp_r6, temp_r7);
//         return;
//     case 3:
//         func_ov043_02096cac(arg0, temp_r6, temp_r7);
//         return;
//     case 4:
//         func_ov043_02096e6c(arg0, temp_r6, temp_r7);
//         return;
//     case 5:
//         func_ov043_02096f38(arg0, temp_r6, temp_r7, 0);
//         return;
//     case 6:
//         func_ov043_02096f38(arg0, temp_r6, temp_r7, 1);
//         return;
//     case 7:
//         func_ov043_02096f38(arg0, temp_r6, temp_r7, 2);
//         return;
//     case 8:
//         func_ov043_02096f38(arg0, temp_r6, temp_r7, 3);
//         return;
//     case 9:
//         func_ov043_02096f38(arg0, temp_r6, temp_r7, 4);
//         return;
//     case 10:
//         func_ov043_02097010(arg0, temp_r6, temp_r7);
//         return;
//     case 11:
//         func_ov043_02097134(arg0, temp_r6, temp_r7);
//         return;
//         return;
// }
// }

// s32 func_ov043_02097348(void* arg1, void* arg2) {
//     void** temp_r4;

// temp_r4 = arg1->unk18;
// *temp_r4 = arg2->unk4;
// func_ov043_02096818(temp_r4);
// func_ov043_02097200(temp_r4);
// return 1;
// }

// s32 func_ov043_02097370(void* arg1) {
//     void* temp_r5;
//     void** temp_r4;

// temp_r4 = arg1->unk18;
// temp_r5 = *temp_r4;
// if (temp_r5->unkAEE8 & 0x100) {
//     func_ov043_02092184(temp_r5 + 0xB220, 0, 1, 6, 0xF, 1);
//     func_ov043_02097200(temp_r4);
//     temp_r5->unkAEE8 = (u16) (temp_r5->unkAEE8 & ~0x100);
// }
// return 1;
// }

// s32 func_ov043_020973d8(void) {
//     return 1;
// }

// s32 func_ov043_020973e0(void* arg1) {
//     s32 var_r4;
//     s32 var_r5;

// var_r4 = 0;
// var_r5 = arg1->unk18 + 4;
// do {
//     func_ov031_0210aabc(var_r5);
//     var_r4 += 1;
//     var_r5 += 0x7C;
// } while (var_r4 < 5);
// return 1;
// }

// void func_ov043_02097410(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
//     subroutine_arg0.unk0 = data_ov043_020c8c5c.unk0;
//     subroutine_arg0.unk4 = (s32) data_ov043_020c8c5c.unk4;
//     subroutine_arg0.unk8 = (s32) data_ov043_020c8c5c.unk8;
//     subroutine_arg0.unkC = (s32) data_ov043_020c8c5c.unkC;
//     (&subroutine_arg0)[arg3](arg0, arg1, arg2);
// }

// void func_ov043_02097458(TaskPool* arg0, s32 arg1, s32 arg2) {
//     s32 spC;
//     s32 sp8;

// sp8 = arg1;
// spC = arg2;
// EasyTask_CreateTask(arg0, &data_ov043_020c8c50, NULL, 0, NULL, &sp8);
// }
