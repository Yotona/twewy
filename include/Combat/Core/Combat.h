#ifndef COMBAT_CORE_COMBAT_H
#define COMBAT_CORE_COMBAT_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatList.h"
#include "Combat/Core/CombatMic.h"
#include "Combat/Core/CombatTouch.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Save/MainData.h"
#include "SndMgrSeIdx.h"

/// One per screen engine; `BtlEnm015_Shake` jitters the offset at 0x44..0x4C.
typedef struct Ov003Camera {
    /* 0x00 */ union {
        s16 unk_00;
        u16 pathId;
    };
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 posY;
    /* 0x18 */ s32 posX;
    /* 0x1C */ u16 rotation;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28; // screen-space centre X used by the battle projection
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 unk_34;
    /* 0x38 */ s32 unk_38;
    /* 0x3C */ s32 unk_3C;
    /* 0x40 */ s32 unk_40; // screen-space centre Y used by the battle projection
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
    /* 0x50 */ s32 shakeY;
    /* 0x54 */ s32 shakeX;
} Ov003Camera; // Size: 0x58

/// The camera shake animation driven through the battle camera views.
typedef struct Ov003CameraAnim {
    /* 0x00 */ s32          index; // which of the two cameras
    /* 0x04 */ Ov003Camera* camera;
    /* 0x08 */ s32          timer; // -1 = idle
    /* 0x0C */ s32          period;
    /* 0x10 */ s32          dir;   // 1 = negative offset
    /* 0x14 */ s32          offset;
} Ov003CameraAnim;                 // Size: 0x18

typedef struct Ov003EffectSlots {
    union {
        struct {
            s32 unk_00;
            s32 unk_04;
            s32 unk_08;
        };
        Vec position;
    };
    /* 0x0C */ union {
        s32 unk_0C;
        void (*onAbsorb)(void*);
    };
    /* 0x10 */ union {
        s32   unk_10;
        void* callbackArg;
    };
    /* 0x14 */ u8 cooldown;
    /* 0x15 */ u8 unk_15;
} Ov003EffectSlots; // Size: 0x18

typedef struct Ov003ActionEffect {
    /* 0x00 */ u16               unk_00;
    /* 0x02 */ u16               unk_02;
    /* 0x04 */ Ov003EffectState* state;
    /* 0x08 */ CombatActor*      target;
    /* 0x0C */ s32               angle;
    /* 0x10 */ CombatListNode    node;
} Ov003ActionEffect; // Size: 0x1C

typedef struct Ov003ActionEffectPool {
    /* 0x000 */ CombatList        free;
    /* 0x008 */ CombatList        active;
    /* 0x010 */ Ov003ActionEffect entries[8];
} Ov003ActionEffectPool; // Size: 0xF0

typedef union Ov003AbilityFlags {
    struct {
        /* 0x0 */ u8 valid;
        /* 0x1 */ u8 usable[3];
    };
    u32 raw;
} Ov003AbilityFlags;

typedef struct Ov003AbilitySlot {
    /* 0x0 */ s32 actionId;
    /* 0x4 */ s32 amount;
} Ov003AbilitySlot; // Size: 0x8

typedef struct Ov003AbilityData {
    /* 0x00 */ Ov003AbilityFlags flags;
    /* 0x04 */ Ov003AbilitySlot  slots[4];
} Ov003AbilityData; // Size: 0x24

typedef struct Ov003TouchEvent {
    /* 0x00 */ u32              unk_00; // ability id, 0xFF = free slot
    /* 0x04 */ u32              anyTouch;
    /* 0x08 */ Ov003AbilityData ability;
    /* 0x2C */ u32              slot; // touch slot the event belongs to
} Ov003TouchEvent;                    // Size: 0x30

typedef struct Ov003ChargeEntry {
    /* 0x00 */ u16            value;
    /* 0x02 */ u16            unk_02;
    /* 0x04 */ CombatListNode node;
} Ov003ChargeEntry; // Size: 0x10

typedef struct Ov003ChargeState {
    /* 0x000 */ u16              unk_00;
    /* 0x002 */ u16              flags;
    /* 0x004 */ u16              unk_04;
    /* 0x006 */ u16              unk_06;
    /* 0x008 */ s32              unk_08;
    /* 0x00C */ CombatList       unk_0C;
    /* 0x014 */ CombatList       unk_14;
    /* 0x01C */ Ov003ChargeEntry entries[16];
} Ov003ChargeState; // Size: 0x11C

typedef struct Ov003EffectCmd {
    /* 0x00 */ u8  mode;     // 0 = charge (difficulty-sized), 1 = seal, 2 = special
    /* 0x01 */ s8  current;  // index into the charge-step table
    /* 0x02 */ s8  count;    // steps in the current charge step sequence
    /* 0x03 */ s8  progress; // charge accumulated so far
    /* 0x04 */ s8  next;     // step length of the entry at `current`
    /* 0x05 */ s8  stage;    // charge stage (per partner progress)
    /* 0x06 */ u16 flags;
    /* 0x08 */ u8  unk_08[0x8];
    /* 0x10 */ s16 remaining; // entries left in `queue`
    /* 0x12 */ u16 capacity;
    /* 0x14 */ u16 queue[12];
    /* 0x2C */ s32 unk_2C; // mode-specific handle (battle difficulty index, ...)
} Ov003EffectCmd;          // Size: 0x30

typedef struct {
    /* 0x00000 */ TaskPool              unk_00000;
    /* 0x00080 */ Heap                  heap_00080;
    /* 0x0008C */ u8                    heapData_00080[0x8000];
    /* 0x0808C */ TaskPool              taskPool;
    /* 0x0810C */ Heap                  heap_0810C;
    /* 0x08118 */ u8                    heapData_0810C[0x8000];
    /* 0x10118 */ TaskPool              unk_10118;
    /* 0x10198 */ Heap                  heap_10198;
    /* 0x101A4 */ u8                    heapData_10198[0x10000];
    /* 0x201A4 */ TaskPool              unk_201A4;
    /* 0x20224 */ Heap                  heap_20224;
    /* 0x20230 */ u8                    heapData_20224[0x14000];
    /* 0x34230 */ TaskPool              unk_34230;
    /* 0x342B0 */ Heap                  heap_342B0;
    /* 0x342BC */ u8                    heapData_342B0[0x8000];
    /* 0x3C2BC */ TaskPool              unk_3C2BC;
    /* 0x3C33C */ Heap                  heap_3C33C;
    /* 0x3C348 */ u8                    heapData_3C33C[0x1000];
    /* 0x3D348 */ PaletteResource*      unk3D348[2];
    /* 0x3D350 */ Data*                 unk3D350;
    /* 0x3D354 */ Data*                 unk3D354;
    /* 0x3D358 */ Ov003EffectSlots      effectSlots;
    /* 0x3D370 */ BtlTouchWork          touchWork;
    /* 0x3D3CC */ Ov003ActionEffectPool actionEffects;
    /* 0x3D4BC */ Ov003TouchEvent       touchEvents[14];
    /* 0x3D75C */ u8                    unk_3D75C[4];
    union {
        BtlMicInput microphone;
        struct {
            /* 0x3D760 */ u8  touchLayerData[0x20];
            /* 0x3D780 */ u32 unk3D780;
            /* 0x3D784 */ u32 unk3D784;
            /* 0x3D788 */ u32 unk3D788;
            /* 0x3D78C */ u32 unk3D78C;
            /* 0x3D790 */ u32 unk3D790;
            /* 0x3D794 */ u32 unk3D794;
            /* 0x3D798 */ u32 unk3D798;
            /* 0x3D79C */ u32 unk3D79C;
            /* 0x3D7A0 */ u8  unk_3D7A0[0x3D7C0 - 0x3D7A0];
        };
    };
    /* 0x3D7C0 */ Ov003Camera       unk3D7C0[2]; // [0] main engine, [1] sub engine
    /* 0x3D870 */ s32               unk3D870;
    /* 0x3D874 */ u8                unk3D874;    // battle state
    /* 0x3D875 */ u8                unk3D875;    // battle sub state
    /* 0x3D876 */ char              unk_3D876[0x2];
    /* 0x3D878 */ u32               unk3D878;
    /* 0x3D87C */ s32               unk3D87C;
    /* 0x3D880 */ s32               unk3D880;
    /* 0x3D884 */ s32               unk3D884;
    /* 0x3D888 */ s32               unk3D888;
    /* 0x3D88C */ s8                unk3D88C; // battle index within the chain, 0-based
    /* 0x3D88D */ u8                unk3D88D; // active partner (PARTNER_*)
    /* 0x3D88E */ u16               unk3D88E; // encounter id of the current battle
    /* 0x3D890 */ u8                unk3D890; // battle music id
    /* 0x3D892 */ s16               unk3D892; // drop rate
    /* 0x3D894 */ s16               unk3D894;
    /* 0x3D896 */ s16               unk3D896; // partner sync rate
    /* 0x3D898 */ void*             unk3D898; // main display level-up gauge actor
    /* 0x3D89C */ void*             unk3D89C; // sub display level-up gauge actor
    /* 0x3D8A0 */ void*             unk3D8A0;
    /* 0x3D8A4 */ CombatList        unk3D8A4; // units the sub screen attacks
    /* 0x3D8AC */ CombatList        unk3D8AC; // units the main screen attacks
    /* 0x3D8B4 */ CombatList        unk3D8B4;
    /* 0x3D8BC */ CombatList        unk3D8BC;
    /* 0x3D8C4 */ u16               touchActorCount;
    /* 0x3D8C6 */ u8                unk_3D8C6[0x2];
    /* 0x3D8C8 */ s32               unk3D8C8;
    /* 0x3D8CC */ s32               unk3D8CC;
    /* 0x3D8D0 */ s16               unk3D8D0; // level-up gauge value
    /* 0x3D8D2 */ s16               unk3D8D2; // level-up gauge maximum
    /* 0x3D8D4 */ s16               unk3D8D4; // banked gauge A
    /* 0x3D8D6 */ s16               unk3D8D6; // banked gauge B
    /* 0x3D8D8 */ s16               unk3D8D8; // second gauge value
    /* 0x3D8DA */ s16               unk3D8DA; // second gauge maximum
    /* 0x3D8DC */ s16               unk3D8DC; // second gauge banked A
    /* 0x3D8DE */ s16               unk3D8DE; // second gauge banked B
    /* 0x3D8E0 */ s32               unk3D8E0;
    /* 0x3D8E4 */ Ov003ChargeState* chargeState;
    /* 0x3D8E8 */ Ov003EffectCmd*   effectCmd;
    /* 0x3D8EC */ u8                unk3D8EC;
    /* 0x3D8ED */ s8                unk3D8ED;
    /* 0x3D8EE */ s16               unk3D8EE;
    /* 0x3D8F0 */ s32               unk3D8F0;
    /* 0x3D8F4 */ s32               unk3D8F4;
    /* 0x3D8F8 */ s16               unk3D8F8;
    /* 0x3D8FA */ s16               unk3D8FA;
    /* 0x3D8FC */ s8                popupCountMain; // live message popups, main screen
    /* 0x3D8FD */ s8                popupCountSub;  // live message popups, sub screen
    /* 0x3D8FE */ u8                unk3D8FE;
    /* 0x3D8FF */ u8                unk3D8FF;
    /* 0x3D900 */ u16               unk3D900;
    /* 0x3D902 */ u16               unk3D902;
    /* 0x3D904 */ s16               unk3D904; // level-up flash timer, main screen
    /* 0x3D906 */ s16               unk3D906; // level-up flash timer queue, main screen
    /* 0x3D908 */ s16               unk3D908; // level-up flash timer, sub screen
    /* 0x3D90A */ s16               unk3D90A; // level-up flash timer queue, sub screen
    /* 0x3D90C */ s32               unk3D90C;
} Ov003Global;                                // Size: 0x3D910

extern Ov003Global* data_ov003_020e71b8;

// Used in TaskStage.render functions
void func_ov003_020c4878(void*);

void func_ov003_020c48fc(void*);

void func_ov003_02087f00(SndMgrSeIdx seIdx, s32 sePan);

// Creates a psych task and forwards its ID (-1 on allocation/initialization failure).
s32 func_ov003_020a6000(const TaskHandle* handle, void* arg);

/// One round collider that pushes its neighbours away (`Ov003Global` keeps three lists of them, by side).
typedef struct Ov003Collider {
    /* 0x00 */ s32            side; // which list the collider is in
    /* 0x04 */ s32            x;
    /* 0x08 */ s32            y;
    /* 0x0C */ s32            z;
    /* 0x10 */ s32            radius;
    /* 0x14 */ s32            height;
    /* 0x18 */ u16            flags;   // bit 0: switched off
    /* 0x1A */ u16            reserved1A;
    /* 0x1C */ u32            hitMask; // one bit per side that touched this collider
    /* 0x20 */ s32            pushX;
    /* 0x24 */ s32            pushY;
    /* 0x28 */ CombatListNode node;
} Ov003Collider; // Size: 0x34

/// One 8-byte record of an `Ov003AnimLayer`'s frame table.
typedef struct Ov003AnimFrame {
    /* 0x00 */ u16 dataId;
    /* 0x02 */ s16 entry;
    /* 0x04 */ u16 duration;
    /* 0x06 */ s16 randomRange;
} Ov003AnimFrame; // Size: 0x8

/// One layer of a layered sprite animation: a chain of slots that share a frame index.
typedef struct Ov003AnimLayer {
    /* 0x00 */ BinIdentifier*        iden;
    /* 0x04 */ Data*                 data;
    /* 0x08 */ s32                   unk_08;
    /* 0x0C */ void**                dest; // receives the current cell pointer
    /* 0x10 */ const Ov003AnimFrame* frames;
    /* 0x14 */ s16                   loadedId;
    /* 0x16 */ s16                   index;
    /* 0x18 */ s16                   duration;
    /* 0x1A */ s16                   timer;
    union {
        /* 0x1C */ struct {
            u16 loop    : 1; // restart from frame 0 at the end
            u16 oneShot : 1; // reached the end and stopped
            u16 random  : 1; // `duration` of 0 means "pick a random one in [1, randomRange]"
            u16 chained : 1; // driven by a parent layer
            u16 ended   : 1; // the end marker was already hit once
            u16 _pad1C  : 11;
        };
        u16 flags;
    };
    /* 0x1E */ s16                    unk_1E;
    /* 0x20 */ struct Ov003AnimLayer* next;
} Ov003AnimLayer; // Size: 0x24

typedef struct Ov003CutinSaveState {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 slots[9];
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ u32 magic;
} Ov003CutinSaveState; // Size: 0x34

/// The persistent settings array (25 limits and tunables), validated with a magic word.
typedef struct Ov003Settings {
    /* 0x00 */ s32 reserved00;
    /* 0x04 */ s32 values[25];
    /* 0x68 */ s32 unk_68;
    /* 0x6C */ s32 unk_6C;
    /* 0x70 */ s32 magic;
    /* 0x74 */ s8  current;
    /* 0x75 */ u8  reserved75[3];
} Ov003Settings; // Size: 0x78

/// The battle runtime that extends past the serialized MainData prefix in gSaveData's allocation.
typedef struct Ov003SaveRuntime {
    /* 0x0000 */ MainData            save;
    /* 0x3420 */ Ov003CutinSaveState cutin;
    /* 0x3454 */ Ov003Settings       settings;
} Ov003SaveRuntime;

#define OV003_SAVE ((Ov003SaveRuntime*)&gSaveData)

/// Battle scene work area, allocated from `gDebugHeap` and referenced from the overlay dispatcher
/// (`MainOvlDisp_SetCbArg`) and the debug overlay stack.
typedef struct BtlMgrWork {
    /* 0x00000 */ TaskPool        taskPool;
    /* 0x00080 */ Heap            heap;
    /* 0x0008C */ u8              heapData[0x800];
    /* 0x0088C */ ResourceManager resources;
    /* 0x11E0C */ s32             active;         // tasks are only ticked while a battle is running
    /* 0x11E10 */ u16             step;           // result/continue state machine step
    /* 0x11E12 */ u16             musicId;        // BGM currently requested
    /* 0x11E14 */ u16             encounterId;    // noise id of the battle being fought
    /* 0x11E16 */ u16             battleDuration; // frames the current chained battle lasts
    /* 0x11E18 */ s16             fadeCounter;
    /* 0x11E1A */ s16             unk_11E1A;
    /* 0x11E1C */ s32             keepBattleState; // carry the level-up gauge into the next battle
} BtlMgrWork;                                      // Size: 0x11E20

#endif                                             // COMBAT_CORE_COMBAT_H
