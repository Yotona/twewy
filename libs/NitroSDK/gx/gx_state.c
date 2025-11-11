#include <NitroSDK/gx/gx.h>
#include <NitroSDK/gx/gx_state.h>
#include <registers.h>

GXState GxState;

void GX_InitGxState(void) {
    GxState.lcdc          = 0;
    GxState.bg            = 0;
    GxState.obj           = 0;
    GxState.unk_06        = 0;
    GxState.tex           = 0;
    GxState.texPltt       = 0;
    GxState.clearImage    = 0;
    GxState.bgExtPltt     = 0;
    GxState.objExtPltt    = 0;
    GxState.subBg         = 0;
    GxState.subObj        = 0;
    GxState.subBgExtPltt  = 0;
    GxState.subObjExtPltt = 0;

    REG_VRAM_CNT    = 0;
    REG_VRAM_CNT_E  = 0;
    REG_VRAM_CNT_F  = 0;
    REG_VRAM_CNT_G  = 0;
    REG_VRAM_CNT_HI = 0;
}

static void SetLCDC(u32 lcdc) {
    if ((lcdc & GX_VRAM_A) != 0) {
        REG_VRAM_CNT_A = 0x80;
    }
    if ((lcdc & GX_VRAM_B) != 0) {
        REG_VRAM_CNT_B = 0x80;
    }
    if ((lcdc & GX_VRAM_C) != 0) {
        REG_VRAM_CNT_C = 0x80;
    }
    if ((lcdc & GX_VRAM_D) != 0) {
        REG_VRAM_CNT_D = 0x80;
    }
    if ((lcdc & GX_VRAM_E) != 0) {
        REG_VRAM_CNT_E = 0x80;
    }
    if ((lcdc & GX_VRAM_F) != 0) {
        REG_VRAM_CNT_F = 0x80;
    }
    if ((lcdc & GX_VRAM_G) != 0) {
        REG_VRAM_CNT_G = 0x80;
    }
    if ((lcdc & GX_VRAM_H) != 0) {
        REG_VRAM_CNT_H = 0x80;
    }
    if ((lcdc & GX_VRAM_I) != 0) {
        REG_VRAM_CNT_I = 0x80;
    }
}

void GX_SetBankForBg(GXVRam bg) {
    GxState.lcdc = (~bg & (GxState.lcdc | GxState.bg));
    GxState.bg   = bg;

    switch (bg) {
        case GX_VRAM_D:
            REG_VRAM_CNT_D = 129;
            break;

        case GX_VRAM_CD:
            REG_VRAM_CNT_D = 137;
        case GX_VRAM_C:
            REG_VRAM_CNT_C = 129;
            break;

        case GX_VRAM_BCD:
            REG_VRAM_CNT_D = 145;
        case GX_VRAM_BC:
            REG_VRAM_CNT_C = 137;
        case GX_VRAM_B:
            REG_VRAM_CNT_B = 129;
            break;

        case GX_VRAM_ABCD:
            REG_VRAM_CNT_D = 153;
        case GX_VRAM_ABC:
            REG_VRAM_CNT_C = 145;
        case GX_VRAM_AB:
            REG_VRAM_CNT_B = 137;
        case GX_VRAM_A:
            REG_VRAM_CNT_A = 129;
            break;

        case GX_VRAM_ABD:
            REG_VRAM_CNT_A = 129;
            REG_VRAM_CNT_B = 137;
            REG_VRAM_CNT_D = 145;
            break;

        case GX_VRAM_ACD:
            REG_VRAM_CNT_D = 145;
        case GX_VRAM_AC:
            REG_VRAM_CNT_A = 129;
            REG_VRAM_CNT_C = 137;
            break;

        case GX_VRAM_AD:
            REG_VRAM_CNT_A = 129;
            REG_VRAM_CNT_D = 137;
            break;

        case GX_VRAM_BD:
            REG_VRAM_CNT_B = 129;
            REG_VRAM_CNT_D = 137;
            break;

        case GX_VRAM_EFG:
            REG_VRAM_CNT_G = 153;
        case GX_VRAM_EF:
            REG_VRAM_CNT_F = 145;
        case GX_VRAM_E:
            REG_VRAM_CNT_E = 129;
            break;

        case GX_VRAM_EG:
            REG_VRAM_CNT_G = 145;
            REG_VRAM_CNT_E = 129;
            break;

        case GX_VRAM_FG:
            REG_VRAM_CNT_G = 137;
        case GX_VRAM_F:
            REG_VRAM_CNT_F = 129;
            break;

        case GX_VRAM_G:
            REG_VRAM_CNT_G = 129;
            break;

        default:
            break;
    }
    SetLCDC(GxState.lcdc);
}

void GX_SetBankForObj(GXVRam obj) {
    GxState.lcdc = (~obj & (GxState.lcdc | GxState.obj));
    GxState.obj  = obj;

    switch (obj) {
        case GX_VRAM_AB:
            REG_VRAM_CNT_B = 138;
        case GX_VRAM_A:
            REG_VRAM_CNT_A = 130;
        case GX_VRAM_NONE:
            break;

        case GX_VRAM_B:
            REG_VRAM_CNT_B = 130;
            break;

        case GX_VRAM_EFG:
            REG_VRAM_CNT_G = 154;
        case GX_VRAM_EF:
            REG_VRAM_CNT_F = 146;
        case GX_VRAM_E:
            REG_VRAM_CNT_E = 130;
            break;

        case GX_VRAM_EG:
            REG_VRAM_CNT_G = 146;
            REG_VRAM_CNT_E = 130;
            break;

        case GX_VRAM_FG:
            REG_VRAM_CNT_G = 138;
        case GX_VRAM_F:
            REG_VRAM_CNT_F = 130;
            break;

        case GX_VRAM_G:
            REG_VRAM_CNT_G = 130;
            break;

        default:
            break;
    }
    SetLCDC(GxState.lcdc);
}

void GX_SetBankForBgExtPltt(GXVRam bgExtPltt) {
    GxState.lcdc      = (~bgExtPltt & (GxState.lcdc | GxState.bgExtPltt));
    GxState.bgExtPltt = bgExtPltt;

    switch (bgExtPltt) {
        case GX_VRAM_E:
            REG_DISPCNT |= 0x40000000;
            REG_VRAM_CNT_E = 132;
            break;

        case GX_VRAM_G:
            REG_DISPCNT |= 0x40000000;
            REG_VRAM_CNT_G = 140;
            break;

        case GX_VRAM_FG:
            REG_VRAM_CNT_G = 140;
        case GX_VRAM_F:
            REG_VRAM_CNT_F = 132;
            REG_DISPCNT |= 0x40000000;
            break;

        case GX_VRAM_NONE:
            REG_DISPCNT &= ~0x40000000;
            break;

        default:
            break;
    }
    SetLCDC(GxState.lcdc);
}

void GX_SetBankForObjExtPltt(GXVRam objExtPltt) {
    GxState.lcdc       = (~objExtPltt & (GxState.lcdc | GxState.objExtPltt));
    GxState.objExtPltt = objExtPltt;

    switch (objExtPltt) {
        case GX_VRAM_F:
            REG_DISPCNT |= 0x80000000;
            REG_VRAM_CNT_F = 133;
            break;

        case GX_VRAM_G:
            REG_DISPCNT |= 0x80000000;
            REG_VRAM_CNT_G = 133;
            break;

        case GX_VRAM_NONE:
            REG_DISPCNT &= ~0x80000000;
            break;

        default:
            break;
    }
    SetLCDC(GxState.lcdc);
}

void GX_SetBankForTex(GXVRam tex) {
    GxState.lcdc = (~tex & (GxState.lcdc | GxState.tex));
    GxState.tex  = tex;

    if (tex == GX_VRAM_NONE) {
        REG_DISP3DCNT &= (u16) ~(1 | 0x1000 | 0x2000);
    } else {
        REG_DISP3DCNT = (u16)((REG_DISP3DCNT & ~(0x1000 | 0x2000)) | 1);

        switch (tex) {
            case GX_VRAM_AC:
                REG_VRAM_CNT_A = 131;
                REG_VRAM_CNT_C = 139;
                break;

            case GX_VRAM_AD:
                REG_VRAM_CNT_A = 131;
                REG_VRAM_CNT_D = 139;
                break;

            case GX_VRAM_BD:
                REG_VRAM_CNT_B = 131;
                REG_VRAM_CNT_D = 139;
                break;

            case GX_VRAM_ABD:
                REG_VRAM_CNT_A = 131;
                REG_VRAM_CNT_B = 139;
                REG_VRAM_CNT_D = 147;
                break;

            case GX_VRAM_ACD:
                REG_VRAM_CNT_A = 131;
                REG_VRAM_CNT_C = 139;
                REG_VRAM_CNT_D = 147;
                break;

            case GX_VRAM_D:
                REG_VRAM_CNT_D = 131;
                break;

            case GX_VRAM_CD:
                REG_VRAM_CNT_D = 139;
            case GX_VRAM_C:
                REG_VRAM_CNT_C = 131;
                break;

            case GX_VRAM_BCD:
                REG_VRAM_CNT_D = 147;
            case GX_VRAM_BC:
                REG_VRAM_CNT_C = 139;
            case GX_VRAM_B:
                REG_VRAM_CNT_B = 131;
                break;

            case GX_VRAM_ABCD:
                REG_VRAM_CNT_D = 155;
            case GX_VRAM_ABC:
                REG_VRAM_CNT_C = 147;
            case GX_VRAM_AB:
                REG_VRAM_CNT_B = 139;
            case GX_VRAM_A:
                REG_VRAM_CNT_A = 131;
                break;
            default:
                break;
        }
    }
    SetLCDC(GxState.lcdc);
}

void GX_SetBankForTexPltt(GXVRam texPltt) {
    GxState.lcdc    = (~texPltt & (GxState.lcdc | GxState.texPltt));
    GxState.texPltt = texPltt;

    switch (texPltt) {
        case GX_VRAM_FG:
            REG_VRAM_CNT_G = 139;
        case GX_VRAM_F:
            REG_VRAM_CNT_F = 131;
            break;

        case GX_VRAM_G:
            REG_VRAM_CNT_G = 131;
            break;

        case GX_VRAM_EFG:
            REG_VRAM_CNT_G = 155;
        case GX_VRAM_EF:
            REG_VRAM_CNT_F = 147;
        case GX_VRAM_E:
            REG_VRAM_CNT_E = 131;
            break;

        case GX_VRAM_NONE:
        default:
            break;
    }
    SetLCDC(GxState.lcdc);
}

void GX_SetBankForClearImage(GXVRam clearImage) {
    GxState.lcdc       = (~clearImage & (GxState.lcdc | GxState.clearImage));
    GxState.clearImage = clearImage;

    switch (clearImage) {
        case GX_VRAM_AB:
            REG_VRAM_CNT_A = 147;
        case GX_VRAM_B:
            REG_VRAM_CNT_B = 155;
            REG_DISP3DCNT |= 0x4000;
            break;

        case GX_VRAM_CD:
            REG_VRAM_CNT_C = 147;
        case GX_VRAM_D:
            REG_VRAM_CNT_D = 155;
            REG_DISP3DCNT |= 0x4000;
            break;

        case GX_VRAM_NONE:
            REG_DISP3DCNT &= ~0x4000;
            break;

        case GX_VRAM_A:
            REG_VRAM_CNT_A = 155;
            REG_DISP3DCNT |= 0x4000;
            break;

        case GX_VRAM_C:
            REG_VRAM_CNT_C = 155;
            REG_DISP3DCNT |= 0x4000;
            break;

        default:
            break;
    }
    SetLCDC(GxState.lcdc);
}

void GX_SetBankForLcdc(GXVRam lcdc) {
    GxState.lcdc |= lcdc;
    SetLCDC(lcdc);
}

// Nonmatching: Switch table differences
// Scratch: NyRXI
void GX_SetBankForSubBg(GXVRam subBg) {
    GxState.lcdc  = (~subBg & (GxState.lcdc | GxState.subBg));
    GxState.subBg = subBg;

    switch (subBg) {
        case GX_VRAM_C:
            REG_VRAM_CNT_C = 132;
            break;

        case GX_VRAM_HI:
            REG_VRAM_CNT_I = 129;
        case GX_VRAM_H:
            REG_VRAM_CNT_H = 129;
            break;

        case GX_VRAM_NONE:
            break;
    }
    SetLCDC(GxState.lcdc);
}

void GX_SetBankForSubObj(GXVRam subObj) {
    GxState.lcdc   = (~subObj & (GxState.lcdc | GxState.subObj));
    GxState.subObj = subObj;

    switch (subObj) {
        case GX_VRAM_D:
            REG_VRAM_CNT_D = 132;
            break;

        case GX_VRAM_I:
            REG_VRAM_CNT_I = 130;
            break;

        case GX_VRAM_NONE:
            break;

        default:
            break;
    }

    SetLCDC(GxState.lcdc);
}

void GX_SetBankForSubBgExtPltt(GXVRam subBgExtPltt) {
    GxState.lcdc         = (~subBgExtPltt & (GxState.lcdc | GxState.subBgExtPltt));
    GxState.subBgExtPltt = subBgExtPltt;

    switch (subBgExtPltt) {
        case GX_VRAM_H:
            REG_DISPCNT_SUB |= 0x40000000;
            REG_VRAM_CNT_H = 130;
            break;

        case GX_VRAM_NONE:
            REG_DISPCNT_SUB &= ~0x40000000;
            break;

        default:
            break;
    }

    SetLCDC(GxState.lcdc);
}

void GX_SetBankForSubObjExtPltt(GXVRam subObjExtPltt) {
    GxState.lcdc          = (~subObjExtPltt & (GxState.lcdc | GxState.subObjExtPltt));
    GxState.subObjExtPltt = subObjExtPltt;

    switch (subObjExtPltt) {
        case GX_VRAM_I:
            REG_DISPCNT_SUB |= 0x80000000;
            REG_VRAM_CNT_I = 131;
            break;

        case GX_VRAM_NONE:
            REG_DISPCNT_SUB &= ~0x80000000;
            break;
    }

    SetLCDC(GxState.lcdc);
}

static GXVRam ResetBank(u16* bank) {
    GXVRam vram = *bank;

    *bank = 0;
    GxState.lcdc |= vram;
    SetLCDC(vram);
    return vram;
}

GXVRam GX_ResetBankForBgExtPltt(void) {
    REG_DISPCNT &= ~0x40000000;
    return ResetBank(&GxState.bgExtPltt);
}

GXVRam GX_ResetBankForObjExtPltt(void) {
    REG_DISPCNT &= ~0x80000000;
    return ResetBank(&GxState.objExtPltt);
}

GXVRam GX_ResetBankForTex(void) {
    return ResetBank(&GxState.tex);
}

GXVRam GX_ResetBankForTexPltt(void) {
    return ResetBank(&GxState.texPltt);
}

GXVRam GX_ResetBankForClearImage(void) {
    return ResetBank(&GxState.clearImage);
}

GXVRam GX_ResetBankForSubBg(void) {
    return ResetBank(&GxState.subBg);
}

GXVRam GX_ResetBankForSubObj(void) {
    return ResetBank(&GxState.subObj);
}

GXVRam GX_ResetBankForSubBgExtPltt(void) {
    REG_DISPCNT_SUB &= ~0x40000000;
    return ResetBank(&GxState.subBgExtPltt);
}

GXVRam GX_ResetBankForSubObjExtPltt(void) {
    REG_DISPCNT_SUB &= ~0x80000000;
    return ResetBank(&GxState.subObjExtPltt);
}

static GXVRam DisableBank(u16* bank) {
    GXVRam vram = *bank;
    *bank       = 0;

    if (vram & GX_VRAM_A) {
        REG_VRAM_CNT_A = 0;
    }
    if (vram & GX_VRAM_B) {
        REG_VRAM_CNT_B = 0;
    }
    if (vram & GX_VRAM_C) {
        REG_VRAM_CNT_C = 0;
    }
    if (vram & GX_VRAM_D) {
        REG_VRAM_CNT_D = 0;
    }
    if (vram & GX_VRAM_E) {
        REG_VRAM_CNT_E = 0;
    }
    if (vram & GX_VRAM_F) {
        REG_VRAM_CNT_F = 0;
    }
    if (vram & GX_VRAM_G) {
        REG_VRAM_CNT_G = 0;
    }
    if (vram & GX_VRAM_H) {
        REG_VRAM_CNT_H = 0;
    }
    if (vram & GX_VRAM_I) {
        REG_VRAM_CNT_I = 0;
    }
    func_0203aa48((u16)vram, GX_LockID);
    return vram;
}

GXVRam GX_DisableBankForBg(void) {
    return DisableBank(&GxState.bg);
}

GXVRam GX_DisableBankForObj(void) {
    return DisableBank(&GxState.obj);
}

GXVRam GX_DisableBankForObjExtPltt(void) {
    REG_DISPCNT &= ~0x80000000;
    return DisableBank(&GxState.objExtPltt);
}

GXVRam GX_DisableBankForTex(void) {
    return DisableBank(&GxState.tex);
}

GXVRam GX_DisableBankForTexPltt(void) {
    return DisableBank(&GxState.texPltt);
}

GXVRam GX_DisableBankForLcdc(void) {
    return DisableBank(&GxState.lcdc);
}

GXVRam GX_GetBankForBg(void) {
    return GxState.bg;
}

GXVRam GX_GetBankForObj(void) {
    return GxState.obj;
}

GXVRam GX_GetBankForBgExtPltt(void) {
    return GxState.bgExtPltt;
}

GXVRam GX_GetBankForObjExtPltt(void) {
    return GxState.objExtPltt;
}

GXVRam GX_GetBankForTex(void) {
    return GxState.tex;
}

GXVRam GX_GetBankForSubBg(void) {
    return GxState.subBg;
}

GXVRam GX_GetBankForSubObj(void) {
    return GxState.subObj;
}

GXVRam GX_GetBankForSubBgExtPltt(void) {
    return GxState.subBgExtPltt;
}

GXVRam GX_GetBankForSubObjExtPltt(void) {
    return GxState.subObjExtPltt;
}

static u32 GetBankSize(u32 vram) {
    u32 size = 0;

    if (vram & GX_VRAM_A) {
        size += 0x20000;
    }
    if (vram & GX_VRAM_B) {
        size += 0x20000;
    }
    if (vram & GX_VRAM_C) {
        size += 0x20000;
    }
    if (vram & GX_VRAM_D) {
        size += 0x20000;
    }
    if (vram & GX_VRAM_E) {
        size += 0x10000;
    }
    if (vram & GX_VRAM_F) {
        size += 0x4000;
    }
    if (vram & GX_VRAM_G) {
        size += 0x4000;
    }
    if (vram & GX_VRAM_H) {
        size += 0x8000;
    }
    if (vram & GX_VRAM_I) {
        size += 0x4000;
    }

    return size;
}

u32 GX_GetSizeOfBg(void) {
    return GetBankSize(GxState.bg);
}

u32 GX_GetSizeOfSubBg(void) {
    return GetBankSize(GxState.subBg);
}