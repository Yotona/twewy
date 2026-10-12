#ifndef OTOSU_GAME_H
#define OTOSU_GAME_H

#include "Otosu/OtosuRegion.h"

#include "CriSndMgr.h"
#include "EasyFade.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/BinMgr.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/ScreenMapper.h"
#include "Engine/Text.h"
#include "Player/Inventory/Pins.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/fx/fx_atan.h>
#include <nitro/fx/fx_division.h>
#include <nitro/math.h>
#include <nitro/mi/cpumem.h>
#include <nitro/wm.h>

/// MARK: Scene

typedef void (*OtuStageHandler)();

typedef struct {
    /* 0x00 */ OtuStageHandler        enter;
    /* 0x04 */ OtuStageHandler        exit;
    /* 0x08 */ const OtuStageHandler* steps;
    /* 0x0C */ u32                    blockSize;
} OtuSceneStage; // Size: 0x10

typedef struct {
    /* 0x00 */ Heap*                heap;
    /* 0x04 */ const OtuSceneStage* stage;      // the running stage
    /* 0x08 */ s32                  stageIndex; // which of stage->steps runs
    /* 0x0C */ const OtuSceneStage* next;
    /* 0x10 */ BOOL                 pending;
    /* 0x14 */ void*                stageBlock;
} OtuStageDispatch; // Size: 0x18

typedef struct {
    /* 0x00 */ u8  specialUses[4]; // needle, meteo, hammer, hand
    /* 0x04 */ u8  tuneIndex;
    /* 0x05 */ u8  pad_05;
    /* 0x06 */ s16 friction;
    /* 0x08 */ u16 needleCharge;
    /* 0x0A */ u16 needleHold;
    /* 0x0C */ u16 meteoHold;
    /* 0x0E */ u16 meteoSquash;
    /* 0x10 */ s16 hammerRate;
    /* 0x12 */ s16 hammerLength;
    /* 0x14 */ u16 hammerFrames;
    /* 0x16 */ u16 hammerArc;
    /* 0x18 */ u16 stunFrames; // how long a hit stuns this pin
    /* 0x1A */ u8  pad_1A[0x02];
} OtuBadgeParam;               // Size: 0x1C

#define OTU_NO_PIN PIN_COUNT

typedef struct {
    /* 0x0 */ u16 ai;      // record of BeBadge_AI.bin (TinPinSlammer_Scene.ai) it plays with
    /* 0x2 */ u16 deck[7]; // pin ids, OTU_NO_PIN for an empty slot
} OtuMatchOpponent;        // Size: 0x10

typedef struct {
    /* 0x00 */ u16              board;     // which of the ten boards
    /* 0x02 */ u16              timeLimit; // seconds
    /* 0x04 */ OtuMatchOpponent opponents[3];
} OtuMatch;                                // Size: 0x34

typedef struct {
    /* 0x00000 */ ResourceManager  resMgr;
    /* 0x11580 */ ResourceManager* prevResMgr;
    /* 0x11584 */ s32              spareDataType;
    /* 0x11588 */ s32              dataType;
    /* 0x1158C */ Heap             heap;
    /* 0x11598 */ u8               heapBuffer[0x30000];
    /* 0x41598 */ TaskPool         pool1; // the fade and the screen-wide tasks
    /* 0x41618 */ TaskPool         pool2; // the board's tasks
    /* 0x41698 */ BOOL             playing;
    /* 0x4169C */ s32              fadeTask;
    /* 0x416A0 */ u8               pad_416A0[4];
    /* 0x416A4 */ BOOL             textReady;
    /* 0x416A8 */ u8               textRes[0xC];
    /* 0x416B4 */ TextObject       text; // the debug editor's text layer
    /* 0x417C8 */ u8               pad_417C8[0x2FC];
    /* 0x41AC4 */ OtuStageDispatch stage;
    /* 0x41ADC */ BOOL             done;      // the stage sequence has ended; leave the overlay
    /* 0x41AE0 */ BOOL             linkError; // leave through the connection-error screen
    /* 0x41AE4 */ u16              linkStatus;
    /* 0x41AE6 */ u8               pad_41AE6[2];
    /* 0x41AE8 */ s32              wirelessStatus;

    /* 0x41AEC */ u8            pad_41AEC[4];
    /* 0x41AF0 */ BOOL          multiplayer; // the wireless variant of the scene
    /* 0x41AF4 */ s32           unk_41AF4;
    /* 0x41AF8 */ u8            pad_41AF8[0x3E0];
    /* 0x41ED8 */ u8            parentBssid[6]; // the parent the wireless stage scans for
    /* 0x41EDE */ u8            pad_41EDE[2];
    /* 0x41EE0 */ s32           playerCount;
    /* 0x41EE4 */ BOOL          linkLost;
    /* 0x41EE8 */ BOOL          linkTimeout;
    /* 0x41EEC */ BOOL          linkOpen;
    /* 0x41EF0 */ OtuBadgeParam badgeParams[PIN_COUNT + 1]; // BeBadge_Parm.bin
    /* 0x4404C */ u16           decks[4][7];                // each player's pin ids
    /* 0x44084 */ OtuMatch      matches[44];                // BeBadge_Single.bin
    /* 0x44974 */ u8            ai[7][0x22];                // BeBadge_AI.bin
    /* 0x44A62 */ u8            pad_44A62[2];
    /* 0x44A64 */ s32           matchIndex;
    /* 0x44A68 */ s32           boardIndex;
} TinPinSlammer_Scene; // Size: 0x44A6C

/// MARK: Geometry and animation

typedef struct {
    /* 0x0 */ u16 frames;
    /* 0x4 */ s32 scaleX;
    /* 0x8 */ s32 scaleY;
} OtuScaleKey; // Size: 0xC

typedef struct {
    /* 0x0 */ const OtuScaleKey* keys;
    /* 0x4 */ u16                index;
    /* 0x6 */ u16                count;
    /* 0x8 */ u16                hold;
} OtuScaleAnim; // Size: 0xC

typedef struct {
    /* 0x00 */ s16 duration;
    /* 0x02 */ s16 value;
} OtuFrame4; // Size: 0x4

typedef struct {
    /* 0x00 */ OtuFrame4* table;
    /* 0x04 */ s16        index;
    /* 0x06 */ s16        count;
    /* 0x08 */ s16        framesLeft;
} OtuCursor; // Size: 0xA

typedef struct {
    /* 0x00 */ s16 offset;
    /* 0x02 */ s16 duration; // frames to hold before advancing
} OtuFrameSlot;              // Size: 0x4

typedef struct {
    /* 0x00 */ OtuFrameSlot* slots;
    /* 0x04 */ s16           count;
    /* 0x06 */ s16           index;
    /* 0x08 */ s16           remaining;
    /* 0x0C */ s32           base;
} OtuFrameCursor; // Size: 0x10

/// MARK: Board and task types

typedef struct {
    /* 0x000 */ Heap*                  heap;
    /* 0x004 */ struct OtuBoardLayout* layout;
    /* 0x008 */ Data*                  fileData;
    /* 0x00C */ PaletteResource*       palettes[4];
    /* 0x01C */ BgResource*            chars[4];
    /* 0x02C */ BgResource*            screens[4];
    /* 0x03C */ u32                    screenHeader;
    /* 0x040 */ u16                    screen[32][32];
    /* 0x840 */ OtuFrameCursor         paletteAnim;
} OtosuGame_ovbg;

typedef struct {
    /* 0x000 */ s32    dataType;
    /* 0x004 */ Sprite spriteA;
    /* 0x044 */ Sprite spriteB;
    /* 0x084 */ Sprite digits[4];
    /* 0x184 */ u32    pinId; // the badge whose count is shown
    /* 0x188 */ s32    index;
    /* 0x18C */ s32    resolved;
    /* 0x190 */ s32    hasSpriteA;
    /* 0x194 */ u16    visible; // bit N is digits[N]
    /* 0x196 */ u8     pad_196[2];
} OtosuGame_badgecount;         // Size: 0x198

typedef struct {
    /* 0x000 */ s32    dataType;
    /* 0x004 */ Sprite digits[3];
    /* 0x0C4 */ s32    countdown; // 1/60th ticks remaining
    /* 0x0C8 */ s32    visible;
    /* 0x0CC */ s32    alarmed;   // one-frame pulse at the 0x4B0 crossing
} OtosuGame_timer;                // Size: 0xD0

typedef struct {
    /* 0x00 */ u8 fromX;
    /* 0x01 */ u8 fromY;
    /* 0x02 */ u8 toX;
    /* 0x03 */ u8 toY;
} OtuBoardWarp; // Size: 0x4

typedef struct OtuBoardLayout {
    /* 0x00 */ u8 kind;    // picks the floor art; 1 also clears the OBJ layers each round
    /* 0x01 */ u8 variant; // the floor style; the obstacle factory's palette slot
    /* 0x02 */ u8 width;   // in cells
    /* 0x03 */ u8 height;
    /* 0x04 */ u8 obstacleCount;
    /* 0x05 */ u8 warpCount;
    /* 0x06 */ u8 pad_06[2];
    /* 0x08 */ u8 start[4][2];          // each badge's starting cell
    /* 0x10 */ u8 (*cells)[2];          // 0x1388 bytes from the stage heap
    /* 0x14 */ u8*           obstacles; // 8-byte obstacle records
    /* 0x18 */ OtuBoardWarp* warps;
} OtuBoardLayout;                       // Size: 0x1C

typedef struct {
    /* 0x00 */ u8  touching;
    /* 0x01 */ u8  syncStep; // the handshake step this player has reached
    /* 0x02 */ u8  x;
    /* 0x03 */ u8  y;
    /* 0x04 */ u16 buttons; // SysControl.buttonState.currButtons
} OtuInputPacket;           // Size: 0x6

typedef union {
    OtuInputPacket packets[4];
    u16            raw[0x100]; // a wireless data-sharing block
} OtuInputBuffer;              // Size: 0x200

#define OTU_TOUCH_HELD     0x1
#define OTU_TOUCH_PRESSED  0x2
#define OTU_TOUCH_RELEASED 0x4

typedef struct {
    /* 0x0 */ Vec2p pos;
    /* 0x8 */ s32   scale;
} OtuPinRecord; // Size: 0xC

typedef struct OtosuGame_badge {
    /* 0x000 */ TinPinSlammer_Scene*    scene;
    /* 0x004 */ s32                     dataType;
    /* 0x008 */ TaskPool*               pool;
    /* 0x00C */ Sprite                  spriteA;
    /* 0x04C */ Sprite                  spriteB;
    /* 0x08C */ Sprite                  spriteC;
    /* 0x0CC */ OamAffineParam          affine; // rotation follows `travel`
    /* 0x0DC */ s32                     visible;
    /* 0x0E0 */ s32                     index;
    /* 0x0E4 */ OtuBoardLayout*         board;
    /* 0x0E8 */ OtuInputPacket*         input; // the controlling player's input; NULL for the CPU
    /* 0x0EC */ u16                     prevButtons;
    /* 0x0EE */ u16                     pressedButtons;
    /* 0x0F0 */ u16                     touchFlags; // OTU_TOUCH_*
    /* 0x0F4 */ s32                     step;       // sub-phase within `kind`
    /* 0x0F8 */ s32                     phase;
    /* 0x0FC */ s32                     subKind;
    /* 0x100 */ s32                     frameBudget;
    /* 0x104 */ s32                     lastTileType;
    /* 0x108 */ s32                     lastCellX;
    /* 0x10C */ s32                     lastCellY;
    /* 0x110 */ Vec2p                   origin;
    /* 0x118 */ Vec2p                   homeOffset;
    /* 0x120 */ Vec2p                   pos; // Q12.12
    /* 0x128 */ s32                     height;
    /* 0x12C */ Vec2p                   vel;
    /* 0x134 */ s32                     vz;
    /* 0x138 */ Vec2p                   dir;
    /* 0x140 */ s32                     travel;
    /* 0x144 */ s32                     velMag;
    /* 0x148 */ s32                     stun;     // frames left stunned; the AI waits it out
    /* 0x14C */ Vec2p                   aimStart; // where the current aim began
    /* 0x154 */ Vec2p                   aimCur;   // where it is aiming now
    /* 0x15C */ Vec2p                   startPos; // the badge's starting cell, centred
    /* 0x164 */ s32                     respawnTimer;
    /* 0x168 */ s32                     flags;
    /* 0x16C */ u16*                    pinID;      // a tray slot; OTU_NO_PIN for an empty one
    /* 0x170 */ OtuBadgeParam*          params;     // indexed by pin id
    /* 0x174 */ u16*                    aiChances;  // chance out of 0x10000 for each AI behaviour
    /* 0x178 */ s16                     needleUses; // special attacks left, seeded from params->specialUses[]
    /* 0x17A */ s16                     meteoUses;
    /* 0x17C */ s16                     hammerUses;
    /* 0x17E */ s16                     handUses;
    /* 0x180 */ s32                     hitCount;   // how many of `hits` the current attack fills
    /* 0x184 */ OtuPinRecord            hits[2];    // the attack's hit points, with their reach
    /* 0x19C */ s16                     trailIndex;
    /* 0x19E */ s16                     trailTimer; // frames between track marks
    /* 0x1A0 */ s16                     pointCursor;
    /* 0x1A4 */ s32                     cameraCut;
    /* 0x1A8 */ s32                     hasLabel;
    /* 0x1AC */ s32                     score;
    /* 0x1B0 */ struct OtosuGame_badge* partner;    // the pin this one last touched
    /* 0x1B4 */ s32                     aiBehavior; // 0x11 = parked
    /* 0x1B8 */ s32                     aiTimer;
    /* 0x1BC */ struct OtosuGame_badge* chaseTarget;
    /* 0x1C0 */ Vec2p                   aiGoal;    // where the current AI sent us
    /* 0x1C8 */ s32                     smokeCursor;
    /* 0x1CC */ s32                     aimFrames; // 0x1E = commit
    /* 0x1D0 */ s32                     mode;      // 0x10 once the badge is committed
    /* 0x1D4 */ s32                     shadowId;
    /* 0x1D8 */ s32                     piyoId;
    /* 0x1DC */ s32                     markerId;
    /* 0x1E0 */ s32                     meteoId;
    /* 0x1E4 */ u32                     hammerId;
    /* 0x1E8 */ u32                     needleId;
    /* 0x1EC */ u32                     handId;
    /* 0x1F0 */ s32                     radarId;
    /* 0x1F4 */ s32                     counterId;
    /* 0x1F8 */ u32                     trackIds[12];
    /* 0x228 */ s32                     pointIds[2];
    /* 0x230 */ s32                     entryId;
    /* 0x234 */ s32                     deadId;
    /* 0x238 */ s32                     smokeIds[8];
    /* 0x258 */ u32                     warpId;
} OtosuGame_badge;

typedef char OtosuGame_badge_SizeMustBe_0x25C[(sizeof(OtosuGame_badge) == 0x25C) ? 1 : -1];

typedef struct {
    /* 0x00 */ TaskPool* pool;
    /* 0x04 */ s32       pinId;
    /* 0x08 */ Sprite    sprite;
    /* 0x48 */ Vec2p     origin;
    /* 0x50 */ Vec2p     pos;
    /* 0x58 */ Vec2p     vel;
    /* 0x60 */ s32       active;
    /* 0x64 */ s32       visible;
    /* 0x68 */ u8        pad_68[0x8];
} OtosuGame_track; // Size: 0x70

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ Sprite         labelSprite;
    /* 0x80 */ s32            visible;
    /* 0x84 */ s32            state; // 1 while showing, back to 0 when the timer runs out
    /* 0x88 */ s32            timer; // 0x78 frames
    /* 0x8C */ s32            hasLabel;
    /* 0x90 */ OamAffineParam affine0;
    /* 0xA0 */ OamAffineParam affine1;
    /* 0xB0 */ OtuScaleAnim   scaleAnim0;
    /* 0xBC */ OtuScaleAnim   scaleAnim1;
} OtosuGame_entry; // Size: 0xC8

typedef struct {
    s32 dataType;
} OtuTaskArgs1;

typedef struct {
    s32 dataType;
    s32 pinId;
} OtuPinSpriteArgs; // Size: 0x8

typedef struct {
    /* 0x00 */ Sprite         sprite0;
    /* 0x40 */ Sprite         sprite1; // animations 1 and 2
    /* 0x80 */ Sprite         sprite2;
    /* 0xC0 */ s32            visible;
    /* 0xC4 */ s32            which;   // which sprite: 0, 1 or 2 (sprite1), 3 (sprite2)
    /* 0xC8 */ s32            state;   // 0..3
    /* 0xCC */ s32            counter; // 0x3C frames per state
    /* 0xD0 */ OamAffineParam affine;
    /* 0xE0 */ OtuScaleAnim   scaleAnim;
} OtosuGame_gameover; // Size: 0xEC

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ Vec2p  origin;
    /* 0x48 */ Vec2p  pos;
    /* 0x50 */ s32    height;
    /* 0x54 */ Vec2p  dir;
    /* 0x5C */ s32    speed; // falls by `decel` each frame
    /* 0x60 */ s32    decel;
    /* 0x64 */ s32    unk_64;
    /* 0x68 */ s32    pinId;
    /* 0x6C */ s32    visible;
    /* 0x70 */ s32    state; // 1 intro, 2 hold for `hold` frames, 3 outro
    /* 0x74 */ s32    hold;
} OtosuGame_smoke;           // Size: 0x78

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ Vec2p  origin;
    /* 0x48 */ Vec2p  pos;
    /* 0x50 */ s32    pinId;
    /* 0x54 */ s32    active;
    /* 0x58 */ s32    visible;
} OtosuGame_warp; // Size: 0x5C

typedef struct {
    /* 0x00 */ u16 targetX;
    /* 0x02 */ u16 targetY;
    /* 0x04 */ u16 kind;     // 0..2; picks a row of each sprite table below
} OtosuGame_obstacle_Params; // Size: 0x6

typedef struct {
    /* 0x00 */ Sprite sprite; // Size: 0x40. Passed straight to the sprite API.
    /* 0x40 */ Vec2p  origin;
    /* 0x48 */ Vec2p  pos;    // Q12.12
    /* 0x50 */ s32    scale;  // 16.16 scale factor, picked by kind.
    /* 0x54 */ s32    animPending;
} OtosuGame_obstacle;         // Size: 0x58

typedef struct {
    /* 0x00 */ s16 duration; // frames to hold this entry
    /* 0x02 */ s16 value;
    /* 0x04 */ s16 scale;
} OtuFrame6; // Size: 0x6

typedef struct {
    /* 0x00 */ OtuFrame6* table;
    /* 0x04 */ s16        index;
    /* 0x06 */ s16        count;
    /* 0x08 */ s16        framesLeft;
} OtuCursor6; // Size: 0xA

typedef struct {
    /* 0x000 */ TaskPool*      pool;
    /* 0x004 */ Sprite         spriteA;
    /* 0x044 */ Sprite         spriteB;
    /* 0x084 */ OamAffineParam affineA; // scaleX > 0 reveals sprite A
    /* 0x094 */ OamAffineParam affineB; // scaleY is the shaft's current length
    /* 0x0A4 */ Vec2p          origin;
    /* 0x0AC */ Vec2p          posA;
    /* 0x0B4 */ Vec2p          posB;
    /* 0x0BC */ Vec2p          pinPos; // the pin the hammer swings around
    /* 0x0C4 */ s32            height;
    /* 0x0C8 */ s32            pinId;
    /* 0x0CC */ s32            live;
    /* 0x0D0 */ s32            showA;
    /* 0x0D4 */ s32            showB;
    /* 0x0D8 */ s32            scale;
    /* 0x0DC */ s32            angle0;
    /* 0x0E0 */ s32            angle;
    /* 0x0E4 */ s32            halfLen;
    /* 0x0E8 */ s32            state;
    /* 0x0EC */ s32            framesLeft;
    /* 0x0F0 */ s32            rate0;
    /* 0x0F4 */ s32            scale0;
    /* 0x0F8 */ s32            totalFrames;
    /* 0x0FC */ s32            angleBase;
    /* 0x100 */ OtuCursor      cursorScale;
    /* 0x10C */ OtuCursor6     cursorSpin;
    /* 0x118 */ OtuCursor6     cursorTrail;
    /* 0x124 */ s32            reportTimer;
    /* 0x128 */ s32            children[4]; // hammerhahen
} OtosuGame_hammer;                         // Size: 0x138

typedef struct {
    s32             dataType;
    Heap*           heap;
    OtuBoardLayout* layout;
} OtuBoardArgs;

typedef struct {
    /* 0x00 */ OtuFrameCursor   anim;
    /* 0x10 */ PaletteResource* palette;
} OtuFloorPalette; // Size: 0x14

typedef struct {
    /* 0x00 */ s32              dataType;
    /* 0x04 */ Heap*            heap;
    /* 0x08 */ s32              loaded;
    /* 0x0C */ OtuBoardLayout*  layout;
    /* 0x10 */ ScreenMapEntry   bg;
    /* 0x38 */ u8**             tiles;
    /* 0x3C */ u8*              tilePool; // 0x800 bytes per 8x8-tile cell
    /* 0x40 */ Data*            animData;
    /* 0x44 */ Data*            data;
    /* 0x48 */ BgResource*      animChars;
    /* 0x4C */ PaletteResource* palette;
    /* 0x50 */ BgResource*      chars;
    /* 0x54 */ OtuFloorPalette  animPalettes[5];
} OtosuGame_floor; // Size: 0xB8

typedef struct {
    /* 0x00 */ s32              dataType;
    /* 0x04 */ Heap*            heap;
    /* 0x08 */ s32              active; // the layout has background layers at all
    /* 0x0C */ OtuBoardLayout*  layout;
    /* 0x10 */ Data*            data[2];
    /* 0x18 */ PaletteResource* palettes[2];
    /* 0x20 */ BgResource*      chars[2];
    /* 0x28 */ ScreenMapEntry   maps[2];
    /* 0x78 */ void*            buffers[2];
    /* 0x80 */ s32              widths[2]; // Q12.12, the wrap points of the scroll
    /* 0x88 */ s32              heights[2];
    /* 0x90 */ s32              scrollX[2];
    /* 0x98 */ s32              scrollY[2];
} OtosuGame_bg; // Size: 0xA0

typedef struct {
    /* 0x000 */ s32            step;      // the wireless stages' handshake step
    /* 0x004 */ u16            readyMask; // one bit per player that has answered
    /* 0x006 */ u16            packetKind;
    /* 0x008 */ s32            gotPacket;
    /* 0x00C */ WMBssDesc      parent;
    /* 0x0CC */ u16            active;
    /* 0x0CE */ u8             pad_0CE[0x72];
    /* 0x140 */ s32            badgeCount;
    /* 0x144 */ s32            obstacleCount;
    /* 0x148 */ s32            sparkCursor;
    /* 0x14C */ s32            stageIndex;
    /* 0x150 */ OtuBoardLayout layout;
    /* 0x16C */ OtuMatch*      match;
    /* 0x170 */ s32            timerSeconds; // the round's time limit
    /* 0x174 */ s32            timer;
    /* 0x178 */ s32            outcome;
    /* 0x17C */ s32            badgeIds[4];
    /* 0x18C */ s32            sparkIds[0x40];
    /* 0x28C */ s32            obstacleIds[8];
    /* 0x2AC */ s32            slashId;
    /* 0x2B0 */ s32            floorId;
    /* 0x2B4 */ s32            bgId;
    /* 0x2B8 */ s32            ovbgId;
    /* 0x2BC */ s32            timerId;
    /* 0x2C0 */ s32            gaugeId;
    /* 0x2C4 */ s32            gameoverId;
    /* 0x2C8 */ s32            wriconId;
    /* 0x2CC */ s32            wrwaitId;
} OtuBoardStage; // Size: 0x2D0

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ OamAffineParam affine;
    /* 0x50 */ Vec2p          origin;
    /* 0x58 */ Vec2p          pos;
    /* 0x60 */ s32            height;
    /* 0x64 */ s32            pinId;
    /* 0x68 */ s32            alive;
    /* 0x6C */ s32            state;
    /* 0x70 */ s32            timer;
    /* 0x74 */ s32            hahenIds[8];
    /* 0x94 */ OtuScaleAnim   scaleAnim;
} OtosuGame_meteo; // Size: 0xA0

/// MARK: Overlay data

extern OtuSceneStage OtuScene_DisconnectStage;

extern OtuSceneStage OtuScene_EditorStage;

extern OtuSceneStage OtuScene_SinglePlayerBoard;

extern OtuSceneStage OtuScene_HostBoard;
extern OtuSceneStage OtuScene_GuestBoard;

extern OtuStageHandler OtuScene_SinglePlayerBoardSteps[1];
extern OtuStageHandler OtuScene_GuestBoardSteps[6];
extern OtuStageHandler OtuScene_HostBoardSteps[6];

typedef struct {
    /* 0x0 */ s32 launch;
    /* 0x4 */ s32 accel;  // speed the badge accelerates to
    /* 0x8 */ s32 power;  // how hard it pushes on contact
    /* 0xC */ s32 weight; // how hard it is to push
} OtuPinTune;             // Size: 0x10

extern OtuPinTune OtuPinTunes[12];

extern s32 OtuDebris_LaunchMin;
extern s32 OtuDebris_Bounce;
extern s32 OtuDebris_Decel;
extern s32 OtuPhys_ConveyorSpeed;
extern s32 OtuDebris_LaunchMax;
extern s32 OtuDebris_Gravity;
extern s32 OtuDebris_SpeedMin;
extern s32 OtuPhys_Gravity;
extern s32 OtuPhys_JumpVelocity;
extern s32 OtuPhys_HandJumpVelocity;
extern s32 OtuPhys_WallBounce;
extern s32 OtuDebris_SpeedMax;

extern s32 OtuPhys_StunnedImpact;
extern s32 OtuPhys_HandDashSpeed;
extern s32 OtuPhys_SpinDecay;
extern s32 OtuPhys_SpinPower;
extern s32 OtuPhys_Knockback;

extern s32 OtuPhys_Friction[4];

// The pin game calls FX_Atan2Idx without its prototype in scope, so the result is narrowed by the caller.
#define FX_Atan2IdxNoProto ((s32(*)())FX_Atan2Idx)

extern u8  OtuScene_SyncStep;
extern s32 OtuScene_InputStalled;

extern const BinIdentifier OtuFile_Badge[5];
extern const BinIdentifier OtuFile_CommonObj;
extern const BinIdentifier OtuFile_UnderBg;
extern const BinIdentifier OtuFile_UnderObj;
extern const BinIdentifier OtuFile_OverBg;
extern const BinIdentifier OtuFile_OverObj;
extern const BinIdentifier OtuFile_Stage2Obj;
extern const BinIdentifier OtuFile_BadgeParams;
extern const BinIdentifier OtuFile_MapData;
extern const BinIdentifier OtuFile_SingleMatches;
extern const BinIdentifier OtuFile_AI;

/// MARK: Helpers

#define OTU_AIRTIME(vz) ((FX_Divide((vz), OtuDebris_Gravity) * 3) >> 0xC)

#define OTU_ABS_AIRTIME(vz) (OTU_AIRTIME(vz) < 0 ? -OTU_AIRTIME(vz) : OTU_AIRTIME(vz))

/// MARK: Functions

void OtuScene_LoadPlayerDeck(TinPinSlammer_Scene* scene);
void OtuScene_InitText(TinPinSlammer_Scene* scene);
void OtuScene_ReleaseText(TinPinSlammer_Scene* scene);

void OtuScene_Init(TinPinSlammer_Scene* scene);
void OtuScene_InitSinglePlayer(TinPinSlammer_Scene* scene);
void OtuScene_InitMultiplayer(TinPinSlammer_Scene* scene);
void OtuScene_DeleteFade(TinPinSlammer_Scene* scene);
void OtuScene_RunStages(TinPinSlammer_Scene* scene);
void OtuScene_RunStagesWireless(TinPinSlammer_Scene* scene);

void OtuScene_InitGraphics(void);

void OtuScene_InitDisplay(void);
void OtuScene_InitDisplayWireless(void);

void OtuScene_ClearVBlank(void);

void OtuScaleAnim_Init(OtuScaleAnim* anim, const OtuScaleKey* keys, u16 count, OamAffineParam* affine);
void OtuScaleAnim_Step(OtuScaleAnim* anim, OamAffineParam* affine);
void OtuBoard_SetSyncStep(TinPinSlammer_Scene* scene, s32 step);
void OtuBoard_ReceiveDeck(TinPinSlammer_Scene* scene, s32 slot, const void* src, s32 len);
void OtuBoard_PlaySEAt(SndMgrSeIdx seIdx, Vec2p* from, Vec2p* to);
void OtuBoard_ForgetPartner(TinPinSlammer_Scene* scene, void* badge);

OtosuGame_badge* OtuBoard_FindNearestTargetable(TaskPool* pool, TinPinSlammer_Scene* scene, s32 index);
OtosuGame_badge* OtuBoard_FindNearestHammer(TaskPool* pool, TinPinSlammer_Scene* scene, s32 index);
OtosuGame_badge* OtuBoard_FindNearestMeteo(TaskPool* pool, TinPinSlammer_Scene* scene, s32 index);

s32 OtuBoard_GetObstacleCount(TinPinSlammer_Scene* scene);

void* OtuBoard_GetObstacle(TinPinSlammer_Scene* scene, s32 index);

s32 OtuScene_GetOwnPlayer(s32 mode);

OtuInputPacket* OtuScene_GetPlayerInput(s32 slot);

void OtuScene_ExchangeInputs(void);

void OtuScene_ReadLocalInput(OtuInputPacket* input);
void OtuScene_UpdateLocalInput(void);

s32  OtuBoard_OnGuestConnected(void* record, TinPinSlammer_Scene* scene);
void OtuBoard_OnGuestDisconnected(s32 unused, TinPinSlammer_Scene* scene);

void OtuBoard_OnScanResult(WMBssDesc* bss, TinPinSlammer_Scene* scene);

void OtuBoard_SetupMultiplayer(TinPinSlammer_Scene* scene);

void OtuBoard_Teardown(TinPinSlammer_Scene* scene);
void OtuBoard_SpawnSparks(TinPinSlammer_Scene* scene, Vec2p* at);

void OtuBoard_Update(TinPinSlammer_Scene* scene);

void OtuScene_SinglePlayerBoard_Enter(TinPinSlammer_Scene* scene);
void OtuScene_SinglePlayerBoard_Exit(TinPinSlammer_Scene* scene);
void OtuScene_HostBoard_Enter(TinPinSlammer_Scene* scene);
void OtuScene_HostBoard_Exit(TinPinSlammer_Scene* scene);

void OtuScene_HostBoard_StartHost(TinPinSlammer_Scene* scene);
void OtuScene_HostBoard_SyncDecks(TinPinSlammer_Scene* scene);
void OtuScene_HostBoard_SetupBoard(TinPinSlammer_Scene* scene);
void OtuScene_HostBoard_WaitStart(TinPinSlammer_Scene* scene);
void OtuScene_HostBoard_Play(TinPinSlammer_Scene* scene);
void OtuScene_HostBoard_Disconnect(TinPinSlammer_Scene* scene);
void OtuScene_GuestBoard_ConnectToHost(TinPinSlammer_Scene* scene);
void OtuScene_GuestBoard_SyncDecks(TinPinSlammer_Scene* scene);
void OtuScene_GuestBoard_SetupBoard(TinPinSlammer_Scene* scene);
void OtuScene_GuestBoard_WaitStart(TinPinSlammer_Scene* scene);
void OtuScene_GuestBoard_Play(TinPinSlammer_Scene* scene);
void OtuScene_GuestBoard_Disconnect(TinPinSlammer_Scene* scene);

s32  OtosuGame_badge_Collide(OtosuGame_badge* a, OtosuGame_badge* b);
s32  OtosuGame_badge_Separate(OtosuGame_badge* a, OtosuGame_badge* b);
s32  OtosuGame_badge_CollideObstacle(OtosuGame_badge* a, OtosuGame_obstacle* b);
void OtosuGame_badge_GetDir(OtosuGame_badge* self, Vec2p* out);

void OtosuGame_badge_GetPos(OtosuGame_badge* task, Vec2p* out);
s32  OtosuGame_badge_GetHeight(OtosuGame_badge* task);

void OtosuGame_badge_GetCameraPos(OtosuGame_badge* self, Vec2p* out);
void OtosuGame_badge_SetOrigin(OtosuGame_badge* self, Vec2p* origin);

void OtosuGame_badge_GetOrigin(OtosuGame_badge* self, Vec2p* out);
void OtosuGame_badge_SetHomeOffset(OtosuGame_badge* self, s32 x, s32 y);
void OtosuGame_badge_GetHomeOffset(OtosuGame_badge* self, Vec2p* out);
s32  OtosuGame_badge_IsShown(OtosuGame_badge* task);
s32  OtosuGame_badge_GetShadowScale(OtosuGame_badge* task);
s32  OtosuGame_badge_GetMeteoAimState(OtosuGame_badge* task);

s32 OtosuGame_badge_IsUsingMeteo(OtosuGame_badge* task);
s32 OtosuGame_badge_IsUsingHammer(OtosuGame_badge* task);
s32 OtosuGame_badge_GetBackAngle(OtosuGame_badge* task);
s32 OtosuGame_badge_IsUsingNeedle(OtosuGame_badge* task);
s32 OtosuGame_badge_IsUsingHand(OtosuGame_badge* task);

s32  OtosuGame_badge_UpdateHits(TaskPool* pool, OtosuGame_badge* self);
void OtosuGame_badge_TakeHit(OtosuGame_badge* other, OtosuGame_badge* self);
s32  OtosuGame_badge_IsStunned(OtosuGame_badge* task);
s32  OtosuGame_badge_GetStun(OtosuGame_badge* task);
s16  OtosuGame_badge_GetNeedleUses(OtosuGame_badge* task);
s16  OtosuGame_badge_GetMeteoUses(OtosuGame_badge* task);
s16  OtosuGame_badge_GetHammerUses(OtosuGame_badge* task);
s16  OtosuGame_badge_GetHandUses(OtosuGame_badge* task);
s32  OtosuGame_badge_IsAiming(OtosuGame_badge* pin);
void OtosuGame_badge_GetAimLine(void* pin, Vec2p* a, Vec2p* b);
s32  OtosuGame_badge_IsCharged(OtosuGame_badge* pin);
s32  OtosuGame_badge_IsTargetable(void* task, u32 hiddenChance);
s32  OtosuGame_badge_CanBeTargeted(OtosuGame_badge* task, s32 which);
s32  OtosuGame_badge_TakeCameraCut(OtosuGame_badge* task);
s32  OtosuGame_badge_HasPin(OtosuGame_badge* self);
void OtosuGame_badge_AddSurvivalBonus(OtosuGame_badge* self);
s32  OtosuGame_badge_GetScore(OtosuGame_badge* task);
void OtosuGame_badge_ResetAI(OtosuGame_badge* task);
void OtosuGame_badge_SpawnSmoke(OtosuGame_badge* self, Vec2p* at, s32 selector);
s32  OtosuGame_badge_IsOutOfPins(OtosuGame_badge* self);
void OtosuGame_badge_HideEntry(OtosuGame_badge* task);
void OtosuGame_badge_ForgetPartner(OtosuGame_badge* self, OtosuGame_badge* other);
void OtosuGame_badge_HidePoints(OtosuGame_badge* self);

void OtosuGame_meteo_GetHitRecord(OtosuGame_meteo* self, OtuPinRecord* out);
void OtosuGame_meteo_Start(OtosuGame_meteo* self, s32 frames);
void OtosuGame_meteo_Impact(OtosuGame_meteo* self);
void OtosuGame_meteo_Recover(OtosuGame_meteo* self);

s32  OtosuGame_hammer_GetHitRecord(OtosuGame_hammer* self, OtuPinRecord* out);
s32  OtosuGame_hammer_IsActive(OtosuGame_hammer* self);
void OtosuGame_hammer_Start(OtosuGame_hammer* self, s32 rate, s32 scale, s32 frames, u16 arc);
void OtosuGame_hammer_EndSwing(OtosuGame_hammer* self);
u16  OtosuGame_hammer_GetAngle(OtosuGame_hammer* self);
void OtosuGame_hammer_Shatter(OtosuGame_hammer* self);

void OtosuGame_floor_SetScroll(OtosuGame_floor* self, s32 x, s32 y);
void OtosuGame_floor_Refresh(OtosuGame_floor* self, s32 event);

void OtosuGame_obstacle_SetOrigin(OtosuGame_obstacle* self, Vec2p* origin);
void OtosuGame_obstacle_GetPos(OtosuGame_obstacle* self, Vec2p* out);
s32  OtosuGame_obstacle_GetScale(OtosuGame_obstacle* self);
void OtosuGame_obstacle_PlayHit(OtosuGame_obstacle* self);

void OtosuGame_bg_SetScroll(OtosuGame_bg* self, s32 x, s32 y);
void OtosuGame_bg_Refresh(OtosuGame_bg* self, s32 event);

void OtosuGame_ovbg_StartPaletteAnim(OtosuGame_ovbg* data);

s32  OtosuGame_badgecount_ShowPinCount(OtosuGame_badgecount* data, u16* slots);
void OtosuGame_badgecount_SetValue(OtosuGame_badgecount* data, s32 value);

s32 OtosuGame_timer_GetCountdown(OtosuGame_timer* self);
s32 OtosuGame_timer_IsAlarmed(OtosuGame_timer* self);

void OtosuGame_track_Place(OtosuGame_track* self, Vec2p* at, s32 angle, s32 dir, s32 len);

void OtosuGame_entry_Show(OtosuGame_entry* self, s16 which, s32 hasLabel);
void OtosuGame_entry_Hide(OtosuGame_entry* self);

void OtosuGame_gameover_Show(OtosuGame_gameover* self, s32 which);
s32  OtosuGame_gameover_IsActive(OtosuGame_gameover* self);

void OtosuGame_smoke_Start(OtosuGame_smoke* self, Vec2p* at, s32 selector, s32 closeBy, s32 step, s32 holdFor);

void OtosuGame_warp_Start(OtosuGame_warp* self, Vec2p* at, s32 animation);
s32  OtosuGame_warp_IsActive(OtosuGame_warp* self);

void                 OtuStageDispatch_Init(OtuStageDispatch* dispatch, Heap* heap);
void                 OtuStageDispatch_SetNext(OtuStageDispatch* dispatch, const OtuSceneStage* next);
void                 OtuStageDispatch_NextStep(OtuStageDispatch* dispatch);
void                 OtuStageDispatch_Enter(OtuStageDispatch* dispatch, void* scene);
void                 OtuStageDispatch_Run(OtuStageDispatch* dispatch, void* scene);
void                 OtuStageDispatch_Exit(OtuStageDispatch* dispatch, void* scene);
s32                  OtuStageDispatch_IsFinished(OtuStageDispatch* dispatch);
const OtuSceneStage* OtuStageDispatch_GetStage(OtuStageDispatch* dispatch);
void*                OtuStageDispatch_GetBlock(OtuStageDispatch* dispatch);

void OtuVec_Copy(Vec2p* src, Vec2p* dst);
void OtuVec_Add(Vec2p* a, Vec2p* b, Vec2p* out);

void OtuVec_Sub(Vec2p* a, Vec2p* b, Vec2p* out);
void OtuVec_Scale(s32 angle, Vec2p* a, Vec2p* out);
void OtuVec_ScaleAdd(s32 angle, Vec2p* a, Vec2p* b, Vec2p* out);
s32  OtuVec_Dot(Vec2p* a, Vec2p* b);
s32  OtuVec_Cross(Vec2p* a, Vec2p* b);
s32  OtuVec_Distance(Vec2p* a, Vec2p* b);
s32  OtuVec_Length(Vec2p* v);
void OtuVec_Normalize(Vec2p* src, Vec2p* dest);

s32 OtuFrameCursor_Init(OtuFrameCursor* cursor, s32 base, OtuFrameSlot* slots, s16 count);
s32 OtuFrameCursor_Step(OtuFrameCursor* cursor);

/// MARK: Tasks

s32 OtosuGame_wrwait_CreateTask(TaskPool* pool, s32 dataType);

s32 OtosuGame_marker_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);

s32 OtosuGame_dead_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);

s32 OtosuGame_wricon_CreateTask(TaskPool* pool, s32 dataType);

s32 OtosuGame_piyo_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);

s32 OtosuGame_needle_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);

s32 OtosuGame_hand_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);

s32 OtosuGame_slash_CreateTask(TaskPool* pool, s32 dataType, s32 pin);

s32 OtosuGame_shadow_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);

s32 OtosuGame_point_CreateTask(TaskPool* pool, s32 dataType, s32 pin);

s32 OtosuGame_hammerhahen_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);

s32 OtosuGame_meteo_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);
s32 OtosuGame_obstacle_CreateTask(TaskPool* pool, s32 dataType, s16 boardKind, s16 slot, OtosuGame_obstacle_Params* params);
s32 OtosuGame_needlehahen_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);
s32 OtosuGame_meteohahen_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);
s32 OtosuGame_smoke_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);
s32 OtosuGame_warp_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);
s32 OtosuGame_gameover_CreateTask(TaskPool* pool, s32 dataType);
s32 OtosuGame_track_CreateTask(TaskPool* pool, s32 dataType, s32 pin);
s32 OtosuGame_entry_CreateTask(TaskPool* pool, s32 dataType);
s32 OtosuGame_specialgauge_CreateTask(TaskPool* pool, s32 dataType, s32 pin);
s32 OtosuGame_badgecount_CreateTask(TaskPool* pool, s32 dataType, s32 pinId, s32 index, s32 hasSpriteA);
s32 OtosuGame_badgeradar_CreateTask(TaskPool* pool, s32 dataType, s32 pinId, s32 index, OtuBoardLayout* board, s32 isFirst);
s32 OtosuGame_spark_CreateTask(TaskPool* pool, s32 dataType);
s32 OtosuGame_timer_CreateTask(TaskPool* pool, s32 dataType, s32 seconds);
u32 OtosuGame_hammer_CreateTask(TaskPool* pool, s32 dataType, s32 pinId);
s32 OtosuGame_floor_CreateTask(TaskPool* pool, s32 dataType, Heap* heap, OtuBoardLayout* layout);
s32 OtosuGame_bg_CreateTask(TaskPool* pool, s32 dataType, Heap* heap, OtuBoardLayout* layout);
s32 OtosuGame_ovbg_CreateTask(TaskPool* pool, s32 dataType, Heap* heap, OtuBoardLayout* layout);

/// MARK: Outside the overlay

void func_02025b68(void* resource, UnkStruct_usedby_02025b68* args);
void func_02025e30(void* resource);

s32 func_02047e84(u16 bits);

void func_02047338(void);

s32  func_ov040_0209cb78(void);
void func_ov040_0209d588(void);
s32  func_ov040_0209d728(void);
void func_ov040_0209d970(void* callback);
void func_ov040_0209d990(void);
void func_ov040_0209ece4(void);
void func_ov040_0209ed20(void);
void func_ov040_0209ed30(void);
void func_ov040_0209d434(void* callback, void* arg);

#endif
