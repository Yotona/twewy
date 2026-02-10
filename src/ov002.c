#include "DatMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include "types.h"

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u8  pad_18[0x1C - 0x18];
    u32 unk1C;
    u32 unk20;
    u8  pad_24[0x30 - 0x24];
    u32 unk30;
    u8  pad_34[0x38 - 0x34];
    u32 unk38;
    u8  pad_3C[0x40 - 0x3C];
    u32 unk40;
    u32 unk44;
    u8  pad_48[0x4C - 0x48];
    u32 unk4C;
    u32 unk50;
    u8  pad_54[0x60 - 0x54];
    u32 unk60;
    u32 unk64;
    u8  pad_68[0x8C - 0x68];
    u32 unk8C;
    u8  pad_90[0x98 - 0x90];
    u32 unk98;
    u32 unk9C;
    u32 unkA0;
    u8  pad_A4[0xC8 - 0xA4];
    u32 unkC8;
    u8  pad_CC[0xD4 - 0xCC];
    u32 unkD4;
    u32 unkD8;
    u32 unkDC;
    u8  pad_E0[0x104 - 0xE0];
    u32 unk104;
    u8  pad_108[0x110 - 0x108];
    u32 unk110;
    u32 unk114;
    u32 unk118;
    u8  pad_11C[0x140 - 0x11C];
    u32 unk140;
    u8  pad_144[0x14C - 0x144];
    u32 unk14C;
    u32 unk150;
    u8  pad_154[0x224 - 0x154];
    u32 unk224;
    u32 unk228;
    u32 unk22C;
    u32 unk230;
    u32 unk234;
    u32 unk238;
    u32 unk23C;
    u32 unk240;
    u8  pad_244[0x24C - 0x244];
    u32 unk24C;
    u32 unk250;
    u8  pad_254[0x26C - 0x254];
    u32 unk26C;
    u32 unk270;
    u32 unk274;
    u32 unk278;
    u32 unk27C;
    u8  pad_280[0x284 - 0x280];
    u32 unk284;
    u8  pad_288[0x2AC - 0x288];
    u32 unk2AC;
    u8  pad_2B0[0x2B8 - 0x2B0];
    u32 unk2B8;
    u32 unk2BC;
    u32 unk2C0;
    u8  pad_2C4[0x2E8 - 0x2C4];
    u32 unk2E8;
    u8  pad_2EC[0x2F4 - 0x2EC];
    u32 unk2F4;
    u32 unk2F8;
    u32 unk2FC;
    u8  pad_300[0x324 - 0x300];
    u32 unk324;
    u8  pad_328[0x330 - 0x328];
    u32 unk330;
    u32 unk334;
    u32 unk338;
    u8  pad_33C[0x360 - 0x33C];
    u32 unk360;
    u8  pad_364[0x36C - 0x364];
    u32 unk36C;
    u32 unk370;
    u8  pad_374[0x44C - 0x374];
    u32 unk44C;
    u32 unk450;
    u8  pad_454[0x46C - 0x454];
    u32 unk46C;
    u32 unk470;
    u8  pad_474[0x48C - 0x474];
    u32 unk48C;
    u32 unk490;
    u32 unk494;
    u32 unk498;
    u32 unk49C;
    u8  pad_4A0[0x4A4 - 0x4A0];
} Ov002_DisplaySettingsOverlay;

#define DISPLAY_SETTINGS_VAR g_DisplaySettings
#define g_DisplaySettings    (*((Ov002_DisplaySettingsOverlay*)&DISPLAY_SETTINGS_VAR))

#define OVMGR_U8(base, off)  (*(u8*)((u8*)(base) + (off)))
#define OVMGR_U16(base, off) (*(u16*)((u8*)(base) + (off)))
#define OVMGR_S16(base, off) (*(s16*)((u8*)(base) + (off)))
#define OVMGR_U32(base, off) (*(u32*)((u8*)(base) + (off)))
#define OVMGR_S32(base, off) (*(s32*)((u8*)(base) + (off)))
#define OVMGR_PTR(base, off) (*(void**)((u8*)(base) + (off)))
#define OVPK_DATA(entry)     (*(u8**)((u8*)(entry) + 0x8))
#define OVPK_HDR(entry)      (OVPK_DATA(entry) + 0x20)
#define OVPK_OFF(hdr, off)   (*(u32*)((u8*)(hdr) + (off)))

#define SPRITE_ANIM_FLAGS_1280                                                                                 \
    .bits_0_1 = 0, .dataType = 0, .bit_6 = 0, .bits_7_9 = 5, .bits_10_11 = 0, .bits_12_13 = 1, .bits_14_15 = 0

#define SPRITE_ANIM_FLAGS_1281                                                                                 \
    .bits_0_1 = 1, .dataType = 0, .bit_6 = 0, .bits_7_9 = 5, .bits_10_11 = 0, .bits_12_13 = 1, .bits_14_15 = 0

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
} Ov002_U16_5;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
    };
    u16 data[3];
} Ov002_U16_3;

typedef struct {
    u16 unk0;
    u16 unk2;
} Ov002_U16_2;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
    };
    u16 data[4];
} Ov002_U16_4;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
        u16 unk8;
        u16 unkA;
    };
    u16 data[6];
} Ov002_U16_6;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
        u16 unk8;
        u16 unkA;
        u16 unkC;
        u16 unkE;
    };
    u16 data[8];
} Ov002_U16_8x;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
        u16 unk8;
        u16 unkA;
        u16 unkC;
        u16 unkE;
        u16 unk10;
    };
    u16 data[9];
} Ov002_U16_9;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
        u16 unk8;
        u16 unkA;
        u16 unkC;
        u16 unkE;
        u16 unk10;
        u16 unk12;
    };
    u16 data[10];
} Ov002_U16_10;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
        u16 unk8;
        u16 unkA;
        u16 unkC;
        u16 unkE;
        u16 unk10;
        u16 unk12;
        u16 unk14;
        u16 unk16;
    };
    u16 data[12];
} Ov002_U16_12;

typedef union {
    struct {
        u16 unk0;
        u16 unk2;
        u16 unk4;
        u16 unk6;
        u16 unk8;
        u16 unkA;
        u16 unkC;
        u16 unkE;
        u16 unk10;
        u16 unk12;
        u16 unk14;
        u16 unk16;
        u16 unk18;
        u16 unk1A;
        u16 unk1C;
        u16 unk1E;
        u16 unk20;
        u16 unk22;
        u16 unk24;
        u16 unk26;
        u16 unk28;
        u16 unk2A;
        u16 unk2C;
        u16 unk2E;
        u16 unk30;
        u16 unk32;
        u16 unk34;
        u16 unk36;
        u16 unk38;
        u16 unk3A;
        u16 unk3C;
        u16 unk3E;
        u16 unk40;
        u16 unk42;
        u16 unk44;
        u16 unk46;
        u16 unk48;
        u16 unk4A;
        u16 unk4C;
        u16 unk4E;
        u16 unk50;
        u16 unk52;
        u16 unk54;
        u16 unk56;
        u16 unk58;
        u16 unk5A;
    };
    u16 data[46];
} Ov002_U16_46;

typedef struct {
    u16 data[20];
} Ov002_U16_20;

typedef struct {
    BinIdentifier binIden;
    u16           unk8[4];
    u16           unk10[8];
} Ov002_BinPack;

typedef struct {
    u16   unk0;
    u16   unk2;
    u16   unk4;
    u16   unk6;
    u16   unk8;
    u16   unkA;
    void* unkC;
} Ov002_Config91cc4;

typedef struct {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
} Ov002_ResTask;

typedef struct {
    u8            filler320[0x320];
    void*         unk320;
    void*         unk324;
    void*         unk328;
    void*         unk32C;
    void*         unk330;
    void*         unk334;
    void*         unk338;
    void*         unk33C;
    void*         unk340;
    void*         unk344;
    Ov002_ResTask unk348;
    Ov002_ResTask unk370;
    Ov002_ResTask unk398;
    Ov002_ResTask unk3C0;
    Ov002_ResTask unk3E8;
    Ov002_ResTask unk410;
    Ov002_ResTask unk438;
    Ov002_ResTask unk460;
    void*         unk488;
    void*         unk48C;
    void*         unk490;
    void*         unk494;
    void*         unk498;
    void*         unk49C;
    void*         unk4A0;
    void*         unk4A4;
    void*         unk4A8;
    void*         unk4AC;
    void*         unk4B0;
    void*         unk4B4;
    void*         unk4B8;
    void*         unk4BC;
    void*         unk4C0;
} Ov002_ResBlock;

typedef struct Ov002_Manager {
    void*  taskPool;
    void*  menuMgr;
    void*  subMgr;
    u8     unk41FE9;
    u32    unk4180C;
    u16    unk41834;
    u16    unk41836;
    u16    unk41950;
    u16    unk41954;
    u16    unk41958;
    u16    unk4195A;
    u16    unk41962;
    u16    unk4196A;
    u16    unk4196C;
    u16    unk4198A;
    u16    unk4198C;
    u16    unk41990;
    u16    unk41994;
    u32    unk41FF0;
    void*  unk45FF4;
    s16    unk46070;
    s32    state;
    Sprite sprite;
    s32    spriteActive;
    s32    unk460BC;
    u16    unk46084;
    u16    unk46086;
    u16    unk46090;
    u16    unk46094;
    u16    unk460CC;
    u16    unk460CE;
    s32    unk462E8;
    u32    unk462EC;
    void*  unk462F0;
    void*  unk462F4;
    void*  unk462F8;
    void*  unk462FC;
    u32    unk46300;
    void*  unk46314;
    void*  unk46318;
    void*  unk4631C;
} Ov002_Manager;

typedef struct {
    char filler[0xB4];
    u16  unkB4;
} Data_02073710;
extern Data_02073710 data_02073710;

typedef struct {
    u8 filler40A[0x40A];
    u8 unk40A;
    u8 unk40B;
    u8 unk40C;
    u8 filler40D[0x41C - 0x40D];
    u8 unk41C;
} Data_02074d10;

typedef struct {
    u16 unk0;
    u16 unk2;
} Ov002_SysControlOverlay;

typedef struct {
    u8  filler14[0x14];
    u16 unk14;
} Data_02075110;

typedef struct {
    u8  fillerD84[0xD84];
    u8  unkD84;
    u8  fillerD85[0xD88 - 0xD85];
    u16 unkD88;
} Data_02072d10;

typedef struct {
    u8  filler3414[0x3414];
    u16 unk3414;
} Data_02071d10;

typedef struct BgResMgr        BgResMgr;
extern Data_02074d10           data_02074d10;
extern u8                      data_020750fc[];
extern u8                      data_02075102[];
extern u32                     SystemStatusFlags;
extern Ov002_SysControlOverlay SysControl;
extern Data_02075110           data_02075110;
extern u16                     data_02075124[];
extern Data_02072d10           data_02072d10;
extern Data_02071d10           data_02071d10;
extern u8                      data_020676ec[];
extern u8                      data_0206770c[0x400];
extern u8                      data_02068778[];
extern u8                      data_02068798[0x400];
extern u16                     data_02066aec[0x200];
extern u16                     data_02066cec[0x200];
extern u16                     data_02066eec[0x200];
extern u16                     data_020670ec[0x200];
typedef struct {
    void* unk0;
    void* unk4;
} Data_0206b3cc;

extern Data_0206b3cc data_0206b3cc;
extern struct {
    BgResMgr* unk0;
    BgResMgr* unk4;
} g_BgResourceManagers;

extern SpriteFrameInfo data_0206b408;
static u8              data_ov002_020934fa[0xC6];

static SpriteFrameInfo* Ov002_GetSpriteFrameInfo(Sprite* sprite) {
    s16              frame_index;
    SpriteFrameData* frame_data;

    data_0206b408.unk_04 = 0;
    data_0206b408.unk_08 = 0;
    data_0206b408.unk_0C = 0;
    data_0206b408.unk_10 = -1;
    if ((sprite->animData != NULL) && (sprite->unk1C != NULL) && (sprite->unk16 >= 0)) {
        frame_index          = sprite->unk16;
        frame_data           = sprite->unk1C;
        data_0206b408.unk_04 = frame_data[frame_index].unk_02;
        data_0206b408.unk_08 = (s32)((u16*)frame_data + frame_data[frame_index].unk_00);
    }
    return &data_0206b408;
}

static u8* Ov002_GetPackBase(void* pack) {
    if (pack == NULL) {
        return NULL;
    }
    return *(u8**)((u8*)pack + 8) + 0x20;
}

static u8* Ov002_GetPackOffset(void* pack, u32 offset) {
    u8* base = Ov002_GetPackBase(pack);

    if (base == NULL) {
        return NULL;
    }
    return base + *(u32*)(base + offset);
}

static u8* Ov002_GetPackTableEntry(void* pack, s32 index) {
    u8* base = Ov002_GetPackBase(pack);

    if (base == NULL || index <= 0) {
        return NULL;
    }
    return base + *(u32*)(base + (u32)index * 8);
}

static Ov002_ResBlock* Ov002_GetResBlock(void* mgr) {
    return (Ov002_ResBlock*)((u8*)mgr + 0x47000);
}

static void Ov002_ApplyDisplaySlotFlags(const Ov002_ResTask* task) {
    u8* base  = (u8*)&g_DisplaySettings + 0x64;
    u8* list  = base + task->unk00 * 0x220;
    u32 state = *(u32*)(list + task->unk04 * 0x3C);

    if (state >= 1 && state <= 5) {
        *(u32*)(list + task->unk04 * 0x2C + 0x124) = 1;
    }
    *(u32*)(list + task->unk04 * 8 + 0xF0) = 0x200000;
    *(u32*)(list + task->unk04 * 8 + 0xF4) = 0x200000;
}

static SpriteFrameInfo* func_ov002_020824ac(Sprite* sprite, s32 arg1, s32 mode);
static s32              func_ov002_0208266c(void* arg0, void* arg1);
static s32              func_ov002_02082698(void* arg0, void* arg1);
static s32              func_ov002_0208275c(void* arg0, void* arg1);
static s32              func_ov002_02082878(void* arg0);
static s32              func_ov002_020828a0(void* arg0);
static s32              func_ov002_020828c8(void* arg0);
static s32              func_ov002_020828ec(void* arg0);
static s32              func_ov002_02082914(void* arg0);
static s32              func_ov002_0208293c(void* arg0);
static s32              func_ov002_02082960(void* arg0);
static s32              func_ov002_02082988(void* arg0);
static s32              func_ov002_020829ac(void* arg0);
static s32              func_ov002_020829d4(void* arg0);
static s32              func_ov002_020829f8(void* arg0);
static s32              func_ov002_02082a20(void* arg0);
static void             func_ov002_02082a44(void);
static s8               func_ov002_02082bec(void* arg0);
static void             func_ov002_02082d44(void* arg0);
static void             func_ov002_02085710(void* arg0);
static void             func_ov002_02087eb4(void* arg0, s32 arg1);
static void             func_ov002_0208b860(void);
static void             func_ov002_0208bd40(void);
static void             func_ov002_0208c228(void);
static void             func_ov002_0208ff0c(void* arg0);
static void             func_ov002_02085e20(void* arg0);
static void             func_ov002_02085e8c(void* arg0);
static void             func_ov002_02085f04(void* arg0);
static void             func_ov002_02085fd4(void* arg0);
static void             func_ov002_02086050(void* arg0);
static void             func_ov002_020860ec(void* arg0);
static void             func_ov002_02086160(void* arg0);
static void             func_ov002_020861c8(void* arg0);
static void             func_ov002_0208622c(void* arg0);
static void             func_ov002_02086290(void* arg0);
static void             func_ov002_020862f4(void* arg0);
static void             func_ov002_02086358(void* arg0);
static void             func_ov002_020863bc(void* arg0);
static void             func_ov002_0208678c(void* arg0);
static void             func_ov002_02086b8c(void* arg1);
static void             func_ov002_02086bac(void);
static void             func_ov002_02086bb0(void);
static void             func_ov002_02086bc4(void* arg1);
static void             func_ov002_02086c5c(void);
static void             func_ov002_02086c60(void);
static s32              func_ov002_02086c64(void* arg1);
static s32              func_ov002_02086c84(void* arg0, void* arg1);
static s32              func_ov002_02086cec(void* arg0, void* arg1);
static s32              func_ov002_02086e4c(void* arg0, void* arg1);
static s32              func_ov002_02086eb4(void* arg1);
static s32              func_ov002_02086ee8(void* arg0, void* arg1);
static s32              func_ov002_020870ac(void* arg0);
static s32              func_ov002_020870c8(void* arg0, void* arg1);
static s32              func_ov002_0208749c(void* arg0, void* arg1);
static s32              func_ov002_02087508(void* arg0);
static s32              func_ov002_020875c8(void* arg0, void* arg1);
static s32              func_ov002_02087728(void* arg0, void* arg1);
static void             func_ov002_02087524(void* arg1);
static void             func_ov002_0208757c(void* arg1);
static void             func_ov002_020875c0(void);
static void             func_ov002_020875c4(void);
static s32              func_ov002_020878f8(void* arg0, void* arg1);
static void             func_ov002_02087a18(void* arg1);
static void             func_ov002_02087a40(void* arg1);
static void             func_ov002_02087c80(void* arg1);
static void             func_ov002_02087ca0(void);
extern s32              func_ov031_0210d0b0(void* arg0);
extern s32              func_ov031_0210d0cc(void* arg0);
extern s32              func_ov031_0210d0fc(void* arg0);
extern s32              func_ov031_0210d10c(void* arg0);
extern s32              func_ov031_0210d114(void* arg0);
extern void             func_ov030_020ae92c(void);
static s32              func_ov002_020879f8(void* arg0, void* arg1);
static s32              func_ov002_020880a0(void* arg0);
static void             func_ov002_02088230(void* arg1);
static s32              func_ov002_0208847c(void* arg0, void* arg1);
static s32              func_ov002_02088524(void* arg0, void* arg1);
static s32              func_ov002_02088610(void* arg0, void* arg1);
static s32              func_ov002_020886ec(void* arg0, void* arg1);
static s32              func_ov002_02088998(void* arg0, void* arg1);
static s32              func_ov002_02088b28(void* arg0, void* arg1);
static s32              func_ov002_020890f8(void* arg0, void* arg1);
static s32              func_ov002_0208920c(void* arg0, void* arg1);
static s32              func_ov002_02089364(void* arg0);
static s32              func_ov002_020893b8(void* arg1);
static s32              func_ov002_020893d8(void* arg0, void* arg1);
static s32              func_ov002_02089498(void* arg0);
static void             func_ov002_020894f8(void);
static void             func_ov002_020894fc(void* arg1);
static void             func_ov002_0208950c(void);
static void             func_ov002_02089510(void);
static s32              func_ov002_02089514(void* arg0, s32 arg1);
static s32              func_ov002_0208958c(void* arg0);
static s32              func_ov002_020895c8(void* arg0);
static s32              func_ov002_02089624(void* arg0, void* arg1);
static s32              func_ov002_020896e4(void* arg0, void* arg1);
static s32              func_ov002_02089798(void* arg0, void* arg1);
static s32              func_ov002_02089860(void* arg0, void* arg1);
static s32              func_ov002_02089920(void* arg0, void* arg1);
static s32              func_ov002_02089a94(void* arg0);
static s32              func_ov002_02089ac8(void* arg1);
static s32              func_ov002_02089adc(void* arg1);
static s32              func_ov002_02089af0(void* arg0, void* arg1);
static s32              func_ov002_02089b1c(void* arg0, void* arg1);
static void             func_ov002_02089b3c(void* arg1);
static void             func_ov002_02089b80(void* arg1);
static void             func_ov002_02089ba8(void);
static void             func_ov002_02089bac(void);
static s32              func_ov002_02089bb0(void);
static s32              func_ov002_02089d40(void* arg0, void* arg1);
static s32              func_ov002_02089e3c(void* arg0, void* arg1);
static s32              func_ov002_0208a050(void* arg0, void* arg1);
static s32              func_ov002_0208a070(void* arg0, void* arg1);
static s32              func_ov002_0208a250(void* arg0, void* arg1);
static s32              func_ov002_0208a748(void* arg0);
static s32              func_ov002_0208a764(void* arg0);
static s32              func_ov002_0208aa4c(void* arg0, void* arg1);
static s32              func_ov002_0208aabc(void* arg0, void* arg1);
static s32              func_ov002_0208ab58(void* arg0, void* arg1);
static s32              func_ov002_0208af78(void* arg0);
static s32              func_ov002_0208b0a4(void* arg0, void* arg1);
static s32              func_ov002_0208b154(void* arg0, void* arg1);
static s32              func_ov002_0208b240(void* arg1);
static s32              func_ov002_0208b270(void* arg0, void* arg1);
static s32              func_ov002_0208b570(void* arg0);
static s32              func_ov002_0208b5c4(void* arg0);
static s32              func_ov002_0208b610(void* arg0, void* arg1);
static s32              func_ov002_0208b670(void* arg0, void* arg1);
static s32              func_ov002_0208b6d0(void* arg0);
static s32              func_ov002_0208b734(void* arg0);
static s32              func_ov002_0208c8b0(void* arg1);
static void             func_ov002_0208c794(void* arg1);
static void             func_ov002_0208c864(s32 arg1);
static void             func_ov002_0208c878(s32 arg1);
static void             func_ov002_0208c88c(void* arg1);
static s32              func_ov002_0208caa0(void);
static void             func_ov002_0208c9c8(void* arg1, s16* arg2);
static void             func_ov002_0208ca80(void* arg1);
static void             func_ov002_0208ca90(void* arg1);
static void             func_ov002_0208ca70(void* arg1);
static void             func_ov002_0208cf84(void* arg1);
static void             func_ov002_0208d008(void);
static void             func_ov002_0208d00c(void);
static void             func_ov002_0208cfe0(void* arg1);
static s32              func_ov002_0208d144(void);
static s32              func_ov002_0208d010(void* arg1);
static s32              func_ov002_0208d09c(void* arg1);
static s32              func_ov002_0208d154(void* arg0, void* arg1);
static s32              func_ov002_0208d290(void* arg1);
static s32              func_ov002_0208d22c(void* arg0, void* arg1);
static s32              func_ov002_0208d27c(void* arg1);
static s32              func_ov002_0208d4cc(void* arg0, void* arg1);
static s32              func_ov002_0208d5c0(void* arg0, void* arg1);
static s32              func_ov002_0208d6bc(void* arg0, void* arg1);
static s32              func_ov002_0208d778(void* arg0, void* arg1);
static s32              func_ov002_0208da24(void* arg0, void* arg1);
static s32              func_ov002_0208db58(void* arg0, void* arg1);
static s32              func_ov002_0208de64(void* arg0, void* arg1);
static s32              func_ov002_0208df6c(void* arg0, void* arg1);
static s32              func_ov002_0208e080(void* arg1);
static s32              func_ov002_0208e0b8(void* arg0, void* arg1);
static s32              func_ov002_0208e200(void* arg0, void* arg1);
static s32              func_ov002_0208e30c(void* arg1);
static s32              func_ov002_0208e658(void* arg0);
static s32              func_ov002_0208e6ac(void* arg0);
static s32              func_ov002_0208e6f8(void* arg0, void* arg1);
static s32              func_ov002_0208e750(void* arg0, void* arg1);
static s32              func_ov002_0208e7d8(void* arg0);
static s32              func_ov002_0208e838(void* arg0);
static s32              func_ov002_0208e348(void* arg0, void* arg1);
static s32              func_ov002_0208e410(void* arg0, void* arg1);
static s32              func_ov002_0208e514(void* arg0, void* arg1);
static void             func_ov002_0208e92c(void* arg1, void* arg2);
static void             func_ov002_0208eb04(s32 arg1);
static void             func_ov002_0208eb3c(void* arg1);
static void             func_ov002_0208ebe4(s32 arg1);
static s32              func_ov002_0208ec1c(void* arg1);
static s32              func_ov002_0208ec34(void* arg0, void* arg1);
static void             func_ov002_0208ed78(void* arg1, void* arg2);
static void             func_ov002_0208edac(void);
static void             func_ov002_0208edb0(void* arg1);
static void             func_ov002_0208ee98(void);
static s32              func_ov002_0208ee9c(void);
static s32              func_ov002_0208eeac(void* arg0, void* arg1);
static void             func_ov002_0208f0bc(void* arg1, void* arg2);
static s32              func_ov002_0208f6e0(void);
static s32              func_ov002_0208f6f0(void);
static void             func_ov002_0208f5d8(void* arg1);
static void             func_ov002_0208f630(void* arg1);
static void             func_ov002_0208f688(void* arg1);
static void             func_ov002_0208f794(void* arg1);
static s32              func_ov002_0208f858(void);
static void             func_ov002_0208f828(void* arg1);
static void             func_ov002_0208f838(void* arg1);
static void             func_ov002_0208f848(void* arg1);
static void             func_ov002_0208f860(void);
static void             func_ov002_0208f874(void);
static void             func_ov002_0208f878(void);
static void             func_ov002_0208f864(void* arg1);
static s32              func_ov002_0208f87c(void* arg0, void* arg1);
static s32              func_ov002_0208f89c(void* arg0);
static s32              func_ov002_0208f918(void* arg1);
static s32              func_ov002_0208f92c(void* arg0, s32 arg1);
static void             func_ov002_0208f9d0(void);
static void             func_ov002_0208f9d4(void* arg1);
static void             func_ov002_0208f9e4(void);
static void             func_ov002_0208f9e8(void);
static s32              func_ov002_0208ffac(void* arg0, void* arg1);
static s32              func_ov002_02090014(void* arg0, void* arg1);
static void             func_ov002_020902a4(void* arg1);
static void             func_ov002_02090310(s32 arg1);
static void             func_ov002_0209034c(void);
static void             func_ov002_02090350(void);
static s32              func_ov002_0209095c(void* arg0, void* arg1);
static s32              func_ov002_02090b30(void);
static s32              func_ov002_02090b44(void* arg0, void* arg1);
static s32              func_ov002_02090c28(void* arg0, void* arg1);
static s32              func_ov002_02090d0c(void* arg0, void* arg1);
static s32              func_ov002_02090df0(void* arg0, void* arg1);
static s32              func_ov002_02090ee8(void* arg0, s32 arg1);
static s32              func_ov002_02090f4c(void* arg0, s32 arg1);
static s32              func_ov002_02090fd8(void* arg0, void* arg1);
static s32              func_ov002_020910b0(void* arg0, void* arg1);
static s32              func_ov002_02091110(void* arg0, void* arg1);
static s32              func_ov002_02091208(void* arg0, void* arg1);
static s32              func_ov002_02091300(void* arg0, void* arg1);
static s32              func_ov002_020913d8(void* arg0, void* arg1);
static s32              func_ov002_020914b0(void* arg0, void* arg1);
static s32              func_ov002_02091510(void* arg0, void* arg1);
static s32              func_ov002_02091608(void* arg0, void* arg1);
static s32              func_ov002_02091700(void);
static s32              func_ov002_02091710(void);
static s32              func_ov002_02091720(void* arg1);
static void             func_ov002_020917fc(s32 arg1);
static void             func_ov002_020918ec(s32 arg1);
static void             func_ov002_02091918(s32 arg1);
static void             func_ov002_02091944(s32 arg1);
static s32              func_ov002_02091970(void);
static void             func_ov002_02091a14(void* arg1);
static void             func_ov002_02091a6c(void* arg1);
static void             func_ov002_02091a7c(void* arg1);
static void             func_ov002_02091a8c(void* arg1);
static s32              func_ov002_02091a9c(void);
static SpriteFrameInfo* func_ov002_0208c6f8(Sprite* arg0, s32 arg1, s32 arg2);
static SpriteFrameInfo* func_ov002_0208c92c(Sprite* arg0, s32 arg1, s32 arg2);
static SpriteFrameInfo* func_ov002_0208e890(Sprite* arg0, s32 arg1, s32 arg2);
static SpriteFrameInfo* func_ov002_0208f020(Sprite* arg0, s32 arg1, s32 arg2);
static SpriteFrameInfo* func_ov002_0208f6f8(Sprite* arg0, s32 arg1, s32 arg2);
static SpriteFrameInfo* func_ov002_02091760(Sprite* arg0, s32 arg1, s32 arg2);
static SpriteFrameInfo* func_ov002_02091978(Sprite* arg0, s32 arg1, s32 arg2);
static void*            data_ov002_02092d24[];
static void*            data_ov002_02092d88[];
static void*      data_ov002_02092b00[] = {(void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov031_0210d10c};
static void*      data_ov002_02092b0c[] = {(void*)func_ov002_020828ec, (void*)func_ov002_0208293c, (void*)func_ov031_0210d10c};
static void*      data_ov002_02092b18[] = {(void*)func_ov002_02082914, (void*)func_ov002_0208293c, (void*)func_ov031_0210d10c};
static const char data_ov002_02092bd0[] = "OtosuMenuLinklevel";
static const char data_ov002_02092be4[] = "OtosuMenuObj";
static void*      data_ov002_02092c08[] = {(void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_020870c8,
                                           (void*)func_ov002_020829f8, (void*)func_ov002_02082a20, (void*)func_ov002_0208749c,
                                           (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02087508,
                                           (void*)func_ov031_0210d10c};
static void*      data_ov002_02092c30[] = {(void*)func_ov002_02086cec, (void*)func_ov002_020829ac, (void*)func_ov002_020829d4,
                                           (void*)func_ov002_02086c64, (void*)func_ov002_02086c84, (void*)func_ov002_02086e4c,
                                           (void*)func_ov002_02086eb4, (void*)func_ov002_02086ee8, (void*)func_ov002_020870ac,
                                           (void*)func_ov031_0210d10c};
static void*      data_ov002_02092c58[] = {(void*)func_ov002_02086b8c, (void*)data_ov002_02092c30, (void*)func_ov002_02086c5c,
                                           (void*)func_ov002_02086c60, (void*)func_ov002_02086bb0};
static void*      data_ov002_02092bf4[] = {(void*)func_ov002_02086bac, (void*)data_ov002_02092c08, (void*)func_ov002_02086c5c,
                                           (void*)func_ov002_02086c60, (void*)func_ov002_02086bc4};
static const char data_ov002_02092c6c[] = "OtosuMenu_TitleEnmObj";
static const char data_ov002_02092c84[] = "OtosuMenu_TitleBossObj";
static void*      data_ov002_02092cb0[] = {(void*)func_ov002_020875c8, (void*)func_ov002_02082960, (void*)func_ov002_02082988,
                                           (void*)func_ov002_02087728, (void*)func_ov002_02082878, (void*)func_ov002_020828c8,
                                           (void*)func_ov002_020878f8, (void*)func_ov031_0210d10c};
static void*      data_ov002_02092c9c[] = {(void*)func_ov002_02087524, (void*)data_ov002_02092cb0, (void*)func_ov002_020875c0,
                                           (void*)func_ov002_020875c4, (void*)func_ov002_0208757c};
static const char data_ov002_02092cd0[] = "OtosuMenu_Icon";
static void*      data_ov002_02092ce0[] = {(void*)func_ov002_02087a18, (void*)data_ov002_02092d24, (void*)func_ov002_02087c80,
                                           (void*)func_ov002_02087ca0, (void*)func_ov002_02087a40};
static void*      data_ov002_02092cf4[] = {(void*)func_ov002_02087a18, (void*)data_ov002_02092d88, (void*)func_ov002_02087c80,
                                           (void*)func_ov002_02087ca0, (void*)func_ov002_02087a40};
static const char data_ov002_02092d08[] = "data/BeBadge_MapData.bin";
static void*      data_ov002_02092d24[] = {(void*)func_ov031_0210d0b0, (void*)func_ov031_0210d0cc, (void*)func_ov002_02082878,
                                           (void*)func_ov002_020828c8, (void*)func_ov002_020890f8, (void*)func_ov002_02082960,
                                           (void*)func_ov002_02082988, (void*)func_ov002_0208920c, (void*)func_ov002_02082878,
                                           (void*)func_ov002_020828c8, (void*)func_ov002_02089364, (void*)func_ov002_02082698,
                                           (void*)func_ov002_02088230, (void*)func_ov031_0210d10c};
static void*      data_ov002_02092d88[] = {
    (void*)func_ov002_020880a0, (void*)func_ov002_0208275c, (void*)func_ov002_0208847c, (void*)func_ov002_02088524,
    (void*)func_ov002_02088610, (void*)func_ov002_02082960, (void*)func_ov002_020879f8, (void*)func_ov002_02082988,
    (void*)func_ov002_020886ec, (void*)func_ov031_0210d0b0, (void*)func_ov031_0210d0cc, (void*)func_ov031_0210d0b0,
    (void*)func_ov031_0210d0cc, (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02088998,
    (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_02088b28, (void*)func_ov002_02082878,
    (void*)func_ov002_020828c8, (void*)func_ov002_020893b8, (void*)func_ov002_020893d8, (void*)func_ov002_02089498,
    (void*)func_ov002_02082698, (void*)func_ov002_02088230, (void*)func_ov031_0210d10c};
static char data_ov002_02092df4[] = "OtosuMenu_Icon2";
static char data_ov002_02092e04[] = "OtosuMenu_DaizaObj";

static void* data_ov002_02092e68[];
static void* data_ov002_02092e98[];
static void* data_ov002_02092ec8[];
static void* data_ov002_02092efc[];

static void* data_ov002_02092e18[] = {(void*)func_ov002_020894f8, (void*)data_ov002_02092e68, (void*)func_ov002_0208950c,
                                      (void*)func_ov002_02089510, (void*)func_ov002_020894fc};

static void* data_ov002_02092e2c[] = {(void*)func_ov002_020894f8, (void*)data_ov002_02092ec8, (void*)func_ov002_0208950c,
                                      (void*)func_ov002_02089510, (void*)func_ov002_020894fc};

static void* data_ov002_02092e40[] = {(void*)func_ov002_020894f8, (void*)data_ov002_02092e98, (void*)func_ov002_0208950c,
                                      (void*)func_ov002_02089510, (void*)func_ov002_020894fc};

static void* data_ov002_02092e54[] = {(void*)func_ov002_020894f8, (void*)data_ov002_02092efc, (void*)func_ov002_0208950c,
                                      (void*)func_ov002_02089510, (void*)func_ov002_020894fc};

static void* data_ov002_02092e68[] = {(void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02089514,
                                      (void*)func_ov002_0208958c, (void*)func_ov002_020895c8, (void*)func_ov002_020896e4,
                                      (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_02089a94,
                                      (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02089ac8};

static void* data_ov002_02092e98[] = {(void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02089514,
                                      (void*)func_ov002_0208958c, (void*)func_ov002_02089860, (void*)func_ov002_02082960,
                                      (void*)func_ov002_02082988, (void*)func_ov002_02089920, (void*)func_ov002_02082878,
                                      (void*)func_ov002_020828c8, (void*)func_ov002_02089adc, (void*)func_ov031_0210d10c};

static void* data_ov002_02092ec8[] = {(void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02089514,
                                      (void*)func_ov002_0208958c, (void*)func_ov002_020895c8, (void*)func_ov002_02089798,
                                      (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_02089920,
                                      (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02089b1c,
                                      (void*)func_ov031_0210d10c};

static void* data_ov002_02092efc[] = {(void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02089514,
                                      (void*)func_ov002_0208958c, (void*)func_ov002_020895c8, (void*)func_ov002_02089624,
                                      (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_02089920,
                                      (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_02089af0,
                                      (void*)func_ov031_0210d10c};

static void* data_ov002_02092f30[] = {(void*)func_ov002_0208b570, (void*)func_ov031_0210d10c};

static void* data_ov002_02092f38[] = {(void*)func_ov002_0208b610, (void*)func_ov002_0208b6d0, (void*)func_ov031_0210d10c};
static void* data_ov002_02092f58[];
static void* data_ov002_02092f98[];
static void* data_ov002_02093004[];
static void* data_ov002_0209301c[];
static void* data_ov002_02093070[];
static void* data_ov002_020930a4[];
static void* data_ov002_02093128[];
static void* data_ov002_020931dc[];
static void* data_ov002_020931fc[];
static void* data_ov002_0209321c[];
static void* data_ov002_0209323c[];
static void* data_ov002_0209327c[];
static void* data_ov002_02093298[];
static void* data_ov002_020932cc[];
static void* data_ov002_020932f0[];
static void* data_ov002_02093300[];
static void* data_ov002_02093338[];
static void* data_ov002_02093370[];
static void* data_ov002_020933a8[];
static void* data_ov002_02093494[];
static void* data_ov002_020934ac[];
static void* data_ov002_02092f44[] = {(void*)func_ov002_02089b3c, (void*)data_ov002_02092f98, (void*)func_ov002_02089ba8,
                                      (void*)func_ov002_02089bac, (void*)func_ov002_02089b80};
static void* data_ov002_02092f58[] = {
    (void*)func_ov002_02089bb0, (void*)func_ov002_02089d40, (void*)func_ov002_02089e3c, (void*)func_ov002_0208a070,
    (void*)func_ov002_0208a050, (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_0208a250,
    (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_0208266c, (void*)func_ov002_02082698,
    (void*)func_ov002_0208b670, (void*)func_ov002_0208b734, (void*)func_ov002_0208a748, (void*)func_ov031_0210d10c};
static void* data_ov002_02092f98[] = {
    (void*)func_ov002_02089bb0, (void*)func_ov002_0208aa4c, (void*)func_ov002_0208aabc, (void*)func_ov002_02082960,
    (void*)func_ov002_02082988, (void*)func_ov002_0208ab58, (void*)func_ov002_02082878, (void*)func_ov002_020828c8,
    (void*)func_ov002_0208af78, (void*)func_ov002_0208b0a4, (void*)func_ov002_0208b154, (void*)func_ov002_02082960,
    (void*)func_ov002_02082988, (void*)func_ov002_0208b240, (void*)func_ov002_0208b270, (void*)func_ov002_0208266c,
    (void*)func_ov002_02082698, (void*)func_ov002_0208b5c4, (void*)func_ov002_02082878, (void*)func_ov002_020828c8,
    (void*)func_ov002_0208a764, (void*)func_ov031_0210d10c};
static void* data_ov002_02092ff0[] = {(void*)func_ov002_02089b3c, (void*)data_ov002_02092f58, (void*)func_ov002_02089ba8,
                                      (void*)func_ov002_02089bac, (void*)func_ov002_02089b80};
static void* data_ov002_02093004[] = {(void*)func_ov002_0208c8b0};
static void* data_ov002_02093008[] = {(void*)func_ov002_0208c794, (void*)data_ov002_02093004, (void*)func_ov002_0208c878,
                                      (void*)func_ov002_0208c88c, (void*)func_ov002_0208c864};
static void* data_ov002_0209301c[] = {(void*)func_ov002_0208caa0};
static void* data_ov002_02093020[] = {(void*)func_ov002_0208c9c8, (void*)data_ov002_0209301c, (void*)func_ov002_0208ca80,
                                      (void*)func_ov002_0208ca90, (void*)func_ov002_0208ca70};
static void* data_ov002_02093034[] = {(void*)func_ov002_0208cf84, (void*)data_ov002_02093128, (void*)func_ov002_0208d008,
                                      (void*)func_ov002_0208d00c, (void*)func_ov002_0208cfe0};
static void* data_ov002_02093048[] = {(void*)func_ov002_0208cf84, (void*)data_ov002_02093070, (void*)func_ov002_0208d008,
                                      (void*)func_ov002_0208d00c, (void*)func_ov002_0208cfe0};
static void* data_ov002_0209305c[] = {(void*)func_ov002_0208cf84, (void*)data_ov002_020930a4, (void*)func_ov002_0208d008,
                                      (void*)func_ov002_0208d00c, (void*)func_ov002_0208cfe0};
static void* data_ov002_02093070[] = {(void*)func_ov002_0208d144, (void*)func_ov002_0208275c, (void*)func_ov002_0208e348,
                                      (void*)func_ov002_0208e410, (void*)func_ov002_02082960, (void*)func_ov002_02082988,
                                      (void*)func_ov002_0208e514, (void*)func_ov002_02082878, (void*)func_ov002_020828c8,
                                      (void*)func_ov002_0208d010, (void*)func_ov002_02082698, (void*)func_ov002_0208d290,
                                      (void*)func_ov031_0210d10c};
static void* data_ov002_020930a4[] = {
    (void*)func_ov002_0208d144, (void*)func_ov002_0208275c, (void*)func_ov002_0208de64, (void*)func_ov002_02082960,
    (void*)func_ov002_02082988, (void*)func_ov002_0208df6c, (void*)func_ov002_0208e080, (void*)func_ov002_0208e0b8,
    (void*)func_ov031_0210d0b0, (void*)func_ov031_0210d0cc, (void*)func_ov031_0210d0b0, (void*)func_ov031_0210d0cc,
    (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_0208e200, (void*)func_ov002_0208d010,
    (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_0208e30c, (void*)func_ov002_02082878,
    (void*)func_ov002_020828c8, (void*)func_ov002_0208e6ac, (void*)func_ov002_02082698, (void*)func_ov002_0208d09c,
    (void*)func_ov002_0208d154, (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_0208e658,
    (void*)func_ov002_0208d22c, (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_0208d27c,
    (void*)func_ov031_0210d10c};
static void* data_ov002_02093128[] = {
    (void*)func_ov002_0208d144, (void*)func_ov002_0208275c, (void*)func_ov002_0208d6bc, (void*)func_ov002_0208d4cc,
    (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_0208d5c0, (void*)func_ov002_0208d778,
    (void*)func_ov031_0210d0b0, (void*)func_ov031_0210d0cc, (void*)func_ov031_0210d0b0, (void*)func_ov031_0210d0cc,
    (void*)func_ov002_02082878, (void*)func_ov002_020828c8, (void*)func_ov002_0208da24, (void*)func_ov002_0208d010,
    (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_0208db58, (void*)func_ov002_02082878,
    (void*)func_ov002_020828c8, (void*)func_ov002_0208e6f8, (void*)func_ov002_0208e750, (void*)func_ov002_0208e838,
    (void*)func_ov002_02082698, (void*)func_ov002_0208d09c, (void*)func_ov002_0208d154, (void*)func_ov002_02082960,
    (void*)func_ov002_02082988, (void*)func_ov002_0208e7d8, (void*)func_ov002_0208d22c, (void*)func_ov002_02082878,
    (void*)func_ov002_020828c8, (void*)func_ov002_0208d27c, (void*)func_ov031_0210d10c};
static char  data_ov002_020931b4[] = "OtosuMenu_RankBoardObj";
static char  data_ov002_020931cc[] = "OtosuMenu_Icon2";
static void* data_ov002_020931dc[] = {(void*)func_ov002_0208ec1c, (void*)func_ov002_0208ec34, (void*)func_ov031_0210d114};
static void* data_ov002_020931e8[] = {(void*)func_ov002_0208e92c, (void*)data_ov002_020931dc, (void*)func_ov002_0208eb3c,
                                      (void*)func_ov002_0208ebe4, (void*)func_ov002_0208eb04};
static void* data_ov002_020931fc[] = {(void*)func_ov002_0208ee9c, (void*)func_ov002_0208eeac, (void*)func_ov031_0210d114};
static void* data_ov002_02093208[] = {(void*)func_ov002_0208ed78, (void*)data_ov002_020931fc, (void*)func_ov002_0208edb0,
                                      (void*)func_ov002_0208ee98, (void*)func_ov002_0208edac};
static void* data_ov002_0209321c[] = {(void*)func_ov002_0208f6e0, (void*)func_ov002_0208f6f0, (void*)func_ov031_0210d114};
static void* data_ov002_02093228[] = {(void*)func_ov002_0208f0bc, (void*)data_ov002_0209321c, (void*)func_ov002_0208f630,
                                      (void*)func_ov002_0208f688, (void*)func_ov002_0208f5d8};
static void* data_ov002_0209323c[] = {(void*)func_ov002_0208f858};
static void* data_ov002_02093240[] = {(void*)func_ov002_0208f794, (void*)data_ov002_0209323c, (void*)func_ov002_0208f838,
                                      (void*)func_ov002_0208f848, (void*)func_ov002_0208f828};
static void* data_ov002_02093254[] = {(void*)func_ov002_0208f860, (void*)data_ov002_0209327c, (void*)func_ov002_0208f874,
                                      (void*)func_ov002_0208f878, (void*)func_ov002_0208f864};
static void* data_ov002_02093268[] = {(void*)func_ov002_0208f860, (void*)data_ov002_02093298, (void*)func_ov002_0208f874,
                                      (void*)func_ov002_0208f878, (void*)func_ov002_0208f864};
static void* data_ov002_0209327c[] = {(void*)func_ov002_0208f87c, (void*)func_ov002_020829ac, (void*)func_ov002_020829d4,
                                      (void*)func_ov002_0208f92c, (void*)func_ov002_020828ec, (void*)func_ov002_0208293c,
                                      (void*)func_ov031_0210d10c};
static void* data_ov002_02093298[] = {(void*)func_ov002_0208f87c, (void*)func_ov002_020829ac, (void*)func_ov002_020829d4,
                                      (void*)func_ov002_0208f89c, (void*)func_ov002_020828ec, (void*)func_ov002_0208293c,
                                      (void*)func_ov002_0208f918, (void*)func_ov031_0210d10c};
static void* data_ov002_020932b8[] = {(void*)func_ov002_0208f9d0, (void*)data_ov002_020932cc, (void*)func_ov002_0208f9e4,
                                      (void*)func_ov002_0208f9e8, (void*)func_ov002_0208f9d4};
static void* data_ov002_020932cc[] = {(void*)func_ov002_0208ffac, (void*)func_ov002_02082960, (void*)func_ov002_02082988,
                                      (void*)func_ov002_02090014, (void*)func_ov002_02082878, (void*)func_ov002_020828c8,
                                      (void*)func_ov031_0210d10c};
static void* data_ov002_020932e8[] = {(void*)func_ov002_02091720, (void*)func_ov031_0210d10c};
static void* data_ov002_020932f0[] = {(void*)func_ov002_02091510, (void*)func_ov002_020829ac, (void*)func_ov002_020829d4,
                                      (void*)func_ov002_02091710};
static void* data_ov002_02093300[] = {(void*)func_ov002_02091608, (void*)func_ov002_020829ac, (void*)func_ov002_020829d4,
                                      (void*)func_ov002_02091700};
static void* data_ov002_02093310[] = {(void*)func_ov002_020902a4, (void*)data_ov002_02093300, (void*)func_ov002_0209034c,
                                      (void*)func_ov002_02090350, (void*)func_ov002_02090310};
static void* data_ov002_02093324[] = {(void*)func_ov002_020902a4, (void*)data_ov002_020932f0, (void*)func_ov002_0209034c,
                                      (void*)func_ov002_02090350, (void*)func_ov002_02090310};
static void* data_ov002_02093338[] = {(void*)func_ov002_02091110, (void*)func_ov002_020829ac, (void*)func_ov002_020829d4,
                                      (void*)func_ov002_02090ee8, (void*)func_ov002_02090f4c, (void*)func_ov002_020828a0,
                                      (void*)func_ov002_020828c8, (void*)func_ov002_02091300, (void*)func_ov002_02082960,
                                      (void*)func_ov002_02082988, (void*)func_ov002_020914b0, (void*)func_ov002_02082914,
                                      (void*)func_ov002_0208293c, (void*)func_ov002_02091720};
static void* data_ov002_02093370[] = {(void*)func_ov002_02091208, (void*)func_ov002_020829ac, (void*)func_ov002_020829d4,
                                      (void*)func_ov002_02090ee8, (void*)func_ov002_02090f4c, (void*)func_ov002_020828a0,
                                      (void*)func_ov002_020828c8, (void*)func_ov002_020913d8, (void*)func_ov002_02082960,
                                      (void*)func_ov002_02082988, (void*)func_ov002_020914b0, (void*)func_ov002_02082914,
                                      (void*)func_ov002_0208293c, (void*)func_ov002_02091720};
static void* data_ov002_020933a8[] = {
    (void*)func_ov002_02090b44, (void*)func_ov002_020829ac, (void*)func_ov002_020829d4, (void*)func_ov002_0209095c,
    (void*)func_ov031_0210d0fc, (void*)func_ov002_020828a0, (void*)func_ov002_020828c8, (void*)func_ov002_02090c28,
    (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_0209095c, (void*)func_ov031_0210d0fc,
    (void*)func_ov002_020828a0, (void*)func_ov002_020828c8, (void*)func_ov002_02090d0c, (void*)func_ov002_02082960,
    (void*)func_ov002_02082988, (void*)func_ov002_0209095c, (void*)func_ov002_02090b30, (void*)func_ov002_020828a0,
    (void*)func_ov002_020828c8, (void*)func_ov002_02090ee8, (void*)func_ov002_02090df0, (void*)func_ov002_02082960,
    (void*)func_ov002_02082988, (void*)func_ov002_02090f4c, (void*)func_ov002_020828a0, (void*)func_ov002_020828c8,
    (void*)func_ov002_02090fd8, (void*)func_ov002_02082960, (void*)func_ov002_02082988, (void*)func_ov002_020910b0,
    (void*)func_ov002_02082914, (void*)func_ov002_0208293c, (void*)func_ov002_02091720, (void*)func_ov031_0210d10c};
static void*     data_ov002_02093438[] = {(void*)func_ov002_020902a4, (void*)data_ov002_02093370, (void*)func_ov002_0209034c,
                                          (void*)func_ov002_02090350, (void*)func_ov002_02090310};
static void*     data_ov002_0209344c[] = {(void*)func_ov002_020902a4, (void*)data_ov002_020933a8, (void*)func_ov002_0209034c,
                                          (void*)func_ov002_02090350, (void*)func_ov002_02090310};
static void*     data_ov002_02093460[] = {(void*)func_ov002_020902a4, (void*)data_ov002_02093338, (void*)func_ov002_0209034c,
                                          (void*)func_ov002_02090350, (void*)func_ov002_02090310};
static char      data_ov002_02093474[] = "DelData_WinObj";
static char      data_ov002_02093484[] = "OtosuMenu_Icon2";
static void*     data_ov002_02093494[] = {(void*)func_ov002_02091970};
static void*     data_ov002_02093498[] = {(void*)func_ov002_020917fc, (void*)data_ov002_02093494, (void*)func_ov002_02091918,
                                          (void*)func_ov002_02091944, (void*)func_ov002_020918ec};
static void*     data_ov002_020934ac[] = {(void*)func_ov002_02091a9c};
static void*     data_ov002_020934b0[] = {(void*)func_ov002_02091a14, (void*)data_ov002_020934ac, (void*)func_ov002_02091a7c,
                                          (void*)func_ov002_02091a8c, (void*)func_ov002_02091a6c};
static s32       data_ov002_020934e0;
static u8        data_ov002_020934e4[0x16];
static u16       data_ov002_020935c0[0x10];
static u16       data_ov002_020935e0[0x40];
static s32       data_ov002_02093660;
static s16       data_ov002_02093664[0xE];
static char      data_ov002_02092b24[24]       = "Apl_Fuk/Grp_DelMenu.bin";
static char      data_ov002_02092b3c[28]       = "Apl_Tak/Grp_Menu_BGU.bin";
static char      data_ov002_02092b58[28]       = "Apl_Tak/Grp_Menu_BGD.bin";
static char      data_ov002_02092b74[28]       = "Apl_Fuk/Grp_OtosuMenu.bin";
static char      data_ov002_02092b90[32]       = "Apl_Fuk/Grp_OtosuMenuObj.bin";
static char      data_ov002_02092bb0[32]       = "Apl_Kit/GRP_FldDownScreen.bin";
static const u16 data_ov002_02091aa4[4]        = {0x000C, 0x000D, 0x000E, 0x000F}; /* const */
static const Ov002_BinPack data_ov002_02091aac = {
    {0x02, (char*)data_ov002_02092b74},
    {0x0004, 0x0018, 0x002C, 0x0040},
    {0x0001, 0x0002, 0x0003, 0x0004, 0x0010, 0x0011, 0x0012, 0x0013}
};                                            /* const */
static const BinIdentifier data_ov002_02091acc = {0x02, (char*)data_ov002_02092b90};                              /* const */
static void (*data_ov002_02091ad4[3])(void*)   = {func_ov002_020860ec, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091ae0[3])(void*)   = {func_ov002_02086160, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091aec[3])(void*)   = {func_ov002_020861c8, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091af8[3])(void*)   = {func_ov002_02086050, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091b04[3])(void*)   = {func_ov002_0208622c, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091b10[3])(void*)   = {func_ov002_02086290, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091b1c[3])(void*)   = {func_ov002_02085e20, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091b28[3])(void*)   = {func_ov002_02085e8c, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091b34[3])(void*)   = {func_ov002_020862f4, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091b40[3])(void*)   = {func_ov002_02085f04, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091b4c[3])(void*)   = {func_ov002_02086358, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static void (*data_ov002_02091b58[3])(void*)   = {func_ov002_02085fd4, func_ov002_020863bc, func_ov002_0208678c}; /* const */
static const u16             data_ov002_02091b64[] = {0x23FB, 0x0017, 0x0049, 0x0049, 0x0055, 0x23FC, 0x0067,
                                                      0x0049, 0x0098, 0x0055, 0x23FD, 0x00B7, 0x0049, 0x00E9,
                                                      0x0055, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
static const SpriteAnimation data_ov002_02091b8c   = {SPRITE_ANIM_FLAGS_1280,
                                                      .unk_02    = 0x0000,
                                                      .unk_04    = -13,
                                                      .unk_06    = 0x000C,
                                                      .unk_08    = func_ov002_020824ac,
                                                      .unk_0C    = 0,
                                                      .unk_10    = 0,
                                                      .binIden   = (BinIdentifier*)&data_ov002_02091acc,
                                                      .unk_18    = 0x0000,
                                                      .packIndex = 0x0000,
                                                      .unk_1C    = 0x0015,
                                                      .unk_1E    = 0x0000,
                                                      .unk_20    = 0x0018,
                                                      .unk_22    = 0x0002,
                                                      .unk_24    = 0x0000,
                                                      .unk_26    = 0x0016,
                                                      .unk_28    = 0x0017,
                                                      .unk_2A    = 0x0001}; /* const */
static const u16             data_ov002_02091bb8[] = {
    0x23FE, 0x0010, 0x0062, 0x0050, 0x0072, 0x23FF, 0x0010, 0x0082, 0x0050, 0x0092, 0x2400, 0x0010, 0x00A2,
    0x0050, 0x00B2, 0x2401, 0x0060, 0x0062, 0x00A0, 0x0072, 0x2402, 0x0060, 0x0082, 0x00A0, 0x0092, 0x2403,
    0x0060, 0x00A2, 0x00A0, 0x00B2, 0x2404, 0x00B0, 0x0062, 0x00F0, 0x0072, 0x2405, 0x00B0, 0x0082, 0x00F0,
    0x0092, 0x2406, 0x00B0, 0x00A2, 0x00F0, 0x00B2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000};           /* const */
static const BinIdentifier data_ov002_02091c1c    = {0x02, (char*)data_ov002_02092b24};                            /* const */
static const BinIdentifier data_ov002_02091c24    = {0x2B, (char*)data_ov002_02092b58};                            /* const */
static const BinIdentifier data_ov002_02091c2c    = {0x2B, (char*)data_ov002_02092b3c};                            /* const */
static const BinIdentifier data_ov002_02091c34    = {0x1E, (char*)data_ov002_02092bb0};                            /* const */
static const Ov002_U16_2   data_ov002_02091c3c[6] = {
    {0x0002, 0x0002},
    {0x009D, 0x0001},
    {0x003F, 0x004D},
    {0x0001, 0x003F},
    {0x0075, 0x0002},
    {0x00A4, 0x009D}
}; /* const */
static u16 data_ov002_02091c54[0x19] = {
    0,    8,    0xA8, 0x58, 0xB8, 1,    0x4C, 0x58,   0xB8,   0x68,   2,      0x4C,   0x80,
    0xB8, 0x90, 3,    0xA5, 0xA0, 0xF5, 0xB8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
}; /* const */
static const u16               data_ov002_02091c86[] = {0x23F0, 0x0008, 0x0008, 0x00F8, 0x0040, 0x23EB, 0x0018, 0x00A6,
                                                        0x0058, 0x00B6, 0x23F1, 0x0058, 0x0056, 0x00B8, 0x0066, 0x23F2,
                                                        0x0058, 0x007E, 0x00B8, 0x008E, 0x23F3, 0x00C5, 0x00A6, 0x00F5,
                                                        0x00B6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0000}; /* const */
static const Ov002_Config91cc4 data_ov002_02091cc4   = {
    0x0001, 0x0002, 0x0003, 0x0004, 0x0027, 0x0000, data_ov002_02092d08};                                      /* const */
static const Ov002_U16_2 data_ov002_02091cd4[5] = {
    {0x23FA, 0x0008},
    {0x0008, 0x00F8},
    {0x0040, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0x0000}
};                   /* const */
static const Ov002_U16_2 data_ov002_02091ce8[5] = {
    {0x23F9, 0x0008},
    {0x0008, 0x00F8},
    {0x0040, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0x0000}
};                   /* const */
static const Ov002_U16_2 data_ov002_02091cfc[5] = {
    {0x23F8, 0x0008},
    {0x0008, 0x00F8},
    {0x0040, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0x0000}
};                   /* const */
static const Ov002_U16_2 data_ov002_02091d10[5] = {
    {0x23F8, 0x0008},
    {0x0008, 0x00F8},
    {0x0040, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0x0000}
};                   /* const */
static s32 data_ov002_02091d24[0xA] = {0, 0, 0, 0, 0x38, 0, 0, 0, 0, 0};                                         /* const */
static s32 data_ov002_02091d4c[0xA] = {
    0x1398, 0x2730, 0x3AC8, 0x4E78, 0x6220, 0x7600, 0x8998, 0x9D30, 0xB0C8, 0xC460,
}; /* const */
static s32 data_ov002_02091d74[0xA] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}; /* const */
static s32 data_ov002_02091d9c[0xA] = {
    0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388,
}; /* const */
static s32 data_ov002_02091dc4[0xA] = {
    0x1398, 0x2730, 0x3AC8, 0x4E60, 0x6210, 0x75F0, 0x8998, 0x9D30, 0xB0C8, 0xC460,
}; /* const */
static s32 data_ov002_02091dec[0xA] = {
    0x10, 0x13A8, 0x2740, 0x3AD8, 0x4E88, 0x6268, 0x7610, 0x89A8, 0x9D40, 0xB0D8,
}; /* const */
static s32 data_ov002_02091e14[0xA]  = {0, 0, 0, 0x18, 0x10, 0x10, 0, 0, 0, 0};                                     /* const */
static s32 data_ov002_02091e3c[0xA]  = {0, 0x1398, 0x2730, 0x3AC8, 0x4E78, 0x6258, 0x7600, 0x8998, 0x9D30, 0xB0C8}; /* const */
static u16 data_ov002_02091e64[0x1B] = {
    3,    7, 0x55, 3,    7, 0x75, 3,    7, 0x95, 3,    0x57, 0x55, 3,    0x57,
    0x75, 3, 0x57, 0x95, 3, 0xA7, 0x55, 3, 0xA7, 0x75, 3,    0xA7, 0x95,
}; /* const */
static u16 data_ov002_02091e9a[0x1B] = {
    3,    7, 0x55, 3,    7, 0x75, 3,    7, 0x95, 3,    0x57, 0x55, 3,    0x57,
    0x75, 3, 0x57, 0x95, 3, 0xA7, 0x55, 3, 0xA7, 0x75, 3,    0xA7, 0x95,
}; /* const */
static const Ov002_U16_2 data_ov002_02091ed0[0x12] = {
    {0x0000, 0x0032},
    {0x0001, 0x0002},
    {0x0002, 0x0004},
    {0x0003, 0x0004},
    {0x0004, 0x0006},
    {0x0005, 0x0006},
    {0x0006, 0x0008},
    {0x0007, 0x0008},
    {0x0008, 0x0008},
    {0x0009, 0x0006},
    {0x0008, 0x0006},
    {0x0007, 0x0005},
    {0x0006, 0x0004},
    {0x0005, 0x0004},
    {0x0004, 0x0003},
    {0x0003, 0x0002},
    {0x0002, 0x0002},
    {0x0001, 0x0001}
}; /* const */
static const Ov002_U16_2 data_ov002_02091f7c[0xC] = {
    {0x0000, 0x0010},
    {0x0058, 0x0050},
    {0x0070, 0x0001},
    {0x0010, 0x0078},
    {0x0050, 0x0090},
    {0x0002, 0x0010},
    {0x0098, 0x0050},
    {0x00B0, 0x0003},
    {0x0060, 0x0058},
    {0x00A0, 0x0070},
    {0x0004, 0x0060},
    {0x0078, 0x00A0}
}; /* const */
static const Ov002_U16_3 data_ov002_02091fe0    = {0x0002, 0x0002, 0x009D};                                      /* const */
static const Ov002_U16_2 data_ov002_02091fe6[5] = {
    {0x23EE, 0x0000},
    {0x0000, 0x0100},
    {0x00C0, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0x0000}
};                   /* const */
static const Ov002_U16_2 data_ov002_02091ffa[5] = {
    {0x0000, 0x0008},
    {0x00A0, 0x0058},
    {0x00B8, 0xFFFF},
    {0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF}
};                   /* const */
static u16 data_ov002_0209200e[0xF] = {
    0x23EC, 8, 8, 0xF8, 0x40, 0x23EB, 0x18, 0xA6, 0x58, 0xB6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
static u16 data_ov002_0209202c[0xF] = {
    0x23EC, 8, 8, 0xF8, 0x40, 0x23EB, 0x18, 0xA6, 0x58, 0xB6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
static u16 data_ov002_0209204a[0xF] = {
    0x23EF, 8, 8, 0xF8, 0x40, 0x23EB, 0x18, 0xA6, 0x58, 0xB6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
static const Ov002_U16_12 data_ov002_02092068      = {0x0002, 0x0002, 0x009D, 0x0002, 0x002A, 0x0054,
                                                      0x0002, 0x0002, 0x009D, 0x0002, 0x00A4, 0x009D}; /* const */
static const Ov002_U16_10 data_ov002_02092080      = {0x0000, 0x0008, 0x00A0, 0x0058, 0x00B8,
                                                      0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};         /* const */
static const Ov002_U16_12 data_ov002_02092094      = {0x0002, 0x0002, 0x009D, 0x0004, 0x0040, 0x003E,
                                                      0x0004, 0x0040, 0x005E, 0x0004, 0x0040, 0x007E}; /* const */
static u16                data_ov002_020920ac[0xF] = {
    0x23F4, 8, 8, 0xF8, 0x40, 0x23EB, 0x18, 0xA6, 0x58, 0xB6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
static u16 data_ov002_020920ca[0xF] = {
    0x23F5, 8, 8, 0xF8, 0x40, 0x23EB, 0x18, 0xA6, 0x58, 0xB6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
static u16 data_ov002_020920e8[0xF] = {
    0, 8, 0xA0, 0x58, 0xB8, 1, 0xA5, 0xA0, 0xF5, 0xB8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
}; /* const */
static const Ov002_U16_20 data_ov002_02092106       = {0x23F6, 0x0008, 0x0008, 0x00F8, 0x0040, 0x23F7, 0x00C5,
                                                       0x00A6, 0x00F5, 0x00B6, 0x23EB, 0x0018, 0x00A6, 0x0058,
                                                       0x00B6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
static u16                data_ov002_0209212e[0x19] = {
    0,    8,    0xA0, 0x58, 0xB8, 1,    0x48, 0x40,   0xB6,   0x58,   2,      0x48,   0x60,
    0xB6, 0x78, 3,    0x48, 0x80, 0xB6, 0x98, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
}; /* const */
static const Ov002_U16_4     data_ov002_02092160      = {0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
static const Ov002_U16_4     data_ov002_02092168      = {0x0005, 0x0004, 0x0003, 0x0002}; /* const */
static const SpriteAnimation data_ov002_02092170      = {SPRITE_ANIM_FLAGS_1281,
                                                         .unk_02    = 0x0000,
                                                         .unk_04    = -13,
                                                         .unk_06    = 0x000C,
                                                         .unk_08    = func_ov002_0208c6f8,
                                                         .unk_0C    = 0,
                                                         .unk_10    = 0,
                                                         .binIden   = (BinIdentifier*)&data_ov002_02091acc,
                                                         .unk_18    = 0x0000,
                                                         .packIndex = 0x0000,
                                                         .unk_1C    = 0x0015,
                                                         .unk_1E    = 0x0000,
                                                         .unk_20    = 0x0018,
                                                         .unk_22    = 0x0002,
                                                         .unk_24    = 0x0000,
                                                         .unk_26    = 0x0016,
                                                         .unk_28    = 0x0017,
                                                         .unk_2A    = 0x0001}; /* const */
static const SpriteAnimation data_ov002_0209219c      = {SPRITE_ANIM_FLAGS_1280,
                                                         .unk_02    = 0x0400,
                                                         .unk_04    = -13,
                                                         .unk_06    = 0x000C,
                                                         .unk_08    = func_ov002_0208c92c,
                                                         .unk_0C    = 0,
                                                         .unk_10    = 0,
                                                         .binIden   = (BinIdentifier*)&data_ov002_02091acc,
                                                         .unk_18    = 0x0000,
                                                         .packIndex = 0x0000,
                                                         .unk_1C    = 0x0015,
                                                         .unk_1E    = 0x0000,
                                                         .unk_20    = 0x0018,
                                                         .unk_22    = 0x0002,
                                                         .unk_24    = 0x0000,
                                                         .unk_26    = 0x0016,
                                                         .unk_28    = 0x0017,
                                                         .unk_2A    = 0x0001};                       /* const */
static const Ov002_U16_9     data_ov002_020921c8      = {0x0005, 0x0048, 0x0052, 0x0005, 0x0047,
                                                         0x003C, 0x0005, 0x0047, 0x0064};         /* const */
static const Ov002_U16_10    data_ov002_020921da      = {0x23F8, 0x0008, 0x0008, 0x00F8, 0x0040,
                                                         0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
static const Ov002_U16_10    data_ov002_020921ee      = {0x0000, 0x0068, 0x005C, 0x00B0, 0x0068,
                                                         0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}; /* const */
static const Ov002_U16_10    data_ov002_02092202      = {0x23F8, 0x0008, 0x0008, 0x00F8, 0x0040,
                                                         0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
static const Ov002_U16_10    data_ov002_02092216      = {0x240A, 0x0068, 0x005C, 0x00B0, 0x0068,
                                                         0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
static const Ov002_U16_10    data_ov002_0209222a      = {0x2409, 0x0008, 0x0008, 0x00F8, 0x0040,
                                                         0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
static const Ov002_U16_10    data_ov002_0209223e      = {0x240B, 0x0008, 0x0008, 0x00F8, 0x0040,
                                                         0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000}; /* const */
static u16                   data_ov002_02092252[0xF] = {
    0x2407, 0x68, 0x46, 0xB0, 0x56, 0x2408, 0x68, 0x6E, 0xB0, 0x7E, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
static u16 data_ov002_02092270[0xF] = {
    0, 0x50, 0x40, 0xB0, 0x58, 1, 0x50, 0x68, 0xB0, 0x80, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
}; /* const */
static const Ov002_U16_4 data_ov002_0209228e[0x2C] = {
    {0x240C, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0xFFFF, 0xFFFF},
    {0x240C, 0x240D, 0xFFFF, 0xFFFF},
    {0x240C, 0x240E, 0xFFFF, 0xFFFF},
    {0x240C, 0x241F, 0xFFFF, 0xFFFF},
    {0x240C, 0x241B, 0xFFFF, 0xFFFF},
    {0x240C, 0x2428, 0x2429, 0x242A},
    {0x240C, 0x242B, 0x242C, 0x242D},
    {0x240C, 0x2418, 0xFFFF, 0xFFFF},
    {0x240E, 0x2419, 0xFFFF, 0xFFFF},
    {0x240C, 0x2420, 0xFFFF, 0xFFFF},
    {0x240C, 0x241A, 0xFFFF, 0xFFFF},
    {0x240C, 0x241B, 0xFFFF, 0xFFFF},
    {0x240C, 0x241E, 0x241E, 0xFFFF},
    {0x2423, 0x2422, 0x242E, 0x242F},
    {0x2423, 0x241D, 0xFFFF, 0xFFFF},
    {0x2423, 0x2417, 0x2417, 0xFFFF},
    {0x240C, 0x2416, 0x2416, 0x2416},
    {0x240C, 0x241F, 0xFFFF, 0xFFFF},
    {0x2423, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x241C, 0xFFFF, 0xFFFF},
    {0x2423, 0x2425, 0x2425, 0x2425},
    {0x240C, 0x2411, 0xFFFF, 0xFFFF},
    {0x2426, 0x2412, 0xFFFF, 0xFFFF},
    {0x240E, 0x2413, 0xFFFF, 0xFFFF},
    {0x240D, 0x2414, 0xFFFF, 0xFFFF},
    {0x240C, 0x2415, 0xFFFF, 0xFFFF},
    {0x2423, 0x2411, 0xFFFF, 0xFFFF},
    {0x2423, 0x2411, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0xFFFF, 0xFFFF},
    {0x240C, 0x2424, 0xFFFF, 0xFFFF},
    {0x240C, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0xFFFF, 0xFFFF},
    {0x240C, 0x2421, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0xFFFF, 0xFFFF},
    {0x240C, 0x2423, 0x2421, 0xFFFF},
    {0x240C, 0x2427, 0x2425, 0x241C},
    {0x240C, 0x2418, 0xFFFF, 0xFFFF},
    {0x240C, 0x2419, 0xFFFF, 0xFFFF},
    {0x240C, 0x2420, 0xFFFF, 0xFFFF},
    {0x240C, 0x241A, 0xFFFF, 0xFFFF},
    {0x240C, 0x241E, 0xFFFF, 0xFFFF},
    {0x240C, 0x242E, 0xFFFF, 0xFFFF}
};                            /* const */
static const Ov002_U16_6 data_ov002_020923f0    = {0x0303, 0x0202, 0x002C, 0x0026, 0x0075, 0x006C}; /* const */
static const Ov002_U16_2 data_ov002_020923fc[8] = {
    {0x0001, 0x0004},
    {0x0002, 0x0003},
    {0x0005, 0x0008},
    {0x0006, 0x0007},
    {0x0009, 0x000C},
    {0x000A, 0x000B},
    {0x000D, 0x0010},
    {0x000E, 0x000F}
}; /* const */
static const SpriteAnimation data_ov002_0209241c = {SPRITE_ANIM_FLAGS_1280,
                                                    .unk_02    = 0x0000,
                                                    .unk_04    = -13,
                                                    .unk_06    = 0x000C,
                                                    .unk_08    = func_ov002_0208e890,
                                                    .unk_0C    = 0,
                                                    .unk_10    = 0,
                                                    .binIden   = (BinIdentifier*)&data_ov002_02091acc,
                                                    .unk_18    = 0x0000,
                                                    .packIndex = 0x0000,
                                                    .unk_1C    = 0x0015,
                                                    .unk_1E    = 0x0000,
                                                    .unk_20    = 0x0018,
                                                    .unk_22    = 0x0002,
                                                    .unk_24    = 0x0000,
                                                    .unk_26    = 0x0016,
                                                    .unk_28    = 0x0017,
                                                    .unk_2A    = 0x0001}; /* const */
static const Ov002_U16_46    data_ov002_02092448 = {
    0x0004, 0x0005, 0x0006, 0x0007, 0xFFE8, 0x0010, 0x0038, 0x0060, 0x0000, 0x0028, 0x0050, 0x0078,
    0x0000, 0x0028, 0x0050, 0x0078, 0x0000, 0x0028, 0x0050, 0x0078, 0x0009, 0x000A, 0x000B, 0x000C,
    0x0013, 0x0014, 0x0015, 0x0016, 0xFFEC, 0x0014, 0x003C, 0x0064, 0x0009, 0x000A, 0x000B, 0x000C,
    0x0013, 0x0014, 0x0015, 0x0016, 0x0000, 0xFFFA, 0xFFF4, 0xFFEE, 0xFFE8, 0xFFE2};        /* const */
static const Ov002_U16_2 data_ov002_020924a4[5] = {
    {0x0012, 0x0013},
    {0x0014, 0x0015},
    {0x0016, 0x0017},
    {0x0018, 0x0019},
    {0x001A, 0x001B}
}; /* const */
static const Ov002_U16_2 data_ov002_020924b8[5] = {
    {0x0008, 0x0009},
    {0x000A, 0x000B},
    {0x000C, 0x000D},
    {0x000E, 0x000F},
    {0x0010, 0x0011}
}; /* const */
static const SpriteAnimation data_ov002_020924cc = {SPRITE_ANIM_FLAGS_1281,
                                                    .unk_02    = 0x0000,
                                                    .unk_04    = -13,
                                                    .unk_06    = 0x000C,
                                                    .unk_08    = func_ov002_0208f020,
                                                    .unk_0C    = 0,
                                                    .unk_10    = 0,
                                                    .binIden   = (BinIdentifier*)&data_ov002_02091acc,
                                                    .unk_18    = 0x0000,
                                                    .packIndex = 0x0000,
                                                    .unk_1C    = 0x0015,
                                                    .unk_1E    = 0x0000,
                                                    .unk_20    = 0x0018,
                                                    .unk_22    = 0x0002,
                                                    .unk_24    = 0x0000,
                                                    .unk_26    = 0x0016,
                                                    .unk_28    = 0x0017,
                                                    .unk_2A    = 0x0001}; /* const */
static const SpriteAnimation data_ov002_020924f8 = {SPRITE_ANIM_FLAGS_1281,
                                                    .unk_02    = 0x0000,
                                                    .unk_04    = 0x0080,
                                                    .unk_06    = 0x0060,
                                                    .unk_08    = func_ov002_0208f6f8,
                                                    .unk_0C    = 0,
                                                    .unk_10    = 0,
                                                    .binIden   = (BinIdentifier*)&data_ov002_02091acc,
                                                    .unk_18    = 0x0000,
                                                    .packIndex = 0x0000,
                                                    .unk_1C    = 0x0015,
                                                    .unk_1E    = 0x0000,
                                                    .unk_20    = 0x0018,
                                                    .unk_22    = 0x0002,
                                                    .unk_24    = 0x0000,
                                                    .unk_26    = 0x0016,
                                                    .unk_28    = 0x0017,
                                                    .unk_2A    = 0x0001};                                            /* const */
static const Ov002_U16_8x data_ov002_02092524 = {0x0C0E, 0x000A, 0x20A4, 0x20A3, 0x20A5, 0x20A7, 0x20A8, 0x20A9}; /* const */
static u16                data_ov002_02092534[0xD6] = {
    0x209E, 0x2093, 0x2094, 0x2095, 0x2096, 0x2097, 0x2098, 0x2099, 0x209A, 0x208E, 0x208F, 0x2090, 0x2091, 0x2092, 0xFFFF, 0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,
}; /* const */
static const u16 data_ov002_020926e0[0xC8] = {
    0xFFFF,
}; /* const */
static const Ov002_U16_46 data_ov002_02092870 = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0x0003, 0x0080, 0x0056, 0x0003, 0x0080, 0x006A,
}; /* const */
static const Ov002_U16_10 data_ov002_020928cc = {
    0x2A36, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_020928e0 = {
    0x2A35, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_020928f4 = {
    0x2A41, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_02092908 = {
    0x2A38, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_0209291c = {
    0x2A42, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_02092930 = {
    0x3383, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_02092944 = {
    0x2A3F, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_02092958 = {
    0x2A40, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_0209296c = {
    0x2A37, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_02092980 = {
    0x3384, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static const Ov002_U16_10 data_ov002_02092994 = {
    0x2A34, 0x0010, 0x0050, 0x00F0, 0x0098, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000,
}; /* const */
static u16 data_ov002_020929a8[0xF] = {
    0x2A3B, 0x38, 0x51, 0xC8, 0x5B, 0x2A3C, 0x38, 0x65, 0xC8, 0x6F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
static u16 data_ov002_020929c6[0xF] = {
    0x2A3D, 0x38, 0x51, 0xC8, 0x5B, 0x2A3E, 0x38, 0x65, 0xC8, 0x6F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
static u16 data_ov002_020929e4[0xF] = {
    0, 0x38, 0x4C, 0xC8, 0x60, 1, 0x38, 0x61, 0xC8, 0x74, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
}; /* const */
static u16 data_ov002_02092a02[0xF] = {
    0x2A39, 0x38, 0x51, 0xC8, 0x5B, 0x2A3A, 0x38, 0x65, 0xC8, 0x6F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0,
}; /* const */
extern SpriteAnimation       data_ov002_02092a20;
static const SpriteAnimation data_ov002_02092a4c       = {SPRITE_ANIM_FLAGS_1280,
                                                          .unk_02    = 0x0800,
                                                          .unk_04    = 0x0080,
                                                          .unk_06    = 0x0060,
                                                          .unk_08    = func_ov002_02091760,
                                                          .unk_0C    = 0,
                                                          .unk_10    = 0,
                                                          .binIden   = (BinIdentifier*)&data_ov002_02091c34,
                                                          .unk_18    = 0x0002,
                                                          .packIndex = 0x000D,
                                                          .unk_1C    = 0x0002,
                                                          .unk_1E    = 0x0000,
                                                          .unk_20    = 0x0001,
                                                          .unk_22    = 0x0002,
                                                          .unk_24    = 0x0000,
                                                          .unk_26    = 0x0003,
                                                          .unk_28    = 0x0004,
                                                          .unk_2A    = 0x0001}; /* const */
static const Ov002_U16_2     data_ov002_02092a78[0x10] = {
    {0x000C, 0x0002},
    {0x0001, 0x0003},
    {0x0004, 0x0000},
    {0xFFF6, 0x0001},
    {0x000D, 0x0002},
    {0x0001, 0x0003},
    {0x0004, 0x0000},
    {0xFFF6, 0x0001},
    {0x000D, 0x0002},
    {0x0001, 0x0003},
    {0x0004, 0x0000},
    {0x000A, 0x0001},
    {0x000C, 0x0002},
    {0x0001, 0x0003},
    {0x0004, 0x0000},
    {0x000A, 0x0002}
}; /* const */
static const SpriteAnimation data_ov002_02092ab8 = {SPRITE_ANIM_FLAGS_1280,
                                                    .unk_02    = 0x0800,
                                                    .unk_04    = 0x0080,
                                                    .unk_06    = 0x0060,
                                                    .unk_08    = func_ov002_02091978,
                                                    .unk_0C    = 0,
                                                    .unk_10    = 0,
                                                    .binIden   = (BinIdentifier*)&data_ov002_02091acc,
                                                    .unk_18    = 0x0000,
                                                    .packIndex = 0x0000,
                                                    .unk_1C    = 0x0021,
                                                    .unk_1E    = 0x0000,
                                                    .unk_20    = 0x0024,
                                                    .unk_22    = 0x0001,
                                                    .unk_24    = 0x0000,
                                                    .unk_26    = 0x0022,
                                                    .unk_28    = 0x0023,
                                                    .unk_2A    = 0x0001}; /* const */

u8* func_ov002_020824a0(void) {
    return data_ov002_020934fa;
}

static SpriteFrameInfo* func_ov002_020824ac(Sprite* sprite, s32 arg1, s32 mode) {
    (void)arg1;
    if (mode == 1) {
        data_0206b408.unk_00 = 1;
        return &data_0206b408;
    } else if (mode == 2) {
        data_0206b408.unk_04 = 0;
        data_0206b408.unk_08 = 0;
        data_0206b408.unk_0C = 0;
        data_0206b408.unk_10 = -1;

        if (sprite->animData != NULL && sprite->unk1C != NULL && sprite->unk16 >= 0) {
            data_0206b408.unk_04 = *((u16*)sprite->unk1C + (sprite->unk16 * 4 + 1));
            data_0206b408.unk_08 = (s32)((u16*)sprite->unk1C + *((u16*)((u8*)sprite->unk1C + (sprite->unk16 * 8))));
        }
        return &data_0206b408;
    }
    return NULL;
}

static u16 subroutine_arg0[0x400];

void func_ov002_02082548(Ov002_Manager* mgr) {
    SpriteAnimation anim = data_ov002_02091b8c;

    anim.dataType = (*(u8*)((u8*)mgr + 0x1158C)) & 0xF;
    anim.unk_2A   = 1;
    anim.unk_04   = 0x80;
    anim.unk_06   = 0xC8;
    anim.unk_1C   = 0x15;
    anim.unk_20   = 0x18;
    anim.unk_22   = 2;
    anim.unk_26   = 0x16;
    anim.unk_28   = 0x17;

    if (_Sprite_Load(&mgr->sprite, &anim) == 0) {
        OS_WaitForever();
    }
    mgr->spriteActive = 1;
}

void func_ov002_02082610(Ov002_Manager* mgr) {
    mgr->spriteActive = 0;
    Sprite_Destroy(&mgr->sprite);
    func_ov031_0210cff0((u8*)mgr + 0x41804, (u8*)mgr + 0x46100);
}

void func_ov002_0208264c(Ov002_Manager* mgr) {
    mgr->spriteActive = 0;
    Sprite_Destroy(&mgr->sprite);
}

s32 func_ov002_0208266c(void* arg0, void* arg1) {
    ((u8*)&data_02074d10)[0x40A] = func_ov002_02082bec(arg1);
    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov002_02082698(void* arg0, void* arg1) {
    u8* mgr = (u8*)arg1;

    func_0203b454(mgr + 0x41862, &data_020750fc, 6);
    func_0203b454(mgr + 0x41862, mgr + 0x41FB6, 6);
    func_ov002_02082d44(arg1);

    ((u8*)&data_02074d10)[0x40B] = *(u16*)(mgr + 0x4198A);
    ((u8*)&data_02074d10)[0x40C] = *(u16*)(mgr + 0x4196C);

    for (int i = 0; i < 4; i++) {
        func_0203b454(mgr + 0x41862 + i * 0x30, (u8*)&data_02075102 + i * 6, 6);
    }

    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov002_0208275c(void* arg0, void* arg1) {
    u8* mgr = (u8*)arg1;
    s32 paletteData;

    func_ov002_020824a0();
    func_0203b454(&data_020750fc, mgr + 0x41862, 6);

    *(u16*)(mgr + 0x4198A) = ((u8*)&data_02074d10)[0x40B];
    *(u16*)(mgr + 0x4196C) = ((u8*)&data_02074d10)[0x40C];

    for (int i = 0; i < 4; i++) {
        func_0203b258(0xFFFF, (u8*)mgr + 0x41838 + i * 0x30, 0x16);
    }

    paletteData = func_ov031_0210c7c4();
    func_0203b454((void*)paletteData, mgr + 0x4196E, 0x16);
    func_0203b454((void*)paletteData, (u8*)mgr + 0x41838 + (((u8*)&data_02074d10)[0x40A] * 0x30), 0x16);
    func_0203b454((void*)paletteData, mgr + 0x4181C, 0x16);

    *(u8*)(mgr + 0x41832) = ((u8*)&data_02074d10)[0x40A];
    Mem_Free(&gDebugHeap, paletteData);

    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov002_02082878(void* arg0) {
    EasyFade_FadeBothDisplays(3, 0x10, 1);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020828a0(void* arg0) {
    EasyFade_FadeBothDisplays(3, -0x10, 1);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020828c8(void* arg0) {
    if (EasyFade_IsFading() == 0) {
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_020828ec(void* arg0) {
    EasyFade_FadeBothDisplays(3, 0x10, 8);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02082914(void* arg0) {
    EasyFade_FadeBothDisplays(3, -0x10, 8);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208293c(void* arg0) {
    if (EasyFade_IsFading() == 0) {
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_02082960(void* arg0) {
    EasyFade_FadeBothDisplays(3, 0, 8);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02082988(void* arg0) {
    if (EasyFade_IsFading() == 0) {
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_020829ac(void* arg0) {
    EasyFade_FadeBothDisplays(3, 0, 8);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020829d4(void* arg0) {
    if (EasyFade_IsFading() == 0) {
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_020829f8(void* arg0) {
    EasyFade_FadeBothDisplays(3, 0, 0x28);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02082a20(void* arg0) {
    if (EasyFade_IsFading() == 0) {
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

void func_ov002_02082a44(void) {
    u32 state = func_ov040_0209cb78();

    switch (state) {
        case 0:
            *((u32*)&SystemStatusFlags) |= 0x80;
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 2:
            func_ov040_0209c158();
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 7:
            func_ov040_0209cde4();
            break;
        default:
            break;
    }
}

void func_ov002_02082ab4(void* arg0) {
    u8* mgr = (u8*)arg0;
    u16 temp_values[4];
    u16 i;

    temp_values[0] = data_ov002_02091aac.unk8[0];
    temp_values[1] = data_ov002_02091aac.unk8[1];
    temp_values[2] = data_ov002_02091aac.unk8[2];
    temp_values[3] = data_ov002_02091aac.unk8[3];

    func_0203b258(0, mgr + 0x41FF4, 0x2800, temp_values[0], temp_values[1], temp_values[2], temp_values[3]);
    func_0203b258(0, data_ov002_020934e4, 0x16);

    for (i = 0; i < 4; i++) {
        if ((*(u16*)(mgr + 0x41834) & (1 << i)) == 0) {
            *(u16*)((u8*)&data_ov002_020934e0 + 4) = 0xFFFF;
            continue;
        }

        func_ov031_0210b5fc(mgr + 0x45FF4, mgr + 0x41838 + i * 0x30);
        func_ov031_0210ab28(mgr + 0x45FF4, 0, temp_values[i]);
        func_ov031_0210ab3c(mgr + 0x45FF4, 0, 0x100);
        func_ov031_0210ab48(mgr + 0x45FF4, 3, 0x50);
        func_ov031_0210bd58(mgr + 0x45FF4, mgr + 0x41FF4, 0x20, 0xA);
    }

    func_0203abec(3, mgr + 0x41FF4, (u8*)G2_GetBG0CharPtr() + 0x2400, 0x2800);
}

s8 func_ov002_02082bec(void* arg0) {
    u8  keys[6];
    u8* mgr         = (u8*)arg0;
    u8  match_index = 0xFF;
    u8  i;
    u16 flags;

    func_0203a96c(keys);
    if (*(u32*)(mgr + 0x462EC) != 0) {
        return 0;
    }

    flags = *(u16*)(mgr + 0x41834);
    for (i = 0; i < 4; i++) {
        u8* entry;

        if ((flags & (1 << i)) == 0) {
            continue;
        }

        entry = mgr + 0x41862 + i * 0x30;
        if (keys[0] != entry[0] || keys[1] != entry[1] || keys[2] != entry[2] || keys[3] != entry[3] || keys[4] != entry[4] ||
            keys[5] != entry[5])
        {
            continue;
        }
        match_index = i;
        break;
    }

    if (match_index == 0xFF) {
        OS_WaitForever();
    }
    return (s8)match_index;
}

void func_ov002_02082d44(void* arg0) {
    u8* mgr         = (u8*)arg0;
    u16 write_index = 0;
    u16 i;

    for (i = 0; i < 4; i++) {
        if ((*(u16*)(mgr + 0x41834) & (1 << i)) == 0) {
            continue;
        }
        func_0203b454(mgr + 0x41FB6 + (i * 6), mgr + 0x41862 + (write_index * 0x30), 6);
        write_index++;
    }
}

void func_ov002_02082dbc(void* arg0, const Ov002_U16_5* arg1, void* arg2, void* arg3) {
    const Ov002_U16_5* entry = arg1;

    if (entry->unk0 == 0xFFFF) {
        return;
    }
    do {
        func_ov031_0210b630(arg0, entry->unk0);
        func_ov031_0210ab28(arg0, entry->unk2, entry->unk4);
        func_ov031_0210ab3c(arg0, 0, (u16)(entry->unk6 - entry->unk2));
        func_ov031_0210ab48(arg0, 0, (u16)(entry->unk8 - entry->unk4));
        func_ov031_0210be18(arg0, arg3, arg2, 0);
        entry++;
    } while (entry->unk0 != 0xFFFF);
}

void func_ov002_02082e70(void* arg0, s32* arg1, void* arg2, void* arg3) {
    s32* entry = arg1;
    u16  index = 0;

    if (*entry == 0) {
        return;
    }
    do {
        func_ov031_0210b5fc(arg0, *entry);
        func_ov031_0210ab28(arg0, 0x48, (u16)((index + 1) * 0x28));
        func_ov031_0210ab3c(arg0, 0, 0x70);
        func_ov031_0210ab48(arg0, 0, 0x10);
        func_ov031_0210be18(arg0, arg3, arg2, 0);
        index++;
        entry++;
    } while (*entry != 0);
}

void func_ov002_02082f18(void* arg0, s32 arg1, s32 arg2, void* arg3) {
    u8*             mgr = (u8*)arg0;
    Ov002_ResBlock* res = Ov002_GetResBlock(arg0);
    void*           pack;
    u8*             ptr;
    u8*             table_entry;

    func_ov002_02085710(arg0);
    g_DisplaySettings.unk2B8 = 0;
    g_DisplaySettings.unk2F4 = 1;
    g_DisplaySettings.unk330 = 2;
    g_DisplaySettings.unk36C = 3;

    pack                     = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    *(void**)(mgr + 0x462F0) = pack;

    g_DisplaySettings.unk1C = 0x1D;
    g_DisplaySettings.unk4C = 0x1C;

    ptr         = Ov002_GetPackOffset(pack, 0x20);
    res->unk340 = func_0200adf8(data_0206b3cc.unk4, ptr, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, res->unk340);

    ptr         = Ov002_GetPackOffset(pack, 0x50);
    res->unk32C = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, ptr, g_DisplaySettings.unk360, 0, 0x6000);

    ptr         = Ov002_GetPackOffset(pack, 0x68);
    res->unk4A0 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk3C0, 1, 3, 0, &res->unk4A0, 1, 1);

    ptr         = Ov002_GetPackOffset(pack, 0x58);
    res->unk328 = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, ptr, g_DisplaySettings.unk324, 0, 0x4D20);

    ptr         = Ov002_GetPackOffset(pack, 0x70);
    res->unk498 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk398, 1, 2, 0, &res->unk498, 1, 1);

    ptr         = Ov002_GetPackOffset(pack, 0x18);
    res->unk344 = func_0200adf8(data_0206b3cc.unk0, ptr, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk0, res->unk344);

    ptr         = Ov002_GetPackOffset(pack, 0x80);
    res->unk33C = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, ptr, g_DisplaySettings.unk140, 0, 0x6000);

    ptr         = Ov002_GetPackOffset(pack, 0x98);
    res->unk4C0 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk460, 0, 3, 0, &res->unk4C0, 1, 1);

    ptr         = Ov002_GetPackOffset(pack, 0x88);
    res->unk338 = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, ptr, g_DisplaySettings.unk104, 0, 0x8000);

    table_entry = Ov002_GetPackTableEntry(pack, arg2);
    res->unk4B8 = table_entry != NULL ? table_entry + 4 : NULL;
    func_0200d1d8(&res->unk438, 0, 2, 0, &res->unk4B8, 1, 1);

    if (arg1 != 0xFFFF) {
        table_entry = Ov002_GetPackTableEntry(pack, arg1);
        res->unk4B0 = table_entry != NULL ? table_entry + 4 : NULL;
        func_0200d1d8(&res->unk410, 0, 1, 0, &res->unk4B0, 1, 1);
        g_DisplaySettings.unk1C |= 2;
    } else {
        g_DisplaySettings.unk1C &= ~2;
    }

    ptr         = Ov002_GetPackOffset(pack, 0x38);
    table_entry = Ov002_GetPackOffset(pack, 0x40);
    res->unk4A8 = table_entry != NULL ? table_entry + 4 : NULL;

    func_ov031_0210ab34(mgr + 0x45FF4, 1);
    func_ov002_02082dbc(mgr + 0x45FF4, (const Ov002_U16_5*)arg3, ptr != NULL ? ptr + 4 : NULL, res->unk4A8);

    res->unk330 = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, ptr, g_DisplaySettings.unk8C, 0, 0x6000);
    func_0200d1d8(&res->unk3E8, 0, 0, 0, &res->unk4A8, 1, 1);
    Display_Commit();
}

void func_ov002_02083484(void* arg0, u16* arg1) {
    u8*             mgr = (u8*)arg0;
    Ov002_ResBlock* res = Ov002_GetResBlock(arg0);
    void*           pack_main;
    void*           pack_sub;
    u8*             ptr;
    u8*             table_ptr;

    g_DisplaySettings.unk2B8 = 0;
    g_DisplaySettings.unk2F4 = 1;
    g_DisplaySettings.unk330 = 2;
    g_DisplaySettings.unk36C = 3;

    pack_main                = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    *(void**)(mgr + 0x462F0) = pack_main;

    pack_sub                 = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 5, 0);
    *(void**)(mgr + 0x462F4) = pack_sub;

    g_DisplaySettings.unk1C = 0;
    g_DisplaySettings.unk4C = 2;

    ptr         = Ov002_GetPackOffset(pack_sub, 0x30);
    res->unk340 = func_0200adf8(data_0206b3cc.unk4, ptr, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, res->unk340);

    table_ptr   = Ov002_GetPackOffset(pack_main, 0x28);
    ptr         = Ov002_GetPackOffset(pack_main, 0x30);
    res->unk490 = ptr != NULL ? ptr + 4 : NULL;

    func_ov031_0210ab34(mgr + 0x45FF4, 1);
    func_ov031_0210ab60(mgr + 0x45FF4, 4);
    func_ov031_0210ab54(mgr + 0x45FF4, 1, 0);
    func_ov002_02082dbc(mgr + 0x45FF4, (const Ov002_U16_5*)arg1, table_ptr != NULL ? table_ptr + 4 : NULL, res->unk490);
    func_ov031_0210ab60(mgr + 0x45FF4, 2);
    func_ov031_0210ab54(mgr + 0x45FF4, 1, 0);

    res->unk324 = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, table_ptr, g_DisplaySettings.unk2E8, 0, 0x6000);
    func_0200d1d8(&res->unk370, 1, 1, 0, &res->unk490, 1, 1);
    Display_Commit();
}

void func_ov002_02083694(void* arg0) {
    u8*             mgr = (u8*)arg0;
    Ov002_ResBlock* res = Ov002_GetResBlock(arg0);
    void*           pack_main;
    void*           pack_sub;
    u8*             ptr;

    func_ov002_02085710(arg0);

    pack_main                = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    *(void**)(mgr + 0x462F0) = pack_main;

    pack_sub                 = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 2, 0);
    *(void**)(mgr + 0x462F4) = pack_sub;

    ptr         = Ov002_GetPackOffset(pack_sub, 0x08);
    res->unk340 = func_0200adf8(data_0206b3cc.unk4, ptr, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, res->unk340);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x20);
    res->unk32C = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, ptr, g_DisplaySettings.unk360, 0, 0x5800);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x28);
    res->unk4A0 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk3C0, 1, 3, 0, &res->unk4A0, 1, 1);
    ptr         = Ov002_GetPackOffset(pack_sub, 0x10);
    res->unk328 = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, ptr, g_DisplaySettings.unk324, 0, 0x8000);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x18);
    res->unk498 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk398, 1, 2, 0, &res->unk498, 1, 1);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x30);
    res->unk344 = func_0200adf8(data_0206b3cc.unk0, ptr, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk0, res->unk344);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x48);
    res->unk33C = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, ptr, g_DisplaySettings.unk140, 0, 0x6000);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x50);
    res->unk4C0 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk460, 0, 3, 0, &res->unk4C0, 1, 1);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x38);
    res->unk338 = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, ptr, g_DisplaySettings.unk104, 0, 0x8000);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x40);
    res->unk4B8 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk438, 0, 2, 0, &res->unk4B8, 1, 1);

    g_DisplaySettings.unk1C = 0x1C;
    g_DisplaySettings.unk4C = 0x1C;
    Display_Commit();
}

void func_ov002_02083a74(void* arg0) {
    u8*   mgr = (u8*)arg0;
    s32   temp_r0_11;
    s32   temp_r0_13;
    s32   temp_r0_21;
    s32   temp_r0_25;
    s32   temp_r0_7;
    s32   temp_r2_6;
    u8*   temp_r3;
    u8*   temp_r3_2;
    u8*   temp_r3_3;
    u8*   temp_r3_4;
    u8*   temp_r3_5;
    s32   temp_r4;
    u32   temp_r0_28;
    u32   temp_r1_10;
    u32   temp_r1_12;
    u32   temp_r1_19;
    u32   temp_r1_22;
    u32   temp_r1_7;
    void* temp_r0;
    void* temp_r0_10;
    void* temp_r0_12;
    void* temp_r0_14;
    void* temp_r0_15;
    void* temp_r0_16;
    void* temp_r0_17;
    void* temp_r0_18;
    void* temp_r0_19;
    void* temp_r0_20;
    void* temp_r0_22;
    void* temp_r0_23;
    void* temp_r0_24;
    void* temp_r0_26;
    void* temp_r0_27;
    void* temp_r0_2;
    void* temp_r0_3;
    void* temp_r0_4;
    void* temp_r0_5;
    void* temp_r0_6;
    void* temp_r0_8;
    void* temp_r0_9;
    void* temp_r1;
    void* temp_r1_11;
    void* temp_r1_13;
    void* temp_r1_14;
    void* temp_r1_15;
    void* temp_r1_16;
    void* temp_r1_17;
    void* temp_r1_18;
    void* temp_r1_20;
    void* temp_r1_21;
    void* temp_r1_23;
    void* temp_r1_2;
    void* temp_r1_3;
    void* temp_r1_4;
    void* temp_r1_5;
    void* temp_r1_6;
    void* temp_r1_8;
    void* temp_r1_9;
    void* temp_r2;
    void* temp_r2_2;
    void* temp_r2_3;
    void* temp_r2_4;
    void* temp_r2_5;
    void* temp_r2_7;
    void* var_r0;
    void* var_r0_10;
    void* var_r0_11;
    void* var_r0_2;
    void* var_r0_3;
    void* var_r0_4;
    void* var_r0_5;
    void* var_r0_6;
    void* var_r0_7;
    void* var_r0_8;
    void* var_r0_9;
    void* var_r1;
    void* var_r1_10;
    void* var_r1_2;
    void* var_r1_3;
    void* var_r1_4;
    void* var_r1_5;
    void* var_r1_6;
    void* var_r1_7;
    void* var_r1_8;
    void* var_r1_9;

    func_ov002_02085710(arg0);
    OVMGR_PTR(mgr, 0x462F0) = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    temp_r0                 = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 3, 0);
    OVMGR_PTR(mgr, 0x462F4) = temp_r0;
    if (temp_r0 == NULL) {
        var_r1 = NULL;
    } else {
        temp_r1 = OVPK_HDR(temp_r0);
        var_r1  = temp_r1 + OVPK_OFF(temp_r1, 0x8);
    }
    OVMGR_PTR(mgr, 0x47340) = func_0200adf8(data_0206b3cc.unk4, var_r1, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, OVMGR_PTR(mgr, 0x47340));
    temp_r0_2 = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r0_2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1_2 = OVPK_HDR(temp_r0_2);
        var_r1_2  = temp_r1_2 + OVPK_OFF(temp_r1_2, 0x48);
    }
    OVMGR_PTR(mgr, 0x4732C) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_2, g_DisplaySettings.unk360, 0, 0x6000);
    temp_r0_3               = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r0_3 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_3 = OVPK_HDR(temp_r0_3);
        var_r0    = temp_r1_3 + OVPK_OFF(temp_r1_3, 0x60);
    }
    OVMGR_PTR(mgr, 0x474A0) = (void*)(var_r0 + 4);
    func_0200d1d8(mgr + 0x473C0, 1, 3, 0, mgr + 0x474A0, 1, 1);
    temp_r0_4 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_4 == NULL) {
        var_r1_3 = NULL;
    } else {
        temp_r1_4 = OVPK_HDR(temp_r0_4);
        var_r1_3  = temp_r1_4 + OVPK_OFF(temp_r1_4, 0x18);
    }
    OVMGR_PTR(mgr, 0x47328) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_3, g_DisplaySettings.unk324, 0, 0x8000);
    temp_r0_5               = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_5 == NULL) {
        var_r0_2 = NULL;
    } else {
        temp_r1_5 = OVPK_HDR(temp_r0_5);
        var_r0_2  = temp_r1_5 + OVPK_OFF(temp_r1_5, 0x28);
    }
    OVMGR_PTR(mgr, 0x47498) = (void*)(var_r0_2 + 4);
    temp_r0_6               = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_6 == NULL) {
        var_r0_3 = NULL;
    } else {
        temp_r1_6 = OVPK_HDR(temp_r0_6);
        var_r0_3  = temp_r1_6 + OVPK_OFF(temp_r1_6, 0x60);
    }
    OVMGR_PTR(mgr, 0x4749C) = (void*)(var_r0_3 + 4);
    func_0200d1d8(mgr + 0x47398, 1, 2, 0, mgr + 0x47498, 1, 2);
    func_0200d858(mgr + 0x47398, 0, 0, 0);
    temp_r0_7 = OVMGR_S32(mgr, 0x4739C);
    temp_r3   = (u8*)((OVMGR_S32(mgr, 0x47398) * 0x220) + (s32)(&g_DisplaySettings + 0x64));
    temp_r1_7 = *(u32*)(temp_r3 + (temp_r0_7 * 0x3C));
    switch (temp_r1_7) { /* switch 1 */
        default:         /* switch 1 */
            break;
        case 1:          /* switch 1 */
        case 2:          /* switch 1 */
        case 3:          /* switch 1 */
        case 4:          /* switch 1 */
        case 5:          /* switch 1 */
            *(s32*)(temp_r3 + (temp_r0_7 * 0x2C) + 0x124) = 1;
            break;
    }
    temp_r0_8                      = temp_r3 + (temp_r0_7 * 8);
    *(s32*)((u8*)temp_r0_8 + 0xF0) = 0x200000;
    *(s32*)((u8*)temp_r0_8 + 0xF4) = 0x200000;
    temp_r0_9                      = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r0_9 == NULL) {
        var_r1_4 = NULL;
    } else {
        temp_r1_8 = OVPK_HDR(temp_r0_9);
        var_r1_4  = temp_r1_8 + OVPK_OFF(temp_r1_8, 0x58);
    }
    OVMGR_PTR(mgr, 0x47324) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_4, g_DisplaySettings.unk2E8, 0, 0x4D20);
    temp_r0_10              = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r0_10 == NULL) {
        var_r0_4 = NULL;
    } else {
        temp_r1_9 = OVPK_HDR(temp_r0_10);
        var_r0_4  = temp_r1_9 + OVPK_OFF(temp_r1_9, 0x70);
    }
    OVMGR_PTR(mgr, 0x47490) = (void*)(var_r0_4 + 4);
    func_0200d1d8(mgr + 0x47370, 1, 1, 0, mgr + 0x47490, 1, 1);
    func_0200d858(mgr + 0x47370, 0, 0, 0);
    temp_r0_11 = OVMGR_S32(mgr, 0x47374);
    temp_r3_2  = (u8*)((OVMGR_S32(mgr, 0x47370) * 0x220) + (s32)(&g_DisplaySettings + 0x64));
    temp_r1_10 = *(u32*)(temp_r3_2 + (temp_r0_11 * 0x3C));
    switch (temp_r1_10) { /* switch 2 */
        default:          /* switch 2 */
            break;
        case 1:           /* switch 2 */
        case 2:           /* switch 2 */
        case 3:           /* switch 2 */
        case 4:           /* switch 2 */
        case 5:           /* switch 2 */
            *(s32*)(temp_r3_2 + (temp_r0_11 * 0x2C) + 0x124) = 1;
            break;
    }
    temp_r0_12                      = temp_r3_2 + (temp_r0_11 * 8);
    *(s32*)((u8*)temp_r0_12 + 0xF0) = 0x200000;
    *(s32*)((u8*)temp_r0_12 + 0xF4) = 0x200000;
    temp_r2                         = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r2 == NULL) {
        var_r1_5 = NULL;
    } else {
        temp_r1_11 = OVPK_HDR(temp_r2);
        var_r1_5   = temp_r1_11 + OVPK_OFF(temp_r1_11, 0x10);
    }
    if (temp_r2 == NULL) {
        var_r0_5 = NULL;
    } else {
        temp_r2_2 = OVPK_HDR(temp_r2);
        var_r0_5  = temp_r2_2 + OVPK_OFF(temp_r2_2, 0x20);
    }
    OVMGR_PTR(mgr, 0x47488) = (void*)(var_r0_5 + 4);
    OVMGR_PTR(mgr, 0x47320) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_5, g_DisplaySettings.unk2AC, 0, 0x11A0);
    func_0200d1d8(mgr + 0x47348, 1, 0, 0, mgr + 0x47488, 1, 1);
    func_0200d858(mgr + 0x47348, 0, 0, 0);
    temp_r0_13 = OVMGR_S32(mgr, 0x4734C);
    temp_r3_3  = (u8*)((OVMGR_S32(mgr, 0x47348) * 0x220) + (s32)(&g_DisplaySettings + 0x64));
    temp_r1_12 = *(u32*)(temp_r3_3 + (temp_r0_13 * 0x3C));
    switch (temp_r1_12) { /* switch 3 */
        default:          /* switch 3 */
            break;
        case 1:           /* switch 3 */
        case 2:           /* switch 3 */
        case 3:           /* switch 3 */
        case 4:           /* switch 3 */
        case 5:           /* switch 3 */
            *(s32*)(temp_r3_3 + (temp_r0_13 * 0x2C) + 0x124) = 1;
            break;
    }
    temp_r0_14                      = temp_r3_3 + (temp_r0_13 * 8);
    *(s32*)((u8*)temp_r0_14 + 0xF0) = 0x200000;
    *(s32*)((u8*)temp_r0_14 + 0xF4) = 0x200000;
    temp_r0_15                      = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_15 == NULL) {
        var_r1_6 = NULL;
    } else {
        temp_r1_13 = OVPK_HDR(temp_r0_15);
        var_r1_6   = temp_r1_13 + OVPK_OFF(temp_r1_13, 0x30);
    }
    OVMGR_PTR(mgr, 0x47344) = func_0200adf8(data_0206b3cc.unk0, var_r1_6, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk0, OVMGR_PTR(mgr, 0x47344));
    temp_r0_16 = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r0_16 == NULL) {
        var_r1_7 = NULL;
    } else {
        temp_r1_14 = OVPK_HDR(temp_r0_16);
        var_r1_7   = temp_r1_14 + OVPK_OFF(temp_r1_14, 0x78);
    }
    OVMGR_PTR(mgr, 0x4733C) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_7, g_DisplaySettings.unk140, 0, 0x6000);
    temp_r0_17              = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r0_17 == NULL) {
        var_r0_6 = NULL;
    } else {
        temp_r1_15 = OVPK_HDR(temp_r0_17);
        var_r0_6   = temp_r1_15 + OVPK_OFF(temp_r1_15, 0x90);
    }
    OVMGR_PTR(mgr, 0x474C0) = (void*)(var_r0_6 + 4);
    func_0200d1d8(mgr + 0x47460, 0, 3, 0, mgr + 0x474C0, 1, 1);
    temp_r0_18 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_18 == NULL) {
        var_r1_8 = NULL;
    } else {
        temp_r1_16 = OVPK_HDR(temp_r0_18);
        var_r1_8   = temp_r1_16 + OVPK_OFF(temp_r1_16, 0x18);
    }
    OVMGR_PTR(mgr, 0x47338) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_8, g_DisplaySettings.unk104, 0, 0x8000);
    temp_r0_19              = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_19 == NULL) {
        var_r0_7 = NULL;
    } else {
        temp_r1_17 = OVPK_HDR(temp_r0_19);
        var_r0_7   = temp_r1_17 + OVPK_OFF(temp_r1_17, 0x28);
    }
    OVMGR_PTR(mgr, 0x474B8) = (void*)(var_r0_7 + 4);
    temp_r0_20              = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_20 == NULL) {
        var_r0_8 = NULL;
    } else {
        temp_r1_18 = OVPK_HDR(temp_r0_20);
        var_r0_8   = temp_r1_18 + OVPK_OFF(temp_r1_18, 0x60);
    }
    OVMGR_PTR(mgr, 0x474BC) = (void*)(var_r0_8 + 4);
    func_0200d1d8(mgr + 0x47438, 0, 2, 0, mgr + 0x474B8, 1, 2);
    func_0200d858(mgr + 0x47438, 0, 0, 0);
    temp_r0_21 = OVMGR_S32(mgr, 0x4743C);
    temp_r3_4  = (u8*)((OVMGR_S32(mgr, 0x47438) * 0x220) + (s32)(&g_DisplaySettings + 0x64));
    temp_r1_19 = *(u32*)(temp_r3_4 + (temp_r0_21 * 0x3C));
    switch (temp_r1_19) { /* switch 4 */
        default:          /* switch 4 */
            break;
        case 1:           /* switch 4 */
        case 2:           /* switch 4 */
        case 3:           /* switch 4 */
        case 4:           /* switch 4 */
        case 5:           /* switch 4 */
            *(s32*)(temp_r3_4 + (temp_r0_21 * 0x2C) + 0x124) = 1;
            break;
    }
    temp_r0_22                      = temp_r3_4 + (temp_r0_21 * 8);
    *(s32*)((u8*)temp_r0_22 + 0xF0) = 0x200000;
    *(s32*)((u8*)temp_r0_22 + 0xF4) = 0x200000;
    temp_r0_23                      = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_23 == NULL) {
        var_r1_9 = NULL;
    } else {
        temp_r1_20 = OVPK_HDR(temp_r0_23);
        var_r1_9   = temp_r1_20 + OVPK_OFF(temp_r1_20, 0x40);
    }
    OVMGR_PTR(mgr, 0x47334) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_9, g_DisplaySettings.unkC8, 0, 0x3EA0);
    temp_r0_24              = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_24 == NULL) {
        var_r0_9 = NULL;
    } else {
        temp_r1_21 = OVPK_HDR(temp_r0_24);
        var_r0_9   = temp_r1_21 + OVPK_OFF(temp_r1_21, 0x58);
    }
    OVMGR_PTR(mgr, 0x474B0) = (void*)(var_r0_9 + 4);
    func_0200d1d8(mgr + 0x47410, 0, 1, 0, mgr + 0x474B0, 1, 1);
    func_0200d858(mgr + 0x47410, 0, 0, 0);
    temp_r0_25 = OVMGR_S32(mgr, 0x47414);
    temp_r3_5  = (u8*)((OVMGR_S32(mgr, 0x47410) * 0x220) + (s32)(&g_DisplaySettings + 0x64));
    temp_r1_22 = *(u32*)(temp_r3_5 + (temp_r0_25 * 0x3C));
    switch (temp_r1_22) { /* switch 5 */
        default:          /* switch 5 */
            break;
        case 1:           /* switch 5 */
        case 2:           /* switch 5 */
        case 3:           /* switch 5 */
        case 4:           /* switch 5 */
        case 5:           /* switch 5 */
            *(s32*)(temp_r3_5 + (temp_r0_25 * 0x2C) + 0x124) = 1;
            break;
    }
    temp_r0_26                      = temp_r3_5 + (temp_r0_25 * 8);
    *(s32*)((u8*)temp_r0_26 + 0xF0) = 0x200000;
    *(s32*)((u8*)temp_r0_26 + 0xF4) = 0x200000;
    temp_r2_3                       = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r2_3 == NULL) {
        var_r1_10 = NULL;
    } else {
        temp_r1_23 = OVPK_HDR(temp_r2_3);
        var_r1_10  = temp_r1_23 + OVPK_OFF(temp_r1_23, 0x38);
    }
    if (temp_r2_3 == NULL) {
        var_r0_10 = NULL;
    } else {
        temp_r2_4 = OVPK_HDR(temp_r2_3);
        var_r0_10 = temp_r2_4 + OVPK_OFF(temp_r2_4, 0x48);
    }
    OVMGR_PTR(mgr, 0x474A8) = (void*)(var_r0_10 + 4);
    temp_r0_27              = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_27 == NULL) {
        var_r0_11 = NULL;
    } else {
        temp_r2_5 = OVPK_HDR(temp_r0_27);
        var_r0_11 = temp_r2_5 + OVPK_OFF(temp_r2_5, 0x50);
    }
    OVMGR_PTR(mgr, 0x474AC) = (void*)(var_r0_11 + 4);
    OVMGR_PTR(mgr, 0x47330) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_10, g_DisplaySettings.unk8C, 0, 0x8000);
    func_0200d1d8(mgr + 0x473E8, 0, 0, 0, mgr + 0x474A8, 2, 1);
    func_0200d858(mgr + 0x473E8, 0, 0, 0);
    temp_r4    = OVMGR_S32(mgr, 0x473EC);
    temp_r2_6  = (OVMGR_S32(mgr, 0x473E8) * 0x220) + (s32)(&g_DisplaySettings + 0x64);
    temp_r0_28 = *(u32*)((u8*)temp_r2_6 + (temp_r4 * 0x3C));
    switch (temp_r0_28) { /* switch 6 */
        default:          /* switch 6 */
            break;
        case 1:           /* switch 6 */
        case 2:           /* switch 6 */
        case 3:           /* switch 6 */
        case 4:           /* switch 6 */
        case 5:           /* switch 6 */
            *(s32*)((u8*)temp_r2_6 + (temp_r4 * 0x2C) + 0x124) = 1;
            break;
    }
    temp_r2_7                      = (u8*)temp_r2_6 + (temp_r4 * 8);
    *(s32*)((u8*)temp_r2_7 + 0xF0) = 0x200000;
    *(s32*)((u8*)temp_r2_7 + 0xF4) = 0x200000;
    g_DisplaySettings.unk1C        = 0x1F;
    g_DisplaySettings.unk4C        = 0x1F;
    Display_Commit();
}

void func_ov002_02084494(void* arg0, u8 arg1, u16* arg2) {
    u8*        mgr = (u8*)arg0;
    u16        sp4C[0x32];
    u16        sp24[0x14];
    u16        sp22;
    u16        sp20;
    u16        sp1E;
    u16        sp1C;
    u16        sp1A;
    u16        sp18;
    u16        sp16;
    u16        sp14;
    u16        sp12;
    u16        sp10;
    u16        spE;
    u16        spC;
    const u16* var_ip;
    const u16* var_ip_2;
    u8*        temp_r1_10;
    u8*        temp_r1_12;
    u8*        temp_r1_4;
    s32        var_r0_3;
    s32        var_r2;
    s32        var_r2_2;
    u16*       var_r3;
    u16*       var_r3_2;
    u16        temp_r2;
    u16        temp_r2_2;
    u16        temp_r2_3;
    void*      temp_r0_10;
    void*      temp_r0_11;
    void*      temp_r0_12;
    void*      temp_r0_13;
    void*      temp_r0_14;
    void*      temp_r0_3;
    void*      temp_r0_4;
    void*      temp_r0_5;
    void*      temp_r0_6;
    void*      temp_r0_7;
    void*      temp_r0_8;
    void*      temp_r0_9;
    void*      temp_r1_11;
    void*      temp_r1_13;
    void*      temp_r1_14;
    void*      temp_r1_15;
    void*      temp_r1_16;
    void*      temp_r1_17;
    void*      temp_r1_18;
    void*      temp_r1_19;
    void*      temp_r1_3;
    void*      temp_r1_5;
    void*      temp_r1_6;
    void*      temp_r1_7;
    void*      temp_r1_8;
    void*      temp_r1_9;
    void*      temp_r2_4;
    void*      temp_r2_5;
    void*      temp_r2_6;
    void*      var_r0;
    void*      var_r0_2;
    void*      var_r0_4;
    void*      var_r0_5;
    void*      var_r0_6;
    void*      var_r1;
    void*      var_r1_10;
    void*      var_r1_2;
    void*      var_r1_3;
    void*      var_r1_4;
    void*      var_r1_5;
    void*      var_r1_6;
    void*      var_r1_7;
    void*      var_r1_8;
    void*      var_r1_9;
    void*      var_r4;

    var_ip = data_ov002_02091b64;
    var_r3 = sp24;
    var_r2 = 0xA;
    do {
        *var_r3++ = *var_ip++;
        *var_r3++ = *var_ip++;
        var_r2 -= 1;
    } while (var_r2 != 0);
    var_ip_2 = data_ov002_02091bb8;
    var_r3_2 = sp4C;
    var_r2_2 = 0x19;
    do {
        *var_r3_2++ = *var_ip_2++;
        *var_r3_2++ = *var_ip_2++;
        var_r2_2 -= 1;
    } while (var_r2_2 != 0);
    func_ov002_02085710(arg0);
    g_DisplaySettings.unk2B8 = 3;
    g_DisplaySettings.unk2F4 = 0;
    g_DisplaySettings.unk330 = 1;
    g_DisplaySettings.unk36C = 2;
    OVMGR_PTR(mgr, 0x462F0)  = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    OVMGR_PTR(mgr, 0x462F4)  = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 4, 0);
    temp_r1_3                = OVMGR_PTR(mgr, 0x462F4);
    sp1C                     = data_ov002_02091aac.unk10[0];
    sp1E                     = data_ov002_02091aac.unk10[1];
    sp22                     = data_ov002_02091aac.unk10[3];
    sp20                     = data_ov002_02091aac.unk10[2];
    temp_r2                  = (&sp1C)[arg1];
    if ((temp_r1_3 == NULL) || ((s32)temp_r2 <= 0)) {
        var_r1 = NULL;
    } else {
        temp_r1_4 = OVPK_HDR(temp_r1_3);
        var_r1    = temp_r1_4 + *(u32*)(temp_r1_4 + (temp_r2 * 8));
    }
    OVMGR_PTR(mgr, 0x47340) = func_0200adf8(data_0206b3cc.unk4, var_r1, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, OVMGR_PTR(mgr, 0x47340));
    temp_r0_3 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_3 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1_5 = OVPK_HDR(temp_r0_3);
        var_r1_2  = temp_r1_5 + OVPK_OFF(temp_r1_5, 0x30);
    }
    OVMGR_PTR(mgr, 0x4732C) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_2, g_DisplaySettings.unk360, 0, 0x8000);
    temp_r0_4               = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_4 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_6 = OVPK_HDR(temp_r0_4);
        var_r0    = temp_r1_6 + OVPK_OFF(temp_r1_6, 0x50);
    }
    OVMGR_PTR(mgr, 0x474A0) = (void*)(var_r0 + 4);
    func_0200d1d8(mgr + 0x473C0, 1, 3, 0, mgr + 0x474A0, 1, 1);
    temp_r0_5 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_5 == NULL) {
        var_r1_3 = NULL;
    } else {
        temp_r1_7 = OVPK_HDR(temp_r0_5);
        var_r1_3  = temp_r1_7 + OVPK_OFF(temp_r1_7, 0x38);
    }
    OVMGR_PTR(mgr, 0x47328) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_3, g_DisplaySettings.unk324, 0, 0x6000);
    temp_r0_6               = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_6 == NULL) {
        var_r0_2 = NULL;
    } else {
        temp_r1_8 = OVPK_HDR(temp_r0_6);
        var_r0_2  = temp_r1_8 + OVPK_OFF(temp_r1_8, 0x58);
    }
    OVMGR_PTR(mgr, 0x47498) = (void*)(var_r0_2 + 4);
    func_0200d1d8(mgr + 0x47398, 1, 2, 0, mgr + 0x47498, 1, 1);
    temp_r0_7 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_7 == NULL) {
        var_r1_4 = NULL;
    } else {
        temp_r1_9 = OVPK_HDR(temp_r0_7);
        var_r1_4  = temp_r1_9 + OVPK_OFF(temp_r1_9, 0x48);
    }
    OVMGR_PTR(mgr, 0x47324) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_4, g_DisplaySettings.unk2E8, 0, 0x8000);
    temp_r0_8               = OVMGR_PTR(mgr, 0x462F4);
    sp1A                    = data_ov002_02091aa4[3];
    sp14                    = data_ov002_02091aa4[0];
    sp16                    = data_ov002_02091aa4[1];
    sp18                    = data_ov002_02091aa4[2];
    temp_r2_2               = (&sp14)[arg1];
    if ((temp_r0_8 == NULL) || ((s32)temp_r2_2 <= 0)) {
        var_r0_3 = 0;
    } else {
        temp_r1_10 = OVPK_HDR(temp_r0_8);
        var_r0_3   = (s32)(temp_r1_10 + *(u32*)(temp_r1_10 + (temp_r2_2 * 8)));
    }
    OVMGR_S32(mgr, 0x47490) = (s32)(var_r0_3 + 4);
    func_0200d1d8(mgr + 0x47370, 1, 1, 0, mgr + 0x47490, 1, 1);
    temp_r0_9 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_9 == NULL) {
        var_r1_5 = NULL;
    } else {
        temp_r1_11 = OVPK_HDR(temp_r0_9);
        var_r1_5   = temp_r1_11 + OVPK_OFF(temp_r1_11, 0x40);
    }
    OVMGR_PTR(mgr, 0x47320) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_5, g_DisplaySettings.unk2AC, 0, 0x200);
    g_DisplaySettings.unk4C = 0x1F;
    temp_r0_10              = OVMGR_PTR(mgr, 0x462F4);
    sp12                    = data_ov002_02091aac.unk10[7];
    spC                     = data_ov002_02091aac.unk10[4];
    spE                     = data_ov002_02091aac.unk10[5];
    sp10                    = data_ov002_02091aac.unk10[6];
    temp_r2_3               = (&spC)[arg1];
    if ((temp_r0_10 == NULL) || ((s32)temp_r2_3 <= 0)) {
        var_r1_6 = NULL;
    } else {
        temp_r1_12 = OVPK_HDR(temp_r0_10);
        var_r1_6   = temp_r1_12 + *(u32*)(temp_r1_12 + (temp_r2_3 * 8));
    }
    OVMGR_PTR(mgr, 0x47344) = func_0200adf8(data_0206b3cc.unk0, var_r1_6, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk0, OVMGR_PTR(mgr, 0x47344));
    temp_r0_11 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_11 == NULL) {
        var_r1_7 = NULL;
    } else {
        temp_r1_13 = OVPK_HDR(temp_r0_11);
        var_r1_7   = temp_r1_13 + OVPK_OFF(temp_r1_13, 0xA0);
    }
    OVMGR_PTR(mgr, 0x4733C) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_7, g_DisplaySettings.unk140, 0, 0x6000);
    temp_r0_12              = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_12 == NULL) {
        var_r0_4 = NULL;
    } else {
        temp_r1_14 = OVPK_HDR(temp_r0_12);
        var_r0_4   = temp_r1_14 + OVPK_OFF(temp_r1_14, 0xA8);
    }
    OVMGR_PTR(mgr, 0x474C0) = (void*)(var_r0_4 + 4);
    func_0200d1d8(mgr + 0x47460, 0, 3, 0, mgr + 0x474C0, 1, 1);
    temp_r0_13 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_13 == NULL) {
        var_r1_8 = NULL;
    } else {
        temp_r1_15 = OVPK_HDR(temp_r0_13);
        var_r1_8   = temp_r1_15 + OVPK_OFF(temp_r1_15, 0x48);
    }
    OVMGR_PTR(mgr, 0x47338) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_8, g_DisplaySettings.unk104, 0, 0x8000);
    temp_r0_14              = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r0_14 == NULL) {
        var_r0_5 = NULL;
    } else {
        temp_r1_16 = OVPK_HDR(temp_r0_14);
        var_r0_5   = temp_r1_16 + OVPK_OFF(temp_r1_16, 0xE0);
    }
    OVMGR_PTR(mgr, 0x474B8) = (void*)(var_r0_5 + 4);
    func_0200d1d8(mgr + 0x47438, 0, 2, 0, mgr + 0x474B8, 1, 1);
    temp_r2_4 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r2_4 == NULL) {
        var_r1_9 = NULL;
    } else {
        temp_r1_17 = OVPK_HDR(temp_r2_4);
        var_r1_9   = temp_r1_17 + OVPK_OFF(temp_r1_17, 0x48);
    }
    if (temp_r2_4 == NULL) {
        var_r0_6 = NULL;
    } else {
        temp_r2_5 = OVPK_HDR(temp_r2_4);
        var_r0_6  = temp_r2_5 + OVPK_OFF(temp_r2_5, 0xB0);
    }
    OVMGR_PTR(mgr, 0x474B0) = (void*)(var_r0_6 + 4);
    OVMGR_PTR(mgr, 0x47334) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_9, g_DisplaySettings.unkC8, 0, 0x8000);
    func_0200d1d8(mgr + 0x47410, 0, 1, 0, mgr + 0x474B0, 1, 1);
    temp_r2_6 = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r2_6 == NULL) {
        var_r4 = NULL;
    } else {
        temp_r1_18 = OVPK_HDR(temp_r2_6);
        var_r4     = temp_r1_18 + OVPK_OFF(temp_r1_18, 0x38);
    }
    if (temp_r2_6 == NULL) {
        var_r1_10 = NULL;
    } else {
        temp_r1_19 = OVPK_HDR(temp_r2_6);
        var_r1_10  = temp_r1_19 + OVPK_OFF(temp_r1_19, 0x40);
    }
    OVMGR_PTR(mgr, 0x474A8) = (void*)(var_r1_10 + 4);
    func_ov031_0210ab34(mgr + 0x45FF4, 1U);
    func_ov002_02082dbc(mgr + 0x45FF4, (const Ov002_U16_5*)sp4C, var_r4 + 4, OVMGR_PTR(mgr, 0x474A8));
    func_ov002_02082dbc(mgr + 0x45FF4, (const Ov002_U16_5*)arg2, var_r4 + 4, OVMGR_PTR(mgr, 0x474A8));
    func_ov031_0210ab34(mgr + 0x45FF4, 3U);
    func_ov002_02082dbc(mgr + 0x45FF4, (const Ov002_U16_5*)sp24, var_r4 + 4, OVMGR_PTR(mgr, 0x474A8));
    func_ov031_0210ab34(mgr + 0x45FF4, 1U);
    OVMGR_PTR(mgr, 0x47330) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r4, g_DisplaySettings.unk8C, 0, 0x6000);
    func_0200d1d8(mgr + 0x473E8, 0, 0, 0, mgr + 0x474A8, 1, 1);
    g_DisplaySettings.unk1C = 0x1F;
    Display_Commit();
}

void func_ov002_02084c84(void* arg0, u16* arg1) {
    u8*             mgr = (u8*)arg0;
    Ov002_ResBlock* res = Ov002_GetResBlock(arg0);
    void*           temp_r0;
    void*           temp_r0_2;
    void*           temp_r0_3;
    void*           temp_r0_4;
    void*           temp_r0_5;
    void*           temp_r0_6;
    void*           temp_r0_7;
    void*           temp_r0_8;
    void*           temp_r1;
    void*           temp_r1_10;
    void*           temp_r1_2;
    void*           temp_r1_3;
    void*           temp_r1_4;
    void*           temp_r1_5;
    void*           temp_r1_6;
    void*           temp_r1_7;
    void*           temp_r1_8;
    void*           temp_r1_9;
    void*           temp_r2;
    void*           var_r0;
    void*           var_r0_2;
    void*           var_r0_3;
    void*           var_r1;
    void*           var_r1_2;
    void*           var_r1_3;
    void*           var_r1_4;
    void*           var_r1_5;
    void*           var_r1_6;
    void*           var_r4;

    func_ov002_02085710(arg0);
    g_DisplaySettings.unk2B8 = 0;
    g_DisplaySettings.unk2F4 = 1;
    g_DisplaySettings.unk330 = 2;
    g_DisplaySettings.unk36C = 3;
    OVMGR_PTR(mgr, 0x462F0)  = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    temp_r0                  = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 5, 0);
    OVMGR_PTR(mgr, 0x462F4)  = temp_r0;
    if (temp_r0 == NULL) {
        var_r1 = NULL;
    } else {
        temp_r1 = OVPK_HDR(temp_r0);
        var_r1  = temp_r1 + OVPK_OFF(temp_r1, 0x8);
    }
    res->unk340 = func_0200adf8(data_0206b3cc.unk4, var_r1, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, res->unk340);
    temp_r0_2 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1_2 = OVPK_HDR(temp_r0_2);
        var_r1_2  = temp_r1_2 + OVPK_OFF(temp_r1_2, 0x10);
    }
    res->unk32C = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_2, g_DisplaySettings.unk360, 0, 0x6000);
    temp_r0_3   = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_3 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_3 = OVPK_HDR(temp_r0_3);
        var_r0    = temp_r1_3 + OVPK_OFF(temp_r1_3, 0x20);
    }
    res->unk4A0 = (void*)(var_r0 + 4);
    func_0200d1d8(&res->unk3C0, 1, 3, 0, &res->unk4A0, 1, 1);
    var_r1_3                = NULL;
    g_DisplaySettings.unk4C = 0x18;
    temp_r0_4               = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_4 != NULL) {
        temp_r1_4 = OVPK_HDR(temp_r0_4);
        var_r1_3  = temp_r1_4 + OVPK_OFF(temp_r1_4, 0x30);
    }
    res->unk344 = func_0200adf8(data_0206b3cc.unk0, var_r1_3, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk0, res->unk344);
    temp_r0_5 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_5 == NULL) {
        var_r1_4 = NULL;
    } else {
        temp_r1_5 = OVPK_HDR(temp_r0_5);
        var_r1_4  = temp_r1_5 + OVPK_OFF(temp_r1_5, 0x38);
    }
    res->unk33C = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_4, g_DisplaySettings.unk140, 0, 0x6000);
    temp_r0_6   = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_6 == NULL) {
        var_r0_2 = NULL;
    } else {
        temp_r1_6 = OVPK_HDR(temp_r0_6);
        var_r0_2  = temp_r1_6 + OVPK_OFF(temp_r1_6, 0x40);
    }
    res->unk4C0 = (void*)(var_r0_2 + 4);
    func_0200d1d8(&res->unk460, 0, 3, 0, &res->unk4C0, 1, 1);
    temp_r0_7 = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_7 == NULL) {
        var_r1_5 = NULL;
    } else {
        temp_r1_7 = OVPK_HDR(temp_r0_7);
        var_r1_5  = temp_r1_7 + OVPK_OFF(temp_r1_7, 0x18);
    }
    res->unk338 = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_5, g_DisplaySettings.unk104, 0, 0x8000);
    temp_r0_8   = OVMGR_PTR(mgr, 0x462F4);
    if (temp_r0_8 == NULL) {
        var_r0_3 = NULL;
    } else {
        temp_r1_8 = OVPK_HDR(temp_r0_8);
        var_r0_3  = temp_r1_8 + OVPK_OFF(temp_r1_8, 0x58);
    }
    res->unk4B8 = (void*)(var_r0_3 + 4);
    func_0200d1d8(&res->unk438, 0, 2, 0, &res->unk4B8, 1, 1);
    temp_r2 = OVMGR_PTR(mgr, 0x462F0);
    if (temp_r2 == NULL) {
        var_r4 = NULL;
    } else {
        temp_r1_9 = OVPK_HDR(temp_r2);
        var_r4    = temp_r1_9 + OVPK_OFF(temp_r1_9, 0x38);
    }
    if (temp_r2 == NULL) {
        var_r1_6 = NULL;
    } else {
        temp_r1_10 = OVPK_HDR(temp_r2);
        var_r1_6   = temp_r1_10 + OVPK_OFF(temp_r1_10, 0x40);
    }
    res->unk4A8 = (void*)(var_r1_6 + 4);
    func_ov031_0210ab34(mgr + 0x45FF4, 1U);
    if (arg1 != NULL) {
        func_ov002_02082dbc(mgr + 0x45FF4, (const Ov002_U16_5*)arg1, var_r4 + 4, res->unk4A8);
    }
    res->unk330 = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r4, g_DisplaySettings.unk8C, 0, 0x6000);
    func_0200d1d8(&res->unk3E8, 0, 0, 0, &res->unk4A8, 1, 1);
    g_DisplaySettings.unk1C = 0x1D;
    Display_Commit();
}

void func_ov002_020850c0(void* arg0, s32 arg1, s32 arg2, s32* arg3, u16* arg4) {
    u8*             mgr = (u8*)arg0;
    Ov002_ResBlock* res = Ov002_GetResBlock(arg0);
    void*           pack_main;
    void*           pack_sub;
    u8*             ptr;
    u8*             table_entry;

    func_ov002_02085710(arg0);

    pack_main                = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    *(void**)(mgr + 0x462F0) = pack_main;
    pack_sub                 = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 5, 0);
    *(void**)(mgr + 0x462F4) = pack_sub;

    ptr         = Ov002_GetPackOffset(pack_sub, 0x8);
    res->unk340 = func_0200adf8(data_0206b3cc.unk4, ptr, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, res->unk340);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x10);
    res->unk32C = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, ptr, g_DisplaySettings.unk360, 0, 0x6000);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x20);
    res->unk4A0 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk3C0, 1, 3, 0, &res->unk4A0, 1, 1);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x18);
    res->unk328 = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, ptr, g_DisplaySettings.unk324, 0, 0x8000);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x28);
    res->unk498 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk398, 1, 2, 0, &res->unk498, 1, 1);

    ptr         = Ov002_GetPackOffset(pack_main, 0x28);
    res->unk324 = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, ptr, g_DisplaySettings.unk2E8, 0, 0x6000);

    table_entry = Ov002_GetPackOffset(pack_main, 0x30);
    res->unk490 = table_entry != NULL ? table_entry + 4 : NULL;
    func_ov031_0210ab34(mgr + 0x45FF4, 1U);
    func_ov002_02082e70(mgr + 0x45FF4, arg3, ptr != NULL ? ptr + 4 : NULL, res->unk490);
    func_0200d1d8(&res->unk370, 1, 1, 0, &res->unk490, 1, 1);

    g_DisplaySettings.unk4C = 0x1E;
    g_DisplaySettings.unk1C = 0;

    ptr         = Ov002_GetPackOffset(pack_sub, 0x30);
    res->unk344 = func_0200adf8(data_0206b3cc.unk0, ptr, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk0, res->unk344);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x38);
    res->unk33C = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, ptr, g_DisplaySettings.unk140, 0, 0x6000);

    ptr         = Ov002_GetPackOffset(pack_sub, 0x40);
    res->unk4C0 = ptr != NULL ? ptr + 4 : NULL;
    func_0200d1d8(&res->unk460, 0, 3, 0, &res->unk4C0, 1, 1);

    table_entry = Ov002_GetPackTableEntry(pack_sub, arg2);
    res->unk4B8 = table_entry != NULL ? table_entry + 4 : NULL;
    func_0200d1d8(&res->unk438, 0, 2, 0, &res->unk4B8, 1, 1);

    if (arg1 != 0xFFFF) {
        ptr         = Ov002_GetPackOffset(pack_sub, 0x18);
        res->unk334 = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, ptr, g_DisplaySettings.unkC8, 0, 0x8000);

        table_entry = Ov002_GetPackTableEntry(pack_sub, arg1);
        res->unk4B0 = table_entry != NULL ? table_entry + 4 : NULL;
        func_0200d1d8(&res->unk410, 0, 1, 0, &res->unk4B0, 1, 1);
        g_DisplaySettings.unk1C |= 2;
    } else if ((res->unk4B0 == NULL) && (res->unk334 == NULL)) {
    } else {
        OS_WaitForever();
    }

    ptr         = Ov002_GetPackOffset(pack_main, 0x38);
    res->unk4A8 = Ov002_GetPackOffset(pack_main, 0x40);
    res->unk4A8 = res->unk4A8 != NULL ? (u8*)res->unk4A8 + 4 : NULL;
    func_ov031_0210ab34(mgr + 0x45FF4, 1U);
    func_ov002_02082dbc(mgr + 0x45FF4, (const Ov002_U16_5*)arg4, ptr != NULL ? ptr + 4 : NULL, res->unk4A8);

    res->unk330 = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, ptr, g_DisplaySettings.unk8C, 0, 0x6000);
    func_0200d1d8(&res->unk3E8, 0, 0, 0, &res->unk4A8, 1, 1);
    g_DisplaySettings.unk1C |= 0x1D;
    Display_Commit();
}

void func_ov002_02085710(void* arg0) {
    u8*             mgr = (u8*)arg0;
    Ov002_ResBlock* res = Ov002_GetResBlock(arg0);
    Data*           data;

    if (res->unk344 != NULL) {
        func_0200afec(data_0206b3cc.unk0);
        res->unk344 = NULL;
    }
    if (res->unk330 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers.unk0);
        res->unk330 = NULL;
    }
    if (res->unk334 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers.unk0);
        res->unk334 = NULL;
    }
    if (res->unk338 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers.unk0);
        res->unk338 = NULL;
    }
    if (res->unk33C != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers.unk0);
        res->unk33C = NULL;
    }
    if (res->unk340 != NULL) {
        func_0200afec(data_0206b3cc.unk4);
        res->unk340 = NULL;
    }
    if (res->unk320 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers.unk4);
        res->unk320 = NULL;
    }
    if (res->unk324 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers.unk4);
        res->unk324 = NULL;
    }
    if (res->unk328 != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers.unk4);
        res->unk328 = NULL;
    }
    if (res->unk32C != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers.unk4);
        res->unk32C = NULL;
    }
    func_0200d954(1, 0);
    func_0200d954(1, 1);
    func_0200d954(1, 2);
    func_0200d954(1, 3);
    func_0200d954(0, 2);
    func_0200d954(0, 3);
    data = (Data*)OVMGR_PTR(mgr, 0x462F0);
    if (data != NULL) {
        DatMgr_ReleaseData(data);
        OVMGR_PTR(mgr, 0x462F0) = NULL;
    }
    data = (Data*)OVMGR_PTR(mgr, 0x462F4);
    if (data != NULL) {
        DatMgr_ReleaseData(data);
        OVMGR_PTR(mgr, 0x462F4) = NULL;
    }
    data = (Data*)OVMGR_PTR(mgr, 0x462F8);
    if (data != NULL) {
        DatMgr_ReleaseData(data);
        OVMGR_PTR(mgr, 0x462F8) = NULL;
    }
    data = (Data*)OVMGR_PTR(mgr, 0x462FC);
    if (data == NULL) {
        return;
    }
    DatMgr_ReleaseData(data);
    OVMGR_PTR(mgr, 0x462FC) = NULL;
}

u16 func_ov002_0208597c(u16* arg0, ...) {
    s32 coords[2];
    s32 x;
    s32 y;
    u16 index  = 0;
    u16 result = 0xFFFF;

    TouchInput_GetCoord(coords);
    x = coords[0];
    y = coords[1];

    if (TouchInput_WasTouchPressed() != 0) {
        result = 0xFFFE;
        if (arg0[0] != 0xFFFF) {
            do {
                u16* entry = arg0 + index * 5;

                if ((x >= (s32)entry[1]) && (x <= (s32)entry[3]) && (y >= (s32)entry[2]) && (y <= (s32)entry[4])) {
                    result = entry[0];
                    break;
                }
                index++;
            } while (arg0[index * 5] != 0xFFFF);
        }
    }
    return result;
}

void func_ov002_02085a44(void* arg0) {
    func_ov002_02082548((Ov002_Manager*)arg0);
    func_ov031_0210cff0(arg0 + 0x41804, arg0 + 0x46100);
    func_ov031_0210c94c(arg0 + 0x46100, &data_ov002_02092bd0, 0x48068);
    func_ov031_0210c9a8(arg0 + 0x46100, arg0);
    func_ov031_0210cc84(arg0 + 0x46100, &data_ov002_02093008, NULL);
    func_ov031_0210ce50(arg0 + 0x41804, arg0 + 0x46100);
}

void func_ov002_02085ac4(void* arg0) {
    Ov002_Manager* pManager = (Ov002_Manager*)arg0;
    s32            result   = (s32)func_ov040_0209cb78();

    if (result == 0 || (result >= 8 && result <= 10)) {
        pManager->unk462E8 = 0;
    } else {
        pManager->unk462E8 = 1;
    }

    void* menuMgr      = pManager->menuMgr;
    void* currentTable = func_ov031_0210cde4(menuMgr);

    if (currentTable != (void*)&data_ov002_02092efc && currentTable != (void*)&data_ov002_02092e68 &&
        currentTable != (void*)&data_ov002_02092ec8 && currentTable != (void*)&data_ov002_02092e98)
    {

        if (result == 10) {
            func_ov031_0210cc84(menuMgr, &data_ov002_02092e40, NULL);
        } else if (result == 8 || result == 9) {
            u8 val = pManager->unk41FE9;
            if (val == 0) {
                func_ov031_0210cc84(menuMgr, &data_ov002_02092e54, NULL);
            } else if (val == 1) {
                func_ov031_0210cc84(menuMgr, &data_ov002_02092e18, NULL);
            } else if (val == 2) {
                func_ov031_0210cc84(menuMgr, &data_ov002_02092e2c, NULL);
            }
        }
    }

    pManager->unk4180C = 1;
}

void* func_ov002_02085c44(void) {
    u8* temp_r0;

    FS_LoadOverlay(0, 0x1F);
    temp_r0 = Mem_AllocHeapTail(&gMainHeap, 0x48068);
    Mem_SetSequence(&gMainHeap, temp_r0, data_ov002_02092be4);
    func_0203b2d0(0, temp_r0, 0x48068);
    MainOvlDisp_SetCbArg(temp_r0);
    Mem_InitializeHeap((MemPool*)(temp_r0 + 0x11590), temp_r0 + 0x1159C, 0x30000);
    *(u32*)(temp_r0 + 0x11584) = DatMgr_AllocateSlot();
    *(u32*)(temp_r0 + 0x1158C) = DatMgr_AllocateSlot();
    *(u32*)(temp_r0 + 0x11588) = DatMgr_AllocateSlot();
    data_ov002_020934e0        = 0;
    *(u32*)(temp_r0 + 0x11580) = func_0200cef0(temp_r0);
    func_ov031_0210aaac(temp_r0 + 0x45FF4, 1, 1);
    func_ov031_0210ab60(temp_r0 + 0x45FF4, 3);
    func_ov031_0210ab54(temp_r0 + 0x45FF4, 1, 0);
    func_ov031_0210ab34(temp_r0 + 0x45FF4, 0xCU);
    func_ov031_0210cdfc(temp_r0 + 0x41804, 0x1E);
    EasyTask_InitializePool((TaskPool*)(temp_r0 + 0x4159C), (MemPool*)(temp_r0 + 0x11590), 0x100, NULL, NULL);
    EasyTask_CreateTask((TaskPool*)(temp_r0 + 0x4159C), &Task_EasyFade, NULL, 0, NULL, NULL);
    func_0200d8f0();
    data_02066aec[0]           = 0;
    data_02066eec[0]           = 0;
    *(s16*)(temp_r0 + 0x46070) = (s16)((u32)(SystemStatusFlags << 0x19) >> 0x1F);
    *(u32*)(temp_r0 + 0x41FF0) = 0;
    *(s32*)(temp_r0 + 0x460BC) = 0;
    return temp_r0;
}

void* func_ov002_02085df8(void) {
    u8* temp_r0;

    OvlMgr_LoadOverlay(3, 0x28);
    temp_r0                    = func_ov002_02085c44();
    *(u32*)(temp_r0 + 0x41FF0) = 1;
    return temp_r0;
}

void func_ov002_02085e20(void* arg0) {
    u8* temp_r0;

    (void)arg0;
    func_ov002_0208b860();
    temp_r0 = func_ov002_02085df8();
    func_ov031_0210c94c(temp_r0 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r0 + 0x4161C, temp_r0);
    func_ov031_0210cc84(temp_r0 + 0x4161C, &data_ov002_02093268, NULL);
    *(u8*)(temp_r0 + 0x41FE9) = 0;
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_02085e8c(void* arg0) {
    u8* temp_r0;

    (void)arg0;
    func_ov002_0208b860();
    temp_r0                    = func_ov002_02085df8();
    *(s32*)(temp_r0 + 0x460BC) = 1;
    func_ov031_0210c94c(temp_r0 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r0 + 0x4161C, temp_r0);
    func_ov031_0210cc84(temp_r0 + 0x4161C, &data_ov002_02093254, NULL);
    *(u8*)(temp_r0 + 0x41FE9) = 0;
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_02085f04(void* arg0) {
    u8* temp_r4;

    (void)arg0;
    func_02004a68(&gMainHeap);
    func_02004a68(&gDebugHeap);
    func_ov002_0208bd40();
    temp_r4 = func_ov002_02085c44();
    func_ov040_0209d990();
    *(u32*)(temp_r4 + 0x41FF0) = 1;
    *(s32*)(temp_r4 + 0x460BC) = 1;
    func_ov031_0210c94c(temp_r4 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r4 + 0x4161C, temp_r4);
    *(u32*)(temp_r4 + 0x462EC) = 0;
    if (data_02074d10.unk40A == 0) {
        func_ov031_0210cc84(temp_r4 + 0x4161C, &data_ov002_02093034);
    } else {
        func_ov031_0210cc84(temp_r4 + 0x4161C, &data_ov002_0209305c);
    }
    func_ov002_02085a44(temp_r4);
    func_0202733c(0x19);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_02085fd4(void* arg0) {
    u8* temp_r0;

    (void)arg0;
    func_ov002_0208bd40();
    temp_r0 = func_ov002_02085df8();
    func_ov031_0210c94c(temp_r0 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r0 + 0x4161C, temp_r0);
    *(u32*)(temp_r0 + 0x462EC) = 1;
    func_ov031_0210cc84(temp_r0 + 0x4161C, &data_ov002_02093048, NULL);
    func_ov002_02085a44(temp_r0);
    func_0202733c(0x19);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_02086050(void* arg0) {
    u8* temp_r4;

    (void)arg0;
    func_ov002_0208bd40();
    func_020273a8(0x19, 1);
    temp_r4 = func_ov002_02085df8();
    SystemStatusFlags |= 0x40;
    SystemStatusFlags |= 0x80;
    func_ov031_0210c94c(temp_r4 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r4 + 0x4161C, temp_r4);
    func_ov031_0210cc84(temp_r4 + 0x4161C, &data_ov002_02092e18, NULL);
    func_ov002_02085a44(temp_r4);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_020860ec(void* arg0) {
    u8* temp_r0;

    (void)arg0;
    func_ov002_0208bd40();
    temp_r0                    = func_ov002_02085df8();
    *(s32*)(temp_r0 + 0x460BC) = 1;
    func_ov031_0210c94c(temp_r0 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r0 + 0x4161C, temp_r0);
    func_ov031_0210cc84(temp_r0 + 0x4161C, &data_ov002_02092c9c, NULL);
    func_ov002_02085a44(temp_r0);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_02086160(void* arg0) {
    u8* temp_r0;

    (void)arg0;
    func_ov002_0208bd40();
    temp_r0 = func_ov002_02085df8();
    func_ov031_0210c94c(temp_r0 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r0 + 0x4161C, temp_r0);
    func_ov031_0210cc84(temp_r0 + 0x4161C, &data_ov002_020932b8, NULL);
    func_ov002_02085a44(temp_r0);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_020861c8(void* arg0) {
    u8* temp_r4;

    (void)arg0;
    func_ov002_0208c228();
    temp_r4 = func_ov002_02085df8();
    func_ov002_02085a44(temp_r4);
    func_ov031_0210c94c(temp_r4 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r4 + 0x4161C, temp_r4);
    func_ov031_0210cc84(temp_r4 + 0x4161C, &data_ov002_0209344c, NULL);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_0208622c(void* arg0) {
    u8* temp_r4;

    (void)arg0;
    func_ov002_0208c228();
    temp_r4 = func_ov002_02085df8();
    func_ov002_02085a44(temp_r4);
    func_ov031_0210c94c(temp_r4 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r4 + 0x4161C, temp_r4);
    func_ov031_0210cc84(temp_r4 + 0x4161C, &data_ov002_02093460, NULL);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_02086290(void* arg0) {
    u8* temp_r4;

    (void)arg0;
    func_ov002_0208c228();
    temp_r4 = func_ov002_02085df8();
    func_ov002_02085a44(temp_r4);
    func_ov031_0210c94c(temp_r4 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r4 + 0x4161C, temp_r4);
    func_ov031_0210cc84(temp_r4 + 0x4161C, &data_ov002_02093438, NULL);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_020862f4(void* arg0) {
    u8* temp_r4;

    (void)arg0;
    func_ov002_0208c228();
    temp_r4 = func_ov002_02085df8();
    func_ov002_02085a44(temp_r4);
    func_ov031_0210c94c(temp_r4 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r4 + 0x4161C, temp_r4);
    func_ov031_0210cc84(temp_r4 + 0x4161C, &data_ov002_02093310, NULL);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_02086358(void* arg0) {
    u8* temp_r4;

    (void)arg0;
    func_ov002_0208c228();
    temp_r4 = func_ov002_02085df8();
    func_ov002_02085a44(temp_r4);
    func_ov031_0210c94c(temp_r4 + 0x4161C, &data_ov002_02092be4, 0x48068);
    func_ov031_0210c9a8(temp_r4 + 0x4161C, temp_r4);
    func_ov031_0210cc84(temp_r4 + 0x4161C, &data_ov002_02093324, NULL);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov002_020863bc(void* arg0) {
    Ov002_Manager* pManager = (Ov002_Manager*)arg0;
    void*          sp34;
    void*          sp2C;
    void*          sp24;
    void*          sp1C;
    void*          sp14;
    void*          spC;
    void*          sp4;

    if ((SystemStatusFlags & 0x2) && pManager->unk460BC != 0) {
        func_ov031_0210cd14(pManager->menuMgr, func_ov002_02082a44);
        return;
    }

    TouchInput_Update();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);

    if (!((SystemStatusFlags & 0x2) && pManager->unk460BC != 0)) {
        func_ov031_0210cea0(pManager->subMgr);
        EasyTask_ProcessPendingTasks(pManager->taskPool);
        if (func_ov031_0210ca80(pManager->menuMgr) == 0) {
            MainOvlDisp_Pop(&sp34);
            return;
        }
        if (pManager->spriteActive != 0) {
            Sprite_UpdateAndCheck(&pManager->sprite);
        }
    }

    func_ov031_0210cf08(pManager->subMgr);
    func_ov031_0210cb68(pManager->menuMgr);

    switch (pManager->state) {
        case 0:
            func_ov031_0210cf50(pManager->subMgr);
            EasyTask_UpdateActiveTasks(pManager->taskPool);
            func_ov031_0210cbc4(pManager->menuMgr);
            if (pManager->spriteActive != 0) {
                Sprite_Render(&pManager->sprite);
            }
            func_020034b0(&data_020676ec);
            func_020034b0(&data_02068778);
            func_0200bf60(data_0206b3cc.unk0, 0);
            func_0200bf60(data_0206b3cc.unk4, 0);
            func_0200d90c();
            if (pManager->unk460BC != 0) {
                func_ov002_02085ac4(pManager);
            }
            return;
        case 1:
            SystemStatusFlags |= 0x40;
            SystemStatusFlags |= 0x80;
            MainOvlDisp_ReplaceTop(&sp2C, 0x27, (void*)0x02083240, 0);
            return;
        case 2:
            MainOvlDisp_ReplaceTop(&sp24, 0x27, (void*)0x02083280, 0);
            return;
        case 5:
            SystemStatusFlags |= 0x40;
            SystemStatusFlags |= 0x80;
            MainOvlDisp_ReplaceTop(&sp1C, 0x2B, (void*)0x02084040, 0);
            return;
        case 4:
            SystemStatusFlags |= 0x40;
            SystemStatusFlags |= 0x80;
            MainOvlDisp_ReplaceTop(&sp14, 0x1E, (void*)&func_ov030_020ae92c, 0);
            return;
        case 3:
            if (pManager->unk462EC == 0) {
                data_02073710.unkB4 |= 0x40;
            } else {
                data_02073710.unkB4 |= 0x20;
            }
            SystemStatusFlags |= 0x40;
            SystemStatusFlags |= 0x80;
            MainOvlDisp_ReplaceTop(&spC, 0x2C, (void*)0x02084A88, 0);
            return;
        case 6:
            SystemStatusFlags |= 0x40;
            SystemStatusFlags |= 0x80;
            MainOvlDisp_ReplaceTop(&sp4, 0x25, (void*)0x0208370C, 0);
            return;
        default:
            OS_WaitForever();
    }
}

void func_ov002_0208678c(void* arg0) {
    Ov002_Manager* pManager = (Ov002_Manager*)arg0;
    func_020273a8(0x19, 1);
    if (pManager->spriteActive != 0) {
        func_ov002_0208264c(pManager);
    }
    EasyTask_DestroyPool(pManager->taskPool);
    func_ov031_0210ca38(pManager->menuMgr);
    func_ov031_0210d044(pManager->subMgr);
    func_0200cef0(NULL);
    DatMgr_ClearSlot(OVMGR_U32(pManager, 0x11584));
    DatMgr_ClearSlot(OVMGR_U32(pManager, 0x1158C));
    DatMgr_ClearSlot(OVMGR_U32(pManager, 0x11588));
    Interrupts_RegisterVBlankCallback(NULL, 1);
    Interrupts_RegisterHBlankCallback(0, 1);
    func_ov031_0210aabc(pManager->unk45FF4);
    FS_UnloadOverlay(0, 0x1F);
    if (pManager->unk41FF0 != 0) {
        OvlMgr_UnloadOverlay(3);
    }
    Mem_Free(&gMainHeap, pManager);
    func_02004a68(&gMainHeap);
    func_02004a68(&gDebugHeap);
}

void func_ov002_0208688c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091b1c[count](arg0);
    }
}

void func_ov002_020868cc(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091b28[count](arg0);
    }
}

void func_ov002_0208690c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091b40[count](arg0);
    }
}

void func_ov002_0208694c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091b58[count](arg0);
    }
}

void func_ov002_0208698c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091af8[count](arg0);
    }
}

void func_ov002_020869cc(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091ad4[count](arg0);
    }
}

void func_ov002_02086a0c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091ae0[count](arg0);
    }
}

void func_ov002_02086a4c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091aec[count](arg0);
    }
}

void func_ov002_02086a8c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091b04[count](arg0);
    }
}

void func_ov002_02086acc(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091b10[count](arg0);
    }
}

void func_ov002_02086b0c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091b34[count](arg0);
    }
}

void func_ov002_02086b4c(void* arg0) {
    s32 count = MainOvlDisp_GetRepeatCount();
    if (count == 0x7FFFFFFF) {
        func_ov002_0208678c(arg0);
    } else {
        data_ov002_02091b4c[count](arg0);
    }
}

void func_ov002_02086b8c(void* arg1) {
    data_ov002_02093660 = 0;
    func_ov002_02085710(arg1);
}

void func_ov002_02086bac(void) {}

void func_ov002_02086bb0(void) {
    func_02026b9c(0x55C);
}

void func_ov002_02086bc4(void* arg1) {
    func_02026b9c(0x55C);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x476D0);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x478B8);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x47AA0);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x47C88);
    func_ov002_0208bd40();
    func_ov002_02085710(arg1);
}

void func_ov002_02086c5c(void) {}

void func_ov002_02086c60(void) {}

s32 func_ov002_02086c64(void* arg1) {
    OVMGR_U16(arg1, 0x474CC) = 0x1E;
    func_ov031_0210cdac();
    return 2;
}

s32 func_ov002_02086c84(void* arg0, void* arg1) {
    if (TouchInput_WasTouchPressed() != 0) {
        func_ov031_0210cc84(arg0, &data_ov002_02092bf4, NULL);
        return 0;
    }
    if (OVMGR_U16(arg1, 0x474CC) == 0) {
        func_ov031_0210cdac(arg0);
    }
    OVMGR_U16(arg1, 0x474CC) = (u16)(OVMGR_U16(arg1, 0x474CC) - 1);
    return 0;
}

s32 func_ov002_02086cec(void* arg0, void* arg1) {
    s32   sp20;
    s32   sp1C;
    s32   sp18;
    void* sp14;
    void* sp10;
    void* spC;
    s16   sp8;
    s16   sp6;
    s16   sp4;
    void* sp0;
    u16   var_r8;
    void* temp_r9;

    sp0  = arg0;
    sp14 = arg1 + 0x474E8;
    sp18 = arg1 + 0x476D0;
    sp1C = arg1 + 0x478B8;
    sp20 = arg1 + 0x47AA0;
    func_ov002_02083a74(arg1);
    var_r8 = 0;
    do {
        temp_r9 = (&sp14)[var_r8];
        sp4     = 3 - var_r8;
        sp6     = 0x78;
        func_ov031_0210c94c(temp_r9, &data_ov002_02092c6c, 0x114);
        func_ov031_0210cc84(temp_r9, &data_ov002_020931e8, &sp4);
        func_ov031_0210ce50(arg1 + 0x41804, temp_r9);
        var_r8 += 1;
    } while ((u32)var_r8 < 4U);
    spC  = arg1 + 0x47398;
    sp10 = arg1 + 0x47438;
    sp8  = 0x78;
    func_ov031_0210c94c(arg1 + 0x47C88, &data_ov002_02092c84, 0x118);
    func_ov031_0210cc84(arg1 + 0x47C88, &data_ov002_02093208, &sp8);
    func_ov031_0210ce50(arg1 + 0x41804, arg1 + 0x47C88);
    OVMGR_U16(arg1, 0x474C8) = 0x78;
    OVMGR_S32(arg1, 0x474D8) = -0x200000;
    func_ov031_0210cdac(sp0);
    return 0;
}

s32 func_ov002_02086e4c(void* arg0, void* arg1) {
    if (TouchInput_WasTouchPressed() != 0) {
        func_ov031_0210cc84(arg0, &data_ov002_02092bf4, NULL);
        return 0;
    }
    if (OVMGR_U16(arg1, 0x474C8) == 0) {
        func_ov031_0210cdac(arg0);
    }
    OVMGR_U16(arg1, 0x474C8) = (u16)(OVMGR_U16(arg1, 0x474C8) - 1);
    return 0;
}

s32 func_ov002_02086eb4(void* arg1) {
    OVMGR_U16(arg1, 0x474C8) = 0x14;
    OVMGR_S32(arg1, 0x474D8) = 0x100000;
    OVMGR_S32(arg1, 0x474DC) = -0x100000;
    func_ov031_0210cdac();
    return 0;
}

s32 func_ov002_02086ee8(void* arg0, void* arg1) {
    s32 temp_ip;
    s32 temp_ip_2;
    s32 temp_ip_3;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    u32 temp_r2;
    u32 temp_r2_2;
    u32 temp_r2_3;
    u8* list_base;
    u8* list_base_2;
    u8* list_base_3;
    u8* entry_base;
    u8* entry_base_2;
    u8* entry_base_3;

    if ((TouchInput_WasTouchPressed() != 0) || (OVMGR_U16(arg1, 0x474C8) == 0)) {
        func_ov031_0210cdac(arg0);
        return 0;
    }
    func_020265d4((u8*)arg1 + 0x474D8, 0);
    func_020265d4((u8*)arg1 + 0x474DC, 0, OVMGR_U16(arg1, 0x474C8));
    temp_r0   = OVMGR_S32(arg1, 0x4734C);
    list_base = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47348) * 0x220);
    temp_r2   = *(u32*)(list_base + (temp_r0 * 0x3C));
    switch (temp_r2) { /* switch 1 */
        default:       /* switch 1 */
            break;
        case 1:        /* switch 1 */
        case 2:        /* switch 1 */
        case 3:        /* switch 1 */
        case 4:        /* switch 1 */
        case 5:        /* switch 1 */
            *(u32*)(list_base + (temp_r0 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base                 = list_base + (temp_r0 * 8);
    *(s32*)(entry_base + 0xF0) = OVMGR_S32(arg1, 0x474D8);
    *(s32*)(entry_base + 0xF4) = 0;
    temp_r0_2                  = OVMGR_S32(arg1, 0x473EC);
    list_base_2                = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x473E8) * 0x220);
    temp_r2_2                  = *(u32*)(list_base_2 + (temp_r0_2 * 0x3C));
    switch (temp_r2_2) { /* switch 2 */
        default:         /* switch 2 */
            break;
        case 1:          /* switch 2 */
        case 2:          /* switch 2 */
        case 3:          /* switch 2 */
        case 4:          /* switch 2 */
        case 5:          /* switch 2 */
            *(u32*)(list_base_2 + (temp_r0_2 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_2                 = list_base_2 + (temp_r0_2 * 8);
    *(s32*)(entry_base_2 + 0xF0) = OVMGR_S32(arg1, 0x474D8);
    *(s32*)(entry_base_2 + 0xF4) = 0;
    temp_r0_3                    = OVMGR_S32(arg1, 0x47414);
    list_base_3                  = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47410) * 0x220);
    temp_r2_3                    = *(u32*)(list_base_3 + (temp_r0_3 * 0x3C));
    switch (temp_r2_3) { /* switch 3 */
        default:         /* switch 3 */
            break;
        case 1:          /* switch 3 */
        case 2:          /* switch 3 */
        case 3:          /* switch 3 */
        case 4:          /* switch 3 */
        case 5:          /* switch 3 */
            *(u32*)(list_base_3 + (temp_r0_3 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_3                 = list_base_3 + (temp_r0_3 * 8);
    *(s32*)(entry_base_3 + 0xF0) = OVMGR_S32(arg1, 0x474DC);
    *(s32*)(entry_base_3 + 0xF4) = 0;
    OVMGR_U16(arg1, 0x474C8)     = (u16)(OVMGR_U16(arg1, 0x474C8) - 1);
    return 0;
}

s32 func_ov002_020870ac(void* arg0) {
    func_ov031_0210cc84(arg0, &data_ov002_02092bf4, NULL);
    return 0;
}

s32 func_ov002_020870c8(void* arg0, void* arg1) {
    s32 temp_lr;
    s32 temp_r0_12;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 temp_r0_5;
    s32 temp_r0_7;
    s32 temp_r0_9;
    s32 temp_r2;
    s32 temp_r2_8;
    s32 temp_r5;
    s32 temp_r5_2;
    s32 temp_r5_3;
    s32 temp_r5_4;
    s32 temp_r5_5;
    s32 temp_r5_6;
    s32 temp_r5_7;
    u32 temp_r2_2;
    u32 temp_r2_4;
    u32 temp_r2_5;
    u32 temp_r2_6;
    u32 temp_r2_7;
    u32 temp_r2_9;
    u32 temp_r3;
    u32 temp_r3_2;
    u8* list_base;
    u8* list_base_2;
    u8* list_base_3;
    u8* list_base_4;
    u8* list_base_5;
    u8* list_base_6;
    u8* list_base_7;
    u8* list_base_8;
    u8* entry_base;
    u8* entry_base_2;
    u8* entry_base_3;
    u8* entry_base_4;
    u8* entry_base_5;
    u8* entry_base_6;
    u8* entry_base_7;
    u8* entry_base_8;

    temp_r2   = OVMGR_S32(arg1, 0x4739C);
    list_base = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47398) * 0x220);
    temp_r3   = *(u32*)(list_base + (temp_r2 * 0x3C));
    switch (temp_r3) { /* switch 1 */
        default:       /* switch 1 */
            break;
        case 1:        /* switch 1 */
        case 2:        /* switch 1 */
        case 3:        /* switch 1 */
        case 4:        /* switch 1 */
        case 5:        /* switch 1 */
            *(u32*)(list_base + (temp_r2 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base                 = list_base + (temp_r2 * 8);
    *(s32*)(entry_base + 0xF0) = 0;
    *(s32*)(entry_base + 0xF4) = 0;
    temp_r0_2                  = OVMGR_S32(arg1, 0x4743C);
    list_base_2                = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47438) * 0x220);
    temp_r2_2                  = *(u32*)(list_base_2 + (temp_r0_2 * 0x3C));
    switch (temp_r2_2) { /* switch 2 */
        default:         /* switch 2 */
            break;
        case 1:          /* switch 2 */
        case 2:          /* switch 2 */
        case 3:          /* switch 2 */
        case 4:          /* switch 2 */
        case 5:          /* switch 2 */
            *(u32*)(list_base_2 + (temp_r0_2 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_2                 = list_base_2 + (temp_r0_2 * 8);
    *(s32*)(entry_base_2 + 0xF0) = 0;
    *(s32*)(entry_base_2 + 0xF4) = 0x100000;
    temp_r0_3                    = OVMGR_S32(arg1, 0x4734C);
    list_base_3                  = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47348) * 0x220);
    temp_r2_4                    = *(u32*)(list_base_3 + (temp_r0_3 * 0x3C));
    switch (temp_r2_4) { /* switch 3 */
        default:         /* switch 3 */
            break;
        case 1:          /* switch 3 */
        case 2:          /* switch 3 */
        case 3:          /* switch 3 */
        case 4:          /* switch 3 */
        case 5:          /* switch 3 */
            *(u32*)(list_base_3 + (temp_r0_3 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_3                 = list_base_3 + (temp_r0_3 * 8);
    *(s32*)(entry_base_3 + 0xF0) = 0;
    *(s32*)(entry_base_3 + 0xF4) = 0;
    temp_r0_5                    = OVMGR_S32(arg1, 0x473EC);
    list_base_4                  = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x473E8) * 0x220);
    temp_r2_5                    = *(u32*)(list_base_4 + (temp_r0_5 * 0x3C));
    switch (temp_r2_5) { /* switch 4 */
        default:         /* switch 4 */
            break;
        case 1:          /* switch 4 */
        case 2:          /* switch 4 */
        case 3:          /* switch 4 */
        case 4:          /* switch 4 */
        case 5:          /* switch 4 */
            *(u32*)(list_base_4 + (temp_r0_5 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_4                 = list_base_4 + (temp_r0_5 * 8);
    *(s32*)(entry_base_4 + 0xF0) = 0;
    *(s32*)(entry_base_4 + 0xF4) = 0;
    temp_r0_7                    = OVMGR_S32(arg1, 0x47414);
    list_base_5                  = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47410) * 0x220);
    temp_r2_6                    = *(u32*)(list_base_5 + (temp_r0_7 * 0x3C));
    switch (temp_r2_6) { /* switch 5 */
        default:         /* switch 5 */
            break;
        case 1:          /* switch 5 */
        case 2:          /* switch 5 */
        case 3:          /* switch 5 */
        case 4:          /* switch 5 */
        case 5:          /* switch 5 */
            *(u32*)(list_base_5 + (temp_r0_7 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_5                 = list_base_5 + (temp_r0_7 * 8);
    *(s32*)(entry_base_5 + 0xF0) = 0;
    *(s32*)(entry_base_5 + 0xF4) = 0;
    OVMGR_S32(arg1, 0x473A0)     = (OVMGR_S32(arg1, 0x473A0) | 2);
    OVMGR_S32(arg1, 0x47440)     = (OVMGR_S32(arg1, 0x47440) | 2);
    OVMGR_S32(arg1, 0x47350)     = (OVMGR_S32(arg1, 0x47350) | 2);
    OVMGR_S32(arg1, 0x473F0)     = (OVMGR_S32(arg1, 0x473F0) | 2);
    OVMGR_S32(arg1, 0x47418)     = (OVMGR_S32(arg1, 0x47418) | 2);
    temp_r0_9                    = OVMGR_S32(arg1, 0x47374);
    list_base_6                  = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47370) * 0x220);
    temp_r2_7                    = *(u32*)(list_base_6 + (temp_r0_9 * 0x3C));
    switch (temp_r2_7) { /* switch 6 */
        default:         /* switch 6 */
            break;
        case 1:          /* switch 6 */
        case 2:          /* switch 6 */
        case 3:          /* switch 6 */
        case 4:          /* switch 6 */
        case 5:          /* switch 6 */
            *(u32*)(list_base_6 + (temp_r0_9 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_6                 = list_base_6 + (temp_r0_9 * 8);
    *(s32*)(entry_base_6 + 0xF0) = 0;
    *(s32*)(entry_base_6 + 0xF4) = 0;
    OVMGR_S32(arg1, 0x47378)     = (OVMGR_S32(arg1, 0x47378) | 2);
    data_ov002_02093660          = 1;
    temp_r2_8                    = OVMGR_S32(arg1, 0x4739C);
    list_base_7                  = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47398) * 0x220);
    temp_r3_2                    = *(u32*)(list_base_7 + (temp_r2_8 * 0x3C));
    switch (temp_r3_2) { /* switch 7 */
        default:         /* switch 7 */
            break;
        case 1:          /* switch 7 */
        case 2:          /* switch 7 */
        case 3:          /* switch 7 */
        case 4:          /* switch 7 */
        case 5:          /* switch 7 */
            *(u32*)(list_base_7 + (temp_r2_8 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_7                 = list_base_7 + (temp_r2_8 * 8);
    *(s32*)(entry_base_7 + 0xF0) = 0;
    *(s32*)(entry_base_7 + 0xF4) = 0;
    temp_r0_12                   = OVMGR_S32(arg1, 0x4743C);
    list_base_8                  = (u8*)&g_DisplaySettings + 0x64 + (OVMGR_S32(arg1, 0x47438) * 0x220);
    temp_r2_9                    = *(u32*)(list_base_8 + (temp_r0_12 * 0x3C));
    switch (temp_r2_9) { /* switch 8 */
        default:         /* switch 8 */
            break;
        case 1:          /* switch 8 */
        case 2:          /* switch 8 */
        case 3:          /* switch 8 */
        case 4:          /* switch 8 */
        case 5:          /* switch 8 */
            *(u32*)(list_base_8 + (temp_r0_12 * 0x2C) + 0x124) = 1;
            break;
    }
    entry_base_8                 = list_base_8 + (temp_r0_12 * 8);
    *(s32*)(entry_base_8 + 0xF0) = 0;
    *(s32*)(entry_base_8 + 0xF4) = 0x100000;
    OVMGR_U16(arg1, 0x474C8)     = 0xF0;
    func_02026b20(0x55C);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208749c(void* arg0, void* arg1) {
    OVMGR_U16(arg1, 0x474C8) = (u16)(OVMGR_U16(arg1, 0x474C8) - 1);
    if (TouchInput_WasTouchPressed() != 0) {
        func_02026b20(0x347);
    }
    if ((TouchInput_WasTouchPressed() != 0) || (OVMGR_U16(arg1, 0x474C8) == 0)) {
        func_ov031_0210cdac(arg0);
        return 0;
    }
    return 0;
}

s32 func_ov002_02087508(void* arg0) {
    func_ov031_0210cc84(arg0, &data_ov002_02092c9c, NULL);
    return 0;
}

void func_ov002_02087524(void* arg1) {
    u16 var_r2;
    u8* base;

    var_r2 = 0;
    do {
        base = (u8*)&data_02071d10;
        var_r2 += 1;
        *(u16*)(base + 0x3414 + (var_r2 * 2) - 2) = 0;
    } while ((u32)var_r2 < 4U);
    data_02072d10.unkD88    = 0;
    data_02072d10.unkD84    = 0;
    OVMGR_U8(arg1, 0x41FE9) = 0;
    func_ov002_02085a44(arg1);
}

void func_ov002_0208757c(void* arg1) {
    func_ov002_02085710(arg1);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x476D0);
}

void func_ov002_020875c0(void) {}

void func_ov002_020875c4(void) {}

s32 func_ov002_020875c8(void* arg0, void* arg1) {
    u16        sp2[0x1E];
    const u16* var_ip;
    s32        var_r2;
    u16*       var_r3;

    var_ip = data_ov002_02091c86;
    var_r3 = sp2;
    var_r2 = 0xF;
    do {
        *var_r3++ = *var_ip++;
        *var_r3++ = *var_ip++;
        var_r2 -= 1;
    } while (var_r2 != 0);
    func_ov031_0210ab54(arg1 + 0x45FF4, 1, 0);
    func_ov002_02082f18(arg1, 0x15, 0x14, sp2);
    func_0202733c(0x19);
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    func_ov031_0210c94c(arg1 + 0x474E8, &data_ov002_02092cd0, 0x48068, 0x60);
    func_ov031_0210cc84(arg1 + 0x474E8, &data_ov002_02093020, subroutine_arg0);
    func_ov031_0210ce50(arg1 + 0x41804, arg1 + 0x474E8);
    func_ov031_0210c94c(arg1 + 0x476D0, &data_ov002_02092cd0, 0x48068, 0x88);
    func_ov031_0210cc84(arg1 + 0x476D0, &data_ov002_02093020, subroutine_arg0);
    func_ov031_0210ce50(arg1 + 0x41804, arg1 + 0x476D0);
    SystemStatusFlags |= 0x40;
    SystemStatusFlags |= 0x80;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02087728(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            table_sp18[0x19];
    u16            table_sp0[0xC];
    u16            temp_r0_3;
    u16            i;

    for (i = 0; i < 0x19; i++) {
        table_sp18[i] = data_ov002_02091c54[i];
    }
    for (i = 0; i < 6; i++) {
        table_sp0[i * 2]     = data_ov002_02091c3c[i].unk0;
        table_sp0[i * 2 + 1] = data_ov002_02091c3c[i].unk2;
    }

    temp_r0_3 = func_ov002_0208597c(table_sp18);
    if (temp_r0_3 == 0xFFFF) {
        return 0;
    }
    if (temp_r0_3 == 0xFFFE) {
        OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFFU;
        OVMGR_U16(mgr_bytes, 0x46084) = 0U;
        OVMGR_U16(mgr_bytes, 0x46086) = 0xD2U;
        return 0;
    }
    if (OVMGR_U16(mgr_bytes, 0x474C8) == temp_r0_3) {
        switch (temp_r0_3) { /* switch 1 */
            default:         /* switch 1 */
                /* Duplicate return node #26. Try simplifying control flow for better match */
                return 0;
            case 0: /* switch 1 */
                func_02026b20(0x348);
                func_ov031_0210cdac(arg0);
                return 0;
            case 1: /* switch 1 */
            case 2: /* switch 1 */
            case 3: /* switch 1 */
                OVMGR_U16(mgr_bytes, 0x460CC) = 0x80;
                OVMGR_U16(mgr_bytes, 0x460CE) = 0x60;
                func_02026b20(0x347);
                func_ov031_0210cdac(arg0);
                return 0;
        }
    } else {
        u16 anim_index = (u16)(temp_r0_3 * 3);

        OVMGR_U16(mgr_bytes, 0x474C8) = temp_r0_3;
        func_02026b20(0x346);
        switch (temp_r0_3) { /* switch 2 */
            default:         /* switch 2 */
                return 0;
            case 0:          /* switch 2 */
            case 1:          /* switch 2 */
            case 2:          /* switch 2 */
            case 3:          /* switch 2 */
                OVMGR_U16(mgr_bytes, 0x46084) = table_sp0[anim_index + 1];
                OVMGR_U16(mgr_bytes, 0x46086) = table_sp0[anim_index + 2];
                Sprite_ChangeAnimation((Sprite*)(mgr_bytes + 0x46078), OVMGR_PTR(mgr_bytes, 0x46090),
                                       (s16)table_sp0[anim_index], OVMGR_PTR(mgr_bytes, 0x46094));
                return 0;
        }
    }
}

s32 func_ov002_020878f8(void* arg0, void* arg1) {
    u16 temp_r2;
    u32 temp_ip;
    u32 temp_ip_2;
    u8* mgr = (u8*)arg1;

    temp_r2 = OVMGR_U16(mgr, 0x474C8);
    switch (temp_r2) {
        default:
            return 0;
        case 0:
            OVMGR_U16(mgr, 0x46074) = 5;
            return 0;
        case 1:
            temp_ip = SystemStatusFlags << 0x19;
            SystemStatusFlags &= ~0x40;
            OVMGR_S16(mgr, 0x46070) = (s16)(temp_ip >> 0x1F);
            SystemStatusFlags &= ~0x80;
            func_ov031_0210cc84(arg0, &data_ov002_02092ff0, subroutine_arg0);
            /* Duplicate return node #11. Try simplifying control flow for better match */
            return 0;
        case 2:
            temp_ip_2 = SystemStatusFlags << 0x19;
            SystemStatusFlags &= ~0x40;
            OVMGR_S16(mgr, 0x46070) = (s16)(temp_ip_2 >> 0x1F);
            SystemStatusFlags &= ~0x80;
            func_ov031_0210cc84(arg0, &data_ov002_02092f44, subroutine_arg0);
            /* Duplicate return node #11. Try simplifying control flow for better match */
            return 0;
        case 3:
            data_02074d10.unk41C    = 0x1F;
            data_02074d10.unk40B    = 3;
            OVMGR_U16(mgr, 0x46074) = 1;
            /* Duplicate return node #11. Try simplifying control flow for better match */
            return 0;
    }
}

s32 func_ov002_020879f8(void* arg0, void* arg1) {
    func_ov002_02082ab4(arg1);
    func_ov031_0210cdac(arg0);
    return 0;
}

void func_ov002_02087a18(void* arg1) {
    u8* mgr = (u8*)arg1;

    OVMGR_U8(mgr, 0x41FE9)  = 1;
    OVMGR_U32(mgr, 0x46300) = 0;
    OVMGR_U16(mgr, 0x46084) = 0;
    OVMGR_U16(mgr, 0x46086) = 0xC8;
}

void func_ov002_02087a40(void* arg1) {
    u8* mgr = (u8*)arg1;

    OVMGR_U32(mgr, 0x46300) = 0;
    func_ov002_02085710(arg1);
    OVMGR_U16(mgr, 0x46084) = 0;
    OVMGR_U16(mgr, 0x46086) = 0xC8;
    func_ov031_0210cff0(mgr + 0x41804, mgr + 0x474E8);
    func_ov031_0210cff0(mgr + 0x41804, mgr + 0x47C88);
}

void func_ov002_02087aa8(void* arg0) {
    u8*             mgr = (u8*)arg0;
    Ov002_ResBlock* res = Ov002_GetResBlock(arg0);
    u16             table_pairs[0x12][2];
    u16             select_values[4];
    u16             temp_r0_2;
    u16             temp_r3;
    u16             temp_r4;
    s32             temp_r2;
    u16             i;
    void*           pack;
    u8*             pack_base;
    void*           var_r4_2;
    void*           var_ip;

    for (i = 0; i < 0x12; i++) {
        table_pairs[i][0] = data_ov002_02091ed0[i].unk0;
        table_pairs[i][1] = data_ov002_02091ed0[i].unk2;
    }

    temp_r0_2 = OVMGR_U16(mgr, 0x474D2);
    if (temp_r0_2 == 0) {
        temp_r4                 = (u16)(OVMGR_U16(mgr, 0x474D0) + 1);
        temp_r4                 = (u16)(temp_r4 - (0x12 * ((u32)MULTU_HI(temp_r4, 0x38E38E39) >> 2)));
        OVMGR_U16(mgr, 0x474D0) = temp_r4;
        OVMGR_U16(mgr, 0x474D2) = table_pairs[temp_r4][1];
        if (OVMGR_U32(mgr, 0x46300) == 0) {
            return;
        }
        pack             = OVMGR_PTR(mgr, 0x462F4);
        select_values[0] = data_ov002_02091cc4.unk0;
        select_values[1] = data_ov002_02091cc4.unk2;
        select_values[2] = data_ov002_02091cc4.unk4;
        select_values[3] = data_ov002_02091cc4.unk6;
        temp_r3          = select_values[data_02074d10.unk40A];
        if ((pack == NULL) || ((s32)temp_r3 <= 0)) {
            var_r4_2 = NULL;
        } else {
            pack_base = Ov002_GetPackBase(pack);
            var_r4_2  = pack_base + *(u32*)(pack_base + temp_r3 * 8);
        }
        if (pack == NULL) {
            var_ip = NULL;
        } else {
            pack_base = Ov002_GetPackBase(pack);
            var_ip    = pack_base + *(u32*)(pack_base + 0x28);
        }
        func_0203b270((s32)(var_ip + (table_pairs[temp_r4][0] << 5)), var_r4_2 + 0x20, 0x20);
        if (res->unk340 != NULL) {
            func_0200afec(data_0206b3cc.unk4);
        }
        res->unk340 = func_0200adf8(data_0206b3cc.unk4, var_r4_2, 0, 0, 0x10);
        func_0200bf60(data_0206b3cc.unk4, res->unk340);
        return;
    }
    OVMGR_U16(mgr, 0x474D2) = (u16)(temp_r0_2 - 1);
}

void func_ov002_02087c80(void* arg1) {
    if (OVMGR_U32(arg1, 0x46300) == 0) {
        return;
    }
    func_ov002_02087aa8(arg1);
}

void func_ov002_02087ca0(void) {}

void func_ov002_02087ca4(void* arg0, s32 arg1) {
    u8*           mgr = (u8*)arg0;
    BinIdentifier binId;
    s32           sp8;
    s32           sp4;
    s32           temp_r6;
    s32           temp_r6_2;
    s32           temp_r6_3;
    s32           temp_r6_4;
    Data*         data;

    sp4        = data_ov002_02091cc4.unk8;
    sp8        = data_ov002_02091cc4.unkC;
    binId.id   = (u32)sp4;
    binId.path = (char*)sp8;
    data       = DatMgr_LoadRawDataWithOffset(1, mgr + 0x46304, data_ov002_02091d74[arg1], &binId, data_ov002_02091e3c[arg1]);
    DatMgr_ReleaseData(data);
    if (data_ov002_02091d9c[arg1] != 0) {
        if (OVMGR_PTR(mgr, 0x46314) != NULL) {
            Mem_Free((MemPool*)(mgr + 0x11590), OVMGR_PTR(mgr, 0x46314));
        }
        temp_r6                 = data_ov002_02091d9c[arg1];
        OVMGR_PTR(mgr, 0x46314) = Mem_AllocHeapTail((MemPool*)(mgr + 0x11590), temp_r6);
        temp_r6_2 = DatMgr_LoadRawDataWithOffset(1, OVMGR_PTR(mgr, 0x46314), temp_r6, &binId, data_ov002_02091dec[arg1]);
        if (OVMGR_PTR(mgr, 0x46314) == NULL) {
            OS_WaitForever();
        }
        DatMgr_ReleaseData(temp_r6_2);
    } else {
        OVMGR_PTR(mgr, 0x46314) = NULL;
    }
    if (data_ov002_02091e14[arg1] != 0) {
        if (OVMGR_PTR(mgr, 0x46318) != NULL) {
            Mem_Free((MemPool*)(mgr + 0x11590), OVMGR_PTR(mgr, 0x46318));
        }
        temp_r6_3               = data_ov002_02091e14[arg1];
        OVMGR_PTR(mgr, 0x46318) = Mem_AllocHeapTail((MemPool*)(mgr + 0x11590), temp_r6_3);
        data = DatMgr_LoadRawDataWithOffset(1, OVMGR_PTR(mgr, 0x46318), temp_r6_3, &binId, data_ov002_02091dc4[arg1]);
        DatMgr_ReleaseData(data);
    } else {
        OVMGR_PTR(mgr, 0x46318) = NULL;
    }
    if (data_ov002_02091d24[arg1] != 0) {
        if (OVMGR_PTR(mgr, 0x4631C) != NULL) {
            Mem_Free((MemPool*)(mgr + 0x11590), OVMGR_PTR(mgr, 0x4631C));
        }
        temp_r6_4               = data_ov002_02091d24[arg1];
        OVMGR_PTR(mgr, 0x4631C) = Mem_AllocHeapTail((MemPool*)(mgr + 0x11590), temp_r6_4);
        data = DatMgr_LoadRawDataWithOffset(1, OVMGR_PTR(mgr, 0x4631C), temp_r6_4, &binId, data_ov002_02091d4c[arg1]);
        DatMgr_ReleaseData(data);
        return;
    }
    OVMGR_PTR(mgr, 0x4631C) = NULL;
}

void func_ov002_02087eb4(void* arg0, s32 arg1) {
    typedef struct {
        u8  unk0;
        u8  unk1;
        u8  unk2;
        u8  unk3;
        u8  pad_4[0x10 - 4];
        u8* unk10;
    } Ov002_MapBlock;

    s32             sp10;
    s32             spC;
    s32             sp8;
    s32             sp4;
    s32             sp0;
    s16             var_r4;
    s32             temp_r0;
    s32             temp_r11;
    s32             temp_r2_2;
    s32             var_ip;
    s32             var_r2;
    s32             var_r3;
    s32             var_r6;
    s32             var_r7;
    s32             var_r8;
    s32             var_r9;
    u16             var_r5;
    u32             temp_r2;
    u32             temp_r3_2;
    u32             temp_r4;
    u32             temp_r4_2;
    u8              temp_r0_2;
    u8              temp_r3;
    u8              temp_r5;
    Ov002_MapBlock* temp_lr;
    u8*             temp_r0_3;

    temp_lr   = (Ov002_MapBlock*)((u8*)arg0 + 0x46304);
    temp_r5   = temp_lr->unk2;
    temp_r3   = temp_lr->unk3;
    sp8       = 0;
    temp_r4   = 0x28 - (temp_r5 - 0xA);
    temp_r2   = 0x28 - (temp_r3 - 0xA);
    temp_r3_2 = temp_r3 - 9;
    temp_r0   = (s32)(temp_r3_2 + (temp_r3_2 >> 0x1F)) >> 1;
    temp_r4_2 = temp_r5 - 9;
    sp4       = temp_r0;
    sp0       = ((s32)(temp_r4 + (temp_r4 >> 0x1F)) >> 1) + 8;
    spC       = ((s32)(temp_r2 + (temp_r2 >> 0x1F)) >> 1) + 2;
    temp_r11  = (s32)(temp_r4_2 + (temp_r4_2 >> 0x1F)) >> 1;
    if (temp_r0 <= 0) {
        return;
    }
    var_r6 = 5;
    var_r7 = 7;
    do {
        var_ip = 0;
        if (temp_r11 > 0) {
            var_r8 = 5;
            var_r9 = 7;
            sp10   = arg1 + (sp0 * 2) + (((spC + sp8) << 5) * 2);
            do {
                var_r4 = 0x2000;
                var_r5 = 1;
                var_r3 = var_r6;
                if (var_r6 < var_r7) {
                    do {
                        var_r2 = var_r8;
                        if (var_r8 < var_r9) {
                            do {
                                temp_r0_2 = *(temp_lr->unk10 + (var_r3 * temp_lr->unk2 * 2) + (var_r2 * 2));
                                var_r2 += 1;
                                if (temp_r0_2 != 0) {
                                    var_r4 |= var_r5;
                                }
                                var_r5 *= 2;
                            } while (var_r2 < var_r9);
                        }
                        var_r3 += 1;
                    } while (var_r3 < var_r7);
                }
                var_r8 += 2;
                temp_r0_3 = (u8*)(sp10 + (var_ip * 2));
                var_ip += 1;
                *(u16*)(temp_r0_3 + 4) = var_r4;
                var_r9 += 2;
            } while (var_ip < temp_r11);
        }
        var_r6 += 2;
        temp_r2_2 = sp8 + 1;
        sp8       = temp_r2_2;
        var_r7 += 2;
    } while (temp_r2_2 < sp4);
}

void func_ov002_0208800c(void* arg0) {
    u8* mgr = (u8*)arg0;
    u16 index;

    index = OVMGR_U16(mgr, 0x4196C);
    if (index == 0xFFFF) {
        func_0203aafc(3, G2S_GetBG0ScrPtr(), 0, 0x1000);
        return;
    }
    func_ov002_02087ca4(mgr, index);
    if (OVMGR_PTR(mgr, 0x46314) == NULL) {
        OS_WaitForever();
    }
    OVMGR_PTR(mgr, 0x47488) = (void*)(mgr + 0x46320);
    func_ov002_02087eb4(mgr, (s32)OVMGR_PTR(mgr, 0x47488));
    DC_PurgeAll();
    func_0203ab7c(3, mgr + 0x46320, G2S_GetBG0ScrPtr(), 0x1000);
}

s32 func_ov002_020880a0(void* arg0) {
    u8* temp_ip;
    u8* temp_ip_2;
    u8* temp_lr;
    u8* temp_r3;

    g_DisplaySettings.unk20  = (s32)(g_DisplaySettings.unk20 | 4);
    g_DisplaySettings.unk24C = 0xC;
    g_DisplaySettings.unk250 = 1;
    g_DisplaySettings.unk22C = 0x1F;
    g_DisplaySettings.unk230 = 1;
    g_DisplaySettings.unk26C = 1;
    g_DisplaySettings.unk270 = 4;
    g_DisplaySettings.unk274 = 8;
    g_DisplaySettings.unk278 = 0xC;
    g_DisplaySettings.unk27C = 4;
    g_DisplaySettings.unk48C = 1;
    g_DisplaySettings.unk46C = 0xC;
    g_DisplaySettings.unk470 = 1;
    g_DisplaySettings.unk490 = 4;
    g_DisplaySettings.unk494 = 8;
    g_DisplaySettings.unk44C = 0x1F;
    g_DisplaySettings.unk450 = 0;
    g_DisplaySettings.unk498 = 0xC;
    temp_r3                  = (u8*)&g_DisplaySettings + 0x64;
    g_DisplaySettings.unk49C = 4;
    g_DisplaySettings.unk64  = 0;
    *(u16*)(temp_r3 + 0x4)   = 1;
    *(u16*)(temp_r3 + 0x1C)  = 0;
    *(u16*)(temp_r3 + 0x20)  = 0;
    *(u16*)(temp_r3 + 0x2C)  = 0;
    *(u16*)(temp_r3 + 0x28)  = 5;
    if (g_DisplaySettings.unkC == 0) {
        *(u16*)0x04000008 = (*(u16*)0x04000008 & 0x43) | 0x4014;
    }
    temp_ip                   = (u8*)&g_DisplaySettings + 0xA0;
    g_DisplaySettings.unkA0   = 0;
    *(u16*)(temp_ip + 0x4)    = 1;
    *(u16*)(temp_ip + 0x1C)   = 0;
    *(u16*)(temp_ip + 0x20)   = 2;
    *(u16*)(temp_ip + 0x28)   = 1;
    *(u16*)(temp_ip + 0x2C)   = 0;
    temp_ip_2                 = (u8*)&g_DisplaySettings + 0xDC;
    *(u16*)0x0400000A         = (u16)(((*(u16*)0x0400000A) & 0x43) | 0x4204);
    g_DisplaySettings.unkDC   = 0;
    *(u16*)(temp_ip_2 + 0x4)  = 1;
    *(u16*)(temp_ip_2 + 0x1C) = 0;
    *(u16*)(temp_ip_2 + 0x20) = 4;
    *(u16*)(temp_ip_2 + 0x28) = 1;
    *(u16*)(temp_ip_2 + 0x2C) = 1;
    temp_lr                   = (u8*)&g_DisplaySettings + 0x118;
    *(u16*)(0x0400000A + 2)   = (u16)(((*(u16*)(0x0400000A + 2)) & 0x43) | 0x4404);
    g_DisplaySettings.unk118  = 0;
    *(u16*)(temp_lr + 0x4)    = 1;
    *(u16*)(temp_lr + 0x1C)   = 0;
    *(u16*)(temp_lr + 0x20)   = 6;
    *(u16*)(temp_lr + 0x28)   = 3;
    *(u16*)(temp_lr + 0x2C)   = 1;
    *(u16*)(0x0400000A + 4)   = (u16)(((*(u16*)(0x0400000A + 4)) & 0x43) | 0x460C);
    Display_Commit();
    func_ov031_0210cdac(arg0);
    return 0;
}

void func_ov002_02088230(void* arg1) {
    u8* mgr = (u8*)arg1;

    OVMGR_U16(mgr, 0x46074) = 2;
    OVMGR_U32(mgr, 0x41FF0) = 0;
}

void func_ov002_0208824c(void* arg1) {
    OVMGR_U16(arg1, 0x41950) = 0;
}

s32 func_ov002_0208825c(void* arg0, void* arg1) {
    u8* entry       = (u8*)arg0;
    u8* mgr         = (u8*)arg1;
    u16 entry_index = *(u16*)(entry + 0x10);
    u16 slot_index  = *(u16*)(entry + 0x2A);

    if ((u32)entry_index >= 4U) {
        return 0;
    }
    func_0203b454((s32)(entry + 0x14), (s16*)((slot_index * 0x30) + (mgr + 0x41838)), 0x16);
    func_0203b454((s32)(entry + 0xA), (s16*)((slot_index * 0x30) + (mgr + 0x41862)), 6);
    OVMGR_U16(mgr, 0x41834)                    = (u16)(OVMGR_U16(mgr, 0x41834) | (1 << entry_index));
    OVMGR_U16(mgr, 0x41836)                    = (u16)(OVMGR_U16(mgr, 0x41836) | (1 << entry_index));
    *(u16*)(mgr + 0x4195A + (entry_index * 2)) = 0;
    return 1;
}

void func_ov002_02088310(void* arg0, void* arg1) {
    u8* entry   = (u8*)arg0;
    u8* mgr     = (u8*)arg1;
    u16 temp_r3 = *(u16*)(entry + 0x10);

    if ((u32)temp_r3 >= 4U) {
        return;
    }
    OVMGR_U16(mgr, 0x41834) = (u16)(OVMGR_U16(mgr, 0x41834) & ~(1 << temp_r3));
    OVMGR_U16(mgr, 0x41836) = (u16)(OVMGR_U16(mgr, 0x41836) | (1 << temp_r3));
}

void func_ov002_02088358(void* arg1) {
    OVMGR_U16(arg1, 0x4198C) = 0;
}

void func_ov002_02088368(s32 arg0, void* arg1, void* arg2, s32 arg3) {
    u8* mgr = (u8*)arg3;
    u16 value0;
    u16 value2;

    if (arg1 == NULL) {
        return;
    }
    if (arg2 != (void*)0xA) {
        return;
    }
    if (arg3 == 0) {
        return;
    }
    value0                              = *(u16*)arg1;
    value2                              = *(u16*)((u8*)arg1 + 2);
    *(u16*)(mgr + 0x4195A + (arg0 * 2)) = value0;
    *(u16*)(mgr + 0x41962 + (arg0 * 2)) = value2;
}

void func_ov002_020883a4(void* arg0) {
    u8* mgr = (u8*)arg0;

    OVMGR_U16(mgr, 0x41990) = 1;
    OVMGR_U16(mgr, 0x41954) = 1;
    OS_DisableIRQ();
    OVMGR_U16(mgr, 0x41836) = 0;
    OS_RestoreIRQ();
}

void func_ov002_020883d8(void* arg0) {
    s32 var_ip;
    u8* mgr = (u8*)arg0;

    OVMGR_U16(mgr, 0x41990) = 0;
    OVMGR_U16(mgr, 0x4198A) = func_02047e84(OVMGR_U16(mgr, 0x41834));
    var_ip                  = 1;
    OVMGR_U16(mgr, 0x4198C) = 1;
    if (OVMGR_U16(mgr, 0x41994) != 0) {
        if ((u32)OVMGR_U16(mgr, 0x4198A) >= 4U) {
            var_ip = 0;
        }
    } else {
        var_ip = 0;
    }
    if (func_ov040_0209d788(func_ov002_02088358, mgr, mgr + 0x4196E, 0x1E, var_ip) == 0) {
        OVMGR_U16(mgr, 0x4198C) = 0;
    }
    func_ov002_02082ab4(mgr);
}

s32 func_ov002_0208847c(void* arg0, void* arg1) {
    u8* mgr = (u8*)arg1;

    OVMGR_U16(mgr, 0x41834) = 0;
    OVMGR_U16(mgr, 0x41836) = 0;
    OVMGR_U16(mgr, 0x4198C) = 0;
    OVMGR_U16(mgr, 0x41990) = 0;
    OVMGR_U16(mgr, 0x41994) = 1;
    OVMGR_U16(mgr, 0x41950) = 0;
    OVMGR_U16(mgr, 0x41954) = 0;
    OVMGR_U16(mgr, 0x41958) = 0;
    func_0203b3c0(mgr + 0x4195A, 0, 8);
    func_0203b3c0(mgr + 0x41962, 0, 8);
    OVMGR_U16(mgr, 0x4196A) = 0;
    OVMGR_U16(mgr, 0x4198A) = 1;
    OVMGR_U16(mgr, 0x41834) = 1;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02088524(void* arg0, void* arg1) {
    s32 temp_r4;
    s32 temp_r5;
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        case 0:
        case 2:
        case 3:
        case 5:
        case 6:
            break;
        default:
            break;
        case 1:
            func_ov040_0209cb9c();
            break;
        case 7:
            temp_r5 = func_ov040_0209cde4();
            temp_r4 = func_020442f8();
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(1);
            func_ov040_0209cb08(arg1 + 0x4196E, 0x1E);
            func_ov040_0209d40c(func_ov002_0208825c, arg1);
            func_ov040_0209d420(func_ov002_02088310, arg1);
            func_ov003_0209d434(func_ov002_02088368, arg1);
            func_ov040_0209d0a8(0, temp_r4, temp_r5);
            break;
        case 4:
            func_ov031_0210cdac(arg0);
            break;
    }
    return 0;
}

s32 func_ov002_02088610(void* arg0, void* arg1) {
    u8* mgr = (u8*)arg1;
    u16 table_sp0[0xA];
    u16 i;

    for (i = 0; i < 5; i++) {
        table_sp0[i * 2]     = data_ov002_02091d10[i].unk0;
        table_sp0[i * 2 + 1] = data_ov002_02091d10[i].unk2;
    }
    func_ov031_0210c94c(mgr + 0x47C88, &data_ov002_02092df4, 0x40);
    func_ov031_0210cc84(mgr + 0x47C88, &data_ov002_020934b0, NULL);
    func_ov031_0210ce50(mgr + 0x41804, mgr + 0x47C88);
    OVMGR_U16(mgr, 0x46084) = 0;
    OVMGR_U16(mgr, 0x46086) = 0xC8;
    func_ov002_02082f18(mgr, 0xFFFF, 0x16, table_sp0);
    OVMGR_U16(mgr, 0x474C8) = 0x258;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020886ec(void* arg0, void* arg1) {
    s32   sp4;
    void* sp0;
    s32   var_r4;
    s32   var_r9;
    s32   var_r9_2;
    u16   temp_r6;
    u32   var_r0;
    void* temp_r7;

    sp0    = arg0;
    var_r4 = 0;
    func_ov002_020824a0();
    u8* mgr = (u8*)arg1;

    if (OVMGR_U16(mgr, 0x41836) != 0) {
        func_ov002_020883a4(mgr);
    }
    if ((OVMGR_U16(mgr, 0x41990) != 0) && (OVMGR_U16(mgr, 0x4198C) == 0)) {
        func_ov002_020883d8(mgr);
    }
    if ((OVMGR_U16(mgr, 0x41834) != 1) && (OVMGR_U16(mgr, 0x41950) == 0)) {
        if (OVMGR_U16(mgr, 0x41954) != 0) {
            OVMGR_U16(mgr, 0x41954) = 0;
            var_r4                  = 1;
            OVMGR_U16(mgr, 0x41958) = (u16)(OVMGR_U16(mgr, 0x41958) + 1);
        } else {
            var_r9 = 1;
        loop_16:
            if (var_r9 < 4) {
                temp_r6 = OVMGR_U16(mgr, 0x41834);
                if (temp_r6 & (1 << var_r9)) {
                    temp_r7 = mgr + (var_r9 * 2);
                    if (temp_r6 != *(u16*)(temp_r7 + 0x4195A)) {
                        var_r4 = 1;
                    } else if (OVMGR_U16(mgr, 0x41958) != *(u16*)(temp_r7 + 0x41962)) {
                        var_r4 = 1;
                    } else {
                        goto block_15;
                    }
                } else {
                block_15:
                    var_r9 += 1;
                    goto loop_16;
                }
            }
            if ((var_r4 == 0) && (OVMGR_U16(mgr, 0x4198A) == data_02074d10.unk40B)) {
                OVMGR_U16(mgr, 0x4196A) = 4U;
                var_r4                  = 1;
            }
        }
    }
    if (var_r4 != 0) {
        sp4                    = OS_DisableIRQ();
        data_ov002_020935e0[0] = OVMGR_U16(mgr, 0x4196A);
        if (OVMGR_U16(mgr, 0x4196A) != 0) {

        } else {
            var_r9_2                  = 0;
            data_ov002_020935e0[1]    = OVMGR_U16(mgr, 0x41834);
            data_ov002_020935e0[0x3A] = OVMGR_U16(mgr, 0x41958);
        loop_31:
            if (var_r9_2 < 4) {
                if (OVMGR_U16(mgr, 0x41834) & (1 << var_r9_2)) {
                    if (var_r9_2 == 0) {
                        var_r0 = 0;
                    loop_28:
                        if (var_r0 < 0xBU) {
                            var_r0 = (u32)(u16)(var_r0 + 1);
                            goto loop_28;
                        }
                    }
                    func_0203b454((var_r9_2 * 0x30) + (s32)(mgr + 0x41838),
                                  (s16*)((var_r9_2 * 0x16) + (s32)&data_ov002_020935e0[2]), 0x16);
                }
                var_r9_2 += 1;
                goto loop_31;
            }
        }
        OS_RestoreIRQ(sp4);
        OVMGR_U16(mgr, 0x41950) = 1;
        if (func_ov040_0209d48c(data_ov002_020935e0, 0x78, func_ov002_0208824c, mgr) == 0) {
            OVMGR_U16(mgr, 0x41950) = 0;
        }
    }
    OVMGR_U16(mgr, 0x474C8) = (u16)(OVMGR_U16(mgr, 0x474C8) - 1);
    if (OVMGR_U16(mgr, 0x474C8) == 0) {
        func_ov031_0210cc84(mgr + 0x4161C, &data_ov002_02092e18, NULL);
        return 0;
    }
    if (OVMGR_U16(mgr, 0x4196A) == 4) {
        func_ov031_0210cdac(sp0);
    }
    return 0;
}

s32 func_ov002_02088998(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            table_sp0[0xA];
    u16            i;

    for (i = 0; i < 5; i++) {
        table_sp0[i * 2]     = data_ov002_02091ce8[i].unk0;
        table_sp0[i * 2 + 1] = data_ov002_02091ce8[i].unk2;
    }
    func_ov031_0210cff0((u8*)mgr + 0x41804, (u8*)mgr + 0x47C88);
    func_ov002_02084494(mgr, data_02074d10.unk40A, table_sp0);
    OVMGR_U32(mgr_bytes, 0x46300) = 1;
    func_ov031_0210c94c((u8*)mgr + 0x474E8, &data_ov002_02092e04, 0x40);
    func_ov031_0210cc84((u8*)mgr + 0x474E8, &data_ov002_02093240, NULL);
    func_ov031_0210ce50((u8*)mgr + 0x41804, (u8*)mgr + 0x474E8);
    OVMGR_U16(mgr_bytes, 0x4196C) = 0xFFFF;
    OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFF;
    OVMGR_U16(mgr_bytes, 0x474CA) = OVMGR_U16(mgr_bytes, 0x41834);
    func_ov031_0210cdac(arg0);
    return 0;
}

void func_ov002_02088aa4(void* arg0, void* arg1) {
    s32 temp_r5;
    u16 temp_r0;

    temp_r5                = OS_DisableIRQ();
    data_ov002_020935e0[0] = OVMGR_U16(arg1, 0x4196A);
    temp_r0                = OVMGR_U16(arg1, 0x4196A);
    switch (temp_r0) { /* irregular */
        case 5:
            data_ov002_020935e0[0x3B] = OVMGR_U16(arg1, 0x4196C);
            break;
        case 6:
            func_ov031_0210cdac(arg0);
            break;
    }
    OS_RestoreIRQ(temp_r5);
    func_ov040_0209d48c(data_ov002_020935e0, 0x78, func_ov002_0208824c, arg1);
}

s32 func_ov002_02088b28(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            table_sp36[0x32];
    u16            table_sp0[0x1B];
    u16            temp_r4;
    u16            i;

    for (i = 0; i < 0x19 * 2; i++) {
        table_sp36[i] = ((const u16*)data_ov002_02091f7c)[i];
    }
    for (i = 0; i < 0x1B; i++) {
        table_sp0[i] = data_ov002_02091e9a[i];
    }

    temp_r4 = func_ov002_0208597c(table_sp36);
    if (OVMGR_U16(mgr_bytes, 0x474CA) != func_ov040_0209cb68()) {
        func_ov031_0210cc84((u8*)mgr + 0x4161C, &data_ov002_02092e18, NULL);
        return 0;
    }
    if (temp_r4 == 0xFFFF) {
        return 0;
    }
    if (temp_r4 == 0xFFFE) {
        OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFF;
        OVMGR_U16(mgr_bytes, 0x46084) = 0U;
        OVMGR_U16(mgr_bytes, 0x46086) = 0xD2U;
        OVMGR_U16(mgr_bytes, 0x4196A) = 5;
        OVMGR_U16(mgr_bytes, 0x4196C) = 0xFFFFU;
        func_ov002_0208800c(mgr);
    } else if (OVMGR_U16(mgr_bytes, 0x4196C) == temp_r4) {
        OVMGR_U16(mgr_bytes, 0x4196A) = 6;
        func_02026b20(0x347);
    } else {
        u16 anim_index = (u16)(temp_r4 * 3);

        func_02026b20(0x346);
        OVMGR_U16(mgr_bytes, 0x4196C) = temp_r4;
        OVMGR_U16(mgr_bytes, 0x46084) = table_sp0[anim_index + 1];
        OVMGR_U16(mgr_bytes, 0x46086) = table_sp0[anim_index + 2];
        Sprite_ChangeAnimation((Sprite*)(mgr_bytes + 0x46078), OVMGR_PTR(mgr_bytes, 0x46090), (s16)table_sp0[anim_index],
                               OVMGR_PTR(mgr_bytes, 0x46094));
        OVMGR_U16(mgr_bytes, 0x4196A) = 5;
        func_ov002_0208800c(mgr);
    }
    func_ov002_02088aa4(arg0, mgr);
    return 0;
}

void func_ov002_02088d00(void* arg1, void* arg2, void* arg3) {
    typedef struct {
        u16 unk0;
        u16 unk2;
        u8  filler04[0x70];
        u16 unk74;
        u16 unk76;
    } Ov002_88d00Data;
    Ov002_88d00Data* msg       = (Ov002_88d00Data*)arg1;
    Ov002_Manager*   mgr       = (Ov002_Manager*)arg3;
    u8*              msg_bytes = (u8*)arg1;
    s16*             var_r5;
    s32              var_r4;
    u16              temp_r0;
    void*            var_r6;

    if (arg1 == NULL) {
        return;
    }
    if (arg2 != (void*)0x78) {
        return;
    }
    if (arg3 == NULL) {
        return;
    }
    OVMGR_U16(mgr, 0x4196A) = (u16)msg->unk0;
    temp_r0                 = msg->unk0;
    switch (temp_r0) { /* irregular */
        case 3:
            return;
        case 0:
            var_r6 = msg_bytes + 4;
            var_r5 = (s16*)((u8*)mgr + 0x41838);
            var_r4 = 0;
            do {
                if (msg->unk2 & (1 << var_r4)) {
                    func_0203b454((s32)var_r6, var_r5, 0x16);
                }
                var_r4 += 1;
                var_r5 += 0x30;
                var_r6 += 0x16;
            } while (var_r4 < 4);
            OVMGR_U16(mgr, 0x41834) = (u16)msg->unk2;
            OVMGR_U16(mgr, 0x41836) = 0xF;
            OVMGR_U16(mgr, 0x41958) = (u16)msg->unk74;
            return;
    }
}

s32 func_ov002_02088dc0(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            table_sp0[0xA];
    u16            i;

    for (i = 0; i < 5; i++) {
        table_sp0[i * 2]     = data_ov002_02091cfc[i].unk0;
        table_sp0[i * 2 + 1] = data_ov002_02091cfc[i].unk2;
    }
    func_ov031_0210c94c((u8*)mgr + 0x47C88, &data_ov002_02092df4, 0x40);
    func_ov031_0210cc84((u8*)mgr + 0x47C88, &data_ov002_020934b0, NULL);
    func_ov031_0210ce50((u8*)mgr + 0x41804, (u8*)mgr + 0x47C88);
    OVMGR_U16(mgr_bytes, 0x46084) = 0;
    OVMGR_U16(mgr_bytes, 0x46086) = 0xC8;
    func_ov002_02082f18(mgr, 0xFFFF, 0x16, table_sp0);
    OVMGR_U16(mgr_bytes, 0x474C8) = 0x258;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02088e9c(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u32            temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        case 0:
        case 2:
        case 3:
            break;
        default:
            break;
        case 1:
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(1);
            func_ov040_0209cabc((u8*)mgr + 0x4181C, 0x18);
            func_ov040_0209b8b8(1, (u8*)mgr + 0x41862, 0);
            func_ov003_0209d434((void (*)(s32, void*, void*, s32))func_ov002_02088d00, arg1);
            break;
        case 4:
            func_ov031_0210cdac(arg0);
            break;
    }
    OVMGR_U16(mgr_bytes, 0x474C8) = (u16)(OVMGR_U16(mgr_bytes, 0x474C8) - 1);
    if (OVMGR_U16(mgr_bytes, 0x474C8) != 0) {
        return 0;
    }
    func_ov031_0210cc84((u8*)mgr + 0x4161C, &data_ov002_02092e18, NULL);
    return 0;
}

void func_ov002_02088f90(void* arg1) {
    OVMGR_U16(arg1, 0x41950) = 0;
}

s32 func_ov002_02088fa0(void* arg1) {
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    OVMGR_U16(arg1, 0x474CA) = 0;
    OVMGR_U16(arg1, 0x474CC) = 0;
    func_ov031_0210cdac();
    return 0;
}

s32 func_ov002_02088fd8(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;

    if (OVMGR_U16(mgr_bytes, 0x41836) != 0) {
        func_ov002_02082ab4(mgr);
        OVMGR_U16(mgr_bytes, 0x474CC) = 1U;
    }
    if ((OVMGR_U16(mgr_bytes, 0x41950) == 0) && (OVMGR_U16(mgr_bytes, 0x474CC) != 0)) {
        OS_DisableIRQ();
        data_ov002_020935c0[0] = OVMGR_U16(mgr_bytes, 0x41834);
        data_ov002_020935c0[1] = OVMGR_U16(mgr_bytes, 0x41958);
        OS_RestoreIRQ();
        OVMGR_U16(mgr_bytes, 0x474CC) = 0U;
        OVMGR_U16(mgr_bytes, 0x41950) = 1;
        if (func_ov040_0209d48c(data_ov002_020935c0, 0xA, func_ov002_02088f90, arg1) == 0) {
            OVMGR_U16(mgr_bytes, 0x41950) = 0;
        }
    }
    if (OVMGR_U16(mgr_bytes, 0x4196A) == 4) {
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

void func_ov002_020890c0(void* arg1, void* arg2, void* arg3) {
    typedef struct {
        u16 unk0;
        u16 unk2;
        u8  filler04[0x70];
        u16 unk74;
        u16 unk76;
    } Ov002_88d00Data;
    Ov002_88d00Data* msg = (Ov002_88d00Data*)arg1;
    Ov002_Manager*   mgr = (Ov002_Manager*)arg3;

    if (arg1 == NULL) {
        return;
    }
    if (arg2 != (void*)0x78) {
        return;
    }
    if (arg3 == NULL) {
        return;
    }
    OVMGR_U16(mgr, 0x4196A) = (u16)msg->unk0;
    OVMGR_U16(mgr, 0x4196C) = (u16)msg->unk76;
}

s32 func_ov002_020890f8(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            table_sp0[0xA];
    u16            i;

    for (i = 0; i < 5; i++) {
        table_sp0[i * 2]     = data_ov002_02091cd4[i].unk0;
        table_sp0[i * 2 + 1] = data_ov002_02091cd4[i].unk2;
    }
    func_ov031_0210cff0((u8*)mgr + 0x41804, (u8*)mgr + 0x47C88);
    func_ov031_0210c94c((u8*)mgr + 0x474E8, &data_ov002_02092e04, 0x40);
    func_ov031_0210cc84((u8*)mgr + 0x474E8, &data_ov002_02093240, NULL);
    func_ov031_0210ce50((u8*)mgr + 0x41804, (u8*)mgr + 0x474E8);
    OVMGR_U32(mgr_bytes, 0x46300) = 1;
    OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFF;
    func_ov002_02084494(mgr, data_02074d10.unk40A, table_sp0);
    OVMGR_U16(mgr_bytes, 0x474CA) = OVMGR_U16(mgr_bytes, 0x4196C);
    func_ov003_0209d434((void (*)(s32, void*, void*, s32))func_ov002_020890c0, arg1);
    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov002_0208920c(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            table_sp0[0x1B];
    u16            temp_r7;
    u16            temp_r3;
    u16            i;

    for (i = 0; i < 0x1B; i++) {
        table_sp0[i] = data_ov002_02091e64[i];
    }
    if (OVMGR_U16(mgr_bytes, 0x4196A) == 5) {
        temp_r3 = OVMGR_U16(mgr_bytes, 0x4196C);
        if (temp_r3 == 0xFFFF) {
            OVMGR_U16(mgr_bytes, 0x474CA) = temp_r3;
            OVMGR_U16(mgr_bytes, 0x46084) = 0U;
            OVMGR_U16(mgr_bytes, 0x46086) = 0xD2U;
            OVMGR_U16(mgr_bytes, 0x4196A) = 5U;
            func_ov002_0208800c(mgr);
        } else if (OVMGR_U16(mgr_bytes, 0x474CA) != temp_r3) {
            func_02026b20(0x346);
            OVMGR_U16(mgr_bytes, 0x474CA) = OVMGR_U16(mgr_bytes, 0x4196C);
            temp_r7                       = (u16)(OVMGR_U16(mgr_bytes, 0x4196C) * 3);
            OVMGR_U16(mgr_bytes, 0x46084) = table_sp0[temp_r7 + 1];
            OVMGR_U16(mgr_bytes, 0x46086) = table_sp0[temp_r7 + 2];
            Sprite_ChangeAnimation((Sprite*)(mgr_bytes + 0x46078), OVMGR_PTR(mgr_bytes, 0x46090), (s16)table_sp0[temp_r7],
                                   OVMGR_PTR(mgr_bytes, 0x46094));
            func_ov002_0208800c(mgr);
        }
    }
    if (OVMGR_U16(mgr_bytes, 0x4196A) == 6) {
        func_02026b20(0x347);
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_02089364(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        default:
            break;
        case 0:
            OS_WaitForever();
            break;
        case 1:
            func_ov031_0210cdac(arg0);
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return 0;
}

s32 func_ov002_020893b8(void* arg1) {
    OVMGR_U16(arg1, 0x474C8) = 0;
    func_ov031_0210cdac();
    return 0;
}

s32 func_ov002_020893d8(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;

    if (func_ov040_0209cb68() == 1) {
        func_ov031_0210cdac(arg0);
        return 0;
    }
    OS_DisableIRQ();
    *data_ov002_020935e0 = 6;
    OS_RestoreIRQ();
    if (func_ov040_0209d48c(data_ov002_020935e0, 0x78, func_ov002_0208824c, arg1) == 0) {
        func_ov031_0210cc84((u8*)mgr + 0x4161C, &data_ov002_02092e18, NULL);
        return 0;
    }
    OVMGR_U16(mgr_bytes, 0x474C8) = (u16)(OVMGR_U16(mgr_bytes, 0x474C8) + 1);
    if (OVMGR_U16(mgr_bytes, 0x474C8) == 0xB4) {
        func_ov031_0210cc84((u8*)mgr + 0x4161C, &data_ov002_02092e18, NULL);
    }
    return 0;
}

s32 func_ov002_02089498(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        default:
        case 2:
            func_ov040_0209d588();
            break;
        case 0:
            OS_WaitForever();
            break;
        case 1:
            func_ov031_0210cdac(arg0);
            break;
        case 4:
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
            break;
    }
    return 0;
}

void func_ov002_020894f8(void) {}

void func_ov002_020894fc(void* arg1) {
    func_ov002_02085710(arg1);
}

void func_ov002_0208950c(void) {}

void func_ov002_02089510(void) {}

s32 func_ov002_02089514(void* arg0, s32 arg1) {
    u8* mgr = (u8*)arg1;

    func_ov031_0210cff0(mgr + 0x41804, mgr + 0x474E8);
    func_ov031_0210cff0(mgr + 0x41804, mgr + 0x476D0);
    func_ov031_0210cff0(mgr + 0x41804, mgr + 0x478B8);
    func_ov031_0210cff0(mgr + 0x41804, mgr + 0x47AA0);
    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov002_0208958c(void* arg0) {
    g_DisplaySettings.unk224 = 0x1F;
    g_DisplaySettings.unk228 = 1;
    g_DisplaySettings.unk22C = 0x1F;
    g_DisplaySettings.unk230 = 1;
    Display_Commit();
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020895c8(void* arg0) {
    s32 temp_r0;

    temp_r0 = (s32)func_ov040_0209cb78();
    switch (temp_r0) { /* irregular */
        case 3:
            break;
        case 10:
        case 0:
            func_ov031_0210cdac(arg0);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        default:
            func_ov040_0209d540();
            break;
    }
    return 0;
}

s32 func_ov002_02089624(void* arg0, void* arg1) {
    u8*  mgr_bytes = (u8*)arg1;
    s32  var_r2;
    u16* var_lr;
    u16* var_r3;
    u16  temp_r0;
    u16  temp_r1;

    var_r3 = data_ov002_0209200e;
    var_lr = subroutine_arg0;
    var_r2 = 7;
    do {
        temp_r1 = var_r3[0];
        temp_r0 = var_r3[1];
        var_r3 += 4;
        var_lr[0] = temp_r1;
        var_lr[1] = temp_r0;
        var_lr += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);
    SystemStatusFlags |= 0x40;
    *var_lr = *var_r3;
    SystemStatusFlags |= 0x80;
    func_ov002_02082f18(arg1, 0x18, 0x1B, subroutine_arg0);
    OVMGR_U16(mgr_bytes, 0x46084) = 0;
    OVMGR_U16(mgr_bytes, 0x46086) = 0xC8;
    OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020896e4(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            table_sp0[0xA];
    u16            i;

    for (i = 0; i < 5; i++) {
        table_sp0[i * 2]     = data_ov002_02091fe6[i].unk0;
        table_sp0[i * 2 + 1] = data_ov002_02091fe6[i].unk2;
    }
    func_020273a8(0x19, 1);
    SystemStatusFlags |= 0x40;
    SystemStatusFlags |= 0x80;
    func_ov002_02085710(arg1);
    func_ov002_0208bd40();
    func_ov002_02083484(arg1, table_sp0);
    OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02089798(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    s32            var_r2;
    u16*           var_lr;
    u16*           var_r3;
    u16            temp_r0;
    u16            temp_r1;

    var_r3 = data_ov002_0209202c;
    var_lr = subroutine_arg0;
    var_r2 = 7;
    do {
        temp_r1 = var_r3[0];
        temp_r0 = var_r3[1];
        var_r3 += 4;
        var_lr[0] = temp_r1;
        var_lr[1] = temp_r0;
        var_lr += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);
    SystemStatusFlags |= 0x40;
    *var_lr = *var_r3;
    SystemStatusFlags |= 0x80;
    func_ov002_02082f18(mgr, 0x18, 0x1B, subroutine_arg0);
    OVMGR_U16(mgr_bytes, 0x46084) = 0;
    OVMGR_U16(mgr_bytes, 0x46086) = 0xC8;
    OVMGR_U16(mgr_bytes, 0x460CC) = 0;
    OVMGR_U16(mgr_bytes, 0x460CE) = 0xC8;
    OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02089860(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    s32            var_r3;
    u16*           var_r2;
    u16*           var_r6;
    u16            temp_r0;
    u16            temp_r1;

    var_r6 = data_ov002_0209204a;
    var_r2 = subroutine_arg0;
    var_r3 = 7;
    do {
        temp_r1 = var_r6[0];
        temp_r0 = var_r6[1];
        var_r6 += 4;
        var_r2[0] = temp_r1;
        var_r2[1] = temp_r0;
        var_r2 += 4;
        var_r3 -= 1;
    } while (var_r3 != 0);
    *var_r2 = *var_r6;
    SystemStatusFlags |= 0x40;
    SystemStatusFlags |= 0x80;
    OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFF;
    func_ov002_02082f18(mgr, 0x18, 0x1B, subroutine_arg0);
    OVMGR_U16(mgr_bytes, 0x46084) = 0;
    OVMGR_U16(mgr_bytes, 0x46086) = 0xC8;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02089920(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            sp6;
    u16            sp4;
    u16            sp2;
    s32            temp_ip;
    s32            var_r2;
    u16            temp_r0_2;
    u16            table_sp6[0xA];
    u16            i;

    for (i = 0; i < 5; i++) {
        table_sp6[i * 2]     = data_ov002_02091ffa[i].unk0;
        table_sp6[i * 2 + 1] = data_ov002_02091ffa[i].unk2;
    }
    sp2       = data_ov002_02091fe0.unk2;
    sp4       = data_ov002_02091fe0.unk4;
    temp_r0_2 = func_ov002_0208597c(table_sp6, data_ov002_02091fe0.unk0);
    if (temp_r0_2 == 0xFFFF) {
        return 0;
    }
    if (temp_r0_2 == 0xFFFE) {
        OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFFU;
        OVMGR_U16(mgr_bytes, 0x46084) = 0U;
        OVMGR_U16(mgr_bytes, 0x46086) = 0xD2U;
        return 0;
    }
    if (OVMGR_U16(mgr_bytes, 0x474C8) == temp_r0_2) {
        if (temp_r0_2 == 0) {
            OVMGR_U16(mgr_bytes, 0x46084) = 0U;
            OVMGR_U16(mgr_bytes, 0x46086) = 0xD2U;
            func_02026b20(0x347);
            func_ov031_0210cdac(arg0);
            return 0;
        }
        /* Duplicate return node #11. Try simplifying control flow for better match */
        return 0;
    }
    OVMGR_U16(mgr_bytes, 0x474C8) = temp_r0_2;
    func_02026b20(0x346);
    if (temp_r0_2 == 0) {
        temp_ip                       = temp_r0_2 * 6;
        OVMGR_U16(mgr_bytes, 0x46084) = (u16) * (&sp2 + temp_ip);
        OVMGR_U16(mgr_bytes, 0x46086) = (u16) * (&sp4 + temp_ip);
        Sprite_ChangeAnimation((Sprite*)(mgr_bytes + 0x46078), OVMGR_PTR(mgr_bytes, 0x46090), (s16)table_sp6[0],
                               OVMGR_PTR(mgr_bytes, 0x46094));
        return 0;
    }
    return 0;
}

s32 func_ov002_02089a94(void* arg0) {
    if (SysControl.unk2 & 1) {
        func_02026b20(0x348);
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_02089ac8(void* arg1) {
    OVMGR_U16(arg1, 0x46074) = 3;
    return 0;
}

s32 func_ov002_02089adc(void* arg1) {
    OVMGR_U16(arg1, 0x46074) = 5;
    return 0;
}

s32 func_ov002_02089af0(void* arg0, void* arg1) {
    func_ov002_02085710(arg1);
    func_ov031_0210cc84(arg0, &data_ov002_02092c9c, NULL);
    return 0;
}

s32 func_ov002_02089b1c(void* arg0, void* arg1) {
    func_ov002_02085710(arg1);
    func_ov031_0210cdac(arg0);
    return 0;
}

void func_ov002_02089b3c(void* arg1) {
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xC8;
}

void func_ov002_02089b54(void) {
    g_DisplaySettings.unk224 = 0x1F;
    g_DisplaySettings.unk228 = 1;
    g_DisplaySettings.unk22C = 0x1F;
    g_DisplaySettings.unk230 = 1;
    Display_Commit();
}

void func_ov002_02089b80(void* arg1) {
    func_ov002_02085710(arg1);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xC8;
}

void func_ov002_02089ba8(void) {}

void func_ov002_02089bac(void) {}

s32 func_ov002_02089bb0(void) {
    func_ov031_0210cdac();
    return 0;
}

void func_ov002_02089bc0(void* arg1) {
    ((Ov002_Manager*)arg1)->unk41950 = 0;
}

s32 func_ov002_02089bd0(void) {
    return 0;
}

s32 func_ov002_02089bd8(void* arg0, void* arg1) {
    typedef struct {
        u8  filler10[0x10];
        u16 unk10;
    } Ov002_89bd8Data;
    Ov002_89bd8Data* msg = (Ov002_89bd8Data*)arg0;
    Ov002_Manager*   mgr = (Ov002_Manager*)arg1;
    u16              temp_r2;

    temp_r2 = msg->unk10;
    if ((u32)temp_r2 >= 4U) {
        return 0;
    }
    func_0203b454((s32)((u8*)msg + 0x14), (s16*)((temp_r2 * 0x30) + ((u8*)mgr + 0x41838)), 0x16);
    func_0203b454((s32)((u8*)msg + 0x0A), (s16*)((msg->unk10 * 0x30) + ((u8*)mgr + 0x41862)), 6);
    mgr->unk41834                                  = (u16)(mgr->unk41834 | (1 << msg->unk10));
    mgr->unk41836                                  = (u16)(mgr->unk41836 | (1 << msg->unk10));
    *(u16*)((u8*)mgr + 0x4195A + (msg->unk10 * 2)) = 0;
    return 1;
}

void func_ov002_02089c88(void* arg0, void* arg1) {
    typedef struct {
        u8  filler10[0x10];
        u16 unk10;
    } Ov002_89bd8Data;
    Ov002_89bd8Data* msg = (Ov002_89bd8Data*)arg0;
    Ov002_Manager*   mgr = (Ov002_Manager*)arg1;
    u16              temp_r3;

    temp_r3 = msg->unk10;
    if ((u32)temp_r3 >= 4U) {
        return;
    }
    mgr->unk41834 = (u16)(mgr->unk41834 & ~(1 << temp_r3));
    mgr->unk41836 = (u16)(mgr->unk41836 | (1 << msg->unk10));
}

void func_ov002_02089cd0(void* arg1) {
    ((Ov002_Manager*)arg1)->unk4198C = 0;
}

void func_ov002_02089ce0(s32 arg0, void* arg1, void* arg2, s32 arg3) {
    typedef struct {
        u16 unk0;
        u16 unk2;
    } Ov002_89ce0Data;
    Ov002_89ce0Data* msg = (Ov002_89ce0Data*)arg1;
    void*            temp_r5;

    if (arg1 == NULL) {
        return;
    }
    if (arg2 != (void*)0xA) {
        return;
    }
    if (arg3 == 0) {
        return;
    }
    temp_r5                         = (u8*)arg3 + (arg0 * 2);
    *(u16*)((u8*)temp_r5 + 0x4195A) = (u16)msg->unk0;
    *(u16*)((u8*)temp_r5 + 0x41962) = (u16)msg->unk2;
    func_0203b454((s32)((u8*)msg + 4), (s16*)((arg0 * 6) + ((u8*)arg3 + 0x41FB6)));
}

s32 func_ov002_02089d40(void* arg0, void* arg1) {
    s32 temp_r0;

    Ov002_Manager* mgr = (Ov002_Manager*)arg1;

    mgr->unk41834 = 0;
    mgr->unk41836 = 0;
    mgr->unk4198C = 0;
    mgr->unk41990 = 0;
    mgr->unk41994 = 1;
    mgr->unk41950 = 0;
    mgr->unk41954 = 0;
    mgr->unk41958 = 0;
    func_0203b3c0((u8*)mgr + 0x4195A, 0, 8);
    func_0203b3c0((u8*)mgr + 0x41962, 0, 8);
    mgr->unk4196A = 0;
    mgr->unk4198A = 1;
    mgr->unk41834 = 1;
    temp_r0       = func_ov031_0210c7c4();
    func_0203b454(temp_r0, (u8*)mgr + 0x4196E, 0x16);
    func_0203b454(temp_r0, (u8*)mgr + 0x41838, 0x16);
    Mem_Free(&gDebugHeap, temp_r0);
    func_0203a96c((u8*)mgr + 0x41862);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02089e3c(void* arg0, void* arg1) {
    s32 temp_r4;
    s32 temp_r5;
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        default:
            break;
        case 0:
            func_ov040_0209cf20();
            func_ov040_0209caac(0x400548);
            break;
        case 1:
            func_ov040_0209cb9c();
            break;
        case 7:
            temp_r5 = func_ov040_0209cde4();
            temp_r4 = func_020442f8();
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(0);
            func_ov040_0209cb08((u8*)arg1 + 0x4196E, 0x1E);
            func_ov040_0209d40c(func_ov002_02089bd8, arg1);
            func_ov040_0209d420(func_ov002_02089c88, arg1);
            func_ov003_0209d434(func_ov002_02089ce0, arg1);
            func_ov040_0209d0a8(0, temp_r4, temp_r5);
            break;
        case 4:
            func_ov031_0210cdac(arg0);
            break;
    }
    return 0;
}

void func_ov002_02089f3c(void* arg0, s32 arg1) {
    Ov002_Manager* mgr = (Ov002_Manager*)arg0;
    u8*            pack;
    u8*            base;
    u8*            var_r0;
    u8*            var_r0_2;

    pack = (u8*)OVMGR_PTR(mgr, 0x462F0);
    if (arg1 != 0) {
        if (pack == NULL) {
            var_r0 = NULL;
        } else {
            base   = OVPK_HDR(pack);
            var_r0 = base + OVPK_OFF(base, 0xB8);
        }
        OVMGR_PTR(mgr, 0x474B0)  = (void*)(var_r0 + 4);
        g_DisplaySettings.unk224 = 0x1F;
        g_DisplaySettings.unk228 = 1;
        g_DisplaySettings.unk22C = 0x1F;
        g_DisplaySettings.unk230 = 1;
    } else {
        if (pack == NULL) {
            var_r0_2 = NULL;
        } else {
            base     = OVPK_HDR(pack);
            var_r0_2 = base + OVPK_OFF(base, 0xC0);
        }
        OVMGR_PTR(mgr, 0x474B0)  = (void*)(var_r0_2 + 4);
        g_DisplaySettings.unk240 = 0xA8000;
        g_DisplaySettings.unk234 = 0xA0000;
        g_DisplaySettings.unk23C = 0xF8000;
        g_DisplaySettings.unk238 = 0xB8000;
        g_DisplaySettings.unk224 = 0x1E;
        g_DisplaySettings.unk228 = 1;
        g_DisplaySettings.unk22C = 0x1F;
        g_DisplaySettings.unk230 = 1;
        g_DisplaySettings.unk20  = (s32)(g_DisplaySettings.unk20 | 1);
    }
    Display_Commit();
    func_0200d1d8((u8*)mgr + 0x47410, 0, 1, 0, (u8*)mgr + 0x474B0, 1, 1);
}

s32 func_ov002_0208a050(void* arg0, void* arg1) {
    func_ov002_02082ab4(arg1);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208a070(void* arg0, void* arg1) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg1;
    u8*            mgr_bytes = (u8*)mgr;
    u16            table_sp0[0x14];
    u16            i;

    for (i = 0; i < 0x14; i++) {
        table_sp0[i] = data_ov002_02092106.data[i];
    }
    func_ov002_02082f18(mgr, 0x18, 0x16, table_sp0);
    func_ov002_02089f3c(mgr, 0);
    OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

void func_ov002_0208a0f8(void* arg0) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg0;
    u8*            mgr_bytes = (u8*)mgr;
    s16*           var_r7;
    s32            temp_r11;
    s32            temp_r9;
    s32            var_r6;
    void*          var_r8;

    OVMGR_U16(mgr_bytes, 0x41990) = 1;
    OVMGR_U16(mgr_bytes, 0x41954) = 1;
    var_r6                        = 0;
    var_r7                        = (s16*)subroutine_arg0;
    temp_r9                       = OVMGR_U16(mgr_bytes, 0x41834) & OVMGR_U16(mgr_bytes, 0x41836);
    temp_r11                      = OS_DisableIRQ();
    OVMGR_U16(mgr_bytes, 0x41836) = 0U;
    var_r8                        = (u8*)mgr + 0x41838;
    do {
        if (temp_r9 & (1 << var_r6)) {
            func_0203b454((s32)var_r8, var_r7, 0x16);
        }
        var_r6 += 1;
        var_r7 += 0x30;
        var_r8 += 0x30;
    } while (var_r6 < 4);
    OS_RestoreIRQ(temp_r11);
    func_ov002_02082ab4(mgr);
}

void func_ov002_0208a198(void* arg0) {
    Ov002_Manager* mgr       = (Ov002_Manager*)arg0;
    u8*            mgr_bytes = (u8*)mgr;
    s32            var_ip;
    u16            temp_r0;

    OVMGR_U16(mgr_bytes, 0x41990) = 0;
    OVMGR_U16(mgr_bytes, 0x4198A) = func_02047e84(OVMGR_U16(mgr_bytes, 0x41834));
    var_ip                        = 1;
    OVMGR_U16(mgr_bytes, 0x4198C) = 1;
    if (OVMGR_U16(mgr_bytes, 0x41994) != 0) {
        if ((u32)OVMGR_U16(mgr_bytes, 0x4198A) >= 4U) {
            var_ip = 0;
        }
    } else {
        var_ip = 0;
    }
    if (func_ov040_0209d788(func_ov002_02089cd0, mgr, (u8*)mgr + 0x4196E, 0x1E, var_ip) == 0) {
        OVMGR_U16(mgr_bytes, 0x4198C) = 0;
    }
    temp_r0 = OVMGR_U16(mgr_bytes, 0x4198A);
    func_ov002_02089f3c(mgr, (temp_r0 > 1U) ? 1 : 0);
}

s32 func_ov002_0208a250(void* arg0, void* arg1) {
    Ov002_Manager*     mgr       = (Ov002_Manager*)arg1;
    u8*                mgr_bytes = (u8*)mgr;
    u16                sp14;
    u16                sp12;
    u16                sp10;
    u16                spE;
    u16                spC;
    u16                spA;
    u16                sp8;
    s32                sp4;
    void*              sp0;
    s32                temp_r4;
    s32                temp_r6;
    s32                var_r0;
    s32                var_r2;
    s32                var_r5;
    s32                var_r9;
    s32                var_r9_2;
    const Ov002_U16_2* var_r3;
    Ov002_U16_2*       var_r4;
    u16                temp_r0;
    u16                temp_r1;
    u16                temp_r6_2;
    u16                temp_r8;
    u16                var_r4_2;
    void*              temp_r1_2;
    void*              temp_r7;
    void*              temp_r9;

    var_r3 = (const Ov002_U16_2*)data_ov002_020920e8;
    var_r4 = (Ov002_U16_2*)&sp14;
    sp0    = arg0;
    var_r5 = 0;
    var_r2 = 7;
    do {
        temp_r1 = var_r3->unk0;
        temp_r0 = var_r3->unk2;
        var_r3 += 4;
        var_r4->unk0 = temp_r1;
        var_r4->unk2 = temp_r0;
        var_r4 += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);
    *var_r4  = *var_r3;
    spA      = data_ov002_02092068.unkE;
    sp8      = data_ov002_02092068.unkC;
    spE      = data_ov002_02092068.unk12;
    spC      = data_ov002_02092068.unk10;
    sp10     = data_ov002_02092068.unk14;
    sp12     = data_ov002_02092068.unk16;
    var_r4_2 = func_ov002_0208597c(&sp14);
    if ((var_r4_2 == 1) && (OVMGR_U16(mgr_bytes, 0x4198A) == 1)) {
        var_r4_2 = 0xFFFF;
    }
    if (var_r4_2 == 0xFFFE) {
        var_r4_2                      = 0xFFFF;
        OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFFU;
        OVMGR_U16(mgr_bytes, 0x46084) = 0U;
        OVMGR_U16(mgr_bytes, 0x46086) = 0xC8U;
    }
    if ((OVMGR_U16(mgr_bytes, 0x474C8) == 1) && (OVMGR_U16(mgr_bytes, 0x4198A) == 1)) {
        OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFFU;
        OVMGR_U16(mgr_bytes, 0x46084) = 0U;
        OVMGR_U16(mgr_bytes, 0x46086) = 0xC8U;
    }
    if (var_r4_2 != 0xFFFF) {
        if (OVMGR_U16(mgr_bytes, 0x474C8) == var_r4_2) {
            switch (var_r4_2) { /* irregular */
                case 0:
                    func_02026b20(0x348);
                    func_ov031_0210cc84(sp0, &data_ov002_02092c9c, NULL);
                    OVMGR_U16(mgr_bytes, 0x460CC) = 0;
                    OVMGR_U16(mgr_bytes, 0x460CE) = 0xC8;
                    func_ov031_0210cd48(sp0, &data_ov002_02092f38);
                    func_ov031_0210cd48(sp0, &data_ov002_02092b00);
                    return 0;
                case 1:
                    temp_r4 = OS_DisableIRQ();
                    func_02026b20(0x347);
                    if (((u32)func_02047e84(OVMGR_U16(mgr_bytes, 0x41834)) >= 2U) && (OVMGR_U16(mgr_bytes, 0x41950) == 0)) {
                        var_r0 = 1;
                    loop_25:
                        if (var_r0 < 4) {
                            temp_r8 = OVMGR_U16(mgr_bytes, 0x41834);
                            if (!(temp_r8 & (1 << var_r0)) ||
                                ((temp_r9 = (u8*)mgr + (var_r0 * 2), (temp_r8 == ((u16*)temp_r9)[0x4195A / 2])) &&
                                 (mgr->unk41958 == ((u16*)temp_r9)[0x41962 / 2])))
                            {
                                var_r0 += 1;
                                goto loop_25;
                            }
                        }
                        if (var_r0 == 4) {
                            func_ov040_0209d40c(func_ov002_02089bd0, mgr);
                            func_ov040_0209d420(NULL, mgr);
                            OVMGR_U16(mgr_bytes, 0x4196A) = 2U;
                            OVMGR_U16(mgr_bytes, 0x41994) = 0;
                            var_r5                        = 1;
                            OVMGR_U16(mgr_bytes, 0x41990) = 1;
                        }
                    }
                    OS_RestoreIRQ(temp_r4);
                    goto block_33;
            }
        } else {
            OVMGR_U16(mgr_bytes, 0x474C8) = var_r4_2;
            func_02026b20(0x346);
            if ((var_r4_2 != 0) && (var_r4_2 != 1)) {

            } else {
                temp_r6                       = var_r4_2 * 6;
                temp_r1_2                     = &sp8 + temp_r6;
                OVMGR_U16(mgr_bytes, 0x46084) = (u16) * (((u16*)temp_r1_2) + 1);
                OVMGR_U16(mgr_bytes, 0x46086) = (u16) * (((u16*)temp_r1_2) + 2);
                Sprite_ChangeAnimation((Sprite*)(mgr_bytes + 0x46078), OVMGR_PTR(mgr_bytes, 0x46090),
                                       (s16) * (((u16*)temp_r1_2) + 0), OVMGR_PTR(mgr_bytes, 0x46094));
            }
            goto block_33;
        }
    } else {
    block_33:
        if (OVMGR_U16(mgr_bytes, 0x41836) != 0) {
            func_ov002_0208a0f8(mgr);
        }
        if ((OVMGR_U16(mgr_bytes, 0x41990) != 0) && (OVMGR_U16(mgr_bytes, 0x4198C) == 0)) {
            func_ov002_0208a198(mgr);
        }
        if ((OVMGR_U16(mgr_bytes, 0x41834) != 1) && (OVMGR_U16(mgr_bytes, 0x41950) == 0)) {
            if (OVMGR_U16(mgr_bytes, 0x41954) != 0) {
                OVMGR_U16(mgr_bytes, 0x41954) = 0;
                var_r5                        = 1;
                OVMGR_U16(mgr_bytes, 0x41958) = (u16)(OVMGR_U16(mgr_bytes, 0x41958) + 1);
            } else {
                var_r9 = 1;
            loop_49:
                if (var_r9 < 4) {
                    temp_r6_2 = OVMGR_U16(mgr_bytes, 0x41834);
                    if (temp_r6_2 & (1 << var_r9)) {
                        temp_r7 = (u8*)mgr + (var_r9 * 2);
                        if (temp_r6_2 != ((u16*)temp_r7)[0x4195A / 2]) {
                            var_r5 = 1;
                        } else if (OVMGR_U16(mgr_bytes, 0x41958) != ((u16*)temp_r7)[0x41962 / 2]) {
                            var_r5 = 1;
                        } else {
                            goto block_48;
                        }
                    } else {
                    block_48:
                        var_r9 += 1;
                        goto loop_49;
                    }
                }
            }
        }
        if (var_r5 != 0) {
            sp4                    = OS_DisableIRQ();
            data_ov002_020935e0[0] = OVMGR_U16(mgr_bytes, 0x4196A);
            if (OVMGR_U16(mgr_bytes, 0x4196A) == 0) {
                var_r9_2                  = 0;
                data_ov002_020935e0[1]    = OVMGR_U16(mgr_bytes, 0x41834);
                data_ov002_020935e0[0x3A] = OVMGR_U16(mgr_bytes, 0x41958);
            loop_56:
                if (var_r9_2 < 4) {
                    if (OVMGR_U16(mgr_bytes, 0x41834) & (1 << var_r9_2)) {
                        func_0203b454((var_r9_2 * 0x30) + (s32)((u8*)mgr + 0x41838),
                                      (s16*)((var_r9_2 * 0x16) + (s32)&data_ov002_020935e0[2]), 0x16);
                        func_0203b454((var_r9_2 * 0x30) + (s32)((u8*)mgr + 0x41862),
                                      (s16*)((var_r9_2 * 6) + (s32)&data_ov002_020935e0[0x2E]), 6);
                    }
                    var_r9_2 += 1;
                    goto loop_56;
                }
            }
            OS_RestoreIRQ(sp4);
            OVMGR_U16(arg1, 0x41950) = 1;
            if (func_ov040_0209d48c(data_ov002_020935e0, 0x78, func_ov002_02089bc0, arg1) == 0) {
                OVMGR_U16(arg1, 0x41950) = 0;
            }
        }
        if (OVMGR_U16(arg1, 0x4196A) == 2) {
            OVMGR_S32(arg1, 0x41FD0) = (s32)func_02047e84(OVMGR_U16(arg1, 0x41834));
            func_ov031_0210cdac(sp0);
        }
        return 0;
    }
}

s32 func_ov002_0208a748(void* arg0) {
    func_ov031_0210cc84(arg0, &data_ov002_02092cf4, NULL);
    return 0;
}

s32 func_ov002_0208a764(void* arg0) {
    func_ov031_0210cc84(arg0, &data_ov002_02092ce0, NULL);
    return 0;
}

void func_ov002_0208a780(void* arg0) {
    u8* mgr = (u8*)arg0;
    s32 temp_r8;
    u16 var_r5;
    u16 var_r9;
    u32 temp_r1;

    func_0203b258(0, arg0 + 0x41FF4, 0x3000);
    func_0203b258(0, (s32)data_ov002_02093664, 0x16);
    var_r9 = 0;
    do {
        if (!(OVMGR_U16(mgr, 0x41998) & (1 << var_r9))) {
            *data_ov002_02093664 = 0xFFFF;
        } else {
            func_0203b454((var_r9 * 0x1E) + (arg0 + 0x41E38), data_ov002_02093664, 0x16);
        }
        func_ov031_0210b5fc(arg0 + 0x45FF4, (s32)data_ov002_02093664);
        func_ov031_0210ab28(arg0 + 0x45FF4, 0U, 5U);
        func_ov031_0210ab3c(arg0 + 0x45FF4, 0, 0x100U);
        func_ov031_0210ab48(arg0 + 0x45FF4, 3, 0x20U);
        func_ov031_0210bd58(arg0 + 0x45FF4, arg0 + 0x41FF4 + (var_r9 << 0xC), 0x20, 4);
        var_r9 += 1;
    } while ((u32)var_r9 < 3U);
    var_r5 = 0;
    do {
        temp_r1 = ((var_r5 << 5) + 0x48) << 8;
        temp_r8 = G2_GetBG0CharPtr() + ((s32)(temp_r1 + (temp_r1 >> 0x1F)) >> 1);
        DC_PurgeAll();
        func_0203abec(3, arg0 + 0x41FF4 + (var_r5 << 0xC), temp_r8, 0x1000);
        var_r5 += 1;
    } while ((u32)var_r5 < 3U);
}

void func_ov002_0208a8d4(void* arg0, void* arg1) {
    s32 temp_r0;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r3;
    s32 temp_r8_2;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r5;
    u16 temp_r8;
    u8* mgr   = (u8*)arg1;
    u8* entry = (u8*)arg0;
    u8* slot;
    u8* var_ip;

    var_r5 = 0;
    if (*(u32*)(entry + 0x3C) == 0) {
        return;
    }
    if (*(u8*)(entry + 0x4A) != 0x70) {
        return;
    }
    var_r4 = 5;
loop_5:
    slot = mgr + 0x419A0 + (var_r4 * 0xC4);
    if (OVMGR_U16(mgr, 0x41998) & (1 << var_r4)) {
        if ((*(u8*)(slot + 0x4) == *(u8*)(entry + 0x4)) && (*(u8*)(slot + 0x5) == *(u8*)(entry + 0x5)) &&
            (*(u8*)(slot + 0x6) == *(u8*)(entry + 0x6)) && (*(u8*)(slot + 0x7) == *(u8*)(entry + 0x7)) &&
            (*(u8*)(slot + 0x8) == *(u8*)(entry + 0x8)) && (*(u8*)(slot + 0x9) == *(u8*)(entry + 0x9)))
        {
            if (!(*(u8*)(entry + 0x4B) & 1)) {
                OVMGR_U16(mgr, 0x41998) = (u16)(OVMGR_U16(mgr, 0x41998) & ~(1 << var_r4));
                OVMGR_U16(mgr, 0x4199C) = (u16)(OVMGR_U16(mgr, 0x4199C) | (1 << var_r4));
                return;
            }
            goto block_17;
        }
        goto block_17;
    }
    var_r5 = var_r4;
block_17:
    var_r4 -= 1;
    if (var_r4 < 0) {
        if (!(*(u8*)(entry + 0x4B) & 1)) {
            return;
        }
        temp_r8 = func_02047e84();
        if ((u32)func_02047e84(OVMGR_U16(mgr, 0x41998)) >= 6U) {
            return;
        }
        OVMGR_U16(mgr, 0x4199A) = temp_r8;
        if (var_r4 >= 0) {
            var_r5 = var_r4;
        }
        temp_r8_2 = var_r5 * 0xC4;
        var_ip    = mgr + temp_r8_2 + 0x419A0;
        var_r4_2  = 0xC;
        do {
            temp_r0 = *(s32*)(entry + 0x0);
            temp_r1 = *(s32*)(entry + 0x4);
            temp_r2 = *(s32*)(entry + 0x8);
            temp_r3 = *(s32*)(entry + 0xC);
            entry += 0x10;
            *(s32*)(var_ip + 0x0) = temp_r0;
            *(s32*)(var_ip + 0x4) = temp_r1;
            *(s32*)(var_ip + 0x8) = temp_r2;
            *(s32*)(var_ip + 0xC) = temp_r3;
            var_ip += 0x10;
            var_r4_2 -= 1;
        } while (var_r4_2 != 0);
        *(u16*)(mgr + temp_r8_2 + 0x41A60) = 0xB4;
        OVMGR_U16(mgr, 0x41998)            = (u16)(OVMGR_U16(mgr, 0x41998) | (1 << var_r5));
        OVMGR_U16(mgr, 0x4199C)            = (u16)(OVMGR_U16(mgr, 0x4199C) | (1 << var_r5));
        return;
    }
    goto loop_5;
}

s32 func_ov002_0208aa4c(void* arg0, void* arg1) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) { /* irregular */
        case 0:
            func_ov040_0209cf20();
            func_ov040_0209caac(0x400548);
            break;
        case 1:
            func_ov040_0209ba04(func_ov002_0208a8d4, arg1, &data_ov002_02092160, 0);
            break;
        case 2:
            func_ov031_0210cdac((void*)temp_r0);
            break;
    }
    return 0;
}

s32 func_ov002_0208aabc(void* arg0, void* arg1) {
    s32  var_r2;
    u16* var_lr;
    u16* var_r3;
    u16  temp_r0;
    u16  temp_r1;

    var_r3 = data_ov002_020920ac;
    var_lr = subroutine_arg0;
    var_r2 = 7;
    do {
        temp_r1 = var_r3[0];
        temp_r0 = var_r3[1];
        var_r3 += 4;
        var_lr[0] = temp_r1;
        var_lr[1] = temp_r0;
        var_lr += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);
    *var_lr = *var_r3;
    func_ov002_02082f18(arg1, 0x1A, 0x19, subroutine_arg0);
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    OVMGR_U16(arg1, 0x41EEC) = 0xFFFF;
    OVMGR_U16(arg1, 0x41998) = 0;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208ab58(void* arg0, void* arg1) {
    u8*   mgr_bytes = (u8*)arg1;
    u16   table_sp18[0x19];
    u16   table_anim[0xC];
    s32   temp_r0_4;
    s32   temp_r1_3;
    s32   temp_r2;
    s32   temp_r3;
    s32   temp_r5;
    s32   temp_r5_2;
    s32   temp_r6;
    s32   temp_r7;
    s32   temp_r9;
    s32   var_r1;
    s32   var_r2;
    s32   var_r2_2;
    s32   var_r2_4;
    s32   var_r5_2;
    s32   var_r8_2;
    u16*  var_r10;
    u16*  var_r3_3;
    u16*  var_r9;
    u16   temp_lr;
    u16   temp_r0_3;
    u16   temp_r7_2;
    u16   temp_r7_3;
    void* var_r2_3;
    void* var_r5_3;
    void* var_r7_2;
    void* var_r8;
    u16   i;

    for (i = 0; i < 0x19; i++) {
        table_sp18[i] = data_ov002_0209212e[i];
    }
    for (i = 0; i < 0xC; i++) {
        table_anim[i] = data_ov002_02092094.data[i];
    }
    temp_r0_3 = func_ov002_0208597c(table_sp18);
    if (temp_r0_3 == 0xFFFE) {
        OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFFU;
        OVMGR_U16(mgr_bytes, 0x46084) = 0U;
        OVMGR_U16(mgr_bytes, 0x46086) = 0xC8U;
        return 0;
    }
    if ((temp_r0_3 != 0xFFFF) && (temp_r0_3 != 0)) {
        if ((OVMGR_U16(mgr_bytes, 0x41998) & (1 << (temp_r0_3 - 1))) == 0) {
            temp_r0_3 = 0xFFFF;
        }
    }
    if (temp_r0_3 != 0xFFFF) {
        if (OVMGR_U16(mgr_bytes, 0x474C8) == temp_r0_3) {
            switch (temp_r0_3) { /* switch 2 */
                default:         /* switch 2 */
                    goto block_36;
                case 0:          /* switch 2 */
                    func_02026b20(0x348);
                    func_ov031_0210cc84(arg0, &data_ov002_02092c9c, NULL);
                    OVMGR_U16(mgr_bytes, 0x460CC) = 0;
                    OVMGR_U16(mgr_bytes, 0x460CE) = 0xC8;
                    func_ov031_0210cd48(arg0, &data_ov002_02092f30);
                    func_ov031_0210cd48(arg0, &data_ov002_02092b00);
                    return 0;
                case 1: /* switch 2 */
                case 2: /* switch 2 */
                case 3: /* switch 2 */
                    func_02026b20(0x347);
                    if (OVMGR_U16(mgr_bytes, 0x41998) & (1 << OVMGR_U16(mgr_bytes, 0x41EEC))) {
                        temp_r9  = OS_DisableIRQ();
                        var_r8   = mgr_bytes + 0x419A0 + (OVMGR_U16(mgr_bytes, 0x41EEC) * 0xC4);
                        var_r7_2 = mgr_bytes + 0x41EF0;
                        var_r5_2 = 0xC;
                        do {
                            temp_r0_4               = *(s32*)(var_r8 + 0x0);
                            temp_r1_3               = *(s32*)(var_r8 + 0x4);
                            temp_r2                 = *(s32*)(var_r8 + 0x8);
                            temp_r3                 = *(s32*)(var_r8 + 0xC);
                            var_r8                  = (u8*)var_r8 + 0x10;
                            *(s32*)(var_r7_2 + 0x0) = temp_r0_4;
                            *(s32*)(var_r7_2 + 0x4) = temp_r1_3;
                            *(s32*)(var_r7_2 + 0x8) = temp_r2;
                            *(s32*)(var_r7_2 + 0xC) = temp_r3;
                            var_r7_2                = (u8*)var_r7_2 + 0x10;
                            var_r5_2 -= 1;
                        } while (var_r5_2 != 0);
                        OS_RestoreIRQ(temp_r9);
                        OVMGR_U32(mgr_bytes, 0x41810) = 1;
                        func_ov031_0210cdac(arg0);
                    }
                    goto block_36;
            }
        } else {
            func_02026b20(0x346);
            OVMGR_U16(mgr_bytes, 0x474C8) = temp_r0_3;
            if (temp_r0_3 != 0) {
                OVMGR_U16(mgr_bytes, 0x41EEC) = (u16)(temp_r0_3 - 1);
            }
            switch (temp_r0_3) { /* switch 3 */
                default:         /* switch 3 */
                    break;
                case 0:          /* switch 3 */
                case 1:          /* switch 3 */
                case 2:          /* switch 3 */
                case 3:          /* switch 3 */
                    temp_r6                       = temp_r0_3 * 3;
                    OVMGR_U16(mgr_bytes, 0x46084) = table_anim[temp_r6 + 1];
                    OVMGR_U16(mgr_bytes, 0x46086) = table_anim[temp_r6 + 2];
                    Sprite_ChangeAnimation((Sprite*)(mgr_bytes + 0x46078), OVMGR_PTR(mgr_bytes, 0x46090),
                                           (s16)table_anim[temp_r6], OVMGR_PTR(mgr_bytes, 0x46094));
                    break;
            }
            goto block_36;
        }
    } else {
        goto block_36;
    }
block_36:
    temp_r5  = OS_DisableIRQ();
    var_r2_3 = mgr_bytes;
    var_r1   = 0;
    do {
        if (OVMGR_U16(mgr_bytes, 0x41998) & (1 << var_r1)) {
            temp_r7                          = *(s16*)((u8*)var_r2_3 + 0x41A60) - 1;
            *(s16*)((u8*)var_r2_3 + 0x41A60) = (s16)temp_r7;
            if (temp_r7 <= 0) {
                OVMGR_U16(mgr_bytes, 0x41998) = (u16)(OVMGR_U16(mgr_bytes, 0x41998) & ~(1 << var_r1));
                OVMGR_U16(mgr_bytes, 0x4199C) = (u16)(OVMGR_U16(mgr_bytes, 0x4199C) | (1 << var_r1));
            }
        }
        var_r1 += 1;
        var_r2_3 = (u8*)var_r2_3 + 0xC4;
    } while (var_r1 < 6);
    if (OVMGR_U16(mgr_bytes, 0x4199C) != 0) {
        OVMGR_U16(mgr_bytes, 0x4199A) = func_02047e84(OVMGR_U16(mgr_bytes, 0x41998));
    }
    OS_RestoreIRQ(temp_r5);
    if (OVMGR_U16(mgr_bytes, 0x4199C) != 0) {
        OS_DisableIRQ();
        temp_r7_2                     = OVMGR_U16(mgr_bytes, 0x4199C);
        var_r2_4                      = 0;
        OVMGR_U16(mgr_bytes, 0x4199C) = 0U;
        var_r3_3                      = mgr_bytes + 0x419F0;
        var_r5_3                      = mgr_bytes;
        do {
            if (OVMGR_U16(mgr_bytes, 0x41998) & temp_r7_2 & (1 << var_r2_4)) {
                var_r10  = var_r3_3;
                var_r9   = (u16*)((u8*)var_r5_3 + 0x41E38);
                var_r8_2 = 7;
                do {
                    temp_r7_3 = var_r10[0];
                    temp_lr   = var_r10[1];
                    var_r10 += 4;
                    var_r8_2 -= 1;
                    var_r9[0] = temp_r7_3;
                    var_r9[1] = temp_lr;
                    var_r9 += 4;
                } while (var_r8_2 != 0);
                *var_r9 = *var_r10;
            }
            var_r2_4 += 1;
            var_r3_3 = (u16*)((u8*)var_r3_3 + 0xC4);
            var_r5_3 = (u8*)var_r5_3 + 0x1E;
        } while (var_r2_4 < 6);
        OS_RestoreIRQ();
        temp_r5_2 = OVMGR_U16(mgr_bytes, 0x41EEC);
        if ((temp_r5_2 != 0xFFFF) && !(OVMGR_U16(mgr_bytes, 0x41998) & (1 << temp_r5_2))) {
            OVMGR_U16(mgr_bytes, 0x41EEC) = 0xFFFF;
            OVMGR_U16(mgr_bytes, 0x474C8) = 0xFFFFU;
            OVMGR_U16(mgr_bytes, 0x46084) = 0U;
            OVMGR_U16(mgr_bytes, 0x46086) = 0xC8U;
        }
        func_ov002_0208a780(arg1);
    }
    return 0;
}

s32 func_ov002_0208af78(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) { /* irregular */
        case 0:
            func_ov031_0210cdac(arg0);
            break;
        case 1:
            func_ov031_0210cdac(arg0);
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return 0;
}

void func_ov002_0208afc4(void* arg1, void* arg2, void* arg3) {
    typedef struct {
        u16 unk0;
        u16 unk2;
        u8  filler04[0x70];
        u16 unk74;
        u16 unk76;
    } Ov002_8afc4Data;
    Ov002_8afc4Data* msg = (Ov002_8afc4Data*)arg1;
    u8*              mgr = (u8*)arg3;
    s16*             var_r5;
    s16*             var_r7;
    s32              var_r4;
    u16              temp_r0;
    void*            var_r6;
    void*            var_r8;

    if (arg1 == NULL) {
        return;
    }
    if (arg2 != (void*)0x78) {
        return;
    }
    if (arg3 == NULL) {
        return;
    }
    OVMGR_U16(mgr, 0x4196A) = msg->unk0;
    temp_r0                 = msg->unk0;
    switch (temp_r0) { /* irregular */
        case 2:
            return;
        case 0:
            var_r6 = (u8*)msg + 4;
            var_r5 = (s16*)(mgr + 0x41838);
            var_r7 = (s16*)(mgr + 0x41862);
            var_r8 = (u8*)msg + 0x5C;
            var_r4 = 0;
            do {
                if (msg->unk2 & (1 << var_r4)) {
                    func_0203b454((s32)var_r6, var_r5, 0x16);
                    func_0203b454((s32)var_r8, var_r7, 6);
                }
                var_r4 += 1;
                var_r5 += 0x30;
                var_r6 += 0x16;
                var_r7 += 0x30;
                var_r8 += 6;
            } while (var_r4 < 4);
            OVMGR_U16(mgr, 0x41834) = msg->unk2;
            OVMGR_U16(mgr, 0x41836) = 0xF;
            OVMGR_U16(mgr, 0x41958) = msg->unk74;
            return;
    }
}

s32 func_ov002_0208b0a4(void* arg0, void* arg1) {
    s32  temp_r0_2;
    s32  var_r2;
    u16* var_ip;
    u16* var_r3;
    u16  temp_r0;
    u16  temp_r1;

    var_ip = data_ov002_020920ca;
    var_r3 = subroutine_arg0;
    var_r2 = 7;
    do {
        temp_r1 = var_ip[0];
        temp_r0 = var_ip[1];
        var_ip += 4;
        var_r3[0] = temp_r1;
        var_r3[1] = temp_r0;
        var_r3 += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);
    *var_r3                  = *var_ip;
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xC8;
    func_ov002_02082f18(arg1, 0x18, 0x16, subroutine_arg0);
    temp_r0_2 = func_ov031_0210c7c4();
    func_0203b454(temp_r0_2, arg1 + 0x4181C, 0x16);
    Mem_Free(&gDebugHeap, temp_r0_2);
    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov002_0208b154(void* arg0, void* arg1) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) { /* irregular */
        case 0:
            break;
        case 1:
            func_0203b454(arg1 + 0x41EF4, arg1 + 0x41FB0, 6);
            func_0203b454(arg1 + 0x41EF4, arg1 + 0x41862, 6);
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(0);
            func_ov040_0209cabc(arg1 + 0x4181C, 0x18);
            func_ov003_0209d434((void (*)(s32, void*, void*, s32))func_ov002_0208afc4, arg1);
            func_ov040_0209d290(1, arg1 + 0x41EF0);
            break;
        case 4:
            func_ov031_0210cdac(arg0);
            break;
    }
    return 0;
}

void func_ov002_0208b230(void* arg1) {
    OVMGR_U16(arg1, 0x41950) = 0;
}

s32 func_ov002_0208b240(void* arg1) {
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    OVMGR_U16(arg1, 0x41950) = 0;
    func_ov031_0210cdac();
    return 2;
}

s32 func_ov002_0208b270(void* arg0, void* arg1) {
    u8* mgr = (u8*)arg1;
    u8  sp1E[0xC0];
    u16 spA;
    u16 sp8;
    u16 sp6;
    u16 sp4;
    s32 sp0;
    s32 temp_r1_2;
    s32 temp_r5;
    s32 var_r2;
    s32 var_r8;
    u16 temp_r0_3;
    u16 temp_r4;
    u16 temp_r5_2;
    u16 var_r4_2;
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 5; i++) {
        table_spA[i * 2]     = data_ov002_02092080.data[i * 2];
        table_spA[i * 2 + 1] = data_ov002_02092080.data[i * 2 + 1];
    }
    sp4      = data_ov002_02092068.unk0;
    sp6      = data_ov002_02092068.unk2;
    sp8      = data_ov002_02092068.unk4;
    var_r4_2 = func_ov002_0208597c(table_spA);
    if (var_r4_2 == 0xFFFE) {
        var_r4_2                = 0xFFFF;
        OVMGR_U16(mgr, 0x474C8) = 0xFFFFU;
        OVMGR_U16(mgr, 0x46084) = 0U;
        OVMGR_U16(mgr, 0x46086) = 0xC8U;
    }
    if (var_r4_2 != 0xFFFF) {
        if (OVMGR_U16(mgr, 0x474C8) == var_r4_2) {
            if (var_r4_2 == 0) {
                func_02026b20(0x348);
                func_ov031_0210cc84(arg0, &data_ov002_02092f44, NULL);
                func_ov031_0210cd48(arg0, &data_ov002_02092f30);
                func_ov031_0210cd48(arg0, &data_ov002_02092b00);
                return 0;
            }
            goto block_10;
        }
        OVMGR_U16(mgr, 0x474C8) = var_r4_2;
        if (var_r4_2 == 0) {
            func_02026b20(0x348);
            temp_r5 = var_r4_2 * 6;
            (void)temp_r5;
            OVMGR_U16(mgr, 0x46084) = sp6;
            OVMGR_U16(mgr, 0x46086) = sp8;
            Sprite_ChangeAnimation((Sprite*)(mgr + 0x46078), OVMGR_PTR(mgr, 0x46090), (s16)sp4, OVMGR_PTR(mgr, 0x46094));
        }
        goto block_10;
    }
block_10:
    if (OVMGR_U16(mgr, 0x41836) != 0) {
        sp0                     = OS_DisableIRQ();
        temp_r4                 = OVMGR_U16(mgr, 0x41834);
        temp_r5_2               = OVMGR_U16(mgr, 0x41836);
        OVMGR_U16(mgr, 0x4198A) = func_02047e84(temp_r4);
        var_r8                  = 0;
        OVMGR_U16(mgr, 0x41836) = 0U;
    loop_15:
        if (var_r8 < 4) {
            if (temp_r4 & temp_r5_2 & (1 << var_r8)) {
                temp_r1_2 = var_r8 * 0x30;
                func_0203b454((s32)(arg1 + 0x41838 + temp_r1_2), sp1E + temp_r1_2, 0x16);
            }
            var_r8 += 1;
            goto loop_15;
        }
        OS_RestoreIRQ(sp0);
        func_ov002_02082ab4(arg1);
    }
    if (OVMGR_U16(mgr, 0x41950) == 0) {
        OS_DisableIRQ();
        data_ov002_020935c0[0] = OVMGR_U16(mgr, 0x41834);
        data_ov002_020935c0[1] = OVMGR_U16(mgr, 0x41958);
        OS_RestoreIRQ();
        func_0203a96c((u8*)&data_ov002_020935c0[2]);
        OVMGR_U16(mgr, 0x41950) = 1;
        if (func_ov040_0209d48c(data_ov002_020935c0, 0xA, func_ov002_0208b230, arg1) == 0) {
            OVMGR_U16(mgr, 0x41950) = 0;
        }
    }
    temp_r0_3 = OVMGR_U16(mgr, 0x4196A);
    if (temp_r0_3 != 2) {
        if (temp_r0_3 == 3) {
            func_ov031_0210cc84(arg0, &data_ov002_02092c9c, NULL);
            func_ov031_0210cd48(arg0, &data_ov002_02092ec8);
            func_ov031_0210cd48(arg0, &data_ov002_02092f30);
        }
    } else {
        func_02026b20(0x347);
        OVMGR_S32(mgr, 0x41FD0) = (s32)func_02047e84(OVMGR_U16(mgr, 0x41834));
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_0208b570(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        default:
            break;
        case 0:
            func_ov031_0210cdac(arg0);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return 0;
}

s32 func_ov002_0208b5c4(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        case 0:
        case 3:
            break;
        default:
            break;
        case 1:
            func_ov031_0210cdac(arg0);
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return 0;
}

s32 func_ov002_0208b610(void* arg0, void* arg1) {
    if (func_ov040_0209cb68() == 1) {
        func_ov031_0210cdac(arg0);
        return 0;
    }
    OS_DisableIRQ();
    *data_ov002_020935e0 = 3;
    OS_RestoreIRQ();
    func_ov040_0209d48c(data_ov002_020935e0, 0x78, func_ov002_02089bc0, arg1);
    return 0;
}

s32 func_ov002_0208b670(void* arg0, void* arg1) {
    if (func_ov040_0209cb68() == 1) {
        func_ov031_0210cdac(arg0);
        return 0;
    }
    OS_DisableIRQ();
    *data_ov002_020935e0 = 2;
    OS_RestoreIRQ();
    func_ov040_0209d48c(data_ov002_020935e0, 0x78, func_ov002_02089bc0, arg1);
    return 0;
}

s32 func_ov002_0208b6d0(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        default:
        case 2:
            func_ov040_0209d588();
            break;
        case 0:
            func_ov002_02089b54();
            func_ov031_0210cdac(arg0);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 4:
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
            break;
    }
    return 0;
}

s32 func_ov002_0208b734(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        case 0:
        case 3:
            break;
        default:
        case 2:
            func_ov040_0209d588();
            break;
        case 1:
            func_ov002_02089b54();
            func_ov031_0210cdac(arg0);
            break;
        case 4:
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
            break;
    }
    return 0;
}

u32 func_ov002_0208b790(void) {
    u32 temp_r0;

    temp_r0 = (u32)(SystemStatusFlags << 0x1F) >> 0x1F;
    if (temp_r0 == 0) {
        return temp_r0;
    }
    Display_Commit();
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
    return GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
}

void func_ov002_0208b860(void) {
    u8* temp_r2;
    u8* temp_r3;
    u8* temp_r5;
    u8* temp_r5_2;
    u8* temp_r5_3;
    u8* temp_r5_4;
    u8* temp_r5_5;
    u8* temp_r6;

    Interrupts_ForceVBlank();
    g_DisplaySettings.unk30 = 0x10;
    g_DisplaySettings.unk60 = 0x10;
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    DMA_Init(0x100);
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    func_0203b2d0(0, (void*)0x06800000, 0xA4000);
    func_0203b2d0(0, (void*)0x06000000, 0x80000);
    Interrupts_ForceVBlank();
    func_0203b2d0(0, (void*)0x06200000, 0x20000);
    func_0203b2d0(0, (void*)0x06400000, 0x40000);
    func_0203b2d0(0, (void*)0x06600000, 0x20000);
    Interrupts_ForceVBlank();
    g_DisplaySettings.unk10 = 0x200010;
    g_DisplaySettings.unk14 = 0x40;
    g_DisplaySettings.unk40 = 0x200010;
    g_DisplaySettings.unk44 = 0x40;
    g_DisplaySettings.unk0  = 0;
    *(u16*)0x04000304 &= ~0x8000;
    g_DisplaySettings.unk4 = 1;
    g_DisplaySettings.unk8 = 0;
    g_DisplaySettings.unkC = 0;
    GX_SetGraphicsMode(1, 0, 0);
    temp_r3                 = (u8*)&g_DisplaySettings + 0x64;
    *(u16*)(temp_r3 + 0x4)  = 1;
    g_DisplaySettings.unk64 = 0;
    *(u16*)(temp_r3 + 0x1C) = 0;
    *(u16*)(temp_r3 + 0x20) = 0;
    *(u16*)(temp_r3 + 0x28) = 6;
    *(u16*)(temp_r3 + 0x2C) = 0;
    if (g_DisplaySettings.unkC == 0) {
        *(u16*)0x04000008 = (*(u16*)0x04000008 & 0x43) | 0x4018;
    }
    temp_r5                   = (u8*)&g_DisplaySettings + 0xA0;
    g_DisplaySettings.unkA0   = 0;
    *(u16*)(temp_r5 + 0x4)    = 1;
    *(u16*)(temp_r5 + 0x1C)   = 0;
    *(u16*)(temp_r5 + 0x20)   = 2;
    *(u16*)(temp_r5 + 0x2C)   = 0;
    *(u16*)(temp_r5 + 0x28)   = 5;
    temp_r5_2                 = (u8*)&g_DisplaySettings + 0xDC;
    *(u16*)0x0400000A         = (u16)(((*(u16*)0x0400000A) & 0x43) | 0x4214);
    g_DisplaySettings.unkDC   = 0;
    *(u16*)(temp_r5_2 + 0x4)  = 1;
    *(u16*)(temp_r5_2 + 0x20) = 4;
    *(u16*)(temp_r5_2 + 0x1C) = 0;
    *(u16*)(temp_r5_2 + 0x28) = 3;
    *(u16*)(temp_r5_2 + 0x2C) = 1;
    temp_r6                   = (u8*)&g_DisplaySettings + 0x118;
    *(u16*)(0x0400000A + 2)   = (u16)(((*(u16*)(0x0400000A + 2)) & 0x43) | 0x440C);
    g_DisplaySettings.unk118  = 0;
    *(u16*)(temp_r6 + 0x4)    = 1;
    *(u16*)(temp_r6 + 0x20)   = 6;
    *(u16*)(temp_r6 + 0x1C)   = 0;
    *(u16*)(temp_r6 + 0x28)   = 1;
    *(u16*)(temp_r6 + 0x2C)   = 1;
    *(u16*)(0x0400000A + 4)   = (u16)(((*(u16*)(0x0400000A + 4)) & 0x43) | 0x4604);
    g_DisplaySettings.unk98   = 0;
    g_DisplaySettings.unkD4   = 1;
    g_DisplaySettings.unk110  = 2;
    g_DisplaySettings.unk14C  = 3;
    g_DisplaySettings.unk9C   = 0;
    g_DisplaySettings.unkD8   = 0;
    g_DisplaySettings.unk114  = 0;
    g_DisplaySettings.unk150  = 0;
    g_DisplaySettings.unk1C   = 0x1F;
    g_DisplaySettings.unk38   = 0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    temp_r2                   = (u8*)&g_DisplaySettings + 0x284;
    g_DisplaySettings.unk284  = 0;
    *(u16*)(temp_r2 + 0x4)    = 1;
    *(u16*)(temp_r2 + 0x1C)   = 0;
    *(u16*)(temp_r2 + 0x20)   = 0;
    *(u16*)(temp_r2 + 0x28)   = 7;
    *(u16*)(temp_r2 + 0x2C)   = 0;
    temp_r5_3                 = (u8*)&g_DisplaySettings + 0x2C0;
    *(u16*)0x04001008         = (u16)(((*(u16*)0x04001008) & 0x43) | 0x401C);
    g_DisplaySettings.unk2C0  = 0;
    *(u16*)(temp_r5_3 + 0x4)  = 1;
    *(u16*)(temp_r5_3 + 0x1C) = 0;
    *(u16*)(temp_r5_3 + 0x20) = 2;
    *(u16*)(temp_r5_3 + 0x28) = 5;
    *(u16*)(temp_r5_3 + 0x2C) = 0;
    temp_r5_4                 = (u8*)&g_DisplaySettings + 0x2FC;
    *(u16*)(0x04001008 + 2)   = (u16)(((*(u16*)(0x04001008 + 2)) & 0x43) | 0x4214);
    g_DisplaySettings.unk2FC  = 0;
    *(u16*)(temp_r5_4 + 0x4)  = 1;
    *(u16*)(temp_r5_4 + 0x1C) = 0;
    *(u16*)(temp_r5_4 + 0x20) = 4;
    *(u16*)(temp_r5_4 + 0x28) = 3;
    *(u16*)(temp_r5_4 + 0x2C) = 1;
    temp_r5_5                 = (u8*)&g_DisplaySettings + 0x338;
    *(u16*)(0x04001008 + 4)   = (u16)(((*(u16*)(0x04001008 + 4)) & 0x43) | 0x440C);
    g_DisplaySettings.unk338  = 0;
    *(u16*)(temp_r5_5 + 0x4)  = 1;
    *(u16*)(temp_r5_5 + 0x1C) = 0;
    *(u16*)(temp_r5_5 + 0x20) = 6;
    *(u16*)(temp_r5_5 + 0x28) = 1;
    *(u16*)(temp_r5_5 + 0x2C) = 1;
    *(u16*)(0x04001008 + 6)   = (u16)(((*(u16*)(0x04001008 + 6)) & 0x43) | 0x4604);
    g_DisplaySettings.unk2B8  = 0;
    g_DisplaySettings.unk2F4  = 1;
    g_DisplaySettings.unk330  = 2;
    g_DisplaySettings.unk36C  = 3;
    g_DisplaySettings.unk2BC  = 0;
    g_DisplaySettings.unk2F8  = 0;
    g_DisplaySettings.unk334  = 0;
    g_DisplaySettings.unk370  = 0;
    g_DisplaySettings.unk4C   = 0;
    g_DisplaySettings.unk4C   = 0x1F;
    g_DisplaySettings.unk26C  = 1;
    g_DisplaySettings.unk270  = 1;
    g_DisplaySettings.unk274  = 0x3E;
    g_DisplaySettings.unk278  = 6;
    g_DisplaySettings.unk27C  = 0xA;
    g_DisplaySettings.unk48C  = 1;
    g_DisplaySettings.unk490  = 1;
    g_DisplaySettings.unk494  = 0x3E;
    g_DisplaySettings.unk498  = 6;
    g_DisplaySettings.unk49C  = 0xA;
    g_DisplaySettings.unk20   = (s32)(g_DisplaySettings.unk20 | 4);
    g_DisplaySettings.unk22C  = 0x1F;
    g_DisplaySettings.unk230  = 0;
    g_DisplaySettings.unk50   = (s32)(g_DisplaySettings.unk50 | 4);
    g_DisplaySettings.unk44C  = 0x1F;
    g_DisplaySettings.unk450  = 0;
    Interrupts_RegisterVBlankCallback(func_ov002_0208b790);
    g_DisplaySettings.unk20  = (s32)(g_DisplaySettings.unk20 | 4);
    g_DisplaySettings.unk24C = 0xC;
    g_DisplaySettings.unk250 = 1;
    g_DisplaySettings.unk22C = 0x1F;
    g_DisplaySettings.unk230 = 1;
    g_DisplaySettings.unk26C = 1;
    g_DisplaySettings.unk270 = 4;
    g_DisplaySettings.unk274 = 8;
    g_DisplaySettings.unk278 = 0xC;
    g_DisplaySettings.unk27C = 4;
    g_DisplaySettings.unk48C = 1;
    g_DisplaySettings.unk46C = 0xC;
    g_DisplaySettings.unk470 = 1;
    g_DisplaySettings.unk490 = 4;
    g_DisplaySettings.unk494 = 8;
    g_DisplaySettings.unk44C = 0x1F;
    g_DisplaySettings.unk450 = 0;
    g_DisplaySettings.unk498 = 0xC;
    g_DisplaySettings.unk49C = 4;
    Display_Commit();
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
}

void func_ov002_0208bd40(void) {
    void* temp_r2;
    void* temp_r3;
    void* temp_r3_2;
    void* temp_r5;
    void* temp_r5_2;
    void* temp_r5_3;
    void* temp_r5_4;
    void* temp_r5_5;

    Interrupts_ForceVBlank();
    Interrupts_Init();
    func_0200434c();
    GX_Init();
    DMA_Init(0x100);
    Display_Init();
    g_DisplaySettings.unk224 = 0x1F;
    g_DisplaySettings.unk228 = 1;
    g_DisplaySettings.unk22C = 0x1F;
    g_DisplaySettings.unk230 = 1;
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    func_0203b2d0(0, (void*)0x06800000, 0xA4000);
    func_0203b2d0(0, (void*)0x06000000, 0x80000);
    Interrupts_ForceVBlank();
    func_0203b2d0(0, (void*)0x06200000, 0x20000);
    func_0203b2d0(0, (void*)0x06400000, 0x40000);
    func_0203b2d0(0, (void*)0x06600000, 0x20000);
    Interrupts_ForceVBlank();
    g_DisplaySettings.unk10 = 0x200010;
    g_DisplaySettings.unk14 = 0x40;
    g_DisplaySettings.unk40 = 0x200010;
    g_DisplaySettings.unk44 = 0x40;
    g_DisplaySettings.unk0  = 0;
    *(u16*)0x04000304 &= ~0x8000;
    g_DisplaySettings.unk4 = 1;
    g_DisplaySettings.unk8 = 0;
    g_DisplaySettings.unkC = 0;
    GX_SetGraphicsMode(1, 0, 0);
    g_DisplaySettings.unk98  = 0;
    g_DisplaySettings.unkD4  = 1;
    g_DisplaySettings.unk110 = 2;
    g_DisplaySettings.unk14C = 3;
    g_DisplaySettings.unk9C  = 0;
    g_DisplaySettings.unkD8  = 0;
    g_DisplaySettings.unk114 = 0;
    g_DisplaySettings.unk150 = 0;
    g_DisplaySettings.unk1C  = 0x1F;
    g_DisplaySettings.unk38  = 0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    temp_r3                   = (u8*)&g_DisplaySettings + 0x284;
    g_DisplaySettings.unk284  = 0;
    *(u16*)(temp_r3 + 0x4)    = 1;
    *(u16*)(temp_r3 + 0x1C)   = 0;
    *(u16*)(temp_r3 + 0x20)   = 0;
    *(u16*)(temp_r3 + 0x28)   = 7;
    *(u16*)(temp_r3 + 0x2C)   = 0;
    temp_r3_2                 = (u8*)&g_DisplaySettings + 0x2C0;
    *(u16*)0x04001008         = (u16)(((*(u16*)0x04001008) & 0x43) | 0x401C);
    g_DisplaySettings.unk2C0  = 0;
    *(u16*)(temp_r3_2 + 0x4)  = 1;
    *(u16*)(temp_r3_2 + 0x1C) = 0;
    *(u16*)(temp_r3_2 + 0x20) = 2;
    *(u16*)(temp_r3_2 + 0x28) = 5;
    *(u16*)(temp_r3_2 + 0x2C) = 0;
    temp_r5                   = (u8*)&g_DisplaySettings + 0x2FC;
    *(u16*)(0x04001008 + 2)   = (u16)(((*(u16*)(0x04001008 + 2)) & 0x43) | 0x4214);
    g_DisplaySettings.unk2FC  = 0;
    *(u16*)(temp_r5 + 0x4)    = 1;
    *(u16*)(temp_r5 + 0x1C)   = 0;
    *(u16*)(temp_r5 + 0x20)   = 4;
    *(u16*)(temp_r5 + 0x28)   = 3;
    *(u16*)(temp_r5 + 0x2C)   = 1;
    temp_r5_2                 = (u8*)&g_DisplaySettings + 0x338;
    *(u16*)(0x04001008 + 4)   = (u16)(((*(u16*)(0x04001008 + 4)) & 0x43) | 0x440C);
    g_DisplaySettings.unk338  = 0;
    *(u16*)(temp_r5_2 + 0x4)  = 1;
    *(u16*)(temp_r5_2 + 0x1C) = 0;
    *(u16*)(temp_r5_2 + 0x20) = 6;
    *(u16*)(temp_r5_2 + 0x28) = 1;
    *(u16*)(temp_r5_2 + 0x2C) = 1;
    *(u16*)(0x04001008 + 6)   = (u16)(((*(u16*)(0x04001008 + 6)) & 0x43) | 0x4604);
    g_DisplaySettings.unk2B8  = 0;
    g_DisplaySettings.unk2F4  = 1;
    g_DisplaySettings.unk330  = 2;
    g_DisplaySettings.unk36C  = 3;
    g_DisplaySettings.unk2BC  = 0;
    g_DisplaySettings.unk2F8  = 0;
    g_DisplaySettings.unk334  = 0;
    g_DisplaySettings.unk370  = 0;
    g_DisplaySettings.unk4C   = 0x1F;
    g_DisplaySettings.unk26C  = 1;
    g_DisplaySettings.unk270  = 1;
    g_DisplaySettings.unk274  = 0x3E;
    g_DisplaySettings.unk278  = 6;
    g_DisplaySettings.unk27C  = 0xA;
    g_DisplaySettings.unk48C  = 1;
    g_DisplaySettings.unk490  = 1;
    g_DisplaySettings.unk494  = 0x3E;
    g_DisplaySettings.unk498  = 6;
    g_DisplaySettings.unk49C  = 0xA;
    g_DisplaySettings.unk20   = (s32)(g_DisplaySettings.unk20 | 4);
    g_DisplaySettings.unk22C  = 0x1F;
    g_DisplaySettings.unk230  = 0;
    g_DisplaySettings.unk50   = (s32)(g_DisplaySettings.unk50 | 4);
    g_DisplaySettings.unk44C  = 0x1F;
    g_DisplaySettings.unk450  = 0;
    Interrupts_RegisterVBlankCallback(func_ov002_0208b790);
    g_DisplaySettings.unk20  = (s32)(g_DisplaySettings.unk20 | 4);
    g_DisplaySettings.unk24C = 0xC;
    g_DisplaySettings.unk250 = 1;
    g_DisplaySettings.unk22C = 0x1F;
    g_DisplaySettings.unk230 = 1;
    g_DisplaySettings.unk26C = 1;
    g_DisplaySettings.unk270 = 4;
    g_DisplaySettings.unk274 = 8;
    g_DisplaySettings.unk278 = 0xC;
    g_DisplaySettings.unk27C = 4;
    g_DisplaySettings.unk48C = 1;
    g_DisplaySettings.unk46C = 0xC;
    g_DisplaySettings.unk470 = 1;
    g_DisplaySettings.unk490 = 4;
    g_DisplaySettings.unk494 = 8;
    g_DisplaySettings.unk44C = 0x1F;
    g_DisplaySettings.unk450 = 0;
    g_DisplaySettings.unk498 = 0xC;
    temp_r2                  = (u8*)&g_DisplaySettings + 0x64;
    g_DisplaySettings.unk49C = 4;
    g_DisplaySettings.unk64  = 0;
    *(u16*)(temp_r2 + 0x4)   = 1;
    *(u16*)(temp_r2 + 0x1C)  = 0;
    *(u16*)(temp_r2 + 0x20)  = 0;
    *(u16*)(temp_r2 + 0x28)  = 5;
    *(u16*)(temp_r2 + 0x2C)  = 0;
    if (g_DisplaySettings.unkC == 0) {
        *(u16*)0x04000008 = (*(u16*)0x04000008 & 0x43) | 0x4014;
    }
    temp_r5_3                 = (u8*)&g_DisplaySettings + 0xA0;
    g_DisplaySettings.unkA0   = 0;
    *(u16*)(temp_r5_3 + 0x4)  = 1;
    *(u16*)(temp_r5_3 + 0x1C) = 0;
    *(u16*)(temp_r5_3 + 0x20) = 2;
    *(u16*)(temp_r5_3 + 0x28) = 1;
    *(u16*)(temp_r5_3 + 0x2C) = 0;
    temp_r5_4                 = (u8*)&g_DisplaySettings + 0xDC;
    *(u16*)0x0400000A         = (u16)(((*(u16*)0x0400000A) & 0x43) | 0x4204);
    g_DisplaySettings.unkDC   = 0;
    *(u16*)(temp_r5_4 + 0x4)  = 1;
    *(u16*)(temp_r5_4 + 0x1C) = 0;
    *(u16*)(temp_r5_4 + 0x20) = 4;
    *(u16*)(temp_r5_4 + 0x28) = 1;
    *(u16*)(temp_r5_4 + 0x2C) = 1;
    temp_r5_5                 = (u8*)&g_DisplaySettings + 0x118;
    *(u16*)(0x0400000A + 2)   = (u16)(((*(u16*)(0x0400000A + 2)) & 0x43) | 0x4404);
    g_DisplaySettings.unk118  = 0;
    *(u16*)(temp_r5_5 + 0x4)  = 1;
    *(u16*)(temp_r5_5 + 0x1C) = 0;
    *(u16*)(temp_r5_5 + 0x20) = 6;
    *(u16*)(temp_r5_5 + 0x28) = 3;
    *(u16*)(temp_r5_5 + 0x2C) = 1;
    *(u16*)(0x0400000A + 4)   = (u16)(((*(u16*)(0x0400000A + 4)) & 0x43) | 0x460C);
    Display_Commit();
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
}

void func_ov002_0208c228(void) {
    s32 temp_r0_2;
    u8* temp_r0;
    u8* temp_r0_3;
    u8* temp_r1;
    u8* temp_r5;
    u8* temp_r5_2;
    u8* temp_r5_3;
    u8* temp_r6;
    u8* temp_r6_2;

    Interrupts_ForceVBlank();
    Display_Init();
    Interrupts_Init();
    func_0200434c();
    DMA_Init(0x100);
    Interrupts_RegisterHBlankCallback(0, 1);
    Interrupts_RegisterVBlankCallback(func_ov002_0208b790, 1);
    g_DisplaySettings.unk30 = -0x10;
    g_DisplaySettings.unk60 = -0x10;
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForSubBg(4);
    GX_SetBankForSubObj(8);
    func_0203b2d0(0, (void*)0x06800000, 0xA4000);
    func_0203b2d0(0, (void*)0x06000000, 0x80000);
    Interrupts_ForceVBlank();
    func_0203b2d0(0, (void*)0x06200000, 0x20000);
    func_0203b2d0(0, (void*)0x06400000, 0x40000);
    func_0203b2d0(0, (void*)0x06600000, 0x20000);
    Interrupts_ForceVBlank();
    g_DisplaySettings.unk10 = 0x200010;
    g_DisplaySettings.unk14 = 0x40;
    g_DisplaySettings.unk40 = 0x200010;
    g_DisplaySettings.unk44 = 0x40;
    g_DisplaySettings.unk0  = 0;
    *(u16*)0x04000304 &= ~0x8000;
    g_DisplaySettings.unk4 = 1;
    g_DisplaySettings.unk8 = 0;
    g_DisplaySettings.unkC = 0;
    GX_SetGraphicsMode(1, 0, 0);
    g_DisplaySettings.unk98  = 0;
    g_DisplaySettings.unkD4  = 1;
    g_DisplaySettings.unk110 = 2;
    g_DisplaySettings.unk14C = 3;
    g_DisplaySettings.unk9C  = 0;
    g_DisplaySettings.unkD8  = 0;
    g_DisplaySettings.unk114 = 0;
    g_DisplaySettings.unk150 = 0;
    g_DisplaySettings.unk1C  = 0x1F;
    g_DisplaySettings.unk38  = 0;
    GXs_SetGraphicsMode(GX_BGMODE_0);
    temp_r1                   = (u8*)&g_DisplaySettings + 0x284;
    g_DisplaySettings.unk284  = 0;
    *(u16*)(temp_r1 + 0x4)    = 1;
    *(u16*)(temp_r1 + 0x1C)   = 0;
    *(u16*)(temp_r1 + 0x20)   = 0;
    *(u16*)(temp_r1 + 0x28)   = 7;
    *(u16*)(temp_r1 + 0x2C)   = 0;
    temp_r0                   = (u8*)&g_DisplaySettings + 0x2C0;
    *(u16*)0x04001008         = (u16)(((*(u16*)0x04001008) & 0x43) | 0x401C);
    g_DisplaySettings.unk2C0  = 0;
    *(u16*)(temp_r0 + 0x4)    = 1;
    *(u16*)(temp_r0 + 0x1C)   = 0;
    *(u16*)(temp_r0 + 0x20)   = 2;
    *(u16*)(temp_r0 + 0x28)   = 5;
    *(u16*)(temp_r0 + 0x2C)   = 0;
    temp_r6                   = (u8*)&g_DisplaySettings + 0x2FC;
    *(u16*)(0x04001008 + 2)   = (u16)(((*(u16*)(0x04001008 + 2)) & 0x43) | 0x4214);
    g_DisplaySettings.unk2FC  = 0;
    *(u16*)(temp_r6 + 0x4)    = 1;
    *(u16*)(temp_r6 + 0x1C)   = 0;
    *(u16*)(temp_r6 + 0x20)   = 4;
    *(u16*)(temp_r6 + 0x28)   = 3;
    *(u16*)(temp_r6 + 0x2C)   = 1;
    *(u16*)(0x04001008 + 4)   = (u16)(((*(u16*)(0x04001008 + 4)) & 0x43) | 0x440C);
    temp_r6_2                 = (u8*)&g_DisplaySettings + 0x338;
    g_DisplaySettings.unk338  = 0;
    *(u16*)(temp_r6_2 + 0x4)  = 1;
    *(u16*)(temp_r6_2 + 0x1C) = 0;
    *(u16*)(temp_r6_2 + 0x20) = 6;
    *(u16*)(temp_r6_2 + 0x28) = 1;
    *(u16*)(temp_r6_2 + 0x2C) = 1;
    *(u16*)(0x04001008 + 6)   = (u16)(((*(u16*)(0x04001008 + 6)) & 0x43) | 0x4604);
    g_DisplaySettings.unk2B8  = 0;
    g_DisplaySettings.unk2F4  = 1;
    g_DisplaySettings.unk330  = 2;
    g_DisplaySettings.unk36C  = 3;
    g_DisplaySettings.unk2BC  = 0;
    g_DisplaySettings.unk2F8  = 0;
    g_DisplaySettings.unk334  = 0;
    g_DisplaySettings.unk370  = 0;
    g_DisplaySettings.unk4C   = 0x1F;
    g_DisplaySettings.unk26C  = 1;
    g_DisplaySettings.unk270  = 1;
    g_DisplaySettings.unk274  = 0x3E;
    g_DisplaySettings.unk278  = 6;
    g_DisplaySettings.unk27C  = 0xA;
    g_DisplaySettings.unk48C  = 1;
    g_DisplaySettings.unk490  = 1;
    g_DisplaySettings.unk494  = 0x3E;
    g_DisplaySettings.unk498  = 6;
    g_DisplaySettings.unk49C  = 0xA;
    temp_r0_2                 = g_DisplaySettings.unk20 | 4;
    g_DisplaySettings.unk20   = temp_r0_2;
    g_DisplaySettings.unk22C  = 0x1F;
    g_DisplaySettings.unk230  = 0;
    g_DisplaySettings.unk50   = (s32)(g_DisplaySettings.unk50 | 4);
    g_DisplaySettings.unk44C  = 0x1F;
    g_DisplaySettings.unk450  = 0;
    g_DisplaySettings.unk20   = (s32)(temp_r0_2 | 4);
    g_DisplaySettings.unk24C  = 0xC;
    g_DisplaySettings.unk250  = 1;
    g_DisplaySettings.unk22C  = 0x1F;
    g_DisplaySettings.unk230  = 1;
    g_DisplaySettings.unk26C  = 1;
    g_DisplaySettings.unk270  = 4;
    g_DisplaySettings.unk274  = 8;
    g_DisplaySettings.unk278  = 0xC;
    g_DisplaySettings.unk27C  = 4;
    g_DisplaySettings.unk48C  = 1;
    g_DisplaySettings.unk46C  = 0xC;
    g_DisplaySettings.unk470  = 1;
    g_DisplaySettings.unk490  = 4;
    g_DisplaySettings.unk494  = 8;
    g_DisplaySettings.unk44C  = 0x1F;
    g_DisplaySettings.unk450  = 0;
    g_DisplaySettings.unk498  = 0xC;
    temp_r0_3                 = (u8*)&g_DisplaySettings + 0x64;
    g_DisplaySettings.unk49C  = 4;
    g_DisplaySettings.unk64   = 0;
    *(u16*)(temp_r0_3 + 0x4)  = 1;
    *(u16*)(temp_r0_3 + 0x1C) = 0;
    *(u16*)(temp_r0_3 + 0x20) = 0;
    *(u16*)(temp_r0_3 + 0x28) = 5;
    *(u16*)(temp_r0_3 + 0x2C) = 0;
    if (g_DisplaySettings.unkC == 0) {
        *(u16*)0x04000008 = (*(u16*)0x04000008 & 0x43) | 0x4014;
    }
    temp_r5                   = (u8*)&g_DisplaySettings + 0xA0;
    g_DisplaySettings.unkA0   = 0;
    *(u16*)(temp_r5 + 0x4)    = 1;
    *(u16*)(temp_r5 + 0x1C)   = 0;
    *(u16*)(temp_r5 + 0x20)   = 2;
    *(u16*)(temp_r5 + 0x28)   = 1;
    *(u16*)(temp_r5 + 0x2C)   = 0;
    temp_r5_2                 = (u8*)&g_DisplaySettings + 0xDC;
    *(u16*)0x0400000A         = (u16)(((*(u16*)0x0400000A) & 0x43) | 0x4204);
    g_DisplaySettings.unkDC   = 0;
    *(u16*)(temp_r5_2 + 0x4)  = 1;
    *(u16*)(temp_r5_2 + 0x1C) = 0;
    *(u16*)(temp_r5_2 + 0x20) = 4;
    *(u16*)(temp_r5_2 + 0x28) = 1;
    *(u16*)(temp_r5_2 + 0x2C) = 1;
    temp_r5_3                 = (u8*)&g_DisplaySettings + 0x118;
    *(u16*)(0x0400000A + 2)   = (u16)(((*(u16*)(0x0400000A + 2)) & 0x43) | 0x4404);
    g_DisplaySettings.unk118  = 0;
    *(u16*)(temp_r5_3 + 0x4)  = 1;
    *(u16*)(temp_r5_3 + 0x1C) = 0;
    *(u16*)(temp_r5_3 + 0x20) = 6;
    *(u16*)(temp_r5_3 + 0x28) = 3;
    *(u16*)(temp_r5_3 + 0x2C) = 1;
    *(u16*)(0x0400000A + 4)   = (u16)(((*(u16*)(0x0400000A + 4)) & 0x43) | 0x460C);
    Display_Commit();
    func_0200270c(0, 0);
    func_0200283c(&data_020676ec, 0, 0);
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    func_0200270c(0, 1);
    func_0200283c(&data_02068778, 0, 0);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
}

SpriteFrameInfo* func_ov002_0208c6f8(Sprite* arg0, s32 arg1, s32 arg2) {
    (void)arg1;
    switch (arg2) { /* irregular */
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            return Ov002_GetSpriteFrameInfo(arg0);
        default:
            return NULL;
    }
}

void func_ov002_0208c794(void* arg1) {
    u8*             mgr       = (u8*)arg1;
    SpriteAnimation anim      = data_ov002_02092170;
    u16*            flags     = (u16*)&anim;
    u16             data_type = OVMGR_U8(mgr, 0x1158C);

    *flags      = (u16)((*flags & (u16)~0x3C) | (u16)(((u32)data_type << 0x1C) >> 0x1A));
    anim.unk_2A = 1;
    *flags      = (u16)((*flags & (u16)~3) | 1);
    anim.unk_04 = 0x80;
    anim.unk_06 = 0x60;
    anim.unk_1C = 0x11;
    anim.unk_20 = 0x14;
    anim.unk_22 = 2;
    anim.unk_26 = 0x12;
    anim.unk_28 = 0x13;
    if (_Sprite_Load((Sprite*)(mgr + 0x460C0), &anim) != 0) {
        return;
    }
    OS_WaitForever();
}

void func_ov002_0208c864(s32 arg1) {
    Sprite_Destroy((Sprite*)((u8*)arg1 + 0x460C0));
}

void func_ov002_0208c878(s32 arg1) {
    Sprite_UpdateAndCheck((Sprite*)((u8*)arg1 + 0x460C0));
}

void func_ov002_0208c88c(void* arg1) {
    if (OVMGR_S32(arg1, 0x462E8) == 0) {
        return;
    }
    Sprite_Render((Sprite*)((u8*)arg1 + 0x460C0));
}

s32 func_ov002_0208c8b0(void* arg1) {
    s32 temp_r0;

    temp_r0 = func_ov040_0209cb5c(data_ov002_02092168.unk0, data_ov002_02092168.unk2, data_ov002_02092168.unk4,
                                  data_ov002_02092168.unk6);
    if ((temp_r0 < 0) || (temp_r0 > 3)) {
        OS_WaitForever();
    }
    Sprite_ChangeAnimation((Sprite*)((u8*)arg1 + 0x460C0), OVMGR_PTR(arg1, 0x460D8), (s16)subroutine_arg0[temp_r0],
                           OVMGR_PTR(arg1, 0x460DC));
    return 0;
}

SpriteFrameInfo* func_ov002_0208c92c(Sprite* arg0, s32 arg1, s32 arg2) {
    (void)arg1;
    switch (arg2) { /* irregular */
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            return Ov002_GetSpriteFrameInfo(arg0);
        default:
            return NULL;
    }
}

void func_ov002_0208c9c8(void* arg1, s16* arg2) {
    SpriteAnimation anim  = data_ov002_0209219c;
    u16*            flags = (u16*)&anim;

    anim.unk_2A = 1;
    *flags      = (u16)(*flags & (u16)~3);
    anim.unk_04 = 0x80;
    anim.unk_06 = *arg2;
    anim.unk_1C = 0x11;
    anim.unk_20 = 0x14;
    anim.unk_22 = 2;
    anim.unk_26 = 0x12;
    anim.unk_28 = 0x13;
    if (_Sprite_Load((Sprite*)arg1, &anim) != 0) {
        return;
    }
    OS_WaitForever();
}

void func_ov002_0208ca70(void* arg1) {
    Sprite_Destroy(arg1);
}

void func_ov002_0208ca80(void* arg1) {
    Sprite_UpdateAndCheck(arg1);
}

void func_ov002_0208ca90(void* arg1) {
    Sprite_Render(arg1);
}

s32 func_ov002_0208caa0(void) {
    return 0;
}

void func_ov002_0208caa8(void* arg0) {
    u16 temp_r0_2;
    u16 var_r8;
    u32 temp_r0;

    var_r8 = 0;
    if ((s32)data_02074d10.unk40B > 0) {
        do {
            func_ov031_0210cff0((u8*)arg0 + 0x41804, (u8*)arg0 + subroutine_arg0[var_r8]);
            temp_r0_2 = var_r8 + 1;
            temp_r0   = temp_r0_2 << 0x10;
            var_r8    = temp_r0_2;
        } while ((s32)data_02074d10.unk40B > (s32)(temp_r0 >> 0x10));
    }
    func_ov031_0210cff0(arg0 + 0x41804, arg0 + 0x47C88);
}

void func_ov002_0208cb50(void* arg0) {
    s32 temp_gt;
    u16 temp_r11;
    u16 temp_r11_2;
    u16 temp_r7;
    u16 var_lr;
    u16 var_r4;
    u16 var_r7;
    u16 var_r5;
    u32 temp_r7_2;
    u32 var_r6;
    u32 var_r8;
    u32 var_r9;
    u8* temp_r10;

    var_r9                  = 0;
    OVMGR_U8(arg0, 0x41FE4) = 0;
    var_r6                  = 0xFFFF;
    var_r5                  = 0;
    var_r4                  = 0;
    if ((s32)data_02074d10.unk40B <= 0) {
        return;
    }
loop_3:
    var_r8 = 0;
    var_r7 = 0xFFFF;
    var_lr = 0;
    if ((s32)data_02074d10.unk40B > 0) {
        do {
            if (!(var_r9 & (1 << var_lr))) {
                temp_r11 = *(u16*)((u8*)&data_02071d10 + 0x3414 + (var_lr * 2));
                if (var_r8 <= (u32)temp_r11) {
                    var_r8 = (u32)temp_r11;
                    var_r7 = var_lr;
                }
            }
            temp_r11_2 = var_lr + 1;
            temp_gt    = (s32)data_02074d10.unk40B > (s32)temp_r11_2;
            var_lr     = temp_r11_2;
        } while (temp_gt);
    }
    if ((var_r6 != 0xFFFF) && (var_r8 != var_r6)) {
        var_r5 = (u16)(var_r5 + 1);
        if (var_r4 == 1) {
            OVMGR_U8(arg0, 0x41FE4) = 1;
        }
    }
    temp_r10                             = (u8*)arg0 + var_r4;
    var_r6                               = var_r8;
    *(u8*)(temp_r10 + 0x41FD9)           = (u8)var_r5;
    *(u8*)((u8*)arg0 + 0x41FDD + var_r7) = (u8)var_r5;
    *(s8*)(temp_r10 + 0x41FD5)           = (s8)var_r7;
    temp_r7                              = var_r4 + 1;
    temp_r7_2                            = temp_r7 << 0x10;
    var_r9 |= 1 << var_r7;
    var_r4 = temp_r7;
    if ((s32)data_02074d10.unk40B <= (s32)(temp_r7_2 >> 0x10)) {
        return;
    }
    goto loop_3;
}

void func_ov002_0208cc5c(void* arg0) {
    s32 temp_gt;
    s32 temp_gt_2;
    u16 temp_r2_2;
    u16 temp_r8;
    u16 temp_r8_2;
    u16 var_ip;
    u16 var_r3;
    u16 var_r4;
    u16 var_r9;
    u32 var_r6;
    u32 var_lr;
    u32 var_r5;
    u8  temp_r5;
    u8* mgr = (u8*)arg0;
    u8* temp_r2;
    u8* data_base = (u8*)&data_02071d10;

    OVMGR_U8(mgr, 0x41FE4) = 1;
    var_r6                 = 0;
    var_ip                 = 0;
    var_r3                 = 0;
    var_lr                 = 0xFFFF;
    if ((s32)data_02074d10.unk40B > 0) {
    loop_1:
        var_r5 = 0;
        var_r9 = 0;
        var_r4 = 0xFFFF;
        if ((s32)data_02074d10.unk40B > 0) {
            do {
                if (!(var_r6 & (1 << var_r9))) {
                    temp_r8 = *(u16*)(data_base + 0x3414 + (var_r9 * 2));
                    if (var_r5 <= (u32)temp_r8) {
                        var_r5 = (u32)temp_r8;
                        var_r4 = var_r9;
                    }
                }
                temp_r8_2 = var_r9 + 1;
                temp_gt   = (s32)data_02074d10.unk40B > (s32)temp_r8_2;
                var_r9    = temp_r8_2;
            } while (temp_gt);
        }
        if ((var_lr != 0xFFFF) && (var_r5 != var_lr)) {
            var_ip = (u16)(var_ip + 1);
        } else if ((var_r3 == 1) && (var_r4 == 0)) {
            var_ip = (u16)(var_ip + 1);
        }
        temp_r2                        = mgr + var_r3;
        temp_r2_2                      = var_r3 + 1;
        *(u8*)(temp_r2 + 0x41FD9)      = (u8)var_ip;
        *(u8*)(mgr + 0x41FDD + var_r4) = (u8)var_ip;
        *(s8*)(temp_r2 + 0x41FD5)      = (s8)var_r4;
        var_lr                         = var_r5;
        var_r6 |= 1 << var_r4;
        temp_gt_2 = (s32)data_02074d10.unk40B > (s32)temp_r2_2;
        var_r3    = temp_r2_2;
        if (!temp_gt_2) {
            goto block_16;
        }
        goto loop_1;
    }
block_16:
    if (OVMGR_U8(mgr, 0x41FD6) != 0) {
        return;
    }
    temp_r5 = OVMGR_U8(mgr, 0x41FD5);
    if (data_02075110.unk14 == data_02075124[temp_r5]) {
        OVMGR_U8(mgr, 0x41FD6) = temp_r5;
        OVMGR_U8(mgr, 0x41FD5) = 0U;
    }
}

s32 func_ov002_0208cdb0(void* arg0, u16 arg1) {
    SpriteFrameInfo sp0;
    u8*             mgr = (u8*)arg0;
    u8              index;

    sp0.unk_00 = 0;
    sp0.unk_04 = 0;
    sp0.unk_08 = 0;
    sp0.unk_0C = 0;
    sp0.unk_10 = 0;
    index      = *(u8*)(mgr + 0x41FD5 + arg1);
    return (s32)(mgr + 0x41838 + (index * 0x30));
}

void func_ov002_0208ce00(void* arg0) {
    struct {
        u32 unk0;
        u8  unk4;
        u8  unk5;
        u8  unk6;
        u8  unk7;
        u16 unk8;
    } sp0;
    void* menu_ptrs[4];
    u32   var_r0;
    s8    var_r8;
    u16   temp_r0_2;
    u32   temp_r0_3;
    u8    temp_r1;
    u8*   temp_r0;
    u8*   temp_r9;
    u8*   mgr       = (u8*)arg0;
    u8*   data_base = (u8*)&data_02071d10;

    menu_ptrs[0] = (u8*)arg0 + 0x474E8;
    menu_ptrs[1] = (u8*)arg0 + 0x476D0;
    menu_ptrs[2] = (u8*)arg0 + 0x478B8;
    menu_ptrs[3] = (u8*)arg0 + 0x47AA0;
    var_r8       = 0;
    if ((s32)data_02074d10.unk40B <= 0) {
        return;
    }
loop_3:
    temp_r0 = mgr + var_r8;
    temp_r1 = *(u8*)(temp_r0 + 0x41FD5);
    if ((OVMGR_U8(mgr, 0x41FE4) != 0) && (var_r8 == 0)) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    temp_r9 = menu_ptrs[var_r8];
    func_ov031_0210c94c(temp_r9, &data_ov002_020931b4, 0x220, var_r0, var_r8, *(u8*)(temp_r0 + 0x41FD9), temp_r1,
                        *(u16*)(data_base + 0x3414 + (temp_r1 * 2)));
    sp0.unk0 = var_r0;
    sp0.unk4 = (u8)var_r8;
    sp0.unk5 = *(u8*)(temp_r0 + 0x41FD9);
    sp0.unk6 = temp_r1;
    sp0.unk7 = 0;
    sp0.unk8 = *(u16*)(data_base + 0x3414 + (temp_r1 * 2));
    func_ov031_0210cc84(temp_r9, &data_ov002_02093228, &sp0);
    func_ov031_0210ce50((u8*)arg0 + 0x41804, temp_r9);
    if ((OVMGR_U8(mgr, 0x41FE4) != 0) && (*(u8*)(mgr + 0x41FDD + data_02074d10.unk40A) == 0)) {
        OVMGR_U8(mgr, 0x41FE8) = 2;
        goto block_16;
    }
    if (*(u8*)(mgr + 0x41FDD + data_02074d10.unk40A) == 0) {
        if (OVMGR_U32(mgr, 0x462EC) != 0) {
            OVMGR_U8(mgr, 0x41FE8) = 2;
            goto block_16;
        }
        OVMGR_U8(mgr, 0x41FE8) = 0;
        goto block_16;
    }
    OVMGR_U8(mgr, 0x41FE8) = 1;
block_16:
    temp_r0_2 = var_r8 + 1;
    temp_r0_3 = temp_r0_2 << 0x10;
    var_r8    = (s8)temp_r0_2;
    if ((s32)data_02074d10.unk40B <= (s32)(temp_r0_3 >> 0x10)) {
        return;
    }
    goto loop_3;
}

void func_ov002_0208cf84(void* arg1) {
    u16 var_ip;
    u8* mgr       = (u8*)arg1;
    u8* data_base = (u8*)&data_02071d10;

    var_ip                  = 0;
    OVMGR_U16(mgr, 0x46084) = 0;
    OVMGR_U16(mgr, 0x46086) = 0xC8;
    OVMGR_U8(mgr, 0x41FE9)  = 1;
    do {
        u16* value = (u16*)(data_base + 0x3414 + (var_ip * 2));
        if (*value > 0x270FU) {
            *value = 0x270FU;
        }
        var_ip += 1;
    } while ((u32)var_ip < 4U);
}

void func_ov002_0208cfe0(void* arg1) {
    func_ov002_02085710(arg1);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xC8;
}

void func_ov002_0208d008(void) {}

void func_ov002_0208d00c(void) {}

s32 func_ov002_0208d010(void* arg1) {
    u32 temp_r3;
    u8* mgr = (u8*)arg1;

    if ((OVMGR_U8(mgr, 0x41FE4) != 0) && (*(u8*)(mgr + 0x41FDD + data_02074d10.unk40A) == 0)) {
        data_02072d10.unkD84 = (u8)(data_02072d10.unkD84 + 1);
    }
    temp_r3              = data_02072d10.unkD88 + data_02075124[data_02074d10.unk40A];
    data_02072d10.unkD88 = temp_r3;
    if (temp_r3 > 0x270FU) {
        data_02072d10.unkD88 = 0x270FU;
    }
    func_ov031_0210cdac();
    return 2;
}

s32 func_ov002_0208d09c(void* arg1) {
    u16 temp_r1;
    u8* mgr = (u8*)arg1;

    temp_r1 = OVMGR_U16(mgr, 0x4196A);
    switch (temp_r1) { /* irregular */
        case 7:
            func_ov002_0208caa8(arg1);
            func_ov002_02082610(arg1);
            func_ov002_02085710(arg1);
            func_ov002_0208bd40();
            func_ov002_02085a44(arg1);
            if (data_02074d10.unk40A == 0) {
                func_ov031_0210cc84(arg1 + 0x4161C, &data_ov002_02092cf4);
            } else {
                func_ov031_0210cc84(arg1 + 0x4161C, &data_ov002_02092ce0);
            }
            break;
        case 8:
            func_ov031_0210cdac();
            break;
        default:
            OS_WaitForever();
            break;
    }
    return 0;
}

s32 func_ov002_0208d144(void) {
    func_ov031_0210cdac();
    return 0;
}

s32 func_ov002_0208d154(void* arg0, void* arg1) {
    u16 table_sp0[0xA];
    u16 i;
    u8* mgr = (u8*)arg1;

    for (i = 0; i < 5; i++) {
        table_sp0[i * 2]     = data_ov002_0209223e.data[i * 2];
        table_sp0[i * 2 + 1] = data_ov002_0209223e.data[i * 2 + 1];
    }
    OVMGR_U16(mgr, 0x46084) = 0;
    OVMGR_U16(mgr, 0x46086) = 0xC8;
    func_ov002_0208caa8(arg1);
    func_ov002_02084c84(arg1, table_sp0);
    func_ov031_0210c94c(mgr + 0x47C88, &data_ov002_020931cc, 0x40);
    func_ov031_0210cc84(mgr + 0x47C88, &data_ov002_020934b0, NULL);
    func_ov031_0210ce50(mgr + 0x41804, mgr + 0x47C88);
    OVMGR_U16(mgr, 0x474C8) = 0x3C;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208d22c(void* arg0, void* arg1) {
    u8* mgr = (u8*)arg1;

    OVMGR_U16(mgr, 0x474C8) = (u16)(OVMGR_U16(mgr, 0x474C8) - 1);
    if (OVMGR_U16(mgr, 0x474C8) == 0) {
        func_ov031_0210cf98(mgr + 0x41804, mgr + 0x47C88);
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_0208d27c(void* arg1) {
    OVMGR_U16(arg1, 0x46074) = 3;
    return 0;
}

s32 func_ov002_0208d290(void* arg1) {
    OVMGR_U16(arg1, 0x46074) = 4;
    return 0;
}

void func_ov002_0208d2a4(void* arg1) {
    OVMGR_U16(arg1, 0x41950) = 0;
}

s32 func_ov002_0208d2b4(void* arg0, void* arg1) {
    u8* msg       = (u8*)arg0;
    u8* mgr       = (u8*)arg1;
    u16 msg_unk10 = *(u16*)(msg + 0x10);
    u16 msg_unk2a = *(u16*)(msg + 0x2A);

    if ((u32)msg_unk10 >= 4U) {
        return 0;
    }
    func_0203b454((s32)(msg + 0x14), (s16*)(mgr + 0x41838 + (msg_unk2a * 0x30)), 0x16);
    func_0203b454((s32)(msg + 0xA), (s16*)(mgr + 0x41862 + (msg_unk10 * 0x30)), 6);
    OVMGR_U16(mgr, 0x41834)                  = (u16)(OVMGR_U16(mgr, 0x41834) | (1 << msg_unk10));
    OVMGR_U16(mgr, 0x41836)                  = (u16)(OVMGR_U16(mgr, 0x41836) | (1 << msg_unk10));
    *(u16*)(mgr + 0x4195A + (msg_unk10 * 2)) = 0;
    return 1;
}

void func_ov002_0208d368(void* arg0, void* arg1) {
    u16 temp_r3;
    u8* mgr = (u8*)arg1;

    temp_r3 = *(u16*)((u8*)arg0 + 0x10);
    if ((u32)temp_r3 >= 4U) {
        return;
    }
    OVMGR_U16(mgr, 0x41834) = (u16)(OVMGR_U16(mgr, 0x41834) & ~(1 << temp_r3));
    OVMGR_U16(mgr, 0x41836) = (u16)(OVMGR_U16(mgr, 0x41836) | (1 << temp_r3));
}

void func_ov002_0208d3b0(void* arg1) {
    OVMGR_U16(arg1, 0x4198C) = 0;
}

void func_ov002_0208d3c0(s32 arg0, void* arg1, void* arg2, s32 arg3) {
    u8* mgr = (u8*)arg3;

    if (arg1 == NULL) {
        return;
    }
    if (arg2 != (void*)0xA) {
        return;
    }
    if (arg3 == 0) {
        return;
    }
    *(u16*)(mgr + 0x4195A + (arg0 * 2)) = *(u16*)arg1;
    *(u16*)(mgr + 0x41962 + (arg0 * 2)) = *(u16*)((u8*)arg1 + 2);
}

void func_ov002_0208d3fc(void* arg0) {
    OVMGR_U16(arg0, 0x41990) = 1;
    OVMGR_U16(arg0, 0x41954) = 1;
    OS_DisableIRQ();
    OVMGR_U16(arg0, 0x41836) = 0;
    OS_RestoreIRQ();
}

void func_ov002_0208d430(void* arg0) {
    s32 var_ip;
    u8* mgr = (u8*)arg0;

    OVMGR_U16(mgr, 0x41990) = 0;
    OVMGR_U16(mgr, 0x4198A) = func_02047e84(OVMGR_U16(mgr, 0x41834));
    var_ip                  = 1;
    OVMGR_U16(mgr, 0x4198C) = 1;
    if (OVMGR_U16(mgr, 0x41994) != 0) {
        if ((u32)OVMGR_U16(mgr, 0x4198A) >= 4U) {
            var_ip = 0;
        }
    } else {
        var_ip = 0;
    }
    if (func_ov040_0209d788(func_ov002_0208d3b0, arg0, (u8*)arg0 + 0x4196E, 0x1E, var_ip) == 0) {
        OVMGR_U16(mgr, 0x4198C) = 0;
    }
}

s32 func_ov002_0208d4cc(void* arg0, void* arg1) {
    u8* mgr = (u8*)arg1;

    OVMGR_U16(mgr, 0x41834) = 0;
    OVMGR_U16(mgr, 0x41836) = 0;
    OVMGR_U16(mgr, 0x4198C) = 0;
    OVMGR_U16(mgr, 0x41990) = 0;
    OVMGR_U16(mgr, 0x41994) = 1;
    OVMGR_U16(mgr, 0x41950) = 0;
    OVMGR_U16(mgr, 0x41954) = 0;
    OVMGR_U16(mgr, 0x41958) = 0;
    func_0203b3c0(mgr + 0x4195A, 0, 8);
    func_0203b3c0(mgr + 0x41962, 0, 8);
    OVMGR_U16(mgr, 0x4196A) = 0;
    OVMGR_U16(mgr, 0x4198A) = 1;
    OVMGR_U16(mgr, 0x41834) = 1;
    func_ov031_0210c94c(mgr + 0x47C88, &data_ov002_020931cc, 0x40);
    func_ov031_0210cc84(mgr + 0x47C88, &data_ov002_020934b0, NULL);
    func_ov031_0210ce50(arg1 + 0x41804, arg1 + 0x47C88);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208d5c0(void* arg0, void* arg1) {
    s32 temp_r4;
    s32 temp_r5;
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        default:
            break;
        case 0:
            func_ov040_0209caac(0x400548);
            break;
        case 1:
            func_ov040_0209cb9c();
            break;
        case 7:
            temp_r5 = func_ov040_0209cde4();
            temp_r4 = func_020442f8();
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(2);
            func_ov040_0209cb08(arg1 + 0x4196E, 0x1E);
            func_ov040_0209d40c(func_ov002_0208d2b4, arg1);
            func_ov040_0209d420(func_ov002_0208d368, arg1);
            func_ov003_0209d434(func_ov002_0208d3c0, arg1);
            func_ov040_0209d0a8(0, temp_r4, temp_r5);
            break;
        case 4:
            func_ov031_0210cdac(arg0);
            break;
    }
    return 0;
}

s32 func_ov002_0208d6bc(void* arg0, void* arg1) {
    u16 table_sp0[0xA];
    u16 i;
    u32 temp_r3;
    u8* mgr = (u8*)arg1;

    for (i = 0; i < 5; i++) {
        table_sp0[i * 2]     = data_ov002_02092202.data[i * 2];
        table_sp0[i * 2 + 1] = data_ov002_02092202.data[i * 2 + 1];
    }
    OVMGR_U16(mgr, 0x46084) = 0;
    OVMGR_U16(mgr, 0x46086) = 0xC8;
    func_ov002_02084c84(arg1, table_sp0);
    temp_r3 = SystemStatusFlags << 0x19;
    SystemStatusFlags &= ~0x40;
    OVMGR_S16(mgr, 0x46070) = (s16)(temp_r3 >> 0x1F);
    SystemStatusFlags &= ~0x80;
    OVMGR_U16(mgr, 0x474C8) = 0x258;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208d778(void* arg0, void* arg1) {
    s32   sp4;
    void* sp0;
    s32   var_r4;
    s32   var_r9;
    s32   var_r9_2;
    u16   temp_r6;
    u32   var_r0;
    u8*   mgr = (u8*)arg1;

    sp0    = arg0;
    var_r4 = 0;
    func_ov002_020824a0();
    if (OVMGR_U16(mgr, 0x41836) != 0) {
        func_ov002_0208d3fc(arg1);
    }
    if ((OVMGR_U16(mgr, 0x41990) != 0) && (OVMGR_U16(mgr, 0x4198C) == 0)) {
        func_ov002_0208d430(arg1);
    }
    if ((OVMGR_U16(mgr, 0x41834) != 1) && (OVMGR_U16(mgr, 0x41950) == 0)) {
        if (OVMGR_U16(mgr, 0x41954) != 0) {
            OVMGR_U16(mgr, 0x41954) = 0;
            var_r4                  = 1;
            OVMGR_U16(mgr, 0x41958) = (u16)(OVMGR_U16(mgr, 0x41958) + 1);
        } else {
            var_r9 = 1;
        loop_16:
            if (var_r9 < 4) {
                temp_r6 = OVMGR_U16(mgr, 0x41834);
                if (temp_r6 & (1 << var_r9)) {
                    if (temp_r6 != *(u16*)(mgr + 0x4195A + (var_r9 * 2))) {
                        var_r4 = 1;
                    } else if (OVMGR_U16(mgr, 0x41958) != *(u16*)(mgr + 0x41962 + (var_r9 * 2))) {
                        var_r4 = 1;
                    } else {
                        goto block_15;
                    }
                } else {
                block_15:
                    var_r9 += 1;
                    goto loop_16;
                }
            }
            if ((var_r4 == 0) && (OVMGR_U16(mgr, 0x4198A) == data_02074d10.unk40B)) {
                OVMGR_U16(mgr, 0x4196A) = 4U;
                var_r4                  = 1;
            }
        }
    }
    if (var_r4 != 0) {
        sp4                    = OS_DisableIRQ();
        data_ov002_020935e0[0] = OVMGR_U16(mgr, 0x4196A);
        if (OVMGR_U16(mgr, 0x4196A) != 0) {

        } else {
            var_r9_2                  = 0;
            data_ov002_020935e0[1]    = OVMGR_U16(mgr, 0x41834);
            data_ov002_020935e0[0x3A] = OVMGR_U16(mgr, 0x41958);
        loop_31:
            if (var_r9_2 < 4) {
                if (OVMGR_U16(mgr, 0x41834) & (1 << var_r9_2)) {
                    if (var_r9_2 == 0) {
                        var_r0 = 0;
                    loop_28:
                        if (var_r0 < 0xBU) {
                            var_r0 = (u32)(u16)(var_r0 + 1);
                            goto loop_28;
                        }
                    }
                    func_0203b454((var_r9_2 * 0x30) + (s32)(mgr + 0x41838),
                                  (s16*)((var_r9_2 * 0x16) + (s32)&data_ov002_020935e0[2]), 0x16);
                }
                var_r9_2 += 1;
                goto loop_31;
            }
        }
        OS_RestoreIRQ(sp4);
        OVMGR_U16(mgr, 0x41950) = 1;
        if (func_ov040_0209d48c(data_ov002_020935e0, 0x78, func_ov002_0208d2a4, arg1) == 0) {
            OVMGR_U16(mgr, 0x41950) = 0;
        }
    }
    OVMGR_U16(mgr, 0x474C8) = (u16)(OVMGR_U16(mgr, 0x474C8) - 1);
    if (OVMGR_U16(mgr, 0x474C8) == 0) {
        func_ov031_0210cc84(mgr + 0x4161C, &data_ov002_02092e18, NULL);
        return 0;
    }
    if (OVMGR_U16(mgr, 0x4196A) == 4) {
        func_ov031_0210cdac(sp0);
    }
    return 0;
}

s32 func_ov002_0208da24(void* arg0, void* arg1) {
    u8* mgr = (u8*)arg1;
    u16 table_sp4[0xF];
    s32 sp24[0x19];
    u16 i;
    u16 temp_r0_3;
    u16 var_r5;
    u32 temp_r0_2;

    for (i = 0; i < 0xF; i++) {
        table_sp4[i] = data_ov002_02092252[i];
    }
    sp24[0] = 0;
    sp24[1] = 0;
    sp24[2] = 0;
    sp24[3] = 0;
    sp24[4] = 0;
    func_ov031_0210cff0(mgr + 0x41804, mgr + 0x47C88);
    func_ov002_0208cb50(arg1);
    var_r5 = 0;
    if ((s32)data_02074d10.unk40B > 0) {
        do {
            (&sp24[0])[var_r5] = func_ov002_0208cdb0(arg1, var_r5);
            temp_r0_3          = var_r5 + 1;
            temp_r0_2          = temp_r0_3 << 0x10;
            var_r5             = temp_r0_3;
        } while ((s32)data_02074d10.unk40B > (s32)(temp_r0_2 >> 0x10));
    }
    sp24[var_r5] = 0;
    func_ov002_020850c0(arg1, 0xA, 0xC, sp24, table_sp4);
    func_ov002_0208ce00(arg1);
    OVMGR_U16(mgr, 0x4196C) = 0xFFFF;
    OVMGR_U16(mgr, 0x474CA) = OVMGR_U16(mgr, 0x41834);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208db58(void* arg0, void* arg1) {
    u16                spC;
    s32                temp_ip;
    s32                temp_r5;
    s32                var_r2;
    const Ov002_U16_2* var_r3;
    Ov002_U16_2*       var_r4;
    u16                temp_r0;
    u16                temp_r0_2;
    u16                temp_r1;
    u16                temp_r4;
    u16*               temp_r1_2;
    u8*                mgr = (u8*)arg1;

    var_r3 = (const Ov002_U16_2*)data_ov002_02092270;
    var_r4 = (Ov002_U16_2*)&spC;
    var_r2 = 7;
    do {
        temp_r1 = var_r3->unk0;
        temp_r0 = var_r3->unk2;
        var_r3 += 4;
        var_r4->unk0 = temp_r1;
        var_r4->unk2 = temp_r0;
        var_r4 += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);
    *var_r4 = *var_r3;
    temp_r4 = func_ov002_0208597c(&spC, data_ov002_020921c8.unk6, data_ov002_020921c8.unk8, data_ov002_020921c8.unkA,
                                  data_ov002_020921c8.unkC, data_ov002_020921c8.unkE, data_ov002_020921c8.unk10);
    if (OVMGR_U16(mgr, 0x474CA) != func_ov040_0209cb68()) {
        func_ov031_0210cc84(mgr + 0x4161C, &data_ov002_02092e18, NULL);
        return 0;
    }
    if (temp_r4 == 0xFFFF) {
        return 0;
    }
    if (temp_r4 == 0xFFFE) {
        OVMGR_U16(mgr, 0x4196C) = 0xFFFFU;
        OVMGR_U16(mgr, 0x46084) = 0U;
        OVMGR_U16(mgr, 0x46086) = 0xD2U;
        return 0;
    }
    if (OVMGR_U16(mgr, 0x4196C) == temp_r4) {
        switch (temp_r4) { /* switch 1; irregular */
            case 0:        /* switch 1 */
                func_02026b20(0x347);
                OVMGR_U16(mgr, 0x4196A) = 7U;
                break;
            case 1: /* switch 1 */
                func_02026b20(0x347);
                OVMGR_U16(mgr, 0x4196A) = 8U;
                break;
        }
    } else {
        func_02026b20(0x346);
        temp_ip                 = temp_r4 * 6;
        OVMGR_U16(mgr, 0x4196C) = temp_r4;
        temp_r1_2               = subroutine_arg0 + temp_ip;
        OVMGR_U16(mgr, 0x46084) = temp_r1_2[1];
        OVMGR_U16(mgr, 0x46086) = temp_r1_2[2];
        Sprite_ChangeAnimation((Sprite*)(mgr + 0x46078), OVMGR_PTR(mgr, 0x46090), (s16)temp_r1_2[0], OVMGR_PTR(mgr, 0x46094));
    }
    temp_r5                = OS_DisableIRQ();
    data_ov002_020935e0[0] = OVMGR_U16(mgr, 0x4196A);
    temp_r0_2              = OVMGR_U16(mgr, 0x4196A);
    switch (temp_r0_2) { /* switch 2; irregular */
        case 7:          /* switch 2 */
            data_ov002_020935e0[0x3B] = OVMGR_U16(mgr, 0x4196C);
            func_ov031_0210cdac(arg0);
            break;
        case 8: /* switch 2 */
            data_ov002_020935e0[0x3B] = OVMGR_U16(mgr, 0x4196C);
            func_ov031_0210cdac(arg0);
            break;
    }
    OS_RestoreIRQ(temp_r5);
    func_ov040_0209d48c(data_ov002_020935e0, 0x78, func_ov002_0208d2a4, arg1);
    return 0;
}

void func_ov002_0208dda4(void* arg1, void* arg2, void* arg3) {
    typedef struct {
        u16 unk0;
        u16 unk2;
        u8  filler04[0x70];
        u16 unk74;
        u16 unk76;
    } Ov002_8dda4Data;
    Ov002_8dda4Data* msg = (Ov002_8dda4Data*)arg1;
    s16*             var_r5;
    s32              var_r4;
    u16              temp_r0;
    void*            var_r6;
    u8*              mgr = (u8*)arg3;

    if (arg1 == NULL) {
        return;
    }
    if (arg2 != (void*)0x78) {
        return;
    }
    if (arg3 == NULL) {
        return;
    }
    OVMGR_U16(mgr, 0x4196A) = msg->unk0;
    temp_r0                 = msg->unk0;
    switch (temp_r0) { /* irregular */
        case 3:
            return;
        case 0:
            var_r6 = (u8*)msg + 4;
            var_r5 = (s16*)(mgr + 0x41838);
            var_r4 = 0;
            do {
                if (msg->unk2 & (1 << var_r4)) {
                    func_0203b454((s32)var_r6, var_r5, 0x16);
                }
                var_r4 += 1;
                var_r5 += 0x30;
                var_r6 += 0x16;
            } while (var_r4 < 4);
            OVMGR_U16(mgr, 0x41834) = msg->unk2;
            OVMGR_U16(mgr, 0x41836) = 0xF;
            OVMGR_U16(mgr, 0x41958) = msg->unk74;
            return;
    }
}

s32 func_ov002_0208de64(void* arg0, void* arg1) {
    u16 table_sp0[0xA];
    u16 i;
    u32 temp_r1_2;
    u8* mgr = (u8*)arg1;

    for (i = 0; i < 5; i++) {
        table_sp0[i * 2]     = data_ov002_020921da.data[i * 2];
        table_sp0[i * 2 + 1] = data_ov002_020921da.data[i * 2 + 1];
    }
    OVMGR_U16(mgr, 0x46084) = 0;
    OVMGR_U16(mgr, 0x46086) = 0xC8;
    func_ov002_02084c84(arg1, table_sp0);
    temp_r1_2 = SystemStatusFlags << 0x19;
    SystemStatusFlags &= ~0x40;
    OVMGR_S16(mgr, 0x46070) = (s16)(temp_r1_2 >> 0x1F);
    SystemStatusFlags &= ~0x80;
    func_ov031_0210c94c(mgr + 0x47C88, &data_ov002_020931cc, 0x40);
    func_ov031_0210cc84(mgr + 0x47C88, &data_ov002_020934b0, NULL);
    func_ov031_0210ce50(mgr + 0x41804, mgr + 0x47C88);
    OVMGR_U16(mgr, 0x474C8) = 0x258;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208df6c(void* arg0, void* arg1) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        case 2:
        case 3:
            break;
        default:
            break;
        case 0:
            func_ov040_0209caac(0x400548);
            break;
        case 1:
            func_ov040_0209d818(3, 0, 0x78, 0xA, 1);
            func_ov040_0209d848(2);
            func_ov040_0209cabc(arg1 + 0x4181C, 0x18);
            func_ov040_0209b8b8(1, arg1 + 0x41862, 0);
            func_ov003_0209d434((void (*)(s32, void*, void*, s32))func_ov002_0208dda4, arg1);
            break;
        case 4:
            func_ov031_0210cdac(arg0);
            break;
    }
    OVMGR_U16(arg1, 0x474C8) = (u16)(OVMGR_U16(arg1, 0x474C8) - 1);
    if (OVMGR_U16(arg1, 0x474C8) != 0) {
        return 0;
    }
    func_ov031_0210cc84(arg1 + 0x4161C, &data_ov002_02092e18, NULL);
    return 0;
}

void func_ov002_0208e070(void* arg1) {
    OVMGR_U16(arg1, 0x41950) = 0;
}

s32 func_ov002_0208e080(void* arg1) {
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    OVMGR_U16(arg1, 0x474CA) = 0;
    OVMGR_U16(arg1, 0x474CC) = 0;
    func_ov031_0210cdac();
    return 0;
}

s32 func_ov002_0208e0b8(void* arg0, void* arg1) {
    if (OVMGR_U16(arg1, 0x41836) != 0) {
        OVMGR_U16(arg1, 0x474CC) = 1U;
    }
    if ((OVMGR_U16(arg1, 0x41950) == 0) && (OVMGR_U16(arg1, 0x474CC) != 0)) {
        OS_DisableIRQ();
        data_ov002_020935c0[0] = OVMGR_U16(arg1, 0x41834);
        data_ov002_020935c0[1] = OVMGR_U16(arg1, 0x41958);
        OS_RestoreIRQ();
        OVMGR_U16(arg1, 0x474CC) = 0U;
        OVMGR_U16(arg1, 0x41950) = 1;
        if (func_ov040_0209d48c(data_ov002_020935c0, 0xA, func_ov002_0208e070, arg1) == 0) {
            OVMGR_U16(arg1, 0x41950) = 0;
        }
    }
    if (OVMGR_U16(arg1, 0x4196A) == 4) {
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

void func_ov002_0208e194(void* arg1, void* arg2, void* arg3) {
    u16 temp_r2;

    if (arg1 == NULL) {
        return;
    }
    if (arg2 != (void*)0x78) {
        return;
    }
    if (arg3 == NULL) {
        return;
    }
    temp_r2 = OVMGR_U16(arg1, 0x0);
    switch (temp_r2) { /* irregular */
        case 3:
        case 8:
            OVMGR_U16(arg3, 0x4196A) = 8U;
            OVMGR_U16(arg3, 0x4196C) = (u16)OVMGR_U16(arg1, 0x76);
            return;
        case 7:
            OVMGR_U16(arg3, 0x4196A) = temp_r2;
            OVMGR_U16(arg3, 0x4196C) = (u16)OVMGR_U16(arg1, 0x76);
            return;
    }
}

s32 func_ov002_0208e200(void* arg0, void* arg1) {
    u16 table_sp4[0xA];
    s32 sp18[0x19];
    u16 i;
    u16 temp_r0_3;
    u16 var_r5;
    u32 temp_r0_2;

    for (i = 0; i < 0xA; i++) {
        table_sp4[i] = ((const u16*)&data_ov002_0209222a)[i];
    }
    sp18[0] = 0;
    sp18[1] = 0;
    sp18[2] = 0;
    sp18[3] = 0;
    sp18[4] = 0;
    func_ov002_0208cb50(arg1);
    var_r5 = 0;
    if ((s32)data_02074d10.unk40B > 0) {
        do {
            sp18[var_r5] = func_ov002_0208cdb0(arg1, var_r5);
            temp_r0_3    = var_r5 + 1;
            temp_r0_2    = temp_r0_3 << 0x10;
            var_r5       = temp_r0_3;
        } while ((s32)data_02074d10.unk40B > (s32)(temp_r0_2 >> 0x10));
    }
    sp18[var_r5] = 0;
    func_ov002_020850c0(arg1, 0xFFFF, 0xB, sp18, table_sp4);
    func_ov002_0208ce00(arg1);
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    func_ov003_0209d434((void (*)(s32, void*, void*, s32))func_ov002_0208e194, arg1);
    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov002_0208e30c(void* arg1) {
    u16 temp_r1;

    temp_r1 = OVMGR_U16(arg1, 0x4196A);
    switch (temp_r1) { /* irregular */
        case 3:
        case 8:
            func_ov031_0210cdac();
            break;
        case 7:
            func_ov031_0210cdac();
            break;
    }
    return 0;
}

s32 func_ov002_0208e348(void* arg0, void* arg1) {
    void* sp0;
    s32   temp_r0;
    u16   var_r9;

    sp0                      = arg0;
    var_r9                   = 0;
    OVMGR_S32(arg1, 0x41FD0) = 0;
loop_1:
    if (((const u16*)&data_ov002_0209228e[data_02074d10.unk41C])[var_r9] != 0xFFFF) {
        temp_r0 = func_ov031_0210b698(arg1 + 0x45FF4);
        func_0203b270(temp_r0, (var_r9 * 0x30) + (s32)(arg1 + 0x41838), 0x2A);
        Mem_Free(&gDebugHeap, temp_r0);
        var_r9 += 1;
        OVMGR_S32(arg1, 0x41FD0) = (s32)(OVMGR_S32(arg1, 0x41FD0) + 1);
        if ((u32)var_r9 < 4U) {
            goto loop_1;
        }
    }
    data_02074d10.unk40B = (s8)var_r9;
    func_ov031_0210cdac(sp0);
    return 0;
}

s32 func_ov002_0208e410(void* arg0, void* arg1) {
    u16 table_sp4[0xA];
    s32 sp18[0x19];
    u16 i;
    u16 temp_r0_3;
    u16 var_r5;
    u32 temp_r0_2;

    for (i = 0; i < 0xA; i++) {
        table_sp4[i] = ((const u16*)&data_ov002_02092216)[i];
    }
    sp18[0] = 0;
    sp18[1] = 0;
    sp18[2] = 0;
    sp18[3] = 0;
    sp18[4] = 0;
    func_ov002_0208cc5c(arg1);
    var_r5 = 0;
    if ((s32)OVMGR_S32(arg1, 0x41FD0) > 0) {
        do {
            sp18[var_r5] = func_ov002_0208cdb0(arg1, var_r5);
            temp_r0_3    = var_r5 + 1;
            temp_r0_2    = temp_r0_3 << 0x10;
            var_r5       = temp_r0_3;
        } while ((s32)OVMGR_S32(arg1, 0x41FD0) > (s32)(temp_r0_2 >> 0x10));
    }
    sp18[var_r5] = 0;
    func_ov002_020850c0(arg1, 0xE, 0xD, sp18, table_sp4);
    func_ov002_0208ce00(arg1);
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    OVMGR_U16(arg1, 0x4196C) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208e514(void* arg0, void* arg1) {
    u16 table_sp6[0xA];
    u16 sp4;
    u16 sp2;
    s32 temp_ip;
    u16 temp_r0_2;
    u16 i;

    for (i = 0; i < 5; i++) {
        table_sp6[i * 2]     = data_ov002_020921ee.data[i * 2];
        table_sp6[i * 2 + 1] = data_ov002_020921ee.data[i * 2 + 1];
    }
    sp2       = data_ov002_020921c8.unk2;
    sp4       = data_ov002_020921c8.unk4;
    temp_r0_2 = func_ov002_0208597c(table_sp6, data_ov002_020921c8.unk0);
    if (temp_r0_2 == 0xFFFF) {
        return 0;
    }
    if (temp_r0_2 == 0xFFFE) {
        OVMGR_U16(arg1, 0x4196C) = 0xFFFFU;
        OVMGR_U16(arg1, 0x46084) = 0U;
        OVMGR_U16(arg1, 0x46086) = 0xD2U;
        return 0;
    }
    if (OVMGR_U16(arg1, 0x4196C) == temp_r0_2) {
        if (temp_r0_2 == 0) {
            func_02026b20(0x347);
            func_ov031_0210cdac(arg0);
        }
    } else {
        func_02026b20(0x346);
        temp_ip                  = temp_r0_2 * 3;
        OVMGR_U16(arg1, 0x4196C) = temp_r0_2;
        OVMGR_U16(arg1, 0x46084) = table_sp6[temp_ip + 1];
        OVMGR_U16(arg1, 0x46086) = table_sp6[temp_ip + 2];
        Sprite_ChangeAnimation((Sprite*)((u8*)arg1 + 0x46078), OVMGR_U16(arg1, 0x46090), (s16)table_sp6[temp_ip],
                               OVMGR_U16(arg1, 0x46094));
    }
    return 0;
}

s32 func_ov002_0208e658(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        default:
            break;
        case 0:
            func_ov031_0210cdac(arg0);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return 0;
}

s32 func_ov002_0208e6ac(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        case 0:
        case 3:
            break;
        default:
            break;
        case 1:
            func_ov031_0210cdac(arg0);
            break;
        case 4:
            func_ov040_0209d588();
            break;
        case 2:
            func_ov040_0209c158();
            break;
    }
    return 0;
}

s32 func_ov002_0208e6f8(void* arg0, void* arg1) {
    if (OVMGR_U16(arg1, 0x474CA) != func_ov040_0209cb68()) {
        func_ov031_0210cc84(arg1 + 0x4161C, &data_ov002_02092e18, NULL);
        return 0;
    }
    func_ov031_0210cdac(arg0);
    return 2;
}

s32 func_ov002_0208e750(void* arg0, void* arg1) {
    if (func_ov040_0209cb68() == 1) {
        func_ov031_0210cdac(arg0);
        return 0;
    }
    OS_DisableIRQ();
    *data_ov002_020935e0 = 3;
    OS_RestoreIRQ();
    if (func_ov040_0209d48c(data_ov002_020935e0, 0xA, func_ov002_0208e070, arg1) != 0) {
        return 0;
    }
    func_ov031_0210cc84(arg1 + 0x4161C, &data_ov002_02092e18, NULL);
    return 0;
}

s32 func_ov002_0208e7d8(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        default:
        case 2:
            func_ov040_0209d588();
            break;
        case 0:
            func_ov031_0210cdac(arg0);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 4:
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
            break;
    }
    return 0;
}

s32 func_ov002_0208e838(void* arg0) {
    u32 temp_r0;

    temp_r0 = func_ov040_0209cb78();
    switch (temp_r0) {
        case 0:
        case 3:
            break;
        default:
        case 2:
            func_ov040_0209d588();
            break;
        case 1:
            func_ov031_0210cdac(arg0);
            break;
        case 4:
            if (func_ov040_0209cb68() == 1) {
                func_ov040_0209d588();
            }
            break;
    }
    return 0;
}

SpriteFrameInfo* func_ov002_0208e890(Sprite* arg0, s32 arg1, s32 arg2) {
    (void)arg1;
    switch (arg2) { /* irregular */
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            return Ov002_GetSpriteFrameInfo(arg0);
        default:
            return NULL;
    }
}

void func_ov002_0208e92c(void* arg1, void* arg2) {
    SpriteAnimation anim = data_ov002_0209241c;
    u16             table_pairs[0x10];
    s16             sp2E;
    u16             sp2C;
    u16             sp2A;
    s16             sp26;
    u16             sp24;
    u16             sp20;
    u16             i;
    u16             base_index;
    u16*            anim_u16;

    for (i = 0; i < 8; i++) {
        table_pairs[i * 2]     = data_ov002_020923fc[i].unk0;
        table_pairs[i * 2 + 1] = data_ov002_020923fc[i].unk2;
    }
    anim_u16    = (u16*)&anim;
    anim_u16[0] = data_ov002_020923f0.unk0;
    anim_u16[1] = data_ov002_020923f0.unk2;

    OVMGR_S32(arg1, 0x104) = 0x80000;
    OVMGR_S32(arg1, 0x108) = 0x1C0000;
    base_index             = (u16)(OVMGR_U16(arg2, 0x0) * 8);
    sp20                   = table_pairs[base_index + 0];
    sp26                   = 4;
    sp24                   = table_pairs[base_index + 1];
    sp2A                   = table_pairs[base_index + 2];
    sp2C                   = table_pairs[base_index + 3];
    sp2E                   = 1;
    anim_u16[0]            = (u16)(anim_u16[0] & ~3);
    anim_u16[1]            = (u16)((anim_u16[1] & ~0xC00) | (((u32)subroutine_arg0[OVMGR_U16(arg2, 0x0)] << 0x1E) >> 0x14));
    anim.unk_1C            = (s16)sp20;
    anim.unk_20            = (s16)sp24;
    anim.unk_22            = sp26;
    anim.unk_26            = (s16)sp2A;
    anim.unk_28            = (s16)sp2C;
    anim.unk_2A            = sp2E;
    if (_Sprite_Load((Sprite*)((u8*)arg1 + 4), &anim) == 0) {
        OS_WaitForever();
    }
    sp2E        = 3;
    anim.unk_2A = sp2E;
    if (_Sprite_Load((Sprite*)((u8*)arg1 + 0x44), &anim) == 0) {
        OS_WaitForever();
    }
    anim_u16[0] = (u16)((anim_u16[0] & ~3) | 1);
    sp2E        = 1;
    anim.unk_2A = sp2E;
    if (_Sprite_Load((Sprite*)((u8*)arg1 + 0x84), &anim) == 0) {
        OS_WaitForever();
    }
    sp2E        = 3;
    anim.unk_2A = sp2E;
    if (_Sprite_Load((Sprite*)((u8*)arg1 + 0xC4), &anim) == 0) {
        OS_WaitForever();
    }
    OVMGR_U16(arg1, 0x110) = (u16)OVMGR_U16(arg2, 0x2);
    OVMGR_U16(arg1, 0x112) = (u16)OVMGR_U16(arg2, 0x2);
    OVMGR_U16(arg1, 0x0)   = (u16)OVMGR_U16(arg2, 0x0);
}

void func_ov002_0208eb04(s32 arg1) {
    u16 var_r6;

    var_r6 = 0;
    do {
        Sprite_Destroy(arg1 + 4 + (var_r6 << 6));
        Sprite_Destroy(arg1 + 0x84 + (var_r6 << 6));
        var_r6 += 1;
    } while ((u32)var_r6 < 2U);
}

void func_ov002_0208eb3c(void* arg1) {
    u16 var_r6;

    OVMGR_S16(arg1, 0x10) = (s16)((s32)OVMGR_S32(arg1, 0x104) >> 0xC);
    OVMGR_S16(arg1, 0x12) = (s16)((s32)(OVMGR_S32(arg1, 0x108) - 0x40000) >> 0xC);
    OVMGR_S16(arg1, 0x90) = (s16)((s32)OVMGR_S32(arg1, 0x104) >> 0xC);
    OVMGR_S16(arg1, 0x92) = (s16)((s32)(OVMGR_S32(arg1, 0x108) + 0x80000) >> 0xC);
    OVMGR_S16(arg1, 0x50) = (s16)((s32)OVMGR_S32(arg1, 0x104) >> 0xC);
    OVMGR_S16(arg1, 0x52) = (s16)((s32)(OVMGR_S32(arg1, 0x108) + 0x40000) >> 0xC);
    OVMGR_S16(arg1, 0xD0) = (s16)((s32)OVMGR_S32(arg1, 0x104) >> 0xC);
    OVMGR_S16(arg1, 0xD2) = (s16)((s32)(OVMGR_S32(arg1, 0x108) + 0x100000) >> 0xC);
    var_r6                = 0;
    do {
        Sprite_UpdateAndCheck(arg1 + 4 + (var_r6 << 6));
        Sprite_UpdateAndCheck(arg1 + 0x84 + (var_r6 << 6));
        var_r6 += 1;
    } while ((u32)var_r6 < 2U);
}

void func_ov002_0208ebe4(s32 arg1) {
    u16 var_r6;

    var_r6 = 0;
    do {
        Sprite_Render(arg1 + 4 + (var_r6 << 6));
        Sprite_Render(arg1 + 0x84 + (var_r6 << 6));
        var_r6 += 1;
    } while ((u32)var_r6 < 2U);
}

s32 func_ov002_0208ec1c(void* arg1) {
    OVMGR_S32(arg1, 0x10C) = 0;
    func_ov031_0210cdac();
    return 2;
}

s32 func_ov002_0208ec34(void* arg0, void* arg1) {
    f32 var_r0;
    f32 var_r0_2;
    s32 temp_r0;
    s32 temp_r0_3;
    u16 temp_r0_2;
    u16 temp_r1;
    u16 temp_r2;

    temp_r1 = subroutine_arg0[OVMGR_U16(arg1, 0x0)];
    temp_r0 = temp_r1 << 0xC;
    if (temp_r1 != 0) {
        var_r0 = 0.5f + (f32)temp_r0;
    } else {
        var_r0 = (f32)temp_r0 - 0.5f;
    }
    func_020265d4(arg1 + 0x108, (s32)var_r0, OVMGR_U16(arg1, 0x112));
    func_020265d4(arg1 + 0x10C, 0x10000, OVMGR_U16(arg1, 0x112));
    temp_r2 = OVMGR_U16(arg1, 0x112);
    if (temp_r2 == 0) {
        func_ov031_0210cdac(arg0);
        return 0;
    }
    if (data_ov002_02093660 == 1) {
        temp_r0_2 = subroutine_arg0[OVMGR_U16(arg1, 0x0)];
        temp_r0_3 = temp_r0_2 << 0xC;
        if (temp_r0_2 != 0) {
            var_r0_2 = 0.5f + (f32)temp_r0_3;
        } else {
            var_r0_2 = (f32)temp_r0_3 - 0.5f;
        }
        OVMGR_S32(arg1, 0x108) = (s32)var_r0_2;
        func_ov031_0210cdac(arg0);
        return 0;
    }
    OVMGR_U16(arg1, 0x112) = (u16)(temp_r2 - 1);
    return 0;
}

void func_ov002_0208ed78(void* arg1, void* arg2) {
    OVMGR_S32(arg1, 0x104) = (s32)OVMGR_U32(arg2, 0x4);
    OVMGR_S32(arg1, 0x108) = (s32)OVMGR_U32(arg2, 0x8);
    OVMGR_U16(arg1, 0x114) = (u16)OVMGR_U16(arg2, 0x0);
    OVMGR_U16(arg1, 0x116) = (u16)OVMGR_U16(arg2, 0x0);
    OVMGR_S32(arg1, 0x110) = -0x200000;
}

void func_ov002_0208edac(void) {}

void func_ov002_0208edb0(void* arg1) {
    s32   temp_r0;
    s32   temp_r0_3;
    s32   temp_r4;
    s32   temp_r4_2;
    u32   temp_r1;
    u32   temp_r2_2;
    void* temp_r0_2;
    void* temp_r0_4;
    void* temp_r2;
    void* temp_r2_3;

    temp_r2   = OVMGR_PTR(arg1, 0x104);
    temp_r0   = OVMGR_U32(temp_r2, 0x4);
    temp_r4   = (OVMGR_U32(temp_r2, 0x0) * 0x220) + (s32)(&g_DisplaySettings + 0x64);
    temp_r2_2 = *(u32*)((u8*)temp_r4 + (temp_r0 * 0x3C));
    switch (temp_r2_2) { /* switch 1 */
        default:         /* switch 1 */
            break;
        case 1:          /* switch 1 */
        case 2:          /* switch 1 */
        case 3:          /* switch 1 */
        case 4:          /* switch 1 */
        case 5:          /* switch 1 */
            *(u32*)((u8*)temp_r4 + (temp_r0 * 0x2C) + 0x124) = 1;
            break;
    }
    temp_r0_2                 = (u8*)temp_r4 + (temp_r0 * 8);
    *(s32*)(temp_r0_2 + 0xF0) = 0;
    *(s32*)(temp_r0_2 + 0xF4) = (s32)OVMGR_S32(arg1, 0x110);
    temp_r2_3                 = OVMGR_PTR(arg1, 0x108);
    temp_r0_3                 = OVMGR_U32(temp_r2_3, 0x4);
    temp_r4_2                 = (OVMGR_U32(temp_r2_3, 0x0) * 0x220) + (s32)(&g_DisplaySettings + 0x64);
    temp_r1                   = *(u32*)((u8*)temp_r4_2 + (temp_r0_3 * 0x3C));
    switch (temp_r1) { /* switch 2 */
        default:       /* switch 2 */
            break;
        case 1:        /* switch 2 */
        case 2:        /* switch 2 */
        case 3:        /* switch 2 */
        case 4:        /* switch 2 */
        case 5:        /* switch 2 */
            *(u32*)((u8*)temp_r4_2 + (temp_r0_3 * 0x2C) + 0x124) = 1;
            break;
    }
    temp_r0_4                 = (u8*)temp_r4_2 + (temp_r0_3 * 8);
    *(s32*)(temp_r0_4 + 0xF0) = 0;
    *(s32*)(temp_r0_4 + 0xF4) = (s32)(OVMGR_S32(arg1, 0x110) + 0x100000);
}

void func_ov002_0208ee98(void) {}

s32 func_ov002_0208ee9c(void) {
    func_ov031_0210cdac();
    return 2;
}

s32 func_ov002_0208eeac(void* arg0, void* arg1) {
    s32   temp_r0;
    s32   temp_r0_3;
    s32   temp_r3;
    s32   temp_r3_2;
    u16   temp_r2;
    u32   temp_r1_2;
    u32   temp_r1_4;
    void* temp_r0_2;
    void* temp_r1;
    void* temp_r1_3;
    void* temp_r1_5;
    void* temp_r2_2;
    void* temp_r2_3;

    func_020265d4(arg1 + 0x110, 0, OVMGR_U16(arg1, 0x116));
    temp_r2 = OVMGR_U16(arg1, 0x116);
    if (temp_r2 == 0) {
        func_ov031_0210cdac(arg0);
        return 0;
    }
    if (data_ov002_02093660 == 1) {
        OVMGR_S32(arg1, 0x110) = 0;
        temp_r1                = OVMGR_PTR(arg1, 0x104);
        temp_r0                = OVMGR_U32(temp_r1, 0x4);
        temp_r3                = (OVMGR_U32(temp_r1, 0x0) * 0x220) + (s32)(&g_DisplaySettings + 0x64);
        temp_r1_2              = *(u32*)((u8*)temp_r3 + (temp_r0 * 0x3C));
        switch (temp_r1_2) { /* switch 1 */
            default:         /* switch 1 */
                break;
            case 1:          /* switch 1 */
            case 2:          /* switch 1 */
            case 3:          /* switch 1 */
            case 4:          /* switch 1 */
            case 5:          /* switch 1 */
                *(u32*)((u8*)temp_r3 + (temp_r0 * 0x2C) + 0x124) = 1;
                break;
        }
        temp_r0_2                 = (u8*)temp_r3 + (temp_r0 * 8);
        *(s32*)(temp_r0_2 + 0xF0) = 0;
        *(s32*)(temp_r0_2 + 0xF4) = 0;
        temp_r1_3                 = OVMGR_PTR(arg1, 0x108);
        temp_r0_3                 = OVMGR_U32(temp_r1_3, 0x4);
        temp_r3_2                 = (OVMGR_U32(temp_r1_3, 0x0) * 0x220) + (s32)(&g_DisplaySettings + 0x64);
        temp_r1_4                 = *(u32*)((u8*)temp_r3_2 + (temp_r0_3 * 0x3C));
        switch (temp_r1_4) { /* switch 2 */
            default:         /* switch 2 */
                break;
            case 1:          /* switch 2 */
            case 2:          /* switch 2 */
            case 3:          /* switch 2 */
            case 4:          /* switch 2 */
            case 5:          /* switch 2 */
                *(u32*)((u8*)temp_r3_2 + (temp_r0_3 * 0x2C) + 0x124) = 1;
                break;
        }
        temp_r1_5                 = (u8*)temp_r3_2 + (temp_r0_3 * 8);
        *(s32*)(temp_r1_5 + 0xF0) = 0;
        *(s32*)(temp_r1_5 + 0xF4) = 0x100000;
        temp_r2_2                 = OVMGR_PTR(arg1, 0x104);
        OVMGR_S32(temp_r2_2, 0x8) = (s32)(OVMGR_S32(temp_r2_2, 0x8) | 2);
        temp_r2_3                 = OVMGR_PTR(arg1, 0x108);
        OVMGR_S32(temp_r2_3, 0x8) = (s32)(OVMGR_S32(temp_r2_3, 0x8) | 2);
        func_ov031_0210cdac(arg0);
        return 0;
    }
    OVMGR_U16(arg1, 0x116) = (u16)(temp_r2 - 1);
    return 0;
}

SpriteFrameInfo* func_ov002_0208f020(Sprite* arg0, s32 arg1, s32 arg2) {
    (void)arg1;
    switch (arg2) { /* irregular */
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            return Ov002_GetSpriteFrameInfo(arg0);
        default:
            return NULL;
    }
}

void func_ov002_0208f0bc(void* arg1, void* arg2) {
    SpriteAnimation    anim        = data_ov002_020924cc;
    const u16          table_48[4] = {data_ov002_02092448.unk10, data_ov002_02092448.unk12, data_ov002_02092448.unk14,
                                      data_ov002_02092448.unk16};
    const u16          table_40[4] = {data_ov002_02092448.unk20, data_ov002_02092448.unk22, data_ov002_02092448.unk24,
                                      data_ov002_02092448.unk26};
    const u16          table_38[4] = {data_ov002_02092448.unk0, data_ov002_02092448.unk2, data_ov002_02092448.unk4,
                                      data_ov002_02092448.unk6};
    const u16          table_30[4] = {data_ov002_02092448.unk8, data_ov002_02092448.unkA, data_ov002_02092448.unkC,
                                      data_ov002_02092448.unkE};
    const u16          table_20[4] = {data_ov002_02092448.unk40, data_ov002_02092448.unk42, data_ov002_02092448.unk44,
                                      data_ov002_02092448.unk46};
    const u16          table_28[4] = {data_ov002_02092448.unk48, data_ov002_02092448.unk4A, data_ov002_02092448.unk4C,
                                      data_ov002_02092448.unk4E};
    const u16          table_18[4] = {data_ov002_02092448.unk38, data_ov002_02092448.unk3A, data_ov002_02092448.unk3C,
                                      data_ov002_02092448.unk3E};
    const u16          table_8[4]  = {data_ov002_02092448.unk28, data_ov002_02092448.unk2A, data_ov002_02092448.unk2C,
                                      data_ov002_02092448.unk2E};
    const u16          table_10[4] = {data_ov002_02092448.unk30, data_ov002_02092448.unk32, data_ov002_02092448.unk34,
                                      data_ov002_02092448.unk36};
    const u16          table_78[6] = {data_ov002_02092448.unk50, data_ov002_02092448.unk52, data_ov002_02092448.unk54,
                                      data_ov002_02092448.unk56, data_ov002_02092448.unk58, data_ov002_02092448.unk5A};
    u16                table_64[10];
    u16                table_50[10];
    const Ov002_U16_2* src;
    s32                temp_hi;
    s32                var_r0;
    s32                var_r2;
    s32                var_r4;
    u16                temp_r0_4;
    u16                temp_r2_3;
    u16                temp_r3_2;
    u16                var_r0_5;
    u16                var_r1;
    u16                var_r9;
    u32                temp_r5_3;
    u8                 temp_r0_3;
    OVMGR_S32(arg1, 0xC)  = (s32)OVMGR_U32(arg2, 0x0);
    OVMGR_S32(arg1, 0x10) = (s32)OVMGR_U32(arg2, 0x4);
    OVMGR_S32(arg1, 0x14) = (s32)OVMGR_U32(arg2, 0x8);

    anim.bits_0_1 = 1;
    anim.unk_1C   = 0x1D;
    anim.unk_20   = 0x20;
    anim.unk_22   = 5;
    anim.unk_26   = 0x1E;
    anim.unk_28   = 0x1F;

    anim.unk_04 = 0x80;
    anim.unk_06 = (s16)table_48[OVMGR_U8(arg1, 0x10)] + 0x60;
    anim.unk_02 = (u16)((anim.unk_02 & ~0xC00) | 0x800);
    anim.unk_2A = (OVMGR_S32(arg1, 0xC) != 0) ? 2 : 1;
    if (_Sprite_Load((Sprite*)((u8*)arg1 + 0x20), &anim) == 0) {
        OS_WaitForever();
    }

    anim.unk_04 = 0x80;
    anim.unk_06 = (s16)table_40[OVMGR_U8(arg1, 0x10)] + 0x60;
    anim.unk_02 = (u16)((anim.unk_02 & ~0xC00) | 0x400);
    anim.unk_2A = (OVMGR_S32(arg1, 0xC) != 0) ? 3 : table_38[OVMGR_U8(arg1, 0x11)];
    if (_Sprite_Load((Sprite*)((u8*)arg1 + 0x60), &anim) == 0) {
        OS_WaitForever();
    }

    anim.unk_04 = 0x8C;
    anim.unk_06 = (s16)table_30[OVMGR_U8(arg1, 0x10)] + 0x60;
    anim.unk_02 = (u16)((anim.unk_02 & ~0xC00) | 0x400);
    if (OVMGR_S32(arg1, 0xC) == 0) {
        anim.unk_2A = table_20[OVMGR_U8(arg1, 0x12)];
    } else {
        anim.unk_2A = table_28[OVMGR_U8(arg1, 0x12)];
    }
    if (_Sprite_Load((Sprite*)((u8*)arg1 + 0xA0), &anim) == 0) {
        OS_WaitForever();
    }

    anim.unk_04 = 0x8C;
    anim.unk_06 = (s16)table_18[OVMGR_U8(arg1, 0x10)] + 0x60;
    if (OVMGR_S32(arg1, 0xC) == 0) {
        anim.unk_2A = table_8[OVMGR_U8(arg1, 0x11)];
    } else {
        anim.unk_2A = table_10[OVMGR_U8(arg1, 0x11)];
    }

    var_r0_5                   = OVMGR_U16(arg1, 0x14);
    var_r1                     = 0;
    *((s8*)((u8*)arg1 + 0x1A)) = 0;
    if (var_r0_5 != 0) {
        do {
            temp_r5_3                           = var_r0_5 >> 0x1F;
            *((s8*)((u8*)arg1 + 0x1A + var_r1)) = (s8)(var_r0_5 - (0xA * (temp_r5_3 + (var_r0_5 / 10))));
            var_r0_5                            = temp_r5_3 + (var_r0_5 / 10);
            var_r1 += 1;
        } while (var_r0_5 != 0);
    }
    if (var_r1 == 0) {
        var_r1 = 1;
    }
    OVMGR_U16(arg1, 0x18) = var_r1;
    src                   = data_ov002_020924b8;
    for (var_r4 = 0; var_r4 < 5; var_r4++) {
        table_64[var_r4 * 2]     = src[var_r4].unk0;
        table_64[var_r4 * 2 + 1] = src[var_r4].unk2;
    }
    src = data_ov002_020924a4;
    for (var_r4 = 0; var_r4 < 5; var_r4++) {
        table_50[var_r4 * 2]     = src[var_r4].unk0;
        table_50[var_r4 * 2 + 1] = src[var_r4].unk2;
    }
    var_r9      = 0;
    anim.unk_02 = (u16)((anim.unk_02 & ~0xC00) | 0x400);
    if ((u32)OVMGR_U16(arg1, 0x18) <= 0U) {
        return;
    }
loop_36:
    temp_r0_3   = *((u8*)arg1 + 0x1A + var_r9);
    anim.unk_04 = (s16)table_78[var_r9] + 0x80;
    anim.unk_06 = subroutine_arg0[OVMGR_U8(arg1, 0x10)] + 0x60;
    if (OVMGR_S32(arg1, 0xC) != 0) {
        anim.unk_2A = table_50[temp_r0_3];
    } else {
        anim.unk_2A = table_64[temp_r0_3];
    }
    if (_Sprite_Load((Sprite*)((u8*)arg1 + 0xE0 + (var_r9 << 6)), &anim) == 0) {
        OS_WaitForever();
    }
    temp_r0_4 = var_r9 + 1;
    temp_hi   = (u32)OVMGR_U16(arg1, 0x18) > (u32)temp_r0_4;
    var_r9    = temp_r0_4;
    if (!temp_hi) {
        return;
    }
    goto loop_36;
}

void func_ov002_0208f5d8(void* arg1) {
    u16 temp_r0_2;
    u16 var_r5;
    u32 temp_r0;

    Sprite_Destroy(arg1 + 0x20);
    Sprite_Destroy(arg1 + 0x60);
    Sprite_Destroy(arg1 + 0xA0);
    var_r5 = 0;
    if ((u32)OVMGR_U16(arg1, 0x18) <= 0U) {
        return;
    }
    do {
        Sprite_Destroy(arg1 + 0xE0 + (var_r5 << 6));
        temp_r0_2 = var_r5 + 1;
        temp_r0   = temp_r0_2 << 0x10;
        var_r5    = temp_r0_2;
    } while ((u32)OVMGR_U16(arg1, 0x18) > (u32)(temp_r0 >> 0x10));
}

void func_ov002_0208f630(void* arg1) {
    u16 temp_r0_2;
    u16 var_r5;
    u32 temp_r0;

    Sprite_UpdateAndCheck(arg1 + 0x20);
    Sprite_UpdateAndCheck(arg1 + 0x60);
    Sprite_UpdateAndCheck(arg1 + 0xA0);
    var_r5 = 0;
    if ((u32)OVMGR_U16(arg1, 0x18) <= 0U) {
        return;
    }
    do {
        Sprite_UpdateAndCheck(arg1 + 0xE0 + (var_r5 << 6));
        temp_r0_2 = var_r5 + 1;
        temp_r0   = temp_r0_2 << 0x10;
        var_r5    = temp_r0_2;
    } while ((u32)OVMGR_U16(arg1, 0x18) > (u32)(temp_r0 >> 0x10));
}

void func_ov002_0208f688(void* arg1) {
    u16 temp_r0_2;
    u16 var_r5;
    u32 temp_r0;

    Sprite_Render(arg1 + 0x20);
    Sprite_Render(arg1 + 0x60);
    Sprite_Render(arg1 + 0xA0);
    var_r5 = 0;
    if ((u32)OVMGR_U16(arg1, 0x18) <= 0U) {
        return;
    }
    do {
        Sprite_Render(arg1 + 0xE0 + (var_r5 << 6));
        temp_r0_2 = var_r5 + 1;
        temp_r0   = temp_r0_2 << 0x10;
        var_r5    = temp_r0_2;
    } while ((u32)OVMGR_U16(arg1, 0x18) > (u32)(temp_r0 >> 0x10));
}

s32 func_ov002_0208f6e0(void) {
    func_ov031_0210cdac();
    return 2;
}

s32 func_ov002_0208f6f0(void) {
    return 0;
}

SpriteFrameInfo* func_ov002_0208f6f8(Sprite* arg0, s32 arg1, s32 arg2) {
    (void)arg1;
    switch (arg2) { /* irregular */
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            return Ov002_GetSpriteFrameInfo(arg0);
        default:
            return NULL;
    }
}

void func_ov002_0208f794(void* arg1) {
    SpriteAnimation anim     = data_ov002_020924f8;
    u16*            anim_u16 = (u16*)&anim;

    anim_u16[0] = (u16)((anim_u16[0] & ~3) | 1);
    anim.unk_1C = 0x19;
    anim.unk_20 = 0x1C;
    anim.unk_26 = 0x1A;
    anim.unk_28 = 0x1B;
    anim.unk_2A = 1;
    anim.unk_22 = 1;
    if (_Sprite_Load(arg1, &anim) != 0) {
        return;
    }
    OS_WaitForever();
}

void func_ov002_0208f828(void* arg1) {
    Sprite_Destroy(arg1);
}

void func_ov002_0208f838(void* arg1) {
    Sprite_UpdateAndCheck(arg1);
}

void func_ov002_0208f848(void* arg1) {
    Sprite_Render(arg1);
}

s32 func_ov002_0208f858(void) {
    return 0;
}

void func_ov002_0208f860(void) {}

void func_ov002_0208f864(void* arg1) {
    func_ov002_02085710(arg1);
}

void func_ov002_0208f874(void) {}

void func_ov002_0208f878(void) {}

s32 func_ov002_0208f87c(void* arg0, void* arg1) {
    func_ov002_02083694(arg1);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_0208f89c(void* arg0) {
    if ((TouchInput_WasTouchPressed() != 0) || (SysControl.unk2 & 1) || (SysControl.unk2 & 2) || (SysControl.unk2 & 0x400) ||
        (SysControl.unk2 & 0x800) || (SysControl.unk2 & 0x40) || (SysControl.unk2 & 0x80) || (SysControl.unk2 & 0x20) ||
        (SysControl.unk2 & 0x10))
    {
        func_02026b20(0x347);
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

s32 func_ov002_0208f918(void* arg1) {
    OVMGR_U16(arg1, 0x46074) = 1;
    return 0;
}

s32 func_ov002_0208f92c(void* arg0, s32 arg1) {
    if ((TouchInput_WasTouchPressed() != 0) || (SysControl.unk2 & 1) || (SysControl.unk2 & 2) || (SysControl.unk2 & 0x400) ||
        (SysControl.unk2 & 0x800) || (SysControl.unk2 & 0x40) || (SysControl.unk2 & 0x80) || (SysControl.unk2 & 0x20) ||
        (SysControl.unk2 & 0x10))
    {
        func_02026b20(0x347);
        func_ov031_0210cc84(arg1 + 0x4161C, &data_ov002_02092c58, NULL);
        func_ov031_0210cd48(arg0, &data_ov002_02092b0c);
    }
    return 0;
}

void func_ov002_0208f9d0(void) {}

void func_ov002_0208f9d4(void* arg1) {
    func_ov002_02085710(arg1);
}

void func_ov002_0208f9e4(void) {}

void func_ov002_0208f9e8(void) {}

void func_ov002_0208f9ec(void* arg0, void* arg1, void* arg2) {
    void* temp_r4;

    temp_r4 = OVMGR_PTR(arg0, 0x48058);
    func_ov031_0210b630(temp_r4, data_ov002_02092534[OVMGR_U16(arg0, 0x474CA)]);
    func_ov031_0210ab28(temp_r4, 0U, 0U);
    func_ov031_0210ab3c(temp_r4, 0, 0x100U);
    func_ov031_0210ab48(temp_r4, 0, 0xC0U);
    func_ov031_0210be18(temp_r4, arg2, arg1, 0);
}

void func_ov002_0208fa6c(void* arg0, void* arg1, void* arg2) {
    u16   table_sp14[0xC8];
    u16   table_upper[3];
    u16   table_lower[3];
    s32   temp_r0_2;
    s32   temp_r5;
    s32   temp_r6;
    void* temp_r7;
    u16   i;

    for (i = 0; i < 0xC8; i++) {
        table_sp14[i] = data_ov002_020926e0[i];
    }
    temp_r7        = OVMGR_PTR(arg0, 0x48058);
    table_upper[0] = data_ov002_02092524.unk4;
    table_upper[1] = data_ov002_02092524.unk6;
    table_upper[2] = data_ov002_02092524.unk8;
    table_lower[0] = data_ov002_02092524.unkA;
    table_lower[1] = data_ov002_02092524.unkC;
    table_lower[2] = data_ov002_02092524.unkE;
    temp_r6        = func_ov031_0210b698(temp_r7, 0x20A2U);
    temp_r5        = func_ov031_0210b698(temp_r7, table_upper[OVMGR_U16(arg0, 0x474C8)]);
    temp_r0_2      = func_ov031_0210b698(temp_r7, table_lower[OVMGR_U16(arg0, 0x474CC)]);
    func_ov031_0210a5fc(table_sp14, temp_r6, temp_r5, OVMGR_U16(arg0, 0x474CA) + 1, 0xE, temp_r0_2);
    Mem_Free(&gDebugHeap, temp_r6);
    Mem_Free(&gDebugHeap, temp_r5);
    Mem_Free(&gDebugHeap, temp_r0_2);
    func_ov031_0210b5fc(temp_r7, (s32)table_sp14);
    func_ov031_0210ab28(temp_r7, 0U, 0U);
    func_ov031_0210ab3c(temp_r7, 0, 0x100U);
    func_ov031_0210ab48(temp_r7, 0, 0xC0U);
    func_ov031_0210be18(temp_r7, arg2, arg1, 0);
}

void func_ov002_0208fbf4(void* arg0) {
    u8*   temp_r1;
    u8*   temp_r1_4;
    s32   temp_r3;
    s32   temp_r3_2;
    u16   var_ip;
    u16   var_ip_2;
    void* temp_r0;
    void* temp_r0_2;
    void* temp_r1_2;
    void* temp_r1_3;
    void* temp_r1_5;
    void* temp_r1_6;
    void* temp_r2;
    void* temp_r2_2;
    void* temp_r2_3;
    void* temp_r2_4;
    void* temp_r2_5;
    void* temp_r2_6;
    void* temp_r2_7;
    void* var_r0;
    void* var_r0_2;
    void* var_r4;
    void* var_r4_2;
    void* var_r4_3;
    void* var_r4_4;

    func_ov002_02085710(arg0);
    OVMGR_PTR(arg0, 0x462F0) = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    g_DisplaySettings.unk1C  = 1;
    g_DisplaySettings.unk4C  = 8;
    temp_r2                  = OVMGR_PTR(arg0, 0x462F0);
    temp_r1                  = func_ov031_0210ab68();
    if (temp_r2 == NULL) {
        var_r4 = NULL;
    } else {
        temp_r2_2 = OVPK_HDR(temp_r2);
        var_r4    = temp_r2_2 + OVPK_OFF(temp_r2_2, 0x20);
    }
    var_ip = 0;
    do {
        temp_r3   = var_ip * 2;
        temp_r2_3 = var_r4 + (var_ip * 2);
        var_ip += 1;
        *(u16*)((u8*)temp_r2_3 + 0x120) = *(u16*)(temp_r1 + temp_r3);
    } while ((u32)var_ip < 0x10U);
    Mem_Free(&gDebugHeap, temp_r1);
    DC_PurgeAll();
    OVMGR_PTR(arg0, 0x47340) = func_0200adf8(data_0206b3cc.unk4, var_r4, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, OVMGR_PTR(arg0, 0x47340));
    temp_r0 = OVMGR_PTR(arg0, 0x462F0);
    if (temp_r0 == NULL) {
        var_r4_2 = NULL;
    } else {
        temp_r1_2 = OVPK_HDR(temp_r0);
        var_r4_2  = temp_r1_2 + OVPK_OFF(temp_r1_2, 0x28);
    }
    OVMGR_PTR(arg0, 0x4732C) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r4_2, g_DisplaySettings.unk360, 0, 0x6000);
    temp_r0_2                = OVMGR_PTR(arg0, 0x462F0);
    if (temp_r0_2 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_3 = OVPK_HDR(temp_r0_2);
        var_r0    = temp_r1_3 + OVPK_OFF(temp_r1_3, 0x30);
    }
    OVMGR_PTR(arg0, 0x474A0) = (void*)(var_r0 + 4);
    func_ov002_0208f9ec(arg0, var_r4_2 + 4, OVMGR_PTR(arg0, 0x474A0));
    func_0200d1d8(arg0 + 0x473C0, 1, 3, 0, OVMGR_PTR(arg0, 0x474A0), 1, 1);
    temp_r2_4 = OVMGR_PTR(arg0, 0x462F0);
    temp_r1_4 = func_ov031_0210ab68(0);
    if (temp_r2_4 == NULL) {
        var_r4_3 = NULL;
    } else {
        temp_r2_5 = OVPK_HDR(temp_r2_4);
        var_r4_3  = temp_r2_5 + OVPK_OFF(temp_r2_5, 0x18);
    }
    var_ip_2 = 0;
    do {
        temp_r3_2 = var_ip_2 * 2;
        temp_r2_6 = var_r4_3 + (var_ip_2 * 2);
        var_ip_2 += 1;
        *(u16*)((u8*)temp_r2_6 + 0x120) = *(u16*)(temp_r1_4 + temp_r3_2);
    } while ((u32)var_ip_2 < 0x10U);
    Mem_Free(&gDebugHeap, temp_r1_4);
    DC_PurgeAll();
    OVMGR_PTR(arg0, 0x47344) = func_0200adf8(data_0206b3cc.unk0, var_r4_3, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk0, OVMGR_PTR(arg0, 0x47344));
    temp_r2_7 = OVMGR_PTR(arg0, 0x462F0);
    if (temp_r2_7 == NULL) {
        var_r4_4 = NULL;
    } else {
        temp_r1_5 = OVPK_HDR(temp_r2_7);
        var_r4_4  = temp_r1_5 + OVPK_OFF(temp_r1_5, 0x38);
    }
    if (temp_r2_7 == NULL) {
        var_r0_2 = NULL;
    } else {
        temp_r1_6 = OVPK_HDR(temp_r2_7);
        var_r0_2  = temp_r1_6 + OVPK_OFF(temp_r1_6, 0x40);
    }
    OVMGR_PTR(arg0, 0x474A8) = (void*)(var_r0_2 + 4);
    OVMGR_PTR(arg0, 0x47330) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r4_4, g_DisplaySettings.unk8C, 0, 0x6000);
    func_ov002_0208fa6c(arg0, var_r4_4 + 4, OVMGR_PTR(arg0, 0x474A8));
    func_0200d1d8(arg0 + 0x473E8, 0, 0, 0, OVMGR_PTR(arg0, 0x474A8), 1, 1);
    Display_Commit();
}

void func_ov002_0208ff0c(void* arg0) {
    func_ov002_0208fbf4(arg0);
}

void func_ov002_0208ff18(void* arg0) {
    func_ov031_0210ab34(arg0 + 0x45FF4, subroutine_arg0[OVMGR_U16(arg0, 0x474CC)], data_ov002_02092524.unk0,
                        data_ov002_02092524.unk2, data_ov002_02092524.unk4);
}

void func_ov002_0208ff6c(void* arg0) {
    void* sp0;

    sp0                      = arg0 + 0x45FF4;
    OVMGR_PTR(arg0, 0x48058) = (void*)(&sp0)[OVMGR_U16(arg0, 0x474C8)];
}

s32 func_ov002_0208ffac(void* arg0, void* arg1) {
    void* temp_r0;

    temp_r0                  = arg1 + 0x45FF4;
    OVMGR_PTR(arg1, 0x48058) = temp_r0;
    func_ov031_0210ab34(temp_r0, 0xEU);
    g_DisplaySettings.unk1C = 1;
    g_DisplaySettings.unk4C = 4;
    Display_Commit();
    func_ov002_0208ff6c(arg1);
    func_ov002_0208fbf4(arg1);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02090014(void* arg0, void* arg1) {
    s32 temp_r2;
    s32 var_r0;
    u16 temp_lr;
    u16 temp_r1;
    u16 temp_r1_2;
    u16 temp_r1_3;
    u16 temp_r1_4;
    u16 temp_r1_5;

    var_r0 = 0;
    if (SysControl.unk2 & 1) {
        var_r0                   = 1;
        OVMGR_U16(arg1, 0x474CA) = (u16)(OVMGR_U16(arg1, 0x474CA) + 1);
        temp_lr                  = OVMGR_U16(arg1, 0x474CA);
        temp_r2                  = OVMGR_U16(arg1, 0x474CA) / 7;
        OVMGR_U16(arg1, 0x474CA) = (u16)(temp_lr - (0xE * ((u32)(temp_r2 + ((u32)(temp_lr - temp_r2) >> 1)) >> 3)));
    }
    if (SysControl.unk2 & 2) {
        temp_r1 = OVMGR_U16(arg1, 0x474CA);
        if (temp_r1 == 0) {
            OVMGR_U16(arg1, 0x474CA) = 0xD;
        } else {
            OVMGR_U16(arg1, 0x474CA) = (u16)(temp_r1 - 1);
        }
        var_r0 = 1;
        goto block_7;
    }
block_7:
    if (SysControl.unk2 & 0x20) {
        temp_r1_2 = OVMGR_U16(arg1, 0x474C8);
        if (temp_r1_2 == 0) {
            OVMGR_U16(arg1, 0x474C8) = 0;
        } else {
            OVMGR_U16(arg1, 0x474C8) = (u16)(temp_r1_2 - 1);
        }
        var_r0 = 1;
        goto block_12;
    }
block_12:
    if (SysControl.unk2 & 0x10) {
        temp_r1_3 = OVMGR_U16(arg1, 0x474C8);
        if (temp_r1_3 == 0) {
            OVMGR_U16(arg1, 0x474C8) = 0;
        } else {
            OVMGR_U16(arg1, 0x474C8) = (u16)(temp_r1_3 + 1);
        }
        var_r0 = 1;
        goto block_17;
    }
block_17:
    if (SysControl.unk2 & 0x200) {
        temp_r1_4 = OVMGR_U16(arg1, 0x474CC);
        if (temp_r1_4 == 0) {
            OVMGR_U16(arg1, 0x474CC) = 2;
        } else {
            OVMGR_U16(arg1, 0x474CC) = (u16)(temp_r1_4 - 1);
        }
        var_r0 = 1;
        goto block_22;
    }
block_22:
    if (SysControl.unk2 & 0x100) {
        temp_r1_5 = OVMGR_U16(arg1, 0x474CC);
        if (temp_r1_5 == 2) {
            OVMGR_U16(arg1, 0x474CC) = 0;
        } else {
            OVMGR_U16(arg1, 0x474CC) = (u16)(temp_r1_5 + 1);
        }
        var_r0 = 1;
        goto block_27;
    }
block_27:
    if (var_r0 != 0) {
        func_ov002_0208ff18(arg1);
        func_ov002_0208ff6c(arg1);
        func_ov002_0208ff0c(arg1);
    }
    if (SysControl.unk2 & 8) {
        func_ov031_0210cdac(arg0);
    }
    return 0;
}

SpriteFrameInfo* func_ov002_020901a0(Sprite* arg0, s32 arg1, s32 arg2) {
    (void)arg1;
    switch (arg2) { /* irregular */
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            return Ov002_GetSpriteFrameInfo(arg0);
        default:
            return NULL;
    }
}

void func_ov002_0209023c(void* arg0) {
    SpriteAnimation anim = data_ov002_02092a20;

    _Sprite_Load((Sprite*)((u8*)arg0 + 0x46078), &anim);
    Sprite_ChangeAnimation((Sprite*)((u8*)arg0 + 0x46078), OVMGR_U16(arg0, 0x46090), 3, OVMGR_U16(arg0, 0x46094));
}

void func_ov002_020902a4(void* arg1) {
    func_ov031_0210c94c(arg1 + 0x474E8, &data_ov002_02093474, 0x100);
    func_ov031_0210cc84(arg1 + 0x474E8, &data_ov002_02093498, NULL);
    func_ov031_0210ce50(arg1 + 0x41804, arg1 + 0x474E8);
    Sprite_Destroy(arg1 + 0x46078);
    func_ov002_0209023c(arg1);
}

void func_ov002_02090310(s32 arg1) {
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x476D0);
}

void func_ov002_0209034c(void) {}

void func_ov002_02090350(void) {}

void func_ov002_02090354(void* arg0) {
    u16   var_r1;
    u16   var_r2;
    u16   var_r3;
    void* temp_r0;
    void* temp_r1;
    void* temp_r7;
    void* var_r4;

    if (OVMGR_PTR(arg0, 0x47330) != NULL) {
        BgResMgr_ReleaseChar(g_BgResourceManagers);
        OVMGR_PTR(arg0, 0x47330) = NULL;
    }
    if (OVMGR_PTR(arg0, 0x462F0) != NULL) {
        DatMgr_ReleaseData(OVMGR_PTR(arg0, 0x462F0));
        OVMGR_PTR(arg0, 0x462F0) = NULL;
    }
    temp_r0                  = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    OVMGR_PTR(arg0, 0x462F0) = temp_r0;
    if (temp_r0 == NULL) {
        var_r4 = NULL;
    } else {
        temp_r1 = OVPK_HDR(temp_r0);
        var_r4  = temp_r1 + OVPK_OFF(temp_r1, 0x38);
    }
    var_r3 = 0;
    var_r2 = 0;
    do {
        var_r1 = 0;
    loop_9:
        temp_r7 = (u8*)arg0 + (var_r2 << 7) + (var_r1 * 2);
        var_r1 += 1;
        *(s16*)(temp_r7 + 0x46320) = (s16)(var_r3 | 0x1000);
        var_r3 += 1;
        if ((u32)var_r1 < 0x40U) {
            goto loop_9;
        }
        var_r2 += 1;
    } while ((u32)var_r2 < 0x20U);
    DC_PurgeAll();
    OVMGR_PTR(arg0, 0x474A8) = (void*)((u8*)arg0 + 0x46320);
    OVMGR_PTR(arg0, 0x47330) = BgResMgr_AllocChar32(g_BgResourceManagers, var_r4, g_DisplaySettings.unk8C, 0, 0x6000);
    func_0200d1d8(arg0 + 0x473E8, 0, 0, 0, OVMGR_PTR(arg0, 0x474A8), 1, 1);
    Display_Commit();
}

void func_ov002_020904cc(void* arg0, u16* arg1, u16* arg2) {
    u16   var_ip;
    u16   var_r2;
    u16   var_r3;
    void* temp_r0;
    void* temp_r0_2;
    void* temp_r0_3;
    void* temp_r1;
    void* temp_r1_2;
    void* temp_r1_3;
    void* temp_r1_4;
    void* temp_r1_5;
    void* temp_r1_6;
    void* temp_r1_7;
    void* temp_r2;
    void* temp_r2_2;
    void* temp_r2_3;
    void* temp_r2_4;
    void* temp_r2_5;
    void* temp_r9;
    void* var_r0;
    void* var_r0_2;
    void* var_r1;
    void* var_r1_2;
    void* var_r1_3;
    void* var_r1_4;
    void* var_r1_5;
    void* var_r4;
    void* var_r4_2;

    func_ov002_02085710(arg0);
    OVMGR_PTR(arg0, 0x462F0) = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091aac.binIden, 1, 0);
    OVMGR_PTR(arg0, 0x462F4) = DatMgr_LoadPackEntry(1, 0, 0, &data_ov002_02091c1c, 1, 0);
    OVMGR_PTR(arg0, 0x462F8) = DatMgr_LoadRawData(1, 0, 0, &data_ov002_02091c2c);
    OVMGR_PTR(arg0, 0x462FC) = DatMgr_LoadRawData(1, 0, 0, &data_ov002_02091c24);
    var_r1                   = NULL;
    g_DisplaySettings.unk1C  = 0x19;
    g_DisplaySettings.unk4C  = 0x1C;
    temp_r0                  = OVMGR_PTR(arg0, 0x462F4);
    if (temp_r0 != NULL) {
        temp_r1 = OVPK_HDR(temp_r0);
        var_r1  = temp_r1 + OVPK_OFF(temp_r1, 0x8);
    }
    OVMGR_PTR(arg0, 0x47340) = func_0200adf8(data_0206b3cc.unk4, var_r1, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk4, OVMGR_PTR(arg0, 0x47340));
    temp_r2 = OVMGR_PTR(arg0, 0x462F8);
    if (temp_r2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1_2 = OVPK_HDR(temp_r2);
        var_r1_2  = temp_r1_2 + OVPK_OFF(temp_r1_2, 0x8);
    }
    if (temp_r2 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r2_2 = OVPK_HDR(temp_r2);
        var_r0    = temp_r2_2 + OVPK_OFF(temp_r2_2, 0x10);
    }
    OVMGR_PTR(arg0, 0x474A0) = (void*)(var_r0 + 4);
    OVMGR_PTR(arg0, 0x4732C) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r1_2, g_DisplaySettings.unk360, 0, 0x6000);
    func_0200d1d8(arg0 + 0x473C0, 1, 3, 0, OVMGR_PTR(arg0, 0x474A0), 1, 1);
    temp_r2_3 = OVMGR_PTR(arg0, 0x462F4);
    if (temp_r2_3 == NULL) {
        var_r4 = NULL;
    } else {
        temp_r1_3 = OVPK_HDR(temp_r2_3);
        var_r4    = temp_r1_3 + OVPK_OFF(temp_r1_3, 0x10);
    }
    if (temp_r2_3 == NULL) {
        var_r1_3 = NULL;
    } else {
        temp_r1_4 = OVPK_HDR(temp_r2_3);
        var_r1_3  = temp_r1_4 + OVPK_OFF(temp_r1_4, 0x18);
    }
    OVMGR_PTR(arg0, 0x47498) = (void*)(var_r1_3 + 4);
    func_ov002_02082dbc(arg0 + 0x45FF4, (const Ov002_U16_5*)arg1, var_r4 + 4, OVMGR_PTR(arg0, 0x47498));
    OVMGR_PTR(arg0, 0x47328) = BgResMgr_AllocChar32(g_BgResourceManagers.unk4, var_r4, g_DisplaySettings.unk324, 0, 0x4380);
    func_0200d1d8(arg0 + 0x47398, 1, 2, 0, OVMGR_PTR(arg0, 0x47498), 1, 1);
    temp_r0_2 = OVMGR_PTR(arg0, 0x462F4);
    if (temp_r0_2 == NULL) {
        var_r1_4 = NULL;
    } else {
        temp_r1_5 = OVPK_HDR(temp_r0_2);
        var_r1_4  = temp_r1_5 + OVPK_OFF(temp_r1_5, 0x8);
    }
    OVMGR_PTR(arg0, 0x47344) = func_0200adf8(data_0206b3cc.unk0, var_r1_4, 0, 0, 0x10);
    func_0200bf60(data_0206b3cc.unk0, OVMGR_PTR(arg0, 0x47344));
    temp_r2_4 = OVMGR_PTR(arg0, 0x462FC);
    if (temp_r2_4 == NULL) {
        var_r1_5 = NULL;
    } else {
        temp_r1_6 = OVPK_HDR(temp_r2_4);
        var_r1_5  = temp_r1_6 + OVPK_OFF(temp_r1_6, 0x8);
    }
    if (temp_r2_4 == NULL) {
        var_r0_2 = NULL;
    } else {
        temp_r2_5 = OVPK_HDR(temp_r2_4);
        var_r0_2  = temp_r2_5 + OVPK_OFF(temp_r2_5, 0x10);
    }
    OVMGR_PTR(arg0, 0x474C0) = (void*)(var_r0_2 + 4);
    OVMGR_PTR(arg0, 0x4733C) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r1_5, g_DisplaySettings.unk140, 0, 0x6000);
    func_0200d1d8(arg0 + 0x47460, 0, 3, 0, OVMGR_PTR(arg0, 0x474C0), 1, 1);
    temp_r0_3 = OVMGR_PTR(arg0, 0x462F0);
    if (temp_r0_3 == NULL) {
        var_r4_2 = NULL;
    } else {
        temp_r1_7 = OVPK_HDR(temp_r0_3);
        var_r4_2  = temp_r1_7 + OVPK_OFF(temp_r1_7, 0x38);
    }
    var_ip = 0;
    var_r3 = 0;
    do {
        var_r2 = 0;
    loop_28:
        temp_r9 = (u8*)arg0 + (var_r3 << 7) + (var_r2 * 2);
        var_r2 += 1;
        *(s16*)(temp_r9 + 0x46320) = (s16)(var_ip | 0x1000);
        var_ip += 1;
        if ((u32)var_r2 < 0x40U) {
            goto loop_28;
        }
        var_r3 += 1;
    } while ((u32)var_r3 < 0x20U);
    DC_PurgeAll();
    OVMGR_PTR(arg0, 0x474A8) = (void*)((u8*)arg0 + 0x46320);
    OVMGR_PTR(arg0, 0x47330) = BgResMgr_AllocChar32(g_BgResourceManagers.unk0, var_r4_2, g_DisplaySettings.unk8C, 0, 0x6000);
    func_ov002_02082dbc(arg0 + 0x45FF4, (const Ov002_U16_5*)arg2, var_r4_2 + 4, OVMGR_PTR(arg0, 0x474A8));
    func_0200d1d8(arg0 + 0x473E8, 0, 0, 0, OVMGR_PTR(arg0, 0x474A8), 1, 1);
    Display_Commit();
}

s32 func_ov002_0209095c(void* arg0, void* arg1) {
    u16                spC;
    u16                spA;
    u16                sp8;
    u16                sp6;
    u16                sp4;
    u16                sp2;
    s32                temp_ip;
    s32                var_r2;
    Ov002_U16_2*       var_r3;
    const Ov002_U16_2* var_r4;
    u16                temp_r0;
    u16                temp_r0_2;
    u16                temp_r1;

    var_r4 = (const Ov002_U16_2*)data_ov002_020929e4;
    var_r3 = (Ov002_U16_2*)&spC;
    var_r2 = 7;
    do {
        temp_r1 = var_r4->unk0;
        temp_r0 = var_r4->unk2;
        var_r4 += 4;
        var_r3->unk0 = temp_r1;
        var_r3->unk2 = temp_r0;
        var_r3 += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);
    *var_r3   = *var_r4;
    sp2       = data_ov002_02092870.unk52;
    sp4       = data_ov002_02092870.unk54;
    sp6       = data_ov002_02092870.unk56;
    sp8       = data_ov002_02092870.unk58;
    spA       = data_ov002_02092870.unk5A;
    temp_r0_2 = func_ov002_0208597c(&spC, data_ov002_02092870.unk50);
    if (temp_r0_2 == 0xFFFF) {
        return 0;
    }
    if (temp_r0_2 == 0xFFFE) {
        OVMGR_U16(arg1, 0x474C8) = 0xFFFFU;
        OVMGR_U16(arg1, 0x46084) = 0U;
        OVMGR_U16(arg1, 0x46086) = 0xD2U;
        return 0;
    }
    if (OVMGR_U16(arg1, 0x474C8) == temp_r0_2) {
        func_02026b20(0x11C);
        switch (temp_r0_2) { /* irregular */
            case 0:
                func_ov031_0210cdac(arg0);
                return 0;
            case 1:
                func_ov031_0210cc20(arg0, &data_ov002_020932e8);
                func_ov002_02090354(arg1);
                OVMGR_U16(arg1, 0x46084) = 0U;
                OVMGR_U16(arg1, 0x46086) = 0xD2U;
                func_ov031_0210cd48(arg0, &data_ov002_02092b18);
                return 0;
        }
    } else {
        OVMGR_U16(arg1, 0x474C8) = temp_r0_2;
        func_02026b20(0x11A);
        if ((temp_r0_2 != 0) && (temp_r0_2 != 1)) {
            return 2;
        }
        temp_ip                  = temp_r0_2 * 6;
        OVMGR_U16(arg1, 0x46084) = (u16) * (&sp2 + temp_ip);
        OVMGR_U16(arg1, 0x46086) = (u16) * (&sp4 + temp_ip);
        Sprite_SetAnimation((Sprite*)((u8*)arg1 + 0x46078), OVMGR_U16(arg1, 0x46090), subroutine_arg0[temp_ip],
                            OVMGR_U16(arg1, 0x46094));
        return 0;
    }
}

s32 func_ov002_02090b30(void) {
    func_ov031_0210cdac();
    func_02024d04();
    return 0;
}

s32 func_ov002_02090b44(void* arg0, void* arg1) {
    u16 table_sp1E[0xA];
    u16 table_sub[0xF];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_sp1E[i] = ((const u16*)&data_ov002_02092994)[i];
    }
    for (i = 0; i < 0xF; i++) {
        table_sub[i] = data_ov002_02092a02[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_sp1E, table_sub);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02090c28(void* arg0, void* arg1) {
    u16 table_sp1E[0xA];
    u16 table_sub[0xF];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_sp1E[i] = ((const u16*)&data_ov002_020928e0)[i];
    }
    for (i = 0; i < 0xF; i++) {
        table_sub[i] = data_ov002_020929a8[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_sp1E, table_sub);
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02090d0c(void* arg0, void* arg1) {
    u16 table_sp1E[0xA];
    u16 table_sub[0xF];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_sp1E[i] = ((const u16*)&data_ov002_020928cc)[i];
    }
    for (i = 0; i < 0xF; i++) {
        table_sub[i] = data_ov002_020929c6[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_sp1E, table_sub);
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02090df0(void* arg0, void* arg1) {
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_spA[i] = ((const u16*)&data_ov002_0209296c)[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_spA, subroutine_arg0);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02090ee8(void* arg0, s32 arg1) {
    func_ov031_0210c94c(arg1 + 0x476D0, &data_ov002_02093484, 0x40);
    func_ov031_0210cc84(arg1 + 0x476D0, &data_ov002_020934b0, NULL);
    func_ov031_0210ce50(arg1 + 0x41804, arg1 + 0x476D0);
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02090f4c(void* arg0, s32 arg1) {
    s32 temp_r4;

    if (func_02025444() == 1) {
        temp_r4 = func_020258ac();
        func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x476D0);
        if (temp_r4 == 0) {
            func_02026b20(0x140);
            func_ov031_0210cdac(arg0);
        } else if (temp_r4 & 2) {
            func_ov031_0210cc84(arg0, &data_ov002_02093310);
        } else {
            func_ov031_0210cc84(arg0, &data_ov002_02093324);
        }
    }
    return 0;
}

s32 func_ov002_02090fd8(void* arg0, void* arg1) {
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_spA[i] = ((const u16*)&data_ov002_02092908)[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_spA, subroutine_arg0);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xF0;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020910b0(void* arg0, void* arg1) {
    if (TouchInput_WasTouchPressed() != 0) {
        func_02026b20(0x11C);
    }
    if ((TouchInput_WasTouchPressed() != 0) || (OVMGR_U16(arg1, 0x474C8) == 0)) {
        func_ov031_0210cdac(arg0);
    }
    OVMGR_U16(arg1, 0x474C8) = (u16)(OVMGR_U16(arg1, 0x474C8) - 1);
    return 0;
}

s32 func_ov002_02091110(void* arg0, void* arg1) {
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_spA[i] = ((const u16*)&data_ov002_02092944)[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_spA, subroutine_arg0);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02091208(void* arg0, void* arg1) {
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_spA[i] = ((const u16*)&data_ov002_02092930)[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_spA, subroutine_arg0);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02091300(void* arg0, void* arg1) {
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_spA[i] = ((const u16*)&data_ov002_02092958)[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_spA, subroutine_arg0);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xF0;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020913d8(void* arg0, void* arg1) {
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_spA[i] = ((const u16*)&data_ov002_02092980)[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_spA, subroutine_arg0);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xF0;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_020914b0(void* arg0, void* arg1) {
    if (TouchInput_WasTouchPressed() != 0) {
        func_02026b20(0x11C);
    }
    if ((TouchInput_WasTouchPressed() != 0) || (OVMGR_U16(arg1, 0x474C8) == 0)) {
        func_ov031_0210cdac(arg0);
    }
    OVMGR_U16(arg1, 0x474C8) = (u16)(OVMGR_U16(arg1, 0x474C8) - 1);
    return 0;
}

s32 func_ov002_02091510(void* arg0, void* arg1) {
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_spA[i] = ((const u16*)&data_ov002_0209291c)[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_spA, subroutine_arg0);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02091608(void* arg0, void* arg1) {
    u16 table_spA[0xA];
    u16 i;

    for (i = 0; i < 0xA; i++) {
        table_spA[i] = ((const u16*)&data_ov002_020928f4)[i];
    }
    g_DisplaySettings.unk1C = 0x11;
    g_DisplaySettings.unk4C = 0x14;
    Display_Commit();
    func_ov002_020904cc(arg1, table_spA, subroutine_arg0);
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    OVMGR_U16(arg1, 0x46084) = 0;
    OVMGR_U16(arg1, 0x46086) = 0xD2;
    OVMGR_U16(arg1, 0x474C8) = 0xFFFF;
    func_ov031_0210cdac(arg0);
    return 0;
}

s32 func_ov002_02091700(void) {
    OS_WaitForever();
    return 0;
}

s32 func_ov002_02091710(void) {
    OS_WaitForever();
    return 0;
}

s32 func_ov002_02091720(void* arg1) {
    func_ov031_0210cff0(arg1 + 0x41804, arg1 + 0x474E8);
    func_ov002_02085710(arg1);
    OVMGR_U16(arg1, 0x46074) = 6;
    return 0;
}

SpriteFrameInfo* func_ov002_02091760(Sprite* arg0, s32 arg1, s32 arg2) {
    (void)arg1;
    switch (arg2) { /* irregular */
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            return Ov002_GetSpriteFrameInfo(arg0);
        default:
            return NULL;
    }
}

void func_ov002_020917fc(s32 arg1) {
    SpriteAnimation    anim = data_ov002_02092a4c;
    u16*               var_r3;
    const Ov002_U16_2* var_r4;
    u16                var_r5;
    u16*               temp_r2;
    var_r4 = data_ov002_02092a78;
    var_r3 = &subroutine_arg0[0];
    for (var_r5 = 0; var_r5 < 0x10; var_r5++) {
        var_r3[var_r5 * 2]     = var_r4[var_r5].unk0;
        var_r3[var_r5 * 2 + 1] = var_r4[var_r5].unk2;
    }
    var_r5 = 0;
    do {
        temp_r2     = &subroutine_arg0[var_r5 * 0x10];
        anim.unk_1C = (s16)temp_r2[0];
        anim.unk_20 = (s16)temp_r2[1];
        anim.unk_22 = (s16)temp_r2[2];
        anim.unk_26 = (s16)temp_r2[3];
        anim.unk_28 = (s16)temp_r2[4];
        anim.unk_04 = (s16)(temp_r2[5] + 0x80);
        anim.unk_06 = (s16)(temp_r2[6] + 0x60);
        anim.unk_2A = (s16)temp_r2[7];
        if (_Sprite_Load(arg1 + (var_r5 << 6), &anim) == 0) {
            OS_WaitForever();
        }
        var_r5 += 1;
    } while ((u32)var_r5 < 4U);
}

void func_ov002_020918ec(s32 arg1) {
    u16 var_r4;

    var_r4 = 0;
    do {
        Sprite_Destroy(arg1 + (var_r4 << 6));
        var_r4 += 1;
    } while ((u32)var_r4 < 4U);
}

void func_ov002_02091918(s32 arg1) {
    u16 var_r4;

    var_r4 = 0;
    do {
        Sprite_UpdateAndCheck(arg1 + (var_r4 << 6));
        var_r4 += 1;
    } while ((u32)var_r4 < 4U);
}

void func_ov002_02091944(s32 arg1) {
    u16 var_r4;

    var_r4 = 0;
    do {
        Sprite_Render(arg1 + (var_r4 << 6));
        var_r4 += 1;
    } while ((u32)var_r4 < 4U);
}

s32 func_ov002_02091970(void) {
    return 0;
}

SpriteFrameInfo* func_ov002_02091978(Sprite* arg0, s32 arg1, s32 arg2) {
    (void)arg1;
    switch (arg2) { /* irregular */
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        case 2:
            return Ov002_GetSpriteFrameInfo(arg0);
        default:
            return NULL;
    }
}

void func_ov002_02091a14(void* arg1) {
    SpriteAnimation anim = data_ov002_02092ab8;

    if (_Sprite_Load(arg1, &anim) != 0) {
        return;
    }
    OS_WaitForever();
}

void func_ov002_02091a6c(void* arg1) {
    Sprite_Destroy(arg1);
}

void func_ov002_02091a7c(void* arg1) {
    Sprite_UpdateAndCheck(arg1);
}

void func_ov002_02091a8c(void* arg1) {
    Sprite_Render(arg1);
}

s32 func_ov002_02091a9c(void) {
    return 0;
}
