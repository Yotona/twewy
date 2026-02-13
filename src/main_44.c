#include <types.h>

void func_020274ac(void) {}
void func_020274e4(void) {}
void func_02027520(void* var, u32 r1) {}
void func_02027538(void* var, void* var2) {}
void func_02027564(void) {}
void func_020275b8(void) {}
void func_02027608(void) {}
void func_0202766c(u32 r0, void* r1) {}
void func_020276cc(void) {}
void func_020276e4(void) {}
void func_020276f8(void) {}
u32  func_02027760(void* heap) {
    return 0;
}
void func_02027788(void) {}
void func_02027804(void* heap) {
    func_0202766c(func_02027760(heap), heap);
}
u32 func_0202781c(u32 r0, u32 r1, u32 r2, u32 r3) {
    // func_02027788(r0, 0x46524d48, &r0->0x30, r1, r2);
    // r0->0x24 = r0->0x18;
    // r0->0x28 = r0->0x1C;
    // r0->0x2C = r0->0x20;
    return r0;
}
void func_02027864(void) {}
void func_020278c0(void) {}
void func_02027918(void) {}
void func_0202792c(void) {}
u32  func_02027958(u32 a, u32 b, u32 r2) {
    u32 rounded_b = (b + a) & ~3; // Round (a+b) down to 4-byte boundary
    u32 rounded_a = (a + 3) & ~3; // Round (a+3) down to 4-byte boundary

    if ((rounded_a > rounded_b) || ((rounded_b - rounded_a) < 0x30)) {
        return 0;
    }

    return func_0202781c(rounded_a, rounded_b, r2, (rounded_b - rounded_a));
}
void func_02027990(void* heap) {
    func_02027804(heap);
}
void  func_0202799c(void) {}
void* func_020279d0(void* var, s32 var2) {}
void  func_020279fc(void) {}
void  func_02027a4c(void) {}
void  func_02027a9c(void) {}
void  func_02027ac4(void) {}
void  func_02027ad8(void) {}
void  func_02027aec(void) {}
void  func_02027af4(void) {}
void  func_02027afc(void) {}
void  func_02027b04(void) {}
void  func_02027b0c(void) {}
void  func_02027b58(void) {}
void  func_02027c94(void) {}
void  func_02027c9c(void) {}
void  func_02027cb8(void) {}
void  func_02027cd8(void) {}
void  func_02027cf0(void) {}
void  func_02027d04(void) {}
void  func_02027d30(void) {}
void  func_02027d5c(void) {}
void  func_02027d80(void) {}
void  func_02027da4(void) {}
void  func_02027db0(void) {}
void  func_02027dbc(void) {}
void  func_02027dc8(void) {}
void  func_02027dd4(void) {}
void  func_02027de0(void) {}
void  func_02027dec(void) {}
void  func_02027df8(void) {}
void  func_02027e04(void) {}
void  func_02027e10(void) {}
void  func_02027e1c(void) {}
void  func_02027e28(void) {}
void  func_02027e34(void) {}
void  func_02027e60(void) {}
void  func_02027e8c(void) {}
void  func_02027e98(void) {}
void  func_02027ea4(void) {}
void  func_02027eb0(void) {}
void  func_02027ebc(void) {}
void  func_02027ec8(void) {}
void  func_02027ed4(void) {}
void  func_02027ee0(void) {}
void  func_02027eec(void) {}
void  func_02027ef8(void) {}
void  func_02027f04(void) {}
void  func_02027f10(void) {}
void  func_02027f1c(void) {}
void  func_02027f28(void) {}
void  func_02027f34(void) {}
void  func_02027f60(void) {}
void  func_02027f8c(void) {}
void  func_02027f98(void) {}
void  func_02027fa4(void) {}
void  func_02027fd4(void) {}
void  func_02027fec(void) {}
void  func_02028028(void) {}
void  func_02028038(void) {}
void  func_02028048(void) {}
void  func_02028084(void) {}
void  func_020280a8(void) {}
void  func_02028108(void) {}
void  func_02028170(void) {}
void  func_020281c0(void) {}
void  func_02028250(void) {}
void  func_020282a4(void) {}
void  func_0202835c(void) {}
void  func_020283c4(void) {}
void  func_0202843c(void) {}
void  func_02028450(void) {}
void  func_02028458(void) {}
void  func_02028464(void) {}
void  func_02028470(void) {}
void  func_02028484(void) {}
void  func_0202853c(void) {}
void  func_02028548(void) {}
void  func_02028550(void) {}
void  func_020285a0(void) {}
void  func_02028694(void) {}
void  func_0202873c(void) {}
void  func_020287d4(void) {}
void  func_02028880(void) {}
void  func_020288f0(void) {}
void  func_02028a44(void) {}
void  func_02028a80(void) {}
void  func_02028aa8(void) {}
void  func_02028ad0(void) {}
void  func_02028b0c(void) {}
void  func_02028b20(void) {}
void  func_02028b64(void) {}
void  func_02028bbc(void) {}
void  func_02028bf4(void) {}
void  func_02028c2c(void) {}
void  func_02028c7c(void) {}
void  func_02028cec(void) {}
void  func_02028fd4(void) {}
void  func_02029044(void) {}
void  func_0202908c(void) {}
void  func_02029208(void) {}
void  func_02029270(void) {}
void  func_0202934c(void) {}
void  func_02029374(void) {}
void  func_0202939c(void) {}
void  func_02029518(void) {}
void  func_020295e4(void) {}
void  func_02029a3c(void) {}
void  func_02029abc(void) {}
void  func_02029b90(void) {}
void  func_02029bfc(void) {}
void  func_0202a02c(void) {}
void  func_0202a334(void) {}
void  func_0202a698(void) {}
void  func_0202adf4(void) {}
void  func_0202aed0(void) {}
void  func_0202af40(void) {}
void  func_0202b254(void) {}
void  func_0202b5d4(void) {}
void  func_0202b630(void) {}
void  func_0202b648(void) {}
void  func_0202b654(void) {}
void  func_0202b708(void) {}
void  func_0202b7ec(void) {}
void  func_0202b878(void) {}
void  func_0202b8a0(void) {}
void  func_0202bac0(void) {}
void  func_0202bae4(void) {}
void  func_0202baf0(void) {}
void  func_0202bc2c(void) {}
void  func_0202bd4c(void) {}
void  func_0202bd5c(void) {}
void  func_0202bd9c(void) {}
void  func_0202bdd4(void) {}
void  func_0202bde4(void) {}
void  func_0202bdf4(void) {}
void  func_0202be68(void) {}
void  func_0202be84(void) {}
void  func_0202bf00(void) {}
void  func_0202bf3c(void) {}
void  func_0202bfa4(void) {}
void  func_0202c008(void) {}
void  func_0202c158(void) {}
void  func_0202c50c(void) {}
void  func_0202c66c(void) {}
void  func_0202c7c8(void) {}
void  func_0202c9bc(void) {}
void  func_0202cb50(void) {}
void  func_0202cf58(void) {}
void  func_0202d28c(void) {}
void  func_0202d3f8(void) {}
void  func_0202d560(void) {}
void  func_0202d64c(void) {}
void  func_0202d6e4(void) {}
void  func_0202d7b0(void) {}
void  func_0202d8b0(void) {}
void  func_0202d9bc(void) {}
void  func_0202daa0(void) {}
void  func_0202db38(void) {}
void  func_0202db7c(void) {}
void  func_0202dc1c(void) {}
void  func_0202dce4(void) {}
void  func_0202dd34(void) {}
void  func_0202dda8(void) {}
void  func_0202ddf4(void) {}
void  func_0202de2c(void) {}
void  func_0202dea8(void) {}
void  func_0202dee8(void) {}
void  func_0202dfac(void) {}
void  func_0202e0fc(void) {}
void  func_0202e204(void) {}
void  func_0202e2d4(void) {}
void  func_0202e350(void) {}
void  func_0202e398(void) {}
void  func_0202e478(void) {}
void  func_0202e530(void) {}
void  func_0202e574(void) {}
void  func_0202e598(void) {}
void  func_0202e6e0(void) {}
void  func_0202e80c(void) {}
void  func_0202ea28(void) {}
void  func_0202ebf4(void) {}
void  func_0202ecfc(void) {}
void  func_0202ede4(void) {}
void  func_0202ee64(void) {}
void  func_0202eeac(void) {}
void  func_0202efa4(void) {}

void func_0202f07c(void) {}
void func_0202f0cc(void) {}
void func_0202f0f0(void) {}
void func_0202f238(void) {}
void func_0202f38c(void) {}
void func_0202f46c(void) {}
void func_0202f4f0(void) {}
void func_0202f544(void) {}
void func_0202f610(void) {}
void func_0202f6b4(void) {}
void func_0202f6f4(void) {}
void func_0202f718(void) {}

// func_0202f8a0 - NitroSDK sound system update/main loop
void func_0202f8a0(void) {
    /*extern s32 data_020799c0[];
    extern void func_0203bea8(void);
    extern void func_02041544(void*);
    extern void func_0204155c(void*);
    extern void func_0202fa98(void);
    extern void func_02030a78(void);
    extern void func_0202fdc4(void);
    extern void func_0202f958(void);
    extern void func_0202f990(void);
    extern s32 data_020799d0[];
    extern s32 data_020799dc[];

    if (data_020799c0[0xC / 4] != 0) {
        return;
    }

    data_020799c0[0xC / 4] = 1;
    func_0203bea8(data_020799c0);
    data_020799c0[0x10 / 4] = (s32)func_0202f958;
    data_020799c0[0x14 / 4] = 0;
    data_020799c0[0x1C / 4] = (s32)func_0202f990;
    data_020799c0[0x20 / 4] = 0;
    func_02041544(data_020799d0);
    func_0204155c(data_020799dc);
    func_0202fa98();
    func_02030a78();
    func_0202fdc4();
    *(s8*)&data_020799c0[0] = -1;
    data_020799c0[0x4 / 4] = 1;*/
}

// func_0202f928 - Called by SndMgr_Init to cleanup/shutdown
void func_0202f928(void) {
    /*extern BOOL func_0203bfdc(s32);
    extern void func_0202fe78(void);
    extern void func_02030a90(void);
    extern void func_02032478(void);
    extern void func_0203c1ac(s32);

    s32 i = 0;
    while (func_0203bfdc(i) != 0) {
        // Wait for sound to finish
    }
    func_0202fe78();
    func_02030a90();
    func_02032478();
    func_0203c1ac(0);*/
}

void func_0202f958(void) {}
void func_0202f990(void) {}
void func_0202f99c(void) {}
void func_0202f9e4(void) {}
void func_0202fa14(void) {}
void func_0202fa30(void) {}
void func_0202fa78(void) {}
void func_0202fa98(void) {}
void func_0202fab4(void) {}
void func_0202fad4(void) {}
void func_0202faec(void) {}

// func_0202fb80 - Release/stop sequence handle
void func_0202fb80(void* handle) {
    /*extern void func_02027608(void*);

    void* ptr = *(void**)handle;
    func_02027608(ptr);*/
}

// func_0202fb90 - Stop specific sequence by archive and ID
void func_0202fb90(s32 seqArc, s32 se, s32 param3) {
    /*extern s32 data_0207bde4[];
    extern void func_020300d0(void*, s32);

    for (s32 i = 0; i < 16; i++) {
        void* entry = (void*)((u32)data_0207bde4 + i * 0x44);
        u16* pState = (u16*)((u32)entry + 0x34);
        u16* pSeqArc = (u16*)((u32)entry + 0x38);
        u16* pSe = (u16*)((u32)entry + 0x3A);

        if (*pState == 2 && *pSeqArc == seqArc && *pSe == se) {
            func_020300d0(entry, param3);
        }
    }*/
}

void func_0202fbf4(void) {}

// func_0202fc34 - Sound mode control
void func_0202fc34(s32 mode) {
    /*extern s32 data_0207bdd8[];
    extern void* func_020276cc(void*, s32);
    extern void func_0203011c(void*, s32);

    void* node = func_020276cc(data_0207bdd8, 0);
    if (node == NULL) {
        return;
    }

    while (node != NULL) {
        void* next = func_020276cc(data_0207bdd8, node);
        func_0203011c(node, mode);
        node = next;
    }*/
}

// func_0202fc84 - Initialize/setup sequence handle
void func_0202fc84(void* handle) {
    *(s32*)handle = 0;
}

void func_0202fc90(void) {}

// func_0202fcb0 - Check if sequence is playing
s32 func_0202fcb0(s32 seqArc, s32 se) {
    /*extern s32 data_0207bdd8[];
    extern void* func_020276cc(void*, s32);

    s32 count = 0;
    void* node = func_020276cc(data_0207bdd8, 0);

    if (node == NULL) {
        return 0;
    }

    while (node != NULL) {
        u16* pState = (u16*)((u32)node + 0x34);
        u16* pSeqArc = (u16*)((u32)node + 0x38);
        u16* pSe = (u16*)((u32)node + 0x3A);

        if (*pState == 2 && *pSeqArc == seqArc && *pSe == se) {
            count++;
        }

        node = func_020276cc(data_0207bdd8, node);
    }

    return count;*/
}

// func_0202fd10 - Set sequence parameter (fadeout?)
void func_0202fd10(void* handle, u8 value) {
    /*void* ptr = *(void**)handle;
    if (ptr != NULL) {
        *(u8*)((u32)ptr + 0x41) = value;
    }*/
}

// func_0202fd24 - Set sequence volume
void func_0202fd24(void* handle, u8 volume) {
    /*void* ptr = *(void**)handle;
    if (ptr != NULL) {
        *(u8*)((u32)ptr + 0x40) = volume;
    }*/
}

void func_0202fd38(void) {}

// func_0202fd58 - Set sequence pitch
void func_0202fd58(void* handle, s32 channel, s32 pitch) {
    extern void func_0203bb8c(u8, s32, s32);

    void* ptr = *(void**)handle;
    if (ptr != NULL) {
        u8 channelId = *(u8*)((u32)ptr + 0x3C);
        func_0203bb8c(channelId, channel, pitch);
    }
}

// func_0202fd78 - Set sequence pan
void func_0202fd78(void* handle, s32 channel, s32 pan) {
    extern void func_0203bba8(u8, s32, s32);

    void* ptr = *(void**)handle;
    if (ptr != NULL) {
        u8 channelId = *(u8*)((u32)ptr + 0x3C);
        func_0203bba8(channelId, channel, pan);
    }
}

void func_0202fd98(void) {}
void func_0202fdc4(void) {}
void func_0202fe78(void) {}
void func_0202ffdc(void) {}

void func_02030074(void) {}
void func_02030080(void) {}
void func_020300d0(void) {}
void func_0203011c(void) {}
void func_02030144(void) {}
void func_02030198(void) {}
void func_020301e0(void) {}
void func_02030230(void) {}
void func_02030284(void) {}
void func_020302bc(void) {}
void func_02030324(void) {}
void func_020303a8(void) {}
void func_020303f8(void) {}
void func_02030458(void) {}
void func_020304d0(void) {}
void func_02030530(void) {}
void func_02030558(void) {}
void func_020306f8(void) {}
void func_0203074c(void) {}
void func_02030768(void) {}
void func_020307d4(void) {}
void func_02030828(void) {}
void func_02030858(void) {}
void func_020308cc(void) {}
void func_020308fc(void) {}
void func_0203090c(void) {}
void func_020309bc(void) {}
void func_02030a08(void) {}
void func_02030a78(void) {}
void func_02030a90(void) {}
void func_02030b18(void) {}
void func_02030c08(void) {}
void func_02030c68(void) {}

// snd_initSdatFile - Load master SDAT file from filesystem
// Called from SndMgr_Init with "/Sound/sound_data.sdat"
BOOL snd_initSdatFile(void* sdatMgr, const char* path, s32 heapHandle, s32 flags) {
    /*extern BOOL FS_FilePathAsIden(void*, const char*);
    extern void FS_FileInit(void*);
    extern BOOL FS_FileOpenFromIden(void*, void*);
    extern BOOL func_02030d78(void*, s32, s32);
    extern void* data_0207cd70;

    *(s32*)((u32)sdatMgr + 0x8C) = 0;
    *(s32*)((u32)sdatMgr + 0x84) = 0;
    *(s32*)((u32)sdatMgr + 0x88) = 0;

    void* iden = (void*)((u32)sdatMgr + 0x7C);
    if (!FS_FilePathAsIden(iden, path)) {
        return FALSE;
    }

    void* file = (void*)((u32)sdatMgr + 0x34);
    FS_FileInit(file);
    if (!FS_FileOpenFromIden(file, iden)) {
        return FALSE;
    }

    *(s32*)((u32)sdatMgr + 0x30) = 1;
    if (!func_02030d78(sdatMgr, heapHandle, flags)) {
        return FALSE;
    }

    data_0207cd70 = sdatMgr;
    return TRUE;*/
}

void func_02030d78(void) {}
void func_02030f50(void) {}
void func_02030f68(void) {}

// func_02030f78 - Lookup sound data by sequence archive and SE index
// Returns pointer to sound entry structure
void* func_02030f78(s32 seqArc, s32 se) {
    /*extern void* func_02030fbc(s32);
    extern void* func_020312fc(s32);
    extern void* func_02033914(void*, s32);

    void* arc = func_02030fbc(seqArc);
    if (arc == NULL) {
        return NULL;
    }

    void* arcData = *(void**)arc;
    void* seqData = func_020312fc((s32)arcData);
    if (seqData == NULL) {
        return NULL;
    }

    void* entry = func_02033914(seqData, se);
    if (entry == NULL) {
        return NULL;
    }

    return (void*)((u32)entry + 4);*/
}

// func_02030fbc - Get sequence archive info/pointer
void* func_02030fbc(s32 seqArc) {
    extern void* data_0207cd70;

    if (data_0207cd70 == NULL) {
        return NULL;
    }

    void* sdat = *(void**)((u32)data_0207cd70 + 0x8C);
    if (sdat == NULL) {
        return NULL;
    }

    s32 offset = *(s32*)((u32)sdat + 0xC);
    if (offset == 0) {
        return NULL;
    }

    void* table = (void*)((u32)sdat + offset);
    offset      = *(s32*)((u32)table + 0x10);
    if (offset == 0) {
        return NULL;
    }

    void* arcTable = (void*)((u32)table + offset);
    if (seqArc < 0) {
        return NULL;
    }

    s32 count = *(s32*)arcTable;
    if (seqArc >= count) {
        return NULL;
    }

    s32 entryOffset = *(s32*)((u32)arcTable + 4 + seqArc * 4);
    if (entryOffset == 0) {
        return NULL;
    }

    return (void*)((u32)sdat + entryOffset);
}

void func_02031020(void) {}
void func_02031084(void) {}

// func_020310e8 - Lookup stream/voice data (returns structure with unk_04 field)
// Used by SndMgr to get volume info for direct audio
void* func_020310e8(s32 streamId) {
    extern void* data_0207cd70;

    if (data_0207cd70 == NULL) {
        return NULL;
    }

    void* sdat = *(void**)((u32)data_0207cd70 + 0x8C);
    if (sdat == NULL) {
        return NULL;
    }

    s32 offset = *(s32*)((u32)sdat + 0x14);
    if (offset == 0) {
        return NULL;
    }

    void* table = (void*)((u32)sdat + offset);
    if (streamId < 0) {
        return NULL;
    }

    s32 count = *(s32*)table;
    if (streamId >= count) {
        return NULL;
    }

    s32 entryOffset = *(s32*)((u32)table + 4 + streamId * 4);
    if (entryOffset == 0) {
        return NULL;
    }

    return (void*)((u32)sdat + entryOffset);
}

void func_0203114c(void) {}
void func_020311b0(void) {}
void func_02031214(void) {}
void func_0203123c(void) {}
void func_02031264(void) {}
void func_020312dc(void) {}

// func_020312fc - Check if resource is loaded
void* func_020312fc(s32 idx) {
    extern void* data_0207cd70;

    if (data_0207cd70 == NULL) {
        return NULL;
    }

    void* info  = *(void**)((u32)data_0207cd70 + 0x84);
    s32   count = *(s32*)((u32)info + 8);

    if (idx >= count) {
        return NULL;
    }

    void* entry = (void*)((u32)info + idx * 0x10 + 0x14);
    return entry;
}

void func_02031324(void) {}
void func_02031340(void) {}
void func_0203134c(void) {}
void func_02031358(void) {}

// snd_initHeapHandle - Initialize sound data heap
// Called from SndMgr_Init with buffer pointer and size (0x40000)
void* func_02031640(void* var, s32 var1);
void* snd_initHeapHandle(void* buffer, s32 size) {
    u32 end     = (u32)buffer + size;
    u32 aligned = ((u32)buffer + 3) & ~3;

    if (aligned > end) {
        return NULL;
    }

    u32 remaining = end - aligned;
    if (remaining < 0x10) {
        return NULL;
    }

    u32 r4 = func_02027958((aligned + 0x10), (remaining - 0x10), 0);
    if (!r4) {
        return NULL;
    }

    void* heap = func_02031640((void*)aligned, r4);
    if (heap != NULL) {
        return aligned;
    }

    func_02027990(r4);
    return NULL;
}

void func_020313d4(void) {}
void func_020313ec(void) {}
void func_020314a8(void) {}

// func_02031510 - Finalize/commit sound system initialization
s32 func_02031510(s32 handle) {
    /*extern s32 func_020279fc(void*, u16);
    extern BOOL func_02031674(void*);
    extern void func_02027a4c(void*, s32);

    void* heap = *(void**)handle;
    u16 count = *(u16*)((u32)handle + 0xC);

    if (!func_020279fc(heap, count)) {
        return -1;
    }

    if (func_02031674((void*)handle)) {
        return count - 1;
    }

    func_02027a4c(heap, 0);
    return -1;*/
}

// func_0203155c - Unload/release sound resources
void func_0203155c(s32 handle, s32 count) {
    /*extern void func_020276e4(void*, s32);
    extern void func_0202766c(void*, s32);
    extern void func_02027a4c(void*, s32);
    extern BOOL func_02031674(void*);
    extern s32 func_020279fc(void*, u16);
    extern void func_020316b0(void);

    void* heap = *(void**)handle;
    u16 maxCount = *(u16*)((u32)handle + 0xC);
    BOOL needUpdate = FALSE;

    for (s32 i = 0; i < maxCount; i++) {
        void* list = (void*)((u32)handle + 4);
        void* node = func_020276e4(list, 0);

        while (node != NULL) {
            void* callback = *(void**)((u32)node + 0xC);
            if (callback != NULL) {
                void* arg1 = *(void**)((u32)node + 0x8);
                void* arg2 = *(void**)((u32)node + 0x10);
                void* arg3 = *(void**)((u32)node + 0x14);
                void* dataPtr = (void*)((u32)node + 0x20);
                ((void(*)(void*, void*, void*, void*))callback)(dataPtr, arg1, arg2, arg3);
                needUpdate = TRUE;
            }

            node = func_020276e4(list, node);
        }

        func_0202766c(list, list);
    }

    func_02027a4c(heap, count);

    if (needUpdate) {
        func_020316b0();
    }

    func_020279fc(heap, maxCount);
    func_02031674((void*)handle);*/
}

void  func_02031630(void) {}
void* func_02031640(void* var, s32 var1) {
    // func_02027520(&var->0x4, 0xC);
    // var->0x0 = var1;
    // return fun_02031674(var) == 0 ? 0 : 1;
}
void func_02031674(void) {}
void func_020316b0(void) {}

// func_020316d0 - Load sequence archive by index
s32 func_020316d0(s32 seqArc, s32 handle) {
    /*extern s32 func_020317ac(s32, s32, s32, s32, s32);

    return func_020317ac(seqArc, 0, handle, 0, 0) == 0 ? 1 : 0;*/
}

// func_020316fc - Load bank by index
s32 func_020316fc(s32 bank, s32 handle) {
    /*extern s32 func_020317ac(s32, s32, s32, s32, s32);

    return func_020317ac(bank, 0, handle, 1, 0) == 0 ? 1 : 0;*/
}

// func_02031728 - Load with streaming/priority flag
s32 func_02031728(s32 idx, s32 priority, s32 handle) {
    /*extern s32 func_020317ac(s32, s32, s32, s32, s32);

    return func_020317ac(idx, 0, handle, priority, 1) == 0 ? 1 : 0;*/
}

void func_0203174c(void) {}
void func_020317ac(void) {}
void func_020318ec(void) {}
void func_02031978(void) {}
void func_02031a18(void) {}
void func_02031a84(void) {}
void func_02031af0(void) {}
void func_02031b5c(void) {}
void func_02031c64(void) {}
void func_02031cbc(void) {}
void func_02031ce4(void) {}
void func_02031d14(void) {}
void func_02031d44(void) {}
void func_02031d64(void) {}
void func_02031dac(void) {}
void func_02031e74(void) {}

// func_02031f34 - Validate sound data integrity
BOOL func_02031f34(s32 handle) {
    extern void* func_02030fbc(s32);
    extern void* func_020312fc(s32);

    for (s32 i = 0; i < 32; i++) {
        void* arc = func_02030fbc(i);
        if (arc == NULL) {
            continue;
        }

        void* arcData = *(void**)arc;
        if (arcData == NULL) {
            continue;
        }

        void* data = func_020312fc((s32)arcData);
        if (data == NULL) {
            return FALSE;
        }
    }

    return TRUE;
}

// func_02031fd0 - Play sequence by archive and index
BOOL func_02031fd0(void* handle, s32 seqArc, s32 se) {
    /*extern void* func_02030fbc(s32);
    extern void* func_020312fc(s32);
    extern void* func_02033914(void*, s32);
    extern BOOL func_02032050(void*, u8, u16, u8, void*, void*, s32, s32);

    void* arc = func_02030fbc(seqArc);
    if (arc == NULL) {
        return FALSE;
    }

    void* arcData = *(void**)arc;
    void* seqData = func_020312fc((s32)arcData);
    if (seqData == NULL) {
        return FALSE;
    }

    void* entry = func_02033914(seqData, se);
    if (entry == NULL) {
        return FALSE;
    }

    u8 param1 = *(u8*)((u32)entry + 0x9);
    u16 param2 = *(u16*)((u32)entry + 0x4);
    u8 param3 = *(u8*)((u32)entry + 0x8);

    return func_02032050(handle, param1, param2, param3, entry, seqData, seqArc, se);*/
}

void func_02032050(void) {}

// func_02032114 - Initialize direct audio system
void func_02032114(s32 param1, s32 heapHandle) {
    extern s32  data_0207c6a4;
    extern s32  data_0207c6a8[];
    extern void func_02032218(s32);
    extern void func_02027538(void*, void*);

    if (data_0207c6a4 != 0) {
        func_02032218(heapHandle);
        return;
    }

    data_0207c6a4 = 1;
    func_02027520(data_0207c6a8, 0);

    for (s32 i = 0; i < 4; i++) {
        void* entry = (void*)((u32)data_0207c6a8 + i * 0x30);
        func_02027538(data_0207c6a8, entry);
    }
}

void func_02032218(s32 var) {}
// void* func_020322e0(void* var, s32 var2, s32 var3) {}
void func_0203233c(void* var) {}

// func_0203235c - Check if direct audio is playing
BOOL func_0203235c(void* handle, s32 streamId, s32 param3) {
    /*extern void* func_020322e0(void*, s32, s32);
    extern void func_0203233c(void*);

    void* stream = func_020322e0(handle, streamId, param3);
    if (stream == NULL) {
        return FALSE;
    }

    func_0203233c(handle);
    return TRUE;*/
}

// func_02032384 - Stop direct audio playback
void func_02032384(s32 param1) {
    /*extern s32 data_0207c6a8[];
    extern void func_02030198(void*);

    for (s32 i = 0; i < 4; i++) {
        void* entry = (void*)((u32)data_0207c6a8 + i * 0x134);
        s32* pFlags = (s32*)((u32)entry + 0x110);

        if ((*pFlags & 1) != 0) {
            func_02030198(entry);
        }
    }*/
}

// func_020323c8 - Control direct audio stream
void func_020323c8(void* handle, s32 param1, s32 param2) {
    /*extern void func_0203396c(void*, s32);

    void* ptr = *(void**)handle;
    if (ptr == NULL) {
        return;
    }

    s32* pFlags = (s32*)((u32)ptr + 0x110);
    if ((*pFlags & 0x10) != 0) {
        return;
    }

    void* target = (void*)((u32)ptr + 0xE8);
    func_0203396c(target, param1 << 8);*/
}

// func_020323fc - Set direct audio pan
void func_020323fc(void* handle, s32 param1, s32 pan) {
    /*extern void func_02030828(void*, s32);

    void* ptr = *(void**)handle;
    if (ptr == NULL) {
        return;
    }

    func_02030828(ptr, pan);*/
}

// func_02032418 - Reset/initialize direct audio handle
void func_02032418(void* handle) {
    *(s32*)handle = 0;
}

void func_02032424(void) {}

// func_0203243c - Query direct audio state
s32 func_0203243c(void* handle) {
    extern u32 _ll_udiv(u32, u32, u32, u32);

    void* ptr = *(void**)handle;
    if (ptr == NULL) {
        return 0;
    }

    s32 value   = *(s32*)((u32)ptr + 0x160);
    u16 divisor = *(u16*)((u32)ptr + 0xC4);

    return _ll_udiv(value * 1000, 0, divisor, 0);
}

void func_02032478(void) {}
void func_02032588(void) {}
void func_02032614(void) {}
void func_02032640(void) {}
void func_020328b0(void) {}
void func_02032908(void) {}
void func_020329a0(void) {}
void func_02032a04(void) {}
void func_02032a3c(void) {}
void func_02032a60(void) {}
void func_02032ac0(void) {}
void func_02032b30(void) {}
void func_02032b80(void) {}
void func_02032bc0(void) {}
void func_02032bec(void) {}
void func_02032c88(void) {}
void func_02032dc0(void) {}
void func_02032f04(void) {}
void func_0203372c(void) {}
void func_020337ac(void) {}
void func_02033820(void) {}
void func_02033830(void) {}
void func_02033868(void) {}
void func_02033878(void) {}
void func_020338a0(void) {}
void func_020338a4(void) {}
void func_020338c0(void) {}
void func_020338c4(void) {}
void func_02033914(void) {}
void func_02033954(void) {}
void func_0203396c(void) {}
void func_02033994(void) {}
void func_020339c8(void) {}
// void func_020339e0(void) {}
