#ifndef INTERFACE_MENU_MENUBADGE_H
#define INTERFACE_MENU_MENUBADGE_H

#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    s16 x;
    s16 y;
} MenuBadgePoint;

typedef struct {
    /* 0x00 */ u16  unk_00;
    /* 0x02 */ char unk_02[0x08 - 0x02];
    /* 0x08 */ u16  unk_08;
    /* 0x0A */ u16  unk_0A;
    /* 0x0C */ u16  unk_0C;
    /* 0x0E */ char unk_0E[0x14 - 0x0E];
    /* 0x14 */ u8   unk_14;
    /* 0x15 */ u8   unk_15;
    /* 0x16 */ u8   unk_16;
    /* 0x17 */ u8   unk_17;
    /* 0x18 */ char unk_18[0x1A - 0x18];
    /* 0x1A */ u16  unk_1A;
    /* 0x1C */ char unk_1C[0x41 - 0x1C];
    /* 0x41 */ u8   unk_41;
} UnkStruct_MenuBadgeItem;

typedef struct {
    /* 0x0000 */ char                     unk_0000[0xAD60];
    /* 0xAD60 */ UnkStruct_MenuBadgeItem* unk_AD60[0x1E];
    /* 0xADD8 */ u16                      unk_ADD8;
    /* 0xADDA */ u16                      unk_ADDA;
    /* 0xADDC */ u16                      unk_ADDC;
    /* 0xADDE */ u16                      unk_ADDE;
    /* 0xADE0 */ char                     unk_ADE0[0xADE8 - 0xADE0];
    /* 0xADE8 */ u32                      unk_ADE8;
    /* 0xADEC */ u8                       unk_ADEC;
    /* 0xADED */ char                     unk_ADED[0xADF2 - 0xADED];
    /* 0xADF2 */ u16                      unk_ADF2;
    /* 0xADF4 */ char                     unk_ADF4[0xADF7 - 0xADF4];
    /* 0xADF7 */ u8                       unk_ADF7;
    /* 0xADF8 */ char                     unk_ADF8[0xAE19 - 0xADF8];
    /* 0xAE19 */ u8                       unk_AE19;
    /* 0xAE1A */ char                     unk_AE1A[0xAE70 - 0xAE1A];
    /* 0xAE70 */ s32                      unk_AE70[0x1E];
    /* 0xAEE8 */ u16                      unk_AEE8;
    /* 0xAEEA */ u8                       unk_AEEA;
    /* 0xAEEB */ u8                       unk_AEEB;
    /* 0xAEEC */ u16                      unk_AEEC;
    /* 0xAEEE */ char                     unk_AEEE[0xAEF2 - 0xAEEE];
    /* 0xAEF2 */ s16                      unk_AEF2;
    /* 0xAEF4 */ s16                      unk_AEF4;
    /* 0xAEF6 */ u16                      unk_AEF6;
    /* 0xAEF8 */ u16                      unk_AEF8;
    /* 0xAEFA */ u16                      unk_AEFA;
    /* 0xAEFC */ char                     unk_AEFC[0xAEFE - 0xAEFC];
    /* 0xAEFE */ u8                       unk_AEFE;
    /* 0xAEFF */ u8                       unk_AEFF[3];
    /* 0xAF02 */ u8                       unk_AF02;
    /* 0xAF03 */ u8                       unk_AF03;
    /* 0xAF04 */ char                     unk_AF04[0xAF05 - 0xAF04];
    /* 0xAF05 */ u8                       unk_AF05;
    /* 0xAF06 */ char                     unk_AF06[0xAF0C - 0xAF06];
    /* 0xAF0C */ u32                      unk_AF0C;
    /* 0xAF10 */ u16                      unk_AF10;
    /* 0xAF12 */ u8                       unk_AF12[2];
    /* 0xAF14 */ char                     unk_AF14[0xAF18 - 0xAF14];
    /* 0xAF18 */ u8                       unk_AF18[2];
    /* 0xAF1A */ char                     unk_AF1A[0xAF1C - 0xAF1A];
    /* 0xAF1C */ u8                       unk_AF1C[3];
    /* 0xAF1F */ u8                       unk_AF1F;
    /* 0xAF20 */ u16                      unk_AF20;
    /* 0xAF22 */ char                     unk_AF22[0xAF26 - 0xAF22];
    /* 0xAF26 */ u8                       unk_AF26;
    /* 0xAF27 */ u8                       unk_AF27[4];
    /* 0xAF2B */ u8                       unk_AF2B[4];
    /* 0xAF2F */ char                     unk_AF2F[0xAF30 - 0xAF2F];
    /* 0xAF30 */ u8                       unk_AF30;
    /* 0xAF31 */ char                     unk_AF31[0xAF32 - 0xAF31];
    /* 0xAF32 */ s16                      unk_AF32;
    /* 0xAF34 */ char                     unk_AF34[0xB1E8 - 0xAF34];
    /* 0xB1E8 */ s32                      unk_B1E8;
} MenuBadgeObject;

extern MenuBadgePoint data_ov043_020c834c[];

extern BinIdentifier data_ov043_020c82e4;
extern BinIdentifier data_ov043_020c82fc;
extern BinIdentifier data_ov043_020c8314;
extern BinIdentifier data_ov043_020c831c;
extern BinIdentifier data_ov043_020c8334;

void func_ov043_0208fa60(Sprite* sprite, s16 frame, s32 animIndex, s32 frameDataIndex);

u16 func_ov043_0208fb8c(void);

s32 func_ov043_0208fac8(MenuBadgeObject* owner, u16 index);

s32 func_ov043_0208fdf0(MenuBadgeObject* owner, u8 arg1);

s32 func_ov043_020918c8(s16 x, s16 y);

s32 func_ov043_02091760(s16 x, s16 y);

s32 func_ov043_020917c0(s16 x, s16 y, s16 arg2, s16 arg3);

s32 func_ov043_02091820(s16 x, s16 y);

u32 func_ov043_02091928(s16 x, s16 y);

void func_ov043_020921cc(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

#endif // INTERFACE_MENU_MENUBADGE_H
