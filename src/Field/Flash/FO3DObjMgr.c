#include "Field/FieldRegion.h"

#include "Field/FieldTypes.h"
#include "Field/Flash/FO3D.h"
#include "Field/Flash/FO3DObj.h"
#include "Field/Flash/FO3DObjMgr.h"
#include "Field/Flash/FOAllocator.h"
#include "Field/Flash/FOPtrList.h"

#include <string.h>

typedef struct {
    /* 0x00 */ FO3DModel* model;
    /* 0x04 */ BOOL       lineBlend;
    /* 0x08 */ s32        blendMode;
    /* 0x0C */ s32        unk_0C;
    /* 0x10 */ s32        lineStart;
    /* 0x14 */ s32        lineEnd;
    /* 0x18 */ s32        unk_18;
    /* 0x1C */ fx32       lineRatio;
    /* 0x20 */ s32        unk_20;
    /* 0x24 */ s32        sampleIdx[3];
    /* 0x30 */ fx32       sampleWeight[3];
} FO3DObjMgrState; // Size: 0x3C

typedef struct {
    /* 0x0 */ char name[16];
} NNSG3dResName;

typedef struct {
    /* 0x0 */ u32 texImageParam;
    /* 0x4 */ u32 extraParam;
} NNSG3dResDictTexData;

typedef struct {
    /* 0x0 */ u16 offset;
    /* 0x2 */ u16 flag;
} NNSG3dResDictPlttData;

typedef struct {
    /* 0x0 */ u16 idxFrame;
    /* 0x2 */ u8  idTex;
    /* 0x3 */ u8  idPltt;
} NNSG3dTexPatAnmFV;

FO3DObjMgrState gFO3DObjMgr;

extern void          func_02001b44(s32 type, u32 dst, const void* src, u32 size);
extern void          func_020281c0(NNSG3dAnmObj* anmObj, void* resAnm, NNSG3dResMdl* resMdl, NNSG3dResTex* resTex);
extern void          func_02028250(NNSG3dRenderObj* renderObj, NNSG3dResMdl* resMdl);
extern void          func_020283c4(NNSG3dRenderObj* renderObj, NNSG3dAnmObj* anmObj);
extern void          func_0202843c(NNSG3dRenderObj* renderObj, NNSG3dSbcCallBackFunc func, u8* addr, u8 cmd, u32 timing);
extern void          func_02028450(NNSG3dRenderObj* renderObj, NNSG3dSbcCallBackFunc func);
extern void          func_02028a44(void);
extern void          func_02028a80(Vec* trans);
extern void          func_02029270(NNSG3dRenderObj* renderObj);
extern void          func_0202b5d4(void);
extern NNSG3dAnmObj* func_0202bac0(void* allocator, void* resAnm, NNSG3dResMdl* resMdl);
extern void          func_0202bae4(void* allocator, NNSG3dAnmObj* anmObj);
extern const void*   func_0202baf0(const NNSG3dResDict* dict, const void* name);
extern const void*   func_0202bdd4(const void* resAnm, u32 idx);
extern const void*   func_0202bde4(const void* resAnm, u32 idx);
extern const NNSG3dTexPatAnmFV* func_0202bdf4(const void* resAnm, u16 idx, u16 frame);
extern s32                      func_020481e8(const char* str);

static const s32 sGridRing3x3[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

static const s32 sGridRings5x5[9][9] = {
    {0,  2,  3,  4,  5,  6,  7,  8,  1},
    {1, 11,  2,  3,  0,  7,  8,  9, 10},
    {2, 12, 13, 14,  3,  0,  1, 10, 11},
    {3, 13, 14, 15,  4,  5,  0,  1,  2},
    {4, 14, 15, 16, 17, 18,  5,  0,  3},
    {5,  3,  4, 17, 18, 19,  6,  7,  0},
    {6,  0,  5, 18, 19, 20, 21, 22,  7},
    {7,  1,  0,  5,  6, 21, 22, 23,  8},
    {8, 10,  1,  0,  7, 22, 23, 24,  9},
};
void FO3DObjMgr_CaptureMatColorSample(NNSG3dRS* rs);
void FO3DObjMgr_CaptureTexSrtSample(NNSG3dRS* rs);
void FO3DObjMgr_BlendTransAdditive(NNSG3dJntAnmResult* result, u32 node, s32 base);
void FO3DObjMgr_BlendScaleAdditive(NNSG3dJntAnmResult* result, s32 base);
void FO3DObjMgr_BlendRotAdditive(NNSG3dJntAnmResult* result, s32 base);
void FO3DObjMgr_BlendTrans(NNSG3dJntAnmResult* result, u32 node, s32 base);
void FO3DObjMgr_BlendScale(NNSG3dJntAnmResult* result, s32 base);
void FO3DObjMgr_BlendRot(NNSG3dJntAnmResult* result, s32 base);
u16  FO3DObjMgr_BlendColor(u16 c0, u16 c1, u16 c2);
BOOL FO3DObjMgr_CalcGridWeights(FieldPos* pos, FieldPos* points);
BOOL FO3DObjMgr_FindRingSegment(s32* outA, s32* outB, Vec* rel, FieldPos* points, const s32* ring, s32 count);
s32  FO3DObjMgr_FindNearestPoint(FieldPos* pos, FieldPos* points, const s32* list, s32 count);
BOOL FO3DObjMgr_CalcLineRatio(FieldPos* pos, FieldPos* points);
u8   FO3DObjMgr_GetNodeCount(void);
u8   FO3DObjMgr_GetMatCount(void);
void FO3DObjMgr_StepSampling(void);
void FO3DObjMgr_ApplyLineFrame(NNSG3dAnmObj* anmObj, fx32 frameMax);
void FO3DObjMgr_TexPatAnmFunc(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat);
void FO3DObjMgr_NullAnmFunc(void);

static inline fx32 FO3D_Mul32x64c(fx32 v32, s64 v64c) {
    return (fx32)((v64c * v32 + 0x80000000LL) >> 32);
}

void FO3DObjMgr_CaptureJointSample(NNSG3dRS* rs) {
    u32                 node;
    s32                 sample;
    NNSG3dJntAnmResult* result;
    s32                 base;
    long                idx;
    Vec*                trans;
    Vec*                scale;
    Mtx33*              rot;

    if (rs->flag & 0x10) {
        node = rs->currentNodeDesc;
    } else {
        node = -1;
    }
    result = rs->jntAnmResult;
    base   = gFO3DObjMgr.model->sampleCount;
    base   = base * node;
    sample = gFO3DObjMgr.model->sample - 1;
    idx    = base + sample;
    trans  = &gFO3DObjMgr.model->jntTrans[idx];
    if (result->flag & 4) {
        trans->x = 0;
        trans->y = 0;
        trans->z = 0;
    } else {
        *trans = result->trans;
    }
    if (sample == 0 && (gFO3DObjMgr.model->nodeMask & (1 << node)) &&
        (gFO3DObjMgr.model->anmObj[0]->mapData[node] & 0x300) == 0x100)
    {
        FOPtrList_Add(trans);
        FOPtrList_Add(&gFO3DObjMgr.model->nodeTrans[node]);
    }
    if (sample > 0 && gFO3DObjMgr.model->blendMode == 3) {
        Vec* first = &gFO3DObjMgr.model->jntTrans[base];

        trans->x -= first->x;
        trans->y -= first->y;
        trans->z -= first->z;
    }
    scale = &gFO3DObjMgr.model->jntScale[idx];
    if (result->flag & 1) {
        scale->x = FX32_ONE;
        scale->y = FX32_ONE;
        scale->z = FX32_ONE;
    } else {
        *scale = result->scale;
    }
    if (sample > 0 && gFO3DObjMgr.model->blendMode == 3) {
        Vec* first = &gFO3DObjMgr.model->jntScale[base];

        scale->x -= first->x;
        scale->y -= first->y;
        scale->z -= first->z;
    }
    rot = &gFO3DObjMgr.model->jntRot[idx];
    if (result->flag & 2) {
        Mtx_Identity33(rot);
    } else {
        *rot = result->rot;
    }
    if (sample > 0 && gFO3DObjMgr.model->blendMode == 3) {
        Mtx33* first = &gFO3DObjMgr.model->jntRot[base];

        rot->_00 -= first->_00;
        rot->_01 -= first->_01;
        rot->_02 -= first->_02;
        rot->_10 -= first->_10;
        rot->_11 -= first->_11;
        rot->_12 -= first->_12;
        rot->_20 -= first->_20;
        rot->_21 -= first->_21;
        rot->_22 -= first->_22;
    }
}

void FO3DObjMgr_CaptureMaterialSample(NNSG3dRS* rs) {
    if (gFO3DObjMgr.model->matAmbient != NULL) {
        FO3DObjMgr_CaptureMatColorSample(rs);
    }
    if (gFO3DObjMgr.model->texScaleS != NULL) {
        FO3DObjMgr_CaptureTexSrtSample(rs);
    }
}

void FO3DObjMgr_CaptureMatColorSample(NNSG3dRS* rs) {
    u32                 mat;
    NNSG3dMatAnmResult* result;
    s32                 base;
    s32                 sample;
    s32                 idx;

    if (rs->flag & 8) {
        mat = rs->currentMat;
    } else {
        mat = -1;
    }
    result = rs->matAnmResult;
    base   = gFO3DObjMgr.model->sampleCount * mat;
    sample = gFO3DObjMgr.model->sample - 1;
    if (result == NULL) {
        return;
    }
    idx                                = base + sample;
    gFO3DObjMgr.model->matAmbient[idx] = (result->prmMatColor0 >> 16) & 0x7FFF;
    gFO3DObjMgr.model->matDiffuse[idx] = result->prmMatColor0 & 0x7FFF;
    gFO3DObjMgr.model->matAlpha[idx]   = (result->prmPolygonAttr >> 16) & 0x1F;
}

void FO3DObjMgr_CaptureTexSrtSample(NNSG3dRS* rs) {
    u32                 mat;
    NNSG3dMatAnmResult* result;
    s32                 base;
    s32                 sample;
    s32                 idx;
    fx32*               scaleS;
    fx32*               scaleT;
    s16*                rotSin;
    s16*                rotCos;
    fx32*               transS;
    fx32*               transT;

    if (rs->flag & 8) {
        mat = rs->currentMat;
    } else {
        mat = -1;
    }
    result = rs->matAnmResult;
    base   = gFO3DObjMgr.model->sampleCount * mat;
    sample = gFO3DObjMgr.model->sample - 1;
    if (result == NULL) {
        return;
    }
    idx    = base + sample;
    scaleS = gFO3DObjMgr.model->texScaleS;
    scaleT = gFO3DObjMgr.model->texScaleT;
    if (result->flag & 1) {
        scaleS[idx] = FX32_ONE;
        scaleT[idx] = FX32_ONE;
    } else {
        scaleS[idx] = result->scaleS;
        scaleT[idx] = result->scaleT;
    }
    rotSin = &gFO3DObjMgr.model->texRotSin[idx];
    rotCos = &gFO3DObjMgr.model->texRotCos[idx];
    if (result->flag & 2) {
        *rotSin = 0;
        *rotCos = FX32_ONE;
    } else {
        *rotSin = result->rotSin;
        if (*rotSin == 0) {
            *rotCos = FX32_ONE;
        } else {
            *rotCos = result->rotCos;
        }
    }
    transS = gFO3DObjMgr.model->texTransS;
    transT = gFO3DObjMgr.model->texTransT;
    if (result->flag & 4) {
        transS[idx] = 0;
        transT[idx] = 0;
        return;
    }
    transS[idx] = result->transS;
    transT[idx] = result->transT;
}

void FO3DObjMgr_StoreNodeTrans(NNSG3dRS* rs) {
    NNSG3dJntAnmResult* result;
    u32                 node;
    Vec*                trans;

    node   = (rs->flag & 0x10) ? rs->currentNodeDesc : -1;
    result = rs->jntAnmResult;
    trans  = &gFO3DObjMgr.model->nodeTrans[node];
    if (result->flag & 4) {
        trans->x = 0;
        trans->y = 0;
        trans->z = 0;
    } else {
        *trans = result->trans;
    }
}

BOOL FO3DObjMgr_BlendJointAnm(NNSG3dJntAnmResult* result, NNSG3dAnmObj* anmObj, u32 node) {
    s32 base;

    if (anmObj->next != NULL) {
        return FALSE;
    }
    if ((anmObj->mapData[node] & 0x300) != 0x100) {
        return FALSE;
    }
    base = gFO3DObjMgr.model->sampleCount * node;
    if (gFO3DObjMgr.model->blendMode == 3) {
        FO3DObjMgr_BlendTransAdditive(result, node, base);
        FO3DObjMgr_BlendScaleAdditive(result, base);
        FO3DObjMgr_BlendRotAdditive(result, base);
    } else {
        FO3DObjMgr_BlendTrans(result, node, base);
        FO3DObjMgr_BlendScale(result, base);
        FO3DObjMgr_BlendRot(result, base);
    }
    result->flag = 0;
    return TRUE;
}

void FO3DObjMgr_BlendTransAdditive(NNSG3dJntAnmResult* result, u32 node, s32 base) {
    Vec* trans = gFO3DObjMgr.model->jntTrans;
    Vec* p1    = &trans[base + gFO3DObjMgr.sampleIdx[1]];
    Vec* p2    = &trans[base + gFO3DObjMgr.sampleIdx[2]];

    result->trans = trans[base];
    result->trans.x += FX_Mul(p1->x, gFO3DObjMgr.sampleWeight[1]);
    result->trans.x += FX_Mul(p2->x, gFO3DObjMgr.sampleWeight[2]);
    result->trans.y += FX_Mul(p1->y, gFO3DObjMgr.sampleWeight[1]);
    result->trans.y += FX_Mul(p2->y, gFO3DObjMgr.sampleWeight[2]);
    result->trans.z += FX_Mul(p1->z, gFO3DObjMgr.sampleWeight[1]);
    result->trans.z += FX_Mul(p2->z, gFO3DObjMgr.sampleWeight[2]);
    gFO3DObjMgr.model->nodeTrans[node] = result->trans;
}

void FO3DObjMgr_BlendScaleAdditive(NNSG3dJntAnmResult* result, s32 base) {
    Vec* scale = gFO3DObjMgr.model->jntScale;
    Vec* p1    = &scale[base + gFO3DObjMgr.sampleIdx[1]];
    Vec* p2    = &scale[base + gFO3DObjMgr.sampleIdx[2]];

    result->scale = scale[base];
    result->scale.x += FX_Mul(p1->x, gFO3DObjMgr.sampleWeight[1]);
    result->scale.x += FX_Mul(p2->x, gFO3DObjMgr.sampleWeight[2]);
    result->scale.y += FX_Mul(p1->y, gFO3DObjMgr.sampleWeight[1]);
    result->scale.y += FX_Mul(p2->y, gFO3DObjMgr.sampleWeight[2]);
    result->scale.z += FX_Mul(p1->z, gFO3DObjMgr.sampleWeight[1]);
    result->scale.z += FX_Mul(p2->z, gFO3DObjMgr.sampleWeight[2]);
}

void FO3DObjMgr_BlendRotAdditive(NNSG3dJntAnmResult* result, s32 base) {
    Mtx33* rot = gFO3DObjMgr.model->jntRot;
    Mtx33* p1  = &rot[base + gFO3DObjMgr.sampleIdx[1]];
    Mtx33* p2  = &rot[base + gFO3DObjMgr.sampleIdx[2]];

    result->rot = rot[base];
    result->rot._00 += FX_Mul(p1->_00, gFO3DObjMgr.sampleWeight[1]);
    result->rot._00 += FX_Mul(p2->_00, gFO3DObjMgr.sampleWeight[2]);
    result->rot._01 += FX_Mul(p1->_01, gFO3DObjMgr.sampleWeight[1]);
    result->rot._01 += FX_Mul(p2->_01, gFO3DObjMgr.sampleWeight[2]);
    result->rot._02 += FX_Mul(p1->_02, gFO3DObjMgr.sampleWeight[1]);
    result->rot._02 += FX_Mul(p2->_02, gFO3DObjMgr.sampleWeight[2]);
    result->rot._10 += FX_Mul(p1->_10, gFO3DObjMgr.sampleWeight[1]);
    result->rot._10 += FX_Mul(p2->_10, gFO3DObjMgr.sampleWeight[2]);
    result->rot._11 += FX_Mul(p1->_11, gFO3DObjMgr.sampleWeight[1]);
    result->rot._11 += FX_Mul(p2->_11, gFO3DObjMgr.sampleWeight[2]);
    result->rot._12 += FX_Mul(p1->_12, gFO3DObjMgr.sampleWeight[1]);
    result->rot._12 += FX_Mul(p2->_12, gFO3DObjMgr.sampleWeight[2]);
    Vec_CrossProduct((Vec*)&result->rot._00, (Vec*)&result->rot._10, (Vec*)&result->rot._20);
}

void FO3DObjMgr_BlendTrans(NNSG3dJntAnmResult* result, u32 node, s32 base) {
    Vec p0;
    Vec p1;
    Vec p2;

    p0 = gFO3DObjMgr.model->jntTrans[base + gFO3DObjMgr.sampleIdx[0]];
    p1 = gFO3DObjMgr.model->jntTrans[base + gFO3DObjMgr.sampleIdx[1]];
    p2 = gFO3DObjMgr.model->jntTrans[base + gFO3DObjMgr.sampleIdx[2]];
    p1.x -= p0.x;
    p1.y -= p0.y;
    p1.z -= p0.z;
    p2.x -= p0.x;
    p2.y -= p0.y;
    p2.z -= p0.z;
    result->trans = p0;
    result->trans.x += FX_Mul(p1.x, gFO3DObjMgr.sampleWeight[1]);
    result->trans.x += FX_Mul(p2.x, gFO3DObjMgr.sampleWeight[2]);
    result->trans.y += FX_Mul(p1.y, gFO3DObjMgr.sampleWeight[1]);
    result->trans.y += FX_Mul(p2.y, gFO3DObjMgr.sampleWeight[2]);
    result->trans.z += FX_Mul(p1.z, gFO3DObjMgr.sampleWeight[1]);
    result->trans.z += FX_Mul(p2.z, gFO3DObjMgr.sampleWeight[2]);
    gFO3DObjMgr.model->nodeTrans[node] = result->trans;
}

void FO3DObjMgr_BlendScale(NNSG3dJntAnmResult* result, s32 base) {
    Vec p0;
    Vec p1;
    Vec p2;

    p0 = gFO3DObjMgr.model->jntScale[base + gFO3DObjMgr.sampleIdx[0]];
    p1 = gFO3DObjMgr.model->jntScale[base + gFO3DObjMgr.sampleIdx[1]];
    p2 = gFO3DObjMgr.model->jntScale[base + gFO3DObjMgr.sampleIdx[2]];
    p1.x -= p0.x;
    p1.y -= p0.y;
    p1.z -= p0.z;
    p2.x -= p0.x;
    p2.y -= p0.y;
    p2.z -= p0.z;
    result->scale = p0;
    result->scale.x += FX_Mul(p1.x, gFO3DObjMgr.sampleWeight[1]);
    result->scale.x += FX_Mul(p2.x, gFO3DObjMgr.sampleWeight[2]);
    result->scale.y += FX_Mul(p1.y, gFO3DObjMgr.sampleWeight[1]);
    result->scale.y += FX_Mul(p2.y, gFO3DObjMgr.sampleWeight[2]);
    result->scale.z += FX_Mul(p1.z, gFO3DObjMgr.sampleWeight[1]);
    result->scale.z += FX_Mul(p2.z, gFO3DObjMgr.sampleWeight[2]);
}

void FO3DObjMgr_BlendRot(NNSG3dJntAnmResult* result, s32 base) {
    Mtx33* rot = gFO3DObjMgr.model->jntRot;
    Mtx33* r0  = &rot[base + gFO3DObjMgr.sampleIdx[0]];
    Mtx33* r1  = &rot[base + gFO3DObjMgr.sampleIdx[1]];
    Mtx33* r2  = &rot[base + gFO3DObjMgr.sampleIdx[2]];

    result->rot = *r0;
    result->rot._00 += FX_Mul(r1->_00 - r0->_00, gFO3DObjMgr.sampleWeight[1]);
    result->rot._00 += FX_Mul(r2->_00 - r0->_00, gFO3DObjMgr.sampleWeight[2]);
    result->rot._01 += FX_Mul(r1->_01 - r0->_01, gFO3DObjMgr.sampleWeight[1]);
    result->rot._01 += FX_Mul(r2->_01 - r0->_01, gFO3DObjMgr.sampleWeight[2]);
    result->rot._02 += FX_Mul(r1->_02 - r0->_02, gFO3DObjMgr.sampleWeight[1]);
    result->rot._02 += FX_Mul(r2->_02 - r0->_02, gFO3DObjMgr.sampleWeight[2]);
    result->rot._10 += FX_Mul(r1->_10 - r0->_10, gFO3DObjMgr.sampleWeight[1]);
    result->rot._10 += FX_Mul(r2->_10 - r0->_10, gFO3DObjMgr.sampleWeight[2]);
    result->rot._11 += FX_Mul(r1->_11 - r0->_11, gFO3DObjMgr.sampleWeight[1]);
    result->rot._11 += FX_Mul(r2->_11 - r0->_11, gFO3DObjMgr.sampleWeight[2]);
    result->rot._12 += FX_Mul(r1->_12 - r0->_12, gFO3DObjMgr.sampleWeight[1]);
    result->rot._12 += FX_Mul(r2->_12 - r0->_12, gFO3DObjMgr.sampleWeight[2]);
    Vec_CrossProduct((Vec*)&result->rot._00, (Vec*)&result->rot._10, (Vec*)&result->rot._20);
}

// Nonmatching: register allocation; alpha field insert uses and+lsl in the target
BOOL FO3DObjMgr_BlendMatColorAnm(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat) {
    u32 color;
    s32 base;
    Vec a;

    base  = gFO3DObjMgr.model->sampleCount * mat;
    color = result->prmMatColor0 & 0x80008000;
    color |= (FO3DObjMgr_BlendColor(gFO3DObjMgr.model->matAmbient[base + gFO3DObjMgr.sampleIdx[0]],
                                    gFO3DObjMgr.model->matAmbient[base + gFO3DObjMgr.sampleIdx[1]],
                                    gFO3DObjMgr.model->matAmbient[base + gFO3DObjMgr.sampleIdx[2]])
              << 16) &
             0x7FFF0000;
    result->prmMatColor0 = color | (FO3DObjMgr_BlendColor(gFO3DObjMgr.model->matDiffuse[base + gFO3DObjMgr.sampleIdx[0]],
                                                          gFO3DObjMgr.model->matDiffuse[base + gFO3DObjMgr.sampleIdx[1]],
                                                          gFO3DObjMgr.model->matDiffuse[base + gFO3DObjMgr.sampleIdx[2]]) &
                                    0x7FFF);
    a.x                  = gFO3DObjMgr.model->matAlpha[base + gFO3DObjMgr.sampleIdx[0]] << FX32_SHIFT;
    a.y                  = gFO3DObjMgr.model->matAlpha[base + gFO3DObjMgr.sampleIdx[1]] << FX32_SHIFT;
    a.z                  = gFO3DObjMgr.model->matAlpha[base + gFO3DObjMgr.sampleIdx[2]] << FX32_SHIFT;
    result->prmPolygonAttr =
        (result->prmPolygonAttr & ~0x1F0000) |
        ((((a.x + FX_Mul(a.y - a.x, gFO3DObjMgr.sampleWeight[1]) + FX_Mul(a.z - a.x, gFO3DObjMgr.sampleWeight[2])) >>
           FX32_SHIFT) &
          0x1F)
         << 16);
    return TRUE;
}

static inline fx32 FO3D_Lerp3(fx32 v0, fx32 v1, fx32 v2) {
    return v0 + FX_Mul(v1 - v0, gFO3DObjMgr.sampleWeight[1]) + FX_Mul(v2 - v0, gFO3DObjMgr.sampleWeight[2]);
}

// Nonmatching: channel scheduling and register allocation
u16 FO3DObjMgr_BlendColor(u16 c0, u16 c1, u16 c2) {
    Vec v0;
    Vec v1;
    Vec v2;
    s32 r;
    s32 g;
    s32 b;

    v0.x = (c0 & 0x1F) << FX32_SHIFT;
    v0.y = ((c0 & 0x3E0) >> 5) << FX32_SHIFT;
    v0.z = ((c0 & 0x7C00) >> 10) << FX32_SHIFT;
    v1.x = (c1 & 0x1F) << FX32_SHIFT;
    v1.y = ((c1 & 0x3E0) >> 5) << FX32_SHIFT;
    v1.z = ((c1 & 0x7C00) >> 10) << FX32_SHIFT;
    v2.x = (c2 & 0x1F) << FX32_SHIFT;
    v2.y = ((c2 & 0x3E0) >> 5) << FX32_SHIFT;
    v2.z = ((c2 & 0x7C00) >> 10) << FX32_SHIFT;
    r    = (v0.x + FX_Mul(v1.x - v0.x, gFO3DObjMgr.sampleWeight[1]) + FX_Mul(v2.x - v0.x, gFO3DObjMgr.sampleWeight[2])) >>
        FX32_SHIFT;
    g = (v0.y + FX_Mul(v1.y - v0.y, gFO3DObjMgr.sampleWeight[1]) + FX_Mul(v2.y - v0.y, gFO3DObjMgr.sampleWeight[2])) >>
        FX32_SHIFT;
    b = (v0.z + FX_Mul(v1.z - v0.z, gFO3DObjMgr.sampleWeight[1]) + FX_Mul(v2.z - v0.z, gFO3DObjMgr.sampleWeight[2])) >>
        FX32_SHIFT;
    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

// Nonmatching: register allocation
BOOL FO3DObjMgr_BlendTexSrtAnm(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat) {
    s32 base = gFO3DObjMgr.model->sampleCount * mat;
    s32 v0;
    s32 v1;
    s32 v2;

    v0             = gFO3DObjMgr.model->texScaleS[base + gFO3DObjMgr.sampleIdx[0]];
    v1             = gFO3DObjMgr.model->texScaleS[base + gFO3DObjMgr.sampleIdx[1]];
    v2             = gFO3DObjMgr.model->texScaleS[base + gFO3DObjMgr.sampleIdx[2]];
    result->scaleS = v0;
    result->scaleS += FX_Mul(v1 - v0, gFO3DObjMgr.sampleWeight[1]);
    result->scaleS += FX_Mul(v2 - v0, gFO3DObjMgr.sampleWeight[2]);
    v0             = gFO3DObjMgr.model->texScaleT[base + gFO3DObjMgr.sampleIdx[0]];
    v1             = gFO3DObjMgr.model->texScaleT[base + gFO3DObjMgr.sampleIdx[1]];
    v2             = gFO3DObjMgr.model->texScaleT[base + gFO3DObjMgr.sampleIdx[2]];
    result->scaleT = v0;
    result->scaleT += FX_Mul(v1 - v0, gFO3DObjMgr.sampleWeight[1]);
    result->scaleT += FX_Mul(v2 - v0, gFO3DObjMgr.sampleWeight[2]);
    result->flag &= ~1;
    v0             = gFO3DObjMgr.model->texRotSin[base + gFO3DObjMgr.sampleIdx[0]];
    v1             = gFO3DObjMgr.model->texRotSin[base + gFO3DObjMgr.sampleIdx[1]];
    v2             = gFO3DObjMgr.model->texRotSin[base + gFO3DObjMgr.sampleIdx[2]];
    result->rotSin = v0 + FX_Mul(v1 - v0, gFO3DObjMgr.sampleWeight[1]) + FX_Mul(v2 - v0, gFO3DObjMgr.sampleWeight[2]);
    v0             = gFO3DObjMgr.model->texRotCos[base + gFO3DObjMgr.sampleIdx[0]];
    v1             = gFO3DObjMgr.model->texRotCos[base + gFO3DObjMgr.sampleIdx[1]];
    v2             = gFO3DObjMgr.model->texRotCos[base + gFO3DObjMgr.sampleIdx[2]];
    result->rotCos = v0 + FX_Mul(v1 - v0, gFO3DObjMgr.sampleWeight[1]) + FX_Mul(v2 - v0, gFO3DObjMgr.sampleWeight[2]);
    result->flag &= ~2;
    v0             = gFO3DObjMgr.model->texTransS[base + gFO3DObjMgr.sampleIdx[0]];
    v1             = gFO3DObjMgr.model->texTransS[base + gFO3DObjMgr.sampleIdx[1]];
    v2             = gFO3DObjMgr.model->texTransS[base + gFO3DObjMgr.sampleIdx[2]];
    result->transS = v0;
    result->transS += FX_Mul(v1 - v0, gFO3DObjMgr.sampleWeight[1]);
    result->transS += FX_Mul(v2 - v0, gFO3DObjMgr.sampleWeight[2]);
    v0             = gFO3DObjMgr.model->texTransT[base + gFO3DObjMgr.sampleIdx[0]];
    v1             = gFO3DObjMgr.model->texTransT[base + gFO3DObjMgr.sampleIdx[1]];
    v2             = gFO3DObjMgr.model->texTransT[base + gFO3DObjMgr.sampleIdx[2]];
    result->transT = v0;
    result->transT += FX_Mul(v1 - v0, gFO3DObjMgr.sampleWeight[1]);
    result->transT += FX_Mul(v2 - v0, gFO3DObjMgr.sampleWeight[2]);
    result->flag &= ~4;
    result->prmTexImage = (result->prmTexImage & ~0xC0000000) | 0x40000000;
    result->flag |= 8;
    return TRUE;
}

FO3DModel* FO3DObjMgr_Create(void) {
    s32 i;

    gFO3DObjMgr.model             = FO3DObj_Alloc(sizeof(FO3DModel));
    gFO3DObjMgr.model->obj        = FO3DObj_Create();
    gFO3DObjMgr.model->resMdl     = NULL;
    gFO3DObjMgr.model->resTex     = NULL;
    gFO3DObjMgr.model->jntTrans   = NULL;
    gFO3DObjMgr.model->jntScale   = NULL;
    gFO3DObjMgr.model->jntRot     = NULL;
    gFO3DObjMgr.model->matAmbient = NULL;
    gFO3DObjMgr.model->matDiffuse = NULL;
    gFO3DObjMgr.model->matAlpha   = NULL;
    gFO3DObjMgr.model->texScaleS  = NULL;
    gFO3DObjMgr.model->texScaleT  = NULL;
    gFO3DObjMgr.model->texRotSin  = NULL;
    gFO3DObjMgr.model->texRotCos  = NULL;
    gFO3DObjMgr.model->texTransS  = NULL;
    gFO3DObjMgr.model->texTransT  = NULL;
    for (i = 0; i < 3; i++) {
        gFO3DObjMgr.model->anmObj[i]  = NULL;
        gFO3DObjMgr.model->anmType[i] = 0;
    }
    gFO3DObjMgr.model->nodeTrans  = NULL;
    gFO3DObjMgr.model->offset.z   = 0;
    gFO3DObjMgr.model->offset.y   = gFO3DObjMgr.model->offset.z;
    gFO3DObjMgr.model->offset.x   = gFO3DObjMgr.model->offset.y;
    gFO3DObjMgr.model->nodeMask   = 0;
    gFO3DObjMgr.model->blendMode  = 0;
    gFO3DObjMgr.model->sample     = 0;
    gFO3DObjMgr.model->anmCount   = 0;
    gFO3DObjMgr.model->animating  = TRUE;
    gFO3DObjMgr.model->visible    = TRUE;
    gFO3DObjMgr.model->offsetMode = 0;
    gFO3DObjMgr.sampleIdx[0]      = 0;
    gFO3DObjMgr.sampleIdx[1]      = 0;
    gFO3DObjMgr.sampleIdx[2]      = 0;
    gFO3DObjMgr.sampleWeight[0]   = 0;
    gFO3DObjMgr.sampleWeight[1]   = 0;
    gFO3DObjMgr.sampleWeight[2]   = 0;
    return gFO3DObjMgr.model;
}

void FO3DObjMgr_Destroy(void) {
    s32 i;

    FO3DObj_SetCurrent(gFO3DObjMgr.model->obj);
    FO3DObj_Destroy();
    for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
        NNSG3dAnmObj* anmObj = gFO3DObjMgr.model->anmObj[i];

        if (anmObj != NULL) {
            func_0202bae4(FOAllocator_Get(), anmObj);
        }
    }
    FO3DObj_Free(gFO3DObjMgr.model->nodeTrans);
    FO3DObj_Free(gFO3DObjMgr.model->jntTrans);
    FO3DObj_Free(gFO3DObjMgr.model->jntScale);
    FO3DObj_Free(gFO3DObjMgr.model->jntRot);
    if (gFO3DObjMgr.model->matAmbient != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->matAmbient);
    }
    if (gFO3DObjMgr.model->matDiffuse != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->matDiffuse);
    }
    if (gFO3DObjMgr.model->matAlpha != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->matAlpha);
    }
    if (gFO3DObjMgr.model->texScaleS != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->texScaleS);
    }
    if (gFO3DObjMgr.model->texScaleT != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->texScaleT);
    }
    if (gFO3DObjMgr.model->texRotSin != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->texRotSin);
    }
    if (gFO3DObjMgr.model->texRotCos != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->texRotCos);
    }
    if (gFO3DObjMgr.model->texTransS != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->texTransS);
    }
    if (gFO3DObjMgr.model->texTransT != NULL) {
        FO3DObj_Free(gFO3DObjMgr.model->texTransT);
    }
    gFO3DObjMgr.lineBlend = FALSE;
    FO3DObj_Free(gFO3DObjMgr.model);
    gFO3DObjMgr.model = NULL;
}

void FO3DObjMgr_SetCurrent(FO3DModel* model) {
    gFO3DObjMgr.model = model;
}

void FO3DObjMgr_SetSampleFrames(const s32* frames, s32 count) {
    if (gFO3DObjMgr.model == NULL) {
        return;
    }
    gFO3DObjMgr.model->sampleCount  = count;
    gFO3DObjMgr.model->sampleFrames = frames;
    if (gFO3DObjMgr.model->sampleCount == 1) {
        gFO3DObjMgr.model->blendMode = 0;
        func_0202843c(&gFO3DObjMgr.model->renderObj, FO3DObjMgr_StoreNodeTrans, NULL, 6, 2);
    } else if (gFO3DObjMgr.model->sampleCount >= 25) {
        gFO3DObjMgr.model->blendMode   = 4;
        gFO3DObjMgr.model->sampleCount = 25;
    } else if (gFO3DObjMgr.model->sampleCount >= 9) {
        gFO3DObjMgr.model->blendMode   = 3;
        gFO3DObjMgr.model->sampleCount = 9;
    } else {
        gFO3DObjMgr.model->blendMode = 2;
        gFO3DObjMgr.lineStart        = 1;
        gFO3DObjMgr.lineEnd          = 2;
    }
    gFO3DObjMgr.blendMode = gFO3DObjMgr.model->blendMode;
}

BOOL FO3DObjMgr_UpdateBlendWeights(FieldPos* pos, FieldPos* points) {
    BOOL result = TRUE;

    switch (gFO3DObjMgr.blendMode) {
        case 0:
        case 1:
            break;
        case 2:
            result = FO3DObjMgr_CalcLineRatio(pos, points);
            break;
        case 3:
        case 4:
            result = FO3DObjMgr_CalcGridWeights(pos, points);
            if (gFO3DObjMgr.lineBlend) {
                FO3DObjMgr_CalcLineRatio(pos, points);
            }
            break;
    }
    return result;
}

// Nonmatching: register allocation
BOOL FO3DObjMgr_CalcGridWeights(FieldPos* pos, FieldPos* points) {
    FieldPos   ring[25];
    Vec        rel;
    Vec        edge;
    Vec        toOrigin;
    Vec        toPos;
    Vec        perpOrigin;
    Vec        perpPos;
    Vec        hit;
    s32        a;
    s32        b;
    s32        center = 0;
    s32        i;
    const s32* order;
    FieldPos*  first;
    FieldPos*  pa;
    FieldPos*  pb;
    s64        t;
    fx32       lenSq;
    fx32       distOrigin;
    fx32       distPos;
    fx32       ax;
    fx32       ay;
    fx32       bx;
    fx32       by;
    fx32       u;
    fx32       v;

    rel.x = pos->x - (points[0].x << FX32_SHIFT);
    rel.y = pos->y - (points[0].y << FX32_SHIFT);
    rel.z = 0;
    if (gFO3DObjMgr.blendMode == 3) {
        FO3DObjMgr_FindRingSegment(&a, &b, &rel, points, sGridRing3x3, gFO3DObjMgr.model->sampleCount);
    } else if (gFO3DObjMgr.blendMode == 4) {
        center         = FO3DObjMgr_FindNearestPoint(pos, points, sGridRing3x3, 9);
        order          = sGridRings5x5[center];
        first          = &points[order[0]];
        ring[center].x = first->x;
        ring[center].y = first->y;
        for (i = 1; i < 9; i++) {
            FieldPos* p = &points[order[i]];

            ring[order[i]].x = p->x - first->x;
            ring[order[i]].y = p->y - first->y;
        }
        first  = &ring[center];
        rel.x  = pos->x - (first->x << FX32_SHIFT);
        rel.y  = pos->y - (first->y << FX32_SHIFT);
        points = ring;
        FO3DObjMgr_FindRingSegment(&a, &b, &rel, points, order, 9);
    } else {
        return center;
    }
    pa     = &points[a];
    pb     = &points[b];
    edge.x = pb->x - pa->x;
    edge.y = pb->y - pa->y;
    ax     = pa->x;
    ay     = pa->y;
    if ((rel.x - (ax << FX32_SHIFT)) * edge.y - (rel.y - (ay << FX32_SHIFT)) * edge.x >= 0) {
        return FALSE;
    }
    bx                          = pb->x << FX32_SHIFT;
    by                          = pb->y << FX32_SHIFT;
    edge.x                      = bx - (ax << FX32_SHIFT);
    edge.y                      = by - (ay << FX32_SHIFT);
    lenSq                       = FX_Mul(edge.x, edge.x) + FX_Mul(edge.y, edge.y);
    edge.z                      = 0;
    toOrigin.x                  = -bx;
    toOrigin.y                  = -by;
    toOrigin.z                  = 0;
    toPos.x                     = rel.x - bx;
    toPos.y                     = rel.y - by;
    toPos.z                     = 0;
    t                           = FX_Divide64(Vec_DotProduct(&edge, &toOrigin), lenSq);
    perpOrigin.x                = FO3D_Mul32x64c(edge.x, t) - toOrigin.x;
    perpOrigin.y                = FO3D_Mul32x64c(edge.y, t) - toOrigin.y;
    perpOrigin.z                = 0;
    distOrigin                  = Vec_Magnitude(&perpOrigin);
    t                           = FX_Divide64(Vec_DotProduct(&edge, &toPos), lenSq);
    perpPos.x                   = FO3D_Mul32x64c(edge.x, t) - toPos.x;
    perpPos.y                   = FO3D_Mul32x64c(edge.y, t) - toPos.y;
    perpPos.z                   = 0;
    distPos                     = Vec_Magnitude(&perpPos);
    t                           = FX_Divide64(distPos, distOrigin - distPos);
    hit.x                       = bx - (rel.x + FO3D_Mul32x64c(rel.x, t));
    hit.y                       = by - (rel.y + FO3D_Mul32x64c(rel.y, t));
    hit.z                       = 0;
    u                           = FX_Divide(Vec_Magnitude(&hit), Vec_Magnitude(&edge));
    v                           = FX_Divide(distPos, distOrigin);
    gFO3DObjMgr.sampleIdx[0]    = center;
    gFO3DObjMgr.sampleIdx[1]    = a;
    gFO3DObjMgr.sampleIdx[2]    = b;
    gFO3DObjMgr.sampleWeight[0] = FX32_ONE;
    gFO3DObjMgr.sampleWeight[1] = FX_Mul(u, FX32_ONE - v);
    gFO3DObjMgr.sampleWeight[2] = FX_Mul(FX32_ONE - u, FX32_ONE - v);
    return TRUE;
}

BOOL FO3DObjMgr_FindRingSegment(s32* outA, s32* outB, Vec* rel, FieldPos* points, const s32* ring, s32 count) {
    s32       side;
    s32       i;
    FieldPos* p;

    p = &points[ring[1]];
    if (rel->x * p->y - rel->y * p->x >= 0) {
        side = 1;
    } else {
        side = -1;
    }
    *outB = -1;
    *outA = -1;
    for (i = 2; i < count; i++) {
        p = &points[ring[i]];
        if (rel->x * p->y - rel->y * p->x >= 0) {
            if (side < 0) {
                *outA = ring[i - 1];
                *outB = ring[i];
                break;
            }
            side = 1;
        } else {
            side = -1;
        }
    }
    if (*outA >= 0) {
        return TRUE;
    }
    *outA = ring[count - 1];
    *outB = ring[1];
    return FALSE;
}

s32 FO3DObjMgr_FindNearestPoint(FieldPos* pos, FieldPos* points, const s32* list, s32 count) {
    Vec delta;
    s32 nearest;
    s32 best = 0x7FFFFFFF;
    s32 i;

    for (i = 0; i < count; i++) {
        s32       idx = list[i];
        FieldPos* p   = &points[idx];
        s32       dist;

        delta.x = (p->x << FX32_SHIFT) - pos->x;
        delta.y = (p->y << FX32_SHIFT) - pos->y;
        delta.z = 0;
        dist    = Vec_Magnitude(&delta);
        if (best > dist) {
            best    = dist;
            nearest = idx;
        }
    }
    return nearest;
}

// Nonmatching: load scheduling and register allocation
BOOL FO3DObjMgr_CalcLineRatio(FieldPos* pos, FieldPos* points) {
    Vec  dir;
    Vec  proj;
    s32  start = gFO3DObjMgr.lineStart;
    s32  end   = gFO3DObjMgr.lineEnd;
    s32  x0    = points[0].x;
    s32  y0    = points[0].y;
    s32  sx    = points[start].x;
    s32  sy    = points[start].y;
    s32  ex    = points[end].x;
    s32  ey    = points[end].y;
    s32  px    = (pos->x >> FX32_SHIFT) - x0;
    s32  py    = (pos->y >> FX32_SHIFT) - y0;
    fx32 len;
    s64  t;

    if (start != 1) {
        sx -= x0;
        sy -= y0;
    }
    if (end != 2) {
        ey -= y0;
        ex -= x0;
    }
    dir.x                 = ex - sx;
    dir.y                 = ey - sy;
    dir.z                 = 0;
    len                   = Vec_Magnitude(&dir);
    t                     = FX_Divide64(dir.x * (px - sx) + dir.y * (py - sy), dir.x * dir.x + dir.y * dir.y);
    proj.x                = FO3D_Mul32x64c(dir.x, t);
    proj.y                = FO3D_Mul32x64c(dir.y, t);
    proj.z                = 0;
    gFO3DObjMgr.lineRatio = FX_Divide(Vec_Magnitude(&proj), len);
    gFO3DObjMgr.unk_20    = 0;
    if (gFO3DObjMgr.lineRatio > FX32_ONE) {
        gFO3DObjMgr.lineRatio = FX32_ONE;
    }
    return TRUE;
}

void FO3DObjMgr_ResetObj(void) {
    if (gFO3DObjMgr.model == NULL) {
        return;
    }
    gFO3DObjMgr.model->obj->unk_18 = FX32_ONE;
    gFO3DObjMgr.model->obj->unk_1C = 0;
    FO3DObj_SetCurrent(gFO3DObjMgr.model->obj);
    FO3DObj_SetPos(0, 0, 0);
    FO3DObj_SetRotY(0);
    func_ov030_020cde20(0);
}

#define FO3D_NAME_SUFFIX(name, len, c) ((name)[(len) - 2] == 'a' && (name)[(len) - 1] == (c))

void FO3DObjMgr_SetModel(FieldResourceState* res) {
    s32 numNode;
    s32 count;
    s32 size;
    s32 len;

    if (gFO3DObjMgr.model == NULL || res == NULL) {
        return;
    }
    if (res->resMdl == NULL) {
        return;
    }
    gFO3DObjMgr.model->resMdl = res->resMdl;
    if (gFO3DObjMgr.model->resTex == NULL && res->resTex != NULL) {
        gFO3DObjMgr.model->resTex = res->resTex;
    }
    func_02028250(&gFO3DObjMgr.model->renderObj, gFO3DObjMgr.model->resMdl);
    numNode                      = FO3DObjMgr_GetNodeCount();
    count                        = numNode * gFO3DObjMgr.model->sampleCount;
    size                         = count * sizeof(Vec);
    gFO3DObjMgr.model->jntTrans  = FO3DObj_Alloc(size);
    gFO3DObjMgr.model->jntScale  = FO3DObj_Alloc(size);
    gFO3DObjMgr.model->jntRot    = FO3DObj_Alloc(count * sizeof(Mtx33));
    gFO3DObjMgr.model->nodeTrans = FO3DObj_Alloc(numNode * sizeof(Vec));
    gFO3DObjMgr.model->nodeMask  = 0;
    gFO3DObjMgr.model->name      = res->name;
    if (res->name == NULL) {
        return;
    }
    len = func_020481e8(res->name);
    if (FO3D_NAME_SUFFIX(res->name, len, '0')) {
        gFO3DObjMgr.model->blendMode = 0;
        gFO3DObjMgr.model->animating = TRUE;
        func_0202843c(&gFO3DObjMgr.model->renderObj, FO3DObjMgr_StoreNodeTrans, NULL, 6, 2);
    }
    if (FO3D_NAME_SUFFIX(res->name, len, '1')) {
        gFO3DObjMgr.model->offsetMode = 1;
    }
    if (FO3D_NAME_SUFFIX(res->name, len, '2')) {
        gFO3DObjMgr.model->offsetMode = 2;
    }
    if (FO3D_NAME_SUFFIX(res->name, len, '3')) {
        gFO3DObjMgr.model->blendMode = 2;
        gFO3DObjMgr.lineBlend        = TRUE;
        if (gFO3DObjMgr.model->sampleCount <= 9) {
            gFO3DObjMgr.lineStart = 8;
            gFO3DObjMgr.lineEnd   = 4;
        } else if (gFO3DObjMgr.model->sampleCount <= 25) {
            gFO3DObjMgr.lineStart = 10;
            gFO3DObjMgr.lineEnd   = 18;
        }
    }
    if (FO3D_NAME_SUFFIX(res->name, len, '4')) {
        gFO3DObjMgr.model->blendMode = 1;
        gFO3DObjMgr.model->animating = FALSE;
        func_0202843c(&gFO3DObjMgr.model->renderObj, FO3DObjMgr_StoreNodeTrans, NULL, 6, 2);
    }
}

void FO3DObjMgr_SetTexture(FieldResourceState* res) {
    s32 i;

    if (gFO3DObjMgr.model == NULL || res == NULL) {
        return;
    }
    if (res->resTex == NULL) {
        return;
    }
    gFO3DObjMgr.model->resTex = res->resTex;
    for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
        gFO3DObjMgr.model->anmObj[i]->resTex = gFO3DObjMgr.model->resTex;
    }
}

#define FO3D_ANM_CATEGORY(c0, c1) ((u16)((c1)[0] | ((c1)[1] << 8)))

// Nonmatching: the target reloads the category byte for every check
void FO3DObjMgr_AddAnim(FieldResourceState* res) {
    s32                 slot;
    NNSG3dResAnmCommon* anm;

    if (gFO3DObjMgr.model == NULL || res == NULL) {
        return;
    }
    if (res->resAnm == NULL || gFO3DObjMgr.model->resMdl == NULL) {
        return;
    }
    slot = gFO3DObjMgr.model->anmCount;
    if (slot >= 3) {
        return;
    }
    gFO3DObjMgr.model->anmCount     = slot + 1;
    gFO3DObjMgr.model->anmObj[slot] = func_0202bac0(FOAllocator_Get(), res->resAnm, gFO3DObjMgr.model->resMdl);
    func_020281c0(gFO3DObjMgr.model->anmObj[slot], res->resAnm, gFO3DObjMgr.model->resMdl, gFO3DObjMgr.model->resTex);
    func_020283c4(&gFO3DObjMgr.model->renderObj, gFO3DObjMgr.model->anmObj[slot]);
    gFO3DObjMgr.model->anmFrameMax[slot] = gFO3DObjMgr.model->anmObj[slot]->resAnm->numFrame << FX32_SHIFT;
    anm                                  = res->resAnm;
    if (anm->category0 == 'M' && anm->category1 == ('A' | ('M' << 8))) {
        s32 count;
        s32 size;

        gFO3DObjMgr.model->anmType[slot] = 1;
        if (gFO3DObjMgr.model->matAmbient != NULL) {
            return;
        }
        count                         = FO3DObjMgr_GetMatCount() * gFO3DObjMgr.model->sampleCount;
        size                          = count * sizeof(u16);
        gFO3DObjMgr.model->matAmbient = FO3DObj_Alloc(size);
        gFO3DObjMgr.model->matDiffuse = FO3DObj_Alloc(size);
        gFO3DObjMgr.model->matAlpha   = FO3DObj_Alloc(count);
        return;
    }
    if (anm->category0 == 'M' && anm->category1 == ('A' | ('T' << 8))) {
        s32 count;
        s32 size;

        gFO3DObjMgr.model->anmType[slot] = 2;
        if (gFO3DObjMgr.model->texScaleS != NULL) {
            return;
        }
        count                        = FO3DObjMgr_GetMatCount() * gFO3DObjMgr.model->sampleCount;
        size                         = count * sizeof(fx32);
        gFO3DObjMgr.model->texScaleS = FO3DObj_Alloc(size);
        gFO3DObjMgr.model->texScaleT = FO3DObj_Alloc(size);
        gFO3DObjMgr.model->texRotSin = FO3DObj_Alloc(count * sizeof(s16));
        gFO3DObjMgr.model->texRotCos = FO3DObj_Alloc(count * sizeof(s16));
        gFO3DObjMgr.model->texTransS = FO3DObj_Alloc(size);
        gFO3DObjMgr.model->texTransT = FO3DObj_Alloc(size);
        return;
    }
    if (anm->category0 == 'M' && anm->category1 == ('P' | ('T' << 8))) {
        gFO3DObjMgr.model->anmType[slot]         = 3;
        gFO3DObjMgr.model->anmObj[slot]->funcAnm = FO3DObjMgr_NullAnmFunc;
    }
}

u8 FO3DObjMgr_GetNodeCount(void) {
    u8 numNode = 0;

    if (gFO3DObjMgr.model != NULL && gFO3DObjMgr.model->resMdl != NULL) {
        numNode = gFO3DObjMgr.model->resMdl->numNode;
    }
    return numNode;
}

u8 FO3DObjMgr_GetMatCount(void) {
    u8 numMat = 0;

    if (gFO3DObjMgr.model != NULL && gFO3DObjMgr.model->resMdl != NULL) {
        numMat = gFO3DObjMgr.model->resMdl->numMat;
    }
    return numMat;
}

void FO3DObjMgr_UpdateAndDraw(FieldPos* pos, FieldPos* points) {
    Vec trans;
    s32 i;
    s32 done;

    if (gFO3DObjMgr.model == NULL) {
        return;
    }
    switch (gFO3DObjMgr.model->blendMode) {
        case 0:
            if (gFO3DObjMgr.model->animating) {
                for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
                    gFO3DObjMgr.model->anmObj[i]->frame += FX32_ONE;
                    if (gFO3DObjMgr.model->anmObj[i]->frame >= gFO3DObjMgr.model->anmFrameMax[i]) {
                        gFO3DObjMgr.model->anmObj[i]->frame = 0;
                    }
                }
            }
            break;
        case 1:
            if (gFO3DObjMgr.model->animating) {
                done = 0;
                for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
                    gFO3DObjMgr.model->anmObj[i]->frame += FX32_ONE;
                    if (gFO3DObjMgr.model->anmObj[i]->frame >= gFO3DObjMgr.model->anmFrameMax[i]) {
                        gFO3DObjMgr.model->anmObj[i]->frame = gFO3DObjMgr.model->anmFrameMax[i];
                        done++;
                    }
                }
                if (done >= gFO3DObjMgr.model->anmCount) {
                    gFO3DObjMgr.model->animating = FALSE;
                }
            }
            break;
        case 2:
            for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
                FO3DObjMgr_ApplyLineFrame(gFO3DObjMgr.model->anmObj[i], gFO3DObjMgr.model->anmFrameMax[i]);
            }
            break;
        case 3:
        case 4:
            FO3DObjMgr_StepSampling();
            break;
    }
    if (gFO3DObjMgr.model->animating) {
        for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
            if (gFO3DObjMgr.model->anmType[i] == 3) {
                gFO3DObjMgr.model->anmObj[i]->frame += FX32_ONE;
                if (gFO3DObjMgr.model->anmObj[i]->frame > gFO3DObjMgr.model->anmFrameMax[i]) {
                    gFO3DObjMgr.model->anmObj[i]->frame = 0;
                }
            }
        }
    }
    if (gFO3DObjMgr.model->offsetMode == 1) {
        trans.x = pos->x - (points[0].x << FX32_SHIFT);
        trans.y = pos->y - (points[0].y << FX32_SHIFT);
        trans.z = 0;
        func_02028a80(&trans);
    } else if (gFO3DObjMgr.model->offsetMode == 2) {
        trans.x = gFO3DObjMgr.model->offset.x + (pos->x - (points[0].x << FX32_SHIFT));
        trans.y = (pos->y - (points[0].y << FX32_SHIFT)) - gFO3DObjMgr.model->offset.y;
        trans.z = 0;
        func_02028a80(&trans);
    }
    func_02028a44();
    func_0202b5d4();
    if (gFO3DObjMgr.model->visible) {
        func_02029270(&gFO3DObjMgr.model->renderObj);
    }
    {
        Vec zero = {0, 0, 0};

        func_02028a80(&zero);
    }
}

void FO3DObjMgr_SetOffset(Vec* offset) {
    if (gFO3DObjMgr.model != NULL) {
        gFO3DObjMgr.model->offset.x = offset->x;
        gFO3DObjMgr.model->offset.y = offset->y;
        gFO3DObjMgr.model->offset.z = 0;
    }
}

void FO3DObjMgr_SetupSampleCallbacks(NNSG3dRS* rs) {
    s32 i;

    rs->cbVecFunc[6]   = FO3DObjMgr_CaptureJointSample;
    rs->cbVecTiming[6] = 2;
    for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
        if (gFO3DObjMgr.model->anmType[i] == 1 || gFO3DObjMgr.model->anmType[i] == 2) {
            rs->cbVecFunc[4]   = FO3DObjMgr_CaptureMaterialSample;
            rs->cbVecTiming[4] = 2;
        }
    }
}

void FO3DObjMgr_StepSampling(void) {
    s32  sample = gFO3DObjMgr.model->sample;
    fx32 frame;
    s32  i;

    if (sample == -1) {
        return;
    }
    if (sample < gFO3DObjMgr.model->sampleCount) {
        frame = gFO3DObjMgr.model->sampleFrames[sample] << FX32_SHIFT;
        for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
            gFO3DObjMgr.model->anmObj[i]->frame = frame;
        }
        func_02028450(&gFO3DObjMgr.model->renderObj, FO3DObjMgr_SetupSampleCallbacks);
        gFO3DObjMgr.model->sample++;
        return;
    }
    for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
        gFO3DObjMgr.model->anmObj[i]->frame = 0;
    }
    gFO3DObjMgr.model->sample = -1;
    func_02028450(&gFO3DObjMgr.model->renderObj, NULL);
    for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
        if (gFO3DObjMgr.model->anmType[i] == 1) {
            gFO3DObjMgr.model->anmObj[i]->funcAnm = FO3DObjMgr_BlendMatColorAnm;
        } else if (gFO3DObjMgr.model->anmType[i] == 2) {
            gFO3DObjMgr.model->anmObj[i]->funcAnm = FO3DObjMgr_BlendTexSrtAnm;
        } else if (gFO3DObjMgr.model->anmType[i] == 3) {
            gFO3DObjMgr.model->anmObj[i]->funcAnm = FO3DObjMgr_TexPatAnmFunc;
        } else {
            gFO3DObjMgr.model->anmObj[i]->funcAnm = FO3DObjMgr_BlendJointAnm;
        }
    }
}

void FO3DObjMgr_ApplyLineFrame(NNSG3dAnmObj* anmObj, fx32 frameMax) {
    gFO3DObjMgr.model->sample = -1;
    anmObj->frame             = gFO3DObjMgr.lineRatio * ((frameMax >> FX32_SHIFT) - 1);
}

static inline const NNSG3dResName* FO3D_GetResNameByIdx(const NNSG3dResDict* dict, u32 idx) {
    const NNSG3dResDictEntryHeader* entry = (const NNSG3dResDictEntryHeader*)((const u8*)dict + dict->ofsEntry);
    return (const NNSG3dResName*)((const u8*)entry + entry->ofsName) + idx;
}

// Nonmatching: the gFO3DObjMgr address is hoisted out of the loop here but reloaded in the target
BOOL FO3DObjMgr_GetNodePos(const char* name, Vec* pos) {
    s32                  i;
    s32                  numNode;
    FO3DModel*           model;
    const NNSG3dResDict* dict;

    if (gFO3DObjMgr.model == NULL) {
        return FALSE;
    }
    numNode = FO3DObjMgr_GetNodeCount();
    for (i = 0; i < numNode; i++) {
        model = gFO3DObjMgr.model;
        dict  = &model->resMdl->nodeDict;
        if (strcmp(name, FO3D_GetResNameByIdx(dict, i)->name) == 0) {
            *pos = model->nodeTrans[i];
            return TRUE;
        }
    }
    return FALSE;
}

fx32 FO3DObjMgr_GetFrame(void) {
    if (gFO3DObjMgr.model != NULL && gFO3DObjMgr.model->anmCount > 0) {
        return gFO3DObjMgr.model->anmObj[0]->frame;
    }
    return 0;
}

BOOL FO3DObjMgr_HasTexPatAnm(void) {
    s32 i;

    for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
        if (gFO3DObjMgr.model->anmType[i] == 3) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL FO3DObjMgr_IsNamed(const char* name) {
    if (gFO3DObjMgr.model->name != NULL && strcmp((char*)gFO3DObjMgr.model->name, (char*)name) == 0) {
        return TRUE;
    }
    return FALSE;
}

void FO3DObjMgr_RestartAnim(void) {
    s32 i;

    if (gFO3DObjMgr.model == NULL) {
        return;
    }
    if ((u32)gFO3DObjMgr.model->blendMode > 1) {
        return;
    }
    gFO3DObjMgr.model->animating = TRUE;
    for (i = 0; i < gFO3DObjMgr.model->anmCount; i++) {
        gFO3DObjMgr.model->anmObj[i]->frame = 0;
    }
}

void FO3DObjMgr_Show(void) {
    if (gFO3DObjMgr.model != NULL) {
        gFO3DObjMgr.model->visible = TRUE;
    }
}

void FO3DObjMgr_Hide(void) {
    if (gFO3DObjMgr.model != NULL) {
        gFO3DObjMgr.model->visible = FALSE;
    }
}

// Nonmatching: load scheduling of the texture offset
void FO3DObjMgr_LoadPatternTex(NNSG3dResTex* tex, const void* name, NNSG3dMatAnmResult* result) {
    const NNSG3dResDictTexData* texData = func_0202baf0(&tex->texDict, name);
    u32                         param   = texData->texImageParam;
    const u8*                   src     = (const u8*)tex + tex->texOfsData + ((param & 0xFFFF) << 3);
    u32                         dst     = (result->prmTexImage & 0xFFFF) << 3;
    u32                         width;
    u32                         height;
    u32                         size;

    result->prmTexImage &= 0xC00FFFFF;
    result->prmTexImage |= texData->texImageParam & 0x3FF00000;
    result->origWidth  = texData->extraParam & 0x7FF;
    result->origHeight = (texData->extraParam & (0x7FF << 11)) >> 11;
    width              = texData->extraParam & 0x7FF;
    height             = (texData->extraParam >> 11) & 0x7FF;
    if (width == result->origWidth) {
        result->magW = FX32_ONE;
    } else {
        result->magW = FX_Divide(width << FX32_SHIFT, result->origWidth << FX32_SHIFT);
    }
    if (height == result->origHeight) {
        result->magH = FX32_ONE;
    } else {
        result->magH = FX_Divide(height << FX32_SHIFT, result->origHeight << FX32_SHIFT);
    }
    size = result->origWidth * result->origHeight;
    switch ((texData->texImageParam >> 26) & 7) {
        case 0:
        case 1:
        case 4:
        case 5:
        case 6:
            break;
        case 2:
            size >>= 2;
            break;
        case 3:
            size >>= 1;
            break;
        case 7:
            size <<= 1;
            break;
    }
    func_02001b44(1, dst, src, size);
}

// Nonmatching: load scheduling of plttSize
void FO3DObjMgr_LoadPatternPltt(NNSG3dResTex* tex, const void* name, NNSG3dMatAnmResult* result) {
    const NNSG3dResDictPlttData* plttData = func_0202baf0((const NNSG3dResDict*)((const u8*)tex + tex->plttOfsDict), name);
    const u8*                    src      = (const u8*)tex + tex->plttOfsData + plttData->offset * 8;
    u32                          dst;

    if (plttData->flag != 0) {
        dst = (result->prmTexPltt & 0x1FFF) << 3;
    } else {
        dst = (result->prmTexPltt & 0x1FFF) << 4;
    }
    func_02001b44(2, dst, src, tex->plttSize << 3);
}

void FO3DObjMgr_TexPatAnmFunc(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat) {
    const void*              resAnm = anmObj->resAnm;
    const NNSG3dTexPatAnmFV* fv     = func_0202bdf4(resAnm, mat, anmObj->frame >> FX32_SHIFT);

    FO3DObjMgr_LoadPatternTex((NNSG3dResTex*)anmObj->resTex, func_0202bdd4(resAnm, fv->idTex), result);
    if (fv->idPltt == 0xFF) {
        return;
    }
    FO3DObjMgr_LoadPatternPltt((NNSG3dResTex*)anmObj->resTex, func_0202bde4(resAnm, fv->idPltt), result);
}

void FO3DObjMgr_NullAnmFunc(void) {}
