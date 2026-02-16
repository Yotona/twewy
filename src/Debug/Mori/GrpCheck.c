#include "Debug/Mori/GrpCheck.h"
#include "BgResMgr.h"
#include "DatMgr.h"
#include "Display.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "System.h"
#include "common_data.h"

void GrpCheck_RegisterVBlank(void);
void GrpCheck_ClearVBlank(void);

GrpCheckUnk data_ov038_02086fe4[];
GrpCheckUnk data_ov038_02087004[];

struct {
    /* 0x0 */ const char*   sequence;
    /* 0x4 */ SpriteConfig* unk_04;
} data_ov038_020ac980 = {
    .sequence = "Seq_GrpCheck()",
    .unk_04   = NULL,
};

struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} data_ov038_020b7200;

u16 func_ov038_020824a0(void* arg0) {
    // Not yet implemented
}

void func_ov038_02082500(s32* arg0, void* arg1) {
    // Not yet implemented
}

u16 func_ov038_02082578(void* arg0) {
    // Not yet implemented
}

u16 func_ov038_02082594(void* arg0) {
    // Not yet implemented
}

s32 func_ov038_020825b0(void* arg0) {
    // Not yet implemented
}

s32 func_ov038_020825c8(void* arg0, s16 arg1, s16 arg2) {
    // Not yet implemented
}

s32 func_ov038_020826b4(void* arg0) {
    // Not yet implemented
}

void* func_ov038_020827e4(s32 arg0, s32 arg1, s32* arg2, s32 arg3, s32 arg4, u32 arg5) {
    switch (arg0) {
        case 0:
            return BgResMgr_AllocChar32(g_BgResourceManagers[arg1], arg2, arg3, arg4, arg5);
        case 1:
            return BgResMgr_AllocChar64(g_BgResourceManagers[arg1], arg2, arg3, arg4, arg5);
        case 2:
            return BgResMgr_AllocCharExtended(g_BgResourceManagers[arg1], arg2, arg3, arg4, arg5);
        default:
            return NULL;
    }
}

void func_ov038_0208287c(void* arg0, void* arg1) {
    // Not yet implemented
}

void func_ov038_02082e44(void* arg0) {
    // Not yet implemented
}

void func_ov038_02082ed0(void* arg0) {
    // Not yet implemented
}

void func_ov038_02082fc4(void* arg0, s32 arg1, s32 arg2) {
    // Not yet implemented
}

s32 func_ov038_02082fd0(GrpCheckUnk* arg0, GrpCheckUnk* arg1, s32 arg2) {
    // Not yet implemented
}

u32 func_ov038_02083010(GrpCheckState* state) {
    u32 current = state->unk_1165C;

    while (current < 294) {
        GrpCheckUnk* address = &data_ov038_02086fe4[current];
        if (func_ov038_02082fd0(address, &data_ov038_02086fe4[state->unk_1165C], 0) != 0) {
            return current;
        }
        current++;
    }

    return 0;
}

s32 func_ov038_0208307c(void* arg0) {
    // Not yet implemented
}

s32 func_ov038_0208312c(void* arg0, s32 arg1) {
    // Not yet implemented
}

s32 func_ov038_020831a4(void* arg0, s32 arg1) {
    // Not yet implemented
}

s32 func_ov038_02083260(void* arg0) {
    // Not yet implemented
}

s32 func_ov038_02083288(void* arg0) {
    // Not yet implemented
}

s32 func_ov038_020832a8(GrpCheckState* state, s32 arg1) {
    return data_ov038_020ac980.unk_04[arg1].unk_28 != 0;
}

s32 func_ov038_020832d0(s32 arg0) {
    while (data_ov038_020ac980.unk_04[arg0].unk_28 != 1) {
        arg0--;
    }
    return arg0;
}

s32 func_ov038_02083300(s32 arg0) {
    if (data_ov038_020ac980.unk_04[arg0].unk_28 - 1 <= 1) {
        return arg0 + 1;
    }
    if (data_ov038_020ac980.unk_04[arg0].unk_28 != 3) {
        return -1;
    }
    return func_ov038_020832d0(arg0);
}

void func_ov038_02083340(void* arg0) {
    // Not yet implemented
}

void func_ov038_020833a4(GrpCheckState* state) {
    func_02025e30(&state->unk_11BA4);
}

void GrpCheck_PrintRowIndicator(GrpCheckState* state) {
    func_02010b84(&state->textObj, 0, 0, 8, 176);
    Text_RenderToScreen(&state->textObj, 0, state->currentRow * 8, ">");
}

void GrpCheck_PrintSpritePath(GrpCheckState* state) {
    GrpCheckBigUnk* temp_r4 = &state->unk_116C0[state->unk_11B80];

    func_02010b84(&state->textObj, 8, 0, 0x100, 0x20);
    func_02010b84(&state->textObj, 8, 0x20, 0xA8, 8);
    Text_RenderToScreen(&state->textObj, 8, 0, func_02006930("%s", data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_00));
    Text_RenderToScreen(&state->textObj, 8, 8, func_02006930("└%s", data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_04));
    Text_RenderToScreen(&state->textObj, 8, 0x10, func_02006930("　└%s", data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_08));
    Text_RenderToScreen(&state->textObj, 8, 0x18, func_02006930("%s", data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_24));
    Text_RenderToScreen(&state->textObj, 8, 0x20, func_02006930("anime:%d", temp_r4->sprite.animIndex));
}

// TODO: Fix encoding errors, contains multiple characters with 0x5c escapes which are not properly handled
const char data_ov038_020b6fe0[]; // "ソフトウェアスプライトを表示できません"

void GrpCheck_PrintSpriteDetails(GrpCheckState* state) {
    GrpCheckBigUnk* temp_r4 = &state->unk_116C0[state->unk_11B80];
    func_02010b84(&state->textObj, 8, 0x28, 0xA8, 0x58);

    if ((data_ov038_020b7200.unk_4 != 2) && (data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_0C == 2)) {
        Text_RenderToScreen(&state->textObj, 8, 0x28, func_02006930(&data_ov038_020b6fe0));
        return;
    }

    u16 useChr      = func_ov038_02082594(temp_r4);
    u16 useAtr      = func_ov038_02082578(temp_r4);
    s32 useLineChar = func_ov038_020826b4(temp_r4);

    s32 var_r1 = state->maxChr;
    if (var_r1 < useChr) {
        var_r1 = useChr;
    }
    state->maxChr = var_r1;

    s32 var_r1_2 = state->maxAtr;
    if (var_r1_2 < useAtr) {
        var_r1_2 = useAtr;
    }
    state->maxAtr = var_r1_2;

    s32 var_r1_3 = state->maxLineChr;
    if (var_r1_3 < useLineChar) {
        var_r1_3 = useLineChar;
    }
    state->maxLineChr = var_r1_3;

    Text_RenderToScreen(&state->textObj, 8, 0x28, func_02006930("MaxAtr:%d", state->maxAtr));
    Text_RenderToScreen(&state->textObj, 8, 0x30, func_02006930("UseAtr:%d", useAtr));
    Text_RenderToScreen(&state->textObj, 8, 0x38, func_02006930("MaxChr:%d", state->maxChr));
    Text_RenderToScreen(&state->textObj, 8, 0x40, func_02006930("UseChr:%d", useChr));
    Text_RenderToScreen(&state->textObj, 8, 0x48, func_02006930("MaxLineChr:%d", state->maxLineChr));
    Text_RenderToScreen(&state->textObj, 8, 0x50, func_02006930("UseLineChr:%d", useLineChar));
    Text_RenderToScreen(&state->textObj, 8, 0x58, func_02006930("pic:%d", temp_r4->sprite.unk16 - 1));
    Text_RenderToScreen(&state->textObj, 8, 0x60,
                        func_02006930("aob:%d", temp_r4->sprite.currentFrame - temp_r4->sprite.loopFrame));
    Text_RenderToScreen(&state->textObj, 8, 0x68, func_02006930("frame:%d", temp_r4->sprite.frameTimer));
    Text_RenderToScreen(&state->textObj, 8, 0x70, func_02006930("cmd_ofs:%d", temp_r4->sprite.currentFrame));
    Text_RenderToScreen(&state->textObj, 8, 0x78, func_02006930("cmd_lop:%d", temp_r4->sprite.loopFrame));
}

void GrpCheck_PrintScale(GrpCheckState* state) {
    s32 raw        = state->unk_116C0[state->unk_11B80].unk_48;
    s32 integer    = F2I(raw);
    s32 fractional = F2I(raw * 100) - (integer * 100);

    func_02010b84(&state->textObj, 8, 0x80, 0xa8, 8);

    Text_RenderToScreen(&state->textObj, 8, 128, func_02006930("scale:%d.%02d", integer, fractional));
}

void GrpCheck_PrintColorChannels(GrpCheckState* state) {
    func_02010b84(&state->textObj, 8, 136, 168, 24);
    Text_RenderToScreen(&state->textObj, 8, 136, func_02006930("BG R:%d", state->bgR));
    Text_RenderToScreen(&state->textObj, 8, 144, func_02006930("BG G:%d", state->bgG));
    Text_RenderToScreen(&state->textObj, 8, 152, func_02006930("BG B:%d", state->bgB));
}

void GrpCheck_PrintScreenPos(GrpCheckState* state) {
    func_02010b84(&state->textObj, 8, 160, 256, 24);
    Text_RenderToScreen(&state->textObj, 8, 160, func_02006930("%s", data_ov038_02086fe4[state->unk_1165C].unk_00));
    Text_RenderToScreen(&state->textObj, 8, 168, func_02006930("└%s", data_ov038_02087004[state->unk_1165C].unk_00));
    Text_RenderToScreen(&state->textObj, 8, 176,
                        func_02006930("SCREEN POS %d %d", F2I(state->screenPosX), F2I(state->screenPosY)));
}

void GrpCheck_PrintWarnings(GrpCheckState* state) {
    s32 yOffset = 23;
    func_02010b84(&state->textObj, 8, 184, 256, 16);

    if (state->isVramLimitReached) {
        yOffset++;
        // "VRAM limit reached. Please delete objects by pressing X or Touch + X."
        Text_RenderToScreen(&state->textObj, 8, 184, "VRAM上限。Xまたはタッチ+XでOBJを削除してください。");
    }

    if (state->isPaused != FALSE) {
        Text_RenderToScreen(&state->textObj, 8, yOffset * 8, "pause");
    }
}

void GrpCheck_PrintControls(GrpCheckState* state) {
    Text_RenderToScreen(&state->textObj, 176, 40, "←→：アニメへんこう");  // "←→: Change animation"
    Text_RenderToScreen(&state->textObj, 176, 48, "↑↓：カーソ\ル上下");   // "↑↓: Move cursor up/down"
    Text_RenderToScreen(&state->textObj, 176, 56, "ＬＲ：あたいへんこう");  // "LR: Change value"
    Text_RenderToScreen(&state->textObj, 176, 64, "タッチ：いどう");        // "Touch: Move"
    Text_RenderToScreen(&state->textObj, 176, 72, "タッチ＋Ｘ：さくじょ");  // "Touch + X: Delete"
    Text_RenderToScreen(&state->textObj, 176, 80, "Ｘ：すべてさくじょ");    // "X: Delete all"
    Text_RenderToScreen(&state->textObj, 176, 88, "Ａ：コマおくり");        // "A: Step forward"
    Text_RenderToScreen(&state->textObj, 176, 96, "　　アニメさいかい");    // "  Restart animation"
    Text_RenderToScreen(&state->textObj, 176, 104, "Ｂ：アニメひひょうじ"); // "B: Display animation"
    Text_RenderToScreen(&state->textObj, 176, 112, "ＳＴＡＲＴ：ポーズ");   // "START: Pause"
    Text_RenderToScreen(&state->textObj, 176, 120, "ＳＥＬＥＣＴ：おわる"); // "SELECT: Exit"
}

s32 func_ov038_02083c64(void* arg0) {
    // Not yet implemented
}

void* func_ov038_02083c94(void* arg0, s32 arg2) {
    // Not yet implemented
}

struct _struct_data_ov038_02085c6c_0x8* func_ov038_02083de0(s32 arg0) {
    // Not yet implemented
}

void func_ov038_02083e28(void* arg0, u16 arg1, void* arg2) {
    // Not yet implemented
}

void func_ov038_02083ff8(GrpCheckState* state) {
    state->maxChr     = 0;
    state->maxAtr     = 0;
    state->maxLineChr = 0;
    state->unk_116B4  = state->unk_11B8C;
    state->unk_116BC  = 0x1000;
    if (data_ov038_020b7200.unk_4 != 2) {
        func_ov038_02083e28(state, 1, &state->unk_11674);
    }
    GrpCheck_PrintScale(state);
}

void func_ov038_02084054(GrpCheckState* state) {
    // Not yet implemented
}

void GrpCheck_DestroySprite(GrpCheckState* state, s32 index) {
    Sprite_Destroy(&state->unk_116C0[index].sprite);
    state->unk_116C0[index].unk_44 = 0;
}

void GrpCheck_DestroyAllSprites(GrpCheckState* state) {
    if (state->hasSprite) {
        Sprite_Destroy(&state->unk_11674);
    }

    for (s32 i = 0; i < 16; i++) {
        if (state->unk_116C0[i].unk_44 != 0) {
            GrpCheck_DestroySprite(state, i);
        }
    }
}

void GrpCheck_Delete(GrpCheckState* state) {
    // Not yet implemented
}

void func_ov038_0208424c(void* arg0) {
    // Not yet implemented
}

void func_ov038_020843a8(void* arg0) {
    // Not yet implemented
}

void func_ov038_020843bc(GrpCheckState* state) {
    // Not yet implemented
}

void func_ov038_020843fc(GrpCheckState* state) {
    // Not yet implemented
}

void func_ov038_02084434(GrpCheckState* state) {
    // Not yet implemented
}

void func_ov038_02084498(GrpCheckState* state) {
    // Not yet implemented
}

void func_ov038_020844c0(GrpCheckState* state) {
    // Not yet implemented
}

s32 func_ov038_020844e8(void* arg0, s16 arg1, s16 arg2) {
    // Not yet implemented
}

void func_ov038_0208454c(void* arg0) {
    // Not yet implemented
}

s32 func_ov038_020846f4(void* arg0) {
    // Not yet implemented
}

s32 func_ov038_02084734(void* arg0) {
    // Not yet implemented
}

void func_ov038_0208476c(GrpCheckState* state) {
    // Not yet implemented
}

void func_ov038_02084874(GrpCheckState* state) {
    if (state->hasSprite) {
        Sprite_ChangeAnimation(&state->unk_11674, state->unk_11674.animData, state->unk_11B90, state->unk_11674.unk1C);
    }

    if (func_ov038_020846f4(state) != 0) {
        Sprite* sprite = &state->unk_116C0[state->unk_11B80].sprite;
        Sprite_ChangeAnimation(sprite, sprite->animData, state->unk_11B90, sprite->unk1C);
    }

    GrpCheck_PrintSpritePath(state);
    state->updateForced = TRUE;
    state->maxChr       = 0;
    state->maxAtr       = 0;
    state->maxLineChr   = 0;
}

void func_ov038_02084910(GrpCheckState* state) {
    state->unk_11B8C = func_ov038_0208312c(state, state->currentRow);
    func_ov038_0208476c(state);
    state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animIndex;
    func_ov038_02084874(state);
}

void func_ov038_02084968(GrpCheckState* state) {
    state->unk_11B8C = func_ov038_020831a4(state, state->currentRow);
    func_ov038_0208476c(state);
    state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animIndex;
    func_ov038_02084874(state);
}

void func_ov038_020849c0(GrpCheckState* state) {
    if (func_ov038_020832a8(state, state->unk_11B8C) != 0) {
        state->unk_11B8C = func_ov038_020832d0(state->unk_11B8C);
    }
    func_ov038_0208476c(state);
    state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animIndex;
    func_ov038_02084874(state);
}

void func_ov038_02084a2c(GrpCheckState* state) {
    state->unk_11B8C = func_ov038_02083288(state);

    if (func_ov038_020832a8(state, state->unk_11B8C)) {
        func_ov038_020849c0(state);
    } else {
        func_ov038_0208476c(state);
        state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animData[0] - 1;
        func_ov038_02084874(state);
    }
}

void func_ov038_02084a9c(GrpCheckState* state) {
    while (data_ov038_020ac980.unk_04[state->unk_11B8C].unk_28 != 3) {
        state->unk_11B8C = func_ov038_02083260(state);
    }

    state->unk_11B8C = func_ov038_02083260(state);
    func_ov038_0208476c(state);
    state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animIndex;
    func_ov038_02084874(state);
}

void func_ov038_02084b3c(GrpCheckState* state) {
    state->unk_11B8C = func_ov038_020832d0(state->unk_11B8C);
    func_ov038_02084a2c(state);
}

void func_ov038_02084b64(GrpCheckState* state) {
    // Not yet implemented
}

void func_ov038_02084bf0(GrpCheckState* state) {
    state->unk_11B90--;
    if (state->unk_11B90 < 1) {
        func_ov038_02084a2c(state);
        return;
    }
    if ((func_ov038_020846f4(state) != 0) && (func_ov038_02084734(state) != 0)) {
        func_ov038_02084054(state);
    }
    func_ov038_02084874(state);
}

void GrpCheck_IncreaseValue(GrpCheckState* state) {
    if (func_ov038_020832a8(state, state->unk_11B8C) != 0) {
        func_ov038_02084a9c(state);
    } else {
        func_ov038_02084b64(state);
    }
}

void GrpCheck_DecreaseValue(GrpCheckState* state) {
    if (func_ov038_020832a8(state, state->unk_11B8C) != 0) {
        func_ov038_02084b3c(state);
    } else {
        func_ov038_02084bf0(state);
    }
}

void func_ov038_02084cb0(GrpCheckState* state, Sprite* arg1) {
    // Not yet implemented
}

void GrpCheck_RenderAllSprites(GrpCheckState* state) {
    for (s32 i = 0; i < 16; i++) {
        if (state->unk_116C0[i].unk_44 != 0) {
            if ((state->isPaused == FALSE) || (state->updateForced)) {
                func_ov038_02084cb0(state, &state->unk_116C0[i].sprite);
            }
            if ((i == state->unk_11B80) && (state->isAnimDisplayed == FALSE)) {

            } else {
                Sprite_Render(&state->unk_116C0[i].sprite);
            }
        }
    }

    if (state->hasSprite) {
        if ((state->isPaused == FALSE) || (state->updateForced)) {
            func_ov038_02084cb0(state, &state->unk_11674);
            state->unk_11B8C = state->unk_116B4;
            GrpCheck_PrintSpritePath(state);
        }
        if (state->isAnimDisplayed) {
            Sprite_Render(&state->unk_11674);
        }
    }
}

void func_ov038_02084e48(GrpCheckState* state) {
    if (state->isPaused) {
        state->updateForced = TRUE;
    } else {
        func_ov038_020849c0(state);
    }
}

void GrpCheck_ToggleAnimDisplay(GrpCheckState* state) {
    state->isAnimDisplayed = !state->isAnimDisplayed;
}

void GrpCheck_TogglePause(GrpCheckState* state) {
    state->isPaused = !state->isPaused;
}

void GrpCheck_Exit(GrpCheckState* state) {
    MainOvlDisp_NextProcessStage();
}

void GrpCheck_MoveCursorUp(GrpCheckState* state) {
    state->currentRow--;
    if (state->currentRow < 0) {
        state->currentRow = 21;
    }

    if ((state->currentRow > 2) && (state->currentRow < 16)) {
        state->currentRow = 2;
    }
    GrpCheck_PrintRowIndicator(state);
}

void GrpCheck_MoveCursorDown(GrpCheckState* state) {
    state->currentRow++;
    if (state->currentRow > 21) {
        state->currentRow = 0;
    }

    if ((state->currentRow > 2) && (state->currentRow < 16)) {
        state->currentRow = 16;
    }
    GrpCheck_PrintRowIndicator(state);
}

void GrpCheck_IncreaseScale(GrpCheckState* state) {
    state->unk_116BC += 41;
    state->unk_116C0[state->unk_11B80].unk_48 += 41;

    if (state->unk_116BC >= 0x2000) {
        state->unk_116BC                          = 0x2000;
        state->unk_116C0[state->unk_11B80].unk_48 = 0x2000;
    }
    GrpCheck_PrintScale(state);
}

void GrpCheck_DecreaseScale(GrpCheckState* state) {
    state->unk_116BC -= 41;
    state->unk_116C0[state->unk_11B80].unk_48 -= 41;

    if (state->unk_116BC <= 0) {
        state->unk_116BC                          = 41;
        state->unk_116C0[state->unk_11B80].unk_48 = 41;
    }
    GrpCheck_PrintScale(state);
}

void GrpCheck_AdjustBGColorChannel(GrpCheckState* state, RGBAChannel channel, s32 delta) {
    s32* value;

    switch (channel) {
        case RGB_RED:
            value = &state->bgR;
            break;
        case RGB_GREEN:
            value = &state->bgG;
            break;
        case RGB_BLUE:
            value = &state->bgB;
            break;
        default:
            return;
    }
    *value = (*value + delta) & 0x1F;

    data_02066eec = (state->bgA << 15) | (state->bgB << 10) | (state->bgG << 5) | state->bgR;
    GrpCheck_PrintColorChannels(state);
}

void GrpCheck_IncreaseBGRed(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_RED, 1);
}

void GrpCheck_DecreaseBGRed(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_RED, -1);
}

void GrpCheck_IncreaseBGGreen(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_GREEN, 1);
}

void GrpCheck_DecreaseBGGreen(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_GREEN, -1);
}

void GrpCheck_IncreaseBGBlue(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_BLUE, 1);
}

void GrpCheck_DecreaseBGBlue(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_BLUE, -1);
}

typedef struct {
    /* 0x0 */ u32 menuOption;
    /* 0x4 */ void (*decCallback)(GrpCheckState*);
    /* 0x8 */ void (*incCallback)(GrpCheckState*);
} GrpCheckScalarCallback;

typedef struct {
    /* 0x0 */ u16 button;
    /* 0x4 */ void (*callback)(GrpCheckState*);
} GrpCheckInputCallback;

const GrpCheckScalarCallback data_ov038_02085c24[6] = {
    {16,   GrpCheck_DecreaseScale,   GrpCheck_IncreaseScale},
    {17,   GrpCheck_DecreaseBGRed,   GrpCheck_IncreaseBGRed},
    {18, GrpCheck_DecreaseBGGreen, GrpCheck_IncreaseBGGreen},
    {19,  GrpCheck_DecreaseBGBlue,  GrpCheck_IncreaseBGBlue},
    {20,      func_ov038_020844c0,      func_ov038_02084498},
    {21,      func_ov038_020843fc,      func_ov038_020843bc},
};

const GrpCheckInputCallback data_ov038_02085b70[4] = {
    {     INPUT_BUTTON_B, GrpCheck_ToggleAnimDisplay},
    {     INPUT_BUTTON_X,            GrpCheck_Delete},
    { INPUT_BUTTON_START,       GrpCheck_TogglePause},
    {INPUT_BUTTON_SELECT,              GrpCheck_Exit},
};

const GrpCheckInputCallback data_ov038_02085b90[5] = {
    {   INPUT_BUTTON_UP,   GrpCheck_MoveCursorUp},
    { INPUT_BUTTON_DOWN, GrpCheck_MoveCursorDown},
    { INPUT_BUTTON_LEFT,  GrpCheck_DecreaseValue},
    {INPUT_BUTTON_RIGHT,  GrpCheck_IncreaseValue},
    {    INPUT_BUTTON_A,     func_ov038_02084e48},
};

void GrpCheck_ProcessUserInput(GrpCheckState* state) {
    u32 i;

    func_ov038_0208454c(state);

    for (i = 0; i < 5; i++) {
        if (SysControl.buttonState.holdButtons & data_ov038_02085b90[i].button) {
            data_ov038_02085b90[i].callback(state);
        }
    }

    for (i = 0; i < 4; i++) {
        if (SysControl.buttonState.pressedButtons & data_ov038_02085b70[i].button) {
            data_ov038_02085b70[i].callback(state);
        }
    }

    for (i = 0; i < 6; i++) {
        GrpCheckScalarCallback* scalarCb = &data_ov038_02085c24[i];
        if (state->currentRow == scalarCb->menuOption) {
            if (SysControl.buttonState.holdButtons == INPUT_BUTTON_L) {
                scalarCb->decCallback(state);
            }
            if (SysControl.buttonState.holdButtons == INPUT_BUTTON_R) {
                scalarCb->incCallback(state);
            }
        }
    }

    if (state->currentRow >= 0 && state->currentRow < 3) {
        if (SysControl.buttonState.holdButtons == INPUT_BUTTON_L) {
            func_ov038_02084968(state);
        }
        if (SysControl.buttonState.holdButtons == INPUT_BUTTON_R) {
            func_ov038_02084910(state);
        }
    }
    func_ov038_02084434(state);
    GrpCheck_RenderAllSprites(state);

    // Reset manual animation advance flag after processing input, so that it only takes effect for one frame
    state->updateForced = FALSE;

    GrpCheck_PrintSpriteDetails(state);
    GrpCheck_PrintWarnings(state);
}

void func_ov038_02085268(GrpCheckState* state) {
    // Not yet implemented
}

void GrpCheck_InitOverlay(GrpCheckState* state) {
    if (state == NULL) {
        const char* sequence = data_ov038_020ac980.sequence;

        state = Mem_AllocHeapTail(&gDebugHeap, sizeof(GrpCheckState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        MainOvlDisp_SetCbArg(state);
    }
    MI_CpuFill(0, state, Mem_GetBlockSize(&gDebugHeap, state));
    GrpCheck_RegisterVBlank();

    u16 stack[4];
    func_0203ff44(&stack);
    func_0203ffd8(&stack);

    func_0200d8f0();
    state->unk_00000 = DatMgr_AllocateSlot();
    state->unk_00004 = DatMgr_AllocateSlot();
    state->unk_1158C = func_0200cef0(&state->unk_0000C);
    func_ov038_02085268(state);
    func_ov038_02083340(state);
    func_ov038_0208424c(state);
    func_ov038_02083ff8(state);
    func_ov038_02084054(state);
    GrpCheck_PrintRowIndicator(state);
    GrpCheck_PrintSpritePath(state);
    GrpCheck_PrintSpriteDetails(state);
    GrpCheck_PrintScale(state);
    GrpCheck_PrintColorChannels(state);
    GrpCheck_PrintScreenPos(state);
    GrpCheck_PrintControls(state);
    data_02066aec = 0;
    data_02066eec = (state->bgA << 15) | (state->bgB << 10) | (state->bgG << 5) | state->bgR;
    DebugOvlDisp_Init();
    MainOvlDisp_NextProcessStage();
}

void GrpCheck_InitOverlayForHwSprites(GrpCheckState* state) {
    data_ov038_020b7200.unk_4 = 1;
    GrpCheck_InitOverlay(state);
}

void GrpCheck_InitOverlayForSwSprites(GrpCheckState* state) {
    data_ov038_020b7200.unk_4 = 2;
    GrpCheck_InitOverlay(state);
}

void GrpCheck_RunOverlay(GrpCheckState* state) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    if (data_ov038_020b7200.unk_4 == 2) {
        func_02003c4c();
        func_02002890(&data_02069804, 0);
    }
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    GrpCheck_ProcessUserInput(state);

    if (data_ov038_02086fe4[state->unk_1165C].unk_18 != 0) {
        func_0200d90c();
    }

    DebugOvlDisp_Run();
    func_02003c68();
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    if (data_ov038_020b7200.unk_4 == 2) {
        func_0200bf60(data_0206b3cc[2], 0);
        func_02001b44(2, 0, &data_020672ec, 0x400);
    }
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
}

void GrpCheck_TerminateOverlay(GrpCheckState* state) {
    OverlayTag tag;

    GrpCheck_ClearVBlank();
    func_ov038_020833a4(state);
    GrpCheck_DestroyAllSprites(state);
    func_0200cfe8(state->unk_1158C);
    DatMgr_ClearSlot(state->unk_00004);
    DatMgr_ClearSlot(state->unk_00000);
    DatMgr_ReleaseData(state->unk_00008);
    Mem_Free(&gDebugHeap, state);
    MainOvlDisp_SetProcessStage(PROCESS_STAGE_EXIT);
    MainOvlDisp_Pop(&tag);
}

static const OverlayProcess OvlProc_GrpCheck_HwSprites = {
    .init = GrpCheck_InitOverlayForHwSprites,
    .main = GrpCheck_RunOverlay,
    .exit = GrpCheck_TerminateOverlay,
};

static const OverlayProcess OvlProc_GrpCheck_SwSprites = {
    .init = GrpCheck_InitOverlayForSwSprites,
    .main = GrpCheck_RunOverlay,
    .exit = GrpCheck_TerminateOverlay,
};

void ProcessOverlay_GrpCheck_HwSprites(void* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        GrpCheck_TerminateOverlay(state);
    } else {
        OvlProc_GrpCheck_HwSprites.funcs[stage](state);
    }
}

void ProcessOverlay_GrpCheck_SwSprites(void* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        GrpCheck_TerminateOverlay(state);
    } else {
        OvlProc_GrpCheck_SwSprites.funcs[stage](state);
    }
}

void func_ov038_020856f4(void) {
    Interrupts_Init();
    func_0200434c();
    DMA_Init(0x100);
    GX_Init();
    G3X_Init();
    G3X_InitMtxStack();
    GX_DispOn();
    REG_DISPCNT_SUB |= 0x10000;
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForBgExtPltt(0x20);
    GX_SetBankForObjExtPltt(0x40);
    GX_SetBankForSubBg(4);
    if (data_ov038_020b7200.unk_4 == 2) {
        GX_SetBankForTex(8);
        GX_SetBankForTexPltt(0x10);
    } else {
        GX_SetBankForSubObj(8);
        GX_SetBankForSubObjExtPltt(0x100);
    }
    func_02003ad0();
    REG_POWER_CNT &= ~0x8000;

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    DisplayBGSettings* mainBg1 = Display_GetBG1Settings(DISPLAY_MAIN);
    mainBg1->bgMode            = 0;
    mainBg1->screenSizeText    = 1;
    mainBg1->colorMode         = 0;
    mainBg1->screenBase        = 0;
    mainBg1->charBase          = 1;
    mainBg1->extPlttSlot       = 0;
    REG_BG1CNT                 = REG_BG1CNT & 0x43 | 0x4004;

    DisplayBGSettings* mainBg2 = Display_GetBG2Settings(DISPLAY_MAIN);
    mainBg2->bgMode            = 0;
    mainBg2->screenSizeText    = 1;
    mainBg2->colorMode         = 0;
    mainBg2->screenBase        = 2;
    mainBg2->charBase          = 3;
    mainBg2->extPlttSlot       = 1;
    REG_BG2CNT                 = REG_BG2CNT & 0x43 | 0x420c;

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers      = 23;
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode       = GX_BGMODE_0;

    GXs_SetGraphicsMode(0);

    DisplayBGSettings* subBg0 = Display_GetBG0Settings(DISPLAY_SUB);
    subBg0->bgMode            = 0;
    subBg0->screenSizeText    = 0;
    subBg0->colorMode         = 0;
    subBg0->screenBase        = 0;
    subBg0->charBase          = 1;
    subBg0->extPlttSlot       = 0;
    REG_BG0CNT_SUB            = REG_BG0CNT_SUB & 0x43 | 4;

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 1;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    g_DisplaySettings.controls[DISPLAY_SUB].layers      = 17;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;

    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    func_0200270c(0, 2);
    func_02002890(&data_02069804, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_02001c34(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    func_02001c34(&data_02066eec, &data_0205a128, 0, 0x200, 1);
}

void GrpCheck_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 0x400);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

void GrpCheck_RegisterVBlank(void) {
    func_ov038_020856f4();
    Interrupts_RegisterVBlankCallback(GrpCheck_VBlank, 1);
}

void GrpCheck_ClearVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}
