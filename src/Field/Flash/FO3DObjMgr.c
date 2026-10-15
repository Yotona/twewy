#include "Field/FO3D.h"
#include "Field/FO3DRegion.h"
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

FO3DObjMgrState data_ov030_02109da0;

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

static const s32 data_ov030_020ed1c4[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

static const s32 data_ov030_020ed1e8[9][9] = {
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

void func_ov030_020ce164(NNSG3dRS* rs);
void func_ov030_020ce1a8(NNSG3dRS* rs);
void func_ov030_020ce234(NNSG3dRS* rs);
void func_ov030_020ce330(NNSG3dRS* rs);
void func_ov030_020ce430(NNSG3dJntAnmResult* result, u32 node, s32 base);
void func_ov030_020ce57c(NNSG3dJntAnmResult* result, s32 base);
void func_ov030_020ce6b0(NNSG3dJntAnmResult* result, s32 base);
void func_ov030_020ce8f0(NNSG3dJntAnmResult* result, u32 node, s32 base);
void func_ov030_020cead0(NNSG3dJntAnmResult* result, s32 base);
void func_ov030_020cec90(NNSG3dJntAnmResult* result, s32 base);
u16  func_ov030_020cf0d8(u16 c0, u16 c1, u16 c2);
BOOL func_ov030_020cf984(FieldPos* pos, FieldPos* points);
BOOL func_ov030_020cfd9c(s32* outA, s32* outB, Vec* rel, FieldPos* points, const s32* ring, s32 count);
s32  func_ov030_020cfe7c(FieldPos* pos, FieldPos* points, const s32* list, s32 count);
BOOL func_ov030_020cff00(FieldPos* pos, FieldPos* points);
u8   func_ov030_020d05e8(void);
u8   func_ov030_020d060c(void);
void func_ov030_020d0920(NNSG3dRS* rs);
void func_ov030_020d0990(void);
void func_ov030_020d0af8(NNSG3dAnmObj* anmObj, fx32 frameMax);
BOOL func_ov030_020cef78(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat);
BOOL func_ov030_020cf214(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat);
BOOL func_ov030_020ce38c(NNSG3dJntAnmResult* result, NNSG3dAnmObj* anmObj, u32 node);
void func_ov030_020d0ea0(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat);
void func_ov030_020d0f14(void);

static inline fx32 FO3D_Mul32x64c(fx32 v32, s64 v64c) {
    return (fx32)((v64c * v32 + 0x80000000LL) >> 32);
}

void func_ov030_020cdea0(NNSG3dRS* rs) {
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
    base   = data_ov030_02109da0.model->sampleCount;
    base   = base * node;
    sample = data_ov030_02109da0.model->sample - 1;
    idx    = base + sample;
    trans  = &data_ov030_02109da0.model->jntTrans[idx];
    if (result->flag & 4) {
        trans->x = 0;
        trans->y = 0;
        trans->z = 0;
    } else {
        *trans = result->trans;
    }
    if (sample == 0 && (data_ov030_02109da0.model->nodeMask & (1 << node)) &&
        (data_ov030_02109da0.model->anmObj[0]->mapData[node] & 0x300) == 0x100)
    {
        func_ov030_020d25cc(trans);
        func_ov030_020d25cc(&data_ov030_02109da0.model->nodeTrans[node]);
    }
    if (sample > 0 && data_ov030_02109da0.model->blendMode == 3) {
        Vec* first = &data_ov030_02109da0.model->jntTrans[base];

        trans->x -= first->x;
        trans->y -= first->y;
        trans->z -= first->z;
    }
    scale = &data_ov030_02109da0.model->jntScale[idx];
    if (result->flag & 1) {
        scale->x = FX32_ONE;
        scale->y = FX32_ONE;
        scale->z = FX32_ONE;
    } else {
        *scale = result->scale;
    }
    if (sample > 0 && data_ov030_02109da0.model->blendMode == 3) {
        Vec* first = &data_ov030_02109da0.model->jntScale[base];

        scale->x -= first->x;
        scale->y -= first->y;
        scale->z -= first->z;
    }
    rot = &data_ov030_02109da0.model->jntRot[idx];
    if (result->flag & 2) {
        Mtx_Identity33(rot);
    } else {
        *rot = result->rot;
    }
    if (sample > 0 && data_ov030_02109da0.model->blendMode == 3) {
        Mtx33* first = &data_ov030_02109da0.model->jntRot[base];

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

void func_ov030_020ce164(NNSG3dRS* rs) {
    if (data_ov030_02109da0.model->matAmbient != NULL) {
        func_ov030_020ce1a8(rs);
    }
    if (data_ov030_02109da0.model->texScaleS != NULL) {
        func_ov030_020ce234(rs);
    }
}

void func_ov030_020ce1a8(NNSG3dRS* rs) {
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
    base   = data_ov030_02109da0.model->sampleCount * mat;
    sample = data_ov030_02109da0.model->sample - 1;
    if (result == NULL) {
        return;
    }
    idx                                        = base + sample;
    data_ov030_02109da0.model->matAmbient[idx] = (result->prmMatColor0 >> 16) & 0x7FFF;
    data_ov030_02109da0.model->matDiffuse[idx] = result->prmMatColor0 & 0x7FFF;
    data_ov030_02109da0.model->matAlpha[idx]   = (result->prmPolygonAttr >> 16) & 0x1F;
}

void func_ov030_020ce234(NNSG3dRS* rs) {
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
    base   = data_ov030_02109da0.model->sampleCount * mat;
    sample = data_ov030_02109da0.model->sample - 1;
    if (result == NULL) {
        return;
    }
    idx    = base + sample;
    scaleS = data_ov030_02109da0.model->texScaleS;
    scaleT = data_ov030_02109da0.model->texScaleT;
    if (result->flag & 1) {
        scaleS[idx] = FX32_ONE;
        scaleT[idx] = FX32_ONE;
    } else {
        scaleS[idx] = result->scaleS;
        scaleT[idx] = result->scaleT;
    }
    rotSin = &data_ov030_02109da0.model->texRotSin[idx];
    rotCos = &data_ov030_02109da0.model->texRotCos[idx];
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
    transS = data_ov030_02109da0.model->texTransS;
    transT = data_ov030_02109da0.model->texTransT;
    if (result->flag & 4) {
        transS[idx] = 0;
        transT[idx] = 0;
        return;
    }
    transS[idx] = result->transS;
    transT[idx] = result->transT;
}

void func_ov030_020ce330(NNSG3dRS* rs) {
    NNSG3dJntAnmResult* result;
    u32                 node;
    Vec*                trans;

    node   = (rs->flag & 0x10) ? rs->currentNodeDesc : -1;
    result = rs->jntAnmResult;
    trans  = &data_ov030_02109da0.model->nodeTrans[node];
    if (result->flag & 4) {
        trans->x = 0;
        trans->y = 0;
        trans->z = 0;
    } else {
        *trans = result->trans;
    }
}

BOOL func_ov030_020ce38c(NNSG3dJntAnmResult* result, NNSG3dAnmObj* anmObj, u32 node) {
    s32 base;

    if (anmObj->next != NULL) {
        return FALSE;
    }
    if ((anmObj->mapData[node] & 0x300) != 0x100) {
        return FALSE;
    }
    base = data_ov030_02109da0.model->sampleCount * node;
    if (data_ov030_02109da0.model->blendMode == 3) {
        func_ov030_020ce430(result, node, base);
        func_ov030_020ce57c(result, base);
        func_ov030_020ce6b0(result, base);
    } else {
        func_ov030_020ce8f0(result, node, base);
        func_ov030_020cead0(result, base);
        func_ov030_020cec90(result, base);
    }
    result->flag = 0;
    return TRUE;
}

void func_ov030_020ce430(NNSG3dJntAnmResult* result, u32 node, s32 base) {
    Vec* trans = data_ov030_02109da0.model->jntTrans;
    Vec* p1    = &trans[base + data_ov030_02109da0.sampleIdx[1]];
    Vec* p2    = &trans[base + data_ov030_02109da0.sampleIdx[2]];

    result->trans = trans[base];
    result->trans.x += FX_Mul(p1->x, data_ov030_02109da0.sampleWeight[1]);
    result->trans.x += FX_Mul(p2->x, data_ov030_02109da0.sampleWeight[2]);
    result->trans.y += FX_Mul(p1->y, data_ov030_02109da0.sampleWeight[1]);
    result->trans.y += FX_Mul(p2->y, data_ov030_02109da0.sampleWeight[2]);
    result->trans.z += FX_Mul(p1->z, data_ov030_02109da0.sampleWeight[1]);
    result->trans.z += FX_Mul(p2->z, data_ov030_02109da0.sampleWeight[2]);
    data_ov030_02109da0.model->nodeTrans[node] = result->trans;
}

void func_ov030_020ce57c(NNSG3dJntAnmResult* result, s32 base) {
    Vec* scale = data_ov030_02109da0.model->jntScale;
    Vec* p1    = &scale[base + data_ov030_02109da0.sampleIdx[1]];
    Vec* p2    = &scale[base + data_ov030_02109da0.sampleIdx[2]];

    result->scale = scale[base];
    result->scale.x += FX_Mul(p1->x, data_ov030_02109da0.sampleWeight[1]);
    result->scale.x += FX_Mul(p2->x, data_ov030_02109da0.sampleWeight[2]);
    result->scale.y += FX_Mul(p1->y, data_ov030_02109da0.sampleWeight[1]);
    result->scale.y += FX_Mul(p2->y, data_ov030_02109da0.sampleWeight[2]);
    result->scale.z += FX_Mul(p1->z, data_ov030_02109da0.sampleWeight[1]);
    result->scale.z += FX_Mul(p2->z, data_ov030_02109da0.sampleWeight[2]);
}

void func_ov030_020ce6b0(NNSG3dJntAnmResult* result, s32 base) {
    Mtx33* rot = data_ov030_02109da0.model->jntRot;
    Mtx33* p1  = &rot[base + data_ov030_02109da0.sampleIdx[1]];
    Mtx33* p2  = &rot[base + data_ov030_02109da0.sampleIdx[2]];

    result->rot = rot[base];
    result->rot._00 += FX_Mul(p1->_00, data_ov030_02109da0.sampleWeight[1]);
    result->rot._00 += FX_Mul(p2->_00, data_ov030_02109da0.sampleWeight[2]);
    result->rot._01 += FX_Mul(p1->_01, data_ov030_02109da0.sampleWeight[1]);
    result->rot._01 += FX_Mul(p2->_01, data_ov030_02109da0.sampleWeight[2]);
    result->rot._02 += FX_Mul(p1->_02, data_ov030_02109da0.sampleWeight[1]);
    result->rot._02 += FX_Mul(p2->_02, data_ov030_02109da0.sampleWeight[2]);
    result->rot._10 += FX_Mul(p1->_10, data_ov030_02109da0.sampleWeight[1]);
    result->rot._10 += FX_Mul(p2->_10, data_ov030_02109da0.sampleWeight[2]);
    result->rot._11 += FX_Mul(p1->_11, data_ov030_02109da0.sampleWeight[1]);
    result->rot._11 += FX_Mul(p2->_11, data_ov030_02109da0.sampleWeight[2]);
    result->rot._12 += FX_Mul(p1->_12, data_ov030_02109da0.sampleWeight[1]);
    result->rot._12 += FX_Mul(p2->_12, data_ov030_02109da0.sampleWeight[2]);
    Vec_CrossProduct((Vec*)&result->rot._00, (Vec*)&result->rot._10, (Vec*)&result->rot._20);
}

void func_ov030_020ce8f0(NNSG3dJntAnmResult* result, u32 node, s32 base) {
    Vec p0;
    Vec p1;
    Vec p2;

    p0 = data_ov030_02109da0.model->jntTrans[base + data_ov030_02109da0.sampleIdx[0]];
    p1 = data_ov030_02109da0.model->jntTrans[base + data_ov030_02109da0.sampleIdx[1]];
    p2 = data_ov030_02109da0.model->jntTrans[base + data_ov030_02109da0.sampleIdx[2]];
    p1.x -= p0.x;
    p1.y -= p0.y;
    p1.z -= p0.z;
    p2.x -= p0.x;
    p2.y -= p0.y;
    p2.z -= p0.z;
    result->trans = p0;
    result->trans.x += FX_Mul(p1.x, data_ov030_02109da0.sampleWeight[1]);
    result->trans.x += FX_Mul(p2.x, data_ov030_02109da0.sampleWeight[2]);
    result->trans.y += FX_Mul(p1.y, data_ov030_02109da0.sampleWeight[1]);
    result->trans.y += FX_Mul(p2.y, data_ov030_02109da0.sampleWeight[2]);
    result->trans.z += FX_Mul(p1.z, data_ov030_02109da0.sampleWeight[1]);
    result->trans.z += FX_Mul(p2.z, data_ov030_02109da0.sampleWeight[2]);
    data_ov030_02109da0.model->nodeTrans[node] = result->trans;
}

void func_ov030_020cead0(NNSG3dJntAnmResult* result, s32 base) {
    Vec p0;
    Vec p1;
    Vec p2;

    p0 = data_ov030_02109da0.model->jntScale[base + data_ov030_02109da0.sampleIdx[0]];
    p1 = data_ov030_02109da0.model->jntScale[base + data_ov030_02109da0.sampleIdx[1]];
    p2 = data_ov030_02109da0.model->jntScale[base + data_ov030_02109da0.sampleIdx[2]];
    p1.x -= p0.x;
    p1.y -= p0.y;
    p1.z -= p0.z;
    p2.x -= p0.x;
    p2.y -= p0.y;
    p2.z -= p0.z;
    result->scale = p0;
    result->scale.x += FX_Mul(p1.x, data_ov030_02109da0.sampleWeight[1]);
    result->scale.x += FX_Mul(p2.x, data_ov030_02109da0.sampleWeight[2]);
    result->scale.y += FX_Mul(p1.y, data_ov030_02109da0.sampleWeight[1]);
    result->scale.y += FX_Mul(p2.y, data_ov030_02109da0.sampleWeight[2]);
    result->scale.z += FX_Mul(p1.z, data_ov030_02109da0.sampleWeight[1]);
    result->scale.z += FX_Mul(p2.z, data_ov030_02109da0.sampleWeight[2]);
}

void func_ov030_020cec90(NNSG3dJntAnmResult* result, s32 base) {
    Mtx33* rot = data_ov030_02109da0.model->jntRot;
    Mtx33* r0  = &rot[base + data_ov030_02109da0.sampleIdx[0]];
    Mtx33* r1  = &rot[base + data_ov030_02109da0.sampleIdx[1]];
    Mtx33* r2  = &rot[base + data_ov030_02109da0.sampleIdx[2]];

    result->rot = *r0;
    result->rot._00 += FX_Mul(r1->_00 - r0->_00, data_ov030_02109da0.sampleWeight[1]);
    result->rot._00 += FX_Mul(r2->_00 - r0->_00, data_ov030_02109da0.sampleWeight[2]);
    result->rot._01 += FX_Mul(r1->_01 - r0->_01, data_ov030_02109da0.sampleWeight[1]);
    result->rot._01 += FX_Mul(r2->_01 - r0->_01, data_ov030_02109da0.sampleWeight[2]);
    result->rot._02 += FX_Mul(r1->_02 - r0->_02, data_ov030_02109da0.sampleWeight[1]);
    result->rot._02 += FX_Mul(r2->_02 - r0->_02, data_ov030_02109da0.sampleWeight[2]);
    result->rot._10 += FX_Mul(r1->_10 - r0->_10, data_ov030_02109da0.sampleWeight[1]);
    result->rot._10 += FX_Mul(r2->_10 - r0->_10, data_ov030_02109da0.sampleWeight[2]);
    result->rot._11 += FX_Mul(r1->_11 - r0->_11, data_ov030_02109da0.sampleWeight[1]);
    result->rot._11 += FX_Mul(r2->_11 - r0->_11, data_ov030_02109da0.sampleWeight[2]);
    result->rot._12 += FX_Mul(r1->_12 - r0->_12, data_ov030_02109da0.sampleWeight[1]);
    result->rot._12 += FX_Mul(r2->_12 - r0->_12, data_ov030_02109da0.sampleWeight[2]);
    Vec_CrossProduct((Vec*)&result->rot._00, (Vec*)&result->rot._10, (Vec*)&result->rot._20);
}

// Nonmatching: register allocation; alpha field insert uses and+lsl in the target
BOOL func_ov030_020cef78(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat) {
    u32 color;
    s32 base;
    Vec a;

    base  = data_ov030_02109da0.model->sampleCount * mat;
    color = result->prmMatColor0 & 0x80008000;
    color |= (func_ov030_020cf0d8(data_ov030_02109da0.model->matAmbient[base + data_ov030_02109da0.sampleIdx[0]],
                                  data_ov030_02109da0.model->matAmbient[base + data_ov030_02109da0.sampleIdx[1]],
                                  data_ov030_02109da0.model->matAmbient[base + data_ov030_02109da0.sampleIdx[2]])
              << 16) &
             0x7FFF0000;
    result->prmMatColor0 =
        color | (func_ov030_020cf0d8(data_ov030_02109da0.model->matDiffuse[base + data_ov030_02109da0.sampleIdx[0]],
                                     data_ov030_02109da0.model->matDiffuse[base + data_ov030_02109da0.sampleIdx[1]],
                                     data_ov030_02109da0.model->matDiffuse[base + data_ov030_02109da0.sampleIdx[2]]) &
                 0x7FFF);
    a.x = data_ov030_02109da0.model->matAlpha[base + data_ov030_02109da0.sampleIdx[0]] << FX32_SHIFT;
    a.y = data_ov030_02109da0.model->matAlpha[base + data_ov030_02109da0.sampleIdx[1]] << FX32_SHIFT;
    a.z = data_ov030_02109da0.model->matAlpha[base + data_ov030_02109da0.sampleIdx[2]] << FX32_SHIFT;
    result->prmPolygonAttr =
        (result->prmPolygonAttr & ~0x1F0000) | ((((a.x + FX_Mul(a.y - a.x, data_ov030_02109da0.sampleWeight[1]) +
                                                   FX_Mul(a.z - a.x, data_ov030_02109da0.sampleWeight[2])) >>
                                                  FX32_SHIFT) &
                                                 0x1F)
                                                << 16);
    return TRUE;
}

static inline fx32 FO3D_Lerp3(fx32 v0, fx32 v1, fx32 v2) {
    return v0 + FX_Mul(v1 - v0, data_ov030_02109da0.sampleWeight[1]) + FX_Mul(v2 - v0, data_ov030_02109da0.sampleWeight[2]);
}

// Nonmatching: channel scheduling and register allocation
u16 func_ov030_020cf0d8(u16 c0, u16 c1, u16 c2) {
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
    r    = (v0.x + FX_Mul(v1.x - v0.x, data_ov030_02109da0.sampleWeight[1]) +
         FX_Mul(v2.x - v0.x, data_ov030_02109da0.sampleWeight[2])) >>
        FX32_SHIFT;
    g = (v0.y + FX_Mul(v1.y - v0.y, data_ov030_02109da0.sampleWeight[1]) +
         FX_Mul(v2.y - v0.y, data_ov030_02109da0.sampleWeight[2])) >>
        FX32_SHIFT;
    b = (v0.z + FX_Mul(v1.z - v0.z, data_ov030_02109da0.sampleWeight[1]) +
         FX_Mul(v2.z - v0.z, data_ov030_02109da0.sampleWeight[2])) >>
        FX32_SHIFT;
    return (r & 0x1F) | ((g & 0x1F) << 5) | ((b & 0x1F) << 10);
}

// Nonmatching: register allocation
BOOL func_ov030_020cf214(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat) {
    s32 base = data_ov030_02109da0.model->sampleCount * mat;
    s32 v0;
    s32 v1;
    s32 v2;

    v0             = data_ov030_02109da0.model->texScaleS[base + data_ov030_02109da0.sampleIdx[0]];
    v1             = data_ov030_02109da0.model->texScaleS[base + data_ov030_02109da0.sampleIdx[1]];
    v2             = data_ov030_02109da0.model->texScaleS[base + data_ov030_02109da0.sampleIdx[2]];
    result->scaleS = v0;
    result->scaleS += FX_Mul(v1 - v0, data_ov030_02109da0.sampleWeight[1]);
    result->scaleS += FX_Mul(v2 - v0, data_ov030_02109da0.sampleWeight[2]);
    v0             = data_ov030_02109da0.model->texScaleT[base + data_ov030_02109da0.sampleIdx[0]];
    v1             = data_ov030_02109da0.model->texScaleT[base + data_ov030_02109da0.sampleIdx[1]];
    v2             = data_ov030_02109da0.model->texScaleT[base + data_ov030_02109da0.sampleIdx[2]];
    result->scaleT = v0;
    result->scaleT += FX_Mul(v1 - v0, data_ov030_02109da0.sampleWeight[1]);
    result->scaleT += FX_Mul(v2 - v0, data_ov030_02109da0.sampleWeight[2]);
    result->flag &= ~1;
    v0 = data_ov030_02109da0.model->texRotSin[base + data_ov030_02109da0.sampleIdx[0]];
    v1 = data_ov030_02109da0.model->texRotSin[base + data_ov030_02109da0.sampleIdx[1]];
    v2 = data_ov030_02109da0.model->texRotSin[base + data_ov030_02109da0.sampleIdx[2]];
    result->rotSin =
        v0 + FX_Mul(v1 - v0, data_ov030_02109da0.sampleWeight[1]) + FX_Mul(v2 - v0, data_ov030_02109da0.sampleWeight[2]);
    v0 = data_ov030_02109da0.model->texRotCos[base + data_ov030_02109da0.sampleIdx[0]];
    v1 = data_ov030_02109da0.model->texRotCos[base + data_ov030_02109da0.sampleIdx[1]];
    v2 = data_ov030_02109da0.model->texRotCos[base + data_ov030_02109da0.sampleIdx[2]];
    result->rotCos =
        v0 + FX_Mul(v1 - v0, data_ov030_02109da0.sampleWeight[1]) + FX_Mul(v2 - v0, data_ov030_02109da0.sampleWeight[2]);
    result->flag &= ~2;
    v0             = data_ov030_02109da0.model->texTransS[base + data_ov030_02109da0.sampleIdx[0]];
    v1             = data_ov030_02109da0.model->texTransS[base + data_ov030_02109da0.sampleIdx[1]];
    v2             = data_ov030_02109da0.model->texTransS[base + data_ov030_02109da0.sampleIdx[2]];
    result->transS = v0;
    result->transS += FX_Mul(v1 - v0, data_ov030_02109da0.sampleWeight[1]);
    result->transS += FX_Mul(v2 - v0, data_ov030_02109da0.sampleWeight[2]);
    v0             = data_ov030_02109da0.model->texTransT[base + data_ov030_02109da0.sampleIdx[0]];
    v1             = data_ov030_02109da0.model->texTransT[base + data_ov030_02109da0.sampleIdx[1]];
    v2             = data_ov030_02109da0.model->texTransT[base + data_ov030_02109da0.sampleIdx[2]];
    result->transT = v0;
    result->transT += FX_Mul(v1 - v0, data_ov030_02109da0.sampleWeight[1]);
    result->transT += FX_Mul(v2 - v0, data_ov030_02109da0.sampleWeight[2]);
    result->flag &= ~4;
    result->prmTexImage = (result->prmTexImage & ~0xC0000000) | 0x40000000;
    result->flag |= 8;
    return TRUE;
}

FO3DModel* func_ov030_020cf554(void) {
    s32 i;

    data_ov030_02109da0.model             = func_ov030_020cde54(sizeof(FO3DModel));
    data_ov030_02109da0.model->obj        = func_ov030_020cdcfc();
    data_ov030_02109da0.model->resMdl     = NULL;
    data_ov030_02109da0.model->resTex     = NULL;
    data_ov030_02109da0.model->jntTrans   = NULL;
    data_ov030_02109da0.model->jntScale   = NULL;
    data_ov030_02109da0.model->jntRot     = NULL;
    data_ov030_02109da0.model->matAmbient = NULL;
    data_ov030_02109da0.model->matDiffuse = NULL;
    data_ov030_02109da0.model->matAlpha   = NULL;
    data_ov030_02109da0.model->texScaleS  = NULL;
    data_ov030_02109da0.model->texScaleT  = NULL;
    data_ov030_02109da0.model->texRotSin  = NULL;
    data_ov030_02109da0.model->texRotCos  = NULL;
    data_ov030_02109da0.model->texTransS  = NULL;
    data_ov030_02109da0.model->texTransT  = NULL;
    for (i = 0; i < 3; i++) {
        data_ov030_02109da0.model->anmObj[i]  = NULL;
        data_ov030_02109da0.model->anmType[i] = 0;
    }
    data_ov030_02109da0.model->nodeTrans  = NULL;
    data_ov030_02109da0.model->offset.z   = 0;
    data_ov030_02109da0.model->offset.y   = data_ov030_02109da0.model->offset.z;
    data_ov030_02109da0.model->offset.x   = data_ov030_02109da0.model->offset.y;
    data_ov030_02109da0.model->nodeMask   = 0;
    data_ov030_02109da0.model->blendMode  = 0;
    data_ov030_02109da0.model->sample     = 0;
    data_ov030_02109da0.model->anmCount   = 0;
    data_ov030_02109da0.model->animating  = TRUE;
    data_ov030_02109da0.model->visible    = TRUE;
    data_ov030_02109da0.model->offsetMode = 0;
    data_ov030_02109da0.sampleIdx[0]      = 0;
    data_ov030_02109da0.sampleIdx[1]      = 0;
    data_ov030_02109da0.sampleIdx[2]      = 0;
    data_ov030_02109da0.sampleWeight[0]   = 0;
    data_ov030_02109da0.sampleWeight[1]   = 0;
    data_ov030_02109da0.sampleWeight[2]   = 0;
    return data_ov030_02109da0.model;
}

void func_ov030_020cf6a0(void) {
    s32 i;

    func_ov030_020cdcec(data_ov030_02109da0.model->obj);
    func_ov030_020cdd74();
    for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
        NNSG3dAnmObj* anmObj = data_ov030_02109da0.model->anmObj[i];

        if (anmObj != NULL) {
            func_0202bae4(func_ov030_020cd7fc(), anmObj);
        }
    }
    func_ov030_020cde88(data_ov030_02109da0.model->nodeTrans);
    func_ov030_020cde88(data_ov030_02109da0.model->jntTrans);
    func_ov030_020cde88(data_ov030_02109da0.model->jntScale);
    func_ov030_020cde88(data_ov030_02109da0.model->jntRot);
    if (data_ov030_02109da0.model->matAmbient != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->matAmbient);
    }
    if (data_ov030_02109da0.model->matDiffuse != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->matDiffuse);
    }
    if (data_ov030_02109da0.model->matAlpha != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->matAlpha);
    }
    if (data_ov030_02109da0.model->texScaleS != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->texScaleS);
    }
    if (data_ov030_02109da0.model->texScaleT != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->texScaleT);
    }
    if (data_ov030_02109da0.model->texRotSin != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->texRotSin);
    }
    if (data_ov030_02109da0.model->texRotCos != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->texRotCos);
    }
    if (data_ov030_02109da0.model->texTransS != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->texTransS);
    }
    if (data_ov030_02109da0.model->texTransT != NULL) {
        func_ov030_020cde88(data_ov030_02109da0.model->texTransT);
    }
    data_ov030_02109da0.lineBlend = FALSE;
    func_ov030_020cde88(data_ov030_02109da0.model);
    data_ov030_02109da0.model = NULL;
}

void func_ov030_020cf838(FO3DModel* model) {
    data_ov030_02109da0.model = model;
}

void func_ov030_020cf848(const s32* frames, s32 count) {
    if (data_ov030_02109da0.model == NULL) {
        return;
    }
    data_ov030_02109da0.model->sampleCount  = count;
    data_ov030_02109da0.model->sampleFrames = frames;
    if (data_ov030_02109da0.model->sampleCount == 1) {
        data_ov030_02109da0.model->blendMode = 0;
        func_0202843c(&data_ov030_02109da0.model->renderObj, func_ov030_020ce330, NULL, 6, 2);
    } else if (data_ov030_02109da0.model->sampleCount >= 25) {
        data_ov030_02109da0.model->blendMode   = 4;
        data_ov030_02109da0.model->sampleCount = 25;
    } else if (data_ov030_02109da0.model->sampleCount >= 9) {
        data_ov030_02109da0.model->blendMode   = 3;
        data_ov030_02109da0.model->sampleCount = 9;
    } else {
        data_ov030_02109da0.model->blendMode = 2;
        data_ov030_02109da0.lineStart        = 1;
        data_ov030_02109da0.lineEnd          = 2;
    }
    data_ov030_02109da0.blendMode = data_ov030_02109da0.model->blendMode;
}

BOOL func_ov030_020cf910(FieldPos* pos, FieldPos* points) {
    BOOL result = TRUE;

    switch (data_ov030_02109da0.blendMode) {
        case 0:
        case 1:
            break;
        case 2:
            result = func_ov030_020cff00(pos, points);
            break;
        case 3:
        case 4:
            result = func_ov030_020cf984(pos, points);
            if (data_ov030_02109da0.lineBlend) {
                func_ov030_020cff00(pos, points);
            }
            break;
    }
    return result;
}

// Nonmatching: register allocation
BOOL func_ov030_020cf984(FieldPos* pos, FieldPos* points) {
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
    if (data_ov030_02109da0.blendMode == 3) {
        func_ov030_020cfd9c(&a, &b, &rel, points, data_ov030_020ed1c4, data_ov030_02109da0.model->sampleCount);
    } else if (data_ov030_02109da0.blendMode == 4) {
        center         = func_ov030_020cfe7c(pos, points, data_ov030_020ed1c4, 9);
        order          = data_ov030_020ed1e8[center];
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
        func_ov030_020cfd9c(&a, &b, &rel, points, order, 9);
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
    bx                                  = pb->x << FX32_SHIFT;
    by                                  = pb->y << FX32_SHIFT;
    edge.x                              = bx - (ax << FX32_SHIFT);
    edge.y                              = by - (ay << FX32_SHIFT);
    lenSq                               = FX_Mul(edge.x, edge.x) + FX_Mul(edge.y, edge.y);
    edge.z                              = 0;
    toOrigin.x                          = -bx;
    toOrigin.y                          = -by;
    toOrigin.z                          = 0;
    toPos.x                             = rel.x - bx;
    toPos.y                             = rel.y - by;
    toPos.z                             = 0;
    t                                   = FX_Divide64(Vec_DotProduct(&edge, &toOrigin), lenSq);
    perpOrigin.x                        = FO3D_Mul32x64c(edge.x, t) - toOrigin.x;
    perpOrigin.y                        = FO3D_Mul32x64c(edge.y, t) - toOrigin.y;
    perpOrigin.z                        = 0;
    distOrigin                          = Vec_Magnitude(&perpOrigin);
    t                                   = FX_Divide64(Vec_DotProduct(&edge, &toPos), lenSq);
    perpPos.x                           = FO3D_Mul32x64c(edge.x, t) - toPos.x;
    perpPos.y                           = FO3D_Mul32x64c(edge.y, t) - toPos.y;
    perpPos.z                           = 0;
    distPos                             = Vec_Magnitude(&perpPos);
    t                                   = FX_Divide64(distPos, distOrigin - distPos);
    hit.x                               = bx - (rel.x + FO3D_Mul32x64c(rel.x, t));
    hit.y                               = by - (rel.y + FO3D_Mul32x64c(rel.y, t));
    hit.z                               = 0;
    u                                   = FX_Divide(Vec_Magnitude(&hit), Vec_Magnitude(&edge));
    v                                   = FX_Divide(distPos, distOrigin);
    data_ov030_02109da0.sampleIdx[0]    = center;
    data_ov030_02109da0.sampleIdx[1]    = a;
    data_ov030_02109da0.sampleIdx[2]    = b;
    data_ov030_02109da0.sampleWeight[0] = FX32_ONE;
    data_ov030_02109da0.sampleWeight[1] = FX_Mul(u, FX32_ONE - v);
    data_ov030_02109da0.sampleWeight[2] = FX_Mul(FX32_ONE - u, FX32_ONE - v);
    return TRUE;
}

BOOL func_ov030_020cfd9c(s32* outA, s32* outB, Vec* rel, FieldPos* points, const s32* ring, s32 count) {
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

s32 func_ov030_020cfe7c(FieldPos* pos, FieldPos* points, const s32* list, s32 count) {
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
BOOL func_ov030_020cff00(FieldPos* pos, FieldPos* points) {
    Vec  dir;
    Vec  proj;
    s32  start = data_ov030_02109da0.lineStart;
    s32  end   = data_ov030_02109da0.lineEnd;
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
    dir.x                         = ex - sx;
    dir.y                         = ey - sy;
    dir.z                         = 0;
    len                           = Vec_Magnitude(&dir);
    t                             = FX_Divide64(dir.x * (px - sx) + dir.y * (py - sy), dir.x * dir.x + dir.y * dir.y);
    proj.x                        = FO3D_Mul32x64c(dir.x, t);
    proj.y                        = FO3D_Mul32x64c(dir.y, t);
    proj.z                        = 0;
    data_ov030_02109da0.lineRatio = FX_Divide(Vec_Magnitude(&proj), len);
    data_ov030_02109da0.unk_20    = 0;
    if (data_ov030_02109da0.lineRatio > FX32_ONE) {
        data_ov030_02109da0.lineRatio = FX32_ONE;
    }
    return TRUE;
}

void func_ov030_020d0020(void) {
    if (data_ov030_02109da0.model == NULL) {
        return;
    }
    data_ov030_02109da0.model->obj->unk_18 = FX32_ONE;
    data_ov030_02109da0.model->obj->unk_1C = 0;
    func_ov030_020cdcec(data_ov030_02109da0.model->obj);
    func_ov030_020cdde8(0, 0, 0);
    func_ov030_020cdd98(0);
    func_ov030_020cde20(0);
}

#define FO3D_NAME_SUFFIX(name, len, c) ((name)[(len) - 2] == 'a' && (name)[(len) - 1] == (c))

void func_ov030_020d0084(FieldResourceState* res) {
    s32 numNode;
    s32 count;
    s32 size;
    s32 len;

    if (data_ov030_02109da0.model == NULL || res == NULL) {
        return;
    }
    if (res->resMdl == NULL) {
        return;
    }
    data_ov030_02109da0.model->resMdl = res->resMdl;
    if (data_ov030_02109da0.model->resTex == NULL && res->resTex != NULL) {
        data_ov030_02109da0.model->resTex = res->resTex;
    }
    func_02028250(&data_ov030_02109da0.model->renderObj, data_ov030_02109da0.model->resMdl);
    numNode                              = func_ov030_020d05e8();
    count                                = numNode * data_ov030_02109da0.model->sampleCount;
    size                                 = count * sizeof(Vec);
    data_ov030_02109da0.model->jntTrans  = func_ov030_020cde54(size);
    data_ov030_02109da0.model->jntScale  = func_ov030_020cde54(size);
    data_ov030_02109da0.model->jntRot    = func_ov030_020cde54(count * sizeof(Mtx33));
    data_ov030_02109da0.model->nodeTrans = func_ov030_020cde54(numNode * sizeof(Vec));
    data_ov030_02109da0.model->nodeMask  = 0;
    data_ov030_02109da0.model->name      = res->name;
    if (res->name == NULL) {
        return;
    }
    len = func_020481e8(res->name);
    if (FO3D_NAME_SUFFIX(res->name, len, '0')) {
        data_ov030_02109da0.model->blendMode = 0;
        data_ov030_02109da0.model->animating = TRUE;
        func_0202843c(&data_ov030_02109da0.model->renderObj, func_ov030_020ce330, NULL, 6, 2);
    }
    if (FO3D_NAME_SUFFIX(res->name, len, '1')) {
        data_ov030_02109da0.model->offsetMode = 1;
    }
    if (FO3D_NAME_SUFFIX(res->name, len, '2')) {
        data_ov030_02109da0.model->offsetMode = 2;
    }
    if (FO3D_NAME_SUFFIX(res->name, len, '3')) {
        data_ov030_02109da0.model->blendMode = 2;
        data_ov030_02109da0.lineBlend        = TRUE;
        if (data_ov030_02109da0.model->sampleCount <= 9) {
            data_ov030_02109da0.lineStart = 8;
            data_ov030_02109da0.lineEnd   = 4;
        } else if (data_ov030_02109da0.model->sampleCount <= 25) {
            data_ov030_02109da0.lineStart = 10;
            data_ov030_02109da0.lineEnd   = 18;
        }
    }
    if (FO3D_NAME_SUFFIX(res->name, len, '4')) {
        data_ov030_02109da0.model->blendMode = 1;
        data_ov030_02109da0.model->animating = FALSE;
        func_0202843c(&data_ov030_02109da0.model->renderObj, func_ov030_020ce330, NULL, 6, 2);
    }
}

void func_ov030_020d0304(FieldResourceState* res) {
    s32 i;

    if (data_ov030_02109da0.model == NULL || res == NULL) {
        return;
    }
    if (res->resTex == NULL) {
        return;
    }
    data_ov030_02109da0.model->resTex = res->resTex;
    for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
        data_ov030_02109da0.model->anmObj[i]->resTex = data_ov030_02109da0.model->resTex;
    }
}

#define FO3D_ANM_CATEGORY(c0, c1) ((u16)((c1)[0] | ((c1)[1] << 8)))

// Nonmatching: the target reloads the category byte for every check
void func_ov030_020d036c(FieldResourceState* res) {
    s32                 slot;
    NNSG3dResAnmCommon* anm;

    if (data_ov030_02109da0.model == NULL || res == NULL) {
        return;
    }
    if (res->resAnm == NULL || data_ov030_02109da0.model->resMdl == NULL) {
        return;
    }
    slot = data_ov030_02109da0.model->anmCount;
    if (slot >= 3) {
        return;
    }
    data_ov030_02109da0.model->anmCount = slot + 1;
    data_ov030_02109da0.model->anmObj[slot] =
        func_0202bac0(func_ov030_020cd7fc(), res->resAnm, data_ov030_02109da0.model->resMdl);
    func_020281c0(data_ov030_02109da0.model->anmObj[slot], res->resAnm, data_ov030_02109da0.model->resMdl,
                  data_ov030_02109da0.model->resTex);
    func_020283c4(&data_ov030_02109da0.model->renderObj, data_ov030_02109da0.model->anmObj[slot]);
    data_ov030_02109da0.model->anmFrameMax[slot] = data_ov030_02109da0.model->anmObj[slot]->resAnm->numFrame << FX32_SHIFT;
    anm                                          = res->resAnm;
    if (anm->category0 == 'M' && anm->category1 == ('A' | ('M' << 8))) {
        s32 count;
        s32 size;

        data_ov030_02109da0.model->anmType[slot] = 1;
        if (data_ov030_02109da0.model->matAmbient != NULL) {
            return;
        }
        count                                 = func_ov030_020d060c() * data_ov030_02109da0.model->sampleCount;
        size                                  = count * sizeof(u16);
        data_ov030_02109da0.model->matAmbient = func_ov030_020cde54(size);
        data_ov030_02109da0.model->matDiffuse = func_ov030_020cde54(size);
        data_ov030_02109da0.model->matAlpha   = func_ov030_020cde54(count);
        return;
    }
    if (anm->category0 == 'M' && anm->category1 == ('A' | ('T' << 8))) {
        s32 count;
        s32 size;

        data_ov030_02109da0.model->anmType[slot] = 2;
        if (data_ov030_02109da0.model->texScaleS != NULL) {
            return;
        }
        count                                = func_ov030_020d060c() * data_ov030_02109da0.model->sampleCount;
        size                                 = count * sizeof(fx32);
        data_ov030_02109da0.model->texScaleS = func_ov030_020cde54(size);
        data_ov030_02109da0.model->texScaleT = func_ov030_020cde54(size);
        data_ov030_02109da0.model->texRotSin = func_ov030_020cde54(count * sizeof(s16));
        data_ov030_02109da0.model->texRotCos = func_ov030_020cde54(count * sizeof(s16));
        data_ov030_02109da0.model->texTransS = func_ov030_020cde54(size);
        data_ov030_02109da0.model->texTransT = func_ov030_020cde54(size);
        return;
    }
    if (anm->category0 == 'M' && anm->category1 == ('P' | ('T' << 8))) {
        data_ov030_02109da0.model->anmType[slot]         = 3;
        data_ov030_02109da0.model->anmObj[slot]->funcAnm = func_ov030_020d0f14;
    }
}

u8 func_ov030_020d05e8(void) {
    u8 numNode = 0;

    if (data_ov030_02109da0.model != NULL && data_ov030_02109da0.model->resMdl != NULL) {
        numNode = data_ov030_02109da0.model->resMdl->numNode;
    }
    return numNode;
}

u8 func_ov030_020d060c(void) {
    u8 numMat = 0;

    if (data_ov030_02109da0.model != NULL && data_ov030_02109da0.model->resMdl != NULL) {
        numMat = data_ov030_02109da0.model->resMdl->numMat;
    }
    return numMat;
}

void func_ov030_020d0630(FieldPos* pos, FieldPos* points) {
    Vec trans;
    s32 i;
    s32 done;

    if (data_ov030_02109da0.model == NULL) {
        return;
    }
    switch (data_ov030_02109da0.model->blendMode) {
        case 0:
            if (data_ov030_02109da0.model->animating) {
                for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
                    data_ov030_02109da0.model->anmObj[i]->frame += FX32_ONE;
                    if (data_ov030_02109da0.model->anmObj[i]->frame >= data_ov030_02109da0.model->anmFrameMax[i]) {
                        data_ov030_02109da0.model->anmObj[i]->frame = 0;
                    }
                }
            }
            break;
        case 1:
            if (data_ov030_02109da0.model->animating) {
                done = 0;
                for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
                    data_ov030_02109da0.model->anmObj[i]->frame += FX32_ONE;
                    if (data_ov030_02109da0.model->anmObj[i]->frame >= data_ov030_02109da0.model->anmFrameMax[i]) {
                        data_ov030_02109da0.model->anmObj[i]->frame = data_ov030_02109da0.model->anmFrameMax[i];
                        done++;
                    }
                }
                if (done >= data_ov030_02109da0.model->anmCount) {
                    data_ov030_02109da0.model->animating = FALSE;
                }
            }
            break;
        case 2:
            for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
                func_ov030_020d0af8(data_ov030_02109da0.model->anmObj[i], data_ov030_02109da0.model->anmFrameMax[i]);
            }
            break;
        case 3:
        case 4:
            func_ov030_020d0990();
            break;
    }
    if (data_ov030_02109da0.model->animating) {
        for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
            if (data_ov030_02109da0.model->anmType[i] == 3) {
                data_ov030_02109da0.model->anmObj[i]->frame += FX32_ONE;
                if (data_ov030_02109da0.model->anmObj[i]->frame > data_ov030_02109da0.model->anmFrameMax[i]) {
                    data_ov030_02109da0.model->anmObj[i]->frame = 0;
                }
            }
        }
    }
    if (data_ov030_02109da0.model->offsetMode == 1) {
        trans.x = pos->x - (points[0].x << FX32_SHIFT);
        trans.y = pos->y - (points[0].y << FX32_SHIFT);
        trans.z = 0;
        func_02028a80(&trans);
    } else if (data_ov030_02109da0.model->offsetMode == 2) {
        trans.x = data_ov030_02109da0.model->offset.x + (pos->x - (points[0].x << FX32_SHIFT));
        trans.y = (pos->y - (points[0].y << FX32_SHIFT)) - data_ov030_02109da0.model->offset.y;
        trans.z = 0;
        func_02028a80(&trans);
    }
    func_02028a44();
    func_0202b5d4();
    if (data_ov030_02109da0.model->visible) {
        func_02029270(&data_ov030_02109da0.model->renderObj);
    }
    {
        Vec zero = {0, 0, 0};

        func_02028a80(&zero);
    }
}

void func_ov030_020d08e8(Vec* offset) {
    if (data_ov030_02109da0.model != NULL) {
        data_ov030_02109da0.model->offset.x = offset->x;
        data_ov030_02109da0.model->offset.y = offset->y;
        data_ov030_02109da0.model->offset.z = 0;
    }
}

void func_ov030_020d0920(NNSG3dRS* rs) {
    s32 i;

    rs->cbVecFunc[6]   = func_ov030_020cdea0;
    rs->cbVecTiming[6] = 2;
    for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
        if (data_ov030_02109da0.model->anmType[i] == 1 || data_ov030_02109da0.model->anmType[i] == 2) {
            rs->cbVecFunc[4]   = func_ov030_020ce164;
            rs->cbVecTiming[4] = 2;
        }
    }
}

void func_ov030_020d0990(void) {
    s32  sample = data_ov030_02109da0.model->sample;
    fx32 frame;
    s32  i;

    if (sample == -1) {
        return;
    }
    if (sample < data_ov030_02109da0.model->sampleCount) {
        frame = data_ov030_02109da0.model->sampleFrames[sample] << FX32_SHIFT;
        for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
            data_ov030_02109da0.model->anmObj[i]->frame = frame;
        }
        func_02028450(&data_ov030_02109da0.model->renderObj, func_ov030_020d0920);
        data_ov030_02109da0.model->sample++;
        return;
    }
    for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
        data_ov030_02109da0.model->anmObj[i]->frame = 0;
    }
    data_ov030_02109da0.model->sample = -1;
    func_02028450(&data_ov030_02109da0.model->renderObj, NULL);
    for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
        if (data_ov030_02109da0.model->anmType[i] == 1) {
            data_ov030_02109da0.model->anmObj[i]->funcAnm = func_ov030_020cef78;
        } else if (data_ov030_02109da0.model->anmType[i] == 2) {
            data_ov030_02109da0.model->anmObj[i]->funcAnm = func_ov030_020cf214;
        } else if (data_ov030_02109da0.model->anmType[i] == 3) {
            data_ov030_02109da0.model->anmObj[i]->funcAnm = func_ov030_020d0ea0;
        } else {
            data_ov030_02109da0.model->anmObj[i]->funcAnm = func_ov030_020ce38c;
        }
    }
}

void func_ov030_020d0af8(NNSG3dAnmObj* anmObj, fx32 frameMax) {
    data_ov030_02109da0.model->sample = -1;
    anmObj->frame                     = data_ov030_02109da0.lineRatio * ((frameMax >> FX32_SHIFT) - 1);
}

static inline const NNSG3dResName* FO3D_GetResNameByIdx(const NNSG3dResDict* dict, u32 idx) {
    const NNSG3dResDictEntryHeader* entry = (const NNSG3dResDictEntryHeader*)((const u8*)dict + dict->ofsEntry);
    return (const NNSG3dResName*)((const u8*)entry + entry->ofsName) + idx;
}

// Nonmatching: the data_ov030_02109da0 address is hoisted out of the loop here but reloaded in the target
BOOL func_ov030_020d0b24(const char* name, Vec* pos) {
    s32                  i;
    s32                  numNode;
    FO3DModel*           model;
    const NNSG3dResDict* dict;

    if (data_ov030_02109da0.model == NULL) {
        return FALSE;
    }
    numNode = func_ov030_020d05e8();
    for (i = 0; i < numNode; i++) {
        model = data_ov030_02109da0.model;
        dict  = &model->resMdl->nodeDict;
        if (strcmp(name, FO3D_GetResNameByIdx(dict, i)->name) == 0) {
            *pos = model->nodeTrans[i];
            return TRUE;
        }
    }
    return FALSE;
}

fx32 func_ov030_020d0bc0(void) {
    if (data_ov030_02109da0.model != NULL && data_ov030_02109da0.model->anmCount > 0) {
        return data_ov030_02109da0.model->anmObj[0]->frame;
    }
    return 0;
}

BOOL func_ov030_020d0bf0(void) {
    s32 i;

    for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
        if (data_ov030_02109da0.model->anmType[i] == 3) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL func_ov030_020d0c38(const char* name) {
    if (data_ov030_02109da0.model->name != NULL && strcmp((char*)data_ov030_02109da0.model->name, (char*)name) == 0) {
        return TRUE;
    }
    return FALSE;
}

void func_ov030_020d0c70(void) {
    s32 i;

    if (data_ov030_02109da0.model == NULL) {
        return;
    }
    if ((u32)data_ov030_02109da0.model->blendMode > 1) {
        return;
    }
    data_ov030_02109da0.model->animating = TRUE;
    for (i = 0; i < data_ov030_02109da0.model->anmCount; i++) {
        data_ov030_02109da0.model->anmObj[i]->frame = 0;
    }
}

void func_ov030_020d0cd4(void) {
    if (data_ov030_02109da0.model != NULL) {
        data_ov030_02109da0.model->visible = TRUE;
    }
}

void func_ov030_020d0cf0(void) {
    if (data_ov030_02109da0.model != NULL) {
        data_ov030_02109da0.model->visible = FALSE;
    }
}

// Nonmatching: load scheduling of the texture offset
void func_ov030_020d0d0c(NNSG3dResTex* tex, const void* name, NNSG3dMatAnmResult* result) {
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
void func_ov030_020d0e48(NNSG3dResTex* tex, const void* name, NNSG3dMatAnmResult* result) {
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

void func_ov030_020d0ea0(NNSG3dMatAnmResult* result, NNSG3dAnmObj* anmObj, u32 mat) {
    const void*              resAnm = anmObj->resAnm;
    const NNSG3dTexPatAnmFV* fv     = func_0202bdf4(resAnm, mat, anmObj->frame >> FX32_SHIFT);

    func_ov030_020d0d0c((NNSG3dResTex*)anmObj->resTex, func_0202bdd4(resAnm, fv->idTex), result);
    if (fv->idPltt == 0xFF) {
        return;
    }
    func_ov030_020d0e48((NNSG3dResTex*)anmObj->resTex, func_0202bde4(resAnm, fv->idPltt), result);
}

void func_ov030_020d0f14(void) {}
