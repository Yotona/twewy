#ifndef ADX_DCD_H
#define ADX_DCD_H

#include <types.h>

typedef struct {
    union {
        u16 magic;
        struct {
            u8 magic_0;
            u8 magic_1;
        };
    };

    union {
        s16 copyright_offset;
        struct {
            u8 copyright_offset_0;
            u8 copyright_offset_1;
        };
    };

    s8 encoding_type;
    s8 block_size;
    s8 sample_bitdepth;
    s8 channel_count;
    u8 sample_rate_0;
    u8 sample_rate_1;
    u8 sample_rate_2;
    u8 sample_rate_3;
    u8 total_samples_0;
    u8 total_samples_1;
    u8 total_samples_2;
    u8 total_samples_3;

    union {
        u16 high_pass_frequency;
        struct {
            u8 high_pass_frequency_0;
            u8 high_pass_frequency_1;
        };
    };

    u8   version;
    u8   flags;
    char pad14[4];
    u16  unk18;
    u16  unk1A;
    u16  unk1C;
    u16  unk1E;
} ADXHeader;

#endif // ADX_DCD_H