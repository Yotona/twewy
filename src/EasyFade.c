#include "EasyFade.h"

extern s32 func_020264f0(TaskPool* pool, Task* task, s32 arg2, s32 arg3);

const TaskHandle Task_EasyFade = {"EasyFade", func_020264f0, 0};
