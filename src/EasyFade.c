#include "EasyFade.h"
#include "Display.h"

s32 EasyFade_TaskHandler(TaskPool* pool, Task* task, void* arg2, s32 arg3);

const TaskHandle Task_EasyFade = {"EasyFade", EasyFade_TaskHandler, 0};

Fader gFaders[2] = {0};

static void EasyFade_Setup(FaderMode mode, s32 brightness, s32 rate, s32 arg3) {
    if (mode == FADER_INSTANT) {
        Fader* fader = &gFaders[DISPLAY_MAIN];
        if (arg3 == 0) {
            fader = &gFaders[DISPLAY_SUB];
        }

        if (fader->targetBrightness != (brightness * 0x1000) || fader->rate != rate) {
            fader->framesRemaining = rate;
        }
    }
}

void EasyFade_FadeBothDisplays(FaderMode mode, s32 brightness, s32 rate) {
    EasyFade_Setup(mode, brightness, rate, 1);
    EasyFade_Setup(mode, brightness, rate, 0);
    gFaders[DISPLAY_MAIN].mode             = mode;
    gFaders[DISPLAY_SUB].mode              = mode;
    gFaders[DISPLAY_MAIN].targetBrightness = brightness * 0x1000;
    gFaders[DISPLAY_SUB].targetBrightness  = brightness * 0x1000;
    gFaders[DISPLAY_MAIN].rate             = rate;
    gFaders[DISPLAY_SUB].rate              = rate;
}

void EasyFade_FadeMainDisplay(FaderMode mode, s32 brightness, s32 rate) {
    EasyFade_Setup(mode, brightness, rate, 1);
    gFaders[DISPLAY_MAIN].mode             = mode;
    gFaders[DISPLAY_MAIN].targetBrightness = brightness * 0x1000;
    gFaders[DISPLAY_MAIN].rate             = rate;
}

void EasyFade_FadeSubDisplay(FaderMode mode, s32 brightness, s32 rate) {
    EasyFade_Setup(mode, brightness, rate, 0);
    gFaders[DISPLAY_SUB].mode             = mode;
    gFaders[DISPLAY_SUB].targetBrightness = brightness * 0x1000;
    gFaders[DISPLAY_SUB].rate             = rate;
}

// Nonmatching: Some instruction differences
// Scratch: aTNjX
BOOL EasyFade_IsFading(void) {
    Fader* faderMain = &gFaders[DISPLAY_MAIN];
    Fader* faderSub  = &gFaders[DISPLAY_SUB];

    BOOL val = TRUE;
    if ((faderMain->currentBrightness >> 0xC) != (faderMain->targetBrightness >> 0xC)) {
        return TRUE;
    } else if ((faderSub->currentBrightness >> 0xC) != (faderSub->targetBrightness >> 0xC)) {
        val = TRUE;
    }
    return val;
}

static s32 EasyFade_CalculateInterpolation(s32 current, s32 target, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 diff     = target - current;
    s32 abs_diff = (diff < 0) ? -diff : diff;
    s32 scaled   = arg3 * abs_diff;
    s32 result   = scaled >> arg2;

    if (result < arg4) {
        result = arg4;
    }
    if (result > arg5) {
        result = arg5;
    }

    return (current > target) ? -result : result;
}

static s32 EasyFade_Initialize(Fader* fader) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x800);
    fader[DISPLAY_MAIN].currentBrightness = g_DisplaySettings.controls[DISPLAY_MAIN].brightness * 0x1000;
    fader[DISPLAY_SUB].currentBrightness  = g_DisplaySettings.controls[DISPLAY_SUB].brightness * 0x1000;
    return 1;
}

static void EasyFade_UpdateLinear(DisplayEngine engine, Fader* fader) {
    if ((fader->currentBrightness >> 0xC) == (fader->targetBrightness >> 0xC)) {
        return;
    }

    if (fader->currentBrightness < fader->targetBrightness) {
        fader->currentBrightness += fader->rate;
    } else {
        fader->currentBrightness -= fader->rate;
    }

    s32 brightness = fader->currentBrightness >> 0xC;

    if (brightness > 0x10) {
        brightness = 0x10;
    } else if (brightness < -0x10) {
        brightness = -0x10;
    }

    g_DisplaySettings.controls[engine].brightness = brightness;
}

static void EasyFade_UpdateInterpolated(DisplayEngine engine, Fader* fader) {
    if (fader->framesRemaining != 0) {
        func_02026590(&fader->currentBrightness, fader->targetBrightness, fader->framesRemaining);
        fader->framesRemaining--;

        s32 brightness = fader->currentBrightness >> 0xC;

        if (brightness > 0x10) {
            brightness = 0x10;
        } else if (brightness < -0x10) {
            brightness = -0x10;
        }
        g_DisplaySettings.controls[engine].brightness = brightness;
    }
}

static void EasyFade_UpdateSmooth(DisplayEngine engine, Fader* fader) {
    if (fader->currentBrightness >> 0xC != fader->targetBrightness >> 0xC) {
        s32 newBrightness =
            fader->currentBrightness + EasyFade_CalculateInterpolation(fader->currentBrightness, fader->targetBrightness, 2, 1,
                                                                       fader->rate, fader->rate << 2);

        s32 brightness           = newBrightness >> 0xC;
        fader->currentBrightness = newBrightness;
        if (brightness > 0x10) {
            brightness = 0x10;
        } else if (brightness < -0x10) {
            brightness = -0x10;
        }
        g_DisplaySettings.controls[engine].brightness = brightness;
    }
}

static void EasyFade_UpdateInstant(DisplayEngine engine, Fader* fader) {
    s32 brightness = (s32)fader->targetBrightness >> 0xC;

    if (brightness > 0x10) {
        brightness = 0x10;
    } else if (brightness < -0x10) {
        brightness = -0x10;
    }

    g_DisplaySettings.controls[engine].brightness = brightness;

    fader->currentBrightness = fader->targetBrightness;
}

const struct FaderUpdateDispatch {
    void (*entries[4])(DisplayEngine, Fader*);
} FaderUpdateDispatchFuncs = {
    {EasyFade_UpdateLinear, EasyFade_UpdateInterpolated, EasyFade_UpdateSmooth, EasyFade_UpdateInstant}
};

static s32 EasyFade_Update(Fader* fader) {
    const struct FaderUpdateDispatch funcTable = FaderUpdateDispatchFuncs;
    funcTable.entries[fader[DISPLAY_MAIN].mode](0, &fader[DISPLAY_MAIN]);
    funcTable.entries[fader[DISPLAY_SUB].mode](1, &fader[DISPLAY_SUB]);
    return 1;
}

static s32 func_020264e0() {
    return 1;
}

static s32 func_020264e8() {
    return 1;
}

const struct FaderTaskHandlerDispatch {
    s32 (*entries[4])();
} FaderTaskHandlerDispatchFuncs = {
    {EasyFade_Initialize, EasyFade_Update, func_020264e0, func_020264e8}
};

static s32 EasyFade_TaskHandler(TaskPool* pool, Task* task, void* arg2, s32 arg3) {
    const struct FaderTaskHandlerDispatch funcTable = FaderTaskHandlerDispatchFuncs;
    return funcTable.entries[arg3](&gFaders);
}
