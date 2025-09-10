#ifndef MSL_RUNTIME_EXCEPTIONHANDLER_H
#define MSL_RUNTIME_EXCEPTIONHANDLER_H

#include <MSL/Runtime/ExceptionTables.h>
#include <MSL/Runtime/MWException.h>

typedef struct TargetContext {
    unsigned long  GPR[16];
    char*          throwSP;
    unsigned long  frame_size;
    unsigned long  argument_size;
    unsigned short saved_GPRs;
    bool           has_flushback;
    bool           has_frame_ptr;
    bool           is_Thumb;
} TargetContext;

typedef struct ThrowContext {
    u8*           throwtype;
    void*         location;
    void*         dtor;
    CatchInfo*    catchinfo;
    char*         returnaddr;
    char*         SP;
    char*         FP;
    TargetContext target;
} ThrowContext;

typedef struct TargetExceptionInfo {
    int dummy;
} TargetExceptionInfo;

typedef struct ExceptionInfo {
    char*                current_function;
    char*                exception_record;
    char*                action_pointer;
    ExceptionTableIndex* exception_table_start;
    ExceptionTableIndex* exception_table_end;
    TargetExceptionInfo  target;
} ExceptionInfo;

#define UNWIND_FRAME_IS_THUMB      0x80
#define UNWIND_FRAME_IS_DYNAMIC    0x40
#define UNWIND_FRAME_HAS_FLUSHBACK 0x20

#define __FunctionPointer(info, context, fp)          (fp)
#define __AdjustReturnAddress(info, context, retaddr) (retaddr)
#define __LocalVariable(context, offset)              ((context)->FP + (offset))
#define __Register(context, regno)                    ((context)->target.GPR[regno])

extern int   __FindExceptionTable(ExceptionInfo* info, char* retaddr);
extern char* __SkipUnwindInfo(char* exceptionrecord);

extern char* __PopStackFrame(ThrowContext* context, ExceptionInfo* info);
extern void  __SetupFrameInfo(ThrowContext* context, ExceptionInfo* info);
extern void  __TransferControl(ThrowContext* context, ExceptionInfo* info, char* address);
void         __ThrowHandler(ThrowContext* context);

#endif // MSL_RUNTIME_EXCEPTIONHANDLER_H