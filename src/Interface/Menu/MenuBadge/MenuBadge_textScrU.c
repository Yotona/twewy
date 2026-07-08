// #include "Interface/Menu/MenuBadge.h"

// ? OS_WaitForever();                                 /* extern */
// u8 func_02024434(u16);                              /* extern */
// ? func_ov031_0210a5fc(?*, u16*, u8, u16, u32, u32, u32, u32, s32); /* extern */
// ? func_ov031_0210aa94(void*);                       /* extern */
// ? func_ov031_0210aabc(s32);                         /* extern */
// ? func_ov031_0210ab28(void*, s16, s16);             /* extern */
// ? func_ov031_0210ab34(void*, ?);                    /* extern */
// ? func_ov031_0210ab3c(void*, ?, ?);                 /* extern */
// ? func_ov031_0210ab54(void*, ?, ?);                 /* extern */
// ? func_ov031_0210b630(void*, u16);                  /* extern */
// u16* func_ov031_0210b698(void*, s32, s32, s32);     /* extern */
// ? func_ov031_0210bde8(void*, ?*, s32, s32, s32);    /* extern */
// ? func_ov031_0210be18(void*, s32, s32, ?);          /* extern */
// u32 func_ov043_0208fdd0(s32, s16, u8);              /* extern */
// ? func_ov043_02092184(void*, ?, ?, ?, s32, s32);    /* extern */
// s32 func_ov043_020999cc(void* arg1, void* arg2);    /* static */
// s32 func_ov043_020999f4(void* arg1);                /* static */
// s32 func_ov043_02099a74();                          /* static */
// s32 func_ov043_02099a7c(void* arg1);                /* static */
// void func_ov043_02099aac(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3); /* static */
// extern ? data_ov043_020c8ec8;
// static s8 data_ov043_020cbe34[0x18] = {
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
//     0x55,
//     0,
//     0,
// };
// static TaskHandle data_ov043_020c8ee4[2] = {
//     {
//         data_ov043_020cbe34,
//         ((s32 (*)(TaskPool*, Task*, void*, s32)) func_ov043_02099aac),
//         0xAAC,
//     },
//     { (s8* )0xB0018, (s32 (*)(TaskPool*, Task*, void*, s32))0xA0018, 0x1B000E },
// }; /* const */
// static ? data_ov043_020c8efc;                       /* unable to generate initializer: unknown type; const */
// static ? data_ov043_020c8f0c;                       /* unable to generate initializer: unknown type; const */

// u16 func_ov043_02098738(u16 arg0, s16 arg1, u8 arg2) {
//     return (u16) ((arg1 * (arg2 - 1)) + arg0);
// }

// u16 func_ov043_0209874c(u16 arg0, s16 arg1, u8 arg2) {
//     return (u16) ((arg1 * (arg2 - 1)) + arg0);
// }

// void func_ov043_02098760(u32 arg0, u16* arg1) {
//     s32 temp_asr;
//     u32 temp_ip;
//     u32 temp_lr;
//     u32 temp_r0;

// if (arg0 >= 0x1798U) {
//     arg1->unk0 = 0x63;
//     arg1->unk2 = 9;
// }
// temp_lr = arg0 >> 0x1F;
// temp_ip = (u16) (arg0 - (0x3C * (temp_lr + ((s32) (arg0 + MULT_HI(0x88888889, arg0)) >> 5)))) * 0x64;
// temp_r0 = temp_ip >> 0x1F;
// temp_asr = (s32) (temp_ip + MULT_HI(0x88888889, temp_ip)) >> 5;
// arg1->unk0 = temp_lr + ((s32) (arg0 + MULT_HI(0x88888889, arg0)) >> 5);
// arg1->unk2 = (s16) (((u16) (temp_r0 + temp_asr) >> 0x1F) + ((temp_r0 + temp_asr) / 10));
// }

// void func_ov043_020987fc(void** arg0) {
//     s32 var_r7;
//     void* var_r8;

// var_r7 = 0;
// var_r8 = arg0 + 4;
// do {
//     func_ov031_0210aa94(var_r8);
//     func_ov031_0210ab34(var_r8, 0xE);
//     func_ov031_0210ab54(var_r8, 1, 0);
//     var_r7 += 1;
//     var_r8 += 0x7C;
// } while (var_r7 < 0x16);
// }

// void func_ov043_0209884c(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     func_ov031_0210b630(arg0 + 4, (u16) (arg1->unkADD8 + 0x27C1));
//     func_ov031_0210be18(arg0 + 4, arg3 + 4, arg2 + 4, 0);
// }

// void func_ov043_02098894(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     u8 temp_r1;

// temp_r1 = arg1->unkADF7;
// if ((u32) temp_r1 <= 0xCU) {
//     func_ov031_0210b630(arg0 + 0x80, (u16) (temp_r1 + 0x2A21));
//     func_ov031_0210ab3c(arg0 + 0x80, 1, 0xDC);
//     func_ov031_0210be18(arg0 + 0x80, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// func_ov031_0210b630(arg0 + 0x80, 0x2A2FU);
// func_ov031_0210ab3c(arg0 + 0x80, 1, 0xDC);
// func_ov031_0210be18(arg0 + 0x80, arg3 + 4, arg2 + 4, 0);
// }

// void func_ov043_0209892c(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     ? sp8;
//     u16 sp6;
//     u16 sp4;
//     u8 temp_r6;

// temp_r6 = arg1->unkAE19;
// sp4 = data_ov043_020c8ec8.unkC;
// sp6 = data_ov043_020c8ec8.unkE;
// func_ov031_0210a5fc(&sp8, &sp4, arg1->unkADEC);
// func_ov031_0210ab3c(arg0 + 0xFC, 0, 0xE);
// func_ov031_0210bde8(arg0 + 0xFC, &sp8, arg3 + 4, arg2 + 4, 0);
// func_ov031_0210b630(arg0 + 0x178, 0x333AU);
// func_ov031_0210be18(arg0 + 0x178, arg3 + 4, arg2 + 4, 0);
// func_ov031_0210a5fc(&sp8, &sp4, temp_r6);
// func_ov031_0210ab3c(arg0 + 0x1F4, 0, 0xE);
// func_ov031_0210bde8(arg0 + 0x1F4, &sp8, arg3 + 4, arg2 + 4, 0);
// }

// void func_ov043_02098a04(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     u16 temp_lr;
//     u8 temp_r0;

// temp_lr = arg1->unkADF4;
// temp_r0 = arg1->unkAE18;
// if (temp_lr != 0xFFFF) {
//     func_ov031_0210b630(arg0 + 0x270, (u16) (temp_lr + 0x28F1));
//     func_ov031_0210ab3c(arg0 + 0x270, 0, 0x79);
//     func_ov031_0210be18(arg0 + 0x270, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// if (temp_r0 == 0xFF) {
//     func_ov031_0210b630(arg0 + 0x270, 0x3339U);
//     func_ov031_0210ab3c(arg0 + 0x270, 0, 0x79);
//     func_ov031_0210be18(arg0 + 0x270, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// func_ov031_0210b630(arg0 + 0x270, (u16) (temp_r0 + 0x22C3));
// func_ov031_0210ab3c(arg0 + 0x270, 0, 0x79);
// func_ov031_0210be18(arg0 + 0x270, arg3 + 4, arg2 + 4, 0);
// }

// void func_ov043_02098af4(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     ? sp8;
//     u16 sp6;
//     u16 sp4;

// if (arg1->unkADEC != arg1->unkAE19) {
//     sp6 = data_ov043_020c8ec8.unk6;
//     sp4 = data_ov043_020c8ec8.unk4;
//     func_ov031_0210a5fc(&sp8, &sp4, (u8) (u16) (arg1->unkADE4 - arg1->unkADE2));
//     func_ov031_0210ab3c(arg0 + 0x2EC, 0, 0x47);
//     func_ov031_0210bde8(arg0 + 0x2EC, &sp8, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// func_ov031_0210b630(arg0 + 0x2EC, 0x3339U);
// func_ov031_0210ab3c(arg0 + 0x2EC, 0, 0x47);
// func_ov031_0210be18(arg0 + 0x2EC, arg3 + 4, arg2 + 4, 0);
// }

// void func_ov043_02098bc8(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     ? spE;
//     u16 spC;
//     u16 spA;
//     u16 sp8;
//     u16 sp6;
//     u16 sp4;
//     s16 temp_r6;
//     u16 temp_r5;
//     u16* temp_r0_2;
//     u16* temp_r10;
//     u8 temp_r0;
//     u8 temp_r4;

// temp_r5 = arg1->unkAE02;
// temp_r4 = arg1->unkADEC;
// temp_r6 = arg1->unkAE04;
// temp_r0 = func_02024434(arg1->unkADD8);
// if (temp_r0 != 0) {
//     temp_r0_2 = func_ov031_0210b698(arg0 + 0x368, 0x3427);
//     func_ov031_0210a5fc(&spE, temp_r0_2, temp_r0);
//     func_ov031_0210ab3c(arg0 + 0x368, 0, 0x5A);
//     func_ov031_0210bde8(arg0 + 0x368, &spE, arg3 + 4, arg2 + 4, 0);
//     Mem_Free(&gDebugHeap, temp_r0_2);
//     return;
// }
// if (temp_r5 == 0) {
//     func_ov031_0210b630(arg0 + 0x368, 0x3357U);
//     func_ov031_0210ab3c(arg0 + 0x368, 0, 0x5A);
//     func_ov031_0210be18(arg0 + 0x368, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// sp6 = data_ov043_020c8ec8.unk12;
// sp4 = data_ov043_020c8ec8.unk10;
// sp8 = data_ov043_020c8ec8.unk14;
// spA = data_ov043_020c8ec8.unk16;
// spC = data_ov043_020c8ec8.unk18;
// temp_r10 = func_ov031_0210b698(arg0 + 0x368, 0x3322);
// func_ov031_0210a5fc(&spE, &sp4, (u8) temp_r10, func_ov043_02098738(temp_r5, temp_r6, temp_r4));
// func_ov031_0210ab3c(arg0 + 0x368, 0, 0x5A);
// func_ov031_0210bde8(arg0 + 0x368, &spE, arg3 + 4, arg2 + 4, 0);
// Mem_Free(&gDebugHeap, temp_r10);
// }

// void func_ov043_02098d58(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     func_ov031_0210b630(arg0 + 0x3E4, (u16) (arg1->unkADF9 + 0x3331));
//     func_ov031_0210ab3c(arg0 + 0x3E4, 0, 0x3D);
//     func_ov031_0210be18(arg0 + 0x3E4, arg3 + 4, arg2 + 4, 0);
// }

// void func_ov043_02098db0(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     ? spA;
//     u16 sp4;
//     s16 temp_r7;
//     u16 temp_ip;
//     u16 temp_r0;
//     u16 temp_r6;
//     u16* temp_r4;
//     u16* var_r0;
//     u8 temp_r5;

// temp_ip = arg1->unkAE06;
// temp_r5 = arg1->unkADEC;
// temp_r6 = arg1->unkAE08;
// temp_r7 = arg1->unkAE0A;
// if (temp_ip == 0) {
//     func_ov031_0210b630(arg0 + 0x460, 0x3357U);
//     func_ov031_0210ab3c(arg0 + 0x460, 0, 0x5A);
//     func_ov031_0210be18(arg0 + 0x460, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// if ((u32) (u16) (temp_ip + 0xFFFF) <= 1U) {
//     temp_r0 = func_ov043_0209874c(temp_r6, temp_r7, temp_r5);
//     if (temp_r0 == 1) {
//         var_r0 = func_ov031_0210b698(arg0 + 0x460, 0x335A);
//     } else {
//         var_r0 = func_ov031_0210b698(arg0 + 0x460, 0x3359);
//     }
//     func_ov031_0210a5fc(&spA, var_r0, (u8) temp_r0);
//     func_ov031_0210ab3c(arg0 + 0x460, 0, 0x5A);
//     func_ov031_0210bde8(arg0 + 0x460, &spA, arg3 + 4, arg2 + 4, 0);
//     Mem_Free(&gDebugHeap, var_r0);
//     return;
// }
// temp_r4 = func_ov031_0210b698(arg0 + 0x460, 0x3358);
// func_ov043_02098760((u32) func_ov043_0209874c(temp_r6, temp_r7, temp_r5), &sp4);
// func_ov031_0210a5fc(&spA, temp_r4, (u8) sp4, sp6);
// func_ov031_0210ab3c(arg0 + 0x460, 0, 0x5A);
// func_ov031_0210bde8(arg0 + 0x460, &spA, arg3 + 4, arg2 + 4, 0);
// Mem_Free(&gDebugHeap, temp_r4);
// }

// void func_ov043_02098f50(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     ? sp1C;
//     s32 sp18;
//     s32 sp14;
//     s32 temp_r1;
//     s32 temp_r1_2;
//     s32 temp_r2;
//     s32 temp_r2_2;
//     s32 temp_r2_3;
//     s32 temp_r2_4;
//     s32 temp_r2_5;
//     s32 temp_r3;
//     s32 temp_r3_2;
//     s32 temp_r8;
//     u16* var_r5;
//     u32 temp_r0;

// sp14 = arg2;
// sp18 = arg3;
// temp_r0 = func_ov043_0208fdd0(arg1->unkADFC, arg1->unkAE00, arg1->unkADEC);
// if (temp_r0 < 0x3E8U) {
//     var_r5 = func_ov031_0210b698(arg0 + 0x4DC, 0x3410);
//     func_ov031_0210a5fc(&sp1C, var_r5, (u8) temp_r0);
// } else if (temp_r0 < 0xF4240U) {
//     temp_r2 = temp_r0 - (0x3E8 * ((u32) MULTU_HI(temp_r0, 0x10624DD3) >> 6));
//     temp_r1 = temp_r2 - (0x64 * ((u32) MULTU_HI(temp_r2, 0x51EB851F) >> 5));
//     temp_r2_2 = 0xA * ((u32) MULTU_HI(temp_r1, 0xCCCCCCCD) >> 3);
//     var_r5 = func_ov031_0210b698(arg0 + 0x4DC, 0x3411, temp_r2_2, MULTU_HI(temp_r1, 0xCCCCCCCD) / 3435973837);
//     func_ov031_0210a5fc(&sp1C, var_r5, (u8) ((u32) MULTU_HI(temp_r0, 0x10624DD3) >> 6), (u16) ((u32) MULTU_HI(temp_r2,
//     0x51EB851F) >> 5), (u32) MULTU_HI(temp_r1, 0xCCCCCCCD) >> 3, temp_r1 - temp_r2_2);
// } else {
//     temp_r2_3 = temp_r0 - (0xF4240 * ((u32) MULTU_HI(temp_r0, 0x431BDE83) >> 0x12));
//     temp_r3 = MULTU_HI(temp_r2_3, 0x4F8B588F);
//     temp_r8 = MULTU_HI(temp_r2_3, 0x4F8B588F);
//     temp_r3_2 = temp_r2_3 - (0x186A0 * ((u32) (temp_r3 + ((u32) (temp_r2_3 - temp_r3) >> 1)) >> 0x10));
//     temp_r2_4 = temp_r3_2 - (0x2710 * ((u32) MULTU_HI(temp_r3_2, 0xD1B71759) >> 0xD));
//     temp_r1_2 = temp_r2_4 - (0x3E8 * ((u32) MULTU_HI(temp_r2_4, 0x10624DD3) >> 6));
//     temp_r2_5 = temp_r1_2 - (0x64 * ((u32) MULTU_HI(temp_r1_2, 0x51EB851F) >> 5));
//     var_r5 = func_ov031_0210b698(arg0 + 0x4DC, 0x3412);
//     func_ov031_0210a5fc(&sp1C, var_r5, (u8) ((u32) MULTU_HI(temp_r0, 0x431BDE83) >> 0x12), (u16) ((u32) (temp_r8 + ((u32)
//     (temp_r2_3 - temp_r8) >> 1)) >> 0x10), (u32) MULTU_HI(temp_r3_2, 0xD1B71759) >> 0xD, (u32) MULTU_HI(temp_r2_4,
//     0x10624DD3) >> 6, (u32) MULTU_HI(temp_r1_2, 0x51EB851F) >> 5, (u32) MULTU_HI(temp_r2_5, 0xCCCCCCCD) >> 3, temp_r2_5 -
//     (0xA * ((u32) MULTU_HI(temp_r2_5, 0xCCCCCCCD) >> 3)));
// }
// func_ov031_0210ab3c(arg0 + 0x4DC, 0, 0x3D);
// func_ov031_0210bde8(arg0 + 0x4DC, &sp1C, sp18 + 4, sp14 + 4, 0);
// Mem_Free(&gDebugHeap, var_r5);
// }

// void func_ov043_020991d8(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     ? spA;
//     u16 sp4;
//     s16 temp_r7;
//     u16 temp_ip;
//     u16 temp_r6;
//     u16* temp_r4;
//     u8 temp_r5;

// temp_ip = arg1->unkAE0C;
// temp_r5 = arg1->unkADEC;
// temp_r6 = arg1->unkAE0E;
// temp_r7 = arg1->unkAE10;
// if (temp_ip == 0) {
//     func_ov031_0210b630(arg0 + 0x558, 0x335BU);
//     func_ov031_0210ab3c(arg0 + 0x558, 0, 0xC4);
//     func_ov031_0210be18(arg0 + 0x558, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// if (temp_ip == 1) {
//     temp_r4 = func_ov031_0210b698(arg0 + 0x558, 0x335C);
//     func_ov043_02098760((u32) func_ov043_0209874c(temp_r6, temp_r7, temp_r5), &sp4);
//     func_ov031_0210a5fc(&spA, temp_r4, (u8) sp4, sp6);
//     func_ov031_0210ab3c(arg0 + 0x558, 0, 0xC4);
//     func_ov031_0210bde8(arg0 + 0x558, &spA, arg3 + 4, arg2 + 4, 0);
//     Mem_Free(&gDebugHeap, temp_r4);
//     return;
// }
// func_ov031_0210b630(arg0 + 0x558, 0x2D56U);
// func_ov031_0210ab3c(arg0 + 0x558, 0, 0xC4);
// func_ov031_0210be18(arg0 + 0x558, arg3 + 4, arg2 + 4, 0);
// }

// void func_ov043_02099330(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     ? spA;
//     u16 sp4;
//     s16 temp_r7;
//     u16 temp_ip;
//     u16 temp_r6;
//     u16* temp_r4;
//     u8 temp_r5;

// temp_ip = arg1->unkAE12;
// temp_r5 = arg1->unkADEC;
// temp_r6 = arg1->unkAE14;
// temp_r7 = arg1->unkAE16;
// switch (temp_ip) {                              /* irregular */
//     case 0:
//         func_ov031_0210b630(arg0 + 0x5D4, 0x335DU);
//         func_ov031_0210ab3c(arg0 + 0x5D4, 0, 0xC4);
//         func_ov031_0210be18(arg0 + 0x5D4, arg3 + 4, arg2 + 4, 0);
//         return;
//     case 1:
//         temp_r4 = func_ov031_0210b698(arg0 + 0x5D4, 0x335E);
//         func_ov043_02098760((u32) func_ov043_0209874c(temp_r6, temp_r7, temp_r5), &sp4);
//         func_ov031_0210a5fc(&spA, temp_r4, (u8) sp4, sp6);
//         func_ov031_0210ab3c(arg0 + 0x5D4, 0, 0xC4);
//         func_ov031_0210bde8(arg0 + 0x5D4, &spA, arg3 + 4, arg2 + 4, 0);
//         Mem_Free(&gDebugHeap, temp_r4);
//         return;
// }
// }

// void func_ov043_02099448(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     u8 temp_r0;

// temp_r0 = arg1->unkAE1A;
// if (arg1->unkAEFE != 0) {
//     return;
// }
// if (temp_r0 != 0xFF) {
//     func_ov031_0210b630(arg0 + 0x650, (u16) (temp_r0 + 0x333F));
//     func_ov031_0210ab3c(arg0 + 0x650, 0, 0x87);
//     func_ov031_0210be18(arg0 + 0x650, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// func_ov031_0210b630(arg0 + 0x650, 0x3339U);
// func_ov031_0210ab3c(arg0 + 0x650, 0, 0x87);
// func_ov031_0210be18(arg0 + 0x650, arg3 + 4, arg2 + 4, 0);
// }

// void func_ov043_020994ec(void** arg0, void* arg1, s32 arg2, s32 arg3) {
//     ? sp8;
//     u16 sp6;
//     u16 sp4;
//     s32 temp_r4;
//     s32 temp_r5_2;
//     u16 temp_r0;
//     u16 var_r7;
//     u8 temp_r5;

// temp_r5 = arg1->unkAEFE;
// temp_r0 = arg1->unkADD8;
// if (temp_r5 == 0) {
//     func_ov031_0210b630(arg0 + 0x6CC, (u16) (temp_r0 + 0x2430));
//     func_ov031_0210be18(arg0 + 0x6CC, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// if (temp_r5 == 1) {
//     func_ov031_0210b630(arg0 + 0x6CC, (u16) (temp_r0 + 0x2560));
//     func_ov031_0210be18(arg0 + 0x6CC, arg3 + 4, arg2 + 4, 0);
//     return;
// }
// sp4 = data_ov043_020c8ec8.unk0;
// sp6 = data_ov043_020c8ec8.unk2;
// func_ov031_0210b630(arg0 + 0x6CC, 0x2690U);
// func_ov031_0210be18(arg0 + 0x6CC, arg3 + 4, arg2 + 4, 0);
// var_r7 = 0;
// temp_r5_2 = arg0 + 4;
// do {
//     func_ov031_0210a5fc(&sp8, &sp4, (arg1 + var_r7)->unkAE1B);
//     temp_r4 = (var_r7 + 0xF) * 0x7C;
//     func_ov031_0210ab3c(temp_r5_2 + temp_r4, 0, 0x14);
//     func_ov031_0210ab34(temp_r5_2 + temp_r4, 0xC);
//     func_ov031_0210bde8(temp_r5_2 + temp_r4, &sp8, arg3 + 4, arg2 + 4, 0);
//     var_r7 += 1;
// } while ((u32) var_r7 < 7U);
// }

// void func_ov043_02099664(void** arg0) {
//     ?* var_r3;
//     ?* var_r4;
//     s32 temp_r7;
//     s32 temp_r8;
//     s32 var_r2;
//     s32 var_r6;
//     u16 temp_r0;
//     u16 temp_r1;
//     void* temp_r5;
//     void* var_r9;

// temp_r5 = *arg0;
// var_r4 = &data_ov043_020c8f0c;
// var_r3 = &subroutine_arg0;
// var_r2 = 0x16;
// do {
//     temp_r1 = var_r4->unk0;
//     temp_r0 = var_r4->unk2;
//     var_r4 += 4;
//     var_r3->unk0 = temp_r1;
//     var_r3->unk2 = temp_r0;
//     var_r3 += 4;
//     var_r2 -= 1;
// } while (var_r2 != 0);
// temp_r7 = temp_r5->unkB1A8;
// temp_r8 = temp_r5->unkB1A4;
// if ((temp_r7 != 0) && (temp_r8 != 0)) {

// } else {
//     OS_WaitForever();
// }
// if (temp_r5->unkADD8 == 0xFFFF) {
//     return;
// }
// var_r9 = arg0 + 4;
// var_r6 = 0;
// do {
//     func_ov031_0210ab28(var_r9, (&subroutine_arg0)[var_r6].unk0, (&subroutine_arg0)[var_r6].unk2);
//     var_r6 += 1;
//     var_r9 += 0x7C;
// } while (var_r6 < 0x16);
// func_ov043_0209884c(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02098894(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_0209892c(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02098a04(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02098af4(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02098bc8(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02098d58(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02098db0(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02098f50(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_020991d8(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02099330(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_02099448(arg0, temp_r5, temp_r8, temp_r7);
// func_ov043_020994ec(arg0, temp_r5, temp_r8, temp_r7);
// }

// void func_ov043_0209981c(void** arg0) {
//     ? sp10;
//     u16 spE;
//     u16 spC;
//     u16 spA;
//     u16 sp8;
//     u16 sp6;
//     u16 sp4;
//     s32 temp_r4;
//     s32 temp_r8;
//     s32 temp_r9;
//     u16 var_r7;
//     u16* temp_r0;
//     void* temp_r5;
//     void* temp_r6;

// temp_r6 = *arg0;
// sp6 = data_ov043_020c8ec8.unk2A;
// sp4 = data_ov043_020c8ec8.unk28;
// sp8 = data_ov043_020c8ec8.unk2C;
// spA = data_ov043_020c8ec8.unk2E;
// spC = data_ov043_020c8ec8.unk30;
// spE = data_ov043_020c8ec8.unk32;
// temp_r8 = temp_r6->unkB1A8;
// temp_r9 = temp_r6->unkB1A4;
// if ((temp_r8 != 0) && (temp_r9 != 0)) {

// } else {
//     OS_WaitForever();
// }
// var_r7 = 0;
// temp_r5 = arg0 + 4;
// do {
//     temp_r4 = var_r7 * 0x7C;
//     func_ov031_0210ab28(temp_r5 + temp_r4, *(&sp4 + (var_r7 * 4)), (&sp4 + (var_r7 * 4))->unk2);
//     func_ov031_0210ab54(temp_r5 + temp_r4, 1, 0);
//     var_r7 += 1;
// } while ((u32) var_r7 < 3U);
// func_ov031_0210b630(arg0 + 4, (u16) (temp_r6->unkAF1F + 0x2AAD));
// func_ov031_0210ab3c(arg0 + 4, 1, 0xE0);
// func_ov031_0210be18(arg0 + 4, temp_r8 + 4, temp_r9 + 4, 0);
// temp_r0 = func_ov031_0210b698(arg0 + 0x80, 0x333D);
// func_ov031_0210a5fc(&sp10, temp_r0, temp_r6->unkAF1F + 1, 0xBU);
// func_ov031_0210ab3c(arg0 + 0x80, 2, 0xDC);
// func_ov031_0210bde8(arg0 + 0x80, &sp10, temp_r8 + 4, temp_r9 + 4, 0);
// Mem_Free(&gDebugHeap, temp_r0);
// func_ov031_0210b630(arg0 + 0xFC, (u16) (temp_r6->unkAF1F + 0x2AB8));
// func_ov031_0210ab3c(arg0 + 0xFC, 1, 0xE0);
// func_ov031_0210be18(arg0 + 0xFC, temp_r8 + 4, temp_r9 + 4, 0);
// }

// s32 func_ov043_020999cc(void* arg1, void* arg2) {
//     void** temp_r4;

// temp_r4 = arg1->unk18;
// *temp_r4 = arg2->unk4;
// func_ov043_020987fc(temp_r4);
// func_ov043_02099664(temp_r4);
// return 1;
// }

// s32 func_ov043_020999f4(void* arg1) {
//     void* temp_r5;

// temp_r5 = *arg1->unk18;
// if (temp_r5->unkAEE8 & 0x80) {
//     func_ov043_02092184(temp_r5 + 0xB194, 1, 0, 7, 0xF, 1);
//     if (temp_r5->unkAF20 == 1) {
//         func_ov043_0209981c();
//     } else {
//         func_ov043_02099664();
//     }
//     temp_r5->unkAEE8 = (u16) (temp_r5->unkAEE8 & ~0x80);
// }
// return 1;
// }

// s32 func_ov043_02099a74(void) {
//     return 1;
// }

// s32 func_ov043_02099a7c(void* arg1) {
//     s32 var_r4;
//     s32 var_r5;

// var_r4 = 0;
// var_r5 = arg1->unk18 + 4;
// do {
//     func_ov031_0210aabc(var_r5);
//     var_r4 += 1;
//     var_r5 += 0x7C;
// } while (var_r4 < 0x16);
// return 1;
// }

// void func_ov043_02099aac(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
//     subroutine_arg0.unk0 = data_ov043_020c8efc.unk0;
//     subroutine_arg0.unk4 = (s32) data_ov043_020c8efc.unk4;
//     subroutine_arg0.unk8 = (s32) data_ov043_020c8efc.unk8;
//     subroutine_arg0.unkC = (s32) data_ov043_020c8efc.unkC;
//     (&subroutine_arg0)[arg3](arg0, arg1, arg2);
// }

// void func_ov043_02099af4(TaskPool* arg0, s32 arg1, s32 arg2) {
//     s32 spC;
//     s32 sp8;

// sp8 = arg1;
// spC = arg2;
// EasyTask_CreateTask(arg0, data_ov043_020c8ee4, NULL, 0, NULL, &sp8);
// }
