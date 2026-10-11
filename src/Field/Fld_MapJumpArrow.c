// USA 0x020bc068-0x020bc4b8: map-jump arrow task. A Sprite-based marker that
// walks toward the selected destination; the task data begins with a 0x30-byte
// spawn header (a by-value FldSpawnTemplate rides inside it) followed by the
// arrow Sprite and the destination window task id.
#include "Engine/EasyTask.h"
#include "Field/Ov030Data.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include "SpriteMgr.h"

// Map-jump code remap entry (13 entries).
typedef struct {
    /* 0x00 */ s16 key;
    /* 0x02 */ s16 val;
} FldMapEntry;

// Task args, copied verbatim into the task data.
typedef struct {
    /* 0x00 */ FieldPos*        origin;
    /* 0x04 */ FieldPos         pos;
    /* 0x0C */ s32              dataType;
    /* 0x10 */ FldSpawnTemplate tmpl;
    /* 0x2C */ s32              destArea;
} MapJumpArgs; // Size: 0x30

typedef struct {
    /* 0x00 */ MapJumpArgs hdr;
    /* 0x30 */ FieldPos    pos;
    /* 0x38 */ s32         timer;
    /* 0x3C */ Sprite      sprite;
    /* 0x7C */ s32         unk_7C;
} MapJumpData; // Size: 0x80

extern const char        data_ov030_02107300[];
extern const FldMapEntry data_ov030_020e60a8[];

extern u16 func_ov030_020b7c34(s32 area);
extern s32 func_ov030_020aeadc(s16 arg0, s32 arg1);
extern s32 func_ov030_020c501c(TaskPool* pool, FieldPos* origin, FieldPos pos, s32 dataType, s32 code, s32 animIndex,
                               s32 useBig);

SpriteFrameInfo* func_ov030_020bc068(Sprite* sprite, s32 arg1, s32 arg2);
void             func_ov030_020bc104(Sprite* sprite, MapJumpData* data);
s16              func_ov030_020bc1dc(s16 arg0);
s32              func_ov030_020bc238(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bc324(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bc364(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bc3cc(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bc3f4(TaskPool* pool, Task* task, void* args, s32 stage);
s32 func_ov030_020bc43c(TaskPool* pool, FieldPos* origin, FieldPos pos, s32 dataType, FldSpawnTemplate tmpl, s32 destArea);

const TaskHandle data_ov030_020e6060 = {data_ov030_02107300, func_ov030_020bc3f4, sizeof(MapJumpData)};

const TaskStages data_ov030_020e606c = {
    {func_ov030_020bc238, func_ov030_020bc324, func_ov030_020bc364, func_ov030_020bc3cc}
};

const SpriteAnimation data_ov030_020e607c = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0C00,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = func_ov030_020bc068,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 3,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 6,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

SpriteFrameInfo* func_ov030_020bc068(Sprite* sprite, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(sprite, arg2);
}

void func_ov030_020bc104(Sprite* sprite, MapJumpData* data) {
    SpriteAnimation anim = data_ov030_020e607c;

    anim.dataType = data->hdr.dataType;
    anim.posX     = (data->pos.x - data->hdr.origin->x) >> 12;
    anim.posY     = (data->pos.y - data->hdr.origin->y) >> 12;
    if (data->hdr.tmpl.unk_0A == 2) {
        anim.animIndex = 2;
    } else if (data->hdr.tmpl.unk_0A == 6) {
        anim.animIndex = 3;
    } else if (data->hdr.tmpl.unk_0A == 7) {
        anim.animIndex = 4;
    }
    if (data->hdr.destArea >= 0x46) {
        anim.unk_20 = 5;
    }
    _Sprite_Load(sprite, &anim);
}

// Remaps a map-jump message code through data_ov030_020e60a8.
s16 func_ov030_020bc1dc(s16 arg0) {
    u32 i;

    if (arg0 == 0x2D42 && gSaveData.unk_2458 == 0x13) {
        return 0x2D55;
    }
    for (i = 0; i < 0xD; i++) {
        if (arg0 == data_ov030_020e60a8[i].key) {
            return data_ov030_020e60a8[i].val;
        }
    }
    return arg0;
}

s32 func_ov030_020bc238(TaskPool* pool, Task* task, void* args) {
    MapJumpData* data = task->data;
    s32          useBig;
    s16          code;

    data->hdr   = *(MapJumpArgs*)args;
    data->pos.x = data->hdr.pos.x;
    data->pos.y = data->hdr.pos.y;
    func_ov030_020bc104(&data->sprite, data);
    useBig      = 0;
    data->timer = 0;
    if (data->hdr.destArea >= 0x46) {
        useBig = 1;
    }
    if (data->hdr.destArea >= 0x46) {
        code = func_ov030_020bc1dc(func_ov030_020b7c34(data->hdr.destArea));
    } else if (func_ov030_020aeadc((s16)data->hdr.destArea, 1) == 0) {
        code = 0x2D56;
    } else {
        code = func_ov030_020bc1dc(func_ov030_020b7c34(data->hdr.destArea));
    }
    data->unk_7C = func_ov030_020c501c(pool, data->hdr.origin, data->pos, data->hdr.dataType, (s32)code,
                                       (s32)data->hdr.tmpl.unk_0A, useBig);
    return 1;
}

s32 func_ov030_020bc324(TaskPool* pool, Task* task, void* args) {
    MapJumpData* data = task->data;

    if ((data->timer & 3) == 0 && gSaveData.unk_2498 == 0) {
        Sprite_Update(&data->sprite);
    }
    data->timer = data->timer + 1;
    return 1;
}

s32 func_ov030_020bc364(TaskPool* pool, Task* task, void* args) {
    MapJumpData* data = task->data;

    if (func_ov030_020aefbc() != 0 || func_ov030_020aefe0() != 0) {
        return 1;
    }
    data->sprite.posX = (data->pos.x - data->hdr.origin->x) >> 12;
    data->sprite.posY = (data->pos.y - data->hdr.origin->y) >> 12;
    Sprite_RenderFrame(&data->sprite);
    return 1;
}

s32 func_ov030_020bc3cc(TaskPool* pool, Task* task, void* args) {
    MapJumpData* data = task->data;

    Sprite_Destroy(&data->sprite);
    EasyTask_DeleteTask(pool, data->unk_7C);
    return 1;
}

s32 func_ov030_020bc3f4(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov030_020e606c;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020bc43c(TaskPool* pool, FieldPos* origin, FieldPos pos, s32 dataType, FldSpawnTemplate tmpl, s32 destArea) {
    MapJumpArgs param;

    param.origin   = origin;
    param.pos.x    = pos.x;
    param.pos.y    = pos.y;
    param.dataType = dataType;
    param.tmpl     = tmpl;
    param.destArea = destArea;
    return EasyTask_CreateTask(pool, &data_ov030_020e6060, NULL, 0, NULL, &param);
}
