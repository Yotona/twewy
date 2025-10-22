#include <NitroSDK/fs.h>
#include <bios.h>

FS_Record Rom_DefaultRecord;

CartridgeRegion Arm7OverlayTable;
CartridgeRegion Arm9OverlayTable;

static u32 data_0207fca0;

s32 data_0207fc9c;

static void func_0203eacc(FS_Record* record) {
    FS_RecordNotifyEnd(record, func_02043408() ? 5 : 0);
}

static FS_CommandResult func_0203eaf0(FS_Record* record, void* dest, u32 src, u32 size) {
    func_0204303c(data_0207fca0, src, dest, size, func_0203eacc, record, TRUE);

    return FS_RESULT_ASYNC;
}

static FS_CommandResult func_0203eb38(FS_Record* record, const void* src, u32 dst, u32 len) {
    return FS_RESULT_FAILURE;
}

static FS_CommandResult func_0203eb40(FS_File* p_file, u32 cmd) {
    switch (cmd) {
        case 9:
            func_02042344((u16)data_0207fc9c);
            return FS_RESULT_SUCCESS;
        case 10:
            func_02042360((u16)data_0207fc9c);
            return FS_RESULT_SUCCESS;
        case 1:
            return FS_RESULT_BADCMD;
        default:
            return FS_RESULT_UNKNOWN;
    }
}

static FS_CommandResult func_0203ebac(FS_Record* record, void* dst, u32 src, u32 len) {
    return FS_RESULT_FAILURE;
}

static FS_CommandResult func_0203ebb4(FS_File* file, u32 cmd) {
    return FS_RESULT_BADCMD;
}

void FS_RomInit(u32 param_1) {
    data_0207fca0           = param_1;
    data_0207fc9c           = func_02038968();
    Arm9OverlayTable.offset = 0;
    Arm9OverlayTable.size   = 0;
    Arm7OverlayTable.offset = 0;
    Arm7OverlayTable.size   = 0;

    func_02043134();
    FS_RecordInit(&Rom_DefaultRecord);
    FS_RecordRegister(&Rom_DefaultRecord, "rom", 3);

    if (BIOS_BOOT_MODE == 2) {
        Arm9OverlayTable.offset = -1;
        Arm9OverlayTable.size   = 0;
        Arm7OverlayTable.offset = -1;
        Arm7OverlayTable.size   = 0;

        FS_RecordSetMethod(&Rom_DefaultRecord, func_0203ebb4, -1);
        FS_RecordLoad(&Rom_DefaultRecord, 0, 0, 0, 0, 0, func_0203ebac, func_0203eb38);
    } else {
        CartridgeRegion* const fnt = Cart_GetFileNameTable();
        CartridgeRegion* const fat = Cart_GetFileAllocTable();

        FS_RecordSetMethod(&Rom_DefaultRecord, func_0203eb40, 0x602);

        if (fnt->offset != -1 && fnt->offset != 0 && fat->offset != -1 && fat->offset != 0) {
            FS_RecordLoad(&Rom_DefaultRecord, 0, fat->offset, fat->size, fnt->offset, fnt->size, func_0203eaf0, func_0203eb38);
        }
    }
}

u32 FS_RomLoadDefaultTables(void* buf, u32 size) {
    return FS_RecordLoadTable(&Rom_DefaultRecord, buf, size);
}
