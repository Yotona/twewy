#ifndef INPUT_H
#define INPUT_H

#include <bios.h>
#include <registers.h>
#include <types.h>

#define INPUT_MASK_DPAD    0x00F0
#define INPUT_MASK_ABXY    0x2F0F
#define INPUT_MASK_ALLBTNS 0x2FFF
#define INPUT_MASK_HINGE   0x8000

typedef enum {
    INPUT_BUTTON_A      = 0x0001,
    INPUT_BUTTON_B      = 0x0002,
    INPUT_BUTTON_SELECT = 0x0004,
    INPUT_BUTTON_START  = 0x0008,
    INPUT_BUTTON_RIGHT  = 0x0010,
    INPUT_BUTTON_LEFT   = 0x0020,
    INPUT_BUTTON_UP     = 0x0040,
    INPUT_BUTTON_DOWN   = 0x0080,
    INPUT_BUTTON_R      = 0x0100,
    INPUT_BUTTON_L      = 0x0200,
    INPUT_BUTTON_X      = 0x0400,
    INPUT_BUTTON_Y      = 0x0800
} InputButton;

#define INPUT_SOFT_RESET (INPUT_BUTTON_START | INPUT_BUTTON_SELECT | INPUT_BUTTON_L | INPUT_BUTTON_R)

typedef struct InputState {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 currButtons;        // Current frame state
    /* 0x06 */ u16 pressedButtons;     // Newly pressed this frame
    /* 0x08 */ u16 holdButtons;        // Still being held this frame
    /* 0x0A */ u16 prevButtons;        // Previous frame state
    /* 0x0C */ s32 repeatInitialDelay; // Initial delay before repeating
    /* 0x10 */ s32 repeatMinDelay;     // Minimum delay between repeats
    /* 0x14 */ s32 repeatStep;         // Amount to change delay by
    /* 0x18 */ s32 repeatDelay[16];    // Current delay for each button
    /* 0x58 */ s32 repeatCounter[16];  // Counter for each button
} InputState;

extern InputState InputStatus;

void Input_Init(InputState* input, s32 delayInit, s32 delayMin, s32 step);
void Input_PollState(InputState* input);
void Input_UpdateRepeat(InputState* state, u16 mask);

static inline BOOL Input_IsSystemHingeClosed(void) {
    return (BIOS_EXTRA_BUTTONS & INPUT_MASK_HINGE) >> 15;
}

static inline u16 Input_ReadState(void) {
    return ((REG_KEYINPUT | BIOS_EXTRA_BUTTONS) ^ INPUT_MASK_ALLBTNS) & INPUT_MASK_ALLBTNS;
}

#endif // INPUT_H