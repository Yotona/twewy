#include "CellText.h"
#include "Engine/Resources/ResourceMgr.h"

extern s32 data_0205caa4;
extern s32 data_0205cac4;

static s32 CellText_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

TaskHandle data_0205ca88 = {"Tsk_CellText", CellText_RunTask, 0x430};

s32 func_02025e74(CellText* cellText, CellTextArgs* arg1) {
    s32   sp28;
    s32   sp24;
    void* sp20;
    s32   sp1C;
    s32   sp18;
    u16   sp12;
    u16   sp10;
    s32   spC;
    s32   sp8;
    s32   sp4;
    s32   sp0;
    void* var_ip;
    void* var_r4_2;
    s32   temp_r0;
    s32   temp_r1;
    s32   temp_r2;
    s32   temp_r3;
    s32   temp_r8;
    s32   var_r4;

    var_ip = &data_0205cac4;
    var_r4 = 4;
    do {
        temp_r0 = *(s32*)((s32)var_ip + 0);
        temp_r1 = *(s32*)((s32)var_ip + 4);
        temp_r2 = *(s32*)((s32)var_ip + 8);
        temp_r3 = *(s32*)((s32)var_ip + 12);
        var_ip  = (void*)((s32)var_ip + 0x10);
        sp0     = temp_r0;
        sp4     = temp_r1;
        sp8     = temp_r2;
        spC     = temp_r3;
        var_r4 -= 1;
    } while (var_r4 != 0);
    sp0      = *(s32*)((s32)var_ip + 0);
    sp4      = *(s32*)((s32)var_ip + 4);
    sp8      = *(s32*)((s32)var_ip + 8);
    sp0      = arg1->unk_00;
    sp4      = arg1->unk_04;
    sp24     = arg1->unk_08;
    sp28     = arg1->unk_0C;
    spC      = arg1->unk_14;
    sp10     = arg1->unk_18;
    sp12     = arg1->unk_1A;
    var_r4_2 = arg1->unk_10;
    temp_r8  = (((sp10 * sp12) + 0x1F) & ~0x1F) << 5;
    if (var_r4_2 == NULL) {
        var_r4_2 = &data_0205caa4;
    }

    void* temp_r7 = Mem_AllocHeapTail(&gMainHeap, temp_r8 + 0xA0);
    Mem_SetSequence(&gMainHeap, temp_r7, "Tsk_CellText");
    cellText->unk_004 = temp_r7;
    sp20              = temp_r7;
    cellText->unk_000 = sp0;
    cellText->unk_008 = ObjResMgr_AllocResource(g_ObjResourceManagers[sp0], 0, temp_r8, 0);
    cellText->unk_00C = PaletteMgr_AcquireContiguous(g_PaletteManagers[sp0], var_r4_2, 5, 1);
    cellText->unk_428 = sp10;
    cellText->unk_42C = sp12;
    sp18              = ((s32*)g_ObjResourceManagers[sp0])[2] + (*(s16*)((s32)cellText + 0x18) << 5);
    sp1C              = *(s32*)((s32)cellText + 0x1C);
    func_0201187c(&cellText->unk_010, &sp0);
    func_02011ecc(&cellText->unk_010);
    if (arg1->unk_24 != 0) {
        func_02011e94(&cellText->unk_010, arg1->unk_1C, arg1->unk_20);
    }
    return 1;
}

s32 func_0202600c(CellText* cellText) {
    return 1;
}

// Nonmatching
void CellText_Draw(CellText* cellText) {
    OamMgr_AddCellToOam(g_ObjResourceManagers[cellText->unk_000] + 0x108C, cellText->unk_428, cellText->unk_42C,
                        cellText->unk_04C, 0, cellText->unk_008 + 0x10);
}

void CellText_Free(CellText* cellText) {
    ObjResMgr_ReleaseResource(g_ObjResourceManagers[cellText->unk_000], cellText->unk_008);
    PaletteMgr_ReleaseResource(g_PaletteManagers[cellText->unk_000], cellText->unk_00C);
    func_02011994(&cellText->unk_010);
    Mem_Free(&gMainHeap, cellText->unk_004);
}

s32 CellText_Init(TaskPool* pool, Task* task, void* args) {
    CellText* cellText = task->data;

    func_02025e74(cellText, args);
}

s32 CellText_Update(TaskPool* pool, Task* task, void* args) {
    CellText* cellText = task->data;

    func_0202600c(cellText);
}

s32 CellText_Render(TaskPool* pool, Task* task, void* args) {
    CellText* cellText = task->data;

    CellText_Draw(cellText);
    return 1;
}

s32 CellText_Destroy(TaskPool* pool, Task* task, void* args) {
    CellText* cellText = task->data;

    CellText_Free(cellText);
    return 1;
}

s32 CellText_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_0205ca94 = {
        .initialize = CellText_Init,
        .update     = CellText_Update,
        .render     = CellText_Render,
        .cleanup    = CellText_Destroy,
    };
    return data_0205ca94.iter[stage](pool, task, args);
}
