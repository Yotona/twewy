#ifndef COMBAT_NOISE_PRIVATE_BTLENM006_H
#define COMBAT_NOISE_PRIVATE_BTLENM006_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/File/BinMgr.h"

typedef struct BtlEnm006 {
    /* 0x000 */ CombatActor       actor;
    /* 0x07C */ s32               unk_07C;
    /* 0x080 */ u16               unk_080; // asset/variant index
    /* 0x082 */ u16               unk_082;
    /* 0x084 */ CombatSprite      sprite;
    /* 0x0E4 */ u8                unk_0E4[0x144 - 0xE4];
    /* 0x144 */ s32               unk_144;
    /* 0x148 */ u8                unk_148[0x184 - 0x148];
    /* 0x184 */ struct BtlEnm006* twin;
    /* 0x188 */ u16*              unk_188; // the UG's unk_1F6 pair: [0] is consumed by the UG, [1] by the RG
    /* 0x18C */ u16               unk_18C;
    /* 0x18E */ u8                unk_18E[0x198 - 0x18E];
    /* 0x198 */ s16               unk_198;
    /* 0x19A */ u8                unk_19A[0x19C - 0x19A];
    /* 0x19C */ s16               unk_19C;
    /* 0x19E */ s16               unk_19E;
    /* 0x1A0 */ u8                unk_1A0[0x1A2 - 0x1A0];
    /* 0x1A2 */ s16               unk_1A2;
    /* 0x1A4 */ u8                unk_1A4[0x1AC - 0x1A4];
    /* 0x1AC */ s32               unk_1AC; // the position the waypoint seek was launched from
    /* 0x1B0 */ s32               unk_1B0;
    /* 0x1B4 */ s32               unk_1B4;
    /* 0x1B8 */ u8                unk_1B8[0x1C0 - 0x1B8];
    /* 0x1C0 */ s16               unk_1C0; // frames spent in the current state
    /* 0x1C2 */ s16               unk_1C2; // state duration
    /* 0x1C4 */ s16               unk_1C4; // state phase
    /* 0x1C6 */ u8                unk_1C6[0x1C8 - 0x1C6];
    /* 0x1C8 */ void*             unk_1C8;
    /* 0x1CC */ s32               unk_1CC;
    /* 0x1D0 */ s32               unk_1D0;
    /* 0x1D4 */ s32               unk_1D4;
    /* 0x1D8 */ s32               unk_1D8;
    /* 0x1DC */ s32               unk_1DC;
    /* 0x1E0 */ s32               unk_1E0;
    /* 0x1E4 */ s32               unk_1E4;
    /* 0x1E8 */ u16               unk_1E8; // damage level, 0..3
    /* 0x1EA */ u8                unk_1EA[0x1EC - 0x1EA];
    /* 0x1EC */ s32               unk_1EC; // waypoint index
    /* 0x1F0 */ s16               unk_1F0; // frames spent on the current waypoint leg
    /* 0x1F2 */ s16               unk_1F2; // frames the current waypoint leg should take
    /* 0x1F4 */ union {
        struct {
            /* 0x1F4 */ u8  unk_1F4; // bits 0-1: follow-up mode, bit 2: waypoint reflection
            /* 0x1F8 */ s32 unk_1F8; // result of the last follower spawn, or -1 for "none"
        } rg;                        // Tsk_BtlEnm006_RG: size 0x1FC
        struct {
            /* 0x1F4 */ u8  unk_1F4;
            /* 0x1F5 */ u8  unk_1F5;    // bit 0 mirrors the sprite, bit 1 keeps flags bit 30 set
            /* 0x1F6 */ u16 unk_1F6[2]; // pending damage steps: [0] for this UG, [1] for its RG twin
            /* 0x1FA */ u8  unk_1FA[0x200 - 0x1FA];
        } ug;                           // Tsk_BtlEnm006_UG: size 0x200
    } ext;
} BtlEnm006;

typedef struct Enm006Spawn {
    /* 0x00 */ BtlEnm006* unk_00;
    /* 0x04 */ BtlEnm006* unk_04;
    /* 0x08 */ BtlEnm006* unk_08;
    /* 0x0C */ BtlEnm006* unk_0C;
} Enm006Spawn;

BinIdentifier* func_ov010_021256c0(s32 index);
void           func_ov010_021256d0(BtlEnm006* owner, BtlEnm006* target);
void           func_ov010_02126c38(BtlEnm006* arg0);
s32            func_ov010_021270a8(BtlEnm006* owner, BtlEnm006* target);
void           func_ov010_02127110(BtlEnm006* owner, BtlEnm006* target);

#endif /* COMBAT_NOISE_PRIVATE_BTLENM006_H */
