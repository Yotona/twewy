#include "Engine/Core/System.h"
#include "Engine/Text.h"
#include "Otosu/OtosuGame.h"

static void OtuEditor_DrawKnockback(TinPinSlammer_Scene* scene);
static void OtuScene_Editor_Enter(TinPinSlammer_Scene* scene);
static void OtuScene_Editor_Exit(TinPinSlammer_Scene* scene);
static void OtuScene_Editor_Run(TinPinSlammer_Scene* scene);

typedef struct {
    /* 0x0 */ s32 digit;      // the selected digit of the row; negative while moving between rows
    /* 0x4 */ s32 drawnDigit; // `digit` when the cursor was last drawn
    /* 0x8 */ s32 row;        // the selected row
    /* 0xC */ s32 drawnRow;   // `row` when the cursor was last drawn
} OtuEditCursor;              // Size: 0x10

typedef struct {
    /* 0x00 */ s32           page;
    /* 0x04 */ OtuEditCursor cursor[3]; // one per page
    /* 0x34 */ s32           blink;     // frame counter; bit 5 blanks the selected digit
    /* 0x38 */ s32           pinId;     // the pin page 3 edits
} OtuEditor;                            // Size: 0x3C

typedef struct {
    /* 0x0 */ char thousands;
    /* 0x1 */ char point;
    /* 0x2 */ char hundreds;
    /* 0x3 */ char tens;
    /* 0x4 */ char ones;
    /* 0x5 */ char space;
} OtuTextDigits; // Size: 0x6

static const s32 OtuEditor_PinTuneColumns[2]       = {0, 1};
static const s32 OtuEditor_StunFramesColumns[3]    = {0, 1, 2};
static const s32 OtuEditor_PinIdColumns[3]         = {0, 1, 2};
static const s32 OtuEditor_StunColumns[4]          = {0, 2, 3, 4};
static const s32 OtuEditor_PinFrictionColumns[4]   = {0, 2, 3, 4};
static const s32 OtuEditor_KnockbackColumns[4]     = {0, 2, 3, 4};
static const s32 OtuEditor_DebrisDecelColumns[4]   = {0, 2, 3, 4};
static const s32 OtuEditor_MeteoColumns[6]         = {0, 1, 2, 4, 5, 6};
static const s32 OtuEditor_NeedleColumns[6]        = {0, 1, 2, 4, 5, 6};
static const s32 OtuEditor_SpecialUsesColumns[8]   = {0, 1, 3, 4, 6, 7, 9, 10};
static const s32 OtuEditor_HandColumns[8]          = {0, 2, 3, 4, 6, 8, 9, 10};
static const s32 OtuEditor_MatchColumns[9]         = {0, 1, 3, 5, 6, 7, 9, 11, 13};
static const s32 OtuEditor_SpinColumns[10]         = {0, 1, 2, 3, 4, 6, 7, 8, 9, 10};
static const s32 OtuEditor_HammerColumns[12]       = {0, 2, 3, 4, 6, 8, 9, 10, 12, 13, 14, 16};
static const s32 OtuEditor_PanelsColumns[12]       = {0, 2, 3, 4, 6, 8, 9, 10, 12, 14, 15, 16};
static const s32 OtuEditor_DebrisSpeedColumns[12]  = {0, 2, 3, 4, 6, 8, 9, 10, 12, 14, 15, 16};
static const s32 OtuEditor_DebrisLaunchColumns[16] = {0, 2, 3, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 22};
static const s32 OtuEditor_FrictionColumns[16]     = {0, 2, 3, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 22};
static const s32 OtuEditor_TuneColumns[16]         = {0, 2, 3, 4, 6, 8, 9, 10, 12, 14, 15, 16, 18, 20, 21, 22};
static const s32 OtuEditor_DeckColumns[18]         = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14, 16, 17, 18, 20, 21, 22};

static OtuStageHandler OtuScene_EditorSteps[1] = {
    OtuScene_Editor_Run,
};

OtuSceneStage OtuScene_EditorStage = {
    OtuScene_Editor_Enter,
    OtuScene_Editor_Exit,
    OtuScene_EditorSteps,
    sizeof(OtuEditor),
};

static void OtuEditor_DrawDeck(TinPinSlammer_Scene* scene, s32 which);

static void OtuEditor_FormatInt(char* buf, s32 value, s32 count) {
    s32 i;

    if (value < 0) {
        value = -value;
    }

    for (i = count - 1; i >= 0; i--) {
        buf[i] = '0' + (value % 10);
        value /= 10;
    }
    buf[count] = ' ';
}

static s32 OtuEditor_StepInt(s32 base, s32 offset, s32 digits, s32 lo, s32 hi) {
    s32 i;
    s32 scale = 1;

    for (i = 0; i < digits; i++) {
        scale *= 10;
    }

    base += offset * scale;
    if (base < lo) {
        return hi;
    }
    if (base > hi) {
        base = lo;
    }
    return base;
}

static void OtuEditor_FormatFixed(OtuTextDigits* out, s32 value) {
    s32 scaled = value * 1000;

    scaled = (s32)scaled / 4096;
    if (scaled < 0) {
        scaled = -scaled;
    }

    out->thousands = '0' + (scaled / 1000) % 10;
    out->point     = '.';
    out->hundreds  = '0' + (scaled / 100) % 10;
    out->tens      = '0' + (scaled / 10) % 10;
    out->ones      = '0' + (scaled % 10);
    out->space     = ' ';
}

static void OtuEditor_StepFixed(s32* value, s32 direction, s32 place) {
    s32 step;
    s32 scaled;
    s32 negative;

    scaled   = *value * 1000 / 4096;
    negative = (scaled < 0) ? 1 : 0;
    if (negative != 0) {
        scaled = -scaled;
    }

    switch (place % 4) {
        case 0:
            step = 1000;
            break;
        case 1:
            step = 100;
            break;
        case 2:
            step = 10;
            break;
        case 3:
            step = 1;
            break;
    }

    scaled += step * direction;

    if (scaled < 0) {
        scaled = 9999;
    } else if (scaled >= 10000) {
        scaled = 0;
    }
    if (negative != 0) {
        scaled = -scaled;
    }

    *value = scaled * 4096 / 1000;
}

static void OtuEditor_DrawCursor(TinPinSlammer_Scene* scene) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);

    if (table->cursor[table->page].row != table->cursor[table->page].drawnRow) {
        if (table->cursor[table->page].drawnRow >= 0) {
            Text_RenderToScreen(&scene->text, 0, table->cursor[table->page].drawnRow * 8 + 8, " ");
        }
        Text_RenderToScreen(&scene->text, 0, table->cursor[table->page].row * 8 + 8, "*");
        table->cursor[table->page].drawnRow = table->cursor[table->page].row;
    }

    if (table->cursor[table->page].digit == table->cursor[table->page].drawnDigit) {
        return;
    }

    if (table->cursor[table->page].digit >= 0) {
        Text_RenderToScreen(&scene->text, 0, table->cursor[table->page].row * 8 + 8, ">");
    } else {
        Text_RenderToScreen(&scene->text, 0, table->cursor[table->page].row * 8 + 8, "*");
    }
    table->cursor[table->page].drawnDigit = table->cursor[table->page].digit;
}

static void OtuEditor_EditMatch(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;

    table->blink = 0;

    value = table->cursor[table->page].digit;
    switch (value) {
        case 0:
        case 1:
            scene->matchIndex = OtuEditor_StepInt(scene->matchIndex, step, 1 - value, 0, 0x2B);
            OtuEditor_DrawDeck(scene, 1);
            OtuEditor_DrawDeck(scene, 2);
            OtuEditor_DrawDeck(scene, 3);
            break;
        case 2:
            scene->matches[scene->matchIndex].board =
                (s16)OtuEditor_StepInt(scene->matches[scene->matchIndex].board, step, 2 - value, 0, 9);
            break;
        case 3:
        case 4:
        case 5:
            scene->matches[scene->matchIndex].timeLimit =
                (s16)OtuEditor_StepInt(scene->matches[scene->matchIndex].timeLimit, step, 5 - value, 0, 0x3E7);
            break;
        case 6:
            scene->matches[scene->matchIndex].opponents[0].ai =
                (s16)OtuEditor_StepInt(scene->matches[scene->matchIndex].opponents[0].ai, step, 6 - value, 0, 6);
            break;
        case 7:
            scene->matches[scene->matchIndex].opponents[1].ai =
                (s16)OtuEditor_StepInt(scene->matches[scene->matchIndex].opponents[1].ai, step, 6 - value, 0, 6);
            break;
        case 8:
            scene->matches[scene->matchIndex].opponents[2].ai =
                (s16)OtuEditor_StepInt(scene->matches[scene->matchIndex].opponents[2].ai, step, 6 - value, 0, 6);
            break;
        default:
            break;
    }
}

static void OtuEditor_DrawMatch(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);

    OtuEditor_FormatInt(&row[0x0], scene->matchIndex, 2);
    OtuEditor_FormatInt(&row[0x3], scene->matches[scene->matchIndex].board, 1);
    OtuEditor_FormatInt(&row[0x5], scene->matches[scene->matchIndex].timeLimit, 3);
    OtuEditor_FormatInt(&row[0x9], scene->matches[scene->matchIndex].opponents[0].ai, 1);
    OtuEditor_FormatInt(&row[0xB], scene->matches[scene->matchIndex].opponents[1].ai, 1);
    OtuEditor_FormatInt(&row[0xD], scene->matches[scene->matchIndex].opponents[2].ai, 1);

    if (table->cursor[table->page].digit >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_MatchColumns[table->cursor[table->page].digit]] = ' ';
    }
    row[0xE] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x10, row);
}

static void OtuEditor_EditFriction(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       anchor;
    s32        value = table->cursor[table->page].digit;

    switch (value / 4) {
        case 0:
            anchor = &OtuPhys_Friction[0];
            break;
        case 1:
            anchor = &OtuPhys_Friction[1];
            break;
        case 2:
            anchor = &OtuPhys_Friction[2];
            break;
        case 3:
            anchor = &OtuPhys_Friction[3];
            break;
    }

    OtuEditor_StepFixed(anchor, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawFriction(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       src   = &OtuPhys_Friction[0];
    s32        value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], src[0]);
    row[0x5] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x6], src[1]);
    row[0xB] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0xC], src[2]);
    row[0x11] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x12], src[3]);

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_FrictionColumns[value]] = ' ';
    }
    row[0x17] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x18, row);
}

// Nonmatching: instruction selection differs
static void OtuEditor_EditDeck(TinPinSlammer_Scene* scene, s32 which, s32 step) {
    u16*       row;
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;
    s32        place;
    s32        v;

    switch (which) {
        case 0:
            row = scene->decks[which];
            break;
        case 1:
        case 2:
        case 3:
            row = scene->matches[scene->matchIndex].opponents[which - 1].deck;
            break;
    }

    value = table->cursor[table->page].digit;
    v     = row[value / 3];

    if (v == OTU_NO_PIN) {
        v = 0;
    } else {
        v = v + 1;
    }

    switch (value % 3) {
        case 0:
            place = 100;
            break;
        case 1:
            place = 10;
            break;
        case 2:
            place = 1;
            break;
    }

    v = v + place * step;

    if (v < 0) {
        v = OTU_NO_PIN + 1;
    } else {
        if (v >= OTU_NO_PIN + 2) {
            v = 0;
        }
        v = (v == 0) ? OTU_NO_PIN : v - 1;
    }

    row[value / 3] = v;

    table->blink = 0;
}

static void OtuEditor_DrawDeck(TinPinSlammer_Scene* scene, s32 which) {
    char       row[0x1D];
    u16*       src;
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;
    s32        i;

    switch (which) {
        case 0:
            src = scene->decks[which];
            break;
        case 1:
        case 2:
        case 3:
            src = scene->matches[scene->matchIndex].opponents[which - 1].deck;
            break;
    }

    for (i = 0; i < 6; i++) {
        value = src[0];

        if (value == OTU_NO_PIN) {
            value = 0;
        } else {
            value = value + 1;
        }

        row[i * 4 + 0] = '0' + (value / 100) % 10;
        row[i * 4 + 1] = '0' + (value / 10) % 10;
        row[i * 4 + 2] = '0' + value % 10;
        row[i * 4 + 3] = ' ';

        src = src + 1;
    }

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_DeckColumns[value]] = ' ';
    }
    row[0x18] = 0;

    Text_RenderToScreen(&scene->text, 0x38, which * 8 + 0x20, row);
}

static void OtuEditor_EditTune(TinPinSlammer_Scene* scene, s32 slot, s32 step) {
    OtuEditor*  table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuPinTune* tune  = &OtuPinTunes[slot];
    s32*        column;
    s32         value = table->cursor[table->page].digit;

    switch (value / 4) {
        case 0:
            column = &tune->launch;
            break;
        case 1:
            column = &tune->accel;
            break;
        case 2:
            column = &tune->power;
            break;
        case 3:
            column = &tune->weight;
            break;
    }

    OtuEditor_StepFixed(column, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawTune(TinPinSlammer_Scene* scene, s32 slot) {
    char        row[0x20];
    OtuEditor*  table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuPinTune* tune  = &OtuPinTunes[slot];
    s32         value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], tune->launch);
    row[0x5] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x6], tune->accel);
    row[0xB] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0xC], tune->power);
    row[0x11] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x12], tune->weight);
    row[0x17] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_TuneColumns[value]] = ' ';
    }
    row[0x17] = 0;

    Text_RenderToScreen(&scene->text, 0x38, slot * 8 + 0x40, row);
}

static void OtuEditor_EditDebrisDecel(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);

    OtuEditor_StepFixed(&OtuPhys_Gravity, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawDebrisDecel(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], OtuPhys_Gravity);

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_DebrisDecelColumns[value]] = ' ';
    }
    row[0x5] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x90, row);
}

static void OtuEditor_EditHand(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       column;
    s32        value = table->cursor[table->page].digit;

    switch (value / 4) {
        case 0:
            column = &OtuPhys_HandDashSpeed;
            break;
        case 1:
            column = &OtuPhys_HandJumpVelocity;
            break;
    }

    OtuEditor_StepFixed(column, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawHand(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], OtuPhys_HandDashSpeed);
    row[0x5] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x6], OtuPhys_HandJumpVelocity);

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_HandColumns[value]] = ' ';
    }
    row[0xB] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x98, row);
}

static void OtuEditor_EditPanels(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       column;
    s32        value = table->cursor[table->page].digit;

    switch (value / 4) {
        case 0:
            column = &OtuPhys_JumpVelocity;
            break;
        case 1:
            column = &OtuPhys_ConveyorSpeed;
            break;
        case 2:
            column = &OtuPhys_WallBounce;
            break;
    }

    OtuEditor_StepFixed(column, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawPanels(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], OtuPhys_JumpVelocity);
    row[0x5] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x6], OtuPhys_ConveyorSpeed);
    row[0xB] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0xC], OtuPhys_WallBounce);
    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_PanelsColumns[value]] = ' ';
    }
    row[0x11] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0xA0, row);
}

static void OtuEditor_EditStun(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       column;

    if (table->cursor[table->page].digit / 4 == 0) {
        column = &OtuPhys_StunnedImpact;
    }

    OtuEditor_StepFixed(column, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawStun(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], OtuPhys_StunnedImpact);

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_StunColumns[value]] = ' ';
    }
    row[0x5] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0xA8, row);
}

static void OtuEditor_EditDebrisSpeed(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       column;
    s32        value = table->cursor[table->page].digit;

    switch (value / 4) {
        case 0:
            column = &OtuDebris_SpeedMin;
            break;
        case 1:
            column = &OtuDebris_SpeedMax;
            break;
        case 2:
            column = &OtuDebris_Decel;
            break;
    }

    OtuEditor_StepFixed(column, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawDebrisSpeed(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], OtuDebris_SpeedMin);
    row[0x5] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x6], OtuDebris_SpeedMax);
    row[0xB] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0xC], OtuDebris_Decel);
    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_DebrisSpeedColumns[value]] = ' ';
    }
    row[0x11] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x8, row);
}

static void OtuEditor_EditDebrisLaunch(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       column;
    s32        value = table->cursor[table->page].digit;

    switch (value / 4) {
        case 0:
            column = &OtuDebris_LaunchMin;
            break;
        case 1:
            column = &OtuDebris_LaunchMax;
            break;
        case 2:
            column = &OtuDebris_Gravity;
            break;
        case 3:
            column = &OtuDebris_Bounce;
            break;
    }

    OtuEditor_StepFixed(column, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawDebrisLaunch(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], OtuDebris_LaunchMin);
    row[0x5] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x6], OtuDebris_LaunchMax);
    row[0xB] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0xC], OtuDebris_Gravity);
    row[0x11] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x12], OtuDebris_Bounce);
    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_DebrisLaunchColumns[value]] = ' ';
    }
    row[0x17] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x10, row);
}

static void OtuEditor_EditKnockback(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       column;

    if (table->cursor[table->page].digit / 4 == 0) {
        column = &OtuPhys_Knockback;
    }

    OtuEditor_StepFixed(column, step, table->cursor[table->page].digit % 4);

    table->blink = 0;
}

static void OtuEditor_DrawKnockback(TinPinSlammer_Scene* scene) {
    char       row[0x20];
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value;

    OtuEditor_FormatFixed((OtuTextDigits*)&row[0x0], OtuPhys_Knockback);
    row[0x5] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_KnockbackColumns[value]] = ' ';
    }
    row[0x5] = 0;
    row[0x5] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x18, row);
}

static void OtuEditor_EditSpin(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32*       counter;
    s32        place;
    s32        value;

    switch (table->cursor[table->page].digit / 5) {
        case 0:
            counter = &OtuPhys_SpinPower;
            break;
        case 1:
            counter = &OtuPhys_SpinDecay;
            break;
    }

    value = *counter;

    switch (table->cursor[table->page].digit % 5) {
        case 0:
            place = 10000;
            break;
        case 1:
            place = 1000;
            break;
        case 2:
            place = 100;
            break;
        case 3:
            place = 10;
            break;
        case 4:
            place = 1;
            break;
    }

    value += place * step;

    if (value >= 100000) {
        value = 0;
    } else if (value < 0) {
        value = 99999;
    }

    *counter = value;

    table->blink = 0;
}

static void OtuEditor_DrawSpin(TinPinSlammer_Scene* scene) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    char       row[0x20];
    s32        value;

    row[5]  = ' ';
    row[11] = ' ';

    value  = OtuPhys_SpinPower;
    row[0] = '0' + (value / 10000) % 10;
    row[1] = '0' + (value / 1000) % 10;
    row[2] = '0' + (value / 100) % 10;
    row[3] = '0' + (value / 10) % 10;
    row[4] = '0' + value % 10;

    value   = OtuPhys_SpinDecay;
    row[6]  = '0' + (value / 10000) % 10;
    row[7]  = '0' + (value / 1000) % 10;
    row[8]  = '0' + (value / 100) % 10;
    row[9]  = '0' + (value / 10) % 10;
    row[10] = '0' + value % 10;

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_SpinColumns[value]] = ' ';
    }
    row[11] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x20, row);
}

static void OtuEditor_EditPinId(TinPinSlammer_Scene* scene, s32 step) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    s32        place;
    s32        count;

    count = table->pinId;

    switch (table->cursor[table->page].digit % 3) {
        case 0:
            place = 100;
            break;
        case 1:
            place = 10;
            break;
        case 2:
            place = 1;
            break;
    }

    count = count + place * step;

    if (count < 0) {
        count = OTU_NO_PIN;
    } else if (count >= OTU_NO_PIN + 1) {
        count = 0;
    }
    table->pinId = count;

    table->blink = 0;
}

static void OtuEditor_DrawPinId(TinPinSlammer_Scene* scene) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);
    char       row[0x20];
    s32        value = table->pinId;

    row[0] = '0' + ((value + 1) / 100) % 10;
    row[1] = '0' + ((value + 1) / 10) % 10;
    row[2] = '0' + (value + 1) % 10;
    row[3] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_PinIdColumns[value]] = ' ';
    }
    row[3] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x8, row);
}

static void OtuEditor_EditSpecialUses(TinPinSlammer_Scene* scene, s32 slot, s32 step) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    u8*            counter;
    s32            place;
    s32            value;

    switch (table->cursor[table->page].digit / 2) {
        case 0:
            counter = &param->specialUses[0];
            break;
        case 1:
            counter = &param->specialUses[1];
            break;
        case 2:
            counter = &param->specialUses[2];
            break;
        case 3:
            counter = &param->specialUses[3];
            break;
    }

    value = *counter;

    switch (table->cursor[table->page].digit % 2) {
        case 0:
            place = 10;
            break;
        case 1:
            place = 1;
            break;
    }

    value += place * step;

    if (value < 0) {
        value = 99;
    } else if (value >= 100) {
        value = 0;
    }

    *counter = value;

    table->blink = 0;
}

static void OtuEditor_DrawSpecialUses(TinPinSlammer_Scene* scene, s32 slot) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    char           row[0x20];
    s32            value;

    value  = param->specialUses[0];
    row[0] = '0' + (value / 10) % 10;
    row[1] = '0' + value % 10;
    row[2] = ' ';

    value  = param->specialUses[1];
    row[3] = '0' + (value / 10) % 10;
    row[4] = '0' + value % 10;
    row[5] = ' ';

    value  = param->specialUses[2];
    row[6] = '0' + (value / 10) % 10;
    row[7] = '0' + value % 10;
    row[8] = ' ';

    value   = param->specialUses[3];
    row[9]  = '0' + (value / 10) % 10;
    row[10] = '0' + value % 10;
    row[11] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_SpecialUsesColumns[value]] = ' ';
    }
    row[11] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x10, row);
}

static void OtuEditor_EditPinTune(TinPinSlammer_Scene* scene, s32 slot, s32 step) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    u8*            counter;
    s32            place;
    s32            value;

    switch (table->cursor[table->page].digit / 2) {
        case 0:
            counter = &param->tuneIndex;
            break;
    }

    value = *counter;

    switch (table->cursor[table->page].digit % 2) {
        case 0:
            place = 10;
            break;
        case 1:
            place = 1;
            break;
    }

    value += place * step;

    if (value < 0) {
        value = 9;
    } else if (value >= 10) {
        value = 0;
    }

    *counter = value;

    table->blink = 0;
}

static void OtuEditor_DrawPinTune(TinPinSlammer_Scene* scene, s32 slot) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    char           row[0x20];
    s32            value = param->tuneIndex;

    row[0] = '0' + ((value + 1) / 10) % 10;
    row[1] = '0' + (value + 1) % 10;
    row[2] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_PinTuneColumns[value]] = ' ';
    }
    row[2] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x18, row);
}

static void OtuEditor_EditPinFriction(TinPinSlammer_Scene* scene, s32 slot, s32 step) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    s16*           counter;
    s32            temp;

    switch (table->cursor[table->page].digit / 4) {
        case 0:
            counter = &param->friction;
            break;
    }

    temp = *counter;
    OtuEditor_StepFixed(&temp, step, table->cursor[table->page].digit % 4);

    if (temp >= 0x8000) {
        temp = 0;
    }

    *counter = temp;

    table->blink = 0;
}

static void OtuEditor_DrawPinFriction(TinPinSlammer_Scene* scene, s32 slot) {
    OtuEditor* table = OtuStageDispatch_GetBlock(&scene->stage);

    char row[0x20];
    s32  value;

    OtuEditor_FormatFixed((OtuTextDigits*)row, scene->badgeParams[slot].friction);

    row[5] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_PinFrictionColumns[value]] = ' ';
    }
    row[5] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x20, row);
}

static void OtuEditor_EditNeedle(TinPinSlammer_Scene* scene, s32 slot, s32 step) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    u16*           counter;
    s32            place;
    s32            value;

    switch (table->cursor[table->page].digit / 3) {
        case 0:
            counter = &param->needleCharge;
            break;
        case 1:
            counter = &param->needleHold;
            break;
    }

    value = *counter;

    switch (table->cursor[table->page].digit % 3) {
        case 0:
            place = 100;
            break;
        case 1:
            place = 10;
            break;
        case 2:
            place = 1;
            break;
    }

    value += place * step;

    if (value < 0) {
        value = 999;
    } else if (value >= 1000) {
        value = 0;
    }

    *counter = value;

    table->blink = 0;
}

static void OtuEditor_DrawNeedle(TinPinSlammer_Scene* scene, s32 slot) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    char           row[0x20];
    s32            value;

    value  = param->needleCharge;
    row[0] = '0' + (value / 100) % 10;
    row[1] = '0' + (value / 10) % 10;
    row[2] = '0' + value % 10;
    row[3] = ' ';

    value  = param->needleHold;
    row[4] = '0' + (value / 100) % 10;
    row[5] = '0' + (value / 10) % 10;
    row[6] = '0' + value % 10;
    row[7] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_NeedleColumns[value]] = ' ';
    }
    row[7] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x28, row);
}

static void OtuEditor_EditMeteo(TinPinSlammer_Scene* scene, s32 slot, s32 step) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    u16*           counter;
    s32            place;
    s32            value;

    switch (table->cursor[table->page].digit / 3) {
        case 0:
            counter = &param->meteoHold;
            break;
        case 1:
            counter = &param->meteoSquash;
            break;
    }

    value = *counter;

    switch (table->cursor[table->page].digit % 3) {
        case 0:
            place = 100;
            break;
        case 1:
            place = 10;
            break;
        case 2:
            place = 1;
            break;
    }

    value += place * step;

    if (value < 0) {
        value = 999;
    } else if (value >= 1000) {
        value = 0;
    }

    *counter = value;

    table->blink = 0;
}

static void OtuEditor_DrawMeteo(TinPinSlammer_Scene* scene, s32 slot) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    char           row[0x20];
    s32            value;

    value  = param->meteoHold;
    row[0] = '0' + (value / 100) % 10;
    row[1] = '0' + (value / 10) % 10;
    row[2] = '0' + value % 10;
    row[3] = ' ';

    value  = param->meteoSquash;
    row[4] = '0' + (value / 100) % 10;
    row[5] = '0' + (value / 10) % 10;
    row[6] = '0' + value % 10;
    row[7] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_MeteoColumns[value]] = ' ';
    }
    row[7] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x30, row);
}

static void OtuEditor_EditHammer(TinPinSlammer_Scene* scene, s32 slot, s32 step) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    u16*           counter;
    s32            value = table->cursor[table->page].digit;

    switch (value) {
        case 0:
        case 1:
        case 2:
        case 3:
            counter = &param->hammerRate;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            counter = &param->hammerLength;
            break;
        case 8:
        case 9:
        case 10:
            counter = &param->hammerFrames;
            break;
        case 11:
            counter = &param->hammerArc;
            break;
    }

    switch (value) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7: {
            s32 temp = *(s16*)counter;
            OtuEditor_StepFixed(&temp, step, table->cursor[table->page].digit % 4);
            *counter = temp;
        } break;
        case 8:
        case 9:
        case 10: {
            s32 place;
            s32 v = *counter;
            switch ((value - 8) % 3) {
                case 0:
                    place = 100;
                    break;
                case 1:
                    place = 10;
                    break;
                case 2:
                    place = 1;
                    break;
            }
            v += place * step;
            if (v < 0) {
                v = 999;
            } else if (v >= 1000) {
                v = 0;
            }
            *counter = v;
        } break;
        case 11: {
            s32 v = *counter + step;
            if (v < 1) {
                v = 9;
            } else if (v >= 10) {
                v = 1;
            }
            *counter = v;
        } break;
    }
    table->blink = 0;
}

static void OtuEditor_DrawHammer(TinPinSlammer_Scene* scene, s32 slot) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];
    char           row[0x20];
    s32            value;

    OtuEditor_FormatFixed((OtuTextDigits*)(row + 0x00), param->hammerRate);
    row[5] = ' ';
    OtuEditor_FormatFixed((OtuTextDigits*)(row + 0x06), param->hammerLength);
    row[11] = ' ';

    value   = param->hammerFrames;
    row[12] = '0' + (value / 100) % 10;
    row[13] = '0' + (value / 10) % 10;
    row[14] = '0' + value % 10;
    row[15] = ' ';

    row[16] = '0' + param->hammerArc % 10;
    row[17] = ' ';

    value = table->cursor[table->page].digit;
    if (value >= 0 && (table->blink & 0x20)) {
        row[OtuEditor_HammerColumns[value]] = ' ';
    }
    row[17] = 0;

    Text_RenderToScreen(&scene->text, 0x38, 0x38, row);
}

static void OtuEditor_EditStunFrames(TinPinSlammer_Scene* scene, s32 slot, s32 step) {
    OtuEditor*     table = OtuStageDispatch_GetBlock(&scene->stage);
    OtuBadgeParam* param = &scene->badgeParams[slot];

    u16* counter = &param->stunFrames;
    s32  value   = *counter;
    s32  place;
    switch (table->cursor[table->page].digit % 3) {
        case 0:
            place = 100;
            break;
        case 1:
            place = 10;
            break;
        case 2:
            place = 1;
            break;
    }

    value += place * step;

    if (value < 0) {
        value = 999;
    } else if (value >= 1000) {
        value = 0;
    }

    *counter = value;

    table->blink = 0;
}

static void OtuEditor_DrawStunFrames(TinPinSlammer_Scene* scene, s32 slot) {
    OtuEditor* block = OtuStageDispatch_GetBlock(&scene->stage);
    s32        value = scene->badgeParams[slot].stunFrames;
    char       row[0x20];

    row[0] = '0' + (value / 100) % 10;
    row[1] = '0' + (value / 10) % 10;
    row[2] = '0' + value % 10;
    row[3] = ' ';

    s32 column = block->cursor[block->page].digit;
    if (column >= 0 && (block->blink & 0x20)) {
        row[OtuEditor_StunFramesColumns[column]] = ' ';
    }

    row[3] = 0;
    Text_RenderToScreen(&scene->text, 0x38, 0x40, row);
}

static void OtuEditor_DrawPage(TinPinSlammer_Scene* scene) {
    OtuEditor* block = OtuStageDispatch_GetBlock(&scene->stage);

    func_02010b50(&scene->text);

    switch (block->page) {
        case 0: {
            Text_RenderToScreen(&scene->text, 0, 0, "SINGLE MENU 1");
            OtuEditor_DrawCursor(scene);
            Text_RenderToScreen(&scene->text, 8, 0x08, "スタート");
            Text_RenderToScreen(&scene->text, 8, 0x10, "ラウンド");
            OtuEditor_DrawMatch(scene);
            Text_RenderToScreen(&scene->text, 8, 0x18, "摩擦");
            OtuEditor_DrawFriction(scene);
            Text_RenderToScreen(&scene->text, 8, 0x20, "バッジ1");
            OtuEditor_DrawDeck(scene, 0);
            Text_RenderToScreen(&scene->text, 8, 0x28, "バッジ2");
            OtuEditor_DrawDeck(scene, 1);
            Text_RenderToScreen(&scene->text, 8, 0x30, "バッジ3");
            OtuEditor_DrawDeck(scene, 2);
            Text_RenderToScreen(&scene->text, 8, 0x38, "バッジ4");
            OtuEditor_DrawDeck(scene, 3);
            Text_RenderToScreen(&scene->text, 8, 0x40, "重さ1");
            OtuEditor_DrawTune(scene, 0);
            Text_RenderToScreen(&scene->text, 8, 0x48, "重さ2");
            OtuEditor_DrawTune(scene, 1);
            Text_RenderToScreen(&scene->text, 8, 0x50, "重さ3");
            OtuEditor_DrawTune(scene, 2);
            Text_RenderToScreen(&scene->text, 8, 0x58, "重さ4");
            OtuEditor_DrawTune(scene, 3);
            Text_RenderToScreen(&scene->text, 8, 0x60, "重さ5");
            OtuEditor_DrawTune(scene, 4);
            Text_RenderToScreen(&scene->text, 8, 0x68, "重さ6");
            OtuEditor_DrawTune(scene, 5);
            Text_RenderToScreen(&scene->text, 8, 0x70, "重さ7");
            OtuEditor_DrawTune(scene, 6);
            Text_RenderToScreen(&scene->text, 8, 0x78, "重さ8");
            OtuEditor_DrawTune(scene, 7);
            Text_RenderToScreen(&scene->text, 8, 0x80, "重さ9");
            OtuEditor_DrawTune(scene, 8);
            Text_RenderToScreen(&scene->text, 8, 0x88, "重さ10");
            OtuEditor_DrawTune(scene, 9);
            Text_RenderToScreen(&scene->text, 8, 0x90, "重力加速度");
            OtuEditor_DrawDebrisDecel(scene);
            Text_RenderToScreen(&scene->text, 8, 0x98, "ターン");
            OtuEditor_DrawHand(scene);
            Text_RenderToScreen(&scene->text, 8, 0xA0, "パネル");
            OtuEditor_DrawPanels(scene);
            Text_RenderToScreen(&scene->text, 8, 0xA8, "気絶");
            OtuEditor_DrawStun(scene);
        } break;
        case 1: {
            Text_RenderToScreen(&scene->text, 0, 0, "SINGLE MENU 2");
            OtuEditor_DrawCursor(scene);
            Text_RenderToScreen(&scene->text, 8, 0x08, "衝突演出1");
            OtuEditor_DrawDebrisSpeed(scene);
            Text_RenderToScreen(&scene->text, 8, 0x10, "衝突演出2");
            OtuEditor_DrawDebrisLaunch(scene);
            Text_RenderToScreen(&scene->text, 8, 0x18, "必殺技");
            OtuEditor_DrawKnockback(scene);
            Text_RenderToScreen(&scene->text, 8, 0x20, "バッジ回転");
            OtuEditor_DrawSpin(scene);
        } break;
        case 2: {
            Text_RenderToScreen(&scene->text, 0, 0, "SINGLE MENU 3");
            OtuEditor_DrawCursor(scene);
            Text_RenderToScreen(&scene->text, 8, 0x08, "バッジID");
            OtuEditor_DrawPinId(scene);
            Text_RenderToScreen(&scene->text, 8, 0x10, "必殺技所持");
            OtuEditor_DrawSpecialUses(scene, block->pinId);
            Text_RenderToScreen(&scene->text, 8, 0x18, "重さ");
            OtuEditor_DrawPinTune(scene, block->pinId);
            Text_RenderToScreen(&scene->text, 8, 0x20, "まがる力");
            OtuEditor_DrawPinFriction(scene, block->pinId);
            Text_RenderToScreen(&scene->text, 8, 0x28, "ニードル");
            OtuEditor_DrawNeedle(scene, block->pinId);
            Text_RenderToScreen(&scene->text, 8, 0x30, "メテオ");
            OtuEditor_DrawMeteo(scene, block->pinId);
            Text_RenderToScreen(&scene->text, 8, 0x38, "ハンマー");
            OtuEditor_DrawHammer(scene, block->pinId);
            Text_RenderToScreen(&scene->text, 8, 0x40, "気絶時間");
            OtuEditor_DrawStunFrames(scene, block->pinId);
        } break;
    }
}

static void OtuScene_Editor_Enter(TinPinSlammer_Scene* scene) {
    char       pad[0x8];
    OtuEditor* block = OtuStageDispatch_GetBlock(&scene->stage);

    Display_SetMainLayers(0);
    Display_SetSubLayers(0x11);

    g_DisplaySettings.engineState[1].blendMode   = 0;
    g_DisplaySettings.engineState[1].blendLayer0 = 0;
    g_DisplaySettings.engineState[1].blendLayer1 = 0x20;

    block->page = 0;

    for (s32 i = 0; i < 3; i++) {
        block->cursor[i].digit      = -1;
        block->cursor[i].drawnDigit = -1;
        block->cursor[i].row        = 0;
        block->cursor[i].drawnRow   = 1;
    }

    block->blink = 0;
    block->pinId = 0;

    OtuScene_InitText(scene);
    OtuEditor_DrawPage(scene);
    func_0203a96c(pad);
    EasyFade_FadeBothDisplays(FADER_INTERPOLATE, 0, 0x1000);
}

static void OtuScene_Editor_Exit(TinPinSlammer_Scene* scene) {
    OtuStageDispatch_GetBlock(&scene->stage);
    OtuScene_ReleaseText(scene);
    EasyFade_FadeBothDisplays(FADER_SMOOTH, 0x10, 0x1000);
}

static void OtuScene_Editor_Run(TinPinSlammer_Scene* scene) {
    OtuEditor* block = OtuStageDispatch_GetBlock(&scene->stage);

    switch (SysControl.buttonState.pressedButtons) {
        case INPUT_BUTTON_UP: {
            if (block->cursor[block->page].digit < 0) {
                switch (block->page) {
                    case 0:
                        block->cursor[block->page].row -= 1;
                        if (block->cursor[block->page].row < 0) {
                            block->cursor[block->page].row = 0x14;
                        }
                        break;
                    case 1:
                        block->cursor[block->page].row -= 1;
                        if (block->cursor[block->page].row < 0) {
                            block->cursor[block->page].row = 0x3;
                        }
                        break;
                    case 2:
                        block->cursor[block->page].row -= 1;
                        if (block->cursor[block->page].row < 0) {
                            block->cursor[block->page].row = 0x7;
                        }
                        break;
                }
            } else {
                switch (block->page) {
                    case 0: {
                        switch (block->cursor[block->page].row) {
                            case 1:
                                OtuEditor_EditMatch(scene, 1);
                                break;
                            case 2:
                                OtuEditor_EditFriction(scene, 1);
                                break;
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                OtuEditor_EditDeck(scene, block->cursor[block->page].row - 3, 1);
                                break;
                            case 7:
                            case 8:
                            case 9:
                            case 10:
                            case 11:
                            case 12:
                            case 13:
                            case 14:
                            case 15:
                            case 16:
                                OtuEditor_EditTune(scene, block->cursor[block->page].row - 7, 1);
                                break;
                            case 17:
                                OtuEditor_EditDebrisDecel(scene, 1);
                                break;
                            case 18:
                                OtuEditor_EditHand(scene, 1);
                                break;
                            case 19:
                                OtuEditor_EditPanels(scene, 1);
                                break;
                            case 20:
                                OtuEditor_EditStun(scene, 1);
                                break;
                        }
                    } break;
                    case 1: {
                        switch (block->cursor[block->page].row) {
                            case 0:
                                OtuEditor_EditDebrisSpeed(scene, 1);
                                break;
                            case 1:
                                OtuEditor_EditDebrisLaunch(scene, 1);
                                break;
                            case 2:
                                OtuEditor_EditKnockback(scene, 1);
                                break;
                            case 3:
                                OtuEditor_EditSpin(scene, 1);
                                break;
                        }
                    } break;
                    case 2: {
                        switch (block->cursor[block->page].row) {
                            case 0:
                                OtuEditor_EditPinId(scene, 1);
                                OtuEditor_DrawSpecialUses(scene, block->pinId);
                                OtuEditor_DrawPinTune(scene, block->pinId);
                                OtuEditor_DrawPinFriction(scene, block->pinId);
                                OtuEditor_DrawNeedle(scene, block->pinId);
                                OtuEditor_DrawMeteo(scene, block->pinId);
                                OtuEditor_DrawHammer(scene, block->pinId);
                                OtuEditor_DrawStunFrames(scene, block->pinId);
                                break;
                            case 1:
                                OtuEditor_EditSpecialUses(scene, block->pinId, 1);
                                break;
                            case 2:
                                OtuEditor_EditPinTune(scene, block->pinId, 1);
                                break;
                            case 3:
                                OtuEditor_EditPinFriction(scene, block->pinId, 1);
                                break;
                            case 4:
                                OtuEditor_EditNeedle(scene, block->pinId, 1);
                                break;
                            case 5:
                                OtuEditor_EditMeteo(scene, block->pinId, 1);
                                break;
                            case 6:
                                OtuEditor_EditHammer(scene, block->pinId, 1);
                                break;
                            case 7:
                                OtuEditor_EditStunFrames(scene, block->pinId, 1);
                                break;
                        }
                    } break;
                }
            }
        } break;
        case INPUT_BUTTON_DOWN: {
            if (block->cursor[block->page].digit < 0) {
                switch (block->page) {
                    case 0:
                        block->cursor[block->page].row += 1;
                        if (block->cursor[block->page].row >= 21) {
                            block->cursor[block->page].row = 0;
                        }
                        break;
                    case 1:
                        block->cursor[block->page].row += 1;
                        if (block->cursor[block->page].row >= 4) {
                            block->cursor[block->page].row = 0;
                        }
                        break;
                    case 2:
                        block->cursor[block->page].row += 1;
                        if (block->cursor[block->page].row >= 8) {
                            block->cursor[block->page].row = 0;
                        }
                        break;
                }
            } else {
                switch (block->page) {
                    case 0: {
                        switch (block->cursor[block->page].row) {
                            case 1:
                                OtuEditor_EditMatch(scene, -1);
                                break;
                            case 2:
                                OtuEditor_EditFriction(scene, -1);
                                break;
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                OtuEditor_EditDeck(scene, block->cursor[block->page].row - 3, -1);
                                break;
                            case 7:
                            case 8:
                            case 9:
                            case 10:
                            case 11:
                            case 12:
                            case 13:
                            case 14:
                            case 15:
                            case 16:
                                OtuEditor_EditTune(scene, block->cursor[block->page].row - 7, -1);
                                break;
                            case 17:
                                OtuEditor_EditDebrisDecel(scene, -1);
                                break;
                            case 18:
                                OtuEditor_EditHand(scene, -1);
                                break;
                            case 19:
                                OtuEditor_EditPanels(scene, -1);
                                break;
                            case 20:
                                OtuEditor_EditStun(scene, -1);
                                break;
                        }
                    } break;
                    case 1: {
                        switch (block->cursor[block->page].row) {
                            case 0:
                                OtuEditor_EditDebrisSpeed(scene, -1);
                                break;
                            case 1:
                                OtuEditor_EditDebrisLaunch(scene, -1);
                                break;
                            case 2:
                                OtuEditor_EditKnockback(scene, -1);
                                break;
                            case 3:
                                OtuEditor_EditSpin(scene, -1);
                                break;
                        }
                    } break;
                    case 2: {
                        switch (block->cursor[block->page].row) {
                            case 0: {
                                OtuEditor_EditPinId(scene, -1);
                                OtuEditor_DrawSpecialUses(scene, block->pinId);
                                OtuEditor_DrawPinTune(scene, block->pinId);
                                OtuEditor_DrawPinFriction(scene, block->pinId);
                                OtuEditor_DrawNeedle(scene, block->pinId);
                                OtuEditor_DrawMeteo(scene, block->pinId);
                                OtuEditor_DrawHammer(scene, block->pinId);
                                OtuEditor_DrawStunFrames(scene, block->pinId);
                            } break;
                            case 1: {
                                OtuEditor_EditSpecialUses(scene, block->pinId, -1);
                            } break;
                            case 2: {
                                OtuEditor_EditPinTune(scene, block->pinId, -1);
                            } break;
                            case 3: {
                                OtuEditor_EditPinFriction(scene, block->pinId, -1);
                            } break;
                            case 4: {
                                OtuEditor_EditNeedle(scene, block->pinId, -1);
                            } break;
                            case 5: {
                                OtuEditor_EditMeteo(scene, block->pinId, -1);
                            } break;
                            case 6: {
                                OtuEditor_EditHammer(scene, block->pinId, -1);
                            } break;
                            case 7: {
                                OtuEditor_EditStunFrames(scene, block->pinId, -1);
                            } break;
                        }
                    } break;
                }
            }
        } break;
        case INPUT_BUTTON_LEFT: {
            if (block->cursor[block->page].digit >= 0) {
                switch (block->page) {
                    case 0:
                        switch (block->cursor[block->page].row) {
                            case 1: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 8;
                                }
                            } break;
                            case 2: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 15;
                                }
                            } break;
                            case 3:
                            case 4:
                            case 5:
                            case 6: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 17;
                                }
                            } break;
                            case 7:
                            case 8:
                            case 9:
                            case 10:
                            case 11:
                            case 12:
                            case 13:
                            case 14:
                            case 15:
                            case 16: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 15;
                                }
                            } break;
                            case 17: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 3;
                                }
                            } break;
                            case 18: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 7;
                                }
                            } // fallthrough
                            case 19: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 11;
                                }
                            } // fallthrough
                            case 20: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 3;
                                }
                            } break;
                        }
                        break;
                    case 1: {
                        switch (block->cursor[block->page].row) {
                            case 0: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 11;
                                }
                            } break;
                            case 1: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 15;
                                }
                            } break;
                            case 2: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 3;
                                }
                            } break;
                            case 3: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 9;
                                }
                            } break;
                        }
                    } break;
                    case 2: {
                        switch (block->cursor[block->page].row) {
                            case 0: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 2;
                                }
                            } break;
                            case 1: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 7;
                                }
                            } break;
                            case 2: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 1;
                                }
                            } break;
                            case 3: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 3;
                                }
                            } break;
                            case 4: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 5;
                                }
                            } break;
                            case 5: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 5;
                                }
                            } break;
                            case 6: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 11;
                                }
                            } break;
                            case 7: {
                                block->cursor[block->page].digit -= 1;
                                if (block->cursor[block->page].digit < 0) {
                                    block->cursor[block->page].digit = 2;
                                }
                            } break;
                        }
                    } break;
                }
            }
        } break;
        case INPUT_BUTTON_RIGHT: {
            if (block->cursor[block->page].digit >= 0) {
                switch (block->page) {
                    case 0: {
                        switch (block->cursor[block->page].row) {
                            case 1: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 9) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 2: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 16) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 3:
                            case 4:
                            case 5:
                            case 6: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 18) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 7:
                            case 8:
                            case 9:
                            case 10:
                            case 11:
                            case 12:
                            case 13:
                            case 14:
                            case 15:
                            case 16: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 16) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 17: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 4) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 18: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 8) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } // fallthrough
                            case 19: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 12) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } // fallthrough
                            case 20: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 4) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                        }
                    } break;
                    case 1: {
                        switch (block->cursor[block->page].row) {
                            case 0: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 12) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 1: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 16) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 2: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 4) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 3: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 10) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                        }
                    } break;
                    case 2: {
                        switch (block->cursor[block->page].row) {
                            case 0: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 3) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 1: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 8) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 2: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 2) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 3: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 4) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 4: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 6) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 5: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 6) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 6: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 12) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                            case 7: {
                                block->cursor[block->page].digit += 1;
                                if (block->cursor[block->page].digit >= 3) {
                                    block->cursor[block->page].digit = 0;
                                }
                            } break;
                        }
                    } break;
                }
            }
        } break;
        case INPUT_BUTTON_A: {
            if (block->cursor[block->page].digit < 0) {
                if (block->page == 0 && block->cursor[block->page].row == 0) {
                    OtuStageDispatch_SetNext(&scene->stage, &OtuScene_SinglePlayerBoard);
                } else {
                    block->cursor[block->page].digit = 0;
                }
            }
        } break;
        case INPUT_BUTTON_B: {
            if (block->cursor[block->page].digit >= 0) {
                block->cursor[block->page].digit = -1;
            } else {
                OtuStageDispatch_SetNext(&scene->stage, NULL);
            }
        } break;
        case INPUT_BUTTON_L: {
            block->page -= 1;
            if (block->page < 0) {
                block->page = 2;
            }
            block->blink                          = 0;
            block->cursor[block->page].drawnDigit = -2;
            block->cursor[block->page].drawnRow   = -2;
            OtuEditor_DrawPage(scene);
        } break;
        case INPUT_BUTTON_R: {
            block->page += 1;
            if (block->page >= 3) {
                block->page = 0;
            }
            block->blink                          = 0;
            block->cursor[block->page].drawnDigit = -2;
            block->cursor[block->page].drawnRow   = -2;
            OtuEditor_DrawPage(scene);
        } break;
    }

    OtuEditor_DrawCursor(scene);

    switch (block->page) {
        case 0: {
            switch (block->cursor[block->page].row) {
                case 1:
                    OtuEditor_DrawMatch(scene);
                    break;
                case 2:
                    OtuEditor_DrawFriction(scene);
                    break;
                case 3:
                case 4:
                case 5:
                case 6:
                    OtuEditor_DrawDeck(scene, block->cursor[block->page].row - 3);
                    break;
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                    OtuEditor_DrawTune(scene, block->cursor[block->page].row - 7);
                    break;
                case 17:
                    OtuEditor_DrawDebrisDecel(scene);
                    break;
                case 18:
                    OtuEditor_DrawHand(scene);
                    break;
                case 19:
                    OtuEditor_DrawPanels(scene);
                    break;
                case 20:
                    OtuEditor_DrawStun(scene);
                    break;
            }
        } break;
        case 1: {
            switch (block->cursor[block->page].row) {
                case 0:
                    OtuEditor_DrawDebrisSpeed(scene);
                    break;
                case 1:
                    OtuEditor_DrawDebrisLaunch(scene);
                    break;
                case 2:
                    OtuEditor_DrawKnockback(scene);
                    break;
                case 3:
                    OtuEditor_DrawSpin(scene);
                    break;
            }
        } break;
        case 2: {
            switch (block->cursor[block->page].row) {
                case 0:
                    OtuEditor_DrawPinId(scene);
                    break;
                case 1:
                    OtuEditor_DrawSpecialUses(scene, block->pinId);
                    break;
                case 2:
                    OtuEditor_DrawPinTune(scene, block->pinId);
                    break;
                case 3:
                    OtuEditor_DrawPinFriction(scene, block->pinId);
                    break;
                case 4:
                    OtuEditor_DrawNeedle(scene, block->pinId);
                    break;
                case 5:
                    OtuEditor_DrawMeteo(scene, block->pinId);
                    break;
                case 6:
                    OtuEditor_DrawHammer(scene, block->pinId);
                    break;
                case 7:
                    OtuEditor_DrawStunFrames(scene, block->pinId);
                    break;
            }
        } break;
    }
    block->blink++;
}
