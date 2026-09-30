#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Private/BtlEnm006.h"

/// One of the `Tsk_BtlEnm006` asset variants.
typedef struct Enm006Variant {
    /* 0x00 */ BinIdentifier*   binIden;
    /* 0x04 */ SpriteAnimEntry* animTable;
    /* 0x08 */ u16              unk_08;
    /* 0x0A */ u16              unk_0A;
    /* 0x0C */ u16              unk_0C;
    /* 0x0E */ u16              unk_0E;
} Enm006Variant; // Size: 0x10

extern char                  data_ov010_0212932c[28];
extern char                  data_ov010_02129348[28];
extern char                  data_ov010_02129364[28];
extern const BinIdentifier   data_ov010_02129050[3];
extern const SpriteAnimEntry data_ov010_02129068[17];
extern const SpriteAnimEntry data_ov010_021290f0[14];
extern const SpriteAnimEntry data_ov010_02129178[17];

typedef struct Enm006VariantTail {
    /* 0x00 */ Enm006Variant   variant;
    /* 0x10 */ SpriteAnimEntry anim[3];
} Enm006VariantTail; // Size: 0x28

const Enm006Variant data_ov010_02129008 = {&data_ov010_02129050[2], data_ov010_021290f0, 0, 0x13, 0x160};
const Enm006Variant data_ov010_02129018 = {&data_ov010_02129050[1], data_ov010_02129068, 0, 0x13, 0x150};

const Enm006VariantTail data_ov010_02129028 = {
    {&data_ov010_02129050[0], data_ov010_02129178, 0, 0x13, 0x150},
    {
     {19, 20, 21, 22},
     {19, 20, 21, 22},
     {19, 20, 21, 22},
     },
};

const BinIdentifier data_ov010_02129050[3] = {
    {3, data_ov010_0212932c},
    {3, data_ov010_02129348},
    {3, data_ov010_02129364},
};

const SpriteAnimEntry data_ov010_02129068[17] = {
    { 1,  3,  2, 0},
    { 1,  3,  2, 1},
    { 1,  3,  2, 2},
    { 4,  6,  5, 0},
    { 4,  6,  5, 1},
    { 4,  6,  5, 2},
    { 7,  9,  8, 0},
    { 7,  9,  8, 1},
    { 7,  9,  8, 2},
    { 7,  9,  8, 3},
    { 7,  9,  8, 3},
    { 7,  9,  8, 4},
    {10, 12, 11, 0},
    {16, 18, 17, 2},
    {13, 15, 14, 0},
    {16, 18, 17, 3},
    {13, 15, 14, 1},
};

const SpriteAnimEntry data_ov010_021290f0[14] = {
    { 1,  3,  2, 0},
    { 1,  3,  2, 1},
    { 1,  3,  2, 2},
    { 4,  6,  5, 0},
    { 4,  6,  5, 1},
    { 4,  6,  5, 2},
    { 7,  9,  8, 0},
    { 7,  9,  8, 1},
    { 7,  9,  8, 2},
    { 7,  9,  8, 3},
    { 7,  9,  8, 3},
    { 7,  9,  8, 4},
    {10, 12, 11, 0},
    {16, 18, 17, 2},
};

const SpriteAnimEntry data_ov010_02129160[3] = {
    {13, 15, 14, 0},
    {16, 18, 17, 3},
    {13, 15, 14, 1},
};

const SpriteAnimEntry data_ov010_02129178[17] = {
    { 1,  3,  2, 0},
    { 1,  3,  2, 1},
    { 1,  3,  2, 2},
    { 4,  6,  5, 0},
    { 4,  6,  5, 1},
    { 4,  6,  5, 2},
    { 7,  9,  8, 0},
    { 7,  9,  8, 1},
    { 7,  9,  8, 2},
    { 7,  9,  8, 3},
    { 7,  9,  8, 3},
    { 7,  9,  8, 4},
    {10, 12, 11, 0},
    {16, 18, 17, 2},
    {13, 15, 14, 0},
    { 1,  3,  2, 0},
    { 1,  3,  2, 0},
};

Enm006Variant* data_ov010_02129320[3] = {
    &data_ov010_02129028.variant,
    &data_ov010_02129018,
    &data_ov010_02129008,
};

char data_ov010_0212932c[28] = "Apl_Suy/Grp_BtlEnm006.bin";
char data_ov010_02129348[28] = "Apl_Suy/Grp_BtlEnm006a.bin";
char data_ov010_02129364[28] = "Apl_Suy/Grp_BtlEnm006b.bin";

BinIdentifier* func_ov010_021256c0(s32 index) {
    return (BinIdentifier*)&data_ov010_02129050[index];
}
