#include <cvFs.h>

struct {
    /* 0x0 */ s32 unk_00;
    /* 0x4 */ void (*unk_04)(s32, const char*);
    /* 0x8 */ s32 unk_08;
} data_020703e0;

s32 data_020703e8;
s8  data_02070768;

void func_0201b874();
void func_0201d540();
void func_0201bfa0();

BOOL isExistDev(char* devName, s32 param_2);

// Nonmatching: Register diffs
void cvFsCallUsrErrFn(char* param_1, const char* error, s32 param_3) {
    if (data_020703e0.unk_04 != NULL) {
        data_020703e0.unk_04(data_020703e0.unk_08, error);
    }
}

void cvFsError(const char* error) {
    cvFsCallUsrErrFn(&data_020703e8, error, 0);
}

void cvFsSetDefDev(char* devName) {
    int iVar1;
    int iVar2;

    if (devName == NULL) {
        cvFsError("cvFsSetDefDev #1:illegal device name");
        return;
    }
    iVar1 = func_0204c814(devName);
    if (iVar1 == 0) {
        data_02070768 = 0;
        return;
    }
    func_0201af08(devName);
    if (isExistDev(devName, iVar1) == TRUE) {
        func_020490f8(&data_02070768, devName, iVar1 + 1);
    } else {
        cvFsError("cvFsSetDefDev #2:unknown device name");
    }
}

// Nonmatching
BOOL isExistDev(char* devName, s32 param_2) {
    /* Not yet implemented */
}

s8* cvFsGetDefDev(void) {
    return &data_02070768;
}

void cvFsClose(CvFileHandle* handle) {
    void (*temp_r1)(CvFileHandle*);

    if (handle == NULL) {
        cvFsError("cvFsClose #1:handle error");
        return;
    }
    if (handle->vtable->close != NULL) {
        handle->vtable->close(handle->unk_04);
        func_0201b21c(handle);
        return;
    }
    cvFsError("cvFsClose #2:vtbl error");
}

s32 cvFsTell(CvFileHandle* hndl) {
    s32 result;

    if (hndl == NULL) {
        cvFsError("cvFsTell #1:handle error");
        return 0;
    }
    if (hndl->vtable->tell != NULL) {
        result = hndl->vtable->tell(hndl->unk_04);
    } else {
        result = 0;
        cvFsError("cvFsTell #2:vtbl error");
    }
    return result;
}

// Nonmatching: Register diffs
s32 cvFsSeek(CvFileHandle* hndl) {
    s32 result;

    if (hndl == NULL) {
        cvFsError("cvFsSeek #1:handle error");
        return 0;
    }
    if (hndl->vtable->seek != NULL) {
        result = hndl->vtable->seek(hndl->unk_04);
    } else {
        result = 0;
        cvFsError("cvFsSeek #2:vtbl error");
    }
    return result;
}

void cvFsSetDefVol() {}

void func_0201b7d8() {}

void func_0201b810() {}

void func_0201b874() {}

static char* const data_0205bff8 = "\nADXNITROSDK Ver.";

void func_0201b884(void) {
    func_0201bfac(data_0205bff8);
    func_0201b640(func_0201b874, 0);
    cvFsAddDev("MFS", func_0201d540, 0);
    cvFsAddDev("NITRO", func_0201bfa0, 0);
}
