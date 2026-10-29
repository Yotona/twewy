#ifndef COMBAT_CORE_COMBATBGSCROLL_H
#define COMBAT_CORE_COMBATBGSCROLL_H

#include "Display.h"

/// Moves BG `layer` of `engine` to (`posX`, `posY`); non-text layers also flag their affine block.
static inline void Ov003_ScrollBg(DisplayEngineState* engine, s32 layer, s32 posX, s32 posY) {
    switch (engine->bgSettings[layer].bgMode) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            engine->bgAffines[layer].unk_14 = 1;
            break;
    }
    engine->bgOffsets[layer].hOffset = posX;
    engine->bgOffsets[layer].vOffset = posY;
}

/// Same, addressing the engine by index.
static inline void Ov003_ScrollBgById(s32 engineId, s32 layer, s32 posX, s32 posY) {
    DisplayEngineState* engine = &g_DisplaySettings.engineState[engineId];

    Ov003_ScrollBg(engine, layer, posX, posY);
}

#endif
