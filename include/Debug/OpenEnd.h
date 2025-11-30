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
void func_ov037_02082b30(s32** r0, s32 r1, s32 r2, s32 r3);
void func_ov037_02082adc(int r0, int r1, int r2, int r3);
void func_ov037_0208290c(s32 r0, s32 r1, s32 r2, s32 r3, s32 s1);
void func_ov037_020833a8(GameState* r0);
s32  func_ov037_02083a34(u32 r0);

void func_ov037_0208280c(void);

extern u32 OVERLAY_2_ID;
extern u32 OVERLAY_30_ID;
extern u32 OVERLAY_44_ID;

int func_ov037_02083814(struct TaskPool* unused_r0, struct Task* r1, s32 r2);
int func_ov037_020838a4(struct TaskPool* unused_r0, struct Task* r1, s32 r2);
int func_ov037_020839ac(struct TaskPool* unused_r0, struct Task* r1, s32 r2);
int func_ov037_020839cc(struct TaskPool* unused_r0, struct Task* r1, s32 r2);
typedef int (*OpenEndExportFunc)(struct TaskPool*, struct Task*, s32);
typedef struct {
    OpenEndExportFunc funcs[4];
} OpenEndExportFuncTable;
static const OpenEndExportFuncTable data_ov037_02083b4c = {func_ov037_02083814, func_ov037_020838a4, func_ov037_020839ac,
                                                           func_ov037_020839cc};

const BinIdentifier data_ov037_02083aa8[] = {
    {0x25,          "Apl_Mor/Grp_Title.bin"},
    {0x25,        "Apl_Mor/COPY_RG_SE.nbfc"},
    {0x25,        "Apl_Mor/COPY_RG_SE.nbfp"},
    {0x25,        "Apl_Mor/COPY_RG_SE.nbfs"},
    {0x25,        "Apl_Mor/COPY_UG_JP.nbfc"},
    {0x25,        "Apl_Mor/COPY_UG_JP.nbfp"},
    {0x25,        "Apl_Mor/COPY_UG_JP.nbfs"},
    {0x25,     "Apl_Mor/N_COPY_RG_NIN.nbfc"},
    {0x25,     "Apl_Mor/N_COPY_RG_NIN.nbfp"},
    {0x25,     "Apl_Mor/N_COPY_RG_NIN.nbfs"},
    {0x25,       "Apl_Mor/COPY_UG_CRI.nbfc"},
    {0x25,       "Apl_Mor/COPY_UG_CRI.nbfp"},
    {0x25,       "Apl_Mor/COPY_UG_CRI.nbfs"},
    {0x25,      "Apl_Mor/OpeningBG_RG.nbfc"},
    {0x25,      "Apl_Mor/OpeningBG_RG.nbfp"},
    {0x25,      "Apl_Mor/OpeningBG_RG.nbfs"},
    {0x25, "Apl_Mor/OpeningBG_UGstart.nbfc"},
    {0x25, "Apl_Mor/OpeningBG_UGstart.nbfp"},
    {0x25, "Apl_Mor/OpeningBG_UGstart.nbfs"},
};

extern s32 data_ov037_02083a74[];
extern s32 data_ov037_02083a7c[];

void func_ov037_0208345c(GameState* r0);
void func_ov037_02083604(GameState* r0);
void func_ov037_020836b4(GameState* r0);
typedef void (*OpenEndFunc)(GameState*);
static const OpenEndFunc data_ov037_02083a84[3] = {func_ov037_0208345c, func_ov037_02083604, func_ov037_020836b4};
void                     func_ov037_02082cd4(GameState* r0);
void                     func_ov037_02082d7c(GameState* r0);
void                     func_ov037_02082f04(GameState* r0);
void                     func_ov037_02082f60(GameState* r0);
void                     func_ov037_020830a8(GameState* r0);
void                     func_ov037_020832dc(GameState* r0);
const OpenEndFunc        data_ov037_02083a90[2][3] = {
    {func_ov037_02082cd4, func_ov037_02082d7c, func_ov037_02082f04},
    {func_ov037_02082f60, func_ov037_020830a8, func_ov037_020832dc}
};
s32 func_ov037_020839ec(struct TaskPool* pool, struct Task* task, s32 r2, s32 index);

typedef struct {
    u32   unk_00;
    u32   unk_04;
    u32   unk_08;
    u32   unk_0c;
    u32   unk_10;
    void* unk_14;
    u32   unk_18;
    u16   unk_1c;
    u16   unk_1e;
    u32   unk_20;
    u16   unk_24;
    u16   unk_26;
    u16   unk_28;
    u16   unk_2a;
} UnkStruct_ov037_02083b84; // Size = 0x2C

extern UnkStruct_ov037_02083b84 data_ov037_02083b84;

TaskPool*  data_ov037_02083e08;
s32        data_ov037_02083e04;
GameState* data_ov037_02083e00;
#endif // OPENEND_H