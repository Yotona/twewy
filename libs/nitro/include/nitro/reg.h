#ifndef _NITRO_REG_H
#define _NITRO_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>

/// MARK: Registers

#define REG_POWER_CNT (*(vu16*)0x04000304)

#define REG_DISPSTAT (*(vu16*)0x04000004)
#define REG_VCOUNT   (*(vu16*)0x04000006)

#define REG_DISP3DCNT  (*(vu16*)0x04000060)
#define REG_DISPCAPCNT (*(vu32*)0x04000064)

#define REG_DMA     ((OSDma*)0x040000b0)
#define REG_DMA0SAD (*(vu32*)0x040000b0)
#define REG_DMA0DAD (*(vu32*)0x040000b4)
#define REG_DMA0CNT (*(vu32*)0x040000b8)

#define REG_KEYINPUT (*(vu16*)0x04000130)

#define REG_EXMEM_CNT (*(vu16*)0x04000204)
#define REG_IME       (*(vu16*)0x04000208)
#define REG_IE        (*(vu32*)0x04000210)
#define REG_IF        (*(vu32*)0x04000214)

#define REG_VRAM_CNT_ABCD (*(vu32*)0x04000240)
#define REG_VRAM_CNT_A    (*(vu8*)0x04000240)
#define REG_VRAM_CNT_B    (*(vu8*)0x04000241)
#define REG_VRAM_CNT_C    (*(vu8*)0x04000242)
#define REG_VRAM_CNT_D    (*(vu8*)0x04000243)
#define REG_VRAM_CNT_E    (*(vu8*)0x04000244)
#define REG_VRAM_CNT_F    (*(vu8*)0x04000245)
#define REG_VRAM_CNT_G    (*(vu8*)0x04000246)
#define REG_WRAM_CNT      (*(vu8*)0x04000247)
#define REG_VRAM_CNT_HI   (*(vu16*)0x04000248)
#define REG_VRAM_CNT_H    (*(vu8*)0x04000248)
#define REG_VRAM_CNT_I    (*(vu8*)0x04000249)

#define REG_DIV_CNT     (*(vu16*)0x04000280)
#define REG_DIV_NUMER   (*(vu64*)0x04000290)
#define REG_DIV_DENOM   (*(vu64*)0x04000298)
#define REG_DIV_RESULT  (*(u64*)0x040002A0)
#define REG_SQRT_CNT    (*(vu16*)0x040002B0)
#define REG_SQRT_RESULT (*(vu32*)0x040002B4)
#define REG_SQRT_PARAM  (*(u64*)0x040002B8)

#define REG_GFX_FIFO                            (*(vu32*)0x04000400)
#define REG_GFX_FIFO_MATRIX_MODE                (*(vu32*)0x04000440)
#define REG_GFX_FIFO_MATRIX_PUSH                (*(vu32*)0x04000444)
#define REG_GFX_FIFO_MATRIX_POP                 (*(vu32*)0x04000448)
#define REG_GFX_FIFO_MATRIX_STORE               (*(vu32*)0x0400044c)
#define REG_GFX_FIFO_MATRIX_RESTORE             (*(vu32*)0x04000450)
#define REG_GFX_FIFO_MATRIX_IDENTITY            (*(vu32*)0x04000454)
#define REG_GFX_FIFO_MATRIX_SCALE               (*(vu32*)0x0400046C)
#define REG_GFX_FIFO_MATRIX_TRANSLATE           (*(vu32*)0x04000470)
#define REG_GFX_FIFO_VERTEX_COLOR               (*(vu32*)0x04000480)
#define REG_GFX_FIFO_VERTEX_TEXCOORD            (*(vu32*)0x04000488)
#define REG_GFX_FIFO_VERTEX_16                  (*(vu32*)0x0400048c)
#define REG_GFX_FIFO_POLYGON_ATTR               (*(vu32*)0x040004a4)
#define REG_GFX_FIFO_TEXTURE_PARAM              (*(vu32*)0x040004a8)
#define REG_GFX_FIFO_TEXTURE_PALETTE            (*(vu32*)0x040004ac)
#define REG_GFX_FIFO_MATERIAL_DIFFUSE_AMBIENT   (*(vu32*)0x040004C0)
#define REG_GFX_FIFO_MATERIAL_SPECULAR_EMISSION (*(vu32*)0x040004C4)
#define REG_GFX_FIFO_LIGHT_DIRECTION            (*(vu32*)0x040004C8)
#define REG_GFX_FIFO_LIGHT_COLOR                (*(vu32*)0x040004CC)
#define REG_GFX_FIFO_POLYGONS_BEGIN             (*(vu32*)0x04000500)
#define REG_GFX_FIFO_POLYGONS_END               (*(vu32*)0x04000504)
#define REG_GFX_FIFO_SWAP_BUFFERS               (*(vu32*)0x04000540)
#define REG_GFX_FIFO_VIEWPORT                   (*(vu32*)0x04000580)
#define REG_GFX_STATUS                          (*(vu32*)0x04000600)

extern u32 __DTCM_LO;
#define DTCM_LO ((u8*)&__DTCM_LO)
#define REG_IRQ (*(u32*)(DTCM_LO + 0x3FF8))

#define _MAIN_REG_BASE 0x04000000
#define _SUB_REG_BASE  0x04001000

#define _REG_DISPCNT(base)       (*(u32*)((base) | 0x0))
#define _REG_BG0CNT(base)        (*(vu16*)((base) | 0x8))
#define _REG_BG1CNT(base)        (*(vu16*)((base) | 0xa))
#define _REG_BG2CNT(base)        (*(vu16*)((base) | 0xc))
#define _REG_BG3CNT(base)        (*(vu16*)((base) | 0xe))
#define _REG_BG0OFS(base)        (*(u32*)((base) | 0x10))
#define _REG_BG1OFS(base)        (*(u32*)((base) | 0x14))
#define _REG_BG2OFS(base)        (*(u32*)((base) | 0x18))
#define _REG_BG3OFS(base)        (*(u32*)((base) | 0x1c))
#define _REG_BG2PA(base)         (*(u16*)((base) | 0x20))
#define _REG_BG2PB(base)         (*(u16*)((base) | 0x22))
#define _REG_BG2PC(base)         (*(u16*)((base) | 0x24))
#define _REG_BG2PD(base)         (*(u16*)((base) | 0x26))
#define _REG_BG2X(base)          (*(u32*)((base) | 0x28))
#define _REG_BG2Y(base)          (*(u32*)((base) | 0x2c))
#define _REG_BG3PA(base)         (*(u16*)((base) | 0x30))
#define _REG_BG3PB(base)         (*(u16*)((base) | 0x32))
#define _REG_BG3PC(base)         (*(u16*)((base) | 0x34))
#define _REG_BG3PD(base)         (*(u16*)((base) | 0x36))
#define _REG_BG3X(base)          (*(u32*)((base) | 0x38))
#define _REG_BG3Y(base)          (*(u32*)((base) | 0x3c))
#define _REG_WIN0H(base)         (*(vu16*)((base) | 0x40))
#define _REG_WIN1H(base)         (*(vu16*)((base) | 0x42))
#define _REG_WIN0V(base)         (*(vu16*)((base) | 0x44))
#define _REG_WIN1V(base)         (*(vu16*)((base) | 0x46))
#define _REG_WININ(base)         (*(u16*)((base) | 0x48))
#define _REG_WINOUT(base)        (*(u16*)((base) | 0x4a))
#define _REG_MOSAIC(base)        (*(u8*)((base) | 0x4c))
#define _REG_OBJMOSAIC(base)     (*(u8*)((base) | 0x4d))
#define _REG_BLDCNT(base)        (*(u16*)((base) | 0x50))
#define _REG_BLDALPHA(base)      (*(u16*)((base) | 0x52))
#define _REG_MASTER_BRIGHT(base) (*(u16*)((base) | 0x6c))

#define REG_DISPCNT       _REG_DISPCNT(_MAIN_REG_BASE)
#define REG_BG0CNT        _REG_BG0CNT(_MAIN_REG_BASE)
#define REG_BG1CNT        _REG_BG1CNT(_MAIN_REG_BASE)
#define REG_BG2CNT        _REG_BG2CNT(_MAIN_REG_BASE)
#define REG_BG3CNT        _REG_BG3CNT(_MAIN_REG_BASE)
#define REG_BG0OFS        _REG_BG0OFS(_MAIN_REG_BASE)
#define REG_BG1OFS        _REG_BG1OFS(_MAIN_REG_BASE)
#define REG_BG2OFS        _REG_BG2OFS(_MAIN_REG_BASE)
#define REG_BG3OFS        _REG_BG3OFS(_MAIN_REG_BASE)
#define REG_BG2PA         _REG_BG2PA(_MAIN_REG_BASE)
#define REG_BG2PB         _REG_BG2PB(_MAIN_REG_BASE)
#define REG_BG2PC         _REG_BG2PC(_MAIN_REG_BASE)
#define REG_BG2PD         _REG_BG2PD(_MAIN_REG_BASE)
#define REG_BG2X          _REG_BG2X(_MAIN_REG_BASE)
#define REG_BG2Y          _REG_BG2Y(_MAIN_REG_BASE)
#define REG_BG3PA         _REG_BG3PA(_MAIN_REG_BASE)
#define REG_BG3PB         _REG_BG3PB(_MAIN_REG_BASE)
#define REG_BG3PC         _REG_BG3PC(_MAIN_REG_BASE)
#define REG_BG3PD         _REG_BG3PD(_MAIN_REG_BASE)
#define REG_BG3X          _REG_BG3X(_MAIN_REG_BASE)
#define REG_BG3Y          _REG_BG3Y(_MAIN_REG_BASE)
#define REG_WIN0H         _REG_WIN0H(_MAIN_REG_BASE)
#define REG_WIN1H         _REG_WIN1H(_MAIN_REG_BASE)
#define REG_WIN0V         _REG_WIN0V(_MAIN_REG_BASE)
#define REG_WIN1V         _REG_WIN1V(_MAIN_REG_BASE)
#define REG_WININ         _REG_WININ(_MAIN_REG_BASE)
#define REG_WINOUT        _REG_WINOUT(_MAIN_REG_BASE)
#define REG_MOSAIC        _REG_MOSAIC(_MAIN_REG_BASE)
#define REG_OBJMOSAIC     _REG_OBJMOSAIC(_MAIN_REG_BASE)
#define REG_BLDCNT        _REG_BLDCNT(_MAIN_REG_BASE)
#define REG_BLDALPHA      _REG_BLDALPHA(_MAIN_REG_BASE)
#define REG_MASTER_BRIGHT _REG_MASTER_BRIGHT(_MAIN_REG_BASE)

#define REG_DISPCNT_SUB       _REG_DISPCNT(_SUB_REG_BASE)
#define REG_BG0CNT_SUB        _REG_BG0CNT(_SUB_REG_BASE)
#define REG_BG1CNT_SUB        _REG_BG1CNT(_SUB_REG_BASE)
#define REG_BG2CNT_SUB        _REG_BG2CNT(_SUB_REG_BASE)
#define REG_BG3CNT_SUB        _REG_BG3CNT(_SUB_REG_BASE)
#define REG_BG0OFS_SUB        _REG_BG0OFS(_SUB_REG_BASE)
#define REG_BG1OFS_SUB        _REG_BG1OFS(_SUB_REG_BASE)
#define REG_BG2OFS_SUB        _REG_BG2OFS(_SUB_REG_BASE)
#define REG_BG3OFS_SUB        _REG_BG3OFS(_SUB_REG_BASE)
#define REG_BG2PA_SUB         _REG_BG2PA(_SUB_REG_BASE)
#define REG_BG2PB_SUB         _REG_BG2PB(_SUB_REG_BASE)
#define REG_BG2PC_SUB         _REG_BG2PC(_SUB_REG_BASE)
#define REG_BG2PD_SUB         _REG_BG2PD(_SUB_REG_BASE)
#define REG_BG2X_SUB          _REG_BG2X(_SUB_REG_BASE)
#define REG_BG2Y_SUB          _REG_BG2Y(_SUB_REG_BASE)
#define REG_BG3PA_SUB         _REG_BG3PA(_SUB_REG_BASE)
#define REG_BG3PB_SUB         _REG_BG3PB(_SUB_REG_BASE)
#define REG_BG3PC_SUB         _REG_BG3PC(_SUB_REG_BASE)
#define REG_BG3PD_SUB         _REG_BG3PD(_SUB_REG_BASE)
#define REG_BG3X_SUB          _REG_BG3X(_SUB_REG_BASE)
#define REG_BG3Y_SUB          _REG_BG3Y(_SUB_REG_BASE)
#define REG_WIN0H_SUB         _REG_WIN0H(_SUB_REG_BASE)
#define REG_WIN1H_SUB         _REG_WIN1H(_SUB_REG_BASE)
#define REG_WIN0V_SUB         _REG_WIN0V(_SUB_REG_BASE)
#define REG_WIN1V_SUB         _REG_WIN1V(_SUB_REG_BASE)
#define REG_WININ_SUB         _REG_WININ(_SUB_REG_BASE)
#define REG_WINOUT_SUB        _REG_WINOUT(_SUB_REG_BASE)
#define REG_MOSAIC_SUB        _REG_MOSAIC(_SUB_REG_BASE)
#define REG_OBJMOSAIC_SUB     _REG_OBJMOSAIC(_SUB_REG_BASE)
#define REG_BLDCNT_SUB        _REG_BLDCNT(_SUB_REG_BASE)
#define REG_BLDALPHA_SUB      _REG_BLDALPHA(_SUB_REG_BASE)
#define REG_MASTER_BRIGHT_SUB _REG_MASTER_BRIGHT(_SUB_REG_BASE)

#define REG_04FFF200 (*(vu32*)0x04fff200)

/// MARK: BIOS

typedef struct {
    /* 0x0 */ u32 offset;
    /* 0x4 */ u32 size;
} CartridgeRegion;

#if NITRO_VERSION >= 0x05057533
    #define _BIOS_REG_BASE 0x02FFF000
#else
    #define _BIOS_REG_BASE 0x027FF000
#endif

#define BIOS_LAUNCHTHREAD_STACK_BOTTOM *(u32*)0x027E0780
#define BIOS_LAUNCHTHREAD_STACK_TOP    *(u32*)0x027E3780

/// Interrupts
// Nonmatching: Every usage seems to be an offset read from dtcm base

#define BIOS_INTERRUPT_IRQSTACK_TOP    *(vu32*)0x027E3780
#define BIOS_INTERRUPT_IRQSTACK_BOTTOM *(vu32*)0x027E3F7C
#define BIOS_INTERRUPT_IRQCHECK        *(vu32*)0x027E3FF8

#define REG_PAD (*(u16*)(_BIOS_REG_BASE | 0xFA8))

#define REG_SYSTEM_RESET        (*(u32*)(_BIOS_REG_BASE | 0xC20))
#define REG_FRAME_COUNTER       (*(u32*)(_BIOS_REG_BASE | 0xC3C))
#define REG_MULTIBOOT_MODE      (*(u16*)(_BIOS_REG_BASE | 0xC40))
#define REG_027FFC42            (*(u16*)(_BIOS_REG_BASE | 0xC42))
#define REG_027FFDE8            (*(u32*)(_BIOS_REG_BASE | 0xDE8))
#define REG_027FFDEA            (*(u16*)(_BIOS_REG_BASE | 0xDEA))
#define REG_027FFDEC            (*(u32*)(_BIOS_REG_BASE | 0xDEC))
#define REG_FNT_ROM_OFFSET      ((_BIOS_REG_BASE | 0xE40))
#define REG_FNT_SIZE            (*(u32*)(_BIOS_REG_BASE | 0xE44))
#define REG_FAT_ROM_OFFSET      ((_BIOS_REG_BASE | 0xE48))
#define REG_FAT_SIZE            (*(u32*)(_BIOS_REG_BASE | 0xE4C))
#define REG_ARM9_OVL_ROM_OFFSET (*(u32*)(_BIOS_REG_BASE | 0xE50))
#define REG_ARM9_OVL_SIZE       (*(u32*)(_BIOS_REG_BASE | 0xE54))
#define REG_ARM7_OVL_ROM_OFFSET (*(u32*)(_BIOS_REG_BASE | 0xE58))
#define REG_ARM7_OVL_SIZE       (*(u32*)(_BIOS_REG_BASE | 0xE5C))
#define REG_027FFF9C            (*(u32*)(_BIOS_REG_BASE | 0xF9C))
#define REG_MAIN_THREAD_INFO    (*(u32*)(_BIOS_REG_BASE | 0xFA0))
#define REG_EXTRA_BUTTONS       (*(vu16*)(_BIOS_REG_BASE | 0xFA8))

/**
 * @brief Checks if the system was booted through DS Download Play via WiFi.
 * @return TRUE if booted through Download Play, FALSE otherwise.
 */
static inline BOOL BIOS_IsDownloadPlay(void) {
    return REG_MULTIBOOT_MODE == 2;
}

static inline const CartridgeRegion* Cart_GetFileNameTable(void) {
    return (const CartridgeRegion*)((const u8*)REG_FNT_ROM_OFFSET);
}

static inline const CartridgeRegion* Cart_GetFileAllocTable(void) {
    return (const CartridgeRegion*)((const u8*)REG_FAT_ROM_OFFSET);
}

static inline const CartridgeRegion* Cart_GetArm9OverlayTable(void) {
    return (const CartridgeRegion*)((const u8*)REG_ARM9_OVL_ROM_OFFSET);
}

static inline const CartridgeRegion* Cart_GetArm7OverlayTable(void) {
    return (const CartridgeRegion*)((const u8*)REG_ARM7_OVL_ROM_OFFSET);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif