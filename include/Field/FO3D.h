#ifndef FIELD_FO3D_H
#define FIELD_FO3D_H

#include "Field/FieldTypes.h"
#include "Field/Shinigami.h"
#include <nitro/fx.h>

// Field 3D object viewer: loads a scene's model archive (BMD0 model, BCA0/BMA0/BTA0/BTP0
// animations, BTX0 textures, COLI collision) and drives one FO3D model per model node.

#define FO3D_MAGIC(a, b, c, d) ((u32)(a) | ((u32)(b) << 8) | ((u32)(c) << 16) | ((u32)(d) << 24))

#define FO3D_CHUNK_BMD0 FO3D_MAGIC('B', 'M', 'D', '0')
#define FO3D_CHUNK_BCA0 FO3D_MAGIC('B', 'C', 'A', '0')
#define FO3D_CHUNK_BMA0 FO3D_MAGIC('B', 'M', 'A', '0')
#define FO3D_CHUNK_BTA0 FO3D_MAGIC('B', 'T', 'A', '0')
#define FO3D_CHUNK_BTP0 FO3D_MAGIC('B', 'T', 'P', '0')
#define FO3D_CHUNK_BTX0 FO3D_MAGIC('B', 'T', 'X', '0')
#define FO3D_CHUNK_COLI FO3D_MAGIC('C', 'O', 'L', 'I')

#define FO3D_VIEWER_EXTRA_MAX 2
#define FO3D_ANIM_MAX         4

// NitroSystem G3D structures, only as far as the field viewer touches them.
typedef struct {
    /* 0x0 */ u8  revision;
    /* 0x1 */ u8  numEntry;
    /* 0x2 */ u16 sizeDictBlk;
    /* 0x4 */ u16 dummy_;
    /* 0x6 */ u16 ofsEntry;
} NNSG3dResDict;

typedef struct {
    /* 0x0 */ u16 sizeUnit;
    /* 0x2 */ u16 ofsName;
} NNSG3dResDictEntryHeader;

typedef struct NNSG3dResMdl {
    /* 0x00 */ u32           size;
    /* 0x04 */ u32           ofsSbc;
    /* 0x08 */ u32           ofsMat;
    /* 0x0C */ u32           ofsShp;
    /* 0x10 */ u32           ofsEvpMtx;
    /* 0x14 */ u8            sbcType;
    /* 0x15 */ u8            scalingRule;
    /* 0x16 */ u8            texMtxMode;
    /* 0x17 */ u8            numNode;
    /* 0x18 */ u8            numMat;
    /* 0x19 */ u8            numShp;
    /* 0x1A */ u8            firstUnusedMtxStackID;
    /* 0x1B */ u8            dummy_;
    /* 0x1C */ u8            unk_1C[0x40 - 0x1C];
    /* 0x40 */ NNSG3dResDict nodeDict;
} NNSG3dResMdl;

typedef struct {
    const void* callbacks;
    u32         state0;
    u32         state1;
    u32         state2;
} FieldAllocatorState;

typedef struct NNSG3dResTex {
    /* 0x00 */ u32           kind;
    /* 0x04 */ u32           size;
    /* 0x08 */ u32           texVramKey;
    /* 0x0C */ u16           texSize;
    /* 0x0E */ u16           texOfsDict;
    /* 0x10 */ u16           texFlag;
    /* 0x12 */ u16           texDummy;
    /* 0x14 */ u32           texOfsData;
    /* 0x18 */ u32           tex4x4VramKey;
    /* 0x1C */ u16           tex4x4Size;
    /* 0x1E */ u16           tex4x4OfsDict;
    /* 0x20 */ u16           tex4x4Flag;
    /* 0x22 */ u16           tex4x4Dummy;
    /* 0x24 */ u32           tex4x4OfsData;
    /* 0x28 */ u32           tex4x4OfsPlttIdx;
    /* 0x2C */ u32           plttVramKey;
    /* 0x30 */ u16           plttSize;
    /* 0x32 */ u16           plttFlag;
    /* 0x34 */ u16           plttOfsDict;
    /* 0x36 */ u16           plttDummy;
    /* 0x38 */ u32           plttOfsData;
    /* 0x3C */ NNSG3dResDict texDict;
} NNSG3dResTex;

typedef struct {
    /* 0x0 */ u8  category0;
    /* 0x1 */ u8  revision;
    /* 0x2 */ u16 category1;
    /* 0x4 */ u16 numFrame;
} NNSG3dResAnmCommon;

typedef struct {
    /* 0x00 */ u32   flag;
    /* 0x04 */ Vec   scale;
    /* 0x10 */ Vec   scaleEx0;
    /* 0x1C */ Vec   scaleEx1;
    /* 0x28 */ Mtx33 rot;
    /* 0x4C */ Vec   trans;
} NNSG3dJntAnmResult; // Size: 0x58

typedef struct {
    /* 0x00 */ u32  flag;
    /* 0x04 */ u32  prmMatColor0;
    /* 0x08 */ u32  prmMatColor1;
    /* 0x0C */ u32  prmPolygonAttr;
    /* 0x10 */ u32  prmTexImage;
    /* 0x14 */ u32  prmTexPltt;
    /* 0x18 */ fx32 scaleS;
    /* 0x1C */ fx32 scaleT;
    /* 0x20 */ s16  rotSin;
    /* 0x22 */ s16  rotCos;
    /* 0x24 */ fx32 transS;
    /* 0x28 */ fx32 transT;
    /* 0x2C */ u16  origWidth;
    /* 0x2E */ u16  origHeight;
    /* 0x30 */ fx32 magW;
    /* 0x34 */ fx32 magH;
} NNSG3dMatAnmResult; // Size: 0x38

typedef struct NNSG3dAnmObj NNSG3dAnmObj;

struct NNSG3dAnmObj {
    /* 0x00 */ fx32                frame;
    /* 0x04 */ fx32                ratio;
    /* 0x08 */ NNSG3dResAnmCommon* resAnm;
    /* 0x0C */ void*               funcAnm;
    /* 0x10 */ NNSG3dAnmObj*       next;
    /* 0x14 */ const NNSG3dResTex* resTex;
    /* 0x18 */ u8                  priority;
    /* 0x19 */ u8                  numMapData;
    /* 0x1A */ u16                 mapData[1];
};

typedef struct NNSG3dRS NNSG3dRS;
typedef void (*NNSG3dSbcCallBackFunc)(NNSG3dRS* rs);

struct NNSG3dRS {
    /* 0x00 */ u8*                   c;
    /* 0x04 */ void*                 renderObj;
    /* 0x08 */ u32                   flag;
    /* 0x0C */ NNSG3dSbcCallBackFunc cbVecFunc[32];
    /* 0x8C */ u8                    cbVecTiming[32];
    /* 0xAC */ u8                    currentNode;
    /* 0xAD */ u8                    currentMat;
    /* 0xAE */ u8                    currentNodeDesc;
    /* 0xAF */ u8                    dummy_;
    /* 0xB0 */ NNSG3dMatAnmResult*   matAnmResult;
    /* 0xB4 */ NNSG3dJntAnmResult*   jntAnmResult;
};

typedef struct {
    /* 0x00 */ u8 data[0x54];
} NNSG3dRenderObj; // Size: 0x54

typedef struct Field3DObject {
    /* 0x00 */ s32   unk_00;
    /* 0x04 */ s32   unk_04;
    /* 0x08 */ s32   unk_08;
    /* 0x0C */ s32   unk_0C;
    /* 0x10 */ s32   unk_10;
    /* 0x14 */ s32   unk_14;
    /* 0x18 */ s32   unk_18;
    /* 0x1C */ s32   unk_1C;
    /* 0x20 */ Mtx33 unk_20;
    /* 0x44 */ u16   unk_44;
    /* 0x46 */ char  unk_46[0x48 - 0x46];
    /* 0x48 */ s32   unk_48;
    /* 0x4C */ s32   unk_4C;
    /* 0x50 */ s32   unk_50;
    /* 0x54 */ s32   unk_54;
    /* 0x58 */ s32   unk_58;
} Field3DObject; // Size: 0x5C

typedef struct FO3DModel {
    /* 0x00 */ Field3DObject*  obj;
    /* 0x04 */ NNSG3dResMdl*   resMdl;
    /* 0x08 */ NNSG3dResTex*   resTex;
    /* 0x0C */ NNSG3dAnmObj*   anmObj[3];
    /* 0x18 */ Vec*            jntTrans;
    /* 0x1C */ Vec*            jntScale;
    /* 0x20 */ Mtx33*          jntRot;
    /* 0x24 */ u16*            matAmbient;
    /* 0x28 */ u16*            matDiffuse;
    /* 0x2C */ u8*             matAlpha;
    /* 0x30 */ fx32*           texScaleS;
    /* 0x34 */ fx32*           texScaleT;
    /* 0x38 */ s16*            texRotSin;
    /* 0x3C */ s16*            texRotCos;
    /* 0x40 */ fx32*           texTransS;
    /* 0x44 */ fx32*           texTransT;
    /* 0x48 */ NNSG3dRenderObj renderObj;
    /* 0x9C */ Vec*            nodeTrans;
    /* 0xA0 */ s32             sample;
    /* 0xA4 */ s32             sampleCount;
    /* 0xA8 */ const s32*      sampleFrames;
    /* 0xAC */ u32             nodeMask;
    /* 0xB0 */ s32             blendMode;
    /* 0xB4 */ s32             anmType[3];
    /* 0xC0 */ fx32            anmFrameMax[3];
    /* 0xCC */ s32             anmCount;
    /* 0xD0 */ BOOL            animating;
    /* 0xD4 */ BOOL            visible;
    /* 0xD8 */ s32             offsetMode;
    /* 0xDC */ Vec             offset;
    /* 0xE8 */ const char*     name;
} FO3DModel; // Size: 0xEC

typedef struct FieldResourceState {
    /* 0x00 */ void*         modelFile;
    /* 0x04 */ void*         anmFile;
    /* 0x08 */ void*         resAnm;
    /* 0x0C */ u32           unk_0C;
    /* 0x10 */ NNSG3dResMdl* resMdl;
    /* 0x14 */ NNSG3dResTex* resTex;
    /* 0x18 */ u32           unk_18;
    /* 0x1C */ const char*   name;
} FieldResourceState; // Size: 0x20

typedef struct FieldPtrList {
    /* 0x0 */ void** items;
    /* 0x4 */ s32    capacity;
} FieldPtrList; // Size: 0x8

typedef struct {
    /* 0x00 */ s32                  scene;
    /* 0x04 */ FieldResourceState** resources;
    /* 0x08 */ FO3DModel**          models;
    /* 0x0C */ s32                  modelCount;
    /* 0x10 */ s32                  resourceCount;
    /* 0x14 */ FieldPos             pos;
    /* 0x1C */ u32                  unk_1C;
    /* 0x20 */ u32                  unk_20;
    /* 0x24 */ FieldPos*            points;
    /* 0x28 */ FieldPtrList*        ptrList;
    /* 0x2C */ ShiniWorld           world;
    /* 0x40 */ s32                  extraCount;
    /* 0x44 */ FieldResourceState** extraResources[FO3D_VIEWER_EXTRA_MAX];
    /* 0x4C */ FO3DModel**          extraModels[FO3D_VIEWER_EXTRA_MAX];
    /* 0x54 */ s32                  extraModelCount[FO3D_VIEWER_EXTRA_MAX];
    /* 0x5C */ s32                  extraResourceCount[FO3D_VIEWER_EXTRA_MAX];
    /* 0x64 */ void*                modelFile;
    /* 0x68 */ void*                extraModelFiles[FO3D_VIEWER_EXTRA_MAX];
} FieldViewerState; // Size: 0x70

extern FieldViewerState* data_ov030_02109ddc;

// FOAllocator
void                 func_ov030_020cd7d4(void);
FieldAllocatorState* func_ov030_020cd7fc(void);

// FO3DRes
FieldResourceState* func_ov030_020cdac0(void);
void                func_ov030_020cdb18(void);
void                func_ov030_020cdb5c(FieldResourceState* state);
FieldResourceState* func_ov030_020cdb6c(void);
u8                  func_ov030_020cdb7c(void* resource);
s32                 func_ov030_020cdba0(void* resource);
void                func_ov030_020cdbac(void* resource, s32 index);
void                func_ov030_020cdc40(void* resource);
void                func_ov030_020cdc70(void* resource, s32 index);
void*               func_ov030_020cdca0(s32 size);
void                func_ov030_020cdcd4(void* data);

// FO3DObj
Field3DObject* func_ov030_020cdcfc(void);
void           func_ov030_020cdcec(Field3DObject* obj);
void           func_ov030_020cdd74(void);
void           func_ov030_020cdd98(s16 rotY);
void           func_ov030_020cdde8(s32 x, s32 y, s32 z);
void           func_ov030_020cde20(s32 arg0);
void*          func_ov030_020cde54(s32 size);
void           func_ov030_020cde88(void* data);

// FO3DObjMgr
FO3DModel* func_ov030_020cf554(void);
void       func_ov030_020cf6a0(void);
void       func_ov030_020cf838(FO3DModel* model);
void       func_ov030_020cf848(const s32* frames, s32 count);
BOOL       func_ov030_020cf910(FieldPos* pos, FieldPos* points);
void       func_ov030_020d0020(void);
void       func_ov030_020d0084(FieldResourceState* res);
void       func_ov030_020d0304(FieldResourceState* res);
void       func_ov030_020d036c(FieldResourceState* res);
void       func_ov030_020d0630(FieldPos* pos, FieldPos* points);
void       func_ov030_020d08e8(Vec* pos);
BOOL       func_ov030_020d0b24(const char* name, Vec* pos);
fx32       func_ov030_020d0bc0(void);
BOOL       func_ov030_020d0bf0(void);
BOOL       func_ov030_020d0c38(const char* name);
void       func_ov030_020d0c70(void);
void       func_ov030_020d0cd4(void);
void       func_ov030_020d0cf0(void);

// FO3DViewerImd
void*             func_ov030_020d0f18(s32 size);
void              func_ov030_020d0f4c(void* data);
FieldViewerState* func_ov030_020d0f64(void);
void              func_ov030_020d1060(void);

// FOFileList
void* func_ov030_020d21e0(void);
void  func_ov030_020d2218(void);
u32   func_ov030_020d224c(const char* path, BOOL loadTable);
void  func_ov030_020d2334(void);
void* func_ov030_020d2378(void* buffer, u32 wantedIndex);
u32   func_ov030_020d246c(void);

// FOFileList
void* func_ov030_020d2488(s32 size);
void  func_ov030_020d24bc(void* data);

// FOPtrList
void*         func_ov030_020d24d4(void);
void          func_ov030_020d2504(void);
void          func_ov030_020d2538(FieldPtrList* list);
FieldPtrList* func_ov030_020d2548(void);
void          func_ov030_020d2558(s32 capacity);
void          func_ov030_020d258c(void);
s32           func_ov030_020d25cc(void* item);

// FO3DViewer
void* func_ov030_020d2188(s32 size);
void  func_ov030_020d21bc(void* data);
void  func_ov030_020d21d4(void* data);

#endif // FIELD_FO3D_H
