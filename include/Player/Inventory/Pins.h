#ifndef INVENTORY_PIN_H
#define INVENTORY_PIN_H

#include <types.h>

typedef union {
    struct {
        u8 level  : 7;
        u8 unk_07 : 1;
    } bits;
    u8 raw;
} PinFlags;

/**
 * @brief Pin information as stored from within an external binary data file.
 */
typedef struct {
    /* 0x00 */ char unk_00[0x8];
    /* 0x08 */ u32  unk_08;
    /* 0x0C */ s16  unk_0C;
    /* 0x0E */ char unk_0E[0x25 - 0x0E];
    /* 0x25 */ u8   unk_25;
    /* 0x26 */ char unk_26[0x34 - 0x26];
} RawPinData; // Size: 0x34

typedef struct {
    /* 0x00 */ u16      pinID;
    /* 0x02 */ u16      battlePP;
    /* 0x04 */ u16      minglePP;
    /* 0x06 */ u16      shutdownPP;
    /* 0x08 */ u8       unk_08;
    /* 0x09 */ PinFlags flags;
} EquippedPin;

typedef struct {
    /* 0x00 */ u16      pinID;
    /* 0x02 */ u16      battlePP;
    /* 0x04 */ u16      minglePP;
    /* 0x06 */ u16      shutdownPP;
    /* 0x08 */ PinFlags flags;
} StockpilePin;

typedef struct {
    /* 0x0 */ u16      pinID;
    /* 0x2 */ u8       count;
    /* 0x3 */ PinFlags flags;
} MasteredPin;

// TODO: Add the rest?
typedef enum {
    PIN_PYROKINESIS   = 265,
    PIN_SHOCKWAVE     = 266,
    PIN_CURE_DRINK    = 267,
    PIN_FORCE_ROUNDS  = 268,
    PIN_PSYCHOKINESIS = 269,
    PIN_THUNDERBOLT   = 270,
} PinID;

#endif // INVENTORY_PIN_H