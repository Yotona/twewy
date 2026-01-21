#ifndef OPENEND_H
#define OPENEND_H

#include "DatMgr.h"
#include "EasyTask.h"
#include "Input.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "SpriteMgr.h"
#include "System.h"
#include "registers.h"

typedef struct {
    /* 0x00000 */ Data*    dataList[3];
    /* 0x0000C */ s32      dataType;
    /* 0x00010 */ s32      unk_10;
    /* 0x11588 */ char     pad_14[0x1157C];
    /* 0x11590 */ TaskPool unk_11590;
    /* 0x11610 */ MemPool  unk_11610;
    /* 0x1161C */ void*    unk_1161C;
    /* 0x11620 */ char     pad_11620[0x3FC];
    /* 0x11A1C */ s16      unk_11A1C;
    /* 0x11A1E */ s16      pad_11A1E;
    /* 0x11A20 */ s16      unk_11A20;
    /* 0x11A22 */ s16      pad_11A22;
    /* 0x11A24 */ s32      unk_11A24;
    /* 0x11A28 */ s32      unk_11A28;
    /* 0x11A2C */ s32      unk_11A2C;
    /* 0x11A30 */ s32      fadeBrightness;
    /* 0x11A34 */ s32      fadeRate;
    /* 0x11A38 */ u16      unk_11A38;
    /* 0x11A3A */ s16      pad_11A3A;
    /* 0x11A3C */ s32      unk_11A3C;
    /* 0x11A40 */ s32      unk_11A40;
    /* 0x11A44 */ s32      unk_11A44;
    /* 0x11A48 */ s32      unk_11A48;
} OpenEndState; // Size: 0x11A4C

void func_ov037_020829f4(s32 r0, s32 r1, void* buffer, s32 r3, s32 dataSize);
void func_ov037_02082c58(OpenEndState* r0);
void func_ov037_02082b30(OpenEndState* r0, s32 r1, s32 r2, s32 r3);
void func_ov037_02082adc(s32 r0, void* buffer, s32 r2, s32 dataSize);
void func_ov037_0208290c(s32 r0, s32 r1, void* buffer, s32 r3, s32 dataSize);
void func_ov037_020833a8(OpenEndState* r0);
s32  OpenEnd_CreateBadgeTask(u32 r0);

void func_ov037_0208280c(void);

int func_ov037_02083814(struct TaskPool* unused_r0, struct Task* r1, void* r2);
int func_ov037_020838a4(struct TaskPool* unused_r0, struct Task* r1, void* r2);
int func_ov037_020839ac(struct TaskPool* unused_r0, struct Task* r1, void* r2);
int func_ov037_020839cc(struct TaskPool* unused_r0, struct Task* r1, void* r2);
typedef int (*OpenEndExportFunc)(struct TaskPool*, struct Task*, void*);
typedef struct {
    OpenEndExportFunc funcs[4];
} OpenEndExportFuncTable;
static const OpenEndExportFuncTable data_ov037_02083b4c = {func_ov037_02083814, func_ov037_020838a4, func_ov037_020839ac,
                                                           func_ov037_020839cc};

const BinIdentifier OpenEnd_FileList[] = {
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

void func_ov037_0208345c(OpenEndState* r0);
void func_ov037_02083604(OpenEndState* r0);
void func_ov037_020836b4(OpenEndState* r0);
typedef void (*OpenEndFunc)(OpenEndState*);
static const OpenEndFunc data_ov037_02083a84[3] = {func_ov037_0208345c, func_ov037_02083604, func_ov037_020836b4};
void                     func_ov037_02082cd4(OpenEndState* r0);
void                     func_ov037_02082d7c(OpenEndState* r0);
void                     func_ov037_02082f04(OpenEndState* r0);
void                     func_ov037_02082f60(OpenEndState* r0);
void                     func_ov037_020830a8(OpenEndState* r0);
void                     func_ov037_020832dc(OpenEndState* r0);
void                     func_ov037_02082c00(OpenEndState* r0);
void                     func_ov037_02082c2c(OpenEndState* r0);
OpenEndFunc              data_ov037_02083a90[2][3] = {
    {func_ov037_02082cd4, func_ov037_02082d7c, func_ov037_02082f04},
    {func_ov037_02082f60, func_ov037_020830a8, func_ov037_020832dc}
};
s32 func_ov037_020839ec(struct TaskPool* pool, struct Task* task, void* taskParam, s32 index);

typedef struct {
    u32   unk_00;
    u32   unk_04;
    u32   unk_08;
    u32   unk_0c;
    u32   unk_10;
    void* file_List;
    u32   unk_18;
    u16   unk_1c;
    u16   unk_1e;
    u32   unk_20;
    u16   unk_24;
    u16   unk_26;
    u16   unk_28;
    u16   unk_2a;
} UnkStruct_ov037_02083b84; // Size = 0x2C

extern SpriteAnimation data_ov037_02083b84;

TaskPool*     data_ov037_02083e08;
s32           data_ov037_02083e04;
OpenEndState* data_ov037_02083e00;
#endif // OPENEND_H