#ifndef MSL_LOCALE_API_H
#define MSL_LOCALE_API_H

#include <types.h>

#define _COMPONENT_NAME_LEN 8

typedef s32 (*__decode_mbyte)(u16*, const char*, s32);
typedef s32 (*__encode_mbyte)(char*, u16);

struct _loc_coll_cmpt {
    s32  char_start_value;
    s32  char_coll_tab_size;
    s16  char_spec_accents;
    u16* char_coll_table_ptr;
};

struct _loc_ctype_cmpt {
    /* 0x00 */ __decode_mbyte decode_mb;
    /* 0x04 */ __encode_mbyte encode_wc;
};

struct _loc_time_cmpt {
    char* am_pm;
    char* DateTime_Format;
    char* Twelve_hr_format;
    char* Date_Format;
    char* Time_Format;
    char* Day_Names;
    char* MonthNames;
    char* TimeZone;
};

struct __locale {
    /* 0x00 */ struct _loc_time_cmpt*  time_cmpt_ptr;
    /* 0x04 */ struct _loc_coll_cmpt*  coll_cmpt_ptr;
    /* 0x08 */ struct _loc_ctype_cmpt* ctype_cmpt_ptr;
};

extern struct __locale _current_locale;

#endif // MSL_COMMON_LOCALE_API_H