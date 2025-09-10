#include <MSL/Runtime/CompressedNumbers.h>
#include <MSL/Runtime/ExceptionHandler.h>

char* __PopStackFrame(ThrowContext* context, ExceptionInfo* info) {
    unsigned long frame_size, flushback_area_size, *GPRs;
    int           i;

    frame_size          = context->target.frame_size;
    flushback_area_size = context->target.has_flushback ? 16 : 0;

    GPRs = (unsigned long*)(context->FP + frame_size - flushback_area_size);
    for (i = 15; i >= 0; i--) {
        if (context->target.saved_GPRs & (1 << i))
            context->target.GPR[i] = *--GPRs;
    }

    context->SP = context->FP + frame_size;

    return (char*)context->target.GPR[14];
}

extern void __SetupFrameInfo(ThrowContext* context, ExceptionInfo* info) {
    char* p     = info->exception_record;
    char  flags = *p++;

    context->target.has_frame_ptr = flags & UNWIND_FRAME_IS_DYNAMIC;
    context->target.has_flushback = flags & UNWIND_FRAME_HAS_FLUSHBACK;
    context->target.is_Thumb      = flags & UNWIND_FRAME_IS_THUMB;

    context->target.saved_GPRs = ((unsigned char)*p++) << 4;
    context->target.saved_GPRs |= (1 << 14);

    p = __DecodeUnsignedNumber(p, &context->target.frame_size);

    if (flags & UNWIND_FRAME_IS_DYNAMIC)
        p = __DecodeUnsignedNumber(p, &context->target.argument_size);

    if (flags & UNWIND_FRAME_IS_DYNAMIC) {
        if (flags & UNWIND_FRAME_IS_THUMB)
            context->FP = (char*)context->target.GPR[7];
        else
            context->FP = (char*)context->target.GPR[11];
    } else
        context->FP = context->SP;
}

int __FindExceptionTable(ExceptionInfo* info, char* retaddr) {
    extern ExceptionTableIndex __exception_table_start__[];
    extern ExceptionTableIndex __exception_table_end__[];

    info->exception_table_start = __exception_table_start__;
    info->exception_table_end   = __exception_table_end__;

    return 1;
}

char* __SkipUnwindInfo(char* p) {
    u8            flags = *p++;
    unsigned long dummy;

    p += 1;
    p = __DecodeUnsignedNumber(p, &dummy);
    if (flags & UNWIND_FRAME_IS_DYNAMIC)
        p = __DecodeUnsignedNumber(p, &dummy);

    return (p);
}

// clang-format off
asm void __TransferControl(register ThrowContext *context, register ExceptionInfo *info, register char *address) {
    ldr		v1,[context,#ThrowContext.target.GPR+16]
    ldr		v2,[context,#ThrowContext.target.GPR+20]
    ldr		v3,[context,#ThrowContext.target.GPR+24]
    ldr		v4,[context,#ThrowContext.target.GPR+28]
    ldr		v5,[context,#ThrowContext.target.GPR+32]
    ldr		v6,[context,#ThrowContext.target.GPR+36]
    ldr		v7,[context,#ThrowContext.target.GPR+40]
    ldr		v8,[context,#ThrowContext.target.GPR+44]
    ldr		sp,[context,#ThrowContext.target.throwSP]
    ldr		ip,[context,#ThrowContext.target.argument_size]
    sub		sp,sp,ip
    bx		address
}

asm void __throw(register char *throwtype, register void *location, register void *dtor) {
    mov		ip,sp
    sub		sp,sp,#sizeof(ThrowContext)
    str		v1,[sp,#ThrowContext.target.GPR+16]
    str		v2,[sp,#ThrowContext.target.GPR+20]
    str		v3,[sp,#ThrowContext.target.GPR+24]
    str		v4,[sp,#ThrowContext.target.GPR+28]
    str		v5,[sp,#ThrowContext.target.GPR+32]
    str		v6,[sp,#ThrowContext.target.GPR+36]
    str		v7,[sp,#ThrowContext.target.GPR+40]
    str		v8,[sp,#ThrowContext.target.GPR+44]
    str		ip,[sp,#ThrowContext.SP]
    str		ip,[sp,#ThrowContext.target.throwSP]
    str		lr,[sp,#ThrowContext.returnaddr]
    str		throwtype,[sp,#ThrowContext.throwtype]
    str		location,[sp,#ThrowContext.location]
    str		dtor,[sp,#ThrowContext.dtor]
    mov		a1,sp
    b		__ThrowHandler
}

asm void __rethrow(void) {
		mov		ip,sp
		sub		sp,sp,#sizeof(ThrowContext)
		str		v1,[sp,#ThrowContext.target.GPR+16]	//	GPR[4]
		str		v2,[sp,#ThrowContext.target.GPR+20]	//	GPR[5]
		str		v3,[sp,#ThrowContext.target.GPR+24]	//	GPR[6]
		str		v4,[sp,#ThrowContext.target.GPR+28]	//	GPR[7]
		str		v5,[sp,#ThrowContext.target.GPR+32]	//	GPR[8]
		str		v6,[sp,#ThrowContext.target.GPR+36]	//	GPR[9]
		str		v7,[sp,#ThrowContext.target.GPR+40]	//	GPR[10]
		str		v8,[sp,#ThrowContext.target.GPR+44]	//	GPR[11]
		str		ip,[sp,#ThrowContext.SP]
		str		ip,[sp,#ThrowContext.target.throwSP]
		str		lr,[sp,#ThrowContext.returnaddr]
		mov		ip,#0
		str		ip,[sp,#ThrowContext.throwtype]
		str		ip,[sp,#ThrowContext.location]
		str		ip,[sp,#ThrowContext.dtor]
		mov		a1,sp
		b		__ThrowHandler
}

// clang-format on