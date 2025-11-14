#include <NitroSDK/fx/fx_division.h>
#include <types.h>

static const s16 AtanTable[129] = {
    0,    81,   163,  244,  326,  407,  489,  570,  651,  732,  813,  894,  975,  1056, 1136, 1217, 1297, 1377, 1457,
    1537, 1617, 1696, 1775, 1854, 1933, 2012, 2090, 2168, 2246, 2324, 2401, 2478, 2555, 2632, 2708, 2784, 2860, 2935,
    3010, 3085, 3159, 3233, 3307, 3380, 3453, 3526, 3599, 3670, 3742, 3813, 3884, 3955, 4025, 4095, 4164, 4233, 4302,
    4370, 4438, 4505, 4572, 4639, 4705, 4771, 4836, 4901, 4966, 5030, 5094, 5157, 5220, 5282, 5344, 5406, 5467, 5528,
    5589, 5649, 5708, 5768, 5826, 5885, 5943, 6000, 6058, 6114, 6171, 6227, 6282, 6337, 6392, 6446, 6500, 6554, 6607,
    6660, 6712, 6764, 6815, 6867, 6917, 6968, 7018, 7068, 7117, 7166, 7214, 7262, 7310, 7358, 7405, 7451, 7498, 7544,
    7589, 7635, 7679, 7724, 7768, 7812, 7856, 7899, 7942, 7984, 8026, 8068, 8110, 8151, 8192};

u16 FX_Atan2Idx(s32 y, s32 x) {
    s32  a, b, c;
    BOOL positive;

    if (y > 0) {
        if (x > 0) {
            if (x > y) {
                a        = y;
                b        = x;
                c        = 0;
                positive = TRUE;
            } else if (x < y) {
                a        = x;
                b        = y;
                c        = 16384;
                positive = FALSE;
            } else {
                return 8192;
            }
        } else if (x < 0) {
            x = -x;
            if (x < y) {
                a        = x;
                b        = y;
                c        = 16384;
                positive = TRUE;
            } else if (x > y) {
                a        = y;
                b        = x;
                c        = 32768;
                positive = FALSE;
            } else {
                return 24576;
            }
        } else {
            return 16384;
        }
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) {
                a        = y;
                b        = x;
                c        = -32768;
                positive = TRUE;
            } else if (x < y) {
                a        = x;
                b        = y;
                c        = -16384;
                positive = FALSE;
            } else {
                return -24576;
            }
        } else if (x > 0) {
            if (x < y) {
                a        = x;
                b        = y;
                c        = -16384;
                positive = TRUE;
            } else if (x > y) {
                a        = y;
                b        = x;
                c        = 0;
                positive = FALSE;
            } else {
                return -8192;
            }
        } else {
            return -16384;
        }
    } else {
        if (x >= 0) {
            return 0;
        } else {
            return 32768;
        }
    }

    if (b == 0) {
        return 0;
    }

    if (positive) {
        return (c + AtanTable[FX_Divide(a, b) >> 5]);
    } else {
        return (c - AtanTable[FX_Divide(a, b) >> 5]);
    }
}