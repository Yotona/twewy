#ifndef ADX_DCD_H
#define ADX_DCD_H

#include <cri/cri_xpt.h>

typedef struct {
    union {
        unsigned short magic;
        struct {
            unsigned char magic_0;
            unsigned char magic_1;
        };
    };

    union {
        short copyright_offset;
        struct {
            unsigned char copyright_offset_0;
            unsigned char copyright_offset_1;
        };
    };

    char          encoding_type;
    char          block_size;
    char          sample_bitdepth;
    char          channel_count;
    unsigned char sample_rate_0;
    unsigned char sample_rate_1;
    unsigned char sample_rate_2;
    unsigned char sample_rate_3;
    unsigned char total_samples_0;
    unsigned char total_samples_1;
    unsigned char total_samples_2;
    unsigned char total_samples_3;

    union {
        unsigned short high_pass_frequency;
        struct {
            unsigned char high_pass_frequency_0;
            unsigned char high_pass_frequency_1;
        };
    };

    unsigned char  version;
    unsigned char  flags;
    char           pad14[4];
    unsigned short unk18;
    unsigned short unk1A;
    unsigned short unk1C;
    unsigned short unk1E;
} ADXHeader;

#endif // ADX_DCD_H