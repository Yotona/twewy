#include "OverlayManager.h"

Overlay data_0206b294;
Overlay data_0206b170;
Overlay data_0206b04c;
Overlay data_0206af28;
s32     data_0206af24;

void func_02006f78(Overlay* overlay, s32 param_2, OverlayCB func, GameState* param_4, s32 param_5);
s32  func_02007278(void);
void func_0200728c(s32 param_1);

void func_02006f50(GameState*) {
    return;
}

void func_02006f54(Overlay* overlay) {
    overlay->unk_10 = 0;
    func_02006f78(overlay, -0x7fffffff, func_02006f50, NULL, 0);
}

void func_02006f78(Overlay* overlay, s32 overlayId, OverlayCB callback, GameState* state, s32 param_5) {
    OverlayData* currentData = &overlay->unk_14[overlay->unk_10];

    currentData->tag.id = overlayId;
    currentData->tag.cb = callback;
    currentData->gState = state;
    currentData->unk_0C = param_5;

    currentData     = &overlay->unk_14[overlay->unk_10];
    overlay->data   = currentData;
    overlay->cb     = currentData->tag.cb;
    overlay->gState = currentData->gState;
    overlay->unk_10++;
}

/* Nonmatching */
void func_02006fc4(OverlayTag* tag, Overlay* overlay) {
    OverlayData* previousData;
    OverlayData* currentData;
    int          currentIndex;

    currentIndex = overlay->unk_10 - 1;
    currentData  = &overlay->unk_14[currentIndex];
    previousData = &overlay->unk_14[currentIndex - 1];

    tag->id = currentData->tag.id;
    tag->cb = currentData->tag.cb;

    overlay->unk_10--;
    overlay->data   = previousData;
    overlay->cb     = previousData->tag.cb;
    overlay->gState = previousData->gState;

    if (overlay->unk_10 == 0) {
        func_02006f78(overlay, 0x80000001, func_02006f50, NULL, 0);
    }
}

/* Nonmatching */
void func_02007050(OverlayTag* tag, Overlay* overlay, s32 param_3, OverlayCB cb, GameState* state, s32 param_6) {
    OverlayTag local;

    func_02006fc4(&local, overlay);
    func_02006f78(overlay, param_3, cb, state, param_6);
    tag->id = local.id;
    tag->cb = local.cb;
}

BOOL func_020070a4(Overlay* overlay) {
    BOOL ret = FALSE;
    if (overlay->unk_10 <= 1) {
        ret = TRUE;
    }
    return ret;
}

GameState* func_020070b8(Overlay* overlay, GameState* state) {
    GameState* prevState  = overlay->gState;
    overlay->data->gState = state;
    overlay->gState       = state;
    return prevState;
}

s32 func_020070d0(Overlay* overlay) {
    return overlay->data->unk_0C;
}

void func_020070dc(Overlay* overlay, s32 param_2) {
    overlay->data->unk_0C = param_2;
}

void func_020070e8(Overlay* overlay) {
    overlay->data->unk_0C++;
}

void func_020070fc(Overlay* overlay) {
    do {
        overlay->unk_0C = 0;
        overlay->cb(overlay->gState);
    } while (overlay->unk_0C == 1);
}

void func_02007128(void) {
    func_02006f54(&data_0206b294);
}

void func_0200713c(s32 overlayId, OverlayCB callback, GameState* state, int param_4) {
    func_02006f78(&data_0206b294, overlayId, callback, state, param_4);
}

/* Nonmatching */
void func_02007174(OverlayTag* tag) {
    if (func_02007278() != 0x7FFFFFFF) {
        func_0200728c(0x7FFFFFFF);
        func_020072b8();
    }

    OverlayData* currentData  = &data_0206b294.unk_14[data_0206b294.unk_10 - 1];
    OverlayData* previousData = &data_0206b294.unk_14[data_0206b294.unk_10 - 2];

    data_0206b294.data   = previousData;
    data_0206b294.cb     = previousData->tag.cb;
    data_0206b294.gState = previousData->gState;
    data_0206b294.unk_10--;

    tag->id = previousData->tag.id;
    tag->cb = currentData->tag.cb;
}

/* Nonmatching */
void func_020071f4(OverlayTag* tag, s32 overlayId, void* callback, GameState* state, s32 param_5) {
    OverlayTag local;

    func_02007174(&local);
    func_0200713c(overlayId, callback, state, param_5);
    tag->id = local.id;
    tag->cb = local.cb;
}

/* Nonmatching */
u32 func_02007240(void) {
    func_0200669c(1, &data_0206af24);
}

GameState* func_02007260(void* state) {
    func_020070b8(&data_0206b294, state);
}

s32 func_02007278(void) {
    return func_020070d0(&data_0206b294);
}

void func_0200728c(s32 param_1) {
    func_020070dc(&data_0206b294, param_1);
}

void func_020072a4(void) {
    func_020070e8(&data_0206b294);
}

/* Nonmatching */
void func_020072b8(void) {
    if (func_020066fc(data_0206b294.data->tag.id) == 1) {
        func_020070fc(&data_0206b294);
    }
}

void func_020072ec(void) {
    func_02006f54(&data_0206b170);
}

void func_02007300(OverlayCB callback, GameState* state, s32 param_3) {
    func_02006f78(&data_0206b170, 0, callback, state, param_3);
}

s32 func_02007328(void) {
    OverlayTag tag;

    func_02006fc4(&tag, &data_0206b170);
    return tag.cb;
}

s32 func_0200734c(OverlayCB callback, GameState* state, s32 param_3) {
    OverlayTag tag;

    func_02007050(&tag, &data_0206b170, 0, callback, state, param_3);
    return tag.cb;
}

BOOL func_0200737c(void) {
    return func_020070a4(&data_0206b170);
}

void func_02007390(void) {
    func_020070fc(&data_0206b170);
}

void func_020073a4(void) {
    func_02006f54(&data_0206af28);
}

void func_020073b8(void) {
    func_02006f54(&data_0206b04c);
}