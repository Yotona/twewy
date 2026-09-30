#ifndef ADXSJD_H
#define ADXSJD_H

#include <cri/cri_xpt.h>
#include <cri/private/adx_b.h>
#include <cri/sj.h>

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
    /* 0x00 */ char used;   // Decoder in use
    /* 0x01 */ char state;
    /* 0x02 */ char maxnch; // Max channels
    /* 0x03 */ char empty_end;
    /* 0x04 */ ADXB adxb;
    /* 0x08 */ SJ   sji;
    /* 0x0C */ SJ   sjo[2];
    /* 0x14 */ SJCK cki;
    /* 0x1C */ SJCK cko[2];
    /* 0x2C */ int  total_decsmpl;
    /* 0x30 */ int  total_decdtlen;
    /* 0x34 */ int  decpos;
    /* 0x38 */ int  maxdecsmpl;
    /* 0x3C */ int  dtrpsmpl; // trap sample count; set to -1
    /* 0x40 */ int  dtrpcnt;
    /* 0x44 */ int  dtrpdtlen;
    /* 0x48 */ void (*dtrpfunc)();
    /* 0x4C */ void* dtrpobj;
    /* 0x50 */ void (*dfltfunc)();
    /* 0x54 */ void* dfltobj;
    /* 0x58 */ void (*unk_58)(); // per-channel decode callback (adxsjd_decexec_end)
    /* 0x5C */ void* unk_5C;     // its user pointer
    /* 0x60 */ char  spsdinfo[0x40];
    /* 0xA0 */ int   hdrlen;
    /* 0xA4 */ int   unk_A4;
    /* 0xA8 */ int   unk_A8;
    /* 0xAC */ int   unk_AC;
    /* 0xB0 */ int   unk_B0;
} ADXSJD; // Size: 0xB4

void ADXSJD_Destroy(ADXSJD* sjd);

char ADXSJD_GetStat(ADXSJD* sjd);

void ADXSJD_SetInSj(ADXSJD* sjd, SJ sj);

#endif // ADXSJD_H
