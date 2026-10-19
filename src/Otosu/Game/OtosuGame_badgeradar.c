#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ s32                    dataType;
    /* 0x04 */ Sprite                 sprite;
    /* 0x44 */ s32                    x;
    /* 0x48 */ s32                    y;     // Q12.12
    /* 0x4C */ u32                    pinId; // the badge being tracked
    /* 0x50 */ s32                    index; // the badge's player index, added to the animation
    /* 0x54 */ struct OtuBoardLayout* board;
    /* 0x58 */ s32                    linked;
    /* 0x5C */ s32                    isFirst; // 1 selects animation 5, anything else animation 1
} OtosuGame_badgeradar;                        // Size: 0x60

typedef struct {
    s32                    dataType;
    s32                    pinId;
    s32                    index;
    struct OtuBoardLayout* board;
    s32                    isFirst;
} OtosuGame_badgeradar_Args;

static s32              OtosuGame_badgeradar_RunTask(TaskPool* pool, Task* self, void* arg, s32 stage);
static SpriteFrameInfo* OtosuGame_badgeradar_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_badgeradar = {"Tsk_OtosuGame_badgeradar", OtosuGame_badgeradar_RunTask,
                                                    sizeof(OtosuGame_badgeradar)};

static const SpriteAnimation OtosuGame_badgeradar_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = OtosuGame_badgeradar_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
    .unk_18            = 2,
    .packIndex         = 8,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* OtosuGame_badgeradar_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 3);
}

static void OtosuGame_badgeradar_Load(OtosuGame_badgeradar* data, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_badgeradar_Anim;

    anim.owner     = data;
    anim.dataType  = data->dataType;
    anim.animIndex = (data->isFirst == 1) ? 5 : 1;
    anim.animIndex = anim.animIndex + (s16)data->index;

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_badgeradar_Init(TaskPool* pool, Task* self, void* args) {
    OtosuGame_badgeradar_Args* taskArgs = args;
    OtosuGame_badgeradar*      data     = (OtosuGame_badgeradar*)self->data;

    data->dataType = taskArgs->dataType;
    data->pinId    = taskArgs->pinId;
    data->index    = taskArgs->index;
    data->board    = taskArgs->board;
    data->linked   = 0;
    data->isFirst  = taskArgs->isFirst;
    data->x        = 0;
    data->y        = 0;

    OtosuGame_badgeradar_Load(data, &data->sprite);
    return 1;
}

static s32 OtosuGame_badgeradar_Update(TaskPool* pool, Task* self, void* args) {
    OtosuGame_badgeradar* data  = (OtosuGame_badgeradar*)self->data;
    OtosuGame_badge*      child = (OtosuGame_badge*)EasyTask_GetTaskData(pool, data->pinId);
    Vec2p                 pt;
    s32                   skew;

    if (child != NULL) {
        if (OtosuGame_badge_IsTargetable(child, 0) == 0) {
            data->linked = 0;
        } else {
            data->linked = 1;
            OtosuGame_badge_GetPos(child, &pt);
            data->linked = 1;

            data->x = pt.x - 0xA0000;
            skew    = (0x28 - (data->board->width - 0xA)) / 2;
            data->x = (data->x + (skew << 17)) / 8 + 0x50000;

            data->y = pt.y - 0xA0000;
            skew    = (0x28 - (data->board->height - 0xA)) / 2;
            data->y = (data->y + (skew << 17)) / 8 + 0x10000;

            Sprite_Update(&data->sprite);
        }
    } else {
        data->linked = 0;
    }

    return 1;
}

static s32 OtosuGame_badgeradar_Render(TaskPool* pool, Task* self, void* args) {
    OtosuGame_badgeradar* data = (OtosuGame_badgeradar*)self->data;

    if (data->linked != 0) {
        data->sprite.posX = data->x >> 12;
        data->sprite.posY = data->y >> 12;
        Sprite_RenderFrame(&data->sprite);
    }

    return 1;
}

static s32 OtosuGame_badgeradar_Destroy(TaskPool* pool, Task* self, void* args) {
    OtosuGame_badgeradar* data = (OtosuGame_badgeradar*)self->data;

    Sprite_Release(&data->sprite);
    return 1;
}

static s32 OtosuGame_badgeradar_RunTask(TaskPool* pool, Task* self, void* arg, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_badgeradar_Init,
        .update     = OtosuGame_badgeradar_Update,
        .render     = OtosuGame_badgeradar_Render,
        .cleanup    = OtosuGame_badgeradar_Destroy,
    };

    return stages.iter[stage](pool, self, arg);
}

s32 OtosuGame_badgeradar_CreateTask(TaskPool* pool, s32 dataType, s32 pinId, s32 index, OtuBoardLayout* board, s32 isFirst) {
    OtosuGame_badgeradar_Args args;

    args.dataType = dataType;
    args.pinId    = pinId;
    args.index    = index;
    args.board    = board;
    args.isFirst  = isFirst;

    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_badgeradar, NULL, 0, NULL, &args);
}
