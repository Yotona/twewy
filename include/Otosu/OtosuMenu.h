#ifndef OTOSUMENU_H
#define OTOSUMENU_H

#include "CriSndMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Engine/Resources/ScreenMapper.h"
#include "PrcMaster.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"
#include "common_data.h"
#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>
#include <nitro/wm.h>

/// MARK: Types

/** @brief Header of a board in data/BeBadge_MapData.bin. */
typedef struct {
    /* 0x0 */ u8 unk_0;
    /* 0x1 */ u8 unk_1;
    /* 0x2 */ u8 width;
    /* 0x3 */ u8 height;
    /* 0x4 */ u8 unk_4[0xC];
} OtosuMenuMapHeader; // Size: 0x10

/** @brief One Tin Pin Slammer participant as the lobby knows them. */
typedef struct {
#ifdef REGION_USA
    /* 0x00 */ u16 name[21]; // USA: room for the longer English opponent names
#else
    /* 0x00 */ u16 name[11];
#endif
    u8 bssid[6];   // MAC address of the player's console
} OtosuMenuPlayer; // Size: 0x30 (JP: 0x1C)

/** @brief What a host advertises to consoles looking for a match. */
typedef struct {
    /* 0x00 */ u16 name[11];
    /* 0x16 */ u8  unk_16[0x1C - 0x16];
    /* 0x1C */ u16 playerCount;
} OtosuMenuBeacon; // Size: 0x1E

/** @brief A host found while scanning for matches. */
typedef struct {
    /* 0x00 */ WMBssDesc bss;
    /* 0xC0 */ s32       timeout; // frames until the host is dropped unless it is seen again
} OtosuMenuFoundHost;             // Size: 0xC4

typedef struct {
    /* 0x00000 */ ResourceManager    resMgr;
    /* 0x11580 */ ResourceManager*   prevResMgr;
    /* 0x11584 */ s32                unk_11584;
    /* 0x11588 */ s32                unk_11588;
    /* 0x1158C */ s32                objDataType;
    /* 0x11590 */ Heap               heap;
    /* 0x1159C */ u8                 heapBuffer[0x30000];
    /* 0x4159C */ TaskPool           taskPool;
    /* 0x4161C */ PrcCtx             mainCtx;
    /* 0x41804 */ PrcMaster          prcMaster;
    /* 0x4180C */ s32                unk_4180C;
    /* 0x41810 */ s32                unk_41810;
    /* 0x41814 */ s32                unk_41814;
    /* 0x41818 */ s32                unk_41818;
    /* 0x4181C */ u16                ownName[11];
    /* 0x41832 */ u8                 ownPlayer;  // this console's index in players[]
    /* 0x41834 */ u16                playerMask; // bit per connected player
    /* 0x41836 */ u16                playerDirtyMask;
    /* 0x41838 */ OtosuMenuPlayer    players[4]; // JP: every later field sits 0x50 earlier
    /* 0x418F8 */ char               unk_418F8[0x58];
    /* 0x41950 */ s32                sendBusy;
    /* 0x41954 */ s32                rosterChanged;
    /* 0x41958 */ u16                rosterVersion;
    /* 0x4195A */ u16                guestAckMask[4];    // per player
    /* 0x41962 */ u16                guestAckVersion[4]; // per player
    /* 0x4196A */ u16                syncState;
    /* 0x4196C */ u16                board;
    /* 0x4196E */ OtosuMenuBeacon    beacon;
    /* 0x4198C */ s32                beaconUpdateBusy;
    /* 0x41990 */ s32                beaconUpdatePending;
    /* 0x41994 */ s32                acceptingGuests;
    /* 0x41998 */ u16                foundHostMask;
    /* 0x4199A */ u16                foundHostCount;
    /* 0x4199C */ u16                foundHostChangedMask;
    /* 0x4199E */ u16                unk_4199E;
    /* 0x419A0 */ OtosuMenuFoundHost foundHosts[6];
    /* 0x41E38 */ OtosuMenuBeacon    foundHostBeacons[6];
    /* 0x41EEC */ u32                selectedHost;
    /* 0x41EF0 */ WMBssDesc          joinHost;              // the host this console is joining
    /* 0x41FB0 */ u8                 hostBssid[6];
    /* 0x41FB6 */ u8                 connectedBssids[4][6]; // players[].bssid of the connected players, packed in order
    /* 0x41FCE */ char               unk_41FCE[0x41FD0 - 0x41FCE];
    /* 0x41FD0 */ s32                matchPlayerCount;
    /* 0x41FD4 */ u8                 unk_41FD4;
    /* 0x41FD5 */ u8                 placePlayers[4];  // player in each finishing place, best score first
    /* 0x41FD9 */ u8                 placeRanks[4];    // rank shown for each place (ties share a rank)
    /* 0x41FDD */ u8                 playerRanks[4];   // rank of each player
    /* 0x41FE1 */ char               unk_41FE1[0x41FE4 - 0x41FE1];
    /* 0x41FE4 */ BOOL               firstPlaceUnique; // no other player tied the winner's score
    /* 0x41FE8 */ u8                 outcome;
    /* 0x41FE9 */ u8                 commErrorScreen;
    /* 0x41FEA */ char               unk_41FEA[0x41FF0 - 0x41FEA];
    /* 0x41FF0 */ s32                wirelessLoaded;
    /* 0x41FF4 */ char               nameChars[0x45FF4 - 0x41FF4];
    /* 0x45FF4 */ SysFont            font;
    /* 0x46070 */ s16                unk_46070;
    /* 0x46072 */ char               unk_46072[0x46074 - 0x46072];
    /* 0x46074 */ u32                nextScene; // 0 stays in the menu; otherwise which overlay OtosuMenu_Update switches to
    /* 0x46078 */ Sprite             cursor;
    /* 0x460B8 */ BOOL               cursorActive;
    /* 0x460BC */ s32                wirelessActive;
    /* 0x460C0 */ Sprite             linkLevelIcon;
    /* 0x46100 */ PrcCtx             linkLevelCtx;
    /* 0x462E8 */ s32                linkLevelVisible;
    /* 0x462EC */ s32                isSinglePlayer;
    /* 0x462F0 */ Data*              packs[4];
    /* 0x46300 */ s32                paletteAnimActive;
    /* 0x46304 */ OtosuMenuMapHeader mapHeader; // board map from data/BeBadge_MapData.bin
    /* 0x46314 */ u8*                mapCells;  // mapHeader.width * mapHeader.height cells, two bytes each
    /* 0x46318 */ void*              mapExtra1;
    /* 0x4631C */ void*              mapExtra2;
    /* 0x46320 */ u16                tilemap[32][64]; // generated screen for a text layer
    /* 0x47320 */ BgResource*        subChars[4];     // indexed by BG layer
    /* 0x47330 */ BgResource*        mainChars[4];
    /* 0x47340 */ void*              subPalette;
    /* 0x47344 */ void*              mainPalette;
    /* 0x47348 */ ScreenMapEntry     subMaps[4];
    /* 0x473E8 */ ScreenMapEntry     mainMaps[4];
    /* 0x47488 */ void*              subScreens[4][2]; // screen blocks each map is built from
    /* 0x474A8 */ void*              mainScreens[4][2];
    /* 0x474C8 */ u16                unk_474C8;
    /* 0x474CA */ u16                unk_474CA;
    /* 0x474CC */ u16                unk_474CC;
    /* 0x474CE */ u16                unk_474CE;
    /* 0x474D0 */ u16                paletteAnimFrame;
    /* 0x474D2 */ u16                paletteAnimTimer;
    /* 0x474D4 */ char               unk_474D4[0x474D8 - 0x474D4];
    /* 0x474D8 */ s32                bg0ScrollX;
    /* 0x474DC */ s32                bg1ScrollX;
    /* 0x474E0 */ s32                unk_474E0;
    /* 0x474E4 */ s32                unk_474E4;
    /* 0x474E8 */ PrcCtx             objCtx[6]; // contexts of the screen's own objects (icons, title enemies, rank boards...)
    /* 0x48058 */ SysFont*           fontListFont; // font the FontList screen draws its sample with
    /* 0x4805C */ char               unk_4805C[0x48068 - 0x4805C];
} OtosuMenuObj;                                    // Size: 0x48068

typedef struct {
    u16 unk0;
    u16 unk2;
} Ov002_U16_2;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
    };
    u16 data[4];
} Ov002_U16_4;

/// MARK: Wireless lobby

/** @brief What the wireless layer reports about a guest that joined or left the session. */
typedef struct {
    /* 0x00 */ u8  unk_00[0xA];
    /* 0x0A */ u8  bssid[6];
    /* 0x10 */ u16 aid; // the guest's slot in the session (0-3)
    /* 0x12 */ u8  unk_12[2];
    /* 0x14 */ u16 name[11];
    /* 0x2A */ u8  player; // the guest's own index in players[], sent after its name
} OtosuMenuChildInfo;

/** @brief Packet the host broadcasts to its guests. */
typedef struct {
    /* 0x00 */ u16 syncState;
    /* 0x02 */ u16 playerMask;
    /* 0x04 */ u16 names[4][11];
    /* 0x5C */ u8  bssids[4][6];
    /* 0x74 */ u16 rosterVersion;
    /* 0x76 */ u16 board;
    /* 0x78 */ u8  unk_78[0x80 - 0x78]; // only 0x78 bytes are sent
} OtosuMenuHostPacket;                  // Size: 0x80

/** @brief Packet each guest sends back to the host. */
typedef struct {
    /* 0x0 */ u16 playerMask; // the roster the guest last received
    /* 0x2 */ u16 rosterVersion;
    /* 0x4 */ u8  bssid[6];
    /* 0xA */ u8  unk_0A[0x20 - 0xA]; // only 0xA bytes are sent
} OtosuMenuGuestPacket;               // Size: 0x20

/// MARK: Screen layouts

/**
 * @brief A screen rectangle tagged with an id. Text layouts use the id as a message index
 * (OtosuMenu_DrawTexts draws each one into its box); touch layouts use it as a button id
 * (OtosuMenu_GetTouchedButton returns the id of the touched box). Lists end at an id of 0xFFFF.
 */
typedef struct {
    /* 0x0 */ u16 id;
    /* 0x2 */ u16 left;
    /* 0x4 */ u16 top;
    /* 0x6 */ u16 right;
    /* 0x8 */ u16 bottom;
} OtosuMenuRect; // Size: 0xA

/*
 * Screens keep their layouts in const tables and copy them into locals by struct assignment
 * (the copies land right where each table is used), so each list length has its own wrapper.
 */
typedef struct {
    OtosuMenuRect rects[1];
} OtosuMenuRectList1;

typedef struct {
    OtosuMenuRect rects[2];
} OtosuMenuRectList2;

typedef struct {
    OtosuMenuRect rects[3];
} OtosuMenuRectList3;

typedef struct {
    OtosuMenuRect rects[4];
} OtosuMenuRectList4;

typedef struct {
    OtosuMenuRect rects[5];
} OtosuMenuRectList5;

typedef struct {
    OtosuMenuRect rects[10];
} OtosuMenuRectList10;

/** @brief Where the selection cursor sits over a button, and which animation it plays there. */
typedef struct {
    /* 0x0 */ s16 anim;
    /* 0x2 */ u16 x;
    /* 0x4 */ u16 y;
} OtosuMenuCursor; // Size: 0x6

/// MARK: Title-screen objects

/** @brief Enter argument of a title-screen enemy object. */
typedef struct {
    /* 0x0 */ u16 index;    // which enemy (0-3): picks its cells and palette
    /* 0x2 */ u16 duration; // frames its entrance slide takes
} OtosuMenu_TitleEnmArg;

/** @brief Work object of a title-screen enemy ("OtosuMenu_TitleEnmObj"). */
typedef struct {
    /* 0x000 */ u16    index;
    /* 0x004 */ Sprite mainSprites[2]; // upper and lower half on the main screen
    /* 0x084 */ Sprite subSprites[2];  // the same two halves on the sub screen
    /* 0x104 */ fx32   x;
    /* 0x108 */ fx32   y;
    /* 0x10C */ fx32   progress; // eased towards 1.0 alongside the slide
    /* 0x110 */ u16    duration;
    /* 0x112 */ u16    timer;    // frames left in the slide
} OtosuMenu_TitleEnmObj;         // Size: 0x114

/** @brief Enter argument of the title-screen boss object: slide length and the two BG maps it scrolls. */
typedef struct {
    /* 0x0 */ u16             duration;
    /* 0x4 */ ScreenMapEntry* upper;
    /* 0x8 */ ScreenMapEntry* lower;
} OtosuMenu_TitleBossArg;

/** @brief Work object of the title-screen boss ("OtosuMenu_TitleBossObj"). */
typedef struct {
    /* 0x000 */ u8              unk_000[0x104];
    /* 0x104 */ ScreenMapEntry* upper;
    /* 0x108 */ ScreenMapEntry* lower;
    /* 0x10C */ u8              unk_10C[0x110 - 0x10C];
    /* 0x110 */ fx32            scrollY;
    /* 0x114 */ u16             duration;
    /* 0x116 */ u16             timer;
} OtosuMenu_TitleBossObj; // Size: 0x118

/// MARK: Results screen

/** @brief Enter argument of a rank board: one finishing place on the results screen. */
typedef struct {
    /* 0x0 */ BOOL winner; // sole first place: gold board and labels
    /* 0x4 */ u8   place;  // row on screen, best score first
    /* 0x5 */ u8   rank;   // shared by players with equal scores
    /* 0x6 */ u8   player;
    /* 0x8 */ u16  score;
} OtosuMenu_RankBoardArg; // Size: 0xC

/// MARK: Data shared between the Otosu menu TUs

extern const BinIdentifier  OtosuMenu_BinId;
extern const BinIdentifier  OtosuMenu_ObjBinId;
extern const BinIdentifier  OtosuMenu_DelMenuBinId;
extern const BinIdentifier  OtosuMenu_MenuBgLowerBinId;
extern const BinIdentifier  OtosuMenu_MenuBgUpperBinId;
extern const BinIdentifier  OtosuMenu_FldDownScreenBinId;
extern const Ov002_U16_4    OtosuMenu_BroadcastBssid;
extern PrcStepFn            PrcSteps_FadeBrightImmediate[];
extern PrcStepFn            PrcSteps_FadeBright[];
extern PrcStepFn            PrcSteps_FadeDark[];
extern PrcFrameDesc         OtosuMenu_Title_FrameDesc;
extern PrcFrameDesc         OtosuMenu_RoleSelect_FrameDesc;
extern PrcFrameDesc         OtosuMenu_StageSelect_GuestFrameDesc;
extern PrcFrameDesc         OtosuMenu_StageSelect_HostFrameDesc;
extern PrcFrameDesc         OtosuMenu_CommError_ConnectionLostFrameDesc;
extern PrcFrameDesc         OtosuMenu_CommError_JoinFailedPopFrameDesc;
extern PrcFrameDesc         OtosuMenu_CommError_WirelessUnavailableFrameDesc;
extern PrcFrameDesc         OtosuMenu_CommError_JoinFailedFrameDesc;
extern PrcStepFn            OtosuMenu_CommError_ConnectionLostStepTable[];
extern PrcStepFn            OtosuMenu_CommError_WirelessUnavailableStepTable[];
extern PrcStepFn            OtosuMenu_CommError_JoinFailedPopStepTable[];
extern PrcStepFn            OtosuMenu_CommError_JoinFailedStepTable[];
extern PrcFrameDesc         OtosuMenu_Lobby_JoinFrameDesc;
extern PrcFrameDesc         OtosuMenu_Lobby_HostFrameDesc;
extern PrcFrameDesc         OtosuMenu_LinkLevel_FrameDesc;
extern PrcFrameDesc         OtosuMenu_Icon_FrameDesc;
extern PrcFrameDesc         OtosuMenu_Ranking_HostFrameDesc;
extern PrcFrameDesc         OtosuMenu_Ranking_SinglePlayerFrameDesc;
extern PrcFrameDesc         OtosuMenu_Ranking_GuestFrameDesc;
extern PrcFrameDesc         OtosuMenu_TitleEnemy_FrameDesc;
extern PrcFrameDesc         OtosuMenu_TitleBoss_FrameDesc;
extern PrcFrameDesc         OtosuMenu_RankBoard_FrameDesc;
extern PrcFrameDesc         OtosuMenu_Daiza_FrameDesc;
extern PrcFrameDesc         OtosuMenu_Entry_MultiplayerFrameDesc;
extern PrcFrameDesc         OtosuMenu_Entry_SinglePlayerFrameDesc;
extern PrcFrameDesc         OtosuMenu_FontList_FrameDesc;
extern PrcFrameDesc         OtosuMenu_SaveData_LoadFailureFrameDesc;
extern PrcFrameDesc         OtosuMenu_SaveData_SaveFailureFrameDesc;
extern PrcFrameDesc         OtosuMenu_SaveData_InitializeFrameDesc;
extern PrcFrameDesc         OtosuMenu_SaveData_DeleteFrameDesc;
extern PrcFrameDesc         OtosuMenu_SaveData_CorruptedFrameDesc;
extern PrcFrameDesc         OtosuMenu_DelDataWin_FrameDesc;
extern PrcFrameDesc         OtosuMenu_Icon2_FrameDesc;
extern OtosuMenuGuestPacket OtosuMenu_GuestPacket;
extern OtosuMenuHostPacket  OtosuMenu_HostPacket;
extern BOOL
    OtosuMenu_TitleSkipped; // set once the title has jumped to its idle screen; title objects snap to their end positions

/// MARK: Functions

u16*          func_ov002_020824a0(void);
void          OtosuMenu_DestroyCursorAndLinkLevel(OtosuMenuObj* menuObj);
PrcStepResult OtosuPrcStep_SaveOwnPlayer(PrcCtx* ctx, void* object);
PrcStepResult OtosuPrcStep_SavePlayers(PrcCtx* ctx, void* object);
PrcStepResult OtosuPrcStep_LoadPlayers(PrcCtx* ctx, void* object);
PrcStepResult OtosuPrcStep_FadeStart_BrightImmediate(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeStart_DarkImmediate(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeWait_Immediate(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeStart_Bright(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeStart_Dark(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeWait(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeStart_Neutral(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeWait_Neutral(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeStart_Neutral2(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeWait_Neutral2(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeStart_NeutralSlow(PrcCtx* ctx, void* unused);
PrcStepResult OtosuPrcStep_FadeWait_NeutralSlow(PrcCtx* ctx, void* unused);
void          OtosuMenu_DrawPlayerNames(OtosuMenuObj* menuObj);
void          OtosuMenu_DrawTexts(SysFont* font, const OtosuMenuRect* texts, void* chars, void* screen);
void          OtosuMenu_SetupScreens(OtosuMenuObj* menuObj, s32 bg1Screen, s32 bg2Screen, const OtosuMenuRect* texts);
void          OtosuMenu_SetupConnectionLostScreens(OtosuMenuObj* menuObj, const OtosuMenuRect* texts);
void          OtosuMenu_SetupEntryScreens(OtosuMenuObj* menuObj);
void          OtosuMenu_SetupTitleScreens(OtosuMenuObj* menuObj);
void          OtosuMenu_SetupStageSelectScreens(OtosuMenuObj* menuObj, u8 player, const OtosuMenuRect* texts);
void          OtosuMenu_SetupResultMessageScreens(OtosuMenuObj* menuObj, const OtosuMenuRect* texts);
void          OtosuMenu_SetupResultRankingScreens(OtosuMenuObj* menuObj, s32 bg1Screen, s32 bg2Screen, s32* names,
                                                  const OtosuMenuRect* texts);
void          OtosuMenu_ReleaseScreens(OtosuMenuObj* menuObj);
u16           OtosuMenu_GetTouchedButton(const OtosuMenuRect* buttons);
void          OtosuMenu_LoadCursorAndLinkLevel(OtosuMenuObj* menuObj);
void          ProcessOverlay_OtosuMenu_DataInitialize(void* menuObj);
void          OtosuMenu_InitDisplayForEntry(void);
void          OtosuMenu_InitDisplay(void);
void          OtosuMenu_InitDisplayForSaveData(void);
void          OtosuMenu_DestroyResultObjects(OtosuMenuObj* menuObj);
void          OtosuMenu_ComputeRanks(OtosuMenuObj* menuObj);
void          OtosuMenu_ComputeSinglePlayerRanks(OtosuMenuObj* menuObj);
u16*          OtosuMenu_GetPlaceName(OtosuMenuObj* menuObj, u16 place);

/// MARK: Launchers

/**
 * @brief Launcher for displaying a test screen that shows pin symbols
 * and a cyclable text field containing either battle tutorials or generic samples
 */
void ProcessOverlay_OtosuMenu_FontList(void* menuObj);

/**
 * @brief Launcher for starting single player mode of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_SinglePlayerEnter(void* menuObj);

/**
 * @brief Launcher for displaying single player rankings of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_SinglePlayerRanking(void* menuObj);

/**
 * @brief Launcher for starting multiplayer mode of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_MultiplayerEnter(void* menuObj);

/**
 * @brief Launcher for displaying multiplayer rankings of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_MultiplayerRanking(void* menuObj);

/**
 * @brief Launcher for displaying connection error screen of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_ConnectionError(void* menuObj);

/**
 * @brief Launcher for the role selection screen of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_RoleSelection(void* menuObj);

/**
 * @brief Launcher for the save-data deletion confirmation screen
 */
void ProcessOverlay_OtosuMenu_DataDeletion(void* menuObj);

/**
 * @brief Launcher for the save-data corrupted notice
 */
void ProcessOverlay_OtosuMenu_DataCorrupted(void* menuObj);

/**
 * @brief Launcher for the save-data load failure notice
 */
void ProcessOverlay_OtosuMenu_DataLoadFailure(void* menuObj);

/**
 * @brief Launcher for the save-data save failure notice
 */
void ProcessOverlay_OtosuMenu_DataSaveFailure(void* menuObj);

#endif // OTOSUMENU_H
