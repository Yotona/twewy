#include <CriWare/cri_cvfs.h>
#include <MSL/Common/string.h>

#define CVFS_DEVICE_MAX      32
#define CVFS_HANDLE_MAX      10
#define CVFS_MAX_NAME_LENGTH 297

typedef struct {
    CVFSDevice* device;
    char        name[12];
} CVFSNamedDevice;

char  data_0207063c[300]               = "";
s32   data_020703e0                    = 0;
char  def_dev[12]                      = "";
void* cvfs_errobj                      = NULL;
void (*cvfs_errfn)(void*, const char*) = NULL;

CVFSHandle      cvfs_handles[CVFS_HANDLE_MAX]       = {0};
CVFSNamedDevice cvfs_named_devices[CVFS_DEVICE_MAX] = {0};

CVFSDevice* func_0201d540();
CVFSDevice* func_0201bfa0();

CVFSDevice* addDevice(const char* device_name, CVFSDevice* (*device_provider)());
BOOL        isExistDev(const char* devName, s32 device_name_len);
CVFSDevice* getDevice(const char* device_name);
void        toUpperStr(char* str);
CVFSHandle* cvFsOpen(const char* fname, void* arg1, s32 arg2);
void        releaseCvFsHn(CVFSHandle* handle);
CVFSHandle* allocCvFsHn(void);
void        getDevName(char* filename, char* device_name, const char* full_path);
void        getDefDev(char* arg0);
void        addDevName(const char* device_name, char* out);

// Nonmatching: Register diffs
static void cvFsCallUsrErrFn(void* object, const char* error, s32 arg2) {
    if (cvfs_errfn != NULL) {
        cvfs_errfn(cvfs_errobj, error);
    }
}

static void cvFsError(const char* error) {
    cvFsCallUsrErrFn(&cvfs_errobj, error, 0);
}

static void cvFsAddDev(const char* device_name, CVFSDevice* (*device_provider)(), s32 arg2) {
    CVFSDevice* device;

    if (device_name == NULL) {
        cvFsError("cvFsAddDev #1:illegal device name");
        return;
    }

    if (device_provider == NULL) {
        cvFsError("cvFsAddDev #2:illegal I/F func name");
        return;
    }

    device = addDevice(device_name, device_provider);

    if (device == NULL) {
        cvFsError("cvFsAddDev #3:can not add device");
        return;
    }

    if (device->EntryErrFunc != NULL) {
        device->EntryErrFunc(cvFsCallUsrErrFn, NULL);
    }
}

CVFSDevice* addDevice(const char* device_name, CVFSDevice* (*device_provider)()) {
    s32         i;
    CVFSDevice* device;

    toUpperStr(device_name);
    device = device_provider();

    if (getDevice(device_name) != NULL) {
        return device;
    }

    for (i = 0; i < CVFS_DEVICE_MAX; i++) {
        if (cvfs_named_devices[i].name[0] == '\0') {
            break;
        }
    }

    if (i == CVFS_DEVICE_MAX) {
        return NULL;
    }

    cvfs_named_devices[i].device = device;
    memcpy(cvfs_named_devices[i].name, device_name, strlen(device_name) + 1);
    return device;
}

CVFSDevice* getDevice(const char* name) {
    u32 len = strlen(name);

    for (u32 i = 0; i < CVFS_DEVICE_MAX; ++i) {
        const char* registered = cvfs_named_devices[i].name;

        if (strncmp(name, registered, len) == 0) {
            return cvfs_named_devices[i].device;
        }
    }

    return NULL;
}

void toUpperStr(char* str) {
    size_t len = strlen(str);

    for (s32 i = 0; i < len + 1; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= ('a' - 'A');
        }
    }
}

void cvFsSetDefDev(const char* devName) {
    if (devName == NULL) {
        cvFsError("cvFsSetDefDev #1:illegal device name");
        return;
    }

    u32 device_name_len = strlen(devName);

    if (device_name_len == 0) {
        def_dev[0] = 0;
        return;
    }

    toUpperStr(devName);

    if (isExistDev(devName, device_name_len) == TRUE) {
        memcpy(&def_dev, devName, device_name_len + 1);
    } else {
        cvFsError("cvFsSetDefDev #2:unknown device name");
    }
}

BOOL isExistDev(const char* devName, s32 device_name_len) {
    for (s32 i = 0; i < CVFS_DEVICE_MAX; i++) {
        if (strncmp(devName, cvfs_named_devices[i].name, device_name_len) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

char* cvFsGetDefDev(void) {
    return def_dev;
}

CVFSDevice* variousProc(char* filename, char* device_name, const char* full_path) {
    CVFSDevice* device;

    if (filename[0] == '\0') {
        getDefDev(filename);

        if (filename[0] == '\0') {
            return NULL;
        }
    }

    addDevName(filename, device_name);
    device = getDevice(filename);

    if (device == NULL) {
        getDefDev(filename);
        device = getDevice(filename);

        if (device == NULL) {
            return NULL;
        }

        func_02021614(device_name, CVFS_MAX_NAME_LENGTH, full_path);
    }

    return device;
}

CVFSHandle* cvFsOpen(const char* fname, void* dir, s32 arg2) {
    char filename[CVFS_MAX_NAME_LENGTH + 1];
    char device_name[CVFS_MAX_NAME_LENGTH + 1];

    if (fname == NULL) {
        cvFsError("cvFsOpen #1:illegal file name");
        return NULL;
    }

    getDevName(filename, device_name, fname);

    if (device_name[0] == '\0') {
        cvFsError("cvFsOpen #1:illegal file name");
        return NULL;
    }

    CVFSHandle* fs_hn = allocCvFsHn();

    if (fs_hn == NULL) {
        cvFsError("cvFsOpen #3:can not allocate handle");
        return NULL;
    }

    CVFSDevice* device = variousProc(filename, device_name, fname);
    fs_hn->device      = device;

    if (device == NULL) {
        releaseCvFsHn(fs_hn);
        cvFsError("cvFsOpen #4:device not found");
        return NULL;
    }

    if (device->Open != NULL) {
        fs_hn->fd = device->Open(device_name, dir, arg2);
    } else {
        releaseCvFsHn(fs_hn);
        cvFsError("cvFsOpen #5:vtbl error");
        return NULL;
    }

    if (fs_hn->fd == NULL) {
        releaseCvFsHn(fs_hn);
        cvFsError("cvFsOpen #6:can not open file");
        return NULL;
    }

    return fs_hn;
}

CVFSHandle* allocCvFsHn(void) {
    s32 i;

    for (i = 0; i < CVFS_HANDLE_MAX; i++) {
        if (cvfs_handles[i].fd == NULL) {
            break;
        }
    }

    if (i == CVFS_HANDLE_MAX) {
        return NULL;
    }

    return &cvfs_handles[i];
}

void releaseCvFsHn(CVFSHandle* handle) {
    handle->fd     = NULL;
    handle->device = NULL;
}

void getDevName(char* filename, char* device_name, const char* full_path) {
    if (full_path == NULL) {
        return;
    }

    s32 i;
    for (i = 0; i < CVFS_MAX_NAME_LENGTH; i++) {
        if (full_path[i] == ':' || full_path[i] == '\0') {
            break;
        }
        filename[i] = full_path[i];
    }

    if (full_path[i] == '\0') {
        filename[i] = '\0';
        memcpy(device_name, filename, strlen(filename) + 1);
        filename[0] = '\0';
        return;
    }

    filename[i] = '\0';
    i += 1;

    if (i == 2) {
        filename[0] = '\0';
        i           = 0;
    }

    s32 j = i;

    for (j = i; j < CVFS_MAX_NAME_LENGTH; j++) {
        if (full_path[j] == '\0') {
            break;
        }
        device_name[j - i] = full_path[j];
    }

    device_name[j - i] = '\0';
    toUpperStr(filename);
}

void getDefDev(char* arg0) {
    u32 len = strlen(def_dev);

    if (def_dev[0] == '\0') {
        arg0[0] = '\0';
    } else {
        memcpy(arg0, def_dev, len + 1);
    }
}

void cvFsClose(CVFSHandle* handle) {
    if (handle == NULL) {
        cvFsError("cvFsClose #1:handle error");
        return;
    }
    if (handle->device->Close != NULL) {
        handle->device->Close(handle->fd);
        releaseCvFsHn(handle);
        return;
    }
    cvFsError("cvFsClose #2:vtbl error");
}

s32 cvFsTell(CVFSHandle* hndl) {
    s32 result;

    if (hndl == NULL) {
        cvFsError("cvFsTell #1:handle error");
        return 0;
    }
    if (hndl->device->Tell != NULL) {
        result = hndl->device->Tell(hndl->fd);
    } else {
        result = 0;
        cvFsError("cvFsTell #2:vtbl error");
    }
    return result;
}

s32 cvFsSeek(CVFSHandle* handle, s32 offset, s32 whence) {
    s32 result;

    if (handle == NULL) {
        cvFsError("cvFsSeek #1:handle error");
        return 0;
    }
    if (handle->device->Seek != NULL) {
        result = handle->device->Seek(handle->fd, offset, whence);
    } else {
        result = 0;
        cvFsError("cvFsSeek #2:vtbl error");
    }
    return result;
}

s32 cvFsReqRd(CVFSHandle* fs_handle, s32 len, void* buf) {
    s32 ret;

    if (fs_handle == NULL) {
        cvFsError("cvFsReqRd #1:handle error");
        return 0;
    }

    if (fs_handle->device->ReqRd != NULL) {
        ret = fs_handle->device->ReqRd(fs_handle->fd, len, buf);
    } else {
        ret = 0;
        cvFsError("cvFsReqRd #2:vtbl error");
    }

    return ret;
}

void cvFsExecServer(void) {
    CVFSDevice* device;
    s32         i;

    for (i = 0; i < CVFS_DEVICE_MAX; i++) {
        device = cvfs_named_devices[i].device;

        if (device != NULL && device->ExecServer != NULL) {
            device->ExecServer();
        }
    }
}

s32 cvFsGetStat(CVFSHandle* fs_handle) {
    s32 stat = 3;

    if (fs_handle == NULL) {
        cvFsError("cvFsGetStat #1:handle error");
        return 3;
    }

    if (fs_handle->device->GetStat != NULL) {
        stat = fs_handle->device->GetStat(fs_handle->fd);
    } else {
        cvFsError("cvFsGetStat #2:vtbl error");
    }

    return stat;
}

s32 cvFsGetFileSize(const char* full_path) {
    char filename[CVFS_MAX_NAME_LENGTH];
    char device_name[CVFS_MAX_NAME_LENGTH];

    if (full_path == NULL) {
        cvFsError("cvFsGetFileSize #1:illegal file name");
        return 0;
    }

    getDevName(filename, device_name, full_path);

    if (device_name[0] == '\0') {
        cvFsError("cvFsGetFileSize #1:illegal file name");
        return 0;
    }

    CVFSDevice* device = variousProc(filename, device_name, full_path);
    if (device == NULL) {
        cvFsError("cvFsGetFileSize #3:device not found");
    }

    if (device->GetFileSize != NULL) {
        return device->GetFileSize(device_name);
    }

    cvFsError("cvFsGetFileSize #4:vtbl error");
    return 0;
}

s32 cvFsGetFileSizeByHndl(CVFSHandle* hndl) {
    if (hndl == NULL) {
        cvFsError("cvFsGetFileSizeByHndl #1:illegal file handle");
        return -1;
    }

    s32 sVar1 = 0x7fffffff;
    if (hndl->device->OptFn1 != NULL) {
        sVar1 = hndl->device->OptFn1(hndl->fd, 300, 0, 0);
    }
    return sVar1;
}

void cvFsEntryErrFunc(void (*function)(void*, const char*), void* object) {
    if (function == NULL) {
        cvfs_errfn  = NULL;
        cvfs_errobj = NULL;
    } else {
        cvfs_errfn  = function;
        cvfs_errobj = object;
    }
}

s32 cvFsGetVolumeInfo(const char* device_name, char* volume_name, s32 param_3, s32 param_4) {
    if (device_name == NULL) {
        cvFsError("cvFsGetVolumeInfo #1:illegal device name");
        return -1;
    }
    if (volume_name == NULL) {
        cvFsError("cvFsGetVolumeInfo #2:illegal volume name");
        return -1;
    }

    CVFSDevice* device = getDevice(device_name);
    if (device == NULL) {
        cvFsError("cvFsGetVolumeInfo #3:device not found");
        return -1;
    }

    // TODO: fake match
    s32 local_20[5] = {0};

    local_20[1] = volume_name;
    local_20[2] = param_3;

    if (device->OptFn1 != NULL) {
        return device->OptFn1(&local_20, 5, 0, 0);
    }
    return -1;
}

void cvFsSetDefVol(char* devName, char* volName) {
    if (devName == NULL) {
        cvFsError("cvFsSetDefVol #1:illegal device name");
        return;
    }
    if (volName == NULL) {
        cvFsError("cvFsSetDefVol #2:illegal volume name");
        return;
    }
    CVFSDevice* device = getDevice(devName);
    if (device == NULL) {
        cvFsError("cvFsSetDefVol #3:device not found");
        return;
    }

    // TODO: fake match
    s32 local_20[5] = {0};

    local_20[1] = volName;

    if (device->OptFn1 != NULL) {
        device->OptFn1(&local_20, 6, 0, 0);
    }
}

s32 isNeedDevName(char* name) {
    CVFSDevice* device = getDevice(name);
    if (device == NULL) {
        return 0;
    }

    if (device->OptFn1 != NULL) {
        return device->OptFn1(0, 100, 0, 0);
    }
    return 0;
}

void addDevName(const char* device_name, char* out) {
    if (device_name == NULL) {
        device_name = cvFsGetDefDev();
    }

    if (isNeedDevName(device_name) == 1) {
        func_02021614(&data_0207063c, CVFS_MAX_NAME_LENGTH, out);
        func_0202165c(out, CVFS_MAX_NAME_LENGTH, "%s:%s", device_name, &data_0207063c);
    }
}

void func_0201b874(void* func, const char* error) {
    ADXERR_CallErrFunc1(error);
}

static char* const data_0205bff8 = "\nADXNITROSDK Ver.";

void func_0201b884(void) {
    func_0201bfac(data_0205bff8);
    cvFsEntryErrFunc(func_0201b874, NULL);
    cvFsAddDev("MFS", func_0201d540, 0);
    cvFsAddDev("NITRO\0\0", func_0201bfa0, 0);
}
