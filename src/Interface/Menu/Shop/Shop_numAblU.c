#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x000 */ Sprite      sprites[5];
    /* 0x140 */ BOOL        shouldRender[5];
    /* 0x154 */ ShopObject* shop;
} Shop_numAblU; // Size: 0x158

typedef struct {
    s32         unk_00;
    ShopObject* shop;
} Shop_numAblU_Args;

SpriteFrameInfo* Shop_numAblU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
s32              Shop_numAblU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_numAblU = {"Tsk_Shop_numAblU", Shop_numAblU_RunTask, sizeof(Shop_numAblU)};

SpriteFrameInfo* Shop_numAblU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void func_ov043_020bb2f0(Shop_numAblU* num) {
    // Not yet implemented
}

void Shop_numAblU_Load(Shop_numAblU* num, Sprite* sprite, Shop_numAblU_Args* numArgs) {
    // Not yet implemented
}

s32 Shop_numAblU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_numAblU*      num     = task->data;
    Shop_numAblU_Args* numArgs = args;

    num->shop = numArgs->shop;
    Shop_numAblU_Load(num, num->sprites, numArgs);
    return 1;
}

s32 Shop_numAblU_Update(TaskPool* pool, Task* task, void* args) {
    Shop_numAblU* num = task->data;

    func_ov043_020bb2f0(num);
    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&num->sprites[i]);
    }
    return 1;
}

s32 Shop_numAblU_Render(TaskPool* pool, Task* task, void* args) {
    Shop_numAblU* num = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (num->shouldRender[i]) {
            Sprite_RenderFrame(&num->sprites[i]);
        }
    }
    return 1;
}

s32 Shop_numAblU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_numAblU* num = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&num->sprites[i]);
    }
    return 1;
}

s32 Shop_numAblU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_numAblU_Init,
        .update     = Shop_numAblU_Update,
        .render     = Shop_numAblU_Render,
        .cleanup    = Shop_numAblU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_numAblU_CreateTask(TaskPool* pool, s32 arg1, ShopObject* shop) {
    Shop_numAblU_Args args;
    args.unk_00 = arg1;
    args.shop   = shop;
    return EasyTask_CreateTask(pool, &Tsk_Shop_numAblU, NULL, 0, NULL, &args);
}
