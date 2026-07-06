#include "Player/Progress.h"
#include "Save.h"
#include <nitro/types.h>

extern s32 data_ov030_020e2364;
extern s32 data_ov030_020e2380;
extern s32 data_ov030_020e239c;
extern s32 data_ov030_020e23b8;
extern s32 data_ov030_020e23d4;
extern s32 data_ov030_020e23f0;
extern s32 data_ov030_020e240c;
extern s32 data_ov030_020e2428;
extern s32 data_ov030_020e2444;
extern s32 data_ov030_020e2460;
extern s32 data_ov030_020e247c;
extern s32 data_ov030_020e2498;
extern s32 data_ov030_020e24b4;
extern s32 data_ov030_020e24d0;
extern s32 data_ov030_020e24ec;
extern s32 data_ov030_020e2508;
extern s32 data_ov030_020e2524;
extern s32 data_ov030_020e2540;
extern s32 data_ov030_020e255c;
extern s32 data_ov030_020e2578;
extern s32 data_ov030_020e2594;
extern s32 data_ov030_020e25b0;
extern s32 data_ov030_020e25cc;
extern s32 data_ov030_020e25e8;
extern s32 data_ov030_020e2604;
extern s32 data_ov030_020e2620;
extern s32 data_ov030_020e263c;
extern s32 data_ov030_020e2658;
extern s32 data_ov030_020e2674;
extern s32 data_ov030_020e2690;
extern s32 data_ov030_020e26ac;
extern s32 data_ov030_020e26c8;
extern s32 data_ov030_020e26e4;
extern s32 data_ov030_020e2700;
extern s32 data_ov030_020e271c;
extern s32 data_ov030_020e2738;
extern s32 data_ov030_020e2754;
extern s32 data_ov030_020e2770;
extern s32 data_ov030_020e278c;
extern s32 data_ov030_020e27a8;
extern s32 data_ov030_020e27c4;
extern s32 data_ov030_020e27e0;
extern s32 data_ov030_020e27fc;
extern s32 data_ov030_020e2818;
extern s32 data_ov030_020e2834;
extern s32 data_ov030_020e2850;
extern s32 data_ov030_020e286c;
extern s32 data_ov030_020e2888;
extern s32 data_ov030_020e28a4;
extern s32 data_ov030_020e28c0;
extern s32 data_ov030_020e28dc;
extern s32 data_ov030_020e28f8;
extern s32 data_ov030_020e2914;
extern s32 data_ov030_020e2930;
extern s32 data_ov030_020e294c;
extern s32 data_ov030_020e2968;
extern s32 data_ov030_020e2984;
extern s32 data_ov030_020e29a0;
extern s32 data_ov030_020e29bc;
extern s32 data_ov030_020e29d8;
extern s32 data_ov030_020e29f4;
extern s32 data_ov030_020e2a10;
extern s32 data_ov030_020e2a2c;
extern s32 data_ov030_020e2a48;
extern s32 data_ov030_020e2a64;
extern s32 data_ov030_020e2a80;
extern s32 data_ov030_020e2a9c;
extern s32 data_ov030_020e2ab8;
extern s32 data_ov030_020e2ad4;
extern s32 data_ov030_020e2af0;
extern s32 data_ov030_020e2b0c;
extern s32 data_ov030_020e2b28;
extern s32 data_ov030_020e2b44;
extern s32 data_ov030_020e2b60;
extern s32 data_ov030_020e2b7c;
extern s32 data_ov030_020e2b98;
extern s32 data_ov030_020e2bb4;
extern s32 data_ov030_020e2bd0;
extern s32 data_ov030_020e2bec;
extern s32 data_ov030_020e2c08;
extern s32 data_ov030_020e2c24;
extern s32 data_ov030_020e2c40;
extern s32 data_ov030_020e2c5c;
extern s32 data_ov030_020e2c78;
extern s32 data_ov030_020e2c94;
extern s32 data_ov030_020e2cb0;
extern s32 data_ov030_020e2ccc;
extern s32 data_ov030_020e2ce8;
extern s32 data_ov030_020e2d04;
extern s32 data_ov030_020e2d20;
extern s32 data_ov030_020e2d3c;
extern s32 data_ov030_020e2d58;
extern s32 data_ov030_020e2d74;
extern s32 data_ov030_020e2d90;
extern s32 data_ov030_020e2dac;
extern s32 data_ov030_020e2dc8;
extern s32 data_ov030_020e2de4;
extern s32 data_ov030_020e2e00;
extern s32 data_ov030_020e2e1c;
extern s32 data_ov030_020e2e38;
extern s32 data_ov030_020e2e54;
extern s32 data_ov030_020e2e70;
extern s32 data_ov030_020e2ea8;
extern s32 data_ov030_020e2ee0;
extern s32 data_ov030_020e2f18;
extern s32 data_ov030_020e2f50;
extern s32 data_ov030_020e2f88;
extern s32 data_ov030_020e2fc0;
extern s32 data_ov030_020e2ff8;
extern s32 data_ov030_020e3030;
extern s32 data_ov030_020e3068;
extern s32 data_ov030_020e30a0;
extern s32 data_ov030_020e30d8;
extern s32 data_ov030_020e3110;
extern s32 data_ov030_020e3148;
extern s32 data_ov030_020e3180;
extern s32 data_ov030_020e31b8;
extern s32 data_ov030_020e31f0;
extern s32 data_ov030_020e3228;
extern s32 data_ov030_020e3260;
extern s32 data_ov030_020e3298;
extern s32 data_ov030_020e32d0;
extern s32 data_ov030_020e3308;
extern s32 data_ov030_020e3340;
extern s32 data_ov030_020e3378;
extern s32 data_ov030_020e33b0;
extern s32 data_ov030_020e33e8;
extern s32 data_ov030_020e3420;
extern s32 data_ov030_020e3458;
extern s32 data_ov030_020e34ac;
extern s32 data_ov030_020e3500;
extern s32 data_ov030_020e3554;
extern s32 data_ov030_020e35a8;
extern s32 data_ov030_020e35fc;
extern s32 data_ov030_020e3650;
extern s32 data_ov030_020e36a4;
extern s32 data_ov030_020e36f8;
extern s32 data_ov030_020e374c;
extern s32 data_ov030_020e37a0;
extern s32 data_ov030_020e37f4;
extern s32 data_ov030_020e3848;
extern s32 data_ov030_020e389c;
extern s32 data_ov030_020e38f0;
extern s32 data_ov030_020e3944;
extern s32 data_ov030_020e3998;
extern s32 data_ov030_020e39ec;
extern s32 data_ov030_020e3a40;
extern s32 data_ov030_020e3a94;
extern s32 data_ov030_020e3ae8;
extern s32 data_ov030_020e3b3c;
extern s32 data_ov030_020e3b90;
extern s32 data_ov030_020e3be4;
extern s32 data_ov030_020e3c38;
extern s32 data_ov030_020e3c8c;
extern s32 data_ov030_020e3ce0;
extern s32 data_ov030_020e3d34;
extern s32 data_ov030_020e3da4;
extern s32 data_ov030_020e3e14;
extern s32 data_ov030_020e3e84;
extern s32 data_ov030_020e3ef4;
extern s32 data_ov030_020e3f64;
extern s32 data_ov030_020e3fd4;
extern s32 data_ov030_020e4044;
extern s32 data_ov030_020e40b4;
extern s32 data_ov030_020e4124;
extern s32 data_ov030_020e4194;
extern s32 data_ov030_020e4204;
extern s32 data_ov030_020e4290;
extern s32 data_ov030_020e431c;
extern s32 data_ov030_020e43a8;
extern s32 data_ov030_020e4434;
extern s32 data_ov030_020e44c0;
extern s32 data_ov030_020e454c;
extern s32 data_ov030_020e45d8;
extern s32 data_ov030_020e4664;
extern s32 data_ov030_020e46f0;
extern s32 data_ov030_020e477c;
extern s32 data_ov030_020e4808;
extern s32 data_ov030_020e4894;
extern s32 data_ov030_020e4920;
extern s32 data_ov030_020e49ac;
extern s32 data_ov030_020e4a38;
extern s32 data_ov030_020e4ac4;
extern s32 data_ov030_020e4b50;
extern s32 data_ov030_020e4bf8;
extern s32 data_ov030_020e4ca0;
extern s32 data_ov030_020e4d48;
extern s32 data_ov030_020e4df0;
extern s32 data_ov030_020e4f48;
extern s32 data_ov030_020e500c;
extern s32 data_ov030_020e50d0;
extern s32 data_ov030_020e51b0;
extern s32 data_ov030_020e5290;
extern s32 data_ov030_020e5370;
extern s32 data_ov030_020e5450;
extern s32 data_ov030_020e554c;
extern s32 data_ov030_020e5648;
extern s32 data_ov030_020e5760;
extern s32 data_ov030_020e5878;
extern s32 data_ov030_020e59ac;
extern s32 data_ov030_020e5b34;
extern s32 data_ov030_020e5cd8;

static s32* func_ov030_020b891c(s32* arg0) {
    *arg0 = 0;
    return NULL;
}

static s32* func_ov030_020b892c(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e2888;
    } else if (gSaveData.unk_2458 == 5) {
        *arg0 = 2;
        return &data_ov030_020e3420;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b897c(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 2;
        return &data_ov030_020e3308;
    } else if (gSaveData.unk_2458 == 17) {
        *arg0 = 6;
        return &data_ov030_020e4bf8;
    } else if (gSaveData.unk_2458 == 5) {
        *arg0 = 3;
        return &data_ov030_020e389c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b89e8(s32* arg0) {
    if (gSaveData.unk_2458 == 19) {
        *arg0 = 5;
        return &data_ov030_020e4664;
    } else if (gSaveData.unk_2458 == 21) {
        *arg0 = 11;
        return &data_ov030_020e5878;
    } else if (gSaveData.unk_2458 == 22) {
        *arg0 = 5;
        return &data_ov030_020e46f0;
    } else if (gSaveData.unk_2458 == 23) {
        *arg0 = 5;
        return &data_ov030_020e477c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8a70(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e26e4;
    } else if (gSaveData.unk_2458 == 3) {
        *arg0 = 2;
        return &data_ov030_020e2ff8;
    } else if (gSaveData.unk_2458 == 11) {
        *arg0 = 6;
        return &data_ov030_020e4ca0;
    } else if (gSaveData.unk_2458 == 12) {
        *arg0 = 2;
        return &data_ov030_020e3030;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8af8(s32* arg0) {
    if (gSaveData.unk_2458 == 2) {
        *arg0 = 1;
        return &data_ov030_020e2834;
    } else if (gSaveData.unk_2458 == 6) {
        *arg0 = 2;
        return &data_ov030_020e3068;
    } else if (gSaveData.unk_2458 == 0) {
        *arg0 = 9;
        return &data_ov030_020e5450;
    } else if (gSaveData.unk_2458 == 8) {
        *arg0 = 4;
        return &data_ov030_020e3ef4;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8b80(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 15;
        return &data_ov030_020e5cd8;
    } else if (gSaveData.unk_2458 == 3) {
        *arg0 = 4;
        return &data_ov030_020e3f64;
    } else if (gSaveData.unk_2458 == 4) {
        *arg0 = 3;
        return &data_ov030_020e39ec;
    } else if (gSaveData.unk_2458 == 0) {
        *arg0 = 2;
        return &data_ov030_020e30d8;
    } else if (gSaveData.unk_2458 == 8) {
        *arg0 = 1;
        return &data_ov030_020e2ad4;
    } else if (gSaveData.unk_2458 == 12) {
        *arg0 = 1;
        return &data_ov030_020e2bec;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8c40(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 3;
        return &data_ov030_020e3a94;
    } else if (gSaveData.unk_2458 == 5) {
        *arg0 = 1;
        return &data_ov030_020e2e00;
    } else if (gSaveData.unk_2458 == 8) {
        *arg0 = 1;
        return &data_ov030_020e27e0;
    } else if (gSaveData.unk_2458 == 17) {
        *arg0 = 1;
        return &data_ov030_020e2d04;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8cc8(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e2460;
    } else if (gSaveData.unk_2458 == 3) {
        *arg0 = 1;
        return &data_ov030_020e2738;
    } else if (gSaveData.unk_2458 == 5) {
        *arg0 = 3;
        return &data_ov030_020e3ae8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8d34(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 2;
        return &data_ov030_020e31b8;
    } else if (gSaveData.unk_2458 == 4) {
        *arg0 = 1;
        return &data_ov030_020e2e38;
    } else if (gSaveData.unk_2458 == 8) {
        *arg0 = 2;
        return &data_ov030_020e31f0;
    } else if (gSaveData.unk_2458 == 11) {
        *arg0 = 6;
        return &data_ov030_020e4d48;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8dbc(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 2;
        return &data_ov030_020e3228;
    } else if ((gSaveData.unk_2458 == 4) && (Progress_Check(9) != 0) && (Progress_Check(11) == 0)) {
        *arg0 = 2;
        return &data_ov030_020e3260;
    } else if (gSaveData.unk_2458 == 6) {
        *arg0 = 3;
        return &data_ov030_020e3b3c;
    } else if ((gSaveData.unk_2458 == 0) && (Progress_Check(9) != 0)) {
        *arg0 = 2;
        return &data_ov030_020e32d0;
    } else if (gSaveData.unk_2458 == 8) {
        *arg0 = 5;
        return &data_ov030_020e4808;
    } else if ((gSaveData.unk_2458 == 12) && (Progress_Check(12) == 0) && (Progress_Check(73) == 0)) {
        *arg0 = 3;
        return &data_ov030_020e3b90;
    } else if (gSaveData.unk_2458 == 15) {
        *arg0 = 1;
        return &data_ov030_020e2a2c;
    } else if (gSaveData.unk_2458 == 19) {
        *arg0 = 7;
        return &data_ov030_020e500c;
    } else if (gSaveData.unk_2458 == 21) {
        *arg0 = 5;
        return &data_ov030_020e4894;
    } else if ((gSaveData.unk_2458 == 22) && (Progress_Check(12) != 0)) {
        *arg0 = 4;
        return &data_ov030_020e3fd4;
    } else if ((gSaveData.unk_2458 == 0x17) && (Progress_Check(12) != 0)) {
        *arg0 = 2;
        return &data_ov030_020e3340;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8fa0(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 3;
        return &data_ov030_020e3be4;
    } else if (gSaveData.unk_2458 == 4) {
        *arg0 = 1;
        return &data_ov030_020e2700;
    } else if (gSaveData.unk_2458 == 5) {
        *arg0 = 1;
        return &data_ov030_020e2380;
    } else if (gSaveData.unk_2458 == 2) {
        *arg0 = 1;
        return &data_ov030_020e2818;
    } else if (gSaveData.unk_2458 == 6) {
        if ((Progress_Check(12) != 0) && (Progress_Check(13) != 0)) {
            *arg0 = 8;
            return &data_ov030_020e5370;
        }
        if ((Progress_Check(12) != 0) || (Progress_Check(13) != 0)) {
            *arg0 = 5;
            return &data_ov030_020e4920;
        }
        *arg0 = 9;
        return &data_ov030_020e554c;
    } else if (gSaveData.unk_2458 == 0) {
        *arg0 = 1;
        return &data_ov030_020e286c;
    } else if (gSaveData.unk_2458 == 8) {
        *arg0 = 2;
        return &data_ov030_020e3378;
    } else if (gSaveData.unk_2458 == 9) {
        *arg0 = 3;
        return &data_ov030_020e3c38;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9104(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 2;
        return &data_ov030_020e33b0;
    } else if (gSaveData.unk_2458 == 5) {
        *arg0 = 1;
        return &data_ov030_020e28a4;
    } else if (gSaveData.unk_2458 == 11) {
        *arg0 = 1;
        return &data_ov030_020e28c0;
    } else if (gSaveData.unk_2458 == 12) {
        *arg0 = 2;
        return &data_ov030_020e33e8;
    } else if (gSaveData.unk_2458 == 13) {
        *arg0 = 1;
        return &data_ov030_020e2578;
    } else if (gSaveData.unk_2458 == 15) {
        *arg0 = 1;
        return &data_ov030_020e2a9c;
    } else if (gSaveData.unk_2458 == 14) {
        *arg0 = 1;
        return &data_ov030_020e2ab8;
    } else if (gSaveData.unk_2458 == 17) {
        *arg0 = 1;
        return &data_ov030_020e263c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b91fc(s32* arg0) {
    if ((Progress_Check(39) == 0) && (gSaveData.unk_2458 == 2)) {
        *arg0 = 1;
        return &data_ov030_020e2af0;
    } else if (gSaveData.unk_2458 == 6) {
        *arg0 = 1;
        return &data_ov030_020e2658;
    } else if ((Progress_Check(42) == 0) && (gSaveData.unk_2458 == 7)) {
        *arg0 = 4;
        return &data_ov030_020e4044;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9294(s32* arg0) {
    if (gSaveData.unk_2458 == 23) {
        *arg0 = 3;
        return &data_ov030_020e3c8c;
    } else if (gSaveData.unk_2458 == 24) {
        *arg0 = 4;
        return &data_ov030_020e40b4;
    } else if (gSaveData.unk_2458 == 25) {
        *arg0 = 4;
        return &data_ov030_020e4124;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9300(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e2bd0;
    } else if (gSaveData.unk_2458 == 5) {
        *arg0 = 1;
        return &data_ov030_020e26c8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9350(s32* arg0) {
    if (gSaveData.unk_2458 == 0x15) {
        *arg0 = 1;
        return &data_ov030_020e2428;
    } else if (gSaveData.unk_2458 == 0x17) {
        *arg0 = 1;
        return &data_ov030_020e2c5c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b93a0(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e25b0;
    } else if ((gSaveData.unk_2458 == 3) && (Progress_Check(39) == 0)) {
        *arg0 = 3;
        return &data_ov030_020e3ce0;
    } else if (gSaveData.unk_2458 == 6) {
        *arg0 = 6;
        return &data_ov030_020e4df0;
    } else if (gSaveData.unk_2458 == 0x11) {
        *arg0 = 1;
        return &data_ov030_020e2d58;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9444(s32* arg0) {
    if (gSaveData.unk_2458 == 14) {
        *arg0 = 1;
        return &data_ov030_020e27a8;
    } else if (gSaveData.unk_2458 == 15) {
        *arg0 = 15;
        return &data_ov030_020e5b34;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9494(s32* arg0) {
    if (gSaveData.unk_2458 == 4) {
        *arg0 = 3;
        return &data_ov030_020e3458;
    } else if (gSaveData.unk_2458 == 6) {
        *arg0 = 8;
        return &data_ov030_020e50d0;
    } else if (gSaveData.unk_2458 == 7) {
        *arg0 = 10;
        return &data_ov030_020e5648;
    } else if (gSaveData.unk_2458 == 9) {
        *arg0 = 3;
        return &data_ov030_020e34ac;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b951c(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 5;
        return &data_ov030_020e43a8;
    } else if (gSaveData.unk_2458 == 3) {
        *arg0 = 3;
        return &data_ov030_020e3500;
    } else if (gSaveData.unk_2458 == 4) {
        *arg0 = 3;
        return &data_ov030_020e3554;
    } else if (gSaveData.unk_2458 == 6) {
        *arg0 = 3;
        return &data_ov030_020e35a8;
    } else if (gSaveData.unk_2458 == 11) {
        *arg0 = 4;
        return &data_ov030_020e3d34;
    } else if (gSaveData.unk_2458 == 12) {
        *arg0 = 6;
        return &data_ov030_020e4b50;
    } else if (gSaveData.unk_2458 == 13) {
        *arg0 = 3;
        return &data_ov030_020e35fc;
    } else if (gSaveData.unk_2458 == 19) {
        *arg0 = 3;
        return &data_ov030_020e3650;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9614(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 5;
        return &data_ov030_020e454c;
    } else if (gSaveData.unk_2458 == 3) {
        *arg0 = 3;
        return &data_ov030_020e36a4;
    } else if (gSaveData.unk_2458 == 4) {
        *arg0 = 3;
        return &data_ov030_020e36f8;
    } else if (gSaveData.unk_2458 == 5) {
        *arg0 = 2;
        return &data_ov030_020e2ea8;
    } else if (gSaveData.unk_2458 == 2) {
        *arg0 = 2;
        return &data_ov030_020e2ee0;
    } else if (gSaveData.unk_2458 == 6) {
        *arg0 = 3;
        return &data_ov030_020e374c;
    } else if (gSaveData.unk_2458 == 0) {
        *arg0 = 3;
        return &data_ov030_020e37a0;
    } else if (gSaveData.unk_2458 == 7) {
        *arg0 = 2;
        return &data_ov030_020e2f18;
    } else if (gSaveData.unk_2458 == 8) {
        *arg0 = 3;
        return &data_ov030_020e37f4;
    } else if (gSaveData.unk_2458 == 9) {
        *arg0 = 2;
        return &data_ov030_020e2f50;
    } else if (gSaveData.unk_2458 == 11) {
        *arg0 = 4;
        return &data_ov030_020e3e84;
    } else if (gSaveData.unk_2458 == 12) {
        *arg0 = 5;
        return &data_ov030_020e45d8;
    } else if (gSaveData.unk_2458 == 13) {
        *arg0 = 3;
        return &data_ov030_020e3848;
    } else if (gSaveData.unk_2458 == 14) {
        *arg0 = 1;
        return &data_ov030_020e2604;
    } else if (gSaveData.unk_2458 == 15) {
        *arg0 = 1;
        return &data_ov030_020e2c40;
    } else if (gSaveData.unk_2458 == 16) {
        *arg0 = 2;
        return &data_ov030_020e2fc0;
    } else if (gSaveData.unk_2458 == 17) {
        *arg0 = 3;
        return &data_ov030_020e38f0;
    } else if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e247c;
    } else if (gSaveData.unk_2458 == 19) {
        *arg0 = 3;
        return &data_ov030_020e3944;
    } else if (gSaveData.unk_2458 == 21) {
        *arg0 = 2;
        return &data_ov030_020e30a0;
    } else if (gSaveData.unk_2458 == 22) {
        *arg0 = 3;
        return &data_ov030_020e3998;
    } else if (gSaveData.unk_2458 == 39) {
        *arg0 = 3;
        return &data_ov030_020e3a40;
    } else if (gSaveData.unk_2458 == 40) {
        *arg0 = 2;
        return &data_ov030_020e3110;
    } else if (gSaveData.unk_2458 == 41) {
        *arg0 = 1;
        return &data_ov030_020e240c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b98cc(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        if ((Progress_Check(3) == 0) && (Progress_Check(56) == 0)) {
            *arg0 = 2;
            return &data_ov030_020e3180;
        } else if (Progress_Check(32) == 0) {
            *arg0 = 1;
            return &data_ov030_020e2674;
        }
    }

    if (gSaveData.unk_2458 == 3) {
        if ((Progress_Check(3) == 0) && (Progress_Check(61) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e278c;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(67) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2b7c;
        }
    }

    if (gSaveData.unk_2458 == 4) {
        if ((Progress_Check(3) == 0) && (Progress_Check(57) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2a80;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(52) == 0)) {
            *arg0 = 2;
            return &data_ov030_020e3298;
        }
    }

    if (gSaveData.unk_2458 == 5 && Progress_Check(4) != 0 && Progress_Check(52) == 0 && Progress_Check(69) == 0) {
        *arg0 = 1;
        return &data_ov030_020e2b60;
    }

    if ((gSaveData.unk_2458 == 2) && (Progress_Check(3) == 0) && (Progress_Check(58) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e27c4;
    }

    if (gSaveData.unk_2458 == 6) {
        if (Progress_Check(3) == 0) {
            *arg0 = 1;
            return &data_ov030_020e23d4;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(73) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2d74;
        }
    }

    if ((gSaveData.unk_2458 == 0) && (Progress_Check(3) == 0) && (Progress_Check(62) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e2770;
    }

    if (gSaveData.unk_2458 == 7) {
        if ((Progress_Check(3) == 0) && (Progress_Check(64) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e29f4;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(75) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e24d0;
        }
    }

    if (gSaveData.unk_2458 == 8) {
        if ((Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(72) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e24ec;
        } else if ((Progress_Check(53) != 0) && (Progress_Check(45) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2364;
        } else if ((Progress_Check(45) != 0) && (Progress_Check(47) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e23f0;
        }
    }

    if (gSaveData.unk_2458 == 9) {
        if ((Progress_Check(3) == 0) && (Progress_Check(65) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2e54;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(74) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e294c;
        }
    }

    if ((gSaveData.unk_2458 == 11) && (Progress_Check(3) == 0) && (Progress_Check(59) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e28dc;
    }

    if ((gSaveData.unk_2458 == 12) && (Progress_Check(3) == 0) && (Progress_Check(63) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e28f8;
    }

    if ((gSaveData.unk_2458 == 13) && (Progress_Check(7) == 0)) {
        if (Progress_Check(3) == 0) {
            *arg0 = 1;
            return &data_ov030_020e2594;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(71) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2690;
        }
    }

    if (gSaveData.unk_2458 == 14) {
        if ((Progress_Check(4) != 0) && (Progress_Check(32) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2c08;
        } else if ((Progress_Check(32) != 0) && (Progress_Check(33) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e49ac;
        } else if ((Progress_Check(33) != 0) && (Progress_Check(34) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4a38;
        } else if ((Progress_Check(34) != 0) && (Progress_Check(35) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4ac4;
        } else if ((Progress_Check(35) != 0) && (Progress_Check(41) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4204;
        } else if ((Progress_Check(41) != 0) && (Progress_Check(36) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4290;
        } else if ((Progress_Check(36) != 0) && (Progress_Check(37) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e431c;
        } else if ((Progress_Check(37) != 0) && (Progress_Check(38) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4434;
        } else if ((Progress_Check(38) != 0) && (Progress_Check(52) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e44c0;
        }
    }

    if (gSaveData.unk_2458 == 15) {
        if (Progress_Check(3) == 0) {
            *arg0 = 1;
            return &data_ov030_020e2e1c;
        } else if (Progress_Check(4) != 0) {
            *arg0 = 1;
            return &data_ov030_020e2de4;
        }
    }

    if ((gSaveData.unk_2458 == 17) && (Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(0x46) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e2a64;
    }

    if ((gSaveData.unk_2458 == 19) && (Progress_Check(3) == 0) && (Progress_Check(0x3C) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e2d20;
    }

    if ((gSaveData.unk_2458 == 21) && (Progress_Check(4) != 0) && (Progress_Check(0x34) == 0) && (Progress_Check(0x44) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e2498;
    }

    if ((gSaveData.unk_2458 == 22) && (Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(0x42) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e2c78;
    }

    if (gSaveData.unk_2458 == 23) {
        if ((Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(76) == 0)) {
            *arg0 = 2;
            return &data_ov030_020e2f88;
        } else {
            *arg0 = 1;
            return &data_ov030_020e2ce8;
        }
    }

    if ((gSaveData.unk_2458 == 41) && (Progress_Check(4) != 0) && (Progress_Check(52) == 0) && (Progress_Check(0x4D) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e2c94;
    }

    if (gSaveData.unk_2458 == 43) {
        if ((Progress_Check(4) != 0) && (Progress_Check(47) == 0) && (Progress_Check(109) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2444;
        } else if ((Progress_Check(4) != 0) && (Progress_Check(47) == 0) && (Progress_Check(109) != 0)) {
            *arg0 = 1;
            return &data_ov030_020e2930;
        }
    }

    if (gSaveData.unk_2458 == 24) {
        *arg0 = 1;
        return &data_ov030_020e2b0c;
    } else if (gSaveData.unk_2458 == 25) {
        *arg0 = 1;
        return &data_ov030_020e2b44;
    } else if (gSaveData.unk_2458 == 26) {
        *arg0 = 1;
        return &data_ov030_020e2b28;
    } else if (gSaveData.unk_2458 == 27) {
        *arg0 = 1;
        return &data_ov030_020e29a0;
    } else if (gSaveData.unk_2458 == 28) {
        *arg0 = 1;
        return &data_ov030_020e2b98;
    } else if (gSaveData.unk_2458 == 29) {
        *arg0 = 1;
        return &data_ov030_020e2620;
    } else if (gSaveData.unk_2458 == 30) {
        *arg0 = 1;
        return &data_ov030_020e2754;
    } else if (gSaveData.unk_2458 == 31) {
        *arg0 = 1;
        return &data_ov030_020e2a10;
    } else if (gSaveData.unk_2458 == 32) {
        *arg0 = 1;
        return &data_ov030_020e239c;
    } else if (gSaveData.unk_2458 == 33) {
        *arg0 = 1;
        return &data_ov030_020e2508;
    } else if (gSaveData.unk_2458 == 34) {
        *arg0 = 1;
        return &data_ov030_020e2968;
    } else if (gSaveData.unk_2458 == 35) {
        *arg0 = 1;
        return &data_ov030_020e2914;
    } else if (gSaveData.unk_2458 == 36) {
        *arg0 = 1;
        return &data_ov030_020e2540;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba4b4(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 4;
        return &data_ov030_020e4194;
    } else if (gSaveData.unk_2458 == 9) {
        *arg0 = 1;
        return &data_ov030_020e2d3c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba504(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 2;
        return &data_ov030_020e2e70;
    }
    *arg0 = 0;
    return NULL;
}

static s32* func_ov030_020ba538(s32* arg0) {
    if (gSaveData.unk_2458 == 21) {
        *arg0 = 8;
        return &data_ov030_020e51b0;
    } else if (gSaveData.unk_2458 == 22) {
        *arg0 = 4;
        return &data_ov030_020e3da4;
    } else if (gSaveData.unk_2458 == 23) {
        *arg0 = 4;
        return &data_ov030_020e3e14;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba5a4(s32* arg0) {
    if (gSaveData.unk_2458 == 0x12) {
        *arg0 = 1;
        return &data_ov030_020e2d90;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba5d8(s32* arg0) {
    if (gSaveData.unk_2458 == 24) {
        *arg0 = 8;
        return &data_ov030_020e5290;
    } else if (gSaveData.unk_2458 == 25) {
        *arg0 = 10;
        return &data_ov030_020e5760;
    } else if (gSaveData.unk_2458 == 26) {
        *arg0 = 7;
        return &data_ov030_020e4f48;
    } else if (gSaveData.unk_2458 == 27) {
        *arg0 = 14;
        return &data_ov030_020e59ac;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba660(s32* arg0) {
    if (gSaveData.unk_2458 == 0x12) {
        *arg0 = 2;
        return &data_ov030_020e3148;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba694(s32* arg0) {
    if (gSaveData.unk_2458 == 0x12) {
        *arg0 = 1;
        return &data_ov030_020e2bb4;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba6c8(s32* arg0) {
    if (gSaveData.unk_2458 == 0x12) {
        *arg0 = 1;
        return &data_ov030_020e26ac;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba6fc(s32* arg0) {
    if (gSaveData.unk_2458 == 0x12) {
        *arg0 = 1;
        return &data_ov030_020e29bc;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba730(s32* arg0) {
    if (gSaveData.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e2984;
    } else if (gSaveData.unk_2458 == 11) {
        *arg0 = 1;
        return &data_ov030_020e255c;
    } else if (gSaveData.unk_2458 == 12) {
        *arg0 = 1;
        return &data_ov030_020e2dac;
    } else if (gSaveData.unk_2458 == 13) {
        *arg0 = 1;
        return &data_ov030_020e2850;
    } else if (gSaveData.unk_2458 == 15) {
        *arg0 = 1;
        return &data_ov030_020e25cc;
    } else if (gSaveData.unk_2458 == 14) {
        *arg0 = 1;
        return &data_ov030_020e27fc;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba7f0(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2c24;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba824(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2dc8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba858(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2a48;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba88c(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e29d8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba8c0(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2cb0;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba8f4(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e23b8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba928(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e271c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba95c(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2ccc;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba990(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e25e8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba9c4(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e24b4;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba9f8(s32* arg0) {
    if (gSaveData.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2524;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

typedef s32* (*UnknownTableFunc)(s32*);

const UnknownTableFunc data_ov030_020e4e98[44] = {
    func_ov030_020b892c, func_ov030_020b897c, func_ov030_020b89e8, func_ov030_020b8a70, func_ov030_020b8af8,
    func_ov030_020b8b80, func_ov030_020b8c40, func_ov030_020b8cc8, func_ov030_020b8d34, func_ov030_020b8dbc,
    func_ov030_020b8fa0, func_ov030_020b9104, func_ov030_020b91fc, func_ov030_020b9294, func_ov030_020b9300,
    func_ov030_020b9350, func_ov030_020b93a0, func_ov030_020b9444, func_ov030_020b9494, func_ov030_020b951c,
    func_ov030_020b9614, func_ov030_020b98cc, func_ov030_020ba4b4, func_ov030_020ba504, func_ov030_020ba538,
    func_ov030_020ba5a4, func_ov030_020ba5d8, func_ov030_020ba660, func_ov030_020ba694, func_ov030_020ba6c8,
    func_ov030_020ba6fc, func_ov030_020ba730, func_ov030_020ba7f0, func_ov030_020ba824, func_ov030_020ba858,
    func_ov030_020ba88c, func_ov030_020ba8c0, func_ov030_020ba8f4, func_ov030_020ba928, func_ov030_020ba95c,
    func_ov030_020ba990, func_ov030_020ba9c4, func_ov030_020ba9f8, func_ov030_020b891c,
};

s32* func_ov030_020baa2c(s32* arg0) {
    if (gSaveData.unk_1AB0 >= 44) {
        gSaveData.unk_1AB0 = 0;
    }
    return data_ov030_020e4e98[gSaveData.unk_1AB0](arg0);
}
