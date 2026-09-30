#ifndef ADXSJD_H
#define ADXSJD_H

#include <CriWare/private/adx_b.h>
#include <CriWare/sj.h>
#include <nitro/types.h>

/**
 * Number of ADXSJD objects in the `adxsjd_obj` pool.
 *
 * Verified from the original module: `delinks.txt` gives `adx_sjd.c` a bss
 * range of 0x0206c0b4-0x0206c398, and `adxsjd_obj` starts at 0x0206c0c8.
 * 0x0206c398 - 0x0206c0c8 = 0x2D0 = 4 * sizeof(ADXSJD) (0xB4), so the pool is
 * exactly 4 entries deep.
 */
#define ADXSJD_MAX_OBJ 4

/**
 * @brief Stream Joint Decoder
 */
typedef struct {
    /* 0x00 */ s8   used;   // Decoder in use
    /* 0x01 */ s8   state;
    /* 0x02 */ s8   maxnch; // Max channels
    /* 0x03 */ s8   unk_03;
    /* 0x04 */ ADXB adxb;
    /* 0x08 */ SJ   sji;
    /* 0x0C */ SJ   sjo[2];
    /* 0x14 */ char unk_14[0x18];
    /* 0x2C */ s32  unk_2C;      // total_decsmpl
    /* 0x30 */ s32  unk_30;      // total_decdtlen
    /* 0x34 */ s32  unk_34;      // decpos
    /* 0x38 */ s32  unk_38;      // maxdecsmpl
    /* 0x3C */ s32  unk_3C;      // dtrpsmpl  (trap sample count; set to -1)
    /* 0x40 */ s32  unk_40;      // dtrpcnt
    /* 0x44 */ s32  unk_44;      // dtrpdtlen
    /* 0x48 */ void (*unk_48)(); // dtrpfunc -- ADXSJD trap callback
    /* 0x4C */ void* unk_4C;     // dtrpobj  -- its user pointer
    /* 0x50 */ void (*unk_50)(); // dfltfunc -- filter callback
    /* 0x54 */ void* unk_54;     // dfltobj
    /* 0x58 */ void (*unk_58)(); // per-channel decode callback (adxsjd_decexec_end)
    /* 0x5C */ void* unk_5C;     // its user pointer
    /* 0x60 */ s32   unk_60;
    /* 0x64 */ char  unk_64[0x3C];
    /* 0xA0 */ s32   unk_A0;
    /* 0xA4 */ s32   unk_A4;
    /* 0xA8 */ s32   unk_A8;
    /* 0xAC */ s32   unk_AC;
    /* 0xB0 */ s32   unk_B0;
} ADXSJD; // Size: 0xB4

void ADXSJD_Destroy(ADXSJD* sjd);

s8 ADXSJD_GetStat(ADXSJD* sjd);

void ADXSJD_SetInSj(ADXSJD* sjd, SJ sj);

#endif // ADXSJD_H
