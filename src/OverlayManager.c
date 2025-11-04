#include "OverlayManager.h"

OverlayManager data_0206b294; // Main Overlay manager. Handles loading/initializing and unloading overlays.
OverlayManager data_0206b170; // Secondary manager? Possibly used for Debug launcher? Needs investigation.
OverlayManager data_0206b04c; // Unknown manager. main() initializes this
OverlayManager data_0206af28; // Unknown manager. main() initializes this
s32*           data_0206af24;

void func_02006f50(GameState*) {
    return;
}

void func_02006f54(OverlayManager* overlayMgr) {
    overlayMgr->unk_10 = 0;
    func_02006f78(overlayMgr, -0x7fffffff, func_02006f50, NULL, 0);
}

void func_02006f78(OverlayManager* overlayMgr, s32 overlayId, OverlayCB callback, GameState* state, s32 param_5) {
    OverlayData* currentData = &overlayMgr->unk_14[overlayMgr->unk_10];

    currentData->tag.id = overlayId;
    currentData->tag.cb = callback;
    currentData->gState = state;
    currentData->unk_0C = param_5;

    currentData        = &overlayMgr->unk_14[overlayMgr->unk_10];
    overlayMgr->data   = currentData;
    overlayMgr->cb     = currentData->tag.cb;
    overlayMgr->gState = currentData->gState;
    overlayMgr->unk_10++;
}

/* Nonmatching: regalloc differences */
/* Why is there stack usage? */
void func_02006fc4(OverlayTag* tag, OverlayManager* overlayMgr) {
    OverlayData* previousData;
    OverlayData* currentData;
    int          currentIndex;

    currentIndex = overlayMgr->unk_10 - 1;
    currentData  = &overlayMgr->unk_14[currentIndex];
    previousData = &overlayMgr->unk_14[currentIndex - 1];

    tag->id = currentData->tag.id;
    tag->cb = currentData->tag.cb;

    overlayMgr->unk_10--;
    overlayMgr->data   = previousData;
    overlayMgr->cb     = previousData->tag.cb;
    overlayMgr->gState = previousData->gState;

    if (overlayMgr->unk_10 == 0) {
        func_02006f78(overlayMgr, 0x80000001, func_02006f50, NULL, 0);
    }
}

/* Nonmatching: opcode reordering */
void func_02007050(OverlayTag* tag, OverlayManager* overlayMgr, s32 param_3, OverlayCB cb, GameState* state, s32 param_6) {
    OverlayTag local;

    func_02006fc4(&local, overlayMgr);
    func_02006f78(overlayMgr, param_3, cb, state, param_6);
    s32 cbFunc = local.cb;
    s32 id     = local.id;
    tag->cb    = cbFunc;
    tag->id    = id;
}

BOOL func_020070a4(OverlayManager* overlayMgr) {
    BOOL ret = FALSE;
    if (overlayMgr->unk_10 <= 1) {
        ret = TRUE;
    }
    return ret;
}

GameState* func_020070b8(OverlayManager* overlayMgr, GameState* state) {
    GameState* prevState     = overlayMgr->gState;
    overlayMgr->data->gState = state;
    overlayMgr->gState       = state;
    return prevState;
}

s32 func_020070d0(OverlayManager* overlayMgr) {
    return overlayMgr->data->unk_0C;
}

void func_020070dc(OverlayManager* overlayMgr, s32 param_2) {
    overlayMgr->data->unk_0C = param_2;
}

void func_020070e8(OverlayManager* overlayMgr) {
    overlayMgr->data->unk_0C++;
}

void func_020070fc(OverlayManager* overlayMgr) {
    do {
        overlayMgr->unk_0C = 0;
        overlayMgr->cb(overlayMgr->gState);
    } while (overlayMgr->unk_0C == 1);
}

void func_02007128(void) {
    func_02006f54(&data_0206b294);
}

void func_0200713c(s32 overlayId, OverlayCB callback, GameState* state, int param_4) {
    func_02006f78(&data_0206b294, overlayId, callback, state, param_4);
}

/* Nonmatching: Regalloc differences */
/* Why is there stack usage? */
void func_02007174(OverlayTag* tag) {
    OverlayManager* overlay = &data_0206b294;
    OverlayTag*     stack;
    if (func_02007278() != 0x7FFFFFFF) {
        func_0200728c(0x7FFFFFFF);
        func_020072b8();
    }
    s32          idx          = overlay->unk_10;
    OverlayData* currentData  = &overlay->unk_14[idx - 1];
    OverlayData* previousData = &overlay->unk_14[idx - 2];

    overlay->data   = previousData;
    overlay->cb     = previousData->tag.cb;
    stack->id       = currentData->tag.id;
    overlay->gState = previousData->gState;
    stack->cb       = currentData->tag.cb;
    overlay->unk_10 = idx;

    tag->id = previousData->tag.id;
    tag->cb = currentData->tag.cb;
}

/* Nonmatching: opcode reordering */
void func_020071f4(OverlayTag* tag, s32 overlayId, void* callback, GameState* state, s32 param_5) {
    OverlayTag local;

    func_02007174(&local);
    func_0200713c(overlayId, callback, state, param_5);
    s32 cbFunc = local.cb;
    s32 id     = local.id;
    tag->cb    = cbFunc;
    tag->id    = id;
}

/* Nonmatching: opcode reordering? maybe? */
u32 func_02007240(void) {
    func_0200669c(1, *(data_0206af24 + 0xDC));
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

void func_020072b8(void) {
    OverlayManager* temp = &data_0206b294;
    if (func_020066fc(data_0206b294.data->tag.id) == 1) {
        func_020070fc(temp);
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