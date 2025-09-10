#include <MSL/C++/exception.hpp>
#include <MSL/Runtime/CompressedNumbers.h>
#include <MSL/Runtime/ExceptionHandler.h>

enum {
    SKIP  = 0,
    APPLY = 1
};

typedef void (*DeleteFunc)(void*);

typedef struct ActionIterator {
    ExceptionInfo info;
    ThrowContext  context;
} ActionIterator;

typedef struct ex_catchblock {
    u8*           catch_type;
    unsigned long pcoffset;
    long          cinfo_ref;
} ex_catchblock;

typedef struct ex_activecatchblock {
    long cinfo_ref;
} ex_activecatchblock;

typedef struct ex_specification {
    unsigned long nspecs;
    unsigned long pcoffset;
    long          cinfo_ref;
    char*         specs;
} ex_specification;

inline void DecodeCatchBlock(char* ap, ex_catchblock* cb) {
    ap += sizeof(exaction_type);
    ap = __DecodeAddress(ap, (void**)&cb->catch_type);
    ap = __DecodeUnsignedNumber(ap, &cb->pcoffset);
    ap = __DecodeSignedNumber(ap, &cb->cinfo_ref);
}

inline void DecodeActiveCatchBlock(char* ap, ex_activecatchblock* acb) {
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &acb->cinfo_ref);
}

inline void DecodeSpecification(char* ap, ex_specification* sp) {
    ap += sizeof(exaction_type);
    ap        = __DecodeUnsignedNumber(ap, &sp->nspecs);
    ap        = __DecodeUnsignedNumber(ap, &sp->pcoffset);
    ap        = __DecodeSignedNumber(ap, &sp->cinfo_ref);
    sp->specs = ap;
}

inline void Branch(ExceptionInfo* info, ThrowContext* context) {
    long target;
    __DecodeSignedNumber(info->action_pointer + sizeof(exaction_type), &target);
    info->action_pointer += target;
}

inline void DestroyLocal(int apply, ExceptionInfo* info, ThrowContext* context) {
    char* ap = info->action_pointer;
    long  local;
    void* dtor;

    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &local);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        DTORCALL_COMPLETE(__FunctionPointer(info, context, dtor), __LocalVariable(context, local));
    }

    info->action_pointer = ap;
}

inline void DestroyLocalCond(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*           ap = info->action_pointer;
    int             regcond;
    long            cond, local;
    void*           dtor;
    local_cond_type destroyIt;

    regcond = *(exaction_type*)ap & REGCOND;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &cond);
    ap = __DecodeSignedNumber(ap, &local);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regcond)
            destroyIt = (local_cond_type)__Register(context, cond);
        else
            destroyIt = *(local_cond_type*)__LocalVariable(context, cond);

        if (destroyIt) {
            DTORCALL_COMPLETE(__FunctionPointer(info, context, dtor), __LocalVariable(context, local));
        }
    }

    info->action_pointer = ap;
}

inline void DestroyLocalPointer(int apply, ExceptionInfo* info, ThrowContext* context) {
    char* ap = info->action_pointer;
    int   regptr;
    long  ptr;
    void *dtor, *objectptr;

    regptr = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &ptr);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regptr)
            objectptr = (void*)__Register(context, ptr);
        else
            objectptr = *(void**)__LocalVariable(context, ptr);

        DTORCALL_COMPLETE(__FunctionPointer(info, context, dtor), objectptr);
    }

    info->action_pointer = ap;
}

inline void DestroyLocalArray(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*         ap = info->action_pointer;
    long          array;
    unsigned long nelements, elemsize, i;
    void*         dtor;
    char*         arrayptr;

    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &array);
    ap = __DecodeUnsignedNumber(ap, &nelements);
    ap = __DecodeUnsignedNumber(ap, &elemsize);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        arrayptr = (char*)__LocalVariable(context, array);
        for (arrayptr += nelements * elemsize, i = nelements; i > 0; i--) {
            arrayptr -= elemsize;
            DTORCALL_COMPLETE(__FunctionPointer(info, context, dtor), arrayptr);
        }
    }

    info->action_pointer = ap;
}

inline void DestroyVarLenArray(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*         ap = info->action_pointer;
    int           regptr1, regptr2;
    long          ptr1, ptr2;
    unsigned long nelements, elemsize, i;
    void*         dtor;
    char *        arrayptr, *arraysize;

    regptr1 = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap      = __DecodeSignedNumber(ap, &ptr1);
    regptr2 = *(u8*)ap & REGPTR;
    ap += sizeof(char);
    ap = __DecodeSignedNumber(ap, &ptr2);
    ap = __DecodeUnsignedNumber(ap, &elemsize);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regptr1)
            arrayptr = (char*)__Register(context, ptr1);
        else
            arrayptr = *(char**)__LocalVariable(context, ptr1);

        if (regptr2)
            arraysize = (char*)__Register(context, ptr2);
        else
            arraysize = *(char**)__LocalVariable(context, ptr2);

        nelements = (unsigned long)arraysize / elemsize;
        for (arrayptr += (unsigned long)arraysize, i = nelements; i > 0; i--) {
            arrayptr -= elemsize;
            DTORCALL_COMPLETE(__FunctionPointer(info, context, dtor), arrayptr);
        }
    }

    info->action_pointer = ap;
}

inline void FreeVarLenArray(int apply, ExceptionInfo* info, ThrowContext* context) {
    char* ap = info->action_pointer;
    int   regptr1;
    long  ptr1, spaddend;
    long* arrayptr;

    regptr1 = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &ptr1);

    if (apply) {
        if (regptr1)
            arrayptr = (long*)__Register(context, ptr1);
        else
            arrayptr = *(long**)__LocalVariable(context, ptr1);
    }

    info->action_pointer = ap;
}

inline void DestroyBaseVTTObject(int apply, ExceptionInfo* info, ThrowContext* context) {
    char* ap = info->action_pointer;
    int   regptr;
    long  ptr, offset, vtt_offset;
    void* dtor;
    char *objectptr, *vtt;

    regptr = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &ptr);
    ap = __DecodeSignedNumber(ap, &offset);
    ap = __DecodeAddress(ap, (void**)&vtt);
    ap = __DecodeSignedNumber(ap, &vtt_offset);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regptr)
            objectptr = (char*)__Register(context, ptr);
        else
            objectptr = *(char**)__LocalVariable(context, ptr);

        DTORCALL_VTTOBJECT(__FunctionPointer(info, context, dtor), objectptr + offset, vtt + vtt_offset);
    }

    info->action_pointer = ap;
}

inline void DestroyBaseVTTPointer(int apply, ExceptionInfo* info, ThrowContext* context) {
    char* ap = info->action_pointer;
    int   regptr1, regptr2;
    long  ptr1, ptr2, offset, vtt_offset;
    void* dtor;
    char *objectptr, *vtt;

    regptr1 = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap      = __DecodeSignedNumber(ap, &ptr1);
    ap      = __DecodeSignedNumber(ap, &offset);
    regptr2 = *(u8*)ap & REGPTR;
    ap += sizeof(char);
    ap = __DecodeSignedNumber(ap, &ptr2);
    ap = __DecodeSignedNumber(ap, &vtt_offset);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regptr1)
            objectptr = (char*)__Register(context, ptr1);
        else
            objectptr = *(char**)__LocalVariable(context, ptr1);

        if (regptr2)
            vtt = (char*)__Register(context, ptr2);
        else
            vtt = *(char**)__LocalVariable(context, ptr2);

        DTORCALL_VTTOBJECT(__FunctionPointer(info, context, dtor), objectptr + offset, vtt + vtt_offset);
    }

    info->action_pointer = ap;
}

inline void DestroyBase(int apply, ExceptionInfo* info, ThrowContext* context) {
    char* ap = info->action_pointer;
    int   regptr;
    long  ptr, offset;
    void* dtor;
    char* objectptr;

    regptr = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &ptr);
    ap = __DecodeSignedNumber(ap, &offset);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regptr)
            objectptr = (char*)__Register(context, ptr);
        else
            objectptr = *(char**)__LocalVariable(context, ptr);

        DTORCALL_PARTIAL(__FunctionPointer(info, context, dtor), objectptr + offset);
    }

    info->action_pointer = ap;
}

inline void DestroyMember(int apply, ExceptionInfo* info, ThrowContext* context) {
    char* ap = info->action_pointer;
    int   regptr;
    long  ptr, offset;
    void* dtor;
    char* objectptr;

    regptr = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &ptr);
    ap = __DecodeSignedNumber(ap, &offset);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regptr)
            objectptr = (char*)__Register(context, ptr);
        else
            objectptr = *(char**)__LocalVariable(context, ptr);

        DTORCALL_COMPLETE(__FunctionPointer(info, context, dtor), objectptr + offset);
    }

    info->action_pointer = ap;
}

inline void DestroyMemberCond(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*               ap = info->action_pointer;
    int                 regptr, regcond;
    long                cond, ptr, offset;
    void*               dtor;
    char*               objectptr;
    vbase_ctor_arg_type destroyIt;

    regcond = *(exaction_type*)ap & REGCOND;
    regptr  = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &cond);
    ap = __DecodeSignedNumber(ap, &ptr);
    ap = __DecodeSignedNumber(ap, &offset);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regcond)
            destroyIt = (vbase_ctor_arg_type)__Register(context, cond);
        else
            destroyIt = *(vbase_ctor_arg_type*)__LocalVariable(context, cond);

        if (destroyIt) {
            if (regptr)
                objectptr = (char*)__Register(context, ptr);
            else
                objectptr = *(char**)__LocalVariable(context, ptr);

            DTORCALL_COMPLETE(__FunctionPointer(info, context, dtor), objectptr + offset);
        }
    }

    info->action_pointer = ap;
}

inline void DestroyMemberArray(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*         ap = info->action_pointer;
    int           regptr;
    long          ptr, offset;
    unsigned long nelements, elemsize, i;
    void*         dtor;
    char*         arrayptr;

    regptr = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &ptr);
    ap = __DecodeSignedNumber(ap, &offset);
    ap = __DecodeUnsignedNumber(ap, &nelements);
    ap = __DecodeUnsignedNumber(ap, &elemsize);
    ap = __DecodeAddress(ap, &dtor);

    if (apply) {
        if (regptr)
            arrayptr = (char*)__Register(context, ptr) + offset;
        else
            arrayptr = *(char**)__LocalVariable(context, ptr) + offset;

        for (arrayptr += nelements * elemsize, i = nelements; i > 0; i--) {
            arrayptr -= elemsize;
            DTORCALL_COMPLETE(__FunctionPointer(info, context, dtor), arrayptr);
        }
    }

    info->action_pointer = ap;
}

inline void DeletePointer(int apply, ExceptionInfo* info, ThrowContext* context) {
    char* ap = info->action_pointer;
    long  ptr;
    void* deletefunc;
    char* objectptr;
    int   regptr;

    regptr = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &ptr);
    ap = __DecodeAddress(ap, &deletefunc);

    if (apply) {
        if (regptr)
            objectptr = (char*)__Register(context, ptr);
        else
            objectptr = *(char**)__LocalVariable(context, ptr);

        ((DeleteFunc)__FunctionPointer(info, context, deletefunc))(objectptr);
    }

    info->action_pointer = ap;
}

inline void DeletePointerCond(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*           ap = info->action_pointer;
    long            cond, ptr;
    void*           deletefunc;
    char*           objectptr;
    int             regptr, regcond;
    local_cond_type deleteIt;

    regcond = *(exaction_type*)ap & REGCOND;
    regptr  = *(exaction_type*)ap & REGPTR;
    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &cond);
    ap = __DecodeSignedNumber(ap, &ptr);
    ap = __DecodeAddress(ap, &deletefunc);

    if (apply) {
        if (regcond)
            deleteIt = (local_cond_type)__Register(context, cond);
        else
            deleteIt = *(local_cond_type*)__LocalVariable(context, cond);

        if (deleteIt) {
            if (regptr)
                objectptr = (char*)__Register(context, ptr);
            else
                objectptr = *(char**)__LocalVariable(context, ptr);

            ((DeleteFunc)__FunctionPointer(info, context, deletefunc))(objectptr);
        }
    }

    info->action_pointer = ap;
}

inline void CatchBlock(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*         ap = info->action_pointer;
    void*         catch_type;
    unsigned long pcoffset;
    long          cinfo_ref;

    ap += sizeof(exaction_type);
    ap = __DecodeAddress(ap, &catch_type);
    ap = __DecodeUnsignedNumber(ap, &pcoffset);
    ap = __DecodeSignedNumber(ap, &cinfo_ref);

    if (apply) {
    }

    info->action_pointer = ap;
}

inline void ActiveCatchBlock(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*      ap = info->action_pointer;
    long       cinfo_ref;
    CatchInfo* catchinfo;

    ap += sizeof(exaction_type);
    ap = __DecodeSignedNumber(ap, &cinfo_ref);

    if (apply) {
        catchinfo = (CatchInfo*)__LocalVariable(context, cinfo_ref);
        if (catchinfo->dtor) {
            if (context->location == catchinfo->location) {
                context->dtor = catchinfo->dtor;
            } else
                DTORCALL_COMPLETE(catchinfo->dtor, catchinfo->location);
        }
    }

    info->action_pointer = ap;
}

inline void Specification(int apply, ExceptionInfo* info, ThrowContext* context) {
    char*         ap = info->action_pointer;
    unsigned long nspecs, pcoffset;
    long          cinfo_ref;

    ap += sizeof(exaction_type);
    ap = __DecodeUnsignedNumber(ap, &nspecs);
    ap = __DecodeUnsignedNumber(ap, &pcoffset);
    ap = __DecodeSignedNumber(ap, &cinfo_ref);
    ap += nspecs * sizeof(char*);

    if (apply) {
    }

    info->action_pointer = ap;
}

static ExceptionTableIndex* BinarySearch(ExceptionTableIndex* table, unsigned long tablesize, char* return_address) {
    ExceptionTableIndex* p;
    long                 i, m, n;

    for (i = 0, n = tablesize - 1; i <= n;) {
        p = &table[m = (i + n) >> 1];
        if (return_address < p->function_address)
            n = m - 1;
        else if (return_address > p->function_address + FUNCTION_SIZE(p))
            i = m + 1;
        else
            return p;
    }

    return 0;
}

static void FindExceptionRecord(char* return_address, ExceptionInfo* info) {
    ExceptionTableIndex* exceptiontable;
    char*                rangetable;
    unsigned long        return_offset;
    unsigned long        currentPC;
    unsigned long        deltaPC;
    unsigned long        length;
    unsigned long        actions;

    info->exception_record = 0;
    info->action_pointer   = 0;

    if (!__FindExceptionTable(info, return_address))
        return;

    return_address = __AdjustReturnAddress(info, context, return_address);

    if (!(exceptiontable = BinarySearch(info->exception_table_start, info->exception_table_end - info->exception_table_start,
                                        return_address)))
        return;

    if (DIRECT_STORED(exceptiontable))
        info->exception_record = (char*)&exceptiontable->exception_table;
    else
        info->exception_record = exceptiontable->exception_table;

    info->current_function = exceptiontable->function_address;
    return_offset          = return_address - exceptiontable->function_address;

    rangetable = __SkipUnwindInfo(info->exception_record);

    for (currentPC = 0;;) {
        rangetable = __DecodeUnsignedNumber(rangetable, &deltaPC);
        if (deltaPC == 0)
            return;
        rangetable = __DecodeUnsignedNumber(rangetable, &length);
        rangetable = __DecodeUnsignedNumber(rangetable, &actions);
        currentPC += deltaPC;
        if (return_offset < currentPC)
            return;
        currentPC += length;
        if (return_offset <= currentPC)
            break;
    }

    info->action_pointer = info->exception_record + actions;
}

static exaction_type CurrentAction(const ActionIterator* iter) {
    return (iter->info.action_pointer ? *(exaction_type*)iter->info.action_pointer & EXACTION_MASK : EXACTION_ENDOFLIST);
}

static exaction_type NextAction(ActionIterator* iter) {
    ExceptionInfo* info    = &iter->info;
    ThrowContext*  context = &iter->context;
    exaction_type  action;

    while (true) {
        if (!info->action_pointer || *(exaction_type*)info->action_pointer & EXACTION_ENDBIT) {
            FindExceptionRecord(__PopStackFrame(context, info), info);
            if (!info->exception_record)
                return -1;
            __SetupFrameInfo(context, info);
            if (!info->action_pointer)
                continue;
        } else {
            switch (*(exaction_type*)info->action_pointer & EXACTION_MASK) {
                case EXACTION_DESTROYLOCAL:
                    DestroyLocal(SKIP, info, context);
                    break;
                case EXACTION_DESTROYLOCALCOND:
                    DestroyLocalCond(SKIP, info, context);
                    break;
                case EXACTION_DESTROYLOCALPOINTER:
                    DestroyLocalPointer(SKIP, info, context);
                    break;
                case EXACTION_DESTROYLOCALARRAY:
                    DestroyLocalArray(SKIP, info, context);
                    break;
                case EXACTION_DESTROYBASE:
                    DestroyBase(SKIP, info, context);
                    break;
                case EXACTION_DESTROYMEMBER:
                    DestroyMember(SKIP, info, context);
                    break;
                case EXACTION_DESTROYMEMBERCOND:
                    DestroyMemberCond(SKIP, info, context);
                    break;
                case EXACTION_DESTROYMEMBERARRAY:
                    DestroyMemberArray(SKIP, info, context);
                    break;
                case EXACTION_DELETEPOINTER:
                    DeletePointer(SKIP, info, context);
                    break;
                case EXACTION_DELETEPOINTERCOND:
                    DeletePointerCond(SKIP, info, context);
                    break;
                case EXACTION_CATCHBLOCK:
                    CatchBlock(SKIP, info, context);
                    break;
                case EXACTION_ACTIVECATCHBLOCK:
                    ActiveCatchBlock(SKIP, info, context);
                    break;
                case EXACTION_SPECIFICATION:
                    Specification(SKIP, info, context);
                    break;
                case EXACTION_DESTROYBASEVTTOBJECT:
                    DestroyBaseVTTObject(SKIP, info, context);
                    break;
                case EXACTION_DESTROYBASEVTTPOINTER:
                    DestroyBaseVTTPointer(SKIP, info, context);
                    break;
                case EXACTION_DESTROYVLA:
                    DestroyVarLenArray(SKIP, info, context);
                    break;
                case EXACTION_FREEVLA:
                    FreeVarLenArray(SKIP, info, context);
                    break;
                default:
                    return -1;
            }
        }

        while ((action = *(exaction_type*)info->action_pointer & EXACTION_MASK) == EXACTION_BRANCH)
            Branch(info, context);

        return (action);
    }
}

static void UnwindStack(ThrowContext* context, ExceptionInfo* info, char* catcher) {
    exaction_type action;

#pragma exception_terminate

    while (true) {
        if (info->action_pointer == NULL) {
            FindExceptionRecord(__PopStackFrame(context, info), info);

            if (!info->exception_record) {
                terminate();
            }
            __SetupFrameInfo(context, info);

            if (info->action_pointer == NULL) {
                continue;
            }
        }

        action = *(exaction_type*)info->action_pointer;

        switch (action & EXACTION_MASK) {
            case EXACTION_BRANCH:
                Branch(info, context);
                break;
            case EXACTION_DESTROYLOCAL:
                DestroyLocal(APPLY, info, context);
                break;
            case EXACTION_DESTROYLOCALCOND:
                DestroyLocalCond(APPLY, info, context);
                break;
            case EXACTION_DESTROYLOCALPOINTER:
                DestroyLocalPointer(APPLY, info, context);
                break;
            case EXACTION_DESTROYLOCALARRAY:
                DestroyLocalArray(APPLY, info, context);
                break;
            case EXACTION_DESTROYBASE:
                DestroyBase(APPLY, info, context);
                break;
            case EXACTION_DESTROYMEMBER:
                DestroyMember(APPLY, info, context);
                break;
            case EXACTION_DESTROYMEMBERCOND:
                DestroyMemberCond(APPLY, info, context);
                break;
            case EXACTION_DESTROYMEMBERARRAY:
                DestroyMemberArray(APPLY, info, context);
                break;
            case EXACTION_DELETEPOINTER:
                DeletePointer(APPLY, info, context);
                break;
            case EXACTION_DELETEPOINTERCOND:
                DeletePointerCond(APPLY, info, context);
                break;
            case EXACTION_CATCHBLOCK:
                if (catcher == info->action_pointer)
                    return; /*	finished unwinding	*/
                CatchBlock(SKIP, info, context);
                break;
            case EXACTION_ACTIVECATCHBLOCK:
                ActiveCatchBlock(APPLY, info, context);
                break;
            case EXACTION_SPECIFICATION:
                if (catcher == info->action_pointer)
                    return; /*	finished unwinding	*/
                Specification(SKIP, info, context);
                break;
            case EXACTION_DESTROYBASEVTTOBJECT:
                DestroyBaseVTTObject(APPLY, info, context);
                break;
            case EXACTION_DESTROYBASEVTTPOINTER:
                DestroyBaseVTTPointer(APPLY, info, context);
                break;
            case EXACTION_DESTROYVLA:
                DestroyVarLenArray(APPLY, info, context);
                break;
            case EXACTION_FREEVLA:
                FreeVarLenArray(APPLY, info, context);
                break;
            default: /*	error	*/
                terminate();
        }

        if (action & EXACTION_ENDBIT) {
            info->action_pointer = 0;
        }
    }
}

static CatchInfo* FindMostRecentException(ThrowContext* context, ExceptionInfo* info) {
    ActionIterator      iter;
    exaction_type       action;
    CatchInfo*          catchinfo;
    ex_activecatchblock acb;

    iter.info    = *info;
    iter.context = *context;
    for (action = CurrentAction(&iter);; action = NextAction(&iter)) {
        switch (action) {
            case EXACTION_ACTIVECATCHBLOCK:
                break;

            case EXACTION_ENDOFLIST:
            case EXACTION_DESTROYLOCAL:
            case EXACTION_DESTROYLOCALCOND:
            case EXACTION_DESTROYLOCALPOINTER:
            case EXACTION_DESTROYLOCALARRAY:
            case EXACTION_DESTROYBASE:
            case EXACTION_DESTROYMEMBER:
            case EXACTION_DESTROYMEMBERCOND:
            case EXACTION_DESTROYMEMBERARRAY:
            case EXACTION_DELETEPOINTER:
            case EXACTION_DELETEPOINTERCOND:
            case EXACTION_CATCHBLOCK:
            case EXACTION_SPECIFICATION:
            case EXACTION_DESTROYBASEVTTOBJECT:
            case EXACTION_DESTROYBASEVTTPOINTER:
            case EXACTION_DESTROYVLA:
                continue;

            case EXACTION_TERMINATE:
            default:
                return NULL;
        }
        break;
    }

    DecodeActiveCatchBlock(iter.info.action_pointer, &acb);
    catchinfo          = (CatchInfo*)__LocalVariable(&iter.context, acb.cinfo_ref);
    context->throwtype = (u8*)catchinfo->typeinfo;
    context->location  = catchinfo->location;
    context->dtor      = 0;
    context->catchinfo = catchinfo;

    return catchinfo;
}

static int IsInSpecification(char* extype, ex_specification* spec) {
    char* specptr = spec->specs;
    char* spectype;
    long  dummy;
    int   i;

    for (i = 0; i < spec->nspecs; i++) {
        __DecodeAddress(specptr, (void**)&spectype);
        if (__throw_catch_compare((u8*)extype, (u8*)spectype, &dummy) != 0) {
            return TRUE;
        }
        specptr += 4;
    }
    return FALSE;
}

void __unexpected(CatchInfo* catchinfo) {
    char*            unexp = (char*)catchinfo->stacktop;
    ex_specification spec;

#pragma exception_magic

    try {
        unexpected();
    } catch (...) {
        DecodeSpecification(unexp, &spec);
        if (IsInSpecification((char*)((CatchInfo*)&__exception_magic)->typeinfo, &spec)) {
            throw;
        }

        if (IsInSpecification("!!std::bad_exception!!", &spec)) {
            throw bad_exception();
        }
    }
    terminate();
}

static void HandleUnexpected(ThrowContext* context, ExceptionInfo* info, ex_specification* spec, char* unexp) {
    CatchInfo* catchinfo;

#pragma exception_terminate

    UnwindStack(context, info, unexp);

    catchinfo           = (CatchInfo*)__LocalVariable(context, spec->cinfo_ref);
    catchinfo->location = context->location;
    catchinfo->typeinfo = context->throwtype;
    catchinfo->dtor     = context->dtor;
    catchinfo->stacktop = unexp;

    __TransferControl(context, info, info->current_function + spec->pcoffset);
}

static char* FindExceptionHandler(ThrowContext* context, ExceptionInfo* info, long* result_offset) {
    ActionIterator   iter;
    exaction_type    action;
    ex_catchblock    handler;
    ex_specification spec;

    iter.info    = *info;
    iter.context = *context;

    for (action = CurrentAction(&iter);; action = NextAction(&iter)) {
        switch (action) {
            case EXACTION_CATCHBLOCK:
                DecodeCatchBlock(iter.info.action_pointer, &handler);
                if (__throw_catch_compare(context->throwtype, handler.catch_type, result_offset) == 0) {
                    continue;
                }
                break;
            case EXACTION_SPECIFICATION:
                DecodeSpecification(iter.info.action_pointer, &spec);
                if (IsInSpecification((char*)context->throwtype, &spec) == FALSE) {
                    HandleUnexpected(context, info, &spec, iter.info.action_pointer);
                }
                continue;
            case EXACTION_ENDOFLIST:
            case EXACTION_DESTROYLOCAL:
            case EXACTION_DESTROYLOCALCOND:
            case EXACTION_DESTROYLOCALPOINTER:
            case EXACTION_DESTROYLOCALARRAY:
            case EXACTION_DESTROYBASE:
            case EXACTION_DESTROYMEMBER:
            case EXACTION_DESTROYMEMBERCOND:
            case EXACTION_DESTROYMEMBERARRAY:
            case EXACTION_DELETEPOINTER:
            case EXACTION_DELETEPOINTERCOND:
            case EXACTION_ACTIVECATCHBLOCK:
            case EXACTION_DESTROYBASEVTTOBJECT:
            case EXACTION_DESTROYBASEVTTPOINTER:
            case EXACTION_DESTROYVLA:
            case EXACTION_FREEVLA:
                continue;
            case EXACTION_TERMINATE:
            default:
                terminate();
                break;
        }
        break;
    }
    return iter.info.action_pointer;
}

static void SetupCatchInfo(ThrowContext* context, long cinfo_ref, long result_offset) {
    CatchInfo* catchinfo = (CatchInfo*)__LocalVariable(context, cinfo_ref);

    catchinfo->location = context->location;
    catchinfo->typeinfo = context->throwtype;
    catchinfo->dtor     = context->dtor;

    if (*context->throwtype == '*') {
        catchinfo->sublocation = &catchinfo->pointercopy;
        catchinfo->pointercopy = *(long*)context->location + result_offset;
    } else {
        catchinfo->sublocation = (char*)context->location + result_offset;
    }
}

void __ThrowHandler(ThrowContext* context) {
    ExceptionInfo info;
    char*         catchblock;
    ex_catchblock handler;
    long          result_offset;

    FindExceptionRecord(context->returnaddr, &info);
    if (info.exception_record == NULL) {
        terminate();
    }

    __SetupFrameInfo(context, &info);

    if (context->throwtype != NULL) {
        context->catchinfo = NULL;
    } else {
        context->catchinfo = FindMostRecentException(context, &info);
        if (context->catchinfo == NULL) {
            terminate();
        }
    }

    catchblock = FindExceptionHandler(context, &info, &result_offset);
    DecodeCatchBlock(catchblock, &handler);
    UnwindStack(context, &info, catchblock);
    SetupCatchInfo(context, handler.cinfo_ref, result_offset);
    __TransferControl(context, &info, info.current_function + handler.pcoffset);
}

void __end_catch(CatchInfo* catchinfo) {
    if (catchinfo->location != NULL && catchinfo->dtor != NULL) {
        DTORCALL_COMPLETE(catchinfo->dtor, catchinfo->location);
    }
}
