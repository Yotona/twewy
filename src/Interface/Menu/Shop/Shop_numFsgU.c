#include "Interface/Menu/Shop.h"

typedef struct {
    /* 0x000 */ Sprite      sprites[5];
    /* 0x140 */ BOOL        shouldRender[5];
    /* 0x154 */ ShopObject* shop;
} Shop_numFsgU; // Size: 0x158

typedef struct {
    /* 0x0 */ s32         unk_0;
    /* 0x4 */ ShopObject* shop;
    /* 0x8 */ s16         unk_8;
} Shop_numFsgU_Args;

SpriteFrameInfo* func_ov043_020bb7d4(Sprite* sprite, s32 arg, s32 mode);
s32              Shop_numFsgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_numFsgU = {"Tsk_Shop_numFsgU", Shop_numFsgU_RunTask, 0x158};

SpriteFrameInfo* func_ov043_020bb7d4(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

void func_ov043_020bb870(Shop_numFsgU* num) {
    // Not yet implemented
}

void func_ov043_020bb9b0(Shop_numFsgU* num, void* arg1, void* arg2) {
    // Not yet implemented
}

s32 Shop_numFsgU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_numFsgU*      num     = task->data;
    Shop_numFsgU_Args* numArgs = args;

    num->shop = numArgs->shop;
    func_ov043_020bb9b0(num, num->sprites, numArgs);
    return 1;
}

s32 Shop_numFsgU_Update(TaskPool* pool, Task* task, void* args) {
    Shop_numFsgU* num = task->data;

    func_ov043_020bb870(num);
    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&num->sprites[i]);
    }
    return 1;
}

s32 Shop_numFsgU_Render(TaskPool* pool, Task* task, void* args) {
    Shop_numFsgU* num = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (num->shouldRender[i])
            Sprite_RenderFrame(&num->sprites[i]);
    }
    return 1;
}

s32 Shop_numFsgU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_numFsgU* num = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&num->sprites[i]);
    }
    return 1;
}

s32 Shop_numFsgU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_numFsgU_Init,
        .update     = Shop_numFsgU_Update,
        .render     = Shop_numFsgU_Render,
        .cleanup    = Shop_numFsgU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_numFsgU_CreateTask(TaskPool* arg0, s32 arg1, ShopObject* shop) {
    Shop_numFsgU_Args args;
    args.unk_0 = arg1;
    args.shop  = shop;
    args.unk_8 = shop->unk_788;
    return EasyTask_CreateTask(arg0, &Tsk_Shop_numFsgU, NULL, 0, NULL, &args);
}
