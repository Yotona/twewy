#include "Save.h"
#include <types.h>

extern s32 func_ov030_020aebb0(s32);

s32 data_ov030_020e2364;
s32 data_ov030_020e2380;
s32 data_ov030_020e239c;
s32 data_ov030_020e23b8;
s32 data_ov030_020e23d4;
s32 data_ov030_020e23f0;
s32 data_ov030_020e240c;
s32 data_ov030_020e2428;
s32 data_ov030_020e2444;
s32 data_ov030_020e2460;
s32 data_ov030_020e247c;
s32 data_ov030_020e2498;
s32 data_ov030_020e24b4;
s32 data_ov030_020e24d0;
s32 data_ov030_020e24ec;
s32 data_ov030_020e2508;
s32 data_ov030_020e2524;
s32 data_ov030_020e2540;
s32 data_ov030_020e255c;
s32 data_ov030_020e2578;
s32 data_ov030_020e2594;
s32 data_ov030_020e25b0;
s32 data_ov030_020e25cc;
s32 data_ov030_020e25e8;
s32 data_ov030_020e2604;
s32 data_ov030_020e2620;
s32 data_ov030_020e263c;
s32 data_ov030_020e2658;
s32 data_ov030_020e2674;
s32 data_ov030_020e2690;
s32 data_ov030_020e26ac;
s32 data_ov030_020e26c8;
s32 data_ov030_020e26e4;
s32 data_ov030_020e2700;
s32 data_ov030_020e271c;
s32 data_ov030_020e2738;
s32 data_ov030_020e2754;
s32 data_ov030_020e2770;
s32 data_ov030_020e278c;
s32 data_ov030_020e27a8;
s32 data_ov030_020e27c4;
s32 data_ov030_020e27e0;
s32 data_ov030_020e27fc;
s32 data_ov030_020e2818;
s32 data_ov030_020e2834;
s32 data_ov030_020e2850;
s32 data_ov030_020e286c;
s32 data_ov030_020e2888;
s32 data_ov030_020e28a4;
s32 data_ov030_020e28c0;
s32 data_ov030_020e28dc;
s32 data_ov030_020e28f8;
s32 data_ov030_020e2914;
s32 data_ov030_020e2930;
s32 data_ov030_020e294c;
s32 data_ov030_020e2968;
s32 data_ov030_020e2984;
s32 data_ov030_020e29a0;
s32 data_ov030_020e29bc;
s32 data_ov030_020e29d8;
s32 data_ov030_020e29f4;
s32 data_ov030_020e2a10;
s32 data_ov030_020e2a2c;
s32 data_ov030_020e2a48;
s32 data_ov030_020e2a64;
s32 data_ov030_020e2a80;
s32 data_ov030_020e2a9c;
s32 data_ov030_020e2ab8;
s32 data_ov030_020e2ad4;
s32 data_ov030_020e2af0;
s32 data_ov030_020e2b0c;
s32 data_ov030_020e2b28;
s32 data_ov030_020e2b44;
s32 data_ov030_020e2b60;
s32 data_ov030_020e2b7c;
s32 data_ov030_020e2b98;
s32 data_ov030_020e2bb4;
s32 data_ov030_020e2bd0;
s32 data_ov030_020e2bec;
s32 data_ov030_020e2c08;
s32 data_ov030_020e2c24;
s32 data_ov030_020e2c40;
s32 data_ov030_020e2c5c;
s32 data_ov030_020e2c78;
s32 data_ov030_020e2c94;
s32 data_ov030_020e2cb0;
s32 data_ov030_020e2ccc;
s32 data_ov030_020e2ce8;
s32 data_ov030_020e2d04;
s32 data_ov030_020e2d20;
s32 data_ov030_020e2d3c;
s32 data_ov030_020e2d58;
s32 data_ov030_020e2d74;
s32 data_ov030_020e2d90;
s32 data_ov030_020e2dac;
s32 data_ov030_020e2dc8;
s32 data_ov030_020e2de4;
s32 data_ov030_020e2e00;
s32 data_ov030_020e2e1c;
s32 data_ov030_020e2e38;
s32 data_ov030_020e2e54;
s32 data_ov030_020e2e70;
s32 data_ov030_020e2ea8;
s32 data_ov030_020e2ee0;
s32 data_ov030_020e2f18;
s32 data_ov030_020e2f50;
s32 data_ov030_020e2f88;
s32 data_ov030_020e2fc0;
s32 data_ov030_020e2ff8;
s32 data_ov030_020e3030;
s32 data_ov030_020e3068;
s32 data_ov030_020e30a0;
s32 data_ov030_020e30d8;
s32 data_ov030_020e3110;
s32 data_ov030_020e3148;
s32 data_ov030_020e3180;
s32 data_ov030_020e31b8;
s32 data_ov030_020e31f0;
s32 data_ov030_020e3228;
s32 data_ov030_020e3260;
s32 data_ov030_020e3298;
s32 data_ov030_020e32d0;
s32 data_ov030_020e3308;
s32 data_ov030_020e3340;
s32 data_ov030_020e3378;
s32 data_ov030_020e33b0;
s32 data_ov030_020e33e8;
s32 data_ov030_020e3420;
s32 data_ov030_020e3458;
s32 data_ov030_020e34ac;
s32 data_ov030_020e3500;
s32 data_ov030_020e3554;
s32 data_ov030_020e35a8;
s32 data_ov030_020e35fc;
s32 data_ov030_020e3650;
s32 data_ov030_020e36a4;
s32 data_ov030_020e36f8;
s32 data_ov030_020e374c;
s32 data_ov030_020e37a0;
s32 data_ov030_020e37f4;
s32 data_ov030_020e3848;
s32 data_ov030_020e389c;
s32 data_ov030_020e38f0;
s32 data_ov030_020e3944;
s32 data_ov030_020e3998;
s32 data_ov030_020e39ec;
s32 data_ov030_020e3a40;
s32 data_ov030_020e3a94;
s32 data_ov030_020e3ae8;
s32 data_ov030_020e3b3c;
s32 data_ov030_020e3b90;
s32 data_ov030_020e3be4;
s32 data_ov030_020e3c38;
s32 data_ov030_020e3c8c;
s32 data_ov030_020e3ce0;
s32 data_ov030_020e3d34;
s32 data_ov030_020e3da4;
s32 data_ov030_020e3e14;
s32 data_ov030_020e3e84;
s32 data_ov030_020e3ef4;
s32 data_ov030_020e3f64;
s32 data_ov030_020e3fd4;
s32 data_ov030_020e4044;
s32 data_ov030_020e40b4;
s32 data_ov030_020e4124;
s32 data_ov030_020e4194;
s32 data_ov030_020e4204;
s32 data_ov030_020e4290;
s32 data_ov030_020e431c;
s32 data_ov030_020e43a8;
s32 data_ov030_020e4434;
s32 data_ov030_020e44c0;
s32 data_ov030_020e454c;
s32 data_ov030_020e45d8;
s32 data_ov030_020e4664;
s32 data_ov030_020e46f0;
s32 data_ov030_020e477c;
s32 data_ov030_020e4808;
s32 data_ov030_020e4894;
s32 data_ov030_020e4920;
s32 data_ov030_020e49ac;
s32 data_ov030_020e4a38;
s32 data_ov030_020e4ac4;
s32 data_ov030_020e4b50;
s32 data_ov030_020e4bf8;
s32 data_ov030_020e4ca0;
s32 data_ov030_020e4d48;
s32 data_ov030_020e4df0;
s32 data_ov030_020e4f48;
s32 data_ov030_020e500c;
s32 data_ov030_020e50d0;
s32 data_ov030_020e51b0;
s32 data_ov030_020e5290;
s32 data_ov030_020e5370;
s32 data_ov030_020e5450;
s32 data_ov030_020e554c;
s32 data_ov030_020e5648;
s32 data_ov030_020e5760;
s32 data_ov030_020e5878;
s32 data_ov030_020e59ac;
s32 data_ov030_020e5b34;
s32 data_ov030_020e5cd8;

static s32* func_ov030_020b891c(s32* arg0) {
    *arg0 = 0;
    return NULL;
}

static s32* func_ov030_020b892c(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e2888;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        *arg0 = 2;
        return &data_ov030_020e3420;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b897c(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 2;
        return &data_ov030_020e3308;
    } else if (data_02071cf0.unk_20.unk_2458 == 17) {
        *arg0 = 6;
        return &data_ov030_020e4bf8;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        *arg0 = 3;
        return &data_ov030_020e389c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b89e8(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 19) {
        *arg0 = 5;
        return &data_ov030_020e4664;
    } else if (data_02071cf0.unk_20.unk_2458 == 21) {
        *arg0 = 11;
        return &data_ov030_020e5878;
    } else if (data_02071cf0.unk_20.unk_2458 == 22) {
        *arg0 = 5;
        return &data_ov030_020e46f0;
    } else if (data_02071cf0.unk_20.unk_2458 == 23) {
        *arg0 = 5;
        return &data_ov030_020e477c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8a70(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e26e4;
    } else if (data_02071cf0.unk_20.unk_2458 == 3) {
        *arg0 = 2;
        return &data_ov030_020e2ff8;
    } else if (data_02071cf0.unk_20.unk_2458 == 11) {
        *arg0 = 6;
        return &data_ov030_020e4ca0;
    } else if (data_02071cf0.unk_20.unk_2458 == 12) {
        *arg0 = 2;
        return &data_ov030_020e3030;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8af8(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 2) {
        *arg0 = 1;
        return &data_ov030_020e2834;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        *arg0 = 2;
        return &data_ov030_020e3068;
    } else if (data_02071cf0.unk_20.unk_2458 == 0) {
        *arg0 = 9;
        return &data_ov030_020e5450;
    } else if (data_02071cf0.unk_20.unk_2458 == 8) {
        *arg0 = 4;
        return &data_ov030_020e3ef4;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8b80(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 15;
        return &data_ov030_020e5cd8;
    } else if (data_02071cf0.unk_20.unk_2458 == 3) {
        *arg0 = 4;
        return &data_ov030_020e3f64;
    } else if (data_02071cf0.unk_20.unk_2458 == 4) {
        *arg0 = 3;
        return &data_ov030_020e39ec;
    } else if (data_02071cf0.unk_20.unk_2458 == 0) {
        *arg0 = 2;
        return &data_ov030_020e30d8;
    } else if (data_02071cf0.unk_20.unk_2458 == 8) {
        *arg0 = 1;
        return &data_ov030_020e2ad4;
    } else if (data_02071cf0.unk_20.unk_2458 == 12) {
        *arg0 = 1;
        return &data_ov030_020e2bec;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8c40(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 3;
        return &data_ov030_020e3a94;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        *arg0 = 1;
        return &data_ov030_020e2e00;
    } else if (data_02071cf0.unk_20.unk_2458 == 8) {
        *arg0 = 1;
        return &data_ov030_020e27e0;
    } else if (data_02071cf0.unk_20.unk_2458 == 17) {
        *arg0 = 1;
        return &data_ov030_020e2d04;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8cc8(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e2460;
    } else if (data_02071cf0.unk_20.unk_2458 == 3) {
        *arg0 = 1;
        return &data_ov030_020e2738;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        *arg0 = 3;
        return &data_ov030_020e3ae8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8d34(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 2;
        return &data_ov030_020e31b8;
    } else if (data_02071cf0.unk_20.unk_2458 == 4) {
        *arg0 = 1;
        return &data_ov030_020e2e38;
    } else if (data_02071cf0.unk_20.unk_2458 == 8) {
        *arg0 = 2;
        return &data_ov030_020e31f0;
    } else if (data_02071cf0.unk_20.unk_2458 == 11) {
        *arg0 = 6;
        return &data_ov030_020e4d48;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8dbc(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 2;
        return &data_ov030_020e3228;
    } else if ((data_02071cf0.unk_20.unk_2458 == 4) && (func_ov030_020aebb0(9) != 0) && (func_ov030_020aebb0(11) == 0)) {
        *arg0 = 2;
        return &data_ov030_020e3260;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        *arg0 = 3;
        return &data_ov030_020e3b3c;
    } else if ((data_02071cf0.unk_20.unk_2458 == 0) && (func_ov030_020aebb0(9) != 0)) {
        *arg0 = 2;
        return &data_ov030_020e32d0;
    } else if (data_02071cf0.unk_20.unk_2458 == 8) {
        *arg0 = 5;
        return &data_ov030_020e4808;
    } else if ((data_02071cf0.unk_20.unk_2458 == 12) && (func_ov030_020aebb0(12) == 0) && (func_ov030_020aebb0(73) == 0)) {
        *arg0 = 3;
        return &data_ov030_020e3b90;
    } else if (data_02071cf0.unk_20.unk_2458 == 15) {
        *arg0 = 1;
        return &data_ov030_020e2a2c;
    } else if (data_02071cf0.unk_20.unk_2458 == 19) {
        *arg0 = 7;
        return &data_ov030_020e500c;
    } else if (data_02071cf0.unk_20.unk_2458 == 21) {
        *arg0 = 5;
        return &data_ov030_020e4894;
    } else if ((data_02071cf0.unk_20.unk_2458 == 22) && (func_ov030_020aebb0(12) != 0)) {
        *arg0 = 4;
        return &data_ov030_020e3fd4;
    } else if ((data_02071cf0.unk_20.unk_2458 == 0x17) && (func_ov030_020aebb0(12) != 0)) {
        *arg0 = 2;
        return &data_ov030_020e3340;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b8fa0(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 3;
        return &data_ov030_020e3be4;
    } else if (data_02071cf0.unk_20.unk_2458 == 4) {
        *arg0 = 1;
        return &data_ov030_020e2700;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        *arg0 = 1;
        return &data_ov030_020e2380;
    } else if (data_02071cf0.unk_20.unk_2458 == 2) {
        *arg0 = 1;
        return &data_ov030_020e2818;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        if ((func_ov030_020aebb0(12) != 0) && (func_ov030_020aebb0(13) != 0)) {
            *arg0 = 8;
            return &data_ov030_020e5370;
        }
        if ((func_ov030_020aebb0(12) != 0) || (func_ov030_020aebb0(13) != 0)) {
            *arg0 = 5;
            return &data_ov030_020e4920;
        }
        *arg0 = 9;
        return &data_ov030_020e554c;
    } else if (data_02071cf0.unk_20.unk_2458 == 0) {
        *arg0 = 1;
        return &data_ov030_020e286c;
    } else if (data_02071cf0.unk_20.unk_2458 == 8) {
        *arg0 = 2;
        return &data_ov030_020e3378;
    } else if (data_02071cf0.unk_20.unk_2458 == 9) {
        *arg0 = 3;
        return &data_ov030_020e3c38;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9104(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 2;
        return &data_ov030_020e33b0;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        *arg0 = 1;
        return &data_ov030_020e28a4;
    } else if (data_02071cf0.unk_20.unk_2458 == 11) {
        *arg0 = 1;
        return &data_ov030_020e28c0;
    } else if (data_02071cf0.unk_20.unk_2458 == 12) {
        *arg0 = 2;
        return &data_ov030_020e33e8;
    } else if (data_02071cf0.unk_20.unk_2458 == 13) {
        *arg0 = 1;
        return &data_ov030_020e2578;
    } else if (data_02071cf0.unk_20.unk_2458 == 15) {
        *arg0 = 1;
        return &data_ov030_020e2a9c;
    } else if (data_02071cf0.unk_20.unk_2458 == 14) {
        *arg0 = 1;
        return &data_ov030_020e2ab8;
    } else if (data_02071cf0.unk_20.unk_2458 == 17) {
        *arg0 = 1;
        return &data_ov030_020e263c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b91fc(s32* arg0) {
    if ((func_ov030_020aebb0(39) == 0) && (data_02071cf0.unk_20.unk_2458 == 2)) {
        *arg0 = 1;
        return &data_ov030_020e2af0;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        *arg0 = 1;
        return &data_ov030_020e2658;
    } else if ((func_ov030_020aebb0(42) == 0) && (data_02071cf0.unk_20.unk_2458 == 7)) {
        *arg0 = 4;
        return &data_ov030_020e4044;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9294(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 23) {
        *arg0 = 3;
        return &data_ov030_020e3c8c;
    } else if (data_02071cf0.unk_20.unk_2458 == 24) {
        *arg0 = 4;
        return &data_ov030_020e40b4;
    } else if (data_02071cf0.unk_20.unk_2458 == 25) {
        *arg0 = 4;
        return &data_ov030_020e4124;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9300(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e2bd0;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        *arg0 = 1;
        return &data_ov030_020e26c8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9350(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 0x15) {
        *arg0 = 1;
        return &data_ov030_020e2428;
    } else if (data_02071cf0.unk_20.unk_2458 == 0x17) {
        *arg0 = 1;
        return &data_ov030_020e2c5c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b93a0(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e25b0;
    } else if ((data_02071cf0.unk_20.unk_2458 == 3) && (func_ov030_020aebb0(39) == 0)) {
        *arg0 = 3;
        return &data_ov030_020e3ce0;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        *arg0 = 6;
        return &data_ov030_020e4df0;
    } else if (data_02071cf0.unk_20.unk_2458 == 0x11) {
        *arg0 = 1;
        return &data_ov030_020e2d58;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9444(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 14) {
        *arg0 = 1;
        return &data_ov030_020e27a8;
    } else if (data_02071cf0.unk_20.unk_2458 == 15) {
        *arg0 = 15;
        return &data_ov030_020e5b34;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9494(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 4) {
        *arg0 = 3;
        return &data_ov030_020e3458;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        *arg0 = 8;
        return &data_ov030_020e50d0;
    } else if (data_02071cf0.unk_20.unk_2458 == 7) {
        *arg0 = 10;
        return &data_ov030_020e5648;
    } else if (data_02071cf0.unk_20.unk_2458 == 9) {
        *arg0 = 3;
        return &data_ov030_020e34ac;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b951c(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 5;
        return &data_ov030_020e43a8;
    } else if (data_02071cf0.unk_20.unk_2458 == 3) {
        *arg0 = 3;
        return &data_ov030_020e3500;
    } else if (data_02071cf0.unk_20.unk_2458 == 4) {
        *arg0 = 3;
        return &data_ov030_020e3554;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        *arg0 = 3;
        return &data_ov030_020e35a8;
    } else if (data_02071cf0.unk_20.unk_2458 == 11) {
        *arg0 = 4;
        return &data_ov030_020e3d34;
    } else if (data_02071cf0.unk_20.unk_2458 == 12) {
        *arg0 = 6;
        return &data_ov030_020e4b50;
    } else if (data_02071cf0.unk_20.unk_2458 == 13) {
        *arg0 = 3;
        return &data_ov030_020e35fc;
    } else if (data_02071cf0.unk_20.unk_2458 == 19) {
        *arg0 = 3;
        return &data_ov030_020e3650;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b9614(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 5;
        return &data_ov030_020e454c;
    } else if (data_02071cf0.unk_20.unk_2458 == 3) {
        *arg0 = 3;
        return &data_ov030_020e36a4;
    } else if (data_02071cf0.unk_20.unk_2458 == 4) {
        *arg0 = 3;
        return &data_ov030_020e36f8;
    } else if (data_02071cf0.unk_20.unk_2458 == 5) {
        *arg0 = 2;
        return &data_ov030_020e2ea8;
    } else if (data_02071cf0.unk_20.unk_2458 == 2) {
        *arg0 = 2;
        return &data_ov030_020e2ee0;
    } else if (data_02071cf0.unk_20.unk_2458 == 6) {
        *arg0 = 3;
        return &data_ov030_020e374c;
    } else if (data_02071cf0.unk_20.unk_2458 == 0) {
        *arg0 = 3;
        return &data_ov030_020e37a0;
    } else if (data_02071cf0.unk_20.unk_2458 == 7) {
        *arg0 = 2;
        return &data_ov030_020e2f18;
    } else if (data_02071cf0.unk_20.unk_2458 == 8) {
        *arg0 = 3;
        return &data_ov030_020e37f4;
    } else if (data_02071cf0.unk_20.unk_2458 == 9) {
        *arg0 = 2;
        return &data_ov030_020e2f50;
    } else if (data_02071cf0.unk_20.unk_2458 == 11) {
        *arg0 = 4;
        return &data_ov030_020e3e84;
    } else if (data_02071cf0.unk_20.unk_2458 == 12) {
        *arg0 = 5;
        return &data_ov030_020e45d8;
    } else if (data_02071cf0.unk_20.unk_2458 == 13) {
        *arg0 = 3;
        return &data_ov030_020e3848;
    } else if (data_02071cf0.unk_20.unk_2458 == 14) {
        *arg0 = 1;
        return &data_ov030_020e2604;
    } else if (data_02071cf0.unk_20.unk_2458 == 15) {
        *arg0 = 1;
        return &data_ov030_020e2c40;
    } else if (data_02071cf0.unk_20.unk_2458 == 16) {
        *arg0 = 2;
        return &data_ov030_020e2fc0;
    } else if (data_02071cf0.unk_20.unk_2458 == 17) {
        *arg0 = 3;
        return &data_ov030_020e38f0;
    } else if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e247c;
    } else if (data_02071cf0.unk_20.unk_2458 == 19) {
        *arg0 = 3;
        return &data_ov030_020e3944;
    } else if (data_02071cf0.unk_20.unk_2458 == 21) {
        *arg0 = 2;
        return &data_ov030_020e30a0;
    } else if (data_02071cf0.unk_20.unk_2458 == 22) {
        *arg0 = 3;
        return &data_ov030_020e3998;
    } else if (data_02071cf0.unk_20.unk_2458 == 39) {
        *arg0 = 3;
        return &data_ov030_020e3a40;
    } else if (data_02071cf0.unk_20.unk_2458 == 40) {
        *arg0 = 2;
        return &data_ov030_020e3110;
    } else if (data_02071cf0.unk_20.unk_2458 == 41) {
        *arg0 = 1;
        return &data_ov030_020e240c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020b98cc(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        if ((func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(56) == 0)) {
            *arg0 = 2;
            return &data_ov030_020e3180;
        } else if (func_ov030_020aebb0(32) == 0) {
            *arg0 = 1;
            return &data_ov030_020e2674;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 3) {
        if ((func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(61) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e278c;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) && (func_ov030_020aebb0(67) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2b7c;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 4) {
        if ((func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(57) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2a80;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0)) {
            *arg0 = 2;
            return &data_ov030_020e3298;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 5 && func_ov030_020aebb0(4) != 0 && func_ov030_020aebb0(52) == 0 &&
        func_ov030_020aebb0(69) == 0)
    {
        *arg0 = 1;
        return &data_ov030_020e2b60;
    }

    if ((data_02071cf0.unk_20.unk_2458 == 2) && (func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(58) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e27c4;
    }

    if (data_02071cf0.unk_20.unk_2458 == 6) {
        if (func_ov030_020aebb0(3) == 0) {
            *arg0 = 1;
            return &data_ov030_020e23d4;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) && (func_ov030_020aebb0(73) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2d74;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 0) && (func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(62) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e2770;
    }

    if (data_02071cf0.unk_20.unk_2458 == 7) {
        if ((func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(64) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e29f4;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) && (func_ov030_020aebb0(75) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e24d0;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 8) {
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) && (func_ov030_020aebb0(72) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e24ec;
        } else if ((func_ov030_020aebb0(53) != 0) && (func_ov030_020aebb0(45) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2364;
        } else if ((func_ov030_020aebb0(45) != 0) && (func_ov030_020aebb0(47) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e23f0;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 9) {
        if ((func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(65) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2e54;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) && (func_ov030_020aebb0(74) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e294c;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 11) && (func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(59) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e28dc;
    }

    if ((data_02071cf0.unk_20.unk_2458 == 12) && (func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(63) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e28f8;
    }

    if ((data_02071cf0.unk_20.unk_2458 == 13) && (func_ov030_020aebb0(7) == 0)) {
        if (func_ov030_020aebb0(3) == 0) {
            *arg0 = 1;
            return &data_ov030_020e2594;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) && (func_ov030_020aebb0(71) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2690;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 14) {
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(32) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2c08;
        } else if ((func_ov030_020aebb0(32) != 0) && (func_ov030_020aebb0(33) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e49ac;
        } else if ((func_ov030_020aebb0(33) != 0) && (func_ov030_020aebb0(34) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4a38;
        } else if ((func_ov030_020aebb0(34) != 0) && (func_ov030_020aebb0(35) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4ac4;
        } else if ((func_ov030_020aebb0(35) != 0) && (func_ov030_020aebb0(41) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4204;
        } else if ((func_ov030_020aebb0(41) != 0) && (func_ov030_020aebb0(36) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4290;
        } else if ((func_ov030_020aebb0(36) != 0) && (func_ov030_020aebb0(37) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e431c;
        } else if ((func_ov030_020aebb0(37) != 0) && (func_ov030_020aebb0(38) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e4434;
        } else if ((func_ov030_020aebb0(38) != 0) && (func_ov030_020aebb0(52) == 0)) {
            *arg0 = 5;
            return &data_ov030_020e44c0;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 15) {
        if (func_ov030_020aebb0(3) == 0) {
            *arg0 = 1;
            return &data_ov030_020e2e1c;
        } else if (func_ov030_020aebb0(4) != 0) {
            *arg0 = 1;
            return &data_ov030_020e2de4;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 17) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) &&
        (func_ov030_020aebb0(0x46) == 0))
    {
        *arg0 = 1;
        return &data_ov030_020e2a64;
    }

    if ((data_02071cf0.unk_20.unk_2458 == 19) && (func_ov030_020aebb0(3) == 0) && (func_ov030_020aebb0(0x3C) == 0)) {
        *arg0 = 1;
        return &data_ov030_020e2d20;
    }

    if ((data_02071cf0.unk_20.unk_2458 == 21) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(0x34) == 0) &&
        (func_ov030_020aebb0(0x44) == 0))
    {
        *arg0 = 1;
        return &data_ov030_020e2498;
    }

    if ((data_02071cf0.unk_20.unk_2458 == 22) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) &&
        (func_ov030_020aebb0(0x42) == 0))
    {
        *arg0 = 1;
        return &data_ov030_020e2c78;
    }

    if (data_02071cf0.unk_20.unk_2458 == 23) {
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) && (func_ov030_020aebb0(76) == 0)) {
            *arg0 = 2;
            return &data_ov030_020e2f88;
        } else {
            *arg0 = 1;
            return &data_ov030_020e2ce8;
        }
    }

    if ((data_02071cf0.unk_20.unk_2458 == 41) && (func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(52) == 0) &&
        (func_ov030_020aebb0(0x4D) == 0))
    {
        *arg0 = 1;
        return &data_ov030_020e2c94;
    }

    if (data_02071cf0.unk_20.unk_2458 == 43) {
        if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(47) == 0) && (func_ov030_020aebb0(109) == 0)) {
            *arg0 = 1;
            return &data_ov030_020e2444;
        } else if ((func_ov030_020aebb0(4) != 0) && (func_ov030_020aebb0(47) == 0) && (func_ov030_020aebb0(109) != 0)) {
            *arg0 = 1;
            return &data_ov030_020e2930;
        }
    }

    if (data_02071cf0.unk_20.unk_2458 == 24) {
        *arg0 = 1;
        return &data_ov030_020e2b0c;
    } else if (data_02071cf0.unk_20.unk_2458 == 25) {
        *arg0 = 1;
        return &data_ov030_020e2b44;
    } else if (data_02071cf0.unk_20.unk_2458 == 26) {
        *arg0 = 1;
        return &data_ov030_020e2b28;
    } else if (data_02071cf0.unk_20.unk_2458 == 27) {
        *arg0 = 1;
        return &data_ov030_020e29a0;
    } else if (data_02071cf0.unk_20.unk_2458 == 28) {
        *arg0 = 1;
        return &data_ov030_020e2b98;
    } else if (data_02071cf0.unk_20.unk_2458 == 29) {
        *arg0 = 1;
        return &data_ov030_020e2620;
    } else if (data_02071cf0.unk_20.unk_2458 == 30) {
        *arg0 = 1;
        return &data_ov030_020e2754;
    } else if (data_02071cf0.unk_20.unk_2458 == 31) {
        *arg0 = 1;
        return &data_ov030_020e2a10;
    } else if (data_02071cf0.unk_20.unk_2458 == 32) {
        *arg0 = 1;
        return &data_ov030_020e239c;
    } else if (data_02071cf0.unk_20.unk_2458 == 33) {
        *arg0 = 1;
        return &data_ov030_020e2508;
    } else if (data_02071cf0.unk_20.unk_2458 == 34) {
        *arg0 = 1;
        return &data_ov030_020e2968;
    } else if (data_02071cf0.unk_20.unk_2458 == 35) {
        *arg0 = 1;
        return &data_ov030_020e2914;
    } else if (data_02071cf0.unk_20.unk_2458 == 36) {
        *arg0 = 1;
        return &data_ov030_020e2540;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba4b4(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 4;
        return &data_ov030_020e4194;
    } else if (data_02071cf0.unk_20.unk_2458 == 9) {
        *arg0 = 1;
        return &data_ov030_020e2d3c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba504(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 2;
        return &data_ov030_020e2e70;
    }
    *arg0 = 0;
    return NULL;
}

static s32* func_ov030_020ba538(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 21) {
        *arg0 = 8;
        return &data_ov030_020e51b0;
    } else if (data_02071cf0.unk_20.unk_2458 == 22) {
        *arg0 = 4;
        return &data_ov030_020e3da4;
    } else if (data_02071cf0.unk_20.unk_2458 == 23) {
        *arg0 = 4;
        return &data_ov030_020e3e14;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba5a4(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 0x12) {
        *arg0 = 1;
        return &data_ov030_020e2d90;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba5d8(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 24) {
        *arg0 = 8;
        return &data_ov030_020e5290;
    } else if (data_02071cf0.unk_20.unk_2458 == 25) {
        *arg0 = 10;
        return &data_ov030_020e5760;
    } else if (data_02071cf0.unk_20.unk_2458 == 26) {
        *arg0 = 7;
        return &data_ov030_020e4f48;
    } else if (data_02071cf0.unk_20.unk_2458 == 27) {
        *arg0 = 14;
        return &data_ov030_020e59ac;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba660(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 0x12) {
        *arg0 = 2;
        return &data_ov030_020e3148;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba694(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 0x12) {
        *arg0 = 1;
        return &data_ov030_020e2bb4;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba6c8(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 0x12) {
        *arg0 = 1;
        return &data_ov030_020e26ac;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba6fc(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 0x12) {
        *arg0 = 1;
        return &data_ov030_020e29bc;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba730(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 1) {
        *arg0 = 1;
        return &data_ov030_020e2984;
    } else if (data_02071cf0.unk_20.unk_2458 == 11) {
        *arg0 = 1;
        return &data_ov030_020e255c;
    } else if (data_02071cf0.unk_20.unk_2458 == 12) {
        *arg0 = 1;
        return &data_ov030_020e2dac;
    } else if (data_02071cf0.unk_20.unk_2458 == 13) {
        *arg0 = 1;
        return &data_ov030_020e2850;
    } else if (data_02071cf0.unk_20.unk_2458 == 15) {
        *arg0 = 1;
        return &data_ov030_020e25cc;
    } else if (data_02071cf0.unk_20.unk_2458 == 14) {
        *arg0 = 1;
        return &data_ov030_020e27fc;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba7f0(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2c24;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba824(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2dc8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba858(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2a48;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba88c(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e29d8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba8c0(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2cb0;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba8f4(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e23b8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba928(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e271c;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba95c(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e2ccc;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba990(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e25e8;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba9c4(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
        *arg0 = 1;
        return &data_ov030_020e24b4;
    } else {
        *arg0 = 0;
        return NULL;
    }
}

static s32* func_ov030_020ba9f8(s32* arg0) {
    if (data_02071cf0.unk_20.unk_2458 == 18) {
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
    if (data_02071cf0.unk_20.unk_1AB0 >= 44) {
        data_02071cf0.unk_20.unk_1AB0 = 0;
    }
    return data_ov030_020e4e98[data_02071cf0.unk_20.unk_1AB0](arg0);
}
