#include <mem.h>
#include <stddef.h>
#include <string.h>

void* rnaerr_obj;
void (*rnaerr_func)(void* obj, const char* msg);
char rnaerr_msg[256];

void RNAERR_Init(void) {
    memset(rnaerr_msg, 0, sizeof(rnaerr_msg));
    rnaerr_func = NULL;
    rnaerr_obj  = NULL;
}

void RNAERR_Finish(void) {
    memset(rnaerr_msg, 0, sizeof(rnaerr_msg));
    rnaerr_func = NULL;
    rnaerr_obj  = NULL;
}

void RNAERR_EntryErrFunc(void (*func)(void* obj, const char* msg), void* obj) {
    rnaerr_func = func;
    rnaerr_obj  = obj;
}

void RNAERR_CallErrFunc(const char* msg) {
    strncpy(rnaerr_msg, msg, 255);
    if (rnaerr_func != NULL) {
        rnaerr_func(rnaerr_obj, rnaerr_msg);
    }
}
