#ifndef FIELD_SCANEVENT_H
#define FIELD_SCANEVENT_H

#include <nitro/types.h>

// Shared scan-event table in arm9; 67 records, each 0xE bytes.
typedef struct {
    /* 0x0 */ s16 firstChapter;
    /* 0x2 */ s16 lastChapter;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
    /* 0x8 */ s16 area;
    /* 0xA */ union {
        s16 encounter;
        u16 encounterId; // retail uses unsigned loads when copying the ID
    };
    /* 0xC */ u16 flags;
} FieldScanEvent;

extern FieldScanEvent data_0205c5be[67];

#endif
