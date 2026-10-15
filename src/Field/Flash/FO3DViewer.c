#include "Engine/Core/Memory.h"
#include "Field/FO3D.h"
#include <nitro/fx.h>

extern const char* data_ov030_02108fe4[];
extern const void* data_ov030_02108f2c[];
extern const s32*  data_ov030_02108e74[];
extern const s32   data_ov030_020ee880[];

s32 func_ov030_020d1b28(const char* name, Vec* pos, s32 slot);

// Nonmatching: stack slot order of count/tableCount and one register swap
void func_ov030_020d1098(s32 scene, BOOL replaceTexture) {
    void* anims[4];
    u32   count;
    void* coli;
    void* new_var[2];
    s32   animCount;
    long  i;
    u32*  chunk;
    s32   resIndex;
    long  k;
    coli       = 0;
    new_var[0] = 0;
    new_var[1] = 0;
    func_ov030_020d21e0();
    func_ov030_020d224c(data_ov030_02108fe4[scene], 1);
    count     = func_ov030_020d246c();
    animCount = 0;
    for (i = 0; i < count; i++) {
        chunk = func_ov030_020d2378(0, i);
        switch (*chunk) {
            case ((((u32)'B') | (((u32)'M') << 8)) | (((u32)'D') << 16)) | (((u32)'0') << 24):
                new_var[0] = chunk;
                break;

            case ((((u32)'B') | (((u32)'T') << 8)) | (((u32)'X') << 16)) | (((u32)'0') << 24):
                if (new_var[1] == 0) {
                    new_var[1] = chunk;
                } else if (replaceTexture) {
                    func_ov030_020d21d4(new_var[1]);
                    new_var[1] = chunk;
                } else {
                    func_ov030_020d21d4(chunk);
                }
                break;

            case ((((u32)'C') | (((u32)'O') << 8)) | (((u32)'L') << 16)) | (((u32)'I') << 24):
                coli = chunk;
                break;

            case ((((u32)'B') | (((u32)'C') << 8)) | (((u32)'A') << 16)) | (((u32)'0') << 24):

            case ((((u32)'B') | (((u32)'M') << 8)) | (((u32)'A') << 16)) | (((u32)'0') << 24):

            case ((((u32)'B') | (((u32)'T') << 8)) | (((u32)'A') << 16)) | (((u32)'0') << 24):

            case ((((u32)'B') | (((u32)'T') << 8)) | (((u32)'P') << 16)) | (((u32)'0') << 24):
                anims[animCount] = chunk;
                animCount++;
                break;
        }
    }

    func_ov030_020d2334();
    func_ov030_020d2218();
    data_ov030_02109ddc->scene         = scene;
    data_ov030_02109ddc->modelCount    = func_ov030_020cdb7c(new_var[0]);
    data_ov030_02109ddc->resourceCount = data_ov030_02109ddc->modelCount * (animCount + 1);
    if (new_var[1] != 0) {
        data_ov030_02109ddc->resourceCount++;
    }
    data_ov030_02109ddc->modelFile = new_var[0];
    data_ov030_02109ddc->resources = func_ov030_020d0f18(data_ov030_02109ddc->resourceCount * (sizeof(FieldResourceState*)));
    data_ov030_02109ddc->models    = func_ov030_020d0f18(data_ov030_02109ddc->modelCount * (sizeof(FO3DModel*)));
    for (i = 0, resIndex = 0; i < data_ov030_02109ddc->modelCount; i++) {
        data_ov030_02109ddc->models[i] = func_ov030_020cf554();
        func_ov030_020d0020();
        func_ov030_020cf848(data_ov030_02108f2c[scene], data_ov030_020ee880[scene]);
        data_ov030_02109ddc->resources[resIndex++] = func_ov030_020cdac0();
        func_ov030_020cdbac(new_var[0], i);
        func_ov030_020d0084(func_ov030_020cdb6c());
        for (k = 0; k < animCount; k++) {
            data_ov030_02109ddc->resources[resIndex++] = func_ov030_020cdac0();
            func_ov030_020cdc70(anims[k], i);
            func_ov030_020d036c(func_ov030_020cdb6c());
        }
    }

    if (new_var[1] != 0) {
        data_ov030_02109ddc->resources[resIndex] = func_ov030_020cdac0();
        func_ov030_020cdc40(new_var[1]);
        for (i = 0; i < data_ov030_02109ddc->modelCount; i++) {
            func_ov030_020cf838(data_ov030_02109ddc->models[i]);
            func_ov030_020d0304(func_ov030_020cdb6c());
        }
    }
    func_ov030_020cb21c(&data_ov030_02109ddc->world, 0, coli);
}

void func_ov030_020d1404(void) {
    if (data_ov030_02109ddc->extraModelFiles[0] != NULL) {
        func_ov030_020cdba0(data_ov030_02109ddc->extraModelFiles[0]);
    }
    if (data_ov030_02109ddc->extraModelFiles[1] != NULL) {
        func_ov030_020cdba0(data_ov030_02109ddc->extraModelFiles[1]);
    }
    if (data_ov030_02109ddc->modelFile != NULL) {
        func_ov030_020cdba0(data_ov030_02109ddc->modelFile);
    }
}

// Nonmatching: register allocation
void func_ov030_020d1458(s32 scene, const char* path) {
    void* anims[FO3D_ANIM_MAX];
    void* model;
    u32   count;
    void* texture;
    s32   animCount;
    s32   slot;
    s32   resIndex;
    long  j;
    long  k;

    model   = NULL;
    texture = NULL;
    func_ov030_020d21e0();
    func_ov030_020d224c(path, TRUE);
    count     = func_ov030_020d246c();
    animCount = 0;
    for (j = 0; j < count; j++) {
        u32* chunk = func_ov030_020d2378(NULL, j);

        switch (*chunk) {
            case FO3D_CHUNK_BMD0:
                model = chunk;
                break;
            case FO3D_CHUNK_BTX0:
                if (texture == NULL) {
                    texture = chunk;
                } else {
                    func_ov030_020d21d4(chunk);
                }
                break;
            case FO3D_CHUNK_BTP0:
            case FO3D_CHUNK_BTA0:
            case FO3D_CHUNK_BMA0:
            case FO3D_CHUNK_BCA0:
                anims[animCount] = chunk;
                animCount++;
                break;
        }
    }
    func_ov030_020d2334();
    func_ov030_020d2218();
    slot = data_ov030_02109ddc->extraCount;
    if (slot >= FO3D_VIEWER_EXTRA_MAX) {
        return;
    }
    data_ov030_02109ddc->extraCount               = slot + 1;
    data_ov030_02109ddc->extraModelCount[slot]    = func_ov030_020cdb7c(model);
    data_ov030_02109ddc->extraResourceCount[slot] = data_ov030_02109ddc->extraModelCount[slot] * (animCount + 1);
    if (texture != NULL) {
        data_ov030_02109ddc->extraResourceCount[slot]++;
    }
    data_ov030_02109ddc->extraModelFiles[slot] = model;
    data_ov030_02109ddc->extraResources[slot] =
        func_ov030_020d0f18(data_ov030_02109ddc->extraResourceCount[slot] * sizeof(FieldResourceState*));
    data_ov030_02109ddc->extraModels[slot] =
        func_ov030_020d0f18(data_ov030_02109ddc->extraModelCount[slot] * sizeof(FO3DModel*));
    for (j = 0, resIndex = 0; j < data_ov030_02109ddc->extraModelCount[slot]; j++) {
        data_ov030_02109ddc->extraModels[slot][j] = func_ov030_020cf554();
        func_ov030_020d0020();
        func_ov030_020cf848(data_ov030_02108f2c[scene], data_ov030_020ee880[scene]);
        data_ov030_02109ddc->extraResources[slot][resIndex++] = func_ov030_020cdac0();
        func_ov030_020cdbac(model, j);
        func_ov030_020d0084(func_ov030_020cdb6c());
        for (k = 0; k < animCount; k++) {
            data_ov030_02109ddc->extraResources[slot][resIndex++] = func_ov030_020cdac0();
            func_ov030_020cdc70(anims[k], j);
            func_ov030_020d036c(func_ov030_020cdb6c());
        }
    }
    if (texture == NULL) {
        return;
    }
    data_ov030_02109ddc->extraResources[slot][resIndex] = func_ov030_020cdac0();
    func_ov030_020cdc40(texture);
    for (j = 0; j < data_ov030_02109ddc->extraModelCount[slot]; j++) {
        func_ov030_020cf838(data_ov030_02109ddc->extraModels[slot][j]);
        func_ov030_020d0304(func_ov030_020cdb6c());
    }
}

void func_ov030_020d17c0(s32 scene) {
    s32 count;
    s32 i;

    func_ov030_020d0f64();
    func_ov030_020cd7d4();
    func_ov030_020d1098(scene, FALSE);
    if (data_ov030_02109ddc == NULL) {
        return;
    }
    count                       = data_ov030_020ee880[scene];
    data_ov030_02109ddc->points = func_ov030_020d0f18(count * sizeof(FieldPos));
    data_ov030_02109ddc->pos.x  = data_ov030_02108e74[scene][0] << FX32_SHIFT;
    data_ov030_02109ddc->pos.y  = data_ov030_02108e74[scene][1] << FX32_SHIFT;
    for (i = 0; i < count; i++) {
        FieldPos* point = &data_ov030_02109ddc->points[i];

        point->x = data_ov030_02108e74[scene][i * 2];
        point->y = data_ov030_02108e74[scene][i * 2 + 1];
        if (i != 0 && count < 25) {
            point->x -= data_ov030_02109ddc->points[0].x;
            point->y -= data_ov030_02109ddc->points[0].y;
        }
    }
}

BOOL func_ov030_020d18cc(s32 x, s32 y) {
    BOOL moved = FALSE;
    BOOL changed;
    s32  oldX;
    s32  oldY;
    s32  i;

    changed = FALSE;
    oldX    = data_ov030_02109ddc->pos.x;
    oldY    = data_ov030_02109ddc->pos.y;
    if (x != oldX) {
        changed = TRUE;
    }
    if (y != oldY) {
        changed = TRUE;
    }
    if (changed) {
        data_ov030_02109ddc->pos.x = x;
        data_ov030_02109ddc->pos.y = y;
        if (func_ov030_020cf910(&data_ov030_02109ddc->pos, data_ov030_02109ddc->points)) {
            moved = TRUE;
        } else {
            data_ov030_02109ddc->pos.x = oldX;
            data_ov030_02109ddc->pos.y = oldY;
        }
    }
    for (i = 0; i < data_ov030_02109ddc->modelCount; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->models[i]);
        func_ov030_020d0630(&data_ov030_02109ddc->pos, data_ov030_02109ddc->points);
    }
    for (i = 0; i < data_ov030_02109ddc->extraModelCount[0]; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->extraModels[0][i]);
        func_ov030_020d0630(&data_ov030_02109ddc->pos, data_ov030_02109ddc->points);
    }
    for (i = 0; i < data_ov030_02109ddc->extraModelCount[1]; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->extraModels[1][i]);
        func_ov030_020d0630(&data_ov030_02109ddc->pos, data_ov030_02109ddc->points);
    }
    return moved;
}

void func_ov030_020d1a1c(s32 x, s32 y) {
    Vec pos;
    s32 i;

    pos.x = x - (128 << FX32_SHIFT);
    pos.y = y - (96 << FX32_SHIFT);
    pos.z = 0;
    for (i = 0; i < data_ov030_02109ddc->modelCount; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->models[i]);
        func_ov030_020d08e8(&pos);
    }
}

BOOL func_ov030_020d1a8c(const char* name, Vec* pos) {
    s32 i;

    for (i = 0; i < data_ov030_02109ddc->modelCount; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->models[i]);
        if (func_ov030_020d0b24(name, pos)) {
            return TRUE;
        }
    }
    if (func_ov030_020d1b28(name, pos, 0)) {
        return TRUE;
    }
    if (func_ov030_020d1b28(name, pos, 1)) {
        return TRUE;
    }
    return FALSE;
}

BOOL func_ov030_020d1b28(const char* name, Vec* pos, s32 slot) {
    s32 i;

    for (i = 0; i < data_ov030_02109ddc->extraModelCount[slot]; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->extraModels[slot][i]);
        if (func_ov030_020d0b24(name, pos)) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 func_ov030_020d1ba0(const char* name) {
    s32 i;

    for (i = 0; i < data_ov030_02109ddc->modelCount; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->models[i]);
        if (func_ov030_020d0c38(name) && data_ov030_02109ddc->models[i]->blendMode == 0 && !func_ov030_020d0bf0()) {
            return func_ov030_020d0bc0();
        }
    }
    for (i = 0; i < data_ov030_02109ddc->extraModelCount[0]; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->extraModels[0][i]);
        if (func_ov030_020d0c38(name) && data_ov030_02109ddc->extraModels[0][i]->blendMode == 0 && !func_ov030_020d0bf0()) {
            return func_ov030_020d0bc0();
        }
    }
    for (i = 0; i < data_ov030_02109ddc->extraModelCount[1]; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->extraModels[1][i]);
        if (func_ov030_020d0c38(name) && data_ov030_02109ddc->extraModels[1][i]->blendMode == 0 && !func_ov030_020d0bf0()) {
            return func_ov030_020d0bc0();
        }
    }
    return 0;
}

#define FO3D_VIEWER_FIND_AND(name, action)                               \
    do {                                                                 \
        s32 i;                                                           \
                                                                         \
        for (i = 0; i < data_ov030_02109ddc->modelCount; i++) {          \
            func_ov030_020cf838(data_ov030_02109ddc->models[i]);         \
            if (func_ov030_020d0c38(name)) {                             \
                action();                                                \
                return;                                                  \
            }                                                            \
        }                                                                \
        for (i = 0; i < data_ov030_02109ddc->extraModelCount[0]; i++) {  \
            func_ov030_020cf838(data_ov030_02109ddc->extraModels[0][i]); \
            if (func_ov030_020d0c38(name)) {                             \
                action();                                                \
                return;                                                  \
            }                                                            \
        }                                                                \
        for (i = 0; i < data_ov030_02109ddc->extraModelCount[1]; i++) {  \
            func_ov030_020cf838(data_ov030_02109ddc->extraModels[1][i]); \
            if (func_ov030_020d0c38(name)) {                             \
                action();                                                \
                return;                                                  \
            }                                                            \
        }                                                                \
    } while (0)

void func_ov030_020d1d08(const char* name) {
    FO3D_VIEWER_FIND_AND(name, func_ov030_020d0c70);
}

void func_ov030_020d1e00(const char* name) {
    FO3D_VIEWER_FIND_AND(name, func_ov030_020d0cd4);
}

void func_ov030_020d1ef8(const char* name) {
    FO3D_VIEWER_FIND_AND(name, func_ov030_020d0cf0);
}

void func_ov030_020d1ff0(s32 slot) {
    s32 i;

    for (i = 0; i < data_ov030_02109ddc->extraResourceCount[slot]; i++) {
        func_ov030_020cdb5c(data_ov030_02109ddc->extraResources[slot][i]);
        func_ov030_020cdb18();
    }
    for (i = 0; i < data_ov030_02109ddc->extraModelCount[slot]; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->extraModels[slot][i]);
        func_ov030_020cf6a0();
    }
    if (data_ov030_02109ddc->extraResources[slot] != NULL) {
        func_ov030_020d0f4c(data_ov030_02109ddc->extraResources[slot]);
    }
    if (data_ov030_02109ddc->extraModels[slot] != NULL) {
        func_ov030_020d0f4c(data_ov030_02109ddc->extraModels[slot]);
    }
}

void func_ov030_020d20bc(void) {
    s32 i;

    for (i = 0; i < data_ov030_02109ddc->resourceCount; i++) {
        func_ov030_020cdb5c(data_ov030_02109ddc->resources[i]);
        func_ov030_020cdb18();
    }
    for (i = 0; i < data_ov030_02109ddc->modelCount; i++) {
        func_ov030_020cf838(data_ov030_02109ddc->models[i]);
        func_ov030_020cf6a0();
    }
    func_ov030_020d0f4c(data_ov030_02109ddc->resources);
    func_ov030_020d0f4c(data_ov030_02109ddc->models);
    func_ov030_020d0f4c(data_ov030_02109ddc->points);
    func_ov030_020d21d4(data_ov030_02109ddc->world.indexTable);
    func_ov030_020d1ff0(0);
    func_ov030_020d1ff0(1);
    func_ov030_020d1060();
}

void* func_ov030_020d2188(s32 size) {
    void* data = Mem_AllocHeapTail(&gMainHeap, size);
    Mem_SetSequence(&gMainHeap, data, "FOFileIO_Alloc");
    return data;
}

void func_ov030_020d21bc(void* data) {
    Mem_Free(&gMainHeap, data);
}

void func_ov030_020d21d4(void* data) {
    func_ov030_020d21bc(data);
}
