#ifndef FIELD_EVENT_EVENTMANAGER_H
#define FIELD_EVENT_EVENTMANAGER_H

#include "Engine/EasyTask.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Field/Event/Events.h"
#include "Field/FieldTypes.h"

typedef struct FieldEventManager {
    /* 0x00000 */ ResourceManager         resources;
    /* 0x11580 */ char                    unk_11580[0x11584 - 0x11580];
    /* 0x11584 */ TaskPool                taskPool;
    /* 0x11604 */ char                    unk_11604[0x21610 - 0x11604];
    /* 0x21610 */ s32                     unk_21610;
    /* 0x21614 */ s32                     dataType;
    /* 0x21618 */ s32                     unk_21618;
    /* 0x2161C */ char                    unk_2161C[0x21620 - 0x2161C];
    /* 0x21620 */ struct Data*            unk_21620;
    /* 0x21624 */ FieldPos                origin; // camera origin on the field map
    /* 0x2162C */ s32                     unk_2162C;
    /* 0x21630 */ s32                     exitRequest;
    /* 0x21634 */ char                    unk_21634[0x216B0 - 0x21634];
    /* 0x216B0 */ s32                     taskId_Badge;
    /* 0x216B4 */ s32                     taskId_ScanArea;
    /* 0x216B8 */ s32                     taskId_KeywordMenu;
    /* 0x216BC */ s32                     unk_216BC;
    /* 0x216C0 */ u32                     unk_216C0;
    /* 0x216C4 */ s32                     unk_216C4;
    /* 0x216C8 */ s32                     unk_216C8;
    /* 0x216CC */ u32                     unk_216CC;
    /* 0x216D0 */ s32                     unk_216D0;
    /* 0x216D4 */ s32                     taskId_Player;
    /* 0x216D8 */ s32                     taskId_Partner;
    /* 0x216DC */ s32                     taskId_UGFade;
    /* 0x216E0 */ s32                     unk_216E0;
    /* 0x216E4 */ s32                     unk_216E4;
    /* 0x216E8 */ s32                     unk_216E8;
    /* 0x216EC */ s32                     unk_216EC;
    /* 0x216F0 */ s32                     taskId_Watch;
    /* 0x216F4 */ s32                     taskId_Phone;
    /* 0x216F8 */ s32                     unk_216F8;
    /* 0x216FC */ s32                     taskId_EventMail;
    /* 0x21700 */ s32                     unk_21700;
    /* 0x21704 */ s32                     unk_21704;
    /* 0x21708 */ s32                     taskId_Mess;  // Task ID for message
    /* 0x2170C */ s32                     taskId_Plate; // Task ID for string plate
    /* 0x21710 */ s32                     taskId_Item;  // Task ID for items
    /* 0x21714 */ s32                     taskId_Arrow; // Task ID for which arrow
    /* 0x21718 */ s16*                    unk_21718;
    /* 0x2171C */ char                    unk_2171C[0x21728 - 0x2171C];
    /* 0x21728 */ s16*                    unk_21728;
    /* 0x2172C */ char                    unk_2172C[0x21738 - 0x2172C];
    /* 0x21738 */ s16*                    unk_21738;
    /* 0x2173C */ char                    unk_2173C[0x21748 - 0x2173C];
    /* 0x21748 */ u16*                    unk_21748;
    /* 0x2174C */ char                    unk_2174C[0x21758 - 0x2174C];
    /* 0x21758 */ u16                     unk_21758;
    /* 0x2175A */ u16                     unk_2175A[2];
    /* 0x2175E */ char                    unk_2175E[0x2176A - 0x2175E];
    /* 0x2176A */ u16                     unk_2176A;
    /* 0x2176C */ u16                     unk_2176C[8];
    /* 0x2177C */ s16                     unk_2177C;
    /* 0x2177E */ char                    unk_2177E[0x21780 - 0x2177E];
    /* 0x21780 */ u32                     taskId_Choice;
    /* 0x21784 */ s32                     unk_21784;
    /* 0x21788 */ u32                     taskId_Humans[2];
    /* 0x21790 */ char                    unk_21790[0x21918 - 0x21790];
    /* 0x21918 */ s32                     humanCount;
    /* 0x2191C */ s32                     unk_2191C;
    /* 0x21920 */ u16                     unk_21920;
    /* 0x21922 */ u16                     unk_21922;
    /* 0x21924 */ u32                     unk_21924[64];
    /* 0x21A24 */ s32                     unk_21A24;
    /* 0x21A28 */ struct Data*            unk_21A28;
    /* 0x21A2C */ void*                   unk_21A2C;
    /* 0x21A30 */ struct PaletteResource* humanPalette;
    /* 0x21A34 */ s32                     taskId_MapJumpArrows[8];
    /* 0x21A54 */ s32                     mapJumpArrowCount;
    /* 0x21A58 */ FieldExit*              activeExit;
    /* 0x21A5C */ s32                     unk_21A5C;
    /* 0x21A60 */ struct BgResource*      unk_21A60;
    /* 0x21A64 */ struct BgResource*      unk_21A64;
    /* 0x21A68 */ struct PaletteResource* unk_21A68;
    /* 0x21A6C */ s32                     unk_21A6C;
    /* 0x21A70 */ struct BgResource*      unk_21A70;
    /* 0x21A74 */ struct PaletteResource* unk_21A74;
    /* 0x21A78 */ s32                     unk_21A78;
    /* 0x21A7C */ struct BgResource*      unk_21A7C;
    /* 0x21A80 */ struct PaletteResource* unk_21A80;
    /* 0x21A84 */ struct BgResource*      eventPicScreen;
    /* 0x21A88 */ struct BgResource*      eventPicChar;
    /* 0x21A8C */ struct PaletteResource* eventPicPalette;
    /* 0x21A90 */ s32                     unk_21A90;
    /* 0x21A94 */ s32                     unk_21A94;
    /* 0x21A98 */ s32                     scanBgLevel;
    /* 0x21A9C */ struct ObjResource*     unk_21A9C;
    /* 0x21AA0 */ u32                     unk_21AA0;
    /* 0x21AA4 */ u32                     unk_21AA4;
    /* 0x21AA8 */ u32                     unk_21AA8;
    /* 0x21AAC */ u16                     unk_21AAC;
    /* 0x21AAE */ u16                     blendTimer;
    /* 0x21AB0 */ s32                     blendEva;
    /* 0x21AB4 */ s32                     blendEvb;
    /* 0x21AB8 */ s32                     canSpawnHumans;
    /* 0x21ABC */ s32                     unk_21ABC;
    /* 0x21AC0 */ s32                     unk_21AC0;
    /* 0x21AC4 */ s32                     unk_21AC4;
    /* 0x21AC8 */ s32                     unk_21AC8;
    /* 0x21ACC */ s32                     unk_21ACC;
    /* 0x21AD0 */ s32                     unk_21AD0;
    /* 0x21AD4 */ s32                     unk_21AD4;
    /* 0x21AD8 */ s32                     unk_21AD8;
    /* 0x21ADC */ s32                     unk_21ADC;
    /* 0x21AE0 */ s32                     unk_21AE0;
    /* 0x21AE4 */ s32                     unk_21AE4;
    /* 0x21AE8 */ s32                     unk_21AE8;
    /* 0x21AEC */ u16                     unk_21AEC;
    /* 0x21AEE */ char                    unk_21AEE[0x21AF0 - 0x21AEE];
    /* 0x21AF0 */ s32                     unk_21AF0;
    /* 0x21AF4 */ s16                     unk_21AF4;
    /* 0x21AF6 */ char                    unk_21AF6[0x21AF8 - 0x21AF6];
    /* 0x21AF8 */ FieldExit               unk_21AF8; // scripted exit
    /* 0x21B08 */ s32                     unk_21B08;
    /* 0x21B0C */ s32                     unk_21B0C;
    /* 0x21B10 */ s32                     scanActive;
    /* 0x21B14 */ s32                     scanPhase;
    /* 0x21B18 */ s32                     unk_21B18;
    /* 0x21B1C */ s32                     scanTimer;
    /* 0x21B20 */ s32                     unk_21B20;
    /* 0x21B24 */ s32                     unk_21B24;
    /* 0x21B28 */ s32                     taskId_LargeGingo;
    /* 0x21B2C */ s32                     taskId_Raster;
    /* 0x21B30 */ s32                     taskId_NSymbolUGMgr;
    /* 0x21B34 */ s16                     chainedEncounters;
    /* 0x21B36 */ char                    unk_21B36[2];
    /* 0x21B38 */ s32                     scanSlideY;
    /* 0x21B3C */ s32                     scanSeenIds[32];
    /* 0x21BBC */ u32                     unk_21BBC[35];
    /* 0x21C48 */ u32                     unk_21C48[32];
    /* 0x21CC8 */ char                    unk_21CC8[0x21CD0 - 0x21CC8];
    /* 0x21CD0 */ u32*                    unk_21CD0;
    /* 0x21CD4 */ s32                     unk_21CD4;
    /* 0x21CD8 */ s32                     unk_21CD8;
    /* 0x21CDC */ s32                     unk_21CDC;
    /* 0x21CE0 */ s32                     unk_21CE0;
    /* 0x21CE4 */ s32                     unk_21CE4;
    /* 0x21CE8 */ u16                     unk_21CE8;
    /* 0x21CEA */ u16                     unk_21CEA;
    /* 0x21CEC */ s32                     unk_21CEC;
    /* 0x21CF0 */ s32                     unk_21CF0;
    /* 0x21CF4 */ s32                     talkEventActive;
    /* 0x21CF8 */ s32                     unk_21CF8;
    /* 0x21CFC */ s32                     unk_21CFC;
    /* 0x21D00 */ s32                     unk_21D00;
    /* 0x21D04 */ s16                     unk_21D04;
    /* 0x21D06 */ s16                     unk_21D06;
    /* 0x21D08 */ s32 (*unk_21D08)(struct FieldEventManager*);
    /* 0x21D0C */ s32 unk_21D0C;
    /* 0x21D10 */ s16 targetBrightness; // Target fade brightness
    /* 0x21D12 */ s16 unk_21D12;
    /* 0x21D14 */ s32 eventPictureId;
    /* 0x21D18 */ s32 touchEventResult;
    /* 0x21D1C */ s16 touchEventTarget;
    /* 0x21D1E */ s16 unk_21D1E;
    /* 0x21D20 */ s16 unk_21D20;
    /* 0x21D22 */ s16 touchEventKind;
    /* 0x21D24 */ s32 (*unk_21D24)(struct FieldEventManager*);
    /* 0x21D28 */ s16          unk_21D28;
    /* 0x21D2A */ s16          unk_21D2A;
    /* 0x21D2C */ struct Data* unk_21D2C;
    /* 0x21D30 */ s32          unk_21D30;
    /* 0x21D34 */ char         unk_21D34[0x22E14 - 0x21D34];
    /* 0x22E14 */ s16          unk_22E14;
    /* 0x22E16 */ char         unk_22E16[2];
    /* 0x22E18 */ s32          unk_22E18;
    /* 0x22E1C */ s16          encounterHumanCount;
    /* 0x22E1E */ char         unk_22E1E[2];
    /* 0x22E20 */ s32          taskId_DebugText;
    /* 0x22E24 */ char         debugAreaName[0x22E2C - 0x22E24];
    /* 0x22E2C */ s32          unk_22E2C;
    /* 0x22E30 */ char         unk_22E30[4];
    /* 0x22E34 */ s32          unk_22E34;
    /* 0x22E38 */ s32          drawnHumanCount;
    /* 0x22E3C */ s32          lightLevel;
} FieldEventManager;

void FldEvent_SetCurrent(FieldEvent event);
void FldEvent_AdvanceScript(FieldEventManager* eventMgr);
void func_ov030_020a6a68(FieldEventManager* eventMgr);
void FldEvent_InitState(FieldEventManager* eventMgr);
s32  func_ov030_020a75c0(FieldEventManager* eventMgr);
void func_ov030_020a7b50(FieldEventManager* eventMgr);
void FldEvent_BeginScript(FieldEventManager* eventMgr, FieldEvent event, s32 arg2);
s32  func_ov030_020a9d00(FieldEventManager* eventMgr, FieldEvent event);
void func_ov030_020a9db8(FieldEventManager* eventMgr, FieldEvent event);
// Creates a choice window from count promoted integer message IDs.
s32  func_ov030_020a9f54(FieldEventManager* eventMgr, s32 (*callback)(FieldEventManager*), s32 arg2, u16 count, ...);
s32  func_ov030_020aa0c8(FieldEventManager* eventMgr, s32 arg1, s16 arg2, s16 arg3, s32 (*arg4)(FieldEventManager*));
void func_ov030_020aa9f4(FieldEventManager* eventMgr, s16 arg1, s16 arg2, s16 arg3);
u8   func_ov030_020aaad8(void);
void FldEvent_GrantItem(FieldEventManager* eventMgr, u16 itemID);
void FldEvent_GrantPin(FieldEventManager* eventMgr, u16 itemID);
void func_ov030_020aac28(FieldEventManager* eventMgr, u16 itemID);

#endif // FIELD_EVENT_EVENTMANAGER_H
