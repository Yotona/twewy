#include "DatMgr.h"
#include "EasyTask.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "System.h"
#include "common_data.h"
#include <types.h>

typedef struct {
    /* 0x00 */ Data* data;
    /* 0x04 */ void* unk_04;
    /* 0x08 */ u16   unk_08;
    /* 0x0A */ u16   unk_0A;
    /* 0x0C */ u32   unk_0C;
} StaffRollUnkA; // Size = 0x10

typedef struct {
    /* 0x00 */ Data* data;
    /* 0x04 */ void* unk_04;
} StaffRollUnkB; // Size = 0x8

typedef struct {
    /*0x00*/ StaffRollUnkA group1[4];
    /*0x40*/ StaffRollUnkB group2[4];
    /*0x60*/ StaffRollUnkB group3[3];
} StaffRollUnkC; // Size = 0x78

typedef struct {
    /* 0x00000 */ TaskPool*     unk_00;
    /* 0x00004 */ char          pad_04[0x8];
    /* 0x0000C */ s32           commandIndex;
    /* 0x00010 */ StaffRollUnkC unk_10[2];
    /* 0x00100 */ s32           unk_100;     // Might be a subset of unk_21614?
    /* 0x00104 */ Data*         commandlist; // Might be a subset of unk_21614?
    /* 0x00108 */ char          pad_108[0x48];
    /* 0x00150 */ void**        unk_150;     //?
    /* 0x00154 */ char          pad_154[0xD54];
    /* 0x00EA8 */ Data*         data;
    /* 0x00EAC */ char          pad_EAC[0x106D4];
    /* 0x11580 */ s32           unk_11580;
    /* 0x11584 */ MemPool       unk_11584;
    /* 0x11590 */ void*         unk_11590;
    /* 0x11594 */ char          pad_11594[0x90];
    /* 0x11624 */ StaffRollUnkC unk_11624[2];
    /* 0x11714 */ char          pad_11714[0xFE80];
    /* 0x21594 */ TaskPool      unk_21594;
    /* 0x21614 */ TaskPool*     unk_21614;
    /* 0x21618 */ char          pad_21618[0x104];
    /* 0x2171C */ void*         unk_2171C;
    /* 0x21720 */ char          pad_21720[0x78];
} StaffRollState; // size of state struct: 0x21798

const BinIdentifier data_ov042_020847e0   = {0x2A, "Apl_Suy/staff_cmdlist.bin"};
const BinIdentifier data_ov042_020847c0[] = {
    {0x2A, "Apl_Suy/Grp_Staff_PartA_Pack.bin"},
    {0x2A, "Apl_Suy/Grp_Staff_PartB_Pack.bin"},
    {0x2A, "Apl_Suy/Grp_Staff_PartC_Pack.bin"},
    {0x2A, "Apl_Suy/Grp_Staff_PartD_Pack.bin"},
};

void func_ov042_020842e4(void);
void func_ov042_02082f94(StaffRollState* state);
void func_ov042_02082ed8(StaffRollState* state);
void func_ov042_02082e9c(StaffRollState* state);

void func_ov042_020824e0(StaffRollState* state);
void func_ov042_02082620(StaffRollState* state);
void func_ov042_02082700(StaffRollState* state);

typedef void (*StaffRollFunc)(StaffRollState*);
const StaffRollFunc data_ov042_020847b4[3] = {func_ov042_020824e0, func_ov042_02082620, func_ov042_02082700};

void func_ov042_020824a0(StaffRollState* state) {
    s32 index = MainOvlDisp_GetRepeatCount();
    if (index == ~0x80000000) {
        func_ov042_02082700(state);
    } else {
        data_ov042_020847b4[index](state);
    }
}

extern s32 OVERLAY_31_ID;
void       func_ov042_020824e0(StaffRollState* state) {
    if (state == NULL) {
        state = Mem_AllocHeapTail(&gDebugHeap, sizeof(StaffRollState));
        Mem_SetSequence(&gDebugHeap, state, "Seq_Staff()");
        MainOvlDisp_SetCbArg(state);
        func_0203b3c0(state, NULL, sizeof(StaffRollState));
    }
    FS_LoadOverlay(0, &OVERLAY_31_ID);
    func_ov031_0210aa94(&state->unk_2171C);
    func_ov042_020842e4();
    state->unk_11580 = func_0200cef0(state);
    Mem_InitializeHeap(&state->unk_11584, &state->unk_11590, 0x10000);
    EasyTask_InitializePool(&state->unk_21594, &state->unk_11584, 0x200, 0, 0);
    state->unk_21614 = &state->unk_21594;
    data_02066aec    = 0;
    data_02066eec    = 0;
    DebugOvlDisp_Init();
    DebugOvlDisp_Push((OverlayCB)func_ov042_02082f94, &state->unk_21614, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov042_02082ed8, &state->unk_21614, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov042_02082e9c, &state->unk_21614, 0);
    MainOvlDisp_IncrementRepeatCount();
}

extern s32   OVERLAY_ID_30;
extern void* data_ov003_020d4f74;
void         func_ov042_02082620(StaffRollState* state) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    DebugOvlDisp_Run();
    EasyTask_ProcessPendingTasks(&state->unk_21594);
    EasyTask_UpdateActiveTasks(&state->unk_21594);
    if (DebugOvlDisp_IsStackAtBase() != FALSE) {
        OverlayTag tag;
        data_02074e10.unk_28 = 2;
        MainOvlDisp_ReplaceTop(&tag, &OVERLAY_ID_30, &data_ov003_020d4f74, 0, 0);
        return;
    }
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc.unk_00, 0);
    func_0200bf60(data_0206b3cc.unk_04, 0);
}

void       func_ov042_020846d0();
extern s32 OVERLAY_ID_31;
void       func_ov042_02082700(StaffRollState* state) {
    EasyTask_DestroyPool(&state->unk_21594);
    func_ov031_0210aabc(&state->unk_2171C);
    func_0200cef0(0);

    StaffRollUnkC* unkC = state->unk_11624;
    for (u32 unkC_index = 0; unkC_index < 2; unkC_index++) {
        for (u32 i = 0; i < 4; i++) {
            DatMgr_ReleaseData(unkC[unkC_index].group1[i].data);
            DatMgr_ReleaseData(unkC[unkC_index].group2[i].data);
            // func_02009e1c(data_0206b3c4[i << 2], unkC[unkC_index].group1[i].unk_04);
            // func_0200a02c(data_0206b3c4[i << 2], unkC[unkC_index].group2[i].unk_04);
        }

        for (u32 i = 0; i < 3; i++) {
            DatMgr_ReleaseData(unkC[unkC_index].group3[i].data);
            // func_0200a02c(data_0206b3c4[unkC_index << 2], unkC[unkC_index].group3[i].unk_04);
        }
    }
    func_ov042_020846d0();
    FS_UnloadOverlay(0, OVERLAY_ID_31);
    Mem_Free(&gDebugHeap, state);
}

void func_ov042_020828c4(StaffRollState* state, void* r1);
void func_ov042_0208293c(StaffRollState* state, void* r2);
void func_ov042_02082a30(StaffRollState* state, void* r2);
void func_ov042_02082b20(StaffRollState* state, void* r2);
void func_ov042_02082bf4(StaffRollState* state, void* r2);
void func_ov042_02082c98(StaffRollState* state, void* r2);
void func_ov042_02082cf4(StaffRollState* state, void* r2);
void func_ov042_02082d50(StaffRollState* state, void* r2);
void func_ov042_02082dac(StaffRollState* state, void* r2);
void func_ov042_02082df4(StaffRollState* state, void* r2);
void func_ov042_02082e3c(StaffRollState* state, void* r2);
void func_ov042_02082e74(StaffRollState* state, void* r2);
/*Nonmatching: this function should be matching, but there seems to be a objdiff bug not showing it as matching*/
void func_ov042_020827ec(StaffRollState* state, s32 r1, void* r2) {
    switch (r1) {
        case 0:
        case 1:
            break;
        case 3:
            func_ov042_0208293c(state, r2);
            return;
        case 2:
            func_ov042_020828c4(state, r2);
            return;
        case 4:
            func_ov042_02082a30(state, r2);
            return;
        case 5:
            func_ov042_02082b20(state, r2);
            return;
        case 6:
            func_ov042_02082bf4(state, r2);
            return;
        case 7:
            func_ov042_02082c98(state, r2);
            return;
        case 8:
            func_ov042_02082cf4(state, r2);
            return;
        case 9:
            func_ov042_02082d50(state, r2);
            return;
        case 10:
            func_ov042_02082dac(state, r2);
            return;
        case 11:
            func_ov042_02082df4(state, r2);
            return;
        case 12:
            func_ov042_02082e3c(state, r2);
            return;
        case 13:
            func_ov042_02082e74(state, r2);
            return;
    }
}

void func_ov042_020828c4(StaffRollState* state, void* r2) {
    // Not implemented: unknown bit magic
    /*
    if(r2->0x4 != 0){
       r1 = 1 << r2->0x2;
       if(r1 < 0){
           g_displaySettings.controls[r2->0x0 * 0x30]->layers = 0x1F;
           return;
       }else{
           g_displaySettings.controls[r2->0x0 * 0x30]->layers |= r1;
       }
    }else{
       r1 = 1 << r2->0x2;
       if(r1 < 0){
           g_displaySettings.controls[r2->0x0 * 0x30]->layers = 0;
           return;
       }else{
           g_displaySettings.controls[r2->0x0 * 0x30]->layers &= ~r1;
       }
   }
    */
}

void func_ov042_0208293c(StaffRollState* state, void* r2) {
    // var = state->unk_10[r2->unk_00 * 0x78][r2->unk_02 << 4];
    // var.unk_08 = r2->unk_04;
    // var.unk_0A = r2->unk_06;
    // var.unk_0C = r2->unk_08;
    /*
    if(var != 0){
        DatMgr_ReleaseData(var.data);
        var->unk_00 = 0; //var.data = NULL;?
    }
    */
    /*
    if(var->unk_04 != 0){
        func_02009e1c(data_0206b3c4[r2->unk_00 << 2]); //and if there's a second param it is var->unk_04
        var->unk_04 = 0;
    }
    */
    // var2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov042_020847c0[r2->unk_04 << 3], r2->unk_06, FALSE);
    /*
    if(var2 != NULL){
        param = var2->unk_08 + 0x20 + (var2->unk_08 + 0x20)->unk_08 //var2->pack->loadedBin + var2->pack->loadedBin->flags ??
    }else param = NULL;
    */
    // var->unk_00 = var2;
    // var->unk_04 = func_02009a74(data_0206b3c4[r2->unk_00 << 2], param, data_0206ab0c[r2->unk_00 * 0x220][r2->unk_02 *
    // 0x3C]); //0x220 = 0x78 x 4
}

void func_ov042_02082a30(StaffRollState* state, void* r2) {
    // var = state->unk_150[r2->unk_00 * 0x78][r2->unk_02 << 3];
    /*
    if(var != NULL){
        DatMgr_ReleaseData(var);
        var->unk_00 = 0;
    }
    if(var->unk_04 != NULL){
        func_0200afec(data_0206b3cc[r2->unk_00 << 2]); //and if there's a second param it is var->unk_04
        var->unk_04 = 0;
    }
    var2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov042_020847c0[r2->unk_04 << 3], r2->unk_06, FALSE);
    if(var2 != NULL){
        param = var2->unk_08 + 0x20 + (var2->unk_08 + 0x20)->unk_08 //var2->pack->loadedBin + var2->pack->loadedBin->flags ??
    } else param = NULL;
     var->unk_04 = func_0200acdc(data_0206b3cc[r2->unk_00 << 2], param, data_0206ab00[r2->unk_00 * 0x220][r2->unk_02 * 0x3C]);
    //0x220 = 0x78 x 4 var->unk_00 = r2->unk_04
    */
}

void func_ov042_02082b20(StaffRollState* state, void* r2) {
    // var = state->unk_10[r2->unk_00 * 0x78]->group3[r2->unk_02 << 3];
    /*
    if(var != NULL){
        DatMgr_ReleaseData(var->data);
        var->data = NULL;
    }
    if(var->unk_04 != NULL){
        func_0200afec(data_0206b3cc[r2->unk_00 << 2]); //and if there's a second param it is var->unk_04
        var->unk_04 = NULL;
    }
    var2 = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov042_020847c0[r2->unk_08 << 3], r2->unk_0A, FALSE);
    if(var2 != NULL){
        param = var2->unk_08 + 0x20 + (var2->unk_08 + 0x20)->unk_08 //var2->pack->loadedBin + var2->pack->loadedBin->flags ??
    } else param = NULL;
     var->unk_04 = func_0200adf8(data_0206b3cc[r2->unk_00 << 2], param, 0, r2->unk_04, r2->unk_06);
     var->unk_00 = var2;
    */
}

void func_ov042_02082bf4(StaffRollState* state, void* r2) {
    /*
    var = state->unk_10[r2->unk_00 * 0x78]->group1[r2->unk_02 << 4];
    if(var->unk_04 != NULL){
        func_02009e1c(data_0206b3c4[r2->unk_00 << 2]); //and if there's a second param it is var->unk_04
        var->unk_04 = NULL;
    }
    if(var->unk_00 != NULL){
        param = var2->unk_08 + 0x20 + (var2->unk_08 + 0x20)->unk_08 //var2->pack->loadedBin + var2->pack->loadedBin->flags ??
    } else param = NULL;
    var->unk_04 = func_02009a74(data_0206b3c4[r2->unk_00 << 2], param, data_0206ab0c[r2->unk_00 * 0x220][r2->unk_02 * 0x3C]);
    */
}

void func_ov042_02082c98(StaffRollState* state, void* r2) {
    /*
    var = state->unk_10[r2->unk_00 * 0x78]->group1[r2->unk_02 << 4];
    if(var != NULL){
        DatMgr_ReleaseData(var->data);
        var->data = NULL;
    }
    if(var->unk_04 != NULL){
        func_02009e1c(data_0206b3c4[r2->unk_00 << 2]); //and if there's a second param it is var->unk_04
        var->unk_04 = NULL;
    }
    */
}

void func_ov042_02082cf4(StaffRollState* state, void* r2) {
    /*
       var = state->unk_50[r2->unk_00 * 0x78][r2->unk_02 << 3];
       if(var != NULL){
           DatMgr_ReleaseData(var);
           var->unk_00 = NULL;
       }
       if(var->unk_04 != NULL){
           func_02009e1c(data_0206b3c4[r2->unk_00 << 2]); //and if there's a second param it is var->unk_04
           var->unk_04 = NULL;
       }
       */
}

void func_ov042_02082d50(StaffRollState* state, void* r2) {
    /*
        var = state->unk_50[r2->unk_00 * 0x78][r2->unk_02 << 3];
        if(var != NULL){
            DatMgr_ReleaseData(var);
            var->unk_00 = NULL;
        }
        if(var->unk_04 != NULL){
            func_0200afec(data_0206b3cc[r2->unk_00 << 2]); //and if there's a second param it is var->unk_04
            var->unk_04 = NULL;
        }
        */
}

void func_ov042_02082dac(StaffRollState* state, void* r2) {
    /*
    func_ov042_020832ec(state->unk_00, r2->unk_00, r2->unk_02, r2->unk_04, r2->unk_06, r2->unk_08, r2->unk_0A, r2->unk_0C);
    */
}

void func_ov042_02082df4(StaffRollState* state, void* r2) {
    /*
    func_ov042_02083678(state->unk_00, r2->unk_00, r2->unk_02, r2->unk_04, r2->unk_06, r2->unk_08, r2->unk_0A, r2->unk_0C);
    */
}

void func_ov042_02082e3c(StaffRollState* state, void* r2) {
    /*
    func_ov042_02083b00(state->unk_00, state, r2->unk_00, r2->unk_02, r2->unk_04, r2->unk_06);
    */
}

void func_ov042_02082e74(StaffRollState* state, void* r2) {
    // func_ov042_02083038(state->unk_00, state, r2->unk_02, r2->unk_04, r2->unk_06);
}

void func_ov042_02082e9c(StaffRollState* state) {
    CriSndMgr_SetLpFlg(FALSE);
    func_0202733c(0x22);
    state->commandlist = DatMgr_LoadRawData(1, 0, 0, &data_ov042_020847e0);
    DebugOvlDisp_Pop();
}

typedef struct {
    u16 unk_00; // StaffRollUnkC index? but also engineState index?
    u16 unk_02; // StaffRollUnkC.group1 index? but also bgSettings index?
    u16 unk_04; // Index into data_ov042_020847c0 (Grp_StaffRoll_Part array)
    u16 unk_06; // Pack Index
    u16 unk_08; // Index into data_ov042_020847c0 (Grp_StaffRoll_Part array)?
    u16 unk_0A; // Pack index?
    u16 unk_0C;
    u16 pad_0E; // is this padding?
} CommandUnk04; // size = 0x10

typedef struct {
    u16          StaffRollUnkCIndex; // or index of the command inside staff_cmdlist.bin?
    u16          group1Index;
    CommandUnk04 unk_04;
} Command; // Size = 0x14

void func_ov042_02082ed8(StaffRollState* state) {
    /*
    state->unk_100 &= ~2;
    command = state->commandlist->buffer[state->commandIndex * 0x14];
    if(command->unk_02 != 0){
        while(state->commandIndex != 0){
            if(command->unk_00 > state->unk_04)
                break;
            func_ov042_020827ec(state, command->unk_02, &command->unk_04);
            //advance to next entry
            command += 0x14;
            state->commandIndex++;
            if(command->unk_02 == 0)
                break;
        }
    }
    if(state->unk_08[state->commandIndex * 0x14].unk_02 == 0 && state->unk_08[state->commandIndex * 0x14].unk_00 <=
    state->unk_04){ DebugOvlDisp_Pop();
    }
    if((s32)state->unk_100 == 0){
        state->unk_04++;
    }
    */
}

void func_ov042_02082f94(StaffRollState* state) {
    DatMgr_ReleaseData(state->commandlist);
    func_02027388(0x22);
    CriSndMgr_SetLpFlg(TRUE);
    if (DebugOvlDisp_IsStackAtBase() == FALSE)
        DebugOvlDisp_Pop();
}

BOOL func_ov042_02082fc4(StaffRollState* state) {
    return state->unk_10->group1->unk_04 != NULL;
}

void func_ov042_02082fd8(StaffRollState* state, s32 r1) {
    /*
    if(state != NULL && state->unk_14 != NULL)
        return;
    if(state->unk_14->unk_00 == 0xFFFF)
        return;
    r1 &= 0xF;
    r1 |= (0xFFFF - 0x4F);
    r3 = (u16)r1;
    r0 = (0xFFFF - 0xF);
    r1 = (0xFFFF - 0x4F);
    while(state->unk_14->unk_02 != 0xFFFF){ //When does this loop end?
        if(state->unk_14->unk_00 == r1)
            state->unk_14->unk_00 = r3;
    }
    */
}

void func_ov042_02083038(void* r0, StaffRollState* r1, s32 r2, s32 r3, s32 s1) {
    // static const TaskHandle data_ov042_020847e8 = {"Tsk_BgBright", func_ov042_02083078, 0x18};
    /*
    stack[0x8] = r1;
    stack[0xC] = r2;
    stack[0x10] = s1;
    stack[0xE] = r3;
    EasyTask_CreateTask(r0, &data_ov042_020847e8, 0, 0, 0, s1);
    */
}

void func_ov042_02083078(struct TaskPool* r0, struct Task* r1, s32 r2, s32 r3) {
    /*
    switch(r3){
        case 0:
            return func_ov042_02083224(r0, r2);
        case 1:
          return func_ov042_02083224(r0); //TODO: check signature
    }
    return 1;
    */
}

s32 func_ov042_020830b0(void* r0, void* r1) {
    /*
    copy r1 content to r0 at offsets 0x0, 0x4, 0x6, 0x8
    r0->0xC = (int)(float)r1->0x4 (Round to nearest int, tho the actual code probably isn't like this?)
    r0->0x10 = FX_Divide((round(r1->0x6) - round(r1->0x4)), round(r1->0x8));
    r0->0x14 = 0;
    r0->0x16 = r1->0x8;
    r5 = if(r1->0x4 <= 0x10 && r1->0x4 < -0x10) -0x10 else 0x10;
    data_0206aab0[r4->0x0 * 0x30] = r5;
    return 1;
    */
}

s32 func_ov042_02083224(struct TaskPool* r0) {
    /*
        r0->0xC += r0->0x10;
        s32 ret = 1;
        if(r0->0x14 > r0->0x16){
            r0->0xC = round(r0->0x6);
            ret = 0;
        }
        data_0206aab0[r0->0x0 * 0x30] = something something max(r0->0xC, -16); ?
        return ret;
    */
}

void func_ov042_020832ec() { //(r0, r1, r2, r3, s1, s2, s3, s4)
    // static const TaskHandle data_ov042_020847f4 = {"Tsk_StaffBg", func_ov042_02083380, 0x20};
    /*
         struct {
         u32 r1, r2, r3,
         u16 s1, s2, s3, s4
         } param_struct; //Size = 0x14
     EasyTask_CreateTask(r0, &data_ov042_020847f4, NULL, 0, NULL, &param_struct);
    */
}

s32 func_ov042_02083344(void* r0) {
    /*
        switch(r0->0x12){
            case 0x20:
                return fraction(0x1F00 - r0->0x14);
            case 0x21:
                return fraction(r0->0x14);
            default:
                return r0->0x12;
        }
    */
}

s32 func_ov042_02083380(struct TaskPool* r0, struct Task* r1, s32 r2, s32 r3) {
    /*
        switch(r3){
            case 0:
                r1 = r2;
                return 1;
            case 1:
                return func_ov042_020835a0(r1->data);
            default:
                return 1;
        }
        return 1;
    */
}

s32 func_ov042_020833b8(
    void* r0, void* r1) { // TODO: are the arrays here legit or is the function just accessing a struct from a single array?
    /*
        data_0206acec[r1->0x0 * 0x220] = 1;
        if(r4->0x8 != 0){
            data_0206acf0[r1->0x0 * 0x220] = r4->0x4; (TODO: double check)
            data_0206acf4[r1->0x0 * 0x220] = 0x20;
        }else{
            data_0206acf0[r1->0x0 * 0x220] = r4->0x4; (TODO: double check)
            data_0206acf4[r1->0x0 * 0x220] = r4->0x8;
        }
        copy r1 content to r0 at offsets 0x0, 0x4, 0x8, 0xc, 0xe, 0x10, 0x12
        r0->0x14 = round(r1->0xC);
        r0->0x18 = FX_Divide((round(r1->0xE) - round(r1->0xC)), round(r1->0x10));
        r0->0x1C = 0;
        r0->0x1E = r1->0x10;
        data_0206acf8[r1->0x0 * 0x220] = r1->0xC;
        data_0206acfc[r1->0x0 * 0x220] = func_ov042_02083344(r0);
        return 1;
    */
}

s32 func_ov042_020835a0(void* r0) { // r0 = "Tsk_StaffBg" task->data
    /*
        r0->0x14 += r0->0x18;
        ret = 1;
        if(r0->0x1C > r0->0x1E){
            r0->0x14 = round(r0->0xE);
            ret = 0;
        }
        tempr5 = fraction(r0->0x14);
        tempr0 = func_ov042_02083344(r0);
        if(tempr5 > 0){
            if tempr5 > 0x1F, tempr5 = 0x1F;
        }else tempr5 = 0;
        if(tempr0 > 0){
            if tempr0 > 0x1F, tempr0 = 0x1F;
        }else tempr0 = 0;
        data_0206acf8[r0->0x0 * 0x220] = tempr5;
        data_0206acfc[r0->0x0 * 0x220] = tempr0;
        r0->0x1C++;
        return ret;
    */
}

void func_ov042_02083678() {
    /*
        Something something EasyTask_CreateTask(...) call. This function has a lot of parameters.
    */
}

s32 func_ov042_020836d0() {
    /*
         switch(r3){
             case 0:
                 return func_ov042_02083708(r0, r2);
             case 1:
                 return func_ov042_02083838(r0);
             default:
                 return 1;
         }
         return 1;
     */
}

s32 func_ov042_02083708(void* r0, void* r1) {
    /*
        func_0203b3c0(r0, 0x0, 0x28);
        copy r1 content to r0 at offsets 0x0, 0x4, 0x8, 0xa, 0xc, 0xe, 0x10
        r0->0x24 = 0;
        switch(data_0206aae4[r0->0x0 * 0x220][r0->0x4 * 0x3C]){
            case 1, 2, 3, 4, 5:
                data_0206aae4[r0->0x0 * 0x220][r0->0x4 * 0x2C].unk_124 = 1;
            case 0, default:
                data_0206aae4[r0->0x0 * 0x220][r0->0x4 * 8].unk_f0 = round(r0->0x8);
                data_0206aae4[r0->0x0 * 0x220][r0->0x4 * 8].unk_f4 = round(r0->0xa);
        }
        return 1;
    */
}

s32 func_ov042_02083838(void* r0) {
    /*
    ret = 1;
    if(r0->0x24 == 0){
        r0->0x26 = r0->0x10;
        r0->0x14 = round(r0->0x8);
        r0->0x18 = round(r0->0xa);
        r0->0x1C = FX_Divide((round(r0->0xC) - r0->0x14), round(r0->0x10));
        r0->0x20 = FX_Divide((round(r0->0xE) - r0->0x18), round(r0->0x10));
    }
    if(r0->0x24 > r0->0x26){
        r0->0x14 = round(r0->0xC);
        r0->0x18 = round(r0->0xE);
        ret = 0;
    }else{
        r0->0x14 += r0->0x1C;
        r0->0x18 += r0->0x20;
    }
    switch(data_0206aae4[r0->0x0 * 0x220][r0->0x4 * 0x3C]){
        case 1, 2, 3, 4, 5:
            data_0206aae4[r0->0x0 * 0x220][r0->0x4 * 0x2C].unk_124 = 1;
        case 0, default:
            data_0206aae4[r0->0x0 * 0x220][r0->0x4 * 8].unk_f0 = r0->0x14;
            data_0206aae4[r0->0x0 * 0x220][r0->0x4 * 8].unk_f4 = r0->0x18;
    }
    r0->0x24++;
    return ret;
    */
}

s32 func_ov042_02083b00() {
    /*
        Something something EasyTask_CreateTask(...) call. This function has a lot of parameters.
    */
}

s32 func_ov042_02083b48(void* r0, s32 r1, s32 r2) {
    // return r1 == 0? r0->0xEA8->0x8 + r2 * 2 : r0->0xEA8->0x8 + (r2 + 0x6A) * 2;
    return 0;
}

s32 func_ov042_02083b64(void* r0) {
    /*
    r4 = data_ov042_02084924[r0->0xC * 4];
    r5 = 0xC0;
    temp = func_ov042_02083b48(r0, r0->0xC, 0);
    if(r4->0x2 == 0){
        for(r3 = 0; r3 < r4->0x2; r3++){
            r5 += temp[r3 * 4] + 0xA; //is it temp[] or is it r0[]?
        }
    }
    return r5;
    */
}

void func_ov042_02083bb8() {
    /*
        //stacksize = 0x10
        stack[0x0] = r0->0x4;
        stack[0x2] = r0->0x8;
        func_ov042_020827ec(state, 0x6, &stack[0]);
    */
}

void func_ov042_02083be8(void* r0) {
    /*
        r0->0xEA0 += r0->0xEA4;
        if(r0->0xEA0 >= 0x100000){
            r0->0xEA0 -= 0x100000;
        }
        switch(data_0206aae4[r0->0x4 * 0x220][r0->0x8 * 0x3C]){
            case 1, 2, 3, 4, 5:
                data_0206aae4[r0->0x4 * 0x220][r0->0x8 * 0x2C].unk_124 = 1;
            case 0, default:
                data_0206aae4[r0->0x4 * 0x220][r0->0x8 * 8].unk_f0 = 0;
                data_0206aae4[r0->0x4 * 0x220][r0->0x8 * 8].unk_f4 = r0->0xEA0;
        }
    */
}

s32 func_ov042_02083c78(void* r0) {
    /*
        if(r0->0xE98 == NULL)
            return 0;
        if(func_ov042_02082fc4(&r0->0x10[r0->0xE98 * 0x7C]) == 0)
            return 0;
        temp = (0x100 - func_ov031_0210c5b4()) - 0x10; //TODO: check this function's signature
        temp2 = (&r0->0x10[r0->0xE98 * 0x7C]->0x70 - fraction(r0->0xEA0)) & 0xFF;
        if(temp2 > temp){
            if(temp2 <= (0x100 - func_ov031_0210c5b4()))
                return 1;
        }
        return 0;
    */
}

void func_ov042_02083cf0() {
    /*
        if(r0->0xE98 == -1)
            return;
        if(func_ov042_02082fc4(&r0->0x10[r0->0xE98 * 0x7C]) == 0)
            return;
        func_ov031_0210ab34(&r0->0x10[r0->0xE98 * 0x7C], 0x8);
        func_ov042_02082fd8(&r0->0x10[r0->0xE98 * 0x7C], 0x8);
        temp = &r0->0x50[r0->0x4 * 0x78][r0->0x8 * 8];
        r2 = if(temp == 0) 0 else{
            r2 = temp->0x8 + (temp->0x8 + 0x20)->0x8;
        }
        temp2 = &r0->0x0->0x10[r0->0x4 * 0x78][r0->0x8 * 16]
        r3 = if(temp2 == 0) 0 else{
            r3 = temp2->0x8 + (temp2->0x8 + 0x20)->0x8;
        }
        func_ov031_0210be18(&r0->0x10[r0->0xE98 * 0x7C], r2 + 4, r3 + 4, 0);
        func_ov031_0210aabc(&r0->0x10[r0->0xE98 * 0x7C]);
        func_ov042_02083bb8(r0);
        r0->0xE98 = (r0->0xE98 + 1) % 30;
    */
}

s32 func_ov042_02083df8() {
    /*
        if(r0->0xE9C == NULL)
            return 0;
        if(r0->0xEAC >= data_ov042_02084924[r0->0xC * 4]->0x2)
            return 0;
        r3 = (fraction(r0->0xEA0) + 0xC0) & 0xFF;
        if(r3 < &r0->0x10[r0->0xE9C * 0x7C]->0x70){
            r3 += 0x100;
        }
        r6 = r3 - &r0->0x10[r0->0xE9C * 0x7C]->0x70;
        r5 = func_ov042_02083b48(r0, r0->0xC, r0->0xEAC);
        r0 = func_ov031_0210c5b4(&r0->0x10[r0->0xE9C * 0x7C]) + r5->0x0;
        return r6 >= r0 ? 1 : 0;
    */
}

void func_ov042_02083e8c(void* r0) {
    /*
        if(r0->0xE9C == -1)
            r4 = 0;
        else{
            r4 = (r0->0xE9C + 1) % 30;
        }
        stack[0x0] = func_ov042_02083b48(r0, r0->0xC, r0->0xEAC);
        if(r0->0xE98 = -1)
            r0->0xE98 = r4;
        r0->0xE9C = r4;
        func_ov031_0210aa94(&r0->0x10[r4 * 0x7C]);
        func_ov031_0210b630(&r0->0x10[r4 * 0x7C], (u16)(data_ov042_02084924[r0->0xC * 4]->0x0 + &r0->0x10[r4 * 0x7C]->0xAC));
        func_ov031_0210ab34(&r0->0x10[r4 * 0x7C], 0xE);
        switch(stack[0]->0x1){
            case 0:
                func_ov031_0210ab3c(&r0->0x10[r4 * 0x7C], 0x1, 0x9C);
                break;
            case 1:
                func_ov031_0210ab3c(&r0->0x10[r4 * 0x7C], 0x0, 0x9C);
                break;
            case 2:
                func_ov031_0210ab3c(&r0->0x10[r4 * 0x7C], 0x2, 0x9C);
                break;
        }
        func_ov031_0210ab54(&r0->0x10[r4 * 0x7C], 1, 0);
        r9 = (fraction(r0->0xEA0) + 0xC0) & 0xFF;
        func_ov031_0210ab28(&r0->0x10[r4 * 0x7C], 0x64, r9);

        temp = &r0->0x50[r0->0x4 * 0x78][r0->0x8 * 8];
        if(temp == 0){
            r2 = 0;
        }else{
            r2 = temp->0x8 + (temp->0x8 + 0x20)->0x8;
        }
        temp2 = &r0->0x0->0x10[r0->0x4 * 0x78][r0->0x8 * 16];
        if(temp2 == 0){
            r3 = 0;
        }else{
            r3 = temp2->0x8 + (temp2->0x8 + 0x20)->0x8;
        }
        func_ov031_0210be18(&r0->0x10[r4 * 0x7C], r2 + 4, r3 + 4, 0);
        func_ov042_02083bb8(r0);
        r0->0xEAC++;
    */
}

s32 func_ov042_02084064() {
    /*
        switch(r3){
            case 0:
                return func_ov042_020840ac(r1->0x18, r2);
            case 1:
                return func_ov042_020841cc(r1->0x18);
            case 3:
                return func_ov042_0208429c(r1->0x18);
            default:
                return 1;
        }
    */
}

s32 func_ov042_020840ac(void* r0, void* r1) {
    /*
        func_0203b3c0(r0, 0x0, 0xEB4);
        copy r1 content to r0 at offsets 0x0, 0x4, 0x8, 0xc, 0xe
        r0->0xEA8 = DatMgr_LoadRawData(1, 0, 0, &data_ov042_02084814);
        r0->0xEA0 = 0x20000;
        r0->0xEA4 = FX_Divide(round(func_ov042_02083b64(r0)), round(r1->0xE));
        r0->0xEB0 = r1->0xE;
        r0->0xE98 = -1;
        r0->0xE9C = -1;
        return 1;
    */
}

s32 func_ov042_020841cc(void* r0) {
    /*
        if(r0->0x0[r0->0x4 * 0x78][r0->0x8 * 16]->0x10 != 0 && (r0->0x0 + 0x50)[r0->0x4 * 0x78][r0->0x8 * 8] == 0)
            return 0;
        if(func_ov042_02083df8() == 1){
            func_ov042_02083e8c(r0);
            r0->0x0->0x100 |= 2;
        }
        if((r0->0x0->0x100 << 0x1E >> 0x1F) == 0 && func_ov042_02083c78(r0) == 1){
            func_ov042_02083cf0(r0);
            r0->0x0->0x100 |= 2;
        }
        func_ov042_02083be8(r0);
        if(r0->0xEAB >= r0->0xEB0){
            return 0;
        }else{
            r0->0xEAB++;
            return 1;
        }
    */
}

s32 func_ov042_0208429c(void* r0) {
    /*
        DatMgr_ReleaseData(r0->0xEA8);
        for(r4 = 0; r4 < 30; r4++){
            if(func_ov042_02082fc4(&r0->0x10[r4 * 0x7C]) != 0)
                func_ov031_0210aabc(&r0->0x10[r4 * 0x7C]);
        }
        return 1;
    */
}

void func_ov042_020842e4() {
    // Not implemented: function is too big.
}

void func_ov042_020846d0() {
    Interrupts_RegisterVBlankCallback(0, 1);
}

void func_ov042_020846e4() {
    if (SystemStatusFlags.vblank == 0)
        return;
    func_02006380();
    DMA_Flush();
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    DC_PurgeRange(&data_02066aec, 0x400);
    GX_LoadBgPltt(&data_02066aec, 0, 0x200);
    GX_LoadObjPltt(&data_02066cec, 0, 0x200);
    DC_PurgeRange(&data_02066eec, 0x400);
    GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
    GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
}