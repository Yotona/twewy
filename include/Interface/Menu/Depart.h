#ifndef DEBUG_TAKAMI_DEPART_H
#define DEBUG_TAKAMI_DEPART_H

#include "Engine/EasyTask.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Interface/Menu/MenuCommon.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x0 */ u16  itemID;
    /* 0x2 */ char unk_2[0x4];
} DepartUnkSmall;

typedef struct {
    /* 0x00 */ u16            unk_00;
    /* 0x02 */ u8             unk_02;
    /* 0x03 */ u8             unk_03;
    /* 0x04 */ s16            unk_04;
    /* 0x06 */ u16            unk_06;
    /* 0x08 */ u16            unk_08;
    /* 0x0A */ u16            unk_0A;
    /* 0x0C */ u16            unk_0C;
    /* 0x0E */ char           unk_0E[0x2];
    /* 0x10 */ DepartUnkSmall unk_10[2];
    /* 0x1C */ char           unk_1C[0x14];
    /* 0x30 */ u8             unk_30;
    /* 0x31 */ u8             unk_31;
    /* 0x32 */ char           unk_32[0x2];
    /* 0x34 */ MenuBgResource resources[8];
} DepartObject;

typedef struct {
    /* 0x00000 */ MenuStateBase base;
    /* 0x21618 */ s32           taskId_Exit;
    /* 0x2161C */ s32           taskIds_Panel[5];
    /* 0x21630 */ s32           taskIds_Board[5];
    /* 0x21644 */ s32           taskId_Cur;
    /* 0x21648 */ s32           taskId_TextScr;
    /* 0x2164C */ s32           taskId_TextScrU;
    /* 0x21650 */ s32           unk_21650;
    /* 0x21654 */ s16           unk_21654;
    /* 0x21656 */ s16           unk_21656;
    /* 0x21658 */ char          unk_21658[0x4];
    /* 0x2165C */ DepartObject  unk_2165C;
} DepartState; // Size: 0x21770

extern Point data_ov043_020ccd00[6];

extern BinIdentifier data_ov043_020cad88;

void Depart_SetSpriteFrame(Sprite* sprite, s16 frame);

s32 DepartPanel_CreateTask(TaskPool* pool, s32 dataType, u16 arg2, DepartObject* depart);
s32 DepartBoard_CreateTask(TaskPool* pool, s32 dataType, u16 arg2, DepartObject* depart);

s32 DepartTextScr_CreateTask(TaskPool* pool, s32 dataType, DepartObject* depart);
s32 Depart_textScrU_CreateTask(TaskPool* pool, s32 dataType, DepartObject* depart);

s32 DepartExit_CreateTask(TaskPool* pool, s32 dataType, DepartObject* depart);
s32 Depart_cur_CreateTask(TaskPool* pool, s32 dataType, DepartObject* depart);

#endif // DEBUG_TAKAMI_DEPART_H