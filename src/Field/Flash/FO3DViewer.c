#include "Field/Flash/FO3DViewer.h"
#include "Engine/Core/Memory.h"
#include "Field/FieldTypes.h"
#include "Field/Flash/FO3D.h"
#include "Field/Flash/FO3DObjMgr.h"
#include "Field/Flash/FO3DRes.h"
#include "Field/Flash/FO3DViewerImd.h"
#include "Field/Flash/FOAllocator.h"
#include "Field/Flash/FOFileList.h"
#include "Field/Tasks/Fld_Shinigami.h"
#include <nitro/fx.h>

extern const char* FO3DViewer_ScenePaths[];
extern const void* FO3DViewer_SceneSampleFrames[];
extern const s32*  FO3DViewer_ScenePoints[];

s32 FO3DViewer_GetExtraNodePos(const char* name, Vec* pos, s32 slot);

// Nonmatching: stack slot order of count/tableCount and one register swap
void FO3DViewer_LoadScene(s32 scene, BOOL replaceTexture) {
    void* anims[4];
    u32   count;
    void* coli;
    void* files[2];
    s32   animCount;
    long  i;
    u32*  chunk;
    s32   resIndex;
    long  k;
    coli     = 0;
    files[0] = 0;
    files[1] = 0;
    FOFileList_Create();
    FOFileList_Open(FO3DViewer_ScenePaths[scene], 1);
    count     = FOFileList_GetEntryCount();
    animCount = 0;
    for (i = 0; i < count; i++) {
        chunk = FOFileList_ReadEntry(0, i);
        switch (*chunk) {
            case ((((u32)'B') | (((u32)'M') << 8)) | (((u32)'D') << 16)) | (((u32)'0') << 24):
                files[0] = chunk;
                break;

            case ((((u32)'B') | (((u32)'T') << 8)) | (((u32)'X') << 16)) | (((u32)'0') << 24):
                if (files[1] == 0) {
                    files[1] = chunk;
                } else if (replaceTexture) {
                    FOFileIO_Release(files[1]);
                    files[1] = chunk;
                } else {
                    FOFileIO_Release(chunk);
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

    FOFileList_Close();
    FOFileList_Destroy();
    gFO3DViewer->scene         = scene;
    gFO3DViewer->modelCount    = FO3DRes_GetModelCount(files[0]);
    gFO3DViewer->resourceCount = gFO3DViewer->modelCount * (animCount + 1);
    if (files[1] != 0) {
        gFO3DViewer->resourceCount++;
    }
    gFO3DViewer->modelFile = files[0];
    gFO3DViewer->resources = FO3DViewerImd_Alloc(gFO3DViewer->resourceCount * (sizeof(FieldResourceState*)));
    gFO3DViewer->models    = FO3DViewerImd_Alloc(gFO3DViewer->modelCount * (sizeof(FO3DModel*)));
    for (i = 0, resIndex = 0; i < gFO3DViewer->modelCount; i++) {
        gFO3DViewer->models[i] = FO3DObjMgr_Create();
        FO3DObjMgr_ResetObj();
        FO3DObjMgr_SetSampleFrames(FO3DViewer_SceneSampleFrames[scene], FO3DViewer_ScenePointCounts[scene]);
        gFO3DViewer->resources[resIndex++] = FO3DRes_Create();
        FO3DRes_SetModel(files[0], i);
        FO3DObjMgr_SetModel(FO3DRes_GetCurrent());
        for (k = 0; k < animCount; k++) {
            gFO3DViewer->resources[resIndex++] = FO3DRes_Create();
            FO3DRes_SetAnim(anims[k], i);
            FO3DObjMgr_AddAnim(FO3DRes_GetCurrent());
        }
    }

    if (files[1] != 0) {
        gFO3DViewer->resources[resIndex] = FO3DRes_Create();
        FO3DRes_SetTexture(files[1]);
        for (i = 0; i < gFO3DViewer->modelCount; i++) {
            FO3DObjMgr_SetCurrent(gFO3DViewer->models[i]);
            FO3DObjMgr_SetTexture(FO3DRes_GetCurrent());
        }
    }
    Fld_Shinigami_SetWorld(&gFO3DViewer->world, 0, coli);
}

void FO3DViewer_LoadToVram(void) {
    if (gFO3DViewer->extraModelFiles[0] != NULL) {
        FO3DRes_LoadToVram(gFO3DViewer->extraModelFiles[0]);
    }
    if (gFO3DViewer->extraModelFiles[1] != NULL) {
        FO3DRes_LoadToVram(gFO3DViewer->extraModelFiles[1]);
    }
    if (gFO3DViewer->modelFile != NULL) {
        FO3DRes_LoadToVram(gFO3DViewer->modelFile);
    }
}

// Nonmatching: register allocation
void FO3DViewer_LoadExtra(s32 scene, const char* path) {
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
    FOFileList_Create();
    FOFileList_Open(path, TRUE);
    count     = FOFileList_GetEntryCount();
    animCount = 0;
    for (j = 0; j < count; j++) {
        u32* chunk = FOFileList_ReadEntry(NULL, j);

        switch (*chunk) {
            case FO3D_CHUNK_BMD0:
                model = chunk;
                break;
            case FO3D_CHUNK_BTX0:
                if (texture == NULL) {
                    texture = chunk;
                } else {
                    FOFileIO_Release(chunk);
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
    FOFileList_Close();
    FOFileList_Destroy();
    slot = gFO3DViewer->extraCount;
    if (slot >= FO3D_VIEWER_EXTRA_MAX) {
        return;
    }
    gFO3DViewer->extraCount               = slot + 1;
    gFO3DViewer->extraModelCount[slot]    = FO3DRes_GetModelCount(model);
    gFO3DViewer->extraResourceCount[slot] = gFO3DViewer->extraModelCount[slot] * (animCount + 1);
    if (texture != NULL) {
        gFO3DViewer->extraResourceCount[slot]++;
    }
    gFO3DViewer->extraModelFiles[slot] = model;
    gFO3DViewer->extraResources[slot] =
        FO3DViewerImd_Alloc(gFO3DViewer->extraResourceCount[slot] * sizeof(FieldResourceState*));
    gFO3DViewer->extraModels[slot] = FO3DViewerImd_Alloc(gFO3DViewer->extraModelCount[slot] * sizeof(FO3DModel*));
    for (j = 0, resIndex = 0; j < gFO3DViewer->extraModelCount[slot]; j++) {
        gFO3DViewer->extraModels[slot][j] = FO3DObjMgr_Create();
        FO3DObjMgr_ResetObj();
        FO3DObjMgr_SetSampleFrames(FO3DViewer_SceneSampleFrames[scene], FO3DViewer_ScenePointCounts[scene]);
        gFO3DViewer->extraResources[slot][resIndex++] = FO3DRes_Create();
        FO3DRes_SetModel(model, j);
        FO3DObjMgr_SetModel(FO3DRes_GetCurrent());
        for (k = 0; k < animCount; k++) {
            gFO3DViewer->extraResources[slot][resIndex++] = FO3DRes_Create();
            FO3DRes_SetAnim(anims[k], j);
            FO3DObjMgr_AddAnim(FO3DRes_GetCurrent());
        }
    }
    if (texture == NULL) {
        return;
    }
    gFO3DViewer->extraResources[slot][resIndex] = FO3DRes_Create();
    FO3DRes_SetTexture(texture);
    for (j = 0; j < gFO3DViewer->extraModelCount[slot]; j++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[slot][j]);
        FO3DObjMgr_SetTexture(FO3DRes_GetCurrent());
    }
}

void FO3DViewer_Init(s32 scene) {
    s32 count;
    s32 i;

    FO3DViewerImd_Create();
    FOAllocator_Init();
    FO3DViewer_LoadScene(scene, FALSE);
    if (gFO3DViewer == NULL) {
        return;
    }
    count               = FO3DViewer_ScenePointCounts[scene];
    gFO3DViewer->points = FO3DViewerImd_Alloc(count * sizeof(FieldPos));
    gFO3DViewer->pos.x  = FO3DViewer_ScenePoints[scene][0] << FX32_SHIFT;
    gFO3DViewer->pos.y  = FO3DViewer_ScenePoints[scene][1] << FX32_SHIFT;
    for (i = 0; i < count; i++) {
        FieldPos* point = &gFO3DViewer->points[i];

        point->x = FO3DViewer_ScenePoints[scene][i * 2];
        point->y = FO3DViewer_ScenePoints[scene][i * 2 + 1];
        if (i != 0 && count < 25) {
            point->x -= gFO3DViewer->points[0].x;
            point->y -= gFO3DViewer->points[0].y;
        }
    }
}

BOOL FO3DViewer_Update(s32 x, s32 y) {
    BOOL moved = FALSE;
    BOOL changed;
    s32  oldX;
    s32  oldY;
    s32  i;

    changed = FALSE;
    oldX    = gFO3DViewer->pos.x;
    oldY    = gFO3DViewer->pos.y;
    if (x != oldX) {
        changed = TRUE;
    }
    if (y != oldY) {
        changed = TRUE;
    }
    if (changed) {
        gFO3DViewer->pos.x = x;
        gFO3DViewer->pos.y = y;
        if (FO3DObjMgr_UpdateBlendWeights(&gFO3DViewer->pos, gFO3DViewer->points)) {
            moved = TRUE;
        } else {
            gFO3DViewer->pos.x = oldX;
            gFO3DViewer->pos.y = oldY;
        }
    }
    for (i = 0; i < gFO3DViewer->modelCount; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->models[i]);
        FO3DObjMgr_UpdateAndDraw(&gFO3DViewer->pos, gFO3DViewer->points);
    }
    for (i = 0; i < gFO3DViewer->extraModelCount[0]; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[0][i]);
        FO3DObjMgr_UpdateAndDraw(&gFO3DViewer->pos, gFO3DViewer->points);
    }
    for (i = 0; i < gFO3DViewer->extraModelCount[1]; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[1][i]);
        FO3DObjMgr_UpdateAndDraw(&gFO3DViewer->pos, gFO3DViewer->points);
    }
    return moved;
}

void FO3DViewer_SetOffset(s32 x, s32 y) {
    Vec pos;
    s32 i;

    pos.x = x - (128 << FX32_SHIFT);
    pos.y = y - (96 << FX32_SHIFT);
    pos.z = 0;
    for (i = 0; i < gFO3DViewer->modelCount; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->models[i]);
        FO3DObjMgr_SetOffset(&pos);
    }
}

BOOL FO3DViewer_GetNodePos(const char* name, Vec* pos) {
    s32 i;

    for (i = 0; i < gFO3DViewer->modelCount; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->models[i]);
        if (FO3DObjMgr_GetNodePos(name, pos)) {
            return TRUE;
        }
    }
    if (FO3DViewer_GetExtraNodePos(name, pos, 0)) {
        return TRUE;
    }
    if (FO3DViewer_GetExtraNodePos(name, pos, 1)) {
        return TRUE;
    }
    return FALSE;
}

BOOL FO3DViewer_GetExtraNodePos(const char* name, Vec* pos, s32 slot) {
    s32 i;

    for (i = 0; i < gFO3DViewer->extraModelCount[slot]; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[slot][i]);
        if (FO3DObjMgr_GetNodePos(name, pos)) {
            return TRUE;
        }
    }
    return FALSE;
}

s32 FO3DViewer_GetAnimFrame(const char* name) {
    s32 i;

    for (i = 0; i < gFO3DViewer->modelCount; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->models[i]);
        if (FO3DObjMgr_IsNamed(name) && gFO3DViewer->models[i]->blendMode == 0 && !FO3DObjMgr_HasTexPatAnm()) {
            return FO3DObjMgr_GetFrame();
        }
    }
    for (i = 0; i < gFO3DViewer->extraModelCount[0]; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[0][i]);
        if (FO3DObjMgr_IsNamed(name) && gFO3DViewer->extraModels[0][i]->blendMode == 0 && !FO3DObjMgr_HasTexPatAnm()) {
            return FO3DObjMgr_GetFrame();
        }
    }
    for (i = 0; i < gFO3DViewer->extraModelCount[1]; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[1][i]);
        if (FO3DObjMgr_IsNamed(name) && gFO3DViewer->extraModels[1][i]->blendMode == 0 && !FO3DObjMgr_HasTexPatAnm()) {
            return FO3DObjMgr_GetFrame();
        }
    }
    return 0;
}

#define FO3D_VIEWER_FIND_AND(name, action)                         \
    do {                                                           \
        s32 i;                                                     \
                                                                   \
        for (i = 0; i < gFO3DViewer->modelCount; i++) {            \
            FO3DObjMgr_SetCurrent(gFO3DViewer->models[i]);         \
            if (FO3DObjMgr_IsNamed(name)) {                        \
                action();                                          \
                return;                                            \
            }                                                      \
        }                                                          \
        for (i = 0; i < gFO3DViewer->extraModelCount[0]; i++) {    \
            FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[0][i]); \
            if (FO3DObjMgr_IsNamed(name)) {                        \
                action();                                          \
                return;                                            \
            }                                                      \
        }                                                          \
        for (i = 0; i < gFO3DViewer->extraModelCount[1]; i++) {    \
            FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[1][i]); \
            if (FO3DObjMgr_IsNamed(name)) {                        \
                action();                                          \
                return;                                            \
            }                                                      \
        }                                                          \
    } while (0)

void FO3DViewer_RestartAnim(const char* name) {
    FO3D_VIEWER_FIND_AND(name, FO3DObjMgr_RestartAnim);
}

void FO3DViewer_Show(const char* name) {
    FO3D_VIEWER_FIND_AND(name, FO3DObjMgr_Show);
}

void FO3DViewer_Hide(const char* name) {
    FO3D_VIEWER_FIND_AND(name, FO3DObjMgr_Hide);
}

void FO3DViewer_FreeExtra(s32 slot) {
    s32 i;

    for (i = 0; i < gFO3DViewer->extraResourceCount[slot]; i++) {
        FO3DRes_SetCurrent(gFO3DViewer->extraResources[slot][i]);
        FO3DRes_Destroy();
    }
    for (i = 0; i < gFO3DViewer->extraModelCount[slot]; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->extraModels[slot][i]);
        FO3DObjMgr_Destroy();
    }
    if (gFO3DViewer->extraResources[slot] != NULL) {
        FO3DViewerImd_Free(gFO3DViewer->extraResources[slot]);
    }
    if (gFO3DViewer->extraModels[slot] != NULL) {
        FO3DViewerImd_Free(gFO3DViewer->extraModels[slot]);
    }
}

void FO3DViewer_Destroy(void) {
    s32 i;

    for (i = 0; i < gFO3DViewer->resourceCount; i++) {
        FO3DRes_SetCurrent(gFO3DViewer->resources[i]);
        FO3DRes_Destroy();
    }
    for (i = 0; i < gFO3DViewer->modelCount; i++) {
        FO3DObjMgr_SetCurrent(gFO3DViewer->models[i]);
        FO3DObjMgr_Destroy();
    }
    FO3DViewerImd_Free(gFO3DViewer->resources);
    FO3DViewerImd_Free(gFO3DViewer->models);
    FO3DViewerImd_Free(gFO3DViewer->points);
    FOFileIO_Release(gFO3DViewer->world.indexTable);
    FO3DViewer_FreeExtra(0);
    FO3DViewer_FreeExtra(1);
    FO3DViewerImd_Destroy();
}

void* FOFileIO_Alloc(s32 size) {
    void* data = Mem_AllocHeapTail(&gMainHeap, size);
    Mem_SetSequence(&gMainHeap, data, "FOFileIO_Alloc");
    return data;
}

void FOFileIO_Free(void* data) {
    Mem_Free(&gMainHeap, data);
}

void FOFileIO_Release(void* data) {
    FOFileIO_Free(data);
}
