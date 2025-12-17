#include "Input.h"

InputState InputStatus;

static const u16 RepeatButtons[26] = {
    INPUT_MASK_DPAD,    INPUT_BUTTON_A,     INPUT_BUTTON_B,     INPUT_BUTTON_X,      INPUT_BUTTON_Y,
    INPUT_BUTTON_L,     INPUT_BUTTON_R,     INPUT_BUTTON_START, INPUT_BUTTON_SELECT, 0,
    INPUT_BUTTON_DOWN,  INPUT_BUTTON_B,     INPUT_BUTTON_UP,    INPUT_BUTTON_X,      INPUT_BUTTON_LEFT,
    INPUT_BUTTON_Y,     INPUT_BUTTON_RIGHT, INPUT_BUTTON_A,     INPUT_BUTTON_B,      INPUT_BUTTON_DOWN,
    INPUT_BUTTON_X,     INPUT_BUTTON_UP,    INPUT_BUTTON_Y,     INPUT_BUTTON_LEFT,   INPUT_BUTTON_A,
    INPUT_BUTTON_RIGHT,
};

void Input_Init(InputState* input, s32 delayInit, s32 delayMin, s32 step) {
    input->buttonState.currButtons    = 0;
    input->buttonState.pressedButtons = 0;
    input->buttonState.holdButtons    = 0;
    input->buttonState.prevButtons    = 0;
    input->repeatInitialDelay         = delayInit;
    input->repeatMinDelay             = delayMin;
    input->repeatStep                 = step;

    for (u32 i = 0; i < ARRAY_COUNT(input->repeatDelay); i++) {
        input->repeatDelay[i]   = delayInit;
        input->repeatCounter[i] = delayInit;
    }
}

void Input_PollState(InputState* input) {
    if (Input_IsSystemHingeClosed() == FALSE) {
        u16 state                         = Input_ReadState();
        input->buttonState.pressedButtons = (input->buttonState.currButtons ^ state) & state & INPUT_MASK_ALLBTNS;
        input->buttonState.currButtons    = INPUT_MASK_ALLBTNS & state;
    } else {
        input->buttonState.currButtons    = 0;
        input->buttonState.pressedButtons = 0;
        input->buttonState.holdButtons    = 0;
    }
}

void Input_UpdateRepeat(InputState* state, u16 mask) {
    s32* delay   = state->repeatDelay;
    s32* counter = state->repeatCounter;

    s32 nlr;
    s32 currMask;
    s32 idx;

    idx      = 0;
    currMask = INPUT_MASK_DPAD;

    do {
        if (currMask & mask) {
            nlr = ~currMask;

            state->buttonState.holdButtons &= nlr;
            state->buttonState.holdButtons |= (state->buttonState.pressedButtons & currMask);

            s32 temp_r7 = state->buttonState.currButtons & currMask;
            if (temp_r7 && (temp_r7 == (state->buttonState.prevButtons & currMask))) {
                counter[idx]--;
                if (counter[idx] <= 0) {
                    delay[idx] -= state->repeatStep;
                    if (delay[idx] < state->repeatMinDelay) {
                        delay[idx] = state->repeatMinDelay;
                    }
                    counter[idx] = delay[idx];
                    state->buttonState.holdButtons &= nlr;
                    state->buttonState.holdButtons |= state->buttonState.currButtons & currMask;
                }
            } else {
                delay[idx] += state->repeatStep;
                if (delay[idx] > state->repeatInitialDelay) {
                    delay[idx] = state->repeatInitialDelay;
                }
                counter[idx] = delay[idx];
            }
        }

        idx++;
        currMask = RepeatButtons[idx];
    } while (currMask);

    state->buttonState.prevButtons = state->buttonState.currButtons;
}
