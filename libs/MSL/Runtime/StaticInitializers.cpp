#include <MSL/Runtime/MWException.h>

// TODO: Hack around linker generated symbol reference
#define __sinit__ ARM9_CTOR_START

typedef void (*StaticInitializer)(void);

extern StaticInitializer __sinit__[];

DestructorChain* __global_destructor_chain;

void __call_static_initializers(void) {
    StaticInitializer s, *p;

    if (__sinit__ == NULL) {
        return;
    }

    for (p = __sinit__; p != NULL && (s = *p); p++) {
        s();
    }
}

void __destroy_global_chain(void) {
    DestructorChain* gdc;

    while ((gdc = __global_destructor_chain) != NULL) {
        __global_destructor_chain = gdc->next;
        DTORCALL_COMPLETE(gdc->dtor, gdc->object);
    }
}
