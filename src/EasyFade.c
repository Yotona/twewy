#include "EasyFade.h"
#include "Display.h"

static s32 EasyFade_RunTask(TaskPool* pool, Task* task, void* arg2, s32 arg3);

const TaskHandle Task_EasyFade = {"EasyFade", EasyFade_RunTask, 0};

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

BOOL EasyFade_IsFading(void) {
    Fader* faderMain = gFaders;
    Fader* faderSub  = gFaders;

    BOOL val = TRUE;
    if (F2I(faderMain[DISPLAY_MAIN].currentBrightness) != F2I(faderMain[DISPLAY_MAIN].targetBrightness)) {
        return TRUE;
    } else if (F2I(faderSub[DISPLAY_SUB].currentBrightness) == F2I(faderSub[DISPLAY_SUB].targetBrightness)) {
        val = FALSE;
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
    {
     EasyFade_UpdateLinear, EasyFade_UpdateSmooth,
     EasyFade_UpdateInstant, EasyFade_UpdateInterpolated,
     }
};

static s32 EasyFade_Update(Fader* fader) {
    const struct FaderUpdateDispatch funcTable = FaderUpdateDispatchFuncs;
    funcTable.entries[fader[DISPLAY_MAIN].mode](0, &fader[DISPLAY_MAIN]);
    funcTable.entries[fader[DISPLAY_SUB].mode](1, &fader[DISPLAY_SUB]);
    return 1;
}

static s32 EasyFade_Render(Fader* fader) {
    return 1;
}

static s32 EasyFade_Release(Fader* fader) {
    return 1;
}

// Identical behavior to TaskStages, except only parameter is Fader*
typedef union {
    struct {
        s32 (*initialize)(Fader*);
        s32 (*update)(Fader*);
        s32 (*render)(Fader*);
        s32 (*cleanup)(Fader*);
    };
    s32 (*iter[4])(Fader*);
} FaderStages;

static s32 EasyFade_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    FaderStages stages = {
        .initialize = EasyFade_Initialize,
        .update     = EasyFade_Update,
        .render     = EasyFade_Render,
        .cleanup    = EasyFade_Release,
    };
    return stages.iter[stage](gFaders);
}
