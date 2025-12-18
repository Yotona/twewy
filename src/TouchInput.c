#include "TouchInput.h"
#include "Input.h"

static struct {
    /* 0x00 */ BOOL       isTouchEnabled; // Whether touch input is enabled
    /* 0x04 */ s32        xMin;           // Minimum X coordinate
    /* 0x08 */ s32        yMin;           // Minimum Y coordinate
    /* 0x0C */ s32        xMax;           // Maximum X coordinate
    /* 0x10 */ s32        yMax;           // Maximum Y coordinate
    /* 0x14 */ BOOL       isTouchActive;  // Whether the screen is currently being touched
    /* 0x18 */ BOOL       prevFrameTouch; // Whether the screen was being touched in the previous frame
    /* 0x1C */ s32        holdDuration;   // Duration the screen has been continuously touched, in frames
    /* 0x20 */ s32        holdThreshold;  // Hold duration threshold for tap detection
    /* 0x24 */ s32        tapTimer;       // Countdown timer for tap detection
    /* 0x28 */ s32        tapCount;
    /* 0x2C */ TouchCoord touchCoord;     // Touch coordinates for the current frame
    /* 0x34 */ TouchCoord prevCoord;      // Touch coordinates from the previous frame
    /* 0x3C */ u16        rawInput[4];    // Raw hardware touch data
} TouchState = {
    .isTouchEnabled = FALSE,
    .xMin           = 0,
    .yMin           = 0,
    .xMax           = 255,
    .yMax           = 191,
    .isTouchActive  = FALSE,
    .prevFrameTouch = FALSE,
    .holdDuration   = 0,
    .holdThreshold  = 15,
    .tapTimer       = -1,
    .tapCount       = 0,
    .touchCoord     = {-1, -1},
    .prevCoord      = {-1, -1},
    .rawInput       = {0, 0, 0, 0},
};

void TouchInput_Init(void) {
    u16 stack[4];

    func_0203fecc();

    TouchState.isTouchEnabled = FALSE;

    if (func_0203ff44(&stack) != 0) {
        func_020401fc(8, 0x14);
        func_020405a8(8);
        if (func_020405c0(8) == 0) {
            func_0203ffd8(&stack);
            TouchState.isTouchEnabled = TRUE;
        }
    }

    TouchState.xMin = 0;
    TouchState.yMin = 0;
    TouchState.xMax = 255;
    TouchState.yMax = 191;

    TouchState.isTouchActive  = FALSE;
    TouchState.prevFrameTouch = FALSE;
    TouchState.holdDuration   = 0;
    TouchInput_SetHoldThreshold(15);
    TouchState.tapTimer     = -1;
    TouchState.tapCount     = 0;
    TouchState.touchCoord.x = -0x80;
    TouchState.touchCoord.y = -0x80;
    TouchState.prevCoord.x  = -0x80;
    TouchState.prevCoord.y  = -0x80;
}

// Nonmatching: Minor instruction reordering
// Scratch: kj9VF
u16* TouchInput_Update(void) {
    func_0203b3c0(TouchState.rawInput, 0, 8);
    if (TouchState.isTouchEnabled == FALSE) {
        return NULL;
    }

    if (Input_IsSystemHingeClosed() == FALSE) {
        func_02040100();
        func_020401e0(TouchState.rawInput);
        TouchState.prevFrameTouch = TouchState.isTouchActive;
    } else {
        TouchState.rawInput[2]    = 0;
        TouchState.rawInput[3]    = 3;
        TouchState.prevFrameTouch = FALSE;
    }

    TouchState.prevCoord = TouchState.touchCoord;

    if (TouchState.rawInput[2] == 1) {
        if (TouchState.rawInput[3] == 0) {
            TouchState.isTouchActive = TRUE;
            TouchState.touchCoord.x  = TouchState.rawInput[0];
            TouchState.touchCoord.y  = TouchState.rawInput[1];
        }
    } else {
        TouchState.isTouchActive = FALSE;
    }

    if (TouchState.touchCoord.x < TouchState.xMin) {
        TouchState.touchCoord.x = TouchState.xMin;
    }
    if (TouchState.touchCoord.x > TouchState.xMax) {
        TouchState.touchCoord.x = TouchState.xMax;
    }
    if (TouchState.touchCoord.y < TouchState.yMin) {
        TouchState.touchCoord.y = TouchState.yMin;
    }
    if (TouchState.touchCoord.y > TouchState.yMax) {
        TouchState.touchCoord.y = TouchState.yMax;
    }

    if (TouchState.isTouchActive != TouchState.prevFrameTouch) {
        TouchState.holdDuration = 0;

        if (TouchState.isTouchActive == TRUE) {
            TouchState.tapCount++;

            if (TouchState.tapTimer < 0) {
                TouchState.tapTimer = TouchState.holdThreshold;
            }
        }
    }

    TouchState.holdDuration++;
    TouchState.tapTimer--;

    if (TouchState.tapTimer < 0) {
        TouchState.tapTimer = -1;
        TouchState.tapCount = 0;
    }
    return TouchState.rawInput;
}

BOOL TouchInput_IsTouchActive(void) {
    if (TouchState.isTouchEnabled == FALSE) {
        return FALSE;
    }
    return TouchState.isTouchActive;
}

BOOL TouchInput_WasTouchPressed(void) {
    if (TouchState.isTouchEnabled == FALSE) {
        return FALSE;
    }
    if (TouchState.isTouchActive == TRUE && TouchState.prevFrameTouch == FALSE) {
        return TRUE;
    }
    return FALSE;
}

BOOL TouchInput_WasTouchReleased(void) {
    if (TouchState.isTouchEnabled == FALSE) {
        return FALSE;
    }
    if (TouchState.isTouchActive == FALSE && TouchState.prevFrameTouch == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL TouchInput_IsBeingHeld(void) {
    if (TouchState.isTouchEnabled == FALSE) {
        return FALSE;
    }
    if (TouchState.isTouchActive == TRUE && TouchState.holdDuration > 1) {
        return TRUE;
    }
    return FALSE;
}

void TouchInput_GetCoord(TouchCoord* out) {
    TouchCoord base;
    base.x = 0;
    base.y = 0;

    if (TouchState.isTouchEnabled == FALSE || TouchState.touchCoord.x < 0 || TouchState.touchCoord.y < 0) {
        *out = base;
    } else {
        *out = TouchState.touchCoord;
    }
}

void TouchInput_GetDelta(TouchCoord* out) {
    TouchCoord delta;
    delta.x = 0;
    delta.y = 0;

    if (TouchState.isTouchEnabled == FALSE) {
        *out = delta;
        return;
    }

    if (TouchInput_IsTouchActive() == 0) {
        *out = delta;
        return;
    }

    if (TouchInput_WasTouchPressed() == 1) {
        *out = delta;
        return;
    }

    if (TouchState.prevCoord.x >= 0) {
        delta.x = TouchState.touchCoord.x - TouchState.prevCoord.x;
    }

    if (TouchState.prevCoord.y >= 0) {
        delta.y = TouchState.touchCoord.y - TouchState.prevCoord.y;
    }

    *out = delta;
}

s32 TouchInput_GetHoldDuration(void) {
    if (TouchState.isTouchEnabled == FALSE) {
        return 0;
    }
    return TouchState.holdDuration;
}

void TouchInput_SetHoldThreshold(s32 threshold) {
    TouchState.holdThreshold = threshold - 1;
}
