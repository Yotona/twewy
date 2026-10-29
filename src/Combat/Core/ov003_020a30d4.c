#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include "EasyFade.h"
#include "Engine/Color.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Save.h"
#include "common_data.h"

#include <nitro/fx.h>
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>

// The touch-action side of the battle UI: the 0x30-byte touch event queue, the touch action
// actors linked into `Ov003Global + 0x3D8BC` and the pin/psych bookkeeping that feeds them.
// The actors themselves are created by `ov003_020a3b9c.c`.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The 0x24-byte record `func_020274ac` loads out of `Data/AbilityData.bin` for one ability id.
/// Byte 0 of `flags` says whether the ability is usable by any touch at all, bytes 1..3 are the
/// per-touch usability flags; the four slots are the touch actions the ability grants.

/// One touch action an ability grants: the action id and the amount it contributes.

/// One queued touch event; the 14-entry pool sits at `Ov003Global + 0x3D4BC`.

/// The 6-byte rendering of the six touch slots that the 3D layer reads back.

/// The 0x10-byte action description `func_ov003_020a37a8` hands to the psych entry points.
typedef struct Ov003PinAction {
    /* 0x00 */ u16               pinId;
    /* 0x02 */ s16               level;
    /* 0x04 */ u8                unk_04;
    /* 0x05 */ u8                unk_05[3];
    /* 0x08 */ const RawPinData* pin;
    /* 0x0C */ u8                slot;
    /* 0x0D */ u8                unk_0D[3];
} Ov003PinAction; // Size: 0x10

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern s32  func_020274ac(void*, s32, const BinIdentifier*);
extern s32  func_02016900(void);
extern void ADXT_SetExtraInfo(s32, s32);

extern void               func_ov003_02083478(void);
extern void               func_ov003_020835d4(void);
extern void               func_ov003_02083700(void);
extern Ov003TouchActor*   func_ov003_0208495c(CombatList*);
extern Ov003TouchActor*   func_ov003_02084984(CombatListNode*);
extern void               func_ov003_020849ac(CombatList*);
extern void               func_ov003_020849bc(CombatList*, CombatListNode*, void*);
extern s32                func_ov003_020a410c(Ov003TouchActor*, s16, s16);
extern void               func_ov003_020a45d4(Ov003TouchActor*);
extern const EquippedPin* func_ov003_020a4718(s32, s32);
extern s32                func_ov003_020a6cc4(u8);

/// The per-touch start positions written by `func_ov003_020a39dc` (owned by arm9).
extern const u8 data_0205c19d[];
/// Task handle of the touch icon task (owned by the battle setup unit).
extern const TaskHandle data_ov003_020d7d24;
extern char             data_ov003_020e3e64[];
extern char             data_ov003_020e3e68[];
extern char             data_ov003_020e3e6c[];
extern char             data_ov003_020e3ea0[];
extern char             data_ov003_020e3ed4[];
extern char             data_ov003_020e3f0c[];
extern char             data_ov003_020e3f10[];
extern char             data_ov003_020e3f18[];
extern char             data_ov003_020e3f38[];
extern char             data_ov003_020e3f58[];
extern char             data_ov003_020e3f78[];
extern char             data_ov003_020e3f98[];
extern char             data_ov003_020e3fb8[];
extern char             data_ov003_020e3fd8[];
extern char             data_ov003_020e3ff8[];
extern char             data_ov003_020e4018[];
extern char             data_ov003_020e4038[];
extern char             data_ov003_020e4058[];
extern char             data_ov003_020e4078[];
extern char             data_ov003_020e4098[];
extern char             data_ov003_020e40b8[];
extern char             data_ov003_020e40d8[];
extern char             data_ov003_020e40f8[];
extern char             data_ov003_020e4118[];
extern char             data_ov003_020e4138[];
extern char             data_ov003_020e4158[];
extern char             data_ov003_020e4178[];
extern char             data_ov003_020e4198[];
extern char             data_ov003_020e41b8[];
extern char             data_ov003_020e41d8[];
extern char             data_ov003_020e41f8[];
extern char             data_ov003_020e4218[];
extern char             data_ov003_020e4238[];
extern char             data_ov003_020e4258[];
extern char             data_ov003_020e4278[];
extern char             data_ov003_020e4298[];
extern char             data_ov003_020e42b8[];
extern char             data_ov003_020e42d8[];
extern char             data_ov003_020e42f8[];
extern char             data_ov003_020e4318[];
extern char             data_ov003_020e4338[];
extern char             data_ov003_020e4358[];
extern char             data_ov003_020e4378[];
extern char             data_ov003_020e4398[];
extern char             data_ov003_020e43b8[];
extern char             data_ov003_020e43d8[];
extern char             data_ov003_020e43f8[];
extern char             data_ov003_020e4418[];
extern char             data_ov003_020e4438[];
extern char             data_ov003_020e4458[];
extern char             data_ov003_020e4478[];
extern char             data_ov003_020e4498[];
extern char             data_ov003_020e44b8[];
extern char             data_ov003_020e44d8[];

// ---------------------------------------------------------------------------------------------
// Ov003Global views
// ---------------------------------------------------------------------------------------------

/// The touch event pool and the actor list, both split out of the middle of `Ov003Global`.
#define OV003_TOUCH_EVENTS (data_ov003_020e71b8->touchEvents)
#define OV003_TOUCH_ACTORS (&data_ov003_020e71b8->unk3D8BC)
#define OV003_TOUCH_SLOTS  (&gSaveData.battleTouchSlots)

/// The per-psych battle flags updates of `func_ov003_020a37a8` touch.
#define OV003_PSYCH_FLAGS data_ov003_020e71b8->unk3D374

/// The touch slots an actor owns: the id of a slot is the word after its 8-byte record header and
/// the amount it contributes is the following word.

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

/// A word of a touch-action resource script: an entry point or one of its in-line operands. The
/// retail symbol table splits these scripts at non-record boundaries, so they stay raw words.
typedef union Ov003ScriptWord {
    u32   value;
    void* ptr;
    s32 (*run)(void);
} Ov003ScriptWord; // Size: 4

/// An eight-byte sprite animation record of the touch resource scripts.
typedef struct Ov003AnimRecord {
    /* 0x0 */ u8  unk_00;
    /* 0x2 */ u16 unk_02;
    /* 0x4 */ s16 unk_04;
} Ov003AnimRecord; // Size: 0x6

/// One record of the per-touch icon table at `data_ov003_020da80c`, indexed by the pack id that
/// `func_ov003_020a3498` receives. Fields 0x04..0x16 feed a `SpriteAnimationEx`.
typedef struct Ov003TouchIconParams {
    /* 0x00 */ const BinIdentifier* binIden;
    /* 0x04 */ u16                  unk_04;
    /* 0x06 */ s16                  unk_06;
    /* 0x08 */ SpriteAnimEntry      entry;
    /* 0x10 */ u16                  unk_10;
    /* 0x12 */ u16                  unk_12;
    /* 0x14 */ u16                  unk_14;
    /* 0x16 */ u16                  unk_16;
} Ov003TouchIconParams; // Size: 0x18

/// One touch target rectangle, in 0x200-based screen units. Same layout as `Ov003CameraPathPos`
/// in `ov003_02083300.c`; both should move into a shared header.
typedef struct Ov003PathRect {
    /* 0x0 */ s16 unk_00;
    /* 0x2 */ s16 unk_02;
    /* 0x4 */ s16 unk_04;
    /* 0x6 */ s16 unk_06;
} Ov003PathRect; // Size: 0x8

/// One entry of a camera path table: the target rectangle, an optional extra table, the sprite
/// parameters and the pack the sprite comes from.
typedef struct __attribute__((packed)) Ov003TouchPathEntry {
    /* 0x00 */ const void* pos;
    /* 0x04 */ const void* anims;
    /* 0x08 */ u8          unk_08[0x10];
    /* 0x18 */ const void* iden;
} Ov003TouchPathEntry; // Size: 0x1C

/// The sub-engine camera path table. Its last record ends with an in-line copy of a bin
/// identifier, so the id word lands in the table and only the path pointer trails it.
typedef struct Ov003SubCameraPathTable {
    /* 0x000 */ Ov003TouchPathEntry entries[40];
    /* 0x460 */ const void*         lastPos;
    /* 0x464 */ const void*         lastAnims;
    /* 0x468 */ u8                  lastData[0x10];
    /* 0x478 */ u32                 lastIdenType;
    /* 0x47C */ const char*         lastIdenPath;
} Ov003SubCameraPathTable; // Size: 0x480

/// The main camera path table is split by the retail symbol table 0x17 bytes into its first
/// record; these two objects are the two halves of the same `Ov003TouchPathEntry[41]` table.
typedef struct __attribute__((packed)) Ov003CameraPathHead {
    /* 0x00 */ const void* pos;
    /* 0x04 */ u8          data[0x13];
} Ov003CameraPathHead; // Size: 0x17

typedef struct __attribute__((packed)) Ov003CameraPathTail {
    /* 0x000 */ u8                   lastData;
    /* 0x001 */ const BinIdentifier* firstIden;
    /* 0x005 */ Ov003TouchPathEntry  entries[40];
} Ov003CameraPathTail; // Size: 0x465

/// A bin identifier with two extra pairs of halfwords (the load parameters of a sprite slot).
typedef struct Ov003BinIdenEx {
    /* 0x0 */ BinIdentifier iden;
    /* 0x8 */ u16           unk_08[4];
} Ov003BinIdenEx; // Size: 0x10

extern const Ov003ScriptWord         data_ov003_020d9a80[4];
extern const Ov003ScriptWord         data_ov003_020d9a90[6];
extern const Ov003ScriptWord         data_ov003_020d9aa8[10];
extern const u8                      data_ov003_020d9ad0[0x2C];
extern const u8                      data_ov003_020d9afc[0x18];
extern const Ov003AnimRecord         data_ov003_020d9b14[1];
extern const Ov003AnimRecord         data_ov003_020d9b1a[1];
extern const Ov003AnimRecord         data_ov003_020d9b20[1];
extern const Ov003AnimRecord         data_ov003_020d9b26[1];
extern const Ov003AnimRecord         data_ov003_020d9b2c[1];
extern const Ov003AnimRecord         data_ov003_020d9b32[1];
extern const Ov003AnimRecord         data_ov003_020d9b38[1];
extern const Ov003AnimRecord         data_ov003_020d9b3e[1];
extern const Ov003AnimRecord         data_ov003_020d9b44[1];
extern const Ov003AnimRecord         data_ov003_020d9b4a[1];
extern const Ov003AnimRecord         data_ov003_020d9b50[1];
extern const Ov003AnimRecord         data_ov003_020d9b56[1];
extern const Ov003AnimRecord         data_ov003_020d9b5c[1];
extern const Ov003AnimRecord         data_ov003_020d9b62[1];
extern const Ov003AnimRecord         data_ov003_020d9b68[1];
extern const Ov003AnimRecord         data_ov003_020d9b6e[1];
extern const Ov003AnimRecord         data_ov003_020d9b74[1];
extern const Ov003AnimRecord         data_ov003_020d9b7a[1];
extern const Ov003AnimRecord         data_ov003_020d9b80[1];
extern const Ov003AnimRecord         data_ov003_020d9b86[1];
extern const Ov003AnimRecord         data_ov003_020d9b8c[1];
extern const Ov003AnimRecord         data_ov003_020d9b92[1];
extern const Ov003AnimRecord         data_ov003_020d9b98[1];
extern const Ov003AnimRecord         data_ov003_020d9b9e[1];
extern const Ov003AnimRecord         data_ov003_020d9ba4[1];
extern const u8                      data_ov003_020d9baa[0xE];
extern const BinIdentifier           data_ov003_020d9bb8;
extern const BinIdentifier           data_ov003_020d9bc0;
extern const BinIdentifier           data_ov003_020d9bc8;
extern const BinIdentifier           data_ov003_020d9bd0;
extern const BinIdentifier           data_ov003_020d9bd8;
extern const BinIdentifier           data_ov003_020d9be0;
extern const BinIdentifier           data_ov003_020d9be8;
extern const BinIdentifier           data_ov003_020d9bf0;
extern const BinIdentifier           data_ov003_020d9bf8;
extern const BinIdentifier           data_ov003_020d9c00;
extern const BinIdentifier           data_ov003_020d9c08;
extern const BinIdentifier           data_ov003_020d9c10;
extern const BinIdentifier           data_ov003_020d9c18;
extern const BinIdentifier           data_ov003_020d9c20;
extern const BinIdentifier           data_ov003_020d9c28;
extern const BinIdentifier           data_ov003_020d9c30;
extern const BinIdentifier           data_ov003_020d9c38;
extern const BinIdentifier           data_ov003_020d9c40;
extern const BinIdentifier           data_ov003_020d9c48;
extern const BinIdentifier           data_ov003_020d9c50;
extern const BinIdentifier           data_ov003_020d9c58;
extern const BinIdentifier           data_ov003_020d9c60;
extern const BinIdentifier           data_ov003_020d9c68;
extern const BinIdentifier           data_ov003_020d9c70;
extern const BinIdentifier           data_ov003_020d9c78;
extern const BinIdentifier           data_ov003_020d9c80[2];
extern const void* const             data_ov003_020d9c90[1];
extern const Ov003SubCameraPathTable data_ov003_020d9c94;
extern const BinIdentifier           data_ov003_020da114;
extern const BinIdentifier           data_ov003_020da11c;
extern const BinIdentifier           data_ov003_020da124;
extern const BinIdentifier           data_ov003_020da12c;
extern const BinIdentifier           data_ov003_020da134;
extern const BinIdentifier           data_ov003_020da13c;
extern const BinIdentifier           data_ov003_020da144;
extern const BinIdentifier           data_ov003_020da14c;
extern const BinIdentifier           data_ov003_020da154;
extern const BinIdentifier           data_ov003_020da15c;
extern const BinIdentifier           data_ov003_020da164;
extern const BinIdentifier           data_ov003_020da16c;
extern const BinIdentifier           data_ov003_020da174;
extern const BinIdentifier           data_ov003_020da17c;
extern const BinIdentifier           data_ov003_020da184;
extern const BinIdentifier           data_ov003_020da18c;
extern const BinIdentifier           data_ov003_020da194;
extern const BinIdentifier           data_ov003_020da19c;
extern const BinIdentifier           data_ov003_020da1a4;
extern const BinIdentifier           data_ov003_020da1ac;
extern const BinIdentifier           data_ov003_020da1b4;
extern const BinIdentifier           data_ov003_020da1bc;
extern const BinIdentifier           data_ov003_020da1c4;
extern const BinIdentifier           data_ov003_020da1cc;
extern const BinIdentifier           data_ov003_020da1d4;
extern const Ov003BinIdenEx          data_ov003_020da1dc;
extern const Ov003PathRect           data_ov003_020da1ec;
extern const Ov003PathRect           data_ov003_020da1f4;
extern const Ov003PathRect           data_ov003_020da1fc;
extern const Ov003PathRect           data_ov003_020da204;
extern const Ov003PathRect           data_ov003_020da20c;
extern const Ov003PathRect           data_ov003_020da214;
extern const Ov003PathRect           data_ov003_020da21c;
extern const Ov003PathRect           data_ov003_020da224;
extern const Ov003PathRect           data_ov003_020da22c;
extern const Ov003PathRect           data_ov003_020da234;
extern const Ov003PathRect           data_ov003_020da23c;
extern const Ov003PathRect           data_ov003_020da244;
extern const Ov003PathRect           data_ov003_020da24c;
extern const Ov003PathRect           data_ov003_020da254;
extern const Ov003PathRect           data_ov003_020da25c;
extern const Ov003PathRect           data_ov003_020da264;
extern const Ov003PathRect           data_ov003_020da26c;
extern const Ov003PathRect           data_ov003_020da274;
extern const Ov003PathRect           data_ov003_020da27c;
extern const Ov003PathRect           data_ov003_020da284;
extern const Ov003PathRect           data_ov003_020da28c;
extern const Ov003PathRect           data_ov003_020da294;
extern const Ov003PathRect           data_ov003_020da29c;
extern const Ov003PathRect           data_ov003_020da2a4;
extern const Ov003PathRect           data_ov003_020da2ac;
extern const Ov003PathRect           data_ov003_020da2b4;
extern const void* const             data_ov003_020da2bc[1];
extern const Ov003TouchPathEntry     data_ov003_020da2c0[41];
extern;
extern const BinIdentifier        data_ov003_020da73c;
extern const BinIdentifier        data_ov003_020da744;
extern const BinIdentifier        data_ov003_020da74c;
extern const BinIdentifier        data_ov003_020da754;
extern const BinIdentifier        data_ov003_020da75c;
extern const BinIdentifier        data_ov003_020da764;
extern const BinIdentifier        data_ov003_020da76c;
extern const BinIdentifier        data_ov003_020da774;
extern const BinIdentifier        data_ov003_020da77c;
extern const BinIdentifier        data_ov003_020da784;
extern const BinIdentifier        data_ov003_020da78c;
extern const BinIdentifier        data_ov003_020da794;
extern const BinIdentifier        data_ov003_020da79c;
extern const BinIdentifier        data_ov003_020da7a4;
extern const BinIdentifier        data_ov003_020da7ac;
extern const BinIdentifier        data_ov003_020da7b4;
extern const BinIdentifier        data_ov003_020da7bc;
extern const BinIdentifier        data_ov003_020da7c4;
extern const BinIdentifier        data_ov003_020da7cc;
extern const BinIdentifier        data_ov003_020da7d4;
extern const BinIdentifier        data_ov003_020da7dc;
extern const BinIdentifier        data_ov003_020da7e4;
extern const BinIdentifier        data_ov003_020da7ec;
extern const BinIdentifier        data_ov003_020da7f4;
extern const BinIdentifier        data_ov003_020da7fc;
extern const BinIdentifier        data_ov003_020da804;
extern const Ov003TouchIconParams data_ov003_020da80c[258];
extern char                       data_ov003_020e44f8[0x18];
extern char                       data_ov003_020e4510[0x18];
extern char                       data_ov003_020e4528[0x1C];
extern char                       data_ov003_020e4544[0x1C];
extern char                       data_ov003_020e4560[0x1C];
extern char                       data_ov003_020e457c[0x1C];
extern char                       data_ov003_020e4598[0x20];
extern char                       data_ov003_020e45b8[0x20];
extern char                       data_ov003_020e45d8[0x20];
extern char                       data_ov003_020e45f8[0x24];
extern char                       data_ov003_020e461c[0x24];
extern char                       data_ov003_020e4640[0x24];
extern char                       data_ov003_020e4664[0x24];
extern char                       data_ov003_020e4688[0x24];
extern char                       data_ov003_020e46ac[0x24];
extern char                       data_ov003_020e46d0[0x24];
extern char                       data_ov003_020e46f4[0x24];
extern char                       data_ov003_020e4718[0x24];
extern char                       data_ov003_020e473c[0x24];
extern char                       data_ov003_020e4760[0x24];
extern char                       data_ov003_020e4784[0x24];
extern char                       data_ov003_020e47a8[0x24];
extern char                       data_ov003_020e47cc[0x24];
extern char                       data_ov003_020e47f0[0x28];
extern char                       data_ov003_020e4818[0x28];
extern char                       data_ov003_020e4840[0x28];
char                              data_ov003_020e44f8[0x18] = "Data/AbilityData.bin";

char data_ov003_020e4510[0x18] = "Apl_Tak/ItemData.bin";

char data_ov003_020e4528[0x1C] = "Apl_Hir/Grp_BtlCircle.bin";

char data_ov003_020e4544[0x1C] = "Apl_Hir/Grp_BtlPsy_Hold.bin";

char data_ov003_020e4560[0x1C] = "Apl_Hir/Grp_BtlPsy_Lime.bin";

char data_ov003_020e457c[0x1C] = "Apl_Hir/Grp_BtlPsy_Bomb.bin";

char data_ov003_020e4598[0x20] = "Apl_Hir/Grp_BtlPsy_Element.bin";

char data_ov003_020e45b8[0x20] = "Apl_Hir/Grp_BtlPsy_Recover.bin";

char data_ov003_020e45d8[0x20] = "Apl_Hir/Grp_BtlPsy_Barrier.bin";

char data_ov003_020e45f8[0x24] = "Apl_Hir/Grp_BtlPsy_BlackHoll.bin";

char data_ov003_020e461c[0x24] = "Apl_Hir/Grp_BtlPsy_ShockWave.bin";

char data_ov003_020e4640[0x24] = "Apl_Hir/Grp_BtlPsy_Territory.bin";

char data_ov003_020e4664[0x24] = "Apl_Hir/Grp_BtlPsy_IcePillar.bin";

char data_ov003_020e4688[0x24] = "Apl_Hir/Grp_BtlPsy_FieldShake.bin";

char data_ov003_020e46ac[0x24] = "Apl_Hir/Grp_BtlPsy_DragAttack.bin";

char data_ov003_020e46d0[0x24] = "Apl_Hir/Grp_BtlPsy_StreetLive.bin";

char data_ov003_020e46f4[0x24] = "Apl_Hir/Grp_BtlPsy_MeteorFall.bin";

char data_ov003_020e4718[0x24] = "Apl_Hir/Grp_BtlPsy_ChargeLaser.bin";

char data_ov003_020e473c[0x24] = "Apl_Hir/Grp_BtlPsy_AttackPaint.bin";

char data_ov003_020e4760[0x24] = "Apl_Hir/Grp_BtlPsy_ElectricGen.bin";

char data_ov003_020e4784[0x24] = "Apl_Hir/Grp_BtlPsy_ElementBind.bin";

char data_ov003_020e47a8[0x24] = "Apl_Hir/Grp_BtlPsy_SlashThunder.bin";

char data_ov003_020e47cc[0x24] = "Apl_Hir/Grp_BtlPsy_AbsorbEffect.bin";

char data_ov003_020e47f0[0x28] = "Apl_Hir/Grp_BtlPsy_Psychokinesis.bin";

char data_ov003_020e4818[0x28] = "Apl_Hir/Grp_BtlPsy_LockOnThunder.bin";

char                  data_ov003_020e4840[0x28] = "Apl_Hir/Grp_BtlPsy_BadgeFormation.bin";
const Ov003ScriptWord data_ov003_020d9a80[4]    = {
    {.value = (u32)func_ov003_02083478},
    {.value = 0x00030001},
    {.value = 0x000D0002},
    {.value = (u32)func_ov003_02083470},
};

const Ov003ScriptWord data_ov003_020d9a90[6] = {
    {.value = (u32)func_ov003_02083700},
    {.value = (u32)data_ov003_020d9ad0},
    {.value = 0x00000002},
    {.value = 0x03000000},
    {.value = 0x000D0001},
    {.value = (u32)func_ov003_02083470},
};

const Ov003ScriptWord data_ov003_020d9aa8[10] = {
    {.value = (u32)func_ov003_02083478}, {.value = 0x00040003}, {.value = 0x000D0002},
    {.value = (u32)func_ov003_020835d4}, {.value = 0x00040001}, {.value = 0x03000001},
    {.value = (u32)func_ov003_020835d4}, {.value = 0x00040002}, {.value = 0x03000002},
    {.value = (u32)func_ov003_02083470},
};

const u8 data_ov003_020d9ad0[0x2C] = {
    0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01,
    0x00, 0x40, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
};

const u8 data_ov003_020d9afc[0x18] = {
    0x05, 0x00, 0xC0, 0x3A, 0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0xC0, 0x15,
    0x03, 0x00, 0x04, 0x00, 0x04, 0x00, 0xC0, 0x0C, 0x03, 0x00, 0x04, 0x00,
};

const Ov003AnimRecord data_ov003_020d9b14[1] = {
    {8, 0x0036, -40},
};

const Ov003AnimRecord data_ov003_020d9b1a[1] = {
    {8, 0x0036, -40},
};

const Ov003AnimRecord data_ov003_020d9b20[1] = {
    {8, 0x003A, -40},
};

const Ov003AnimRecord data_ov003_020d9b26[1] = {
    {8, 0x0036, -40},
};

const Ov003AnimRecord data_ov003_020d9b2c[1] = {
    {8, 0x0036, -40},
};

const Ov003AnimRecord data_ov003_020d9b32[1] = {
    {8, 0x0048, -40},
};

const Ov003AnimRecord data_ov003_020d9b38[1] = {
    {8, 0x003A, -40},
};

const Ov003AnimRecord data_ov003_020d9b3e[1] = {
    {8, 0x0038, -40},
};

const Ov003AnimRecord data_ov003_020d9b44[1] = {
    {8, 0x003C, -40},
};

const Ov003AnimRecord data_ov003_020d9b4a[1] = {
    {8, 0x0038, -40},
};

const Ov003AnimRecord data_ov003_020d9b50[1] = {
    {8, 0x0030, -40},
};

const Ov003AnimRecord data_ov003_020d9b56[1] = {
    {8, 0x0030, -40},
};

const Ov003AnimRecord data_ov003_020d9b5c[1] = {
    {8, 0x0030, -40},
};

const Ov003AnimRecord data_ov003_020d9b62[1] = {
    {8, 0x0038, -40},
};

const Ov003AnimRecord data_ov003_020d9b68[1] = {
    {8, 0x0038, -40},
};

const Ov003AnimRecord data_ov003_020d9b6e[1] = {
    {61, 0x0033, 0},
};

const Ov003AnimRecord data_ov003_020d9b74[1] = {
    {8, 0x0034, -40},
};

const Ov003AnimRecord data_ov003_020d9b7a[1] = {
    {8, 0x0040, -40},
};

const Ov003AnimRecord data_ov003_020d9b80[1] = {
    {8, 0x0038, -40},
};

const Ov003AnimRecord data_ov003_020d9b86[1] = {
    {8, 0x0036, -40},
};

const Ov003AnimRecord data_ov003_020d9b8c[1] = {
    {8, 0x0034, -40},
};

const Ov003AnimRecord data_ov003_020d9b92[1] = {
    {8, 0x0028, -40},
};

const Ov003AnimRecord data_ov003_020d9b98[1] = {
    {8, 0x0030, -40},
};

const Ov003AnimRecord data_ov003_020d9b9e[1] = {
    {8, 0x0038, -40},
};

const Ov003AnimRecord data_ov003_020d9ba4[1] = {
    {8, 0x0028, -40},
};

const u8 data_ov003_020d9baa[0xE] = {
    0x08, 0x00, 0x28, 0x00, 0xD8, 0xFF, 0x08, 0x00, 0x38, 0x00, 0xD8, 0xFF, 0x00, 0x00,
};

const BinIdentifier data_ov003_020d9bb8 = {3, (char*)&data_ov003_020e40b8};

const BinIdentifier data_ov003_020d9bc0 = {3, (char*)&data_ov003_020e40b8};

const BinIdentifier data_ov003_020d9bc8 = {3, (char*)&data_ov003_020e3f18};

const BinIdentifier data_ov003_020d9bd0 = {3, (char*)&data_ov003_020e41d8};

const BinIdentifier data_ov003_020d9bd8 = {3, (char*)&data_ov003_020e41b8};

const BinIdentifier data_ov003_020d9be0 = {3, (char*)&data_ov003_020e4198};

const BinIdentifier data_ov003_020d9be8 = {3, (char*)&data_ov003_020e4178};

const BinIdentifier data_ov003_020d9bf0 = {3, (char*)&data_ov003_020e4158};

const BinIdentifier data_ov003_020d9bf8 = {3, (char*)&data_ov003_020e4138};

const BinIdentifier data_ov003_020d9c00 = {3, (char*)&data_ov003_020e4118};

const BinIdentifier data_ov003_020d9c08 = {3, (char*)&data_ov003_020e40f8};

const BinIdentifier data_ov003_020d9c10 = {3, (char*)&data_ov003_020e40f8};

const BinIdentifier data_ov003_020d9c18 = {3, (char*)&data_ov003_020e40d8};

const BinIdentifier data_ov003_020d9c20 = {3, (char*)&data_ov003_020e3f38};

const BinIdentifier data_ov003_020d9c28 = {3, (char*)&data_ov003_020e4098};

const BinIdentifier data_ov003_020d9c30 = {3, (char*)&data_ov003_020e4078};

const BinIdentifier data_ov003_020d9c38 = {3, (char*)&data_ov003_020e4058};

const BinIdentifier data_ov003_020d9c40 = {3, (char*)&data_ov003_020e4038};

const BinIdentifier data_ov003_020d9c48 = {3, (char*)&data_ov003_020e4018};

const BinIdentifier data_ov003_020d9c50 = {3, (char*)&data_ov003_020e3ff8};

const BinIdentifier data_ov003_020d9c58 = {3, (char*)&data_ov003_020e3fd8};

const BinIdentifier data_ov003_020d9c60 = {3, (char*)&data_ov003_020e3fb8};

const BinIdentifier data_ov003_020d9c68 = {3, (char*)&data_ov003_020e3f98};

const BinIdentifier data_ov003_020d9c70 = {3, (char*)&data_ov003_020e3f78};

const BinIdentifier data_ov003_020d9c78 = {3, (char*)&data_ov003_020e3f58};

const BinIdentifier data_ov003_020d9c80[2] = {
    {3, (char*)&data_ov003_020e3f58},
    {3, (char*)&data_ov003_020e4018},
};

const void* const data_ov003_020d9c90[1] = {(const void*)&data_ov003_020d9bb8};

const Ov003SubCameraPathTable data_ov003_020d9c94 = {
    {
     {&data_ov003_020d9b14,
         &data_ov003_020e3ea0,
         {0x01, 0x02, 0x03, 0x0B, 0x01, 0x02, 0x80, 0x6D, 0x80, 0x68, 0x01, 0x02, 0x0D, 0xFF, 0x00, 0x00},
         &data_ov003_020d9bc0},
     {&data_ov003_020d9b1a,
         &data_ov003_020e3ea0,
         {0x01, 0x02, 0x03, 0x0B, 0x01, 0x02, 0x80, 0x6D, 0x80, 0x68, 0x01, 0x02, 0x0D, 0xFF, 0x00, 0x00},
         &data_ov003_020d9bc8},
     {&data_ov003_020d9b20,
         &data_ov003_020e3ed4,
         {0x01, 0x02, 0x03, 0x0B, 0x01, 0x02, 0xC0, 0x75, 0x00, 0x6C, 0x01, 0x02, 0x0E, 0xFF, 0x00, 0x00},
         &data_ov003_020d9bd0},
     {&data_ov003_020d9b26,
         &data_ov003_020e3e6c,
         {0x01, 0x02, 0x03, 0x0A, 0x01, 0x02, 0x00, 0x60, 0x00, 0x68, 0x01, 0x02, 0x0D, 0xFF, 0x00, 0x00},
         &data_ov003_020d9bd8},
     {&data_ov003_020d9b2c,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x7C, 0xC0, 0x7A, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00},
         &data_ov003_020d9be0},
     {&data_ov003_020d9b32,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x80, 0x75, 0xC0, 0x75, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9be8},
     {&data_ov003_020d9b38,
         &data_ov003_020e3e68,
         {0x01, 0x02, 0x03, 0x0C, 0x01, 0x02, 0x40, 0x62, 0x40, 0x62, 0x01, 0x02, 0x01, 0xFF, 0x00, 0x00},
         &data_ov003_020d9bf0},
     {&data_ov003_020d9b3e,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x80, 0x77, 0x80, 0x74, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00},
         &data_ov003_020d9bf8},
     {&data_ov003_020d9b44,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x80, 0x68, 0x40, 0x6D, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c00},
     {&data_ov003_020d9b4a,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x68, 0x40, 0x76, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c08},
     {&data_ov003_020d9b50,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x78, 0x00, 0x76, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c10},
     {&data_ov003_020d9b56,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x78, 0x00, 0x76, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c18},
     {&data_ov003_020d9b5c,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x5F, 0xC0, 0x7F, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c20},
     {&data_ov003_020d9b62,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x74, 0x40, 0x55, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c28},
     {&data_ov003_020d9b68,
         &data_ov003_020e3e64,
         {0x01, 0x02, 0x03, 0x0C, 0x01, 0x02, 0x80, 0x6B, 0x40, 0x60, 0x01, 0x02, 0x01, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c30},
     {&data_ov003_020d9b6e,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x73, 0x40, 0x6F, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c38},
     {&data_ov003_020d9b74,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x6C, 0x80, 0x5B, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c40},
     {&data_ov003_020d9b7a,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c48},
     {&data_ov003_020d9b80,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x7E, 0x40, 0x7C, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c50},
     {&data_ov003_020d9b86,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x6D, 0xC0, 0x65, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c58},
     {&data_ov003_020d9b8c,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0xC0, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00},
         &data_ov003_020d9c60},
     {&data_ov003_020d9b92,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0xC0, 0x5D, 0x40, 0x67, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c68},
     {&data_ov003_020d9b98,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x7C, 0xC0, 0x62, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c70},
     {&data_ov003_020d9b9e,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x7A, 0x00, 0x80, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c78},
     {&data_ov003_020d9ba4,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x45, 0x00, 0x2F, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c80},
     {&data_ov003_020d9baa,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x45, 0x00, 0x2F, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c48},
     {&data_ov003_020d9b80,
         NULL,
         {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x7E, 0x40, 0x7C, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9be8},
     {&data_ov003_020d9b38,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x40, 0x62, 0x40, 0x62, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c20},
     {&data_ov003_020d9b62,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x40, 0x74, 0x40, 0x55, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c40},
     {&data_ov003_020d9b7a,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9bc8},
     {&data_ov003_020d9b20,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0xC0, 0x75, 0x00, 0x6C, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c28},
     {&data_ov003_020d9b68,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x80, 0x6B, 0x40, 0x60, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c08},
     {&data_ov003_020d9b50,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x78, 0x00, 0x76, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c18},
     {&data_ov003_020d9b5c,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x5F, 0xC0, 0x7F, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c00},
     {&data_ov003_020d9b4a,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x68, 0x40, 0x76, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c70},
     {&data_ov003_020d9b9e,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x40, 0x7A, 0x00, 0x80, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9bd8},
     {&data_ov003_020d9b2c,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x7C, 0xC0, 0x7A, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00},
         &data_ov003_020d9c68},
     {&data_ov003_020d9b98,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x40, 0x7C, 0xC0, 0x62, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9be0},
     {&data_ov003_020d9b32,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x80, 0x75, 0xC0, 0x75, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9bc0},
     {&data_ov003_020d9b1a,
         NULL,
         {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x80, 0x6D, 0x80, 0x68, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
         &data_ov003_020d9c60},
     },
    &data_ov003_020d9b92,
    NULL,
    {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0xC0, 0x5D, 0x40, 0x67, 0x01, 0x02, 0x00, 0xFF, 0x00, 0x00},
    0x3,
    (const char*)&data_ov003_020e41f8,
};

const BinIdentifier data_ov003_020da114 = {3, (char*)&data_ov003_020e4218};

const BinIdentifier data_ov003_020da11c = {3, (char*)&data_ov003_020e44d8};

const BinIdentifier data_ov003_020da124 = {3, (char*)&data_ov003_020e44b8};

const BinIdentifier data_ov003_020da12c = {3, (char*)&data_ov003_020e4498};

const BinIdentifier data_ov003_020da134 = {3, (char*)&data_ov003_020e41f8};

const BinIdentifier data_ov003_020da13c = {3, (char*)&data_ov003_020e4478};

const BinIdentifier data_ov003_020da144 = {3, (char*)&data_ov003_020e4458};

const BinIdentifier data_ov003_020da14c = {3, (char*)&data_ov003_020e4438};

const BinIdentifier data_ov003_020da154 = {3, (char*)&data_ov003_020e4418};

const BinIdentifier data_ov003_020da15c = {3, (char*)&data_ov003_020e43f8};

const BinIdentifier data_ov003_020da164 = {3, (char*)&data_ov003_020e43f8};

const BinIdentifier data_ov003_020da16c = {3, (char*)&data_ov003_020e43d8};

const BinIdentifier data_ov003_020da174 = {3, (char*)&data_ov003_020e43b8};

const BinIdentifier data_ov003_020da17c = {3, (char*)&data_ov003_020e4398};

const BinIdentifier data_ov003_020da184 = {3, (char*)&data_ov003_020e4378};

const BinIdentifier data_ov003_020da18c = {3, (char*)&data_ov003_020e4358};

const BinIdentifier data_ov003_020da194 = {3, (char*)&data_ov003_020e4338};

const BinIdentifier data_ov003_020da19c = {3, (char*)&data_ov003_020e4318};

const BinIdentifier data_ov003_020da1a4 = {3, (char*)&data_ov003_020e42f8};

const BinIdentifier data_ov003_020da1ac = {3, (char*)&data_ov003_020e42d8};

const BinIdentifier data_ov003_020da1b4 = {3, (char*)&data_ov003_020e42b8};

const BinIdentifier data_ov003_020da1bc = {3, (char*)&data_ov003_020e4298};

const BinIdentifier data_ov003_020da1c4 = {3, (char*)&data_ov003_020e4278};

const BinIdentifier data_ov003_020da1cc = {3, (char*)&data_ov003_020e4258};

const BinIdentifier data_ov003_020da1d4 = {3, (char*)&data_ov003_020e4258};

const Ov003BinIdenEx data_ov003_020da1dc = {
    {3, (char*)&data_ov003_020e4238},
    {0x0180, 0x0070, 0x0070, 0x0180}
};

const Ov003PathRect data_ov003_020da1ec = {512, 192, 192, 384};

const Ov003PathRect data_ov003_020da1f4 = {384, 80, 80, 272};

const Ov003PathRect data_ov003_020da1fc = {384, 128, 128, 320};

const Ov003PathRect data_ov003_020da204 = {304, 80, 80, 272};

const Ov003PathRect data_ov003_020da20c = {384, 128, 128, 384};

const Ov003PathRect data_ov003_020da214 = {384, 128, 128, 320};

const Ov003PathRect data_ov003_020da21c = {464, 80, 80, 272};

const Ov003PathRect data_ov003_020da224 = {384, 128, 128, 320};

const Ov003PathRect data_ov003_020da22c = {464, 144, 144, 320};

const Ov003PathRect data_ov003_020da234 = {304, 88, 120, 320};

const Ov003PathRect data_ov003_020da23c = {304, 88, 120, 320};

const Ov003PathRect data_ov003_020da244 = {384, 128, 128, 320};

const Ov003PathRect data_ov003_020da24c = {384, 160, 160, 352};

const Ov003PathRect data_ov003_020da254 = {384, 128, 128, 320};

const Ov003PathRect data_ov003_020da25c = {384, 96, 144, 320};

const Ov003PathRect data_ov003_020da264 = {384, 80, 80, 272};

const Ov003PathRect data_ov003_020da26c = {464, 80, 80, 288};

const Ov003PathRect data_ov003_020da274 = {384, 128, 128, 336};

const Ov003PathRect data_ov003_020da27c = {464, 80, 80, 272};

const Ov003PathRect data_ov003_020da284 = {464, 128, 80, 320};

const Ov003PathRect data_ov003_020da28c = {384, 156, 156, 352};

const Ov003PathRect data_ov003_020da294 = {304, 128, 128, 296};

const Ov003PathRect data_ov003_020da29c = {464, 128, 128, 336};

const Ov003PathRect data_ov003_020da2a4 = {384, 128, 128, 352};

const Ov003PathRect data_ov003_020da2ac = {384, 128, 128, 352};

const Ov003PathRect data_ov003_020da2b4 = {384, 128, 128, 336};

const void* const data_ov003_020da2bc[1] = {(const void*)&data_ov003_020da134};

const Ov003TouchPathEntry data_ov003_020da2c0[41] = {
    {&data_ov003_020da20c,
     NULL,{0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x59, 0xC0, 0x58, 0x01, 0x02, 0x03, 0x04, 0x00, 0x04},
     &data_ov003_020da114},
    {&data_ov003_020da1ec,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x7C, 0x00, 0x80, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00},
     &data_ov003_020da11c},
    {&data_ov003_020da1f4,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x54, 0x60, 0x49, 0x01, 0x02, 0x03, 0x04, 0x00, 0x01},
     &data_ov003_020da124},
    {&data_ov003_020da1fc,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x61, 0x40, 0x5B, 0x01, 0x02, 0x03, 0x04, 0x00, 0x02},
     &data_ov003_020da12c},
    {&data_ov003_020da204,
     &data_ov003_020e3f10,
     {0x01, 0x02, 0x03, 0x0D, 0x01, 0x02, 0xC0, 0x62, 0xC0, 0x62, 0x01, 0x02, 0x03, 0x04, 0x02, 0x03},
     &data_ov003_020da134},
    {&data_ov003_020da20c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x59, 0xC0, 0x58, 0x01, 0x02, 0x03, 0x04, 0x00, 0x04},
     &data_ov003_020da13c},
    {&data_ov003_020da214,
     &data_ov003_020e3f0c,
     {0x01, 0x02, 0x03, 0x0D, 0x01, 0x02, 0x00, 0x54, 0x00, 0x60, 0x01, 0x02, 0x03, 0x04, 0x01, 0x05},
     &data_ov003_020da144},
    {&data_ov003_020da21c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x60, 0x6F, 0x20, 0x70, 0x01, 0x02, 0x03, 0x04, 0x00, 0x06},
     &data_ov003_020da14c},
    {&data_ov003_020da224,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0xA0, 0x5E, 0x00, 0x58, 0x01, 0x02, 0x03, 0x04, 0x00, 0x07},
     &data_ov003_020da154},
    {&data_ov003_020da22c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x76, 0xA0, 0x7F, 0x01, 0x02, 0x03, 0x04, 0x00, 0x08},
     &data_ov003_020da15c},
    {&data_ov003_020da234,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x80, 0x5E, 0x60, 0x5C, 0x01, 0x02, 0x03, 0x04, 0x00, 0x09},
     &data_ov003_020da164},
    {&data_ov003_020da23c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x80, 0x5E, 0x60, 0x5C, 0x01, 0x02, 0x03, 0x04, 0x00, 0x09},
     &data_ov003_020da16c},
    {&data_ov003_020da244,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x20, 0x4A, 0xC0, 0x4B, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0A},
     &data_ov003_020da174},
    {&data_ov003_020da24c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x20, 0x5C, 0xE0, 0x64, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0B},
     &data_ov003_020da17c},
    {&data_ov003_020da254,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x68, 0xE0, 0x66, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0C},
     &data_ov003_020da184},
    {&data_ov003_020da25c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x80, 0x73, 0x40, 0x77, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0D},
     &data_ov003_020da18c},
    {&data_ov003_020da264,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x20, 0x5A, 0xA0, 0x57, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0E},
     &data_ov003_020da194},
    {&data_ov003_020da26c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x6E, 0x20, 0x6D, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0F},
     &data_ov003_020da19c},
    {&data_ov003_020da274,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x62, 0xA0, 0x62, 0x01, 0x02, 0x03, 0x04, 0x00, 0x10},
     &data_ov003_020da1a4},
    {&data_ov003_020da27c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x00, 0x71, 0xE0, 0x6E, 0x01, 0x02, 0x03, 0x04, 0x00, 0x11},
     &data_ov003_020da1ac},
    {&data_ov003_020da284,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x80, 0x73, 0xA0, 0x7F, 0x01, 0x02, 0x03, 0x04, 0x00, 0x12},
     &data_ov003_020da1b4},
    {&data_ov003_020da28c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x78, 0x60, 0x77, 0x01, 0x02, 0x03, 0x04, 0x00, 0x13},
     &data_ov003_020da1bc},
    {&data_ov003_020da294,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x60, 0x39, 0x40, 0x48, 0x01, 0x02, 0x03, 0x04, 0x00, 0x14},
     &data_ov003_020da1c4},
    {&data_ov003_020da29c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x60, 0x74, 0xA0, 0x7E, 0x01, 0x02, 0x03, 0x04, 0x00, 0x15},
     &data_ov003_020da1cc},
    {&data_ov003_020da2a4,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0xE0, 0x5C, 0xC0, 0x57, 0x01, 0x02, 0x03, 0x04, 0x00, 0x16},
     &data_ov003_020da1d4},
    {&data_ov003_020da2ac,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0xE0, 0x5C, 0xC0, 0x57, 0x01, 0x02, 0x03, 0x04, 0x00, 0x16},
     &data_ov003_020da1dc},
    {&data_ov003_020da2b4,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0xE0, 0x61, 0x60, 0x5F, 0x01, 0x02, 0x03, 0x04, 0x00, 0x10},
     &data_ov003_020da13c},
    {&data_ov003_020da214,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x54, 0x00, 0x60, 0x01, 0x02, 0x03, 0x04, 0x00, 0x05},
     &data_ov003_020da174},
    {&data_ov003_020da24c,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x20, 0x5C, 0xE0, 0x64, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0B},
     &data_ov003_020da194},
    {&data_ov003_020da26c,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x6E, 0x20, 0x6D, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0F},
     &data_ov003_020da11c},
    {&data_ov003_020da1f4,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x54, 0x60, 0x49, 0x01, 0x02, 0x03, 0x04, 0x00, 0x01},
     &data_ov003_020da17c},
    {&data_ov003_020da254,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x40, 0x68, 0xE0, 0x66, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0C},
     &data_ov003_020da15c},
    {&data_ov003_020da234,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x80, 0x5E, 0x60, 0x5C, 0x01, 0x02, 0x03, 0x04, 0x00, 0x09},
     &data_ov003_020da16c},
    {&data_ov003_020da244,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x20, 0x4A, 0xC0, 0x4B, 0x01, 0x02, 0x03, 0x04, 0x00, 0x0A},
     &data_ov003_020da154},
    {&data_ov003_020da22c,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x76, 0xA0, 0x7F, 0x01, 0x02, 0x03, 0x04, 0x00, 0x08},
     &data_ov003_020da1c4},
    {&data_ov003_020da29c,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x60, 0x74, 0xA0, 0x7E, 0x01, 0x02, 0x03, 0x04, 0x00, 0x15},
     &data_ov003_020da12c},
    {&data_ov003_020da204,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0xC0, 0x62, 0xC0, 0x62, 0x01, 0x02, 0x03, 0x04, 0x00, 0x03},
     &data_ov003_020da1bc},
    {&data_ov003_020da294,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x60, 0x39, 0x40, 0x48, 0x01, 0x02, 0x03, 0x04, 0x00, 0x14},
     &data_ov003_020da134},
    {&data_ov003_020da20c,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x59, 0xC0, 0x58, 0x01, 0x02, 0x03, 0x04, 0x00, 0x04},
     &data_ov003_020da114},
    {&data_ov003_020da1ec,
     NULL, {0x01, 0x02, 0x04, 0x0F, 0x01, 0x02, 0x00, 0x7C, 0x00, 0x80, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00},
     &data_ov003_020da1b4},
    {&data_ov003_020da28c,
     NULL, {0x01, 0x02, 0x03, 0x0F, 0x01, 0x02, 0x40, 0x78, 0x60, 0x77, 0x01, 0x02, 0x03, 0x04, 0x00, 0x13},
     (const void*)0x4    },
};

const BinIdentifier data_ov003_020da73c = {3, (char*)&data_ov003_020e44f8};

const BinIdentifier data_ov003_020da744 = {3, (char*)&data_ov003_020e4510};

const BinIdentifier data_ov003_020da74c = {3, (char*)&data_ov003_020e46ac};

const BinIdentifier data_ov003_020da754 = {3, (char*)&data_ov003_020e4818};

const BinIdentifier data_ov003_020da75c = {3, (char*)&data_ov003_020e45f8};

const BinIdentifier data_ov003_020da764 = {3, (char*)&data_ov003_020e4688};

const BinIdentifier data_ov003_020da76c = {3, (char*)&data_ov003_020e4598};

const BinIdentifier data_ov003_020da774 = {3, (char*)&data_ov003_020e4784};

const BinIdentifier data_ov003_020da77c = {3, (char*)&data_ov003_020e4664};

const BinIdentifier data_ov003_020da784 = {3, (char*)&data_ov003_020e4840};

const BinIdentifier data_ov003_020da78c = {3, (char*)&data_ov003_020e4640};

const BinIdentifier data_ov003_020da794 = {3, (char*)&data_ov003_020e457c};

const BinIdentifier data_ov003_020da79c = {3, (char*)&data_ov003_020e47f0};

const BinIdentifier data_ov003_020da7a4 = {3, (char*)&data_ov003_020e47cc};

const BinIdentifier data_ov003_020da7ac = {3, (char*)&data_ov003_020e4760};

const BinIdentifier data_ov003_020da7b4 = {3, (char*)&data_ov003_020e4528};

const BinIdentifier data_ov003_020da7bc = {3, (char*)&data_ov003_020e461c};

const BinIdentifier data_ov003_020da7c4 = {3, (char*)&data_ov003_020e45b8};

const BinIdentifier data_ov003_020da7cc = {3, (char*)&data_ov003_020e4560};

const BinIdentifier data_ov003_020da7d4 = {3, (char*)&data_ov003_020e45d8};

const BinIdentifier data_ov003_020da7dc = {3, (char*)&data_ov003_020e46d0};

const BinIdentifier data_ov003_020da7e4 = {3, (char*)&data_ov003_020e47a8};

const BinIdentifier data_ov003_020da7ec = {3, (char*)&data_ov003_020e46f4};

const BinIdentifier data_ov003_020da7f4 = {3, (char*)&data_ov003_020e473c};

const BinIdentifier data_ov003_020da7fc = {3, (char*)&data_ov003_020e4544};

const BinIdentifier data_ov003_020da804 = {3, (char*)&data_ov003_020e4718};

const Ov003TouchIconParams data_ov003_020da80c[258] = {
    {&data_ov003_020da754, 3,  1,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da754, 3,  1,    {1, 3, 2, 0}, 0,  5, 128, 0},
    {&data_ov003_020da754, 3,  1,    {1, 3, 2, 0}, 0,  6, 128, 0},
    {&data_ov003_020da754, 3,  1,    {1, 3, 2, 0}, 0,  7, 128, 0},
    {&data_ov003_020da754, 3,  2,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da754, 3,  2,    {1, 3, 2, 0}, 0,  5, 128, 0},
    {&data_ov003_020da754, 3,  2,    {1, 3, 2, 0}, 0,  6, 128, 0},
    {&data_ov003_020da754, 3,  2,    {1, 3, 2, 0}, 0,  7, 128, 0},
    {&data_ov003_020da754, 3,  3,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da754, 3,  3,    {1, 3, 2, 0}, 0,  5, 128, 0},
    {&data_ov003_020da754, 3,  3,    {1, 3, 2, 0}, 0,  6, 128, 0},
    {&data_ov003_020da754, 3,  3,    {1, 3, 2, 0}, 0,  7, 128, 0},
    {&data_ov003_020da754, 3,  4,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da754, 3,  4,    {1, 3, 2, 0}, 0,  5, 128, 0},
    {&data_ov003_020da754, 3,  4,    {1, 3, 2, 0}, 0,  6, 128, 0},
    {&data_ov003_020da754, 3,  4,    {1, 3, 2, 0}, 0,  7, 128, 0},
    {&data_ov003_020da754, 3,  5,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da754, 3,  5,    {1, 3, 2, 0}, 1,  4, 128, 0},
    {&data_ov003_020da754, 3,  6,    {1, 3, 2, 0}, 1,  4, 128, 0},
    {&data_ov003_020da754, 3,  6,    {1, 3, 2, 0}, 1,  5, 128, 0},
    {&data_ov003_020da754, 3,  6,    {1, 3, 2, 0}, 1,  6, 128, 0},
    {&data_ov003_020da7e4, 3,  1,    {1, 3, 2, 0}, 0,  4, 256, 0},
    {&data_ov003_020da7e4, 3,  1,    {1, 3, 2, 0}, 1,  4, 256, 0},
    {&data_ov003_020da7e4, 3,  1,    {1, 3, 2, 0}, 2,  4, 256, 0},
    {&data_ov003_020da7e4, 3,  2,    {1, 3, 2, 0}, 0,  4, 256, 0},
    {&data_ov003_020da7e4, 3,  2,    {1, 3, 2, 0}, 1,  4, 256, 0},
    {&data_ov003_020da7e4, 3,  2,    {1, 3, 2, 0}, 2,  4, 256, 0},
    {&data_ov003_020da7e4, 3,  1,    {1, 3, 2, 0}, 0,  5, 256, 0},
    {&data_ov003_020da7e4, 3,  1,    {1, 3, 2, 0}, 1,  5, 256, 0},
    {&data_ov003_020da7e4, 3,  1,    {1, 3, 2, 0}, 2,  5, 256, 0},
    {&data_ov003_020da7e4, 3,  3,    {1, 3, 2, 0}, 0,  4, 256, 0},
    {&data_ov003_020da7e4, 3,  3,    {1, 3, 2, 0}, 1,  4, 256, 0},
    {&data_ov003_020da7e4, 3,  3,    {1, 3, 2, 0}, 2,  4, 256, 0},
    {&data_ov003_020da76c, 3,  3,    {1, 3, 2, 0}, 0,  4,  16, 0},
    {&data_ov003_020da76c, 3,  1,    {1, 3, 2, 0}, 0,  4,  16, 0},
    {&data_ov003_020da76c, 3,  2,    {1, 3, 2, 0}, 0,  4,  16, 0},
    {&data_ov003_020da76c, 3,  4,    {1, 3, 2, 0}, 0,  4,  16, 0},
    {&data_ov003_020da76c, 3,  4,    {1, 3, 2, 0}, 0,  5,  16, 0},
    {&data_ov003_020da76c, 3,  4,    {1, 3, 2, 0}, 0,  6,  16, 0},
    {&data_ov003_020da76c, 3,  4,    {1, 3, 2, 0}, 0,  7,  16, 0},
    {&data_ov003_020da7bc, 3,  1,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da7bc, 3,  1,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  1,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  2,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da7bc, 3,  2,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  2,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  3,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da7bc, 3,  3,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  3,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  4,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da7bc, 3,  4,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  4,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  6,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da7bc, 3,  6,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  6,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  5,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da7bc, 3,  5,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  5,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  7,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  7,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  8,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  8,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  9,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  9,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 10,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 10,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 11,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 11,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 11,    {1, 3, 2, 0}, 2,  5,  32, 0},
    {&data_ov003_020da7bc, 3, 11,    {1, 3, 2, 0}, 3,  5,  32, 0},
    {&data_ov003_020da7bc, 3, 12,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 12,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 13,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 13,    {1, 3, 2, 0}, 3,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 14,    {1, 3, 2, 0}, 1,  4,  32, 0},
    {&data_ov003_020da7bc, 3,  9,    {1, 3, 2, 0}, 1,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 17,    {1, 3, 2, 0}, 0,  4,  64, 0},
    {&data_ov003_020da7bc, 3, 17,    {1, 3, 2, 0}, 0,  5,  64, 0},
    {&data_ov003_020da7bc, 3, 18,    {1, 3, 2, 0}, 0,  4,  64, 0},
    {&data_ov003_020da7bc, 3, 18,    {1, 3, 2, 0}, 0,  5,  64, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 3,  4, 128, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 1,  4,  32, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 4,  4, 128, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 2,  4, 128, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 3,  5, 128, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 0,  5, 128, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 1,  5,  32, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 4,  5, 128, 0},
    {&data_ov003_020da7bc, 3, 16,    {1, 3, 2, 0}, 2,  5, 128, 0},
    {&data_ov003_020da7bc, 3, 15,    {1, 3, 2, 0}, 1,  4,  64, 0},
    {&data_ov003_020da7bc, 3, 15,    {1, 3, 2, 0}, 1,  5,  64, 0},
    {&data_ov003_020da7ec, 3,  1, {16, 18, 17, 0}, 0, 19,  72, 0},
    {&data_ov003_020da7ec, 3,  1, {16, 18, 17, 0}, 1, 19,  72, 0},
    {&data_ov003_020da7ec, 3,  1,    {1, 3, 2, 0}, 0, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1,    {1, 3, 2, 0}, 1, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1,    {4, 6, 5, 0}, 0, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1,    {4, 6, 5, 0}, 1, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1,    {7, 9, 8, 0}, 0, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1,    {7, 9, 8, 0}, 1, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1, {10, 12, 11, 0}, 0, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1, {10, 12, 11, 0}, 1, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1, {13, 15, 14, 0}, 0, 19,  64, 0},
    {&data_ov003_020da7ec, 3,  1, {13, 15, 14, 0}, 1, 19,  64, 0},
    {&data_ov003_020da7f4, 3,  1,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da7f4, 3,  2,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da7f4, 3,  1,    {1, 3, 2, 0}, 0,  6, 128, 0},
    {&data_ov003_020da7f4, 3,  1,    {1, 3, 2, 0}, 1,  5, 128, 0},
    {&data_ov003_020da7ac, 3,  1,    {5, 7, 6, 0}, 1,  8,  90, 0},
    {&data_ov003_020da7ac, 3,  1,    {5, 7, 6, 0}, 2,  8,  90, 0},
    {&data_ov003_020da7ac, 3,  1,    {5, 7, 6, 0}, 3,  8,  90, 0},
    {&data_ov003_020da7ac, 3,  1,    {5, 7, 6, 0}, 4,  8,  90, 0},
    {&data_ov003_020da7ac, 3,  1,    {1, 3, 2, 0}, 0,  4,   4, 0},
    {&data_ov003_020da7ac, 3,  1,    {1, 3, 2, 0}, 1,  4,   8, 0},
    {&data_ov003_020da7ac, 3,  1,    {1, 3, 2, 0}, 2,  4,  16, 0},
    {&data_ov003_020da7ac, 3,  1,    {1, 3, 2, 0}, 3,  4,  16, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 1,  4, 128, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 3,  4, 128, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 4,  4, 128, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 4,  4, 128, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 4,  4, 128, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 2,  4, 128, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 0,  4,  48, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 5,  4,  48, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 6,  4,  48, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 7,  4,  48, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 0,  4,  48, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 5,  4,  48, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 8,  4,  48, 0},
    {&data_ov003_020da7ac, 3,  2,    {1, 3, 2, 0}, 9,  4,  48, 0},
    {&data_ov003_020da7ac, 3,  3,    {1, 3, 2, 0}, 0,  4,  48, 0},
    {&data_ov003_020da7d4, 3,  1,    {1, 3, 2, 0}, 0,  7,   1, 0},
    {&data_ov003_020da7d4, 3,  1,    {4, 6, 5, 0}, 0,  7,  96, 0},
    {&data_ov003_020da7d4, 3,  2,    {1, 3, 2, 0}, 0,  7,   1, 0},
    {&data_ov003_020da7d4, 3,  2,    {4, 6, 5, 0}, 0,  7,  48, 0},
    {&data_ov003_020da7d4, 3,  3,    {1, 3, 2, 0}, 0,  7,   1, 0},
    {&data_ov003_020da7d4, 3,  3,    {4, 6, 5, 0}, 0,  7,  48, 0},
    {&data_ov003_020da7d4, 3,  4,    {1, 3, 2, 0}, 0,  7,   1, 0},
    {&data_ov003_020da7d4, 3,  4,    {4, 6, 5, 0}, 0,  7,  48, 0},
    {&data_ov003_020da77c, 3,  1,    {1, 3, 2, 0}, 0,  4,  64, 0},
    {&data_ov003_020da77c, 3,  2,    {1, 3, 2, 0}, 0,  4,  64, 0},
    {&data_ov003_020da77c, 3,  3,    {1, 3, 2, 0}, 0,  4,  64, 0},
    {&data_ov003_020da77c, 3,  3,    {1, 3, 2, 0}, 0,  5,  64, 0},
    {&data_ov003_020da794, 3,  2,    {1, 3, 2, 0}, 0,  4, 128, 0},
    {&data_ov003_020da794, 3,  2,    {1, 3, 2, 0}, 1,  4, 128, 0},
    {&data_ov003_020da794, 3,  2,    {1, 3, 2, 0}, 1,  5, 128, 0},
    {&data_ov003_020da794, 3,  2,    {1, 3, 2, 0}, 1,  6, 128, 0},
    {&data_ov003_020da794, 3,  2,    {1, 3, 2, 0}, 2,  4, 128, 0},
    {&data_ov003_020da794, 3,  2,    {1, 3, 2, 0}, 3,  4, 128, 0},
    {&data_ov003_020da794, 3,  2,    {1, 3, 2, 0}, 3,  5, 128, 0},
    {&data_ov003_020da794, 3,  2,    {1, 3, 2, 0}, 3,  6, 128, 0},
    {&data_ov003_020da794, 3,  6,    {1, 3, 2, 0}, 1,  4,  32, 0},
    {&data_ov003_020da794, 3,  4,    {1, 3, 2, 0}, 1,  4, 208, 0},
    {&data_ov003_020da794, 3,  1,    {1, 3, 2, 0}, 4,  4,  40, 0},
    {&data_ov003_020da794, 3,  1,    {1, 3, 2, 0}, 5,  4,  40, 0},
    {&data_ov003_020da794, 3,  1,    {1, 3, 2, 0}, 6,  4,  40, 0},
    {&data_ov003_020da794, 3,  1,    {1, 3, 2, 0}, 3,  4,  40, 0},
    {&data_ov003_020da794, 3,  6,    {1, 3, 2, 0}, 1,  4,  32, 0},
    {&data_ov003_020da794, 3,  6,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da794, 3,  3,    {1, 3, 2, 0}, 0,  4,  40, 0},
    {&data_ov003_020da794, 3,  3,    {1, 3, 2, 0}, 1,  4,  40, 0},
    {&data_ov003_020da794, 3,  4,    {1, 3, 2, 0}, 0,  4, 208, 0},
    {&data_ov003_020da794, 3,  5,    {1, 3, 2, 0}, 1,  4,  96, 0},
    {&data_ov003_020da75c, 3,  1,    {1, 3, 2, 0}, 0,  4,  64, 0},
    {&data_ov003_020da75c, 3,  1,    {1, 3, 2, 0}, 1,  4,  64, 0},
    {&data_ov003_020da75c, 3,  1,    {1, 3, 2, 0}, 2,  4,  64, 0},
    {&data_ov003_020da74c, 3,  1,    {4, 6, 5, 0}, 0,  8,  64, 0},
    {&data_ov003_020da74c, 3,  1,    {4, 6, 5, 0}, 0,  9,  64, 0},
    {&data_ov003_020da74c, 3,  1,    {4, 6, 5, 0}, 0, 10,  64, 0},
    {&data_ov003_020da74c, 3,  1,    {4, 6, 5, 0}, 0, 11,  64, 0},
    {&data_ov003_020da74c, 3,  1,    {1, 3, 2, 0}, 0,  7,  64, 0},
    {&data_ov003_020da79c, 3,  2,    {1, 3, 2, 0}, 0,  4,  64, 0},
    {&data_ov003_020da79c, 3,  3,    {1, 3, 2, 0}, 1,  4,  32, 0},
    {&data_ov003_020da79c, 3,  3,    {1, 3, 2, 0}, 0,  4,  32, 0},
    {&data_ov003_020da79c, 3,  1,    {1, 3, 2, 0}, 0,  4,  34, 0},
    {&data_ov003_020da7cc, 3,  1,    {1, 3, 2, 0}, 0,  4,  64, 0},
    {&data_ov003_020da7a4, 3,  1,    {1, 3, 2, 0}, 2,  4,   1, 0},
    {&data_ov003_020da7a4, 3,  1,    {1, 3, 2, 0}, 3,  4,   1, 0},
    {&data_ov003_020da7a4, 3,  1,    {1, 3, 2, 0}, 4,  4,   1, 0},
    {&data_ov003_020da7a4, 3,  1,    {1, 3, 2, 0}, 5,  4,   1, 0},
    {&data_ov003_020da7a4, 3,  1,    {1, 3, 2, 0}, 6,  4,   1, 0},
    {&data_ov003_020da7b4, 3,  1,    {1, 3, 2, 0}, 2,  4,   4, 0},
    {&data_ov003_020da7b4, 3,  1,    {1, 3, 2, 0}, 3,  4,   6, 0},
    {&data_ov003_020da774, 3,  1,    {1, 3, 2, 0}, 0,  4,  96, 0},
    {&data_ov003_020da774, 3,  1,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da774, 3,  1,    {1, 3, 2, 0}, 2,  4,  96, 0},
    {&data_ov003_020da774, 3,  2,    {1, 3, 2, 0}, 0,  4,  96, 0},
    {&data_ov003_020da774, 3,  2,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da774, 3,  2,    {1, 3, 2, 0}, 2,  4,  96, 0},
    {&data_ov003_020da764, 3,  1,    {1, 3, 2, 0}, 1,  4,  96, 0},
    {&data_ov003_020da764, 3,  1,    {1, 3, 2, 0}, 3,  4,  96, 0},
    {&data_ov003_020da764, 3,  2,    {1, 3, 2, 0}, 0,  4,  96, 0},
    {&data_ov003_020da764, 3,  2,    {1, 3, 2, 0}, 1,  4,  96, 0},
    {&data_ov003_020da764, 3,  2,    {1, 3, 2, 0}, 2,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  2,    {1, 3, 2, 0}, 1,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  2,    {1, 3, 2, 0}, 2,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  2,    {1, 3, 2, 0}, 3,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  2,    {1, 3, 2, 0}, 4,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  2,    {5, 7, 6, 0}, 0,  8,  96, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 0,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 1,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 3,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 8,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 2,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  1,    {1, 3, 2, 0}, 1,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  1,    {1, 3, 2, 0}, 3,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  1,    {1, 3, 2, 0}, 4,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  1,    {1, 3, 2, 0}, 5,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  4,    {1, 3, 2, 0}, 1,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  4,    {1, 3, 2, 0}, 2,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  4,    {1, 3, 2, 0}, 3,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  4,    {1, 3, 2, 0}, 4,  4,  96, 0},
    {&data_ov003_020da7dc, 3,  1,    {1, 3, 2, 0}, 1,  4,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {1, 3, 2, 0}, 2,  4,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {1, 3, 2, 0}, 3,  4,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {1, 3, 2, 0}, 4,  4,  16, 0},
    {&data_ov003_020da7dc, 3,  1,    {1, 3, 2, 0}, 5,  4,   4, 0},
    {&data_ov003_020da7dc, 3,  1,    {1, 3, 2, 0}, 6,  4,  20, 0},
    {&data_ov003_020da7dc, 3,  1,    {1, 3, 2, 0}, 7,  4,  20, 0},
    {&data_ov003_020da7dc, 3,  1,    {1, 3, 2, 0}, 8,  4,  20, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 1,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 1,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 1,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 1,  9,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 2,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 3,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 5,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 2,  9,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 4,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 6,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 7,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 8,  8,  80, 0},
    {&data_ov003_020da7dc, 3,  1,    {5, 7, 6, 0}, 6,  9,  80, 0},
    {&data_ov003_020da78c, 3,  1,    {1, 3, 2, 0}, 0,  4,  16, 0},
    {&data_ov003_020da78c, 3,  1,    {1, 3, 2, 0}, 1,  4,  16, 0},
    {&data_ov003_020da78c, 3,  1,    {1, 3, 2, 0}, 5,  4,  16, 0},
    {&data_ov003_020da804, 3,  1,    {1, 3, 2, 0}, 0,  4, 220, 0},
    {&data_ov003_020da804, 3,  1,    {1, 3, 2, 0}, 0,  5, 220, 0},
    {&data_ov003_020da804, 3,  1,    {6, 8, 7, 0}, 0,  9,  64, 0},
    {&data_ov003_020da804, 3,  1,    {6, 8, 7, 0}, 0, 10,  64, 0},
    {&data_ov003_020da804, 3,  1,    {1, 3, 2, 0}, 2,  4, 220, 0},
    {&data_ov003_020da804, 3,  1,    {1, 3, 2, 0}, 2,  5, 220, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 0,  4,  32, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 1,  4,  32, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 2,  4,  32, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 0,  5,  32, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 1,  5,  32, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 2,  5,  32, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 0,  6,  32, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 1,  6,  32, 0},
    {&data_ov003_020da7c4, 3,  1,    {1, 3, 2, 0}, 2,  6,  32, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 4,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 5,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 6,  4,  96, 0},
    {&data_ov003_020da7fc, 3,  3,    {1, 3, 2, 0}, 7,  4,  96, 0},
    {&data_ov003_020da784, 3,  1,    {1, 3, 2, 0}, 1,  4,  48, 0},
    {&data_ov003_020da784, 3,  1,    {1, 3, 2, 0}, 0,  4,  48, 0},
    {&data_ov003_020da784, 3,  1,    {1, 3, 2, 0}, 2,  4,  48, 0},
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Queues a touch event for ability `id` unless the ability cannot be used by `touchIndex`
/// (a value of -1 or 0..2 selects "any touch" or one specific touch).
void func_ov003_020a30d4(s32 id, s32 touchIndex, u32 slot) {
    u32              anyTouch = 1;
    Ov003TouchEvent* events;
    Ov003AbilityData ability;
    s32              i;

    events = OV003_TOUCH_EVENTS;
    func_020274ac(&ability, id, &data_ov003_020da73c);
    if (touchIndex >= 0 && touchIndex <= 2) {
        anyTouch = 0;
        if (ability.flags.usable[touchIndex] == 0) {
            return;
        }
    } else if (ability.flags.valid == 0) {
        return;
    }
    for (i = 0; i < 0xE; i++) {
        if (events[i].unk_00 == 0xFF) {
            events[i].unk_00   = id;
            events[i].anyTouch = anyTouch;
            events[i].ability  = ability;
            events[i].slot     = slot;
            return;
        }
    }
}

/// TRUE when a touch event queued for `touchMode` (0 = any touch, 1 = any-touch events,
/// 2 = single-touch events) applies to them.
s32 func_ov003_020a31bc(s32 touchMode, s32 anyTouch) {
    switch (touchMode) {
        case 0:
            break;
        case 1:
            if (anyTouch == 0) {
                return 0;
            }
            break;
        case 2:
            if (anyTouch != 0) {
                return 0;
            }
            break;
    }
    return 1;
}

/// TRUE unless the actor of touch slot `slot` is in a state that cannot accept a touch.
// Nonmatching: mwcc range-folds the `state == 4 || state == 5` test instead of the cmp/cmpne chain.
s32 func_ov003_020a31fc(s32 slot) {
    Ov003TouchActor* actor;

    if (slot == -1) {
        return 1;
    }
    for (actor = func_ov003_0208495c(OV003_TOUCH_ACTORS); actor != NULL; actor = func_ov003_02084984(&actor->node)) {
        if (actor->slot != slot) {
            continue;
        }
        if (actor->state == 4 || actor->state == 5) {
            return 0;
        }
        if (actor->state == 0 && (actor->request == 4 || actor->request == 6)) {
            return 0;
        }
        return 1;
    }
    return 0;
}

/// TRUE when `actionId` is one of the actions a queued touch event grants.
s32 func_ov003_020a3298(s32 actionId, s32 touchMode) {
    Ov003TouchEvent*        events = OV003_TOUCH_EVENTS;
    const Ov003AbilityData* ability;
    s32                     i;
    s32                     j;

    for (i = 0; i < 0xE; i++) {
        Ov003TouchEvent* event = &events[i];

        if (event->unk_00 == 0xFF || func_ov003_020a31bc(touchMode, event->anyTouch) == 0 ||
            func_ov003_020a31fc(event->slot) == 0)
        {
            continue;
        }
        ability = &event->ability;
        for (j = 0; j < 4; j++) {
            if (actionId == ability->slots[j].actionId) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_ov003_020a3330(s32 actionId) {
    return func_ov003_020a3298(actionId, 0);
}

s32 func_ov003_020a3340(s32 actionId) {
    return func_ov003_020a3298(actionId, 1);
}

s32 func_ov003_020a3350(s32 actionId) {
    return func_ov003_020a3298(actionId, 2);
}

/// The amount of `actionId` a queued touch event grants: most actions add the recorded amount,
/// but a few (the multi-hit ones) report their per-action value directly.
s32 func_ov003_020a3360(s32 actionId, s32 touchMode) {
    Ov003TouchEvent*        events = OV003_TOUCH_EVENTS;
    const Ov003AbilityData* ability;
    s32                     i;
    s32                     j;
    s32                     total = 0;

    for (i = 0; i < 0xE; i++) {
        Ov003TouchEvent* event = &events[i];

        if (event->unk_00 == 0xFF || func_ov003_020a31bc(touchMode, event->anyTouch) == 0 ||
            func_ov003_020a31fc(event->slot) == 0)
        {
            continue;
        }
        ability = &event->ability;
        for (j = 0; j < 4; j++) {
            if (actionId != ability->slots[j].actionId) {
                continue;
            }
            switch (actionId) {
                case 6:
                case 10:
                case 11:
                case 15:
                case 33:
                    return ability->slots[j].amount;
                default:
                    total += ability->slots[j].amount;
                    break;
            }
        }
    }
    return total;
}

s32 func_ov003_020a3468(s32 actionId) {
    return func_ov003_020a3360(actionId, 0);
}

s32 func_ov003_020a3478(s32 actionId) {
    return func_ov003_020a3360(actionId, 1);
}

s32 func_ov003_020a3488(s32 actionId) {
    return func_ov003_020a3360(actionId, 2);
}

/// Loads the icon of touch entry `index` into `cSprite`; `arg2` selects the looped/one-shot form
/// and `arg4` is passed through to the animation setup.
// Nonmatching: the SpriteAnimationEx field stores are scheduled differently.
s32 func_ov003_020a3498(CombatSprite* cSprite, s32 arg1, s32 arg2, s32 index, u16 arg4) {
    const Ov003TouchIconParams* params = &data_ov003_020da80c[index];
    SpriteAnimationEx           anim;

    CombatSprite_InitAnim(&anim.anim, arg1, params->binIden);
    anim.anim.bits_7_9  = 6;
    anim.anim.unk_18    = params->unk_04;
    anim.anim.packIndex = params->unk_06;
    if (arg2 == 0) {
        anim.anim.bits_10_11 = 1;
        anim.anim.unk_1C     = params->entry.charDataIndex;
        anim.anim.unk_1E     = params->unk_14 << 5;
        anim.anim.unk_26     = params->entry.frameDataIndex;
        anim.anim.unk_28     = params->entry.paletteDataIndex;
        anim.anim.unk_20     = params->unk_12;
        anim.anim.animIndex  = params->entry.animDataIndex + 1;
        anim.animTable       = &params->entry;
    } else {
        anim.anim.bits_10_11 = 0;
        anim.anim.unk_1C     = params->entry.charDataIndex;
        anim.anim.unk_26     = params->entry.frameDataIndex;
        anim.anim.unk_28     = params->entry.paletteDataIndex;
        anim.anim.unk_20     = params->unk_12;
        anim.anim.animIndex  = 1;
        anim.animTable       = NULL;
    }
    if (CombatSprite_Load(cSprite, &anim) == 0) {
        return 0;
    }
    CombatSprite_SetAnim(cSprite, params->unk_10, arg4);
    return 1;
}

void func_ov003_020a35b8(CombatSprite* cSprite, s32 index, s32 arg2) {
    const Ov003TouchIconParams* params = &data_ov003_020da80c[index];

    CombatSprite_SetAnim(cSprite, params->unk_10, arg2);
}

/// Drops every touch action actor of the battle.
void func_ov003_020a35d8(void) {
    data_ov003_020e71b8->touchActorCount = 0;
    func_ov003_020849ac(OV003_TOUCH_ACTORS);
}

/// Publishes the actor positions to the 3D layer's slot table.
// Nonmatching: the `MainData` slot table base is two adds instead of a folded pool word.
void func_ov003_020a3608(void) {
    Ov003TouchSlotTable* table = OV003_TOUCH_SLOTS;
    Ov003TouchActor*     actor;

    actor = func_ov003_0208495c(OV003_TOUCH_ACTORS);
    MI_CpuSet(table->kind, 0, 6);
    if (actor != NULL && actor->kind == 0xFFFE) {
        table->playerX = actor->x >> 12;
        table->playerY = actor->y >> 12;
        actor          = func_ov003_02084984(&actor->node);
    }
    if (actor == NULL) {
        return;
    }
    do {
        if (!(actor->effect.state == 0x3C && actor->state == 3)) {
            table->x[actor->slot] = actor->x >> 12;
            table->y[actor->slot] = actor->y >> 12;
        }
        if (data_ov003_020e71b8->unk3D890 != 0xD) {
            table->kind[actor->slot] = 0;
        } else {
            table->kind[actor->slot]   = actor->state;
            table->unk_14[actor->slot] = actor->unk_044;
            table->unk_20[actor->slot] = actor->unk_048;
        }
        actor = func_ov003_02084984(&actor->node);
    } while (actor != NULL);
}

/// Queues one touch event for every equipped pin of the current deck (`arg0` selects the deck
/// of the active player).
void func_ov003_020a3728(s32 arg0) {
    RawPinData pin;
    u16        i;

    for (i = 0; i < 6; i++) {
        u16 pinId = func_ov003_020a4718(arg0, i)->pinID;

        if (pinId != 0xFFFF) {
            func_ov003_020a474c(pinId, &pin);
            if (func_ov003_020a6cc4(pin.brand) == 0 && pin.abilityId != 0xFF) {
                func_ov003_020a30d4(pin.abilityId, -1, i);
            }
        }
    }
}

/// Spawns the touch icon task and queues one action per equipped pin of the current deck.
// Nonmatching: the psych-id flag switch lowers to a two-level table in the target.
void func_ov003_020a37a8(s32 arg0) {
    Ov003PinAction action;
    RawPinData     pin;
    u16            i;

    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d7d24, NULL, 0, NULL, NULL);
    for (i = 0; i < 6; i++) {
        u16                pinId = func_ov003_020a4718(arg0, i)->pinID;
        const EquippedPin* equipped;

        if (pinId == 0xFFFF) {
            continue;
        }
        equipped = func_ov003_020a4718(arg0, i);
        func_ov003_020a474c(pinId, &pin);
        action.pinId  = pinId;
        action.level  = equipped->flags.bits.level;
        action.unk_04 = equipped->unk_08;
        action.pin    = &pin;
        action.slot   = i;
        switch (pin.durationType) {
            case 2:
                func_ov003_020a7e68(&action);
                break;
            case 1:
            case 3:
                func_ov003_020a7640(&action);
                break;
            default:
                func_ov003_020a6d24(&action);
                break;
        }
        switch (pin.psychId) {
            case 9:
            case 14:
            case 18:
            case 40:
                OV003_PSYCH_FLAGS |= 2;
                break;
            case 15:
            case 17:
            case 26:
                OV003_PSYCH_FLAGS |= 0x20;
                break;
        }
    }
    func_ov003_020a3924();
}

/// Runs one update of every touch action actor.
void func_ov003_020a3924(void) {
    Ov003TouchActor* actor;

    actor = func_ov003_0208495c(OV003_TOUCH_ACTORS);
    func_ov003_020a4158();
    while (actor != NULL) {
        if (actor->kind != 0xFFFE) {
            func_ov003_020a45d4(actor);
        }
        actor = func_ov003_02084984(&actor->node);
    }
}

/// TRUE while at least one non-player actor is alive.
s32 func_ov003_020a3980(void) {
    Ov003TouchActor* actor;

    for (actor = func_ov003_0208495c(OV003_TOUCH_ACTORS); actor != NULL; actor = func_ov003_02084984(&actor->node)) {
        if (actor->kind != 0xFFFE && actor->unk_00E != 2) {
            return 1;
        }
    }
    return 0;
}

/// Snaps every actor back onto its starting slot position.
void func_ov003_020a39dc(void) {
    Ov003TouchActor* actor;

    for (actor = func_ov003_0208495c(OV003_TOUCH_ACTORS); actor != NULL; actor = func_ov003_02084984(&actor->node)) {
        actor->x = data_0205c19d[actor->slot * 2] << 12;
        actor->y = data_0205c19d[actor->slot * 2 + 1] << 12;
    }
}

/// Clears the "actor is being dragged" flag of every actor.
void func_ov003_020a3a40(void) {
    Ov003TouchActor* actor;

    for (actor = func_ov003_0208495c(OV003_TOUCH_ACTORS); actor != NULL; actor = func_ov003_02084984(&actor->node)) {
        actor->unk_04C = 0;
    }
}

/// Picks the actor under the touch/stylus and moves it there; SELECT resets the positions.
void func_ov003_020a3a80(void) {
    Ov003TouchActor* actor;
    Ov003TouchActor* selected = NULL;

    if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_SELECT) {
        func_ov003_020a39dc();
        return;
    }
    for (actor = func_ov003_0208495c(OV003_TOUCH_ACTORS); actor != NULL; actor = func_ov003_02084984(&actor->node)) {
        s32 active = func_ov003_020a3e50(actor);

        if (actor->unk_04C != 0) {
            if (active != 0 && func_ov003_02085cfc(6) != 0) {
                selected = actor;
            } else {
                actor->unk_04C = 0;
            }
        }
        if (selected == NULL && func_ov003_02085cfc(6) != 0 && active != 0) {
            if (func_ov003_020a410c(actor, data_ov003_020e71b8->unk3D3AA, data_ov003_020e71b8->unk3D3AC) != 0) {
                selected       = actor;
                actor->unk_04C = 1;
            }
        }
    }
    if (selected != NULL) {
        selected->x = data_ov003_020e71b8->unk3D3AA << 12;
        selected->y = data_ov003_020e71b8->unk3D3AC << 12;
    }
    func_ov003_020a4158();
}
