#include "Combat/Noise/Boss03.h"
#include "Combat/Core/Combat.h"
#include "EasyFade.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/Math/Random.h"
#include "Engine/Resources/ResourceMgr.h"
#include "SpriteMgr.h"

#include <NitroSDK/fx.h>
#include <NitroSDK/mi/cpumem.h>

extern s32 func_02047e84(u16);

// void  func_ov003_02082730(void*, s32);                                                /* extern */
// void  func_ov003_02082750(void**, s32);                                               /* extern */
// void  func_ov003_02082b0c(void*);                                                     /* extern */
// void  func_ov003_02082b64(void*);                                                     /* extern */
// void  func_ov003_02082cc4(void**);                                                    /* extern */
// u32   func_ov003_02082f2c(void*);                                                     /* extern */
// void  func_ov003_02083074(s32);                                                       /* extern */
// void  func_ov003_02083118(void**, s32);                                               /* extern */
// void  func_ov003_020831e4(s32, s32);                                                  /* extern */
// void  func_ov003_02084348(s32, s16*, s16*, s32, ...);                                 /* extern */
// s32   func_ov003_020843b0(s32, s32);                                                  /* extern */
// void  func_ov003_02084694(void**, s32);                                               /* extern */
// void  func_ov003_02087ed8(s32);                                                       /* extern */
// s32   func_ov003_020c37f8(void**);                                                    /* extern */
// s32   func_ov003_020c3bf0();                                                          /* extern */
// s32   func_ov003_020c3c28();                                                          /* extern */
// void  func_ov003_020c3efc(void*, s32);                                                /* extern */
// void  func_ov003_020c4520(void**);                                                    /* extern */
// void  func_ov003_020c4668(void**);                                                    /* extern */
// void  func_ov003_020c48b0();                                                          /* extern */
// void  func_ov003_020c492c(s32);                                                       /* extern */
// void  func_ov003_020c4b5c(void**);                                                    /* extern */
// void  func_ov003_020c4fc8(void*);                                                     /* extern */
// s32   func_ov003_020c5b2c(u16, void*, s32, s32, s32);                                 /* extern */
// void  func_ov003_020cb128(void*, s32, u32);                                           /* extern */
// void  func_ov003_020cb130(void*, void*, s32, s32, void*);                             /* extern */
// void  func_ov003_020cb150(void*);                                                     /* extern */
// void  func_ov003_020cb194(void*);                                                     /* extern */
// void  func_ov003_020cb200(void*, u16);                                                /* extern */
// void  func_ov003_020cb304(void*, u16);                                                /* extern */
// s32   func_ov003_020cb32c(void*, s32);                                                /* extern */
// u32   func_ov003_020cb368();                                                          /* extern */
// void* func_ov003_020cb42c(s32, s32, s32);                                             /* extern */
// s32   func_ov003_020cb648(void*, s32);                                                /* extern */
// s32   func_ov003_020cb764(s32);                                                       /* extern */
// s16   func_ov003_020cb910(void**, void**, void**, s32, s32, s32, s32, s32, s32, s32); /* extern */
// s32   func_ov003_020cba54(s32, s32, s32, s32, s32, s32);                              /* extern */
// void  func_ov003_020cc38c(s32, s32, s32, s32);                                        /* extern */
// void* func_ov003_020cc750(s32);                                                       /* extern */
void* func_ov003_0208a114(u16); /* extern */
// s32   func_ov003_0208a1e8(s32, void*, s32);                                           /* extern */
// void  func_ov003_0208afcc(s32);                                                       /* extern */
void* func_ov003_020c3c6c(void*); /* extern */
// s32   func_ov003_020c59a0(void*);                                                     /* extern */
// void  func_ov003_020cb264(void*, void*);                                              /* extern */
// s32   func_ov003_020cb348(void*, s32);                                                /* extern */
// s32   func_ov003_020cb3c4(s32, s32);                                                  /* extern */
// void  func_ov003_02082724(void*, s16, s16);                                           /* extern */
// void  func_ov003_02082f1c(void*, s32);                                                /* extern */
// void  func_ov003_02082fc0();                                                          /* extern */
// s32   func_ov003_02083ca0(void*, void*, s32, s32);                                    /* extern */
// s32   func_ov003_020c3d28(s32, s32, s32*, s32*, s32*, s32*);                          /* extern */
// void  func_ov003_020c495c(void*);                                                     /* extern */
// void  func_ov003_020c4a08(void*);                                                     /* extern */
// s32   func_ov003_020cb55c(void*);                                                     /* extern */
// void  func_ov003_02082a04(u32, void**, void*, void*, s32, s32, s32);                  /* extern */
// s32   func_ov003_020cb744(s32, ...);                                                  /* extern */
// s32   func_ov003_020cb7a4(s32);                                                       /* extern */
// void  func_ov003_020cbafc(void*, void*, void*, s32, s32, s32, s32, s32, s32, s32);    /* extern */
// s32   func_ov003_020cc5d0(void**);                                                    /* extern */
// void  func_ov003_02082a90(s32, void*, void*, s32, s32, s32, s32, s32);                /* extern */
// void  func_ov003_02083a2c(s32, s32, s32, s32, s32, s32);                              /* extern */
// void  func_ov003_02083ab0(s32, s32, s32, s32);                                        /* extern */
// s16   func_ov003_020843ec(s32, s32, s32);                                             /* extern */
// void  func_ov003_02084634(void*, s32, s32, s32);                                      /* extern */
// void  func_ov003_02084678(s32);                                                       /* extern */
// void  func_ov003_020846ac(void*, s32, s32, s32);                                      /* extern */
// s32   func_ov003_020846f0(s32);                                                       /* extern */
// void  func_ov003_020846fc(s32, s32);                                                  /* extern */
// void* func_ov003_0208495c(s32);                                                       /* extern */
// void* func_ov003_02084984(void*);                                                     /* extern */
// s32   func_ov003_020865a0(u16, void*, s32, void*);                                    /* extern */
// void  func_ov003_0208a128(s32, void*, void*, s32);                                    /* extern */
// void  func_ov003_0208b518(s32, void*, s16);                                           /* extern */
// void  func_ov003_0209b564(s32, s16, void*, s32);                                      /* extern */
// void  func_ov003_020c3acc(s32, s32, s32);                                             /* extern */
// void  func_ov003_020c427c();                                                          /* extern */
// s32   func_ov003_020c42ec(void*);                                                     /* extern */
// s32   func_ov003_020c4348(void*);                                                     /* extern */
// void  func_ov003_020c44ac(void*);                                                     /* extern */
// void  func_ov003_020c4628(void*);                                                     /* extern */
// void  func_ov003_020c4b1c(void*);                                                     /* extern */
// s32   func_ov003_020c5bfc(void*);                                                     /* extern */
// s32   func_ov003_020c62c4(void*, s32);                                                /* extern */
// s32   func_ov003_020c65cc(void*, s32);                                                /* extern */
// void* func_ov003_020c72b4(void*, s32, s32);                                           /* extern */
// void  func_ov003_020d3b6c(void*, void*, s32, s32, s32, s32);                          /* extern */
// void  func_ov003_020d3bec(void*, void*, void*, void*, void*);                         /* extern */
// void  func_ov003_020d3c2c(void*, void*);                                              /* extern */
// void  func_ov003_020d3c98(void*, void*);                                              /* extern */
// void  func_ov003_020d3cac(void*);                                                     /* extern */
// void  func_ov003_020d3cf8(void*);                                                     /* extern */
// void  func_ov003_0208722c();                                                          /* extern */
// void  func_ov003_02087fe0(s32, void**, s32);                                          /* extern */
// void  func_ov003_02088064(s32, s32, s32);                                             /* extern */
// void  func_ov003_020880e4(s32, void**, void**);                                       /* extern */
// void  func_ov003_020a4390(s32, s32);                                                  /* extern */
// void  func_ov003_020bb48c(s32, u8, s32, s32, s32, s32, s32);                          /* extern */
// void* func_ov003_020c3c88(void*);                                                     /* extern */
// void  func_ov003_020c49c8(void**);                                                    /* extern */
// void  func_ov003_020c4ff4();                                                          /* extern */
// s32   func_ov003_020c6f40();                                                          /* extern */
// s32   func_ov003_020c6f90();                                                          /* extern */
// s32   func_ov003_0208a164(s32, void*, s32, s32, s32);                                 /* extern */
// void  func_ov003_020c3e88(void**, s32);                                               /* extern */
// void  func_ov003_020c4988(void**);                                                    /* extern */
// void  func_ov003_020c4a34(s32);                                                       /* extern */
// s32   func_ov003_020c5e98();                                                          /* extern */
// s32   func_ov003_020c6f68();                                                          /* extern */
// s32   func_ov003_020c703c();                                                          /* extern */
// void  func_ov003_020826c8(void*, u16);                                                /* extern */
void* func_ov003_02085dc8(s32); /* extern */
void* func_ov003_02085e78(s32); /* extern */
void* func_ov003_02085ec0(s32); /* extern */
void* func_ov003_02086014(s32); /* extern */
// void  func_ov003_020c4ab4(void*, s32);                                                /* extern */
// s32   func_ov003_020c6230(void*);                                                     /* extern */
// s32   func_ov003_020c6b8c(void*, s32);                                                /* extern */
// s32   func_ov003_020c7070();                                                          /* extern */
// void  func_ov003_020cb520(void*, s32);                                                /* extern */
// void  func_ov003_020cb578(void*, s32);                                                /* extern */
// void  func_ov003_020cb5b0(void*, s32, s32);                                           /* extern */
// void  func_ov003_020cbcb4(s32*, s32*, u16, s32, s32);                                 /* extern */
// s32   func_ov003_020cc300(void*, s32);                                                /* extern */
// s32   func_ov003_020cc354(void*);                                                     /* extern */
// u32   func_ov003_020cc430(void*, s32, s32, s32);                                      /* extern */
// void  func_ov003_020cc784(void*, s32);                                                /* extern */
// s32   func_ov003_020cc7c0(s16, s16, s32);                                             /* extern */
// void  func_ov003_020cc834(void*, void*, void*, s32, s32);                             /* extern */
// void  func_ov003_020cd988(void*, void*, void*, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
//                           s32);                                                       /* extern */
// void  func_ov003_020d0518(void*, s32, s32, s32, void*, s32, s32, s32, s32, s32);      /* extern */
// void  func_ov003_020d1dc0(void*, s32, s32, s32, s32);                                 /* extern */
// void  func_ov003_020d23e8(void*, u16, s32, s32, s32, s32);                            /* extern */
// void  func_ov003_020d2bc0(void**);                                                    /* extern */
// s32   func_ov003_0208442c(s32, s32);                                                  /* extern */
// void  func_ov003_0208810c(void*, void*);                                              /* extern */
// void  func_ov003_020c4748();                                                          /* extern */
// void  func_ov003_020c4c5c(void*);                                                     /* extern */

void* func_ov009_020f3448(s32);
void* func_ov009_020f6f88(s32);

void func_ov017_0213ee0c();
void func_ov017_0213efa8(s32*, s32*);
void func_ov017_02140a8c(s32, void*, s32, s32);
void func_ov017_02140b14(u32 arg0, Sprite* arg1, s32 arg2, s32 arg3);
void func_ov017_0213df24(s32 arg0);
void func_ov017_0213e074();
void func_ov017_0213e088(u32 arg0, s32 arg1);
s32* func_ov017_0213e204(s32 arg0);
void func_ov017_0213f394(void* arg0);
void func_ov017_0213f4cc(void* arg0);
s32  func_ov017_0213f630(void* arg1, s32* arg2);
void func_ov017_0212bf64(void* arg0);
void func_ov017_0212c018(void* arg0);
void func_ov017_0212c6c0(void* arg0);
void func_ov017_0212c758(void* arg0);
void func_ov017_0212ce90(void* arg0);
void func_ov017_0212d310(void* arg0);
void func_ov017_0212d730();
void func_ov017_0212d8e0(void* arg0);
void func_ov017_0212db48(void* arg0);
void func_ov017_0212df1c(void* arg0);
void func_ov017_0212e21c(void* arg0, void (*arg1)(void*));
void func_ov017_0212e240(void* arg0);
void func_ov017_0212e36c(void* arg0);
void func_ov017_0212e380(void* arg0);
void func_ov017_0212e5c4(void* arg0);
void func_ov017_0212e5ec(void* arg0);
void func_ov017_0212e954(void* arg0);
void func_ov017_0212ec84(void* arg0);
void func_ov017_0212f064(void* arg0);
void func_ov017_0212f08c(void* arg0);
void func_ov017_0212f3ac(void* arg0);
void func_ov017_0212f3c8(void* arg0);
void func_ov017_0212f674(void* arg0);
void func_ov017_0212f784(void* arg0);
void func_ov017_0212fd18(void* arg0);
void func_ov017_0212c4e0(void* arg0);
void func_ov017_0212c508(void* arg0);
s32  func_ov017_0213731c(void*, s32);
s32  func_ov017_021373d8(void*, s32);
s32  func_ov017_02137494(void*, s32);
s32  func_ov017_02137600(void*, s32);
s16  func_ov017_02137730(void*, s32);
void func_ov017_0213778c(void*, s32, s32, s32);
s32  func_ov017_0213779c(void*);
s32  func_ov017_021377d4(void*);
void func_ov017_02137848(void*, s32);
s32  func_ov017_0213789c(s32, s32);
s32  func_ov017_021378e4();
void func_ov017_0213d6b4(s32, s32);
void func_ov017_0213e17c(u32, s32);
void func_ov017_0213e3a4();
void func_ov017_0213e3b8();
s32  func_ov017_0213eeb8();
s32  func_ov017_0213ef4c();
void func_ov017_0213f0bc(s32);
void func_ov017_0213f15c(s32);
void func_ov017_0213f20c(u8);
s32  func_ov017_0213f5fc(s32);
void func_ov017_02140b9c(void*, s16);
s32  func_ov017_02140c40(void*, s32, s32, s32, s16);
void func_ov017_02130a90();
void func_ov017_02133624(void* arg0);
void func_ov017_02133718(void* arg0);
void func_ov017_02133ad0(void* arg0);
void func_ov017_02133d9c(void* arg0);
void func_ov017_02133f74(void* arg0);
void func_ov017_0213400c(void* arg0);
void func_ov017_02134190(void* arg0);
void func_ov017_02134378(void* arg0);
void func_ov017_02134600(void* arg0);
void func_ov017_021348c0(void* arg0);
void func_ov017_02134d58(void* arg0);
void func_ov017_02135124(void* arg0);
void func_ov017_02135190(void* arg0);
void func_ov017_021352a0(void* arg0);
void func_ov017_021353c4(void* arg0);
s32  func_ov017_02135438(void* arg0, s16 arg1);
s32  func_ov017_021354e4(void* arg0, s16 arg1);
s32  func_ov017_021355a0(void* arg0, s16* arg1, s32 arg2, s16 arg3);
void func_ov017_02135868(void* arg0);
void func_ov017_02135a38(void* arg0);
void func_ov017_02135d60(void* arg0);
s16  func_ov017_02135dac(void* arg0, s32 arg1);
void func_ov017_02135e08(void* arg0, s32 arg1);
void func_ov017_02135e7c(void* arg0, s32 arg1);
s32  func_ov017_02135eb0(void* arg0, s32 arg1);
s16  func_ov017_02135ecc(void* arg0, s32 arg1);
void func_ov017_02135edc(void* arg0, s32 arg1, s16 arg2);
void func_ov017_021360ac(void* arg0);
void func_ov017_021361ac(void* arg0);
void func_ov017_021362b0(void* arg0);
void func_ov017_021363c4(void* arg0);
void func_ov017_021364b8(void* arg0);
void func_ov017_02136d90(void* arg0);
void func_ov017_02136fa0(void* arg0);
s32  func_ov017_02137228(void* arg0, s16 arg1);
s32  func_ov017_0213727c(void* arg0, s32 arg1, s16 arg2);
void func_ov017_021302f0(void*);
void func_ov017_02135eec(void*, void (*)(void*));
s32  func_ov017_0213cfec(void*, s32);
s16  func_ov017_0213d13c(void*, s32);
void func_ov017_0213d198(void*, s32, s32, s32);
s32  func_ov017_0213d1a8(void*);
void func_ov017_0213d1e0(void*, s32);
s32  func_ov017_0213d27c(s32, s32);
s32  func_ov017_0213d298(void*);
s32  func_ov017_0213d2f4(void*);
void func_ov017_0213e17c(u32, s32);
void func_ov017_0213e3a4();
void func_ov017_0213e3b8();
s32  func_ov017_0213eeb8();
s32  func_ov017_0213ef4c();
void func_ov017_0213f20c(u8);
void func_ov017_02140cbc(void*);
void func_ov017_02137c18(void* arg0, void (*arg1)(void*));
void func_ov017_02137cd4(void* arg0);
void func_ov017_02137d34(void* arg0);
void func_ov017_02137d64(void* arg0);
void func_ov017_02137edc(void* arg0);
void func_ov017_02137fd0(void* arg0);
void func_ov017_02138048(void* arg0);
void func_ov017_021380dc(void* arg0);
void func_ov017_021389d0(void* arg0);
void func_ov017_02138b70(void* arg0);
void func_ov017_02138bfc(void* arg0);
void func_ov017_02138ce0(void* arg0);
void func_ov017_02138e44(void* arg0);
s32  func_ov017_02138eb4(void* arg0, s32 arg1, s16 arg2);
s32  func_ov017_02138fdc(void* arg0, s16 arg1);
s32  func_ov017_0213903c(void* arg0, s16 arg1);
s32  func_ov017_021390e4(void* arg0, s16 arg1);
s32  func_ov017_02139194(void* arg0, s16 arg1);
s32  func_ov017_021392b8(void* arg0, s32 arg1);
s16  func_ov017_02139408(void* arg0, s32 arg1);
void func_ov017_02139464(void* arg0, s32 arg1, s32 arg2, s32 arg3);
s32  func_ov017_02139474(void* arg0);
void func_ov017_021394ac(void* arg0, s32 arg1);
s32  func_ov017_02139534(void* arg0, s32 arg1);
s32  func_ov017_02139550(void* arg0);
void func_ov017_02139ae0(void* arg0);
void func_ov017_02139b40(void* arg0);
void func_ov017_02139c54(void* arg0);
void func_ov017_02139d14(void* arg0);
void func_ov017_02139de0(void* arg0);
void func_ov017_02139f1c(void* arg0);
void func_ov017_0213a01c(void* arg0);
void func_ov017_0213a458(void* arg0);
void func_ov017_0213a674(void* arg0);
void func_ov017_0213a728(void* arg0);
s32  func_ov017_0213a7a0(void* arg0, s16 arg1);
s32  func_ov017_0213a818(void* arg0, s16 arg1);
s32  func_ov017_0213a8e0(void* arg0, s16 arg1);
s32  func_ov017_0213a9cc(void* arg0, s16 arg1);
s32  func_ov017_0213ab24(void* arg0, s32 arg1);
s16  func_ov017_0213ac5c(void* arg0, s32 arg1);
void func_ov017_0213acc8(void* arg0, s32 arg1);
s32  func_ov017_0213ad4c(void* arg0, s32 arg1);
void func_ov017_0213ad7c(void* arg0);
s32  func_ov017_0213ada0();
void func_ov017_0213b594(void* arg0);
void func_ov017_0213b6d0(void* arg0);
void func_ov017_0213b7c4(void* arg0);
void func_ov017_0213b83c(void* arg0);
void func_ov017_0213b8cc(void* arg0);
void func_ov017_0213c1d4(void* arg0);
void func_ov017_0213c60c(void* arg0);
s32  func_ov017_0213cbcc(void* arg0, s32 arg1, s16 arg2);
s32  func_ov017_0213cd00(void* arg0, s16 arg1);
s32  func_ov017_0213cd60(void* arg0, s16 arg1);
s32  func_ov017_0213ce14(void* arg0, s16 arg1);
s32  func_ov017_0213ced0(void* arg0, s16 arg1);

extern SubroutineArgs data_ov017_021427b8;
extern s32            data_ov017_021435bc;
extern s32            data_ov017_021435be;
extern s32            data_ov017_02143614;
extern s32            data_ov017_02143660;
extern s32            data_ov017_021436ac;
extern s32            data_ov017_02143a24;
extern s32            data_ov017_02143a38;
extern s32            data_ov017_02143a40;
extern s32            data_ov017_02143a48;
extern s32            data_ov017_02143a50;
extern s32            data_ov017_02143a58;
extern s32            data_ov017_02143a60;
void                  func_ov017_0213b534(void* arg0);
void                  func_ov017_0213c7c0(void* arg0);
void                  func_ov017_0213c930(void* arg0);
void                  func_ov017_0213ca84(void* arg0);
extern s32            data_ov009_02123944;
extern s32            data_ov009_02124174;
extern s32            data_ov009_02124620;
extern s32            data_ov017_02143774;
extern s32            data_ov017_02143790;
extern s32            data_ov017_021437ac;
extern s32            data_ov017_02143a64;
extern s32            data_0205e4e0;
extern s32            data_ov017_021428fc;
extern s32            data_ov017_02142900;
extern s32            data_ov017_02142904;
extern SubroutineArgs data_ov017_02142954;
extern s32            data_ov017_02143810;
extern s32            data_ov017_02143814;

extern const s32 data_ov017_02142be4;
extern const s32 data_ov017_02142ae4;
extern const s32 data_ov017_021429e4;
extern const s32 data_ov017_02142aa4;
extern const s32 data_ov017_021429ac;

extern s32   data_ov017_02143848;
extern s32   data_ov017_0214385c;
extern s32   data_ov017_02143870;
extern s32   data_ov017_021438a8;
static void* data_ov017_021438e0[0xE] = {
    &data_ov017_02142be4,
    &data_ov017_02142ae4,
    &data_ov017_021429e4,
    &data_ov017_02142aa4,
    &data_ov017_021429ac,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

extern const s32 data_ov017_0214299c; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142b9c; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_021429cc; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_021429bc; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142b64; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_0214298c; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142ac4; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142994; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142a84; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142c34; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142984; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_021429fc; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142b04; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142b34; /* unable to generate initializer: unknown type; const */

static void* data_ov017_02143918[0xE] = {
    &data_ov017_0214299c, &data_ov017_02142b9c, &data_ov017_021429cc, &data_ov017_021429bc, &data_ov017_02142b64,
    &data_ov017_0214298c, &data_ov017_02142ac4, &data_ov017_02142994, &data_ov017_02142a84, &data_ov017_02142c34,
    &data_ov017_02142984, &data_ov017_021429fc, &data_ov017_02142b04, &data_ov017_02142b34,
};
extern const s32 data_ov017_02142058; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142970; /* unable to generate initializer: unknown type; const */
static u16       data_ov017_02142a14[0xE] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};                /* const */
static u16       data_ov017_02142a30[0xE] = {4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0};                /* const */
static u16       data_ov017_02142a4c[0xE] = {0x14, 0x40, 0x11, 0xA0, 0xC0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; /* const */
static u16       data_ov017_02142a68[0xE] = {
    0x24, 0x4C, 0x2A, 0x28, 0x40, 0x29, 0x29, 0xA4, 0xC0, 0xE8, 0xC0, 0xE0, 0x138, 0x170,
}; /* const */
extern const s32 data_ov017_02142cc8; /* unable to generate initializer: unknown type; const */
extern const s32 data_ov017_02142ccc; /* unable to generate initializer: unknown type; const */

extern s32              data_ov017_021426f8;
extern s32              data_ov017_021426fa;
extern SubroutineArgs   data_ov017_021426fc;
extern SubroutineArgs   data_ov017_02142754;
extern s32              data_ov017_02142770;
extern s32              data_ov017_02142772;
extern SubroutineArgs   data_ov017_02142780;
extern SubroutineArgs   data_ov017_0214279c;
extern s32              data_ov017_0214357c;
extern s32              data_ov017_0214357e;
void                    func_ov017_02136024(void* arg0);
void                    func_ov017_02136084(void* arg0);
void                    func_ov017_02137020(void* arg0);
void                    func_ov017_02137110(void* arg0);
extern Boss03DataStruct data_ov017_02142040;
extern s32              data_ov017_021425b8;
extern s32              data_ov017_021425c8;
extern s32              data_ov017_021425d8;
extern s32              data_ov017_021425e8;
extern s32              data_ov017_021425f8;
extern s32              data_ov017_02142608;
extern SubroutineArgs   data_ov017_02142624;
extern SubroutineArgs   data_ov017_02142640;

extern SubroutineArgs data_ov017_021426d0;
extern s32            data_ov017_021426e0;
extern s32            data_ov017_021426e2;

/////////////////////// KEEP FORWARD DECLARED ///////////////////////
s32  func_ov017_0213064c(void* arg1);
s32  func_ov017_0213066c(s32 arg0, void* arg1, void (*arg2)(void*));
void func_ov017_0213ed98(s32, void*);
void func_ov017_021337e4(void* arg0);

void BtlBoss03_NoisAttk_CreateTask(s32* arg0);
void BtlBoss03_FailPly_CreateTask(s32, s32);
void BtlBoss03_SeqEntry_CreateTask(void*);
void BtlBoss03_ShotEff_CreateTask(s32);
void BtlBoss03_NoisAppr_CreateTask(u16 arg0, s32 arg1, s32 arg2, void* arg3, void* arg4);
void BtlBoss03_KindanMgr_CreateTask();
s32  BtlBoss03_AppearBlackBall_CreateTask(s32, s32, s32, s32, s32, s32, s32);
s32  BtlBoss03_EffRush_CreateTask(void*);
s32  BtlBoss03_EffSign_CreateTask(void*);
void BtlBoss03_EffPosture_CreateTask(void*);
void BtlBoss03_EffCatch_CreateTask(void*);
/////////////////////// KEEP FORWARD DECLARED ///////////////////////

static s32 Boss03_BeginFadeOut(s32 unused) {
    EasyFade_FadeBothDisplays(FADER_SMOOTH, 16, 0x1000);
    return 0;
}

static s32 func_ov017_021302b8(s32 arg0) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if ((arg0 > 0) && (EasyFade_IsFading() == 0)) {
        return 0;
    }
    return 1;
}

static void func_ov017_021302f0(void* arg0) {
    SubroutineArgs subroutine_arg0;
    s32            spC;
    s32            sp8;
    s32            temp_r4;
    s32            temp_r5;
    s32            var_r8;
    u16            temp_r3;
    u16            var_r2;
    u32            temp_r0;
    u32            temp_r1;
    u32            temp_r1_2;

    sp8     = data_ov017_02142040.unk20;
    spC     = data_ov017_02142040.unk24;
    temp_r4 = func_ov003_020cb744(1, data_ov017_02142040.unk28, data_ov017_02142040.unk2C);
    temp_r5 = func_ov003_020cb7a4(1);
    if ((s32)(((Boss03*)arg0)->unk28 - (temp_r4 >> 1)) >= 0) {
        var_r2 = 1;
    } else {
        var_r2 = 0;
    }
    if ((s32)((Boss03*)arg0)->unk30 <= -0x20000) {
        var_r8 = 1;
    } else {
        var_r8 = 0;
    }
    temp_r0                = ((u32)(var_r8 << 0x10) >> 0xF) + var_r2 + RNG_Next(3) + 1;
    temp_r1                = temp_r0 >> 0x1F;
    temp_r3                = temp_r1 + ROTATE_RIGHT((temp_r0 << 0x1E) - temp_r1, 0x1E);
    temp_r1_2              = temp_r3 >> 0x1F;
    ((Boss03*)arg0)->unk28 = (s32)((&sp8)[temp_r1_2 + ROTATE_RIGHT((temp_r3 << 0x1F) - temp_r1_2, 0x1F)] + (temp_r4 >> 1));
    ((Boss03*)arg0)->unk2C = (s32)(temp_r5 >> 1);
    ((Boss03*)arg0)->unk30 = (s32)(&subroutine_arg0)[temp_r3 >> 1].unk0;
}

static void func_ov017_021303d4(void* arg0) {
    BOOL var_r0 = func_ov003_020c37f8(arg0 + 0x84) != 0;

    func_ov003_020cb498(var_r0, 0x3C, func_ov017_0213066c, &BtlBoss03_NoisAttk_CreateTask);
}

static void* func_ov017_02130408(s32 arg0) {
    for (s32 i = 0; i < 4; i++) {
        void* result = func_ov003_020cb42c(arg0, i + 43, 0);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

static void func_ov017_02130444(void* arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4, void* arg5) {
    s16 sp20;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 spC;
    s32 temp_r7;
    s32 var_r11;
    s32 var_r11_2;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r5;
    s32 var_r5_2;

    s32 var_r6 = (arg5 != NULL) ? ((Boss03*)arg5)->unk30 : -0x80000;

    if (arg2 != 0) {
        var_r11 = 0;
        var_r4  = 0;
        var_r5  = 0;
        if (arg0 != NULL) {
            ((SubroutineArgs*)&sp14)->unk0 = ((Boss03*)arg0)->unk0;
            ((SubroutineArgs*)&sp14)->unk4 = (s32)((Boss03*)arg0)->unk4;
            ((SubroutineArgs*)&sp14)->unk8 = (s32)((Boss03*)arg0)->unk8;
            ((SubroutineArgs*)&sp14)->unkC = (s32)((Boss03*)arg0)->unkC;
        } else if ((arg4 == 0) && (arg5 != NULL)) {
            sp14    = ((Boss03*)arg5)->unk28;
            sp18    = ((Boss03*)arg5)->unk2C;
            var_r11 = ((Boss03*)arg5)->unk40;
            var_r4  = ((Boss03*)arg5)->unk44;
            var_r5  = ((Boss03*)arg5)->unk38;
        } else {
            sp14 = RNG_Next((func_ov003_020cb744(0) >> 0xC) + 1) << 0xC;
            sp18 = RNG_Next((func_ov003_020cb7a4(0) >> 0xC) + 1) << 0xC;
        }
        sp20 = 0;
        sp10 = BtlBoss03_AppearBlackBall_CreateTask(0, sp14, sp18, var_r6, var_r11, var_r4, var_r5);
    }

    if (arg3 != 0) {
        var_r11_2 = 0;
        var_r4_2  = 0;
        var_r5_2  = 0;
        if (arg1 != NULL) {
            ((SubroutineArgs*)&sp14)->unk0 = ((Boss03*)arg1)->unk0;
            ((SubroutineArgs*)&sp14)->unk4 = (s32)((Boss03*)arg1)->unk4;
            ((SubroutineArgs*)&sp14)->unk8 = (s32)((Boss03*)arg1)->unk8;
            ((SubroutineArgs*)&sp14)->unkC = (s32)((Boss03*)arg1)->unkC;
        } else if ((arg4 == 1) && (arg5 != NULL)) {
            sp14      = ((Boss03*)arg5)->unk28;
            sp18      = ((Boss03*)arg5)->unk2C;
            var_r11_2 = ((Boss03*)arg5)->unk40;
            var_r4_2  = ((Boss03*)arg5)->unk44;
            var_r5_2  = ((Boss03*)arg5)->unk38;
        } else {
            temp_r7 = RNG_Next(0x41) - 0x20;
            sp14    = func_ov003_020cb764(1) + (temp_r7 << 0xC);
            if (RNG_Next(0x64) >= 0x32U) {
                sp14 += 0x40000;
            } else {
                sp14 -= 0x40000;
            }
            sp18 = RNG_Next((func_ov003_020cb7a4(1) >> 0xC) + 1) << 0xC;
        }
        sp20 = 1;
        spC  = BtlBoss03_AppearBlackBall_CreateTask(1, sp14, sp18, var_r6, var_r11_2, var_r4_2, var_r5_2);
    }

    if ((arg2 != 0) && (arg3 != 0)) {
        func_ov017_0213d6b4(sp10, spC);
    }
}

static s32 func_ov017_02130630(s32 arg0) {
    func_ov003_020cb498(arg0, 0x3C, func_ov017_0213064c, NULL);
}

static s32 func_ov017_0213064c(void* arg1) {
    s32 temp_r0 = ((Boss03*)arg1)->unk7C;

    if ((temp_r0 >= 0x2B) && (temp_r0 <= 0x2E)) {
        return 0;
    }
    return 1;
}

static s32 func_ov017_0213066c(s32 arg0, void* arg1, void (*arg2)(void*)) {
    s32 temp_r0;

    temp_r0 = ((Boss03*)arg1)->unk7C;
    if ((temp_r0 >= 0x2B) && (temp_r0 <= 0x2E)) {
        return 0;
    }
    arg2(arg1);
    return 1;
}

/// MARK: BtlBoss03_00_RG

typedef struct BtlBoss03_00_RG {
    /* 0x000 */ char                    unk_000[0x20];
    /* 0x020 */ s32                     unk_020;
    /* 0x024 */ char                    unk_024[0x28 - 0x24];
    /* 0x028 */ s32                     unk_028;
    /* 0x02C */ s32                     unk_02C;
    /* 0x030 */ s32                     unk_030;
    /* 0x034 */ char                    unk_034[0x54 - 0x34];
    /* 0x054 */ u32                     unk_054;
    /* 0x058 */ char                    unk_058[0x84 - 0x58];
    /* 0x084 */ Sprite                  unk_084;
    /* 0x0C4 */ char                    unk_0C4[0x184 - 0xC4];
    /* 0x184 */ struct BtlBoss03_00_RG* unk_184;
    /* 0x188 */ struct BtlBoss03_00_RG* unk_188;
    /* 0x18C */ char                    unk_18C[0x1C0 - 0x18C];
    /* 0x1C0 */ s16                     unk_1C0;
    /* 0x1C2 */ s16                     unk_1C2;
    /* 0x1C4 */ s16                     unk_1C4;
    /* 0x1C6 */ s16                     unk_1C6;
    /* 0x1C8 */ void (*unk_1C8)(struct BtlBoss03_00_RG*);
    /* 0x1CC */ s32 unk_1CC;
} BtlBoss03_00_RG; // Size: 0x1D4

void func_ov017_02130714(BtlBoss03_00_RG* rg);
void func_ov017_021307c4(BtlBoss03_00_RG* rg);
void func_ov017_0213082c(BtlBoss03_00_RG* rg);
s32  func_ov017_021309d8(BtlBoss03_00_RG* rg, s16 arg1);

s32 BtlBoss03_00_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_00_RG = {"Tsk_BtlBoss03_00_RG", BtlBoss03_00_RG_RunTask, 0x1D4};

void func_ov017_02130698(BtlBoss03_00_RG* rg, void (*arg1)(BtlBoss03_00_RG*)) {
    func_ov003_020c427c();
    rg->unk_1C8 = arg1;
    rg->unk_184 = 0;
    rg->unk_188 = 0;
}

void func_ov017_021306c0(BtlBoss03_00_RG* rg) {
    if (rg->unk_1C0 == 0) {
        data_ov003_020e71b8->unk3D875 = 2;
    }

    if (func_ov003_020c5bfc(rg) == 0) {
        func_ov017_02130698(rg, func_ov017_02130714);
    }
}

void func_ov017_02130714(BtlBoss03_00_RG* rg) {
    void* temp_r4 = rg->unk_188;

    if (rg->unk_1C0 == 0) {
        func_ov003_02082cc4(&rg->unk_084);
        func_ov017_02140a8c(1, &rg->unk_084, 0, 0);
        func_ov003_020824a0(&rg->unk_084, 1U, 0U);
    }
    if (((Boss03*)temp_r4)->unk4 != 0) {
        func_ov017_02130698(rg, func_ov017_0213082c);
        return;
    }
    if ((rg->unk_1C0 >= 10) && (func_ov017_0213ef4c() > 0)) {
        func_ov017_02130698(rg, func_ov017_021307c4);
    } else if (rg->unk_1C0 < 10) {
        rg->unk_1C0++;
    }
}

void func_ov017_021307c4(BtlBoss03_00_RG* rg) {
    if (rg->unk_1C0 == 0) {
        func_ov003_02087f00(SEIDX_MINAMI_BTL_FB04, func_ov003_020843b0(1, rg->unk_028));
    }

    s32 prev = rg->unk_1C0;
    rg->unk_1C0++;

    if (func_ov017_021309d8(rg, prev) == 0) {
        func_ov017_02130698(rg, func_ov017_02130714);
    }
}

void func_ov017_0213082c(BtlBoss03_00_RG* rg) {
    void* temp_r4 = rg->unk_188;

    switch (rg->unk_1C4) {
        case 0: {
            rg->unk_1C0 = 0;
            rg->unk_1C4 = 1;
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                func_ov003_02087f00(SEIDX_MINAMI_BTL_FB03, func_ov003_020843b0(1, rg->unk_028));
            }
            if (func_ov017_021309d8(rg, rg->unk_1C0) == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 2;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 2: {
            SubroutineArgs subroutine_arg0;

            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                func_ov003_02082cc4(&rg->unk_084);
                func_ov017_02140a8c(1, &rg->unk_084, 1, 0);
                func_ov003_020824a0(&rg->unk_084, 8, 1);
            }
            if (SpriteMgr_IsAnimationFinished(&rg->unk_084) != FALSE) {
                BtlBoss03_SeqEntry_CreateTask(&subroutine_arg0);
                ((Boss03*)temp_r4)->unkC = 1;
                ((Boss03*)temp_r4)->unk8 = 1;
                rg->unk_1CC              = 0;
                rg->unk_054 |= 4;
                func_ov003_02082cc4(&rg->unk_084);
            }
        } break;
    }
}

void func_ov017_02130990(BtlBoss03_00_RG* rg) {
    if (rg->unk_1C0 == 0) {
        rg->unk_1C0++;
    }
    if (func_ov003_020c72b4(rg, 0, 0) == 0) {
        func_ov017_02130698(rg, func_ov017_02130714);
    }
}

s32 func_ov017_021309d8(BtlBoss03_00_RG* rg, s16 arg1) {
    if (arg1 == 0) {
        func_ov003_02082cc4(&rg->unk_084);
        func_ov017_02140a8c(1, &rg->unk_084, 3, 0);
        func_ov003_020824a0(&rg->unk_084, 1, 1);
        func_ov003_020c4ab4(rg, 0);
    }

    if ((rg->unk_084.unk16 == 11) && (rg->unk_084.frameTimer == 1)) {
        while (func_ov017_0213ef4c() != 0) {
            func_ov017_02140b9c(rg, 0);
        }

        func_ov003_02087f00(SEIDX_SE_B005_BADGE_NAGE01, func_ov003_020843b0(1, rg->unk_028));
    }

    return SpriteMgr_IsAnimationFinished(&rg->unk_084) == 0;
}

void func_ov017_02130a90(void) {
    return;
}

s32 BtlBoss03_00_RG_Init(TaskPool* pool, Task* task, void* args) {
    BtlBoss03_00_RG* rg = task->data;

    MI_CpuSet(rg, 0, 0x1D4);
    func_ov003_020c3efc(rg, args);
    func_ov003_020c44ac(rg);
    func_ov003_020c4b1c(rg);
    rg->unk_1CC = 1;
    func_ov003_020cb520(rg, 0);
    rg->unk_028 = (func_ov003_020cb744(1) >> 1);
    rg->unk_02C = (func_ov003_020cb7a4(1) >> 1);
    rg->unk_030 = -0x4A000;
    rg->unk_054 |= 0x30;
    func_ov003_02084694(rg + 0x144, 1);
    func_ov017_02130698(rg, func_ov017_021306c0);
    func_ov017_0213ed98((s32)func_ov017_02130a90, rg);
    return 1;
}

s32 BtlBoss03_00_RG_Update(TaskPool* pool, Task* task, void* args) {
    BtlBoss03_00_RG* rg = task->data;

    rg->unk_188 = rg->unk_184->unk_188;

    if (func_ov003_020c3bf0(rg) != 0) {
        if (rg->unk_020 == 6) {
            func_ov003_02082f2c(rg);
        }
        return 1;
    }

    if (func_ov003_02082f2c(rg) == 6) {
        rg->unk_054 |= 1;
        if (rg->unk_054 == 0) {
            func_ov017_02130698(rg, func_ov017_02130990);
        }
    }
    if (rg->unk_1C8 != NULL) {
        rg->unk_1C8(rg);
    }
    func_ov003_020c4628(rg);
    return rg->unk_1CC;
}

s32 BtlBoss03_00_RG_Render(TaskPool* pool, Task* task, void* args) {
    BtlBoss03_00_RG* rg = task->data;

    func_ov003_020c4878(rg);
    return 1;
}

s32 BtlBoss03_00_RG_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlBoss03_00_RG* rg = task->data;

    func_ov003_020c48fc(rg);
    return 1;
}

s32 BtlBoss03_00_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_02142624 = {
        .initialize = BtlBoss03_00_RG_Init,
        .update     = BtlBoss03_00_RG_Update,
        .render     = BtlBoss03_00_RG_Render,
        .cleanup    = BtlBoss03_00_RG_Destroy,
    };
    return data_ov017_02142624.iter[stage](pool, task, args);
}

/// MARK: BtlBoss03_00_UG

typedef struct BtlBoss03_00_UG {
    /* 0x000 */ char   unk_000[0x54];
    /* 0x054 */ s32    unk_054;
    /* 0x058 */ char   unk_058[0x84 - 0x58];
    /* 0x084 */ Sprite unk_084;
    /* 0x0C4 */ char   unk_0C4[0x144 - 0xC4];
    /* 0x144 */ s32    unk_144;
    /* 0x148 */ char   unk_148[0x188 - 0x148];
    /* 0x188 */ s32    unk_188;
    /* 0x18C */ u16    unk_18C;
    /* 0x18E */ char   unk_18E[0x1CC - 0x18E];
    /* 0x1CC */ s32    unk_1CC;
    /* 0x1D0 */ char   unk_1D0[0x1D4 - 0x1D0];
    /* 0x1D4 */ s32    unk_1D4;
    /* 0x1D8 */ char   unk_1D8[0x1E0 - 0x1D8];
    /* 0x1E0 */ s32    unk_1E0;
} BtlBoss03_00_UG; // Size: 0x1E4

s32 BtlBoss03_00_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_00_UG = {"Tsk_BtlBoss03_00_UG", BtlBoss03_00_UG_RunTask, 0x1E4};

s32 BtlBoss03_00_UG_Init(TaskPool* pool, Task* task, void* args) {
    BtlBoss03_00_UG* ug = task->data;

    MI_CpuSet(ug, 0, 0x1E4);
    func_ov017_0213df24(0);
    func_ov017_0213e088(0, 0);
    func_ov017_0213e088(14, 0);
    func_ov017_0213e088(15, 0);
    func_ov017_0213e088(16, 0);
    BtlBoss03_KindanMgr_CreateTask();
    func_ov017_0213f20c(0);
    func_ov003_020c3efc(ug, args);
    func_ov003_020c4520(ug);
    ug->unk_188 = &ug->unk_1D4;
    ug->unk_18C |= 1;
    ug->unk_054 |= 0x30;
    func_ov003_02084694(&ug->unk_144, 1);
    func_ov003_02082cc4(&ug->unk_084);
    ug->unk_1CC = 1;
}

s32 BtlBoss03_00_UG_Update(TaskPool* pool, Task* task, void* args) {
    BtlBoss03_00_UG* ug = task->data;

    void (*temp_r1_2)(void*);
    s32   temp_r0;
    s32   temp_r1;
    void* temp_r4;
    void* temp_r5;

    temp_r4 = ((Boss03*)task)->unk18;
    temp_r5 = temp_r4 + 0x1D4;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        if (((Boss03*)temp_r4)->unk20 == 6) {
            func_ov003_02082f2c(temp_r4);
        }
        return 1;
    }
    if (((Boss03*)temp_r5)->unk8 != 0) {
        ((Boss03*)temp_r4)->unk1CC = 0;
        ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 4);
    }
    temp_r0 = (s32)func_ov017_02130630(0);
    temp_r1 = ((Boss03*)temp_r4)->unk1D0;
    if (temp_r0 < temp_r1) {
        ((Boss03*)temp_r4)->unk1D4 = (s16)(((Boss03*)temp_r4)->unk1D4 + (temp_r1 - temp_r0));
        ((Boss03*)temp_r5)->unk2   = (s16)(((Boss03*)temp_r5)->unk2 + (((Boss03*)temp_r4)->unk1D0 - temp_r0));
        if ((s32)((Boss03*)temp_r4)->unk1D4 >= 4) {
            ((Boss03*)temp_r5)->unk4 = 1;
        }
    }
    ((Boss03*)temp_r4)->unk1D0 = temp_r0;
    temp_r1_2                  = ((Boss03*)temp_r4)->unk1C8;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    func_ov003_020c4668(temp_r4);
    return ((Boss03*)temp_r4)->unk1CC;
}

s32 BtlBoss03_00_UG_Render(TaskPool* pool, Task* task, void* args) {
    BtlBoss03_00_UG* ug = task->data;

    func_ov003_020c48b0(ug);
    return 1;
}

s32 BtlBoss03_00_UG_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlBoss03_00_UG* ug = task->data;

    func_ov003_020c492c(ug);
    if (ug->unk_1E0 == 0) {
        func_ov017_0213e074();
    }
    return 1;
}

s32 BtlBoss03_00_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_02142640 = {
        .initialize = BtlBoss03_00_UG_Init,
        .update     = BtlBoss03_00_UG_Update,
        .render     = BtlBoss03_00_UG_Render,
        .cleanup    = BtlBoss03_00_UG_Destroy,
    };
    return data_ov017_02142640.iter[stage](pool, task, args);
}

/// MARK: BtlBoss03_01_RG

typedef struct {
    /* 0x00 */ s16   unk_00;
    /* 0x02 */ char  unk_02[0x2];
    /* 0x04 */ s32   unk_04;
    /* 0x08 */ s32   unk_08;
    /* 0x0C */ s32   unk_0C;
    /* 0x10 */ s32   unk_10;
    /* 0x14 */ s16   unk_14;
    /* 0x16 */ char  unk_16[0x18 - 0x16];
    /* 0x18 */ void* unk_18;
    /* 0x1C */ s32   unk_1C;
    /* 0x20 */ s32   unk_20;
    /* 0x24 */ s32   unk_24;
} RGBlob;

typedef struct BtlBoss03_01_RG {
    /* 0x000 */ char                    unk_000[0x20];
    /* 0x020 */ s32                     unk_020;
    /* 0x024 */ s32                     unk_024;
    /* 0x028 */ Vec                     position;
    /* 0x034 */ char                    unk_034[0x054 - 0x034];
    /* 0x054 */ s32                     flags;
    /* 0x058 */ char                    unk_058[0x6C - 0x58];
    /* 0x06C */ s16                     unk_06C;
    /* 0x06E */ s16                     unk_06E;
    /* 0x070 */ char                    unk_070[0x80 - 0x70];
    /* 0x080 */ u16                     unk_080;
    /* 0x082 */ char                    unk_082[0x84 - 0x82];
    /* 0x084 */ Sprite                  sprite;
    /* 0x0C4 */ char                    unk_0C4[0xC8 - 0xC4];
    /* 0x0C8 */ s16                     unk_0C8;
    /* 0x0CA */ char                    unk_0CA[0x144 - 0xCA];
    /* 0x144 */ s32                     unk_144;
    /* 0x148 */ char                    unk_148[0x184 - 0x148];
    /* 0x184 */ struct BtlBoss03_01_RG* unk_184;
    /* 0x188 */ RGBlob*                 unk_188;
    /* 0x18C */ u16                     unk_18C;
    /* 0x18E */ char                    unk_18E[0x1AC - 0x18E];
    /* 0x1AC */ s32                     unk_1AC;
    /* 0x1B0 */ s32                     unk_1B0;
    /* 0x1B4 */ s32                     unk_1B4;
    /* 0x1B8 */ char                    unk_1B8[0x1C0 - 0x1B8];
    /* 0x1C0 */ s16                     unk_1C0;
    /* 0x1C2 */ s16                     unk_1C2;
    /* 0x1C4 */ s16                     unk_1C4;
    /* 0x1C8 */ void (*unk_1C8)(struct BtlBoss03_01_RG*);
    /* 0x1CC */ s32  unk_1CC;
    /* 0x1D0 */ s16  unk_1D0;
    /* 0x1D2 */ s16  unk_1D2[4];
    /* 0x1DA */ s16  unk_1DA;
    /* 0x1DC */ char unk_1DC[0x1E0 - 0x1DC];
    /* 0x1E0 */ s32  unk_1E0;
    /* 0x1E4 */ s16  unk_1E4;
    /* 0x1E6 */ u16  unk_1E6;
    /* 0x1E8 */ char unk_1E8[0x1EC - 0x1E8];
    /* 0x1EC */ s32  unk_1EC[5];
    /* 0x200 */ s32  unk_200[4];
} BtlBoss03_01_RG; // Size: 0x210

typedef struct {
    s32 unk_0;
    s32 unk_4;
} UnkRGTuple;

const UnkRGTuple data_ov017_0214267c[4] = {
    {0x1000, -0x3000},
    {0x1800, -0x3800},
    {0x2000, -0x4000},
    {0x2800, -0x3CCD},
};

const UnkRGTuple data_ov017_0214269c[5] = {
    {-0x1666, -0x3333},
    {-0x1E66, -0x399A},
    {-0x299A, -0x2CCD},
    {-0x3333, -0x3000},
    {-0x4000, -0x2E66},
};

const struct {
    /* 0x0 */ s16 unk_0;
    /* 0x2 */ s16 unk_2;
} data_ov017_0214265c[2][4] = {
    {{1200, 300}, {720, 60},   {0, 0}, {300, 0}},
    { {600, 300},    {0, 0}, {300, 0}, {300, 0}},
};

void func_ov017_02133074(BtlBoss03_01_RG* rg);
void func_ov017_021331fc(BtlBoss03_01_RG* rg);
void func_ov017_021315d4(BtlBoss03_01_RG* rg);
void func_ov017_021318a8(BtlBoss03_01_RG* rg);
void func_ov017_02131cd0(BtlBoss03_01_RG* rg);
void func_ov017_0213104c(BtlBoss03_01_RG* rg);
void func_ov017_02132378(BtlBoss03_01_RG* rg);
void func_ov017_021312fc(BtlBoss03_01_RG* rg);
void func_ov017_021316f0(BtlBoss03_01_RG* rg);
void func_ov017_02131fcc(BtlBoss03_01_RG* rg);
void func_ov017_02131458(BtlBoss03_01_RG* rg);
s32  func_ov017_02132d4c(BtlBoss03_01_RG* rg, s32 arg1, s16 arg2);
s32  func_ov017_02132dc8(BtlBoss03_01_RG* rg, s16* arg1, s32 arg2, s16 arg3);

s32 BtlBoss03_01_RG_WarpDisappear(BtlBoss03_01_RG* rg, s16 arg1);
s32 BtlBoss03_01_RG_WarpAppear(BtlBoss03_01_RG* rg, s16 arg1);

void BtlBoss03_01_RG_ResetCooldownRandom(BtlBoss03_01_RG* rg, s32 cdSlot);
void BtlBoss03_01_RG_TickCooldown(BtlBoss03_01_RG* rg, s32 cdSlot);
BOOL BtlBoss03_01_RG_IsCooldownReady(BtlBoss03_01_RG* rg, s32 cdSlot);
s16  BtlBoss03_01_RG_GetCooldown(BtlBoss03_01_RG* rg, s32 cdSlot);
void BtlBoss03_01_RG_SetCooldown(BtlBoss03_01_RG* rg, s32 cdSlot, s16 cooldown);

s32 BtlBoss03_01_RG_Init(BtlBoss03_01_RG* rg, void* arg1);
s32 BtlBoss03_01_RG_Update(BtlBoss03_01_RG* rg);
s32 BtlBoss03_01_RG_Render(BtlBoss03_01_RG* rg);
s32 BtlBoss03_01_RG_Destroy(BtlBoss03_01_RG* rg);
s32 BtlBoss03_01_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static inline void BtlBoss03_01_RG_PlaySE_BtlFb(BtlBoss03_01_RG* rg, s32 arg1) {
    if ((SndMgr_IsSEPlaying(SEIDX_MINAMI_BTL_FB06) == FALSE) || (SndMgr_IsSEPlaying(SEIDX_MINAMI_BTL_FB01) == FALSE)) {
        SndMgrSeIdx seIdx = (RNG_Next(100) < 50) ? SEIDX_MINAMI_BTL_FB06 : SEIDX_MINAMI_BTL_FB01;
        func_ov003_02087f00(seIdx, func_ov003_020843b0(arg1, rg->position.x));
    }
}

static inline void BtlBoss03_01_RG_PlaySE_BtlAtc(BtlBoss03_01_RG* rg, s32 arg1) {
    SndMgrSeIdx seIdx = (RNG_Next(100) < 50) ? SEIDX_MINAMI_BTL_ATCM02 : SEIDX_MINAMI_BTL_ATCS03;
    func_ov003_02087f00(seIdx, func_ov003_020843b0(arg1, rg->position.x));
}

const TaskHandle Tsk_BtlBoss03_01_RG = {"Tsk_BtlBoss03_01_RG", BtlBoss03_01_RG_RunTask, sizeof(BtlBoss03_01_RG)};

void BtlBoss03_01_RG_SetState(BtlBoss03_01_RG* rg, void (*callback)(BtlBoss03_01_RG*)) {
    func_ov003_020c427c();
    rg->unk_1C8 = callback;
    rg->unk_1C4 = 0;
    rg->unk_1C0 = 0;
}

void func_ov017_02130e7c(BtlBoss03_01_RG* rg) {
    BtlBoss03_01_RG_SetState(rg, func_ov017_021315d4);
    rg->unk_1E4++;
    if (rg->unk_1E4 >= 3) {
        rg->unk_1E4 = 0;
        BtlBoss03_01_RG_ResetCooldownRandom(rg, 3);
    }
}

void func_ov017_02130ec4(BtlBoss03_01_RG* rg) {
    if ((s32)RNG_Next(100) < 60) {
        BtlBoss03_01_RG_SetState(rg, func_ov017_021318a8);
    } else {
        BtlBoss03_01_RG_SetState(rg, func_ov017_02131cd0);
    }
    rg->unk_1E4 = 0;
}

void func_ov017_02130f10(BtlBoss03_01_RG* rg) {
    if (rg->unk_1C0 == 0) {
        rg->unk_1C0++;
        if (rg->unk_080 == 0) {
            data_ov003_020e71b8->unk3D875 = 2;
        }
    }
    if (func_ov003_020c5bfc(rg) == 0) {
        BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
    }
}

void func_ov017_02130f78(BtlBoss03_01_RG* rg) {
    RGBlob* temp = rg->unk_188;

    if (rg->unk_1C0 == 0) {
        rg->unk_1C0++;
        rg->unk_1E6 &= ~2;
    }

    if (temp->unk_10 != 0) {
        rg->unk_1CC = 0;
        rg->flags |= 4;
    } else if (temp->unk_0C != 0) {
        rg->unk_18C &= ~1;
        rg->flags &= ~0x10;
        func_ov003_02084694(&rg->unk_144, 0);
        BtlBoss03_01_RG_SetState(rg, func_ov017_02132378);
    } else if (temp->unk_08 != 0) {
        temp->unk_08 = 0;
        BtlBoss03_01_RG_SetCooldown(rg, 1, temp->unk_14);
        BtlBoss03_01_RG_SetState(rg, func_ov017_021312fc);
    }
}

void func_ov017_0213104c(BtlBoss03_01_RG* rg) {
    RGBlob* temp = rg->unk_188;

    if (rg->unk_1C0 == 0) {
        if (rg->unk_080 == 0) {
            func_ov003_02082cc4(&rg->sprite);
            func_ov017_02140a8c(1, &rg->sprite, 0, 0);
            func_ov003_020824a0(&rg->sprite, 0, 0);
        } else {
            func_ov003_020824a0(&rg->sprite, 0, 0);
        }
        rg->unk_1C2 = func_ov003_020c42ec(rg);
        if (temp != NULL) {
            temp->unk_1C = 1;
        }
        rg->unk_1E6 |= 2;
    }

    if ((rg->unk_080 == 0) && (func_ov017_0213ef4c() > 0)) {
        temp->unk_1C = 0;
        BtlBoss03_01_RG_SetState(rg, func_ov017_021316f0);
        return;
    }

    func_ov003_020c4b1c(rg);

    if (rg->unk_080 == 0) {
        if (((rg->unk_06C * 100) / rg->unk_06E) < 30) {
            temp->unk_1C = 0;
            BtlBoss03_01_RG_SetState(rg, func_ov017_02132378);
            return;
        }

        s32 threshold;
        if ((func_ov003_020cb648(rg, 70) != 0) || (threshold = (rg->unk_06E / 10), temp->unk_00 >= threshold)) {
            temp->unk_1C = 0;
            BtlBoss03_01_RG_SetState(rg, func_ov017_02131fcc);
            return;
        }
        if ((BtlBoss03_01_RG_IsCooldownReady(rg, 1) != 0) && (func_ov017_0213f5fc(1) != 0)) {
            BtlBoss03_01_RG_ResetCooldownRandom(rg, 1);
            temp->unk_1C = 0;
            func_ov017_021303d4(rg);
            return;
        }
    }

    if (rg->unk_080 == 1) {
        if ((temp->unk_24 != 0) && (temp->unk_18 != 0)) {
            if (BtlBoss03_01_RG_IsCooldownReady(rg, 0) != 0) {
                temp->unk_1C = 0;
                temp->unk_18 = 0;
                temp->unk_24 = 0;
                BtlBoss03_01_RG_SetState(rg, func_ov017_02131458);
                func_ov003_020cb578(rg, 0);
                return;
            }
        }
    } else if ((rg->unk_080 == 2) && (temp->unk_24 != 0)) {
        BtlBoss03_01_RG_SetState(rg, func_ov017_02131458);
        func_ov003_020cb578(rg, 0);
        return;
    }

    if (rg->unk_1C0 > rg->unk_1C2) {
        temp->unk_1C = 0;
        if (func_ov003_020c4348(rg) != 0) {
            func_ov017_02130ec4(rg);
        } else if (BtlBoss03_01_RG_IsCooldownReady(rg, 3) != 0) {
            func_ov017_02130e7c(rg);
        } else {
            rg->unk_1C0 = 1;
        }
    } else {
        rg->unk_1C0++;
    }
}

void func_ov017_021312fc(BtlBoss03_01_RG* rg) {
    if (rg->unk_1C0 == 0) {
        func_ov017_021302f0(rg);
        BtlBoss03_01_RG_PlaySE_BtlFb(rg, 1);
        rg->unk_1E6 &= ~2;
    }
    if (BtlBoss03_01_RG_WarpAppear(rg, rg->unk_1C0) == 0) {
        BtlBoss03_01_RG_ResetCooldownRandom(rg, 0);
        BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
    } else {
        rg->unk_1C0++;
    }
}

void func_ov017_021313c8(BtlBoss03_01_RG* rg) {
    RGBlob* temp = rg->unk_188;

    if (rg->unk_1C0 == 0) {
        rg->unk_1E6 &= ~2;
    }
    if (BtlBoss03_01_RG_WarpDisappear(rg, rg->unk_1C0) == 0) {
        rg->flags |= 0x10;
        BtlBoss03_01_RG_SetState(rg, func_ov017_02130f78);
        temp->unk_14 = BtlBoss03_01_RG_GetCooldown(rg, 1);
        temp->unk_04 = 1;
        func_ov003_02082cc4(&rg->sprite);
    } else {
        rg->unk_1C0++;
    }
}

void func_ov017_02131458(BtlBoss03_01_RG* rg) {
    switch (rg->unk_1C4) {
        case 0: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1E6 &= ~2;
            }
            if (BtlBoss03_01_RG_WarpDisappear(rg, rg->unk_1C0) == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 1;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                rg->unk_080 = (rg->unk_080 == 1) ? 2 : 1;
                void* temp  = func_ov003_020c3c6c(rg);
                func_ov003_020826c8(&rg->sprite, ((Boss03*)temp)->unkA);
                ((Boss03*)rg)->unkD4  = (s32)((Boss03*)rg)->unkC0;
                void* temp_r0         = func_ov003_020c3c88(rg);
                ((Boss03*)rg)->unk192 = (s16)((Boss03*)temp_r0)->unk10;
                ((Boss03*)rg)->unk194 = (s16)((Boss03*)temp_r0)->unk12;
                BtlBoss03_01_RG_PlaySE_BtlFb(rg, 1);
            }
            if (BtlBoss03_01_RG_WarpAppear(rg, rg->unk_1C0) == 0) {
                func_ov003_020cb578(rg, 1);
                BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
                BtlBoss03_01_RG_ResetCooldownRandom(rg, 0);
            } else {
                rg->unk_1C0++;
            }
        } break;
    }
}

void func_ov017_021315d4(BtlBoss03_01_RG* rg) {
    switch (rg->unk_1C4) {
        case 0: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1E6 &= ~2;
            }
            if (BtlBoss03_01_RG_WarpDisappear(rg, rg->unk_1C0) == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 1;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                func_ov017_021302f0(rg);
                BtlBoss03_01_RG_PlaySE_BtlFb(rg, 1);
            }
            if (BtlBoss03_01_RG_WarpAppear(rg, rg->unk_1C0) == 0) {
                BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
            } else {
                rg->unk_1C0++;
            }
        } break;
    }
}

void func_ov017_021316f0(BtlBoss03_01_RG* rg) {
    switch (rg->unk_1C4) {
        case 0: {
            if (rg->unk_1C0 == 0) {
                if (func_ov003_020cb648(rg, 70) != 0) {
                    func_ov017_0213f20c(1);
                } else {
                    func_ov017_0213f20c(0);
                }
                func_ov003_020c4ab4(rg, 0);
                BtlBoss03_01_RG_PlaySE_BtlAtc(rg, 1);
                rg->unk_1E6 &= ~2;
            }

            s32 temp_r6 = func_ov017_02132d4c(rg, 0, rg->unk_1C0);
            if (rg->sprite.unk16 == 4 && rg->sprite.frameTimer == 1) {
                while (func_ov017_0213ef4c() > 0) {
                    func_ov017_02140b9c(rg, 0);
                }
                func_ov003_02087f00(SEIDX_SE_B005_BADGE_NAGE01, func_ov003_020843b0(1, rg->position.x));
            }
            if (temp_r6 == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 1;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                func_ov003_02082cc4(&rg->sprite);
                func_ov017_02140a8c(1, &rg->sprite, 0, 0);
                func_ov003_020824a0(&rg->sprite, 0, 0);
                rg->unk_1C2 = 30;
            }
            if (rg->unk_1C0 < rg->unk_1C2) {
                rg->unk_1C0++;
            } else {
                BtlBoss03_01_RG_SetState(rg, func_ov017_021313c8);
            }
        } break;
    }
}

void func_ov017_021318a8(BtlBoss03_01_RG* rg) {
    u16   sp92;
    s16   sp8E;
    s32   sp88;
    s32   sp80;
    s32   sp74;
    s16   sp66;
    s16   sp62;
    s32   sp5C;
    s32   sp54;
    s32   sp48;
    s16   sp3A;
    s16   sp36;
    s32   sp30;
    s32   sp28;
    s32   sp1C;
    void* var_r6;
    s16   temp_r1_4;
    s32   var_r4;
    s32   var_r4_2;
    s32   var_r5;
    u16   temp_r1_3;

    switch (rg->unk_1C4) {
        case 0: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                rg->unk_1E6 |= 2;
            }
            if (func_ov003_020c6230(rg) == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 1;
            }
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                SndMgrSeIdx seIdx = (rg->unk_080 == 0) ? SEIDX_MINAMI_BTL_ATCM04 : SEIDX_MINAMI_H_BTL_ATCS03;
                func_ov003_02087f00(seIdx, func_ov003_020843b0(1, rg->position.x));
                if (rg->unk_080 == 0) {
                    func_ov003_02082cc4(rg);
                    func_ov017_02140a8c(1, &rg->sprite, 4, 0);
                    func_ov003_020824a0(&rg->sprite, 0, 1);
                } else {
                    func_ov003_020824a0(&rg->sprite, 15, 1);
                }
            }
            if ((rg->sprite.unk16 == 4) && (rg->sprite.frameTimer == 1)) {
                temp_r1_3 = rg->unk_080;
                var_r4_2  = 0x30000;
                if (temp_r1_3 == 0) {
                    var_r6 = &data_ov017_021425d8;
                } else {
                    var_r6 = &data_ov017_021425e8;
                }
                if (((Boss03*)rg)->unk24 == 0) {
                    var_r5   = 0x8000;
                    var_r4_2 = -0x30000;
                } else {
                    var_r5 = 0;
                }
                if (temp_r1_3 == 0) {
                    func_ov003_020cc834(&sp74, rg, var_r6, 0, 0);
                    sp80 += var_r4_2;
                    sp88 -= 0x28000;
                    sp8E = 0x20;
                    sp92 = ((Boss03*)var_r6)->unk8;
                    func_ov003_020d1dc0(&sp74, 0x4000, var_r5, 0x1000, 5);
                    func_ov003_02087f00(SEIDX_SE_B005_FIRING_SHOT, func_ov003_020843b0(1, rg->position.x));
                } else {
                    func_ov003_020cc834(&sp48, rg, var_r6, 0, 0);
                    func_ov003_020cc834(&sp1C, rg, var_r6, 0xF8, 0);
                    sp54 += var_r4_2;
                    sp62 = 0x20;
                    sp36 = 0x20;
                    sp5C -= 0x28000;
                    sp28 += var_r4_2;
                    sp30 -= 0x28000;
                    sp66 = ((Boss03*)var_r6)->unk8 - 0x16;
                    sp3A = ((Boss03*)var_r6)->unk8 - 0x16;
                    func_ov003_020d0518(&sp48, 0x4000, var_r5, 4, &sp1C, 0x3000, var_r5, 0x1000, 3, 0x1E);
                    func_ov003_02087f00(SEIDX_SE_B005_FIRING_RAPID_SLOW, func_ov003_020843b0(1, rg->position.x));
                    ((Boss03*)rg)->unk1E8 = 0x1E;
                }
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite) != 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 2;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 2:
            if (rg->unk_1C0 == 0) {
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(&rg->sprite, 1, 1);
                } else {
                    func_ov003_020824a0(&rg->sprite, 16, 1);
                }
            }
            if (rg->unk_080 != 0) {
                temp_r1_4 = ((Boss03*)rg)->unk1E8;
                if ((s32)temp_r1_4 > 0) {
                    ((Boss03*)rg)->unk1E8 = (s16)(temp_r1_4 - 1);
                    if (((Boss03*)rg)->unk1E8 == 0) {
                        func_ov003_02087f00(SEIDX_SE_B005_FIRING_RAPID_SLOW, func_ov003_020843b0(1, rg->position.x));
                    }
                }
            }
            if ((SpriteMgr_IsAnimationFinished(&rg->sprite) != 0) && ((s32)rg->unk_1C0 >= 0x3C)) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 3;
                return;
            }
            rg->unk_1C0 = (s16)(rg->unk_1C0 + 1);
            return;
        case 3:
            if (rg->unk_1C0 == 0) {
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(&rg->sprite, 3);
                } else {
                    func_ov003_020824a0(&rg->sprite, 0x12);
                }
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite) == 0) {
                return;
            }
            BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
            return;
    }
}

void func_ov017_02131cd0(BtlBoss03_01_RG* rg) {
    u16   sp26;
    s16   sp22;
    s32   sp1C;
    s32   sp14;
    s32   sp8;
    void* var_r5_2;
    s32   var_r9;
    u16   temp_r0;
    u16   var_r10;

    switch (rg->unk_1C4) {
        case 0: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                rg->unk_1E6 |= 2;
            }
            if (func_ov003_020c6230(rg) == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 1;
            }
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                SndMgrSeIdx seIdx = (rg->unk_080 == 0) ? SEIDX_MINAMI_BTL_FB05 : SEIDX_MINAMI_H_BTL_ATCS01;
                func_ov003_02087f00(seIdx, func_ov003_020843b0(1, rg->position.x));
                if (rg->unk_080 == 0) {
                    func_ov003_02082cc4(rg);
                    func_ov017_02140a8c(1, &rg->sprite, 4, 0);
                    func_ov003_020824a0(&rg->sprite, 0, 1);
                } else {
                    func_ov003_020824a0(&rg->sprite, 15, 1);
                }
            }
            if ((rg->sprite.unk16 == 4) && (rg->sprite.frameTimer == 1)) {
                s32 var_r6 = 0x30000;
                if (rg->unk_080 == 0) {
                    var_r5_2 = &data_ov017_021425d8;
                } else {
                    var_r5_2 = &data_ov017_021425e8;
                }
                if (rg->unk_024 == 0) {
                    var_r6 = -0x30000;
                }
                func_ov003_020cc834(&sp8, rg, var_r5_2, 0xF8, 0);
                var_r9 = 0;
                sp22   = 0x20;
                sp14 += var_r6;
                sp1C -= 0x28000;
                var_r10 = 0;
                temp_r0 = ((Boss03*)var_r5_2)->unk8;
                if (rg->unk_080 == 0) {
                    sp26 = temp_r0;
                } else {
                    sp26 = temp_r0 - 0x16;
                }
                do {
                    func_ov003_020d23e8(&sp8, var_r10, 0x2000, 0x800, 0x400, 0x800);
                    var_r9 += 1;
                    var_r10 += 0x2000;
                } while (var_r9 < 8);
                func_ov003_02087f00(SEIDX_SE_B005_FIRING_SPIRAL, func_ov003_020843b0(1, rg->position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite) != 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 2;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 2: {
            if (rg->unk_1C0 == 0) {
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(&rg->sprite, 2);
                } else {
                    func_ov003_020824a0(&rg->sprite, 17);
                }
            }
            if (rg->unk_1C0 < 60) {
                rg->unk_1C0++;
            } else {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 3;
            }
        } break;

        case 3: {
            if (rg->unk_1C0 == 0) {
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(&rg->sprite, 3);
                } else {
                    func_ov003_020824a0(&rg->sprite, 18);
                }
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite) != 0) {
                BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
            }
        } break;
    }
}

void func_ov017_02131fcc(BtlBoss03_01_RG* rg) {
    SubroutineArgs subroutine_arg0;
    Sprite*        sprite  = &rg->sprite;
    RGBlob*        temp_r4 = rg->unk_188;

    switch (rg->unk_1C4) {
        case 0: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1DA = 30;
                rg->unk_1E6 &= ~2;
            }
            s32 temp = func_ov017_02132dc8(rg, &rg->unk_1DA, 0, rg->unk_1C0);
            if (rg->unk_1C0 == 12) {
                func_ov017_0213f0bc(1);
            }
            if (temp == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 1;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                if (rg->unk_080 == 0) {
                    func_ov003_02082cc4(rg);
                    func_ov017_02140a8c(1, &rg->sprite, 6, 0);
                    func_ov003_020824a0(sprite, 0, 1);
                } else {
                    func_ov003_020824a0(sprite, 26, 1);
                }
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite)) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 2;
            }
        } break;

        case 2: {
            if (rg->unk_1C0 == 0) {
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(sprite, 1);
                } else {
                    func_ov003_020824a0(sprite, 27);
                }
            }
            if (rg->unk_1E0 == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 3;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 3: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(sprite, 2);
                } else {
                    func_ov003_020824a0(sprite, 28);
                }
                func_ov003_02087f00(SEIDX_MINAMI_BTL_OTHER01, func_ov003_020843b0(1, rg->position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite)) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 4;
            }
        } break;

        case 4: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(sprite, 3);
                } else {
                    func_ov003_020824a0(sprite, 29);
                }
                func_ov003_02087f00(SEIDX_SE_B005_NINGEN_LION_HENSHIN, func_ov003_020843b0(1, rg->position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite)) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 5;
            }
        } break;

        case 5: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                if (rg->unk_080 == 0) {
                    func_ov003_02082cc4(rg);
                    func_ov017_02140a8c(1, &rg->sprite, 7, 0);
                    func_ov003_020824a0(&rg->sprite, 0, 1);
                } else {
                    func_ov003_020824a0(&rg->sprite, 30, 1);
                }
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite)) {
                func_ov017_0213f15c(1);
                Boss03_BeginFadeOut(0);
                func_ov003_02087ed8(753);

                BtlBoss03_SeqEntry_CreateTask(&subroutine_arg0);
                temp_r4->unk_20 = 1;
                temp_r4->unk_10 = 1;
                rg->unk_1CC     = 0;
                rg->flags |= 4;
            }
        } break;
    }
}

void func_ov017_02132378(BtlBoss03_01_RG* rg) {
    s16   sp10;
    s32   spC;
    s32   sp8;
    void* sp4;

    RGBlob* temp_r4 = rg->unk_188;

    switch (rg->unk_1C4) {
        case 0: {
            rg->unk_1E6 &= ~2;
            if (temp_r4->unk_0C == 0) {
                Boss03_BeginFadeOut(0);
            }
            rg->unk_1C0 = 0;
            rg->unk_1C4 = 1;
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                func_ov017_021302f0(rg);
                func_ov003_020c4b1c(rg);
                rg->position.z = 0;
            }
            func_ov017_021302b8(rg->unk_1C0);
            if ((rg->unk_1C0 > 0) && (EasyFade_IsFading() == FALSE)) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 2;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 2: {
            s32 temp_r0 = func_ov017_02132d4c(rg, 1, rg->unk_1C0);
            s32 var_r5  = 1;
            if (rg->unk_024 == 1) {
                var_r5 = -1;
            }
            if (rg->sprite.unk16 == 3 && rg->sprite.frameTimer == 1) {
                for (s32 i = 0; i < 5; i++) {
                    rg->unk_1EC[i] = func_ov017_02140c40(rg, var_r5 * data_ov017_0214269c[i].unk_0, RNG_Next(0x3001) - 0x1800,
                                                         data_ov017_0214269c[i].unk_4, 0x78);
                }
                func_ov003_02087f00(SEIDX_SE_B005_BADGE_NAGE01, func_ov003_020843b0(1, rg->position.x));
            } else if (rg->sprite.unk16 == 8 && rg->sprite.frameTimer == 1) {
                for (s32 i = 0; i < 4; i++) {
                    rg->unk_200[i] = func_ov017_02140c40(rg, var_r5 * data_ov017_0214267c[i].unk_0, RNG_Next(0x3001) - 0x1800,
                                                         data_ov017_0214267c[i].unk_4, 0x78);
                }
                func_ov003_02087f00(SEIDX_SE_B005_BADGE_NAGE01, func_ov003_020843b0(1, rg->position.x));
            }
            if (temp_r0 == 0) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 3;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 3: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                if (rg->unk_080 == 0) {
                    func_ov003_02082cc4(rg);
                    func_ov017_02140a8c(1, &rg->sprite, 6, 0);
                    func_ov003_020824a0(&rg->sprite, 0, 1);
                } else {
                    func_ov003_020824a0(&rg->sprite, 26, 1);
                }
                func_ov017_0213f0bc(1);
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite)) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 4;
            }
        } break;

        case 4: {
            if (rg->unk_1C0 == 0) {
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(&rg->sprite, 1);
                } else {
                    func_ov003_020824a0(&rg->sprite, 27);
                }
                rg->unk_1C2 = 30;
            }
            if (rg->unk_1C0 < rg->unk_1C2) {
                rg->unk_1C0++;
            } else {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 5;
            }
        } break;

        case 5: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(&rg->sprite, 2);
                } else {
                    func_ov003_020824a0(&rg->sprite, 28);
                }
                func_ov003_02087f00(SEIDX_MINAMI_BTL_OTHER02, func_ov003_020843b0(1, rg->position.x));
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite)) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 6;
            }
        } break;

        case 6: {
            if (rg->unk_1C0 == 0) {
                if (rg->unk_080 == 0) {
                    func_ov003_020824a0(&rg->sprite, 3);
                } else {
                    func_ov003_020824a0(&rg->sprite, 29);
                }
                func_ov003_02087f00(SEIDX_SE_B005_NINGEN_LION_HENSHIN, func_ov003_020843b0(1, rg->position.x));
                rg->unk_1C2 = 30;
            }
            if (rg->unk_1C0 < rg->unk_1C2) {
                rg->unk_1C0++;
            } else {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 7;
            }
        } break;

        case 7: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                if (rg->unk_080 == 0) {
                    func_ov003_02082cc4(rg);
                    func_ov017_02140a8c(1, &rg->sprite, 7, 0);
                    func_ov003_020824a0(&rg->sprite, 0, 1);
                } else {
                    func_ov003_020824a0(&rg->sprite, 30, 1);
                }
            }
            if (SpriteMgr_IsAnimationFinished(&rg->sprite)) {
                for (s32 i = 0; i < 9; i++) {
                    Task* task = EasyTask_GetTaskById(&data_ov003_020e71b8->taskPool, rg->unk_1EC[i]);
                    if (task != NULL) {
                        task->markedForDel = TRUE;
                    }
                    void* temp_r0_11              = rg + (i * 4);
                    ((Boss03*)temp_r0_11)->unk1EC = -1;
                }

                func_ov017_0213f15c(1);
                Boss03_BeginFadeOut(0);
                func_ov003_02087ed8(0x2F1);
                sp4  = rg;
                sp10 = 3;
                spC  = 0;
                sp8  = 0;
                BtlBoss03_SeqEntry_CreateTask(&sp4);
                rg->unk_18C |= 1;
                rg->flags |= 0x10;
                func_ov003_02084694(&rg->unk_144, 1);
                temp_r4->unk_20 = 1;
                temp_r4->unk_10 = 1;
                rg->unk_1CC     = 0;
                rg->flags |= 4;
                func_ov003_02082cc4(&rg->sprite);
                func_ov003_02082cc4(((Boss03*)rg)->unk184 + 0x84);
            }
        } break;
    }
}

void func_ov017_02132950(BtlBoss03_01_RG* rg) {
    if (rg->unk_1C0 == 0) {
        func_ov017_021302f0(rg);
        rg->unk_1E6 &= ~2;
    }
    if (BtlBoss03_01_RG_WarpAppear(rg, rg->unk_1C0) == 0) {
        BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
    } else {
        rg->unk_1C0++;
    }
}

void func_ov017_021329bc(BtlBoss03_01_RG* rg) {
    switch (rg->unk_1C4) {
        case 0: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1E6 |= 2;
            }
            EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
            if ((rg->unk_1C0 > 0) && (EasyFade_IsFading() == FALSE)) {
                rg->unk_1C0 = 0;
                rg->unk_1C4 = 1;
            } else {
                rg->unk_1C0++;
            }
        } break;

        case 1: {
            if (rg->unk_1C0 == 0) {
                rg->unk_1C0++;
                func_ov003_02082cc4(&rg->sprite);
                func_ov017_02140a8c(1, &rg->sprite, 0, 0);
                func_ov003_020824a0(&rg->sprite, 0, 0);
            }
            if ((func_ov003_020cb648(rg, 30) != 0) || (func_ov017_0213eeb8() != 0)) {
                BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
            }
        } break;
    }
}

void func_ov017_02132ac0(BtlBoss03_01_RG* rg) {
    RGBlob* temp_r4 = rg->unk_188;

    if (func_ov003_020c703c() == 0) {
        temp_r4->unk_10 = 1;
        rg->unk_1CC     = 0;
        rg->flags |= 4;
    } else {
        rg->unk_1C0++;
    }
}

void func_ov017_02132b0c(BtlBoss03_01_RG* rg) {
    if (rg->unk_1C0 == 0) {
        rg->unk_1C0++;
        if (rg->unk_080 == 0) {
            func_ov003_02082cc4(&rg->sprite);
            func_ov017_02140a8c(1, &rg->sprite, 0, 0);
        }
    }
    if (func_ov003_020c72b4(rg, 0, 0) == 0) {
        BtlBoss03_01_RG_SetState(rg, func_ov017_0213104c);
    }
}

s32 BtlBoss03_01_RG_WarpDisappear(BtlBoss03_01_RG* rg, s16 arg1) {
    if (arg1 == 0) {
        Sprite* sprite = &rg->sprite;

        rg->flags |= 0x10;
        func_ov003_02084694(&rg->unk_144, 1);
        if (rg->unk_080 == 0) {
            func_ov003_02082cc4(sprite);
            func_ov017_02140a8c(1, sprite, 1, 0);
            func_ov003_020824a0(sprite, 6, 1);
        } else {
            func_ov003_020824a0(sprite, 7, 1);
        }
        if (data_ov003_020e71b8->unk3D8A0 == rg) {
            data_ov003_020e71b8->unk3D8A0 = 0;
        }
        func_ov003_02087f00(SEIDX_SE_B005_KUUKANN_TENNI_DISAPPEAR, func_ov003_020843b0(1, rg->position.x));
    }
    if (SpriteMgr_IsAnimationFinished(&rg->sprite) == FALSE) {
        return 1;
    }
    rg->unk_18C |= 1;
    return 0;
}

s32 BtlBoss03_01_RG_WarpAppear(BtlBoss03_01_RG* rg, s16 arg1) {
    if (arg1 == 0) {
        Sprite* sprite = &rg->sprite;

        rg->unk_18C &= ~1;
        if (rg->unk_080 == 0) {
            func_ov003_02082cc4(sprite);
            func_ov017_02140a8c(1, sprite, 1, 0);
            func_ov003_020824a0(sprite, 7, 1);
        } else {
            func_ov003_020824a0(sprite, 8, 1);
        }
        func_ov003_020c4b1c(rg);
        func_ov003_02087f00(SEIDX_SE_B005_KUUKANN_TENNI_APPEAR, func_ov003_020843b0(1, rg->position.x));
        rg->unk_1AC = rg->position.x;
        rg->unk_1B0 = rg->position.y;
        rg->unk_1B4 = rg->position.z;
    }
    if (SpriteMgr_IsAnimationFinished(&rg->sprite)) {
        func_ov003_02084694(&rg->unk_144, 0);
        rg->flags &= ~0x10;
        func_ov003_02084694(&rg->unk_144, 0);
        return 0;
    } else {
        rg->unk_1C0++;
        return 1;
    }
}

s32 func_ov017_02132d4c(BtlBoss03_01_RG* rg, s32 arg1, s16 arg2) {
    if (arg2 == 0) {
        func_ov003_02082cc4(&rg->sprite);
        if (arg1 != 0) {
            func_ov017_02140a8c(1, &rg->sprite, 5, 0);
            func_ov003_020824a0(&rg->sprite, 0, 1);
        } else {
            func_ov017_02140a8c(1, &rg->sprite, 3, 0);
            func_ov003_020824a0(&rg->sprite, 0, 1);
        }
    }
    return (SpriteMgr_IsAnimationFinished(&rg->sprite) == FALSE);
}

s32 func_ov017_02132dc8(BtlBoss03_01_RG* rg, s16* arg1, s32 arg2, s16 arg3) {
    if (arg3 == 0) {
        if (rg->unk_080 == 0) {
            func_ov003_02082cc4(&rg->sprite);
            func_ov017_02140a8c(1, &rg->sprite, 4, 0);
            func_ov003_020824a0(&rg->sprite, 0, 1);
        } else {
            func_ov003_020824a0(&rg->sprite, 15, 1);
        }
    }

    switch (rg->unk_0C8) {
        case 0: {
            if (SpriteMgr_IsAnimationFinished(&rg->sprite) != 0) {
                u16 var_r0 = (arg2 == 0);
                func_ov003_020824a0(&rg->sprite, 1, var_r0);
            }
        } break;

        case 15: {
            if (SpriteMgr_IsAnimationFinished(&rg->sprite) != 0) {
                u16 var_r0_2 = (arg2 == 0);
                func_ov003_020824a0(&rg->sprite, 16, var_r0_2);
            }
        } break;

        case 1: {
            if (*arg1 > 0) {
                *arg1 = *arg1 - 1;
            } else {
                func_ov003_020824a0(&rg->sprite, 3, 1);
            }
        } break;

        case 16: {
            if (*arg1 > 0) {
                *arg1 = *arg1 - 1;
            } else {
                func_ov003_020824a0(&rg->sprite, 18, 1);
            }
        } break;

        case 18:
        case 3: {
            if (SpriteMgr_IsAnimationFinished(&rg->sprite) != 0) {
                return 0;
            }
        } break;
    }
    return 1;
}

s32 BtlBoss03_01_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlBoss03_01_RG* rg = task->data;

    switch (stage) {
        case 0:
            return BtlBoss03_01_RG_Init(rg, args);
        case 1:
            return BtlBoss03_01_RG_Update(rg);
        case 2:
            return BtlBoss03_01_RG_Render(rg);
        case 3:
            return BtlBoss03_01_RG_Destroy(rg);
    }
    return 1;
}

s32 BtlBoss03_01_RG_Init(BtlBoss03_01_RG* rg, void* arg1) {
    MI_CpuSet(rg, 0, sizeof(BtlBoss03_01_RG));

    u16 temp_r0 = ((Boss03*)arg1)->unk4;
    if (temp_r0 == 1) {
        ((Boss03*)arg1)->unk4 = 2;
    } else if (temp_r0 == 2) {
        ((Boss03*)arg1)->unk4 = 1;
    }

    func_ov003_020c3efc(rg, arg1);
    func_ov003_020c44ac(rg);
    func_ov003_020c4b1c(rg);
    func_ov003_020cb520(rg, 0);
    rg->flags |= 0x40000000;

    for (s32 i = 0; i < 4; i++) {
        BtlBoss03_01_RG_ResetCooldownRandom(rg, i);
    }

    BtlBoss03_01_RG_SetCooldown(rg, 3, 0);
    rg->unk_1CC = 1;
    rg->unk_1E6 &= ~2;

    if (rg->unk_080 == 0) {
        func_ov017_02133074(rg);
    } else {
        func_ov017_021331fc(rg);
    }

    return 1;
}

void func_ov017_02133074(BtlBoss03_01_RG* rg) {
    void*            temp_r4 = func_ov003_020cb42c(1, 43, 0);
    BtlBoss03_01_RG* temp    = func_ov003_020cb42c(1, 45, 0);

    rg->flags |= 0x20;
    if (data_ov003_020e71b8->unk3D874 == 0) {
        Input_PollState(&InputStatus);
        if ((InputStatus.buttonState.currButtons & INPUT_BUTTON_A) == 0) {
            rg->unk_18C |= 1;
            rg->flags |= 0x10;
            func_ov003_02084694(&rg->unk_144, 1);
            BtlBoss03_01_RG_SetState(rg, func_ov017_02130f78);
            func_ov003_02082cc4(&rg->sprite);
        } else {
            BtlBoss03_01_RG_SetState(rg, func_ov017_02130f10);
            rg->unk_1E6 |= 2;
        }
        return;
    }

    if (temp_r4 != NULL) {
        rg->unk_18C |= 1;
        BtlBoss03_01_RG_SetState(rg, func_ov017_02132950);
        return;
    }

    rg->unk_06C = temp->unk_06C;

    if ((temp->unk_18C & 1) == 0) {
        rg->position.x = temp->position.x;
        rg->position.y = temp->position.y;
        rg->position.z = temp->position.z;
        BtlBoss03_01_RG_SetState(rg, func_ov017_021329bc);
    } else {
        rg->unk_18C |= 1;
        rg->flags |= 0x10;
        func_ov003_02084694(&rg->unk_144, 1);
        BtlBoss03_01_RG_SetState(rg, func_ov017_02130f78);
        func_ov003_02082cc4(&rg->sprite);
    }
}

void func_ov017_021331fc(BtlBoss03_01_RG* rg) {
    BtlBoss03_01_RG_SetState(rg, func_ov017_02130f10);
    rg->unk_1E6 |= 2;
}

s32 BtlBoss03_01_RG_Update(BtlBoss03_01_RG* rg) {
    RGBlob* temp_r4 = rg->unk_184->unk_188;
    rg->unk_188     = temp_r4;

    if (func_ov003_020c3bf0() != 0) {
        if (rg->unk_020 == 6) {
            func_ov003_02082f2c(rg);
        }
        return 1;
    }

    s16 temp_r1 = rg->unk_1D0 - rg->unk_06C;
    if (temp_r1 > 0) {
        temp_r4->unk_00 += temp_r1;
    }

    if ((rg->unk_080 == 1) && (((u32)(rg->unk_1E6 << 0x1F) >> 0x1F) == 0) && (func_ov003_020cb648(rg, 0xA) != 0)) {
        rg->unk_1E6 = (u16)((rg->unk_1E6 & ~1) | 1);
        func_ov003_02087f00(SEIDX_MINAMI_BTL_PNC01, func_ov003_020843b0(1, rg->position.x));
    }

    switch (func_ov003_02082f2c(rg)) {
        case 3:
            if (rg->unk_080 != 0) {
                func_ov003_02084694(&rg->unk_144, 1);
                BtlBoss03_01_RG_SetState(rg, func_ov017_02132ac0);
            }
            break;
        case 6:
            rg->flags |= 1;
            if (rg->flags == 0) {
                BtlBoss03_01_RG_SetState(rg, func_ov017_02132b0c);
            }
            break;
    }

    if ((data_ov003_020e71b8->unk3D874 == 2) && (func_ov003_020c3bf0(rg) == 0)) {
        s32 var_r4 = 0;
        do {
            BtlBoss03_01_RG_TickCooldown(rg, var_r4);
            var_r4 += 1;
        } while (var_r4 < 4);
    }
    if ((data_ov003_020e71b8->unk3D874 == 2) && (rg->unk_080 != 0) && !(rg->flags & 4) &&
        (BtlBoss03_01_RG_IsCooldownReady(rg, 2) != 0))
    {
        BtlBoss03_01_RG_ResetCooldownRandom(rg, 2);
        func_ov003_020cb5b0(rg, 33, 1);
    }
    if (rg->unk_1C8 != NULL) {
        rg->unk_1C8(rg);
    }
    func_ov003_020c4628(rg);
    rg->unk_1D0 = rg->unk_06C;
    return rg->unk_1CC;
}

s32 BtlBoss03_01_RG_Render(BtlBoss03_01_RG* rg) {
    func_ov003_020c4878(rg);
    return 1;
}

s32 BtlBoss03_01_RG_Destroy(BtlBoss03_01_RG* rg) {
    func_ov003_020c48fc(rg);
    return 1;
}

void BtlBoss03_01_RG_ResetCooldownRandom(BtlBoss03_01_RG* rg, s32 cdSlot) {
    s32 var_r4 = (rg->unk_080 != 0) ? 1 : 0;
    s32 extra  = data_ov017_0214265c[var_r4][cdSlot].unk_2;
    s32 rnd    = RNG_Next((extra * 2) + 1);

    rg->unk_1D2[cdSlot] = data_ov017_0214265c[var_r4][cdSlot].unk_0 + (rnd - extra);
}

void BtlBoss03_01_RG_TickCooldown(BtlBoss03_01_RG* rg, s32 cdSlot) {
    if (rg->unk_1D2[cdSlot] > 0) {
        rg->unk_1D2[cdSlot]--;
    }
}

BOOL BtlBoss03_01_RG_IsCooldownReady(BtlBoss03_01_RG* rg, s32 cdSlot) {
    return rg->unk_1D2[cdSlot] == 0;
}

s16 BtlBoss03_01_RG_GetCooldown(BtlBoss03_01_RG* rg, s32 cdSlot) {
    return rg->unk_1D2[cdSlot];
}

void BtlBoss03_01_RG_SetCooldown(BtlBoss03_01_RG* rg, s32 cdSlot, s16 cooldown) {
    rg->unk_1D2[cdSlot] = cooldown;
}

/// MARK: BtlBoss03_01_UG

s32 BtlBoss03_01_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_01_UG = {"Tsk_BtlBoss03_01_UG", BtlBoss03_01_UG_RunTask, 0x0228};

void func_ov017_02133524(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1C8 = arg1;
    ((Boss03*)arg0)->unk1C4 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_0213354c(void* arg0) {
    u32 temp_r0;

    if (((Boss03*)arg0)->unk80 != 0) {
        func_ov017_02133524(arg0, func_ov017_02133d9c);
        ((Boss03*)arg0)->unk220 = 0;
        return;
    }
    temp_r0 = RNG_Next(0x64);
    if (((((Boss03*)arg0)->unk220 != 0) && ((s32)temp_r0 < 0x14)) || ((s32)temp_r0 < 0x28)) {
        ((Boss03*)arg0)->unk220 = 0;
        func_ov017_02133524(arg0, func_ov017_02133d9c);
        return;
    }
    func_ov017_02133524(arg0, func_ov017_02133ad0);
}

void func_ov017_021335c8(void* arg0) {
    s32 temp_r0;

    temp_r0 = (s32)RNG_Next(0x64);
    if (temp_r0 < 0x28) {
        func_ov017_02133524(arg0, func_ov017_02134378);
        return;
    }
    if (temp_r0 < 0x46) {
        func_ov017_02133524(arg0, func_ov017_02134600);
        return;
    }
    func_ov017_02133524(arg0, func_ov017_021348c0);
}

void func_ov017_02133624(void* arg0) {
    s16 temp_r1;

    temp_r1 = ((Boss03*)arg0)->unk1C0;
    if (temp_r1 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1 + 1);
        if (((Boss03*)arg0)->unk80 == 0) {
            data_ov003_020e71b8->unk3D875 = 2;
        }
    }
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_02133524(arg0, func_ov017_021337e4);
}

void func_ov017_0213368c(void* arg0) {
    s16   temp_r1;
    void* temp_r4;

    temp_r1 = ((Boss03*)arg0)->unk1C0;
    temp_r4 = ((Boss03*)arg0)->unk188;
    if (temp_r1 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1 + 1);
        ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 & ~2);
    }
    if (((Boss03*)temp_r4)->unk4 != 0) {
        ((Boss03*)temp_r4)->unk4 = 0;
        func_ov017_02135edc(arg0, 1, ((Boss03*)temp_r4)->unk14);
        func_ov017_02133524(arg0, func_ov017_02133718);
    }
    if (((Boss03*)temp_r4)->unk10 == 0) {
        return;
    }
    ((Boss03*)arg0)->unk1CC = 0;
    ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 4);
}

void func_ov017_02133718(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk224 &= ~2;
        func_ov017_02135d60(arg0);
        BtlBoss03_01_RG_PlaySE_BtlFb(arg0, 0);
    }

    if (func_ov017_021354e4(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
        func_ov017_02135e08(arg0, 0);
        func_ov017_02133524(arg0, func_ov017_021337e4);
    } else {
        ((Boss03*)arg0)->unk1C0++;
    }
}

void func_ov017_021337e4(void* arg0) {
    s16   temp_r2;
    u16   temp_r0;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg0)->unk188;
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 | 2);
        if (((Boss03*)arg0)->unk80 == 0) {
            func_ov003_02082cc4(arg0 + 0x84);
            func_ov017_02140a8c(0, arg0 + 0x84, 0, 0);
            func_ov003_020824a0(arg0 + 0x84, 0U, 0U);
        } else {
            func_ov003_020824a0(arg0 + 0x84, 0U, 0U);
        }
        ((Boss03*)arg0)->unk1C2 = func_ov003_020c42ec(arg0);
        if (temp_r4 != NULL) {
            ((Boss03*)temp_r4)->unk18 = 1;
        }
    }
    if ((((Boss03*)arg0)->unk80 == 0) && (func_ov017_0213ef4c() > 0)) {
        ((Boss03*)temp_r4)->unk18 = 0;
        func_ov017_02133524(arg0, func_ov017_02134190);
        return;
    }
    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk80 == 0) {
        if (func_ov003_020cb648(arg0, 0x1E) != 0) {
            ((Boss03*)temp_r4)->unk18 = 0;
            func_ov017_02133524(arg0, func_ov017_02135124);
            return;
        }
        if ((func_ov003_020cb648(arg0, 0x46) != 0) ||
            ((s32)((Boss03*)temp_r4)->unk0 >= (s32)(((u32)((Boss03*)arg0)->unk6E >> 0x1F) + (((Boss03*)arg0)->unk6E / 10))))
        {
            ((Boss03*)temp_r4)->unk18 = 0;
            func_ov017_02133524(arg0, func_ov017_02134d58);
            return;
        }
        if ((func_ov017_02135eb0(arg0, 1) != 0) && (func_ov017_0213f5fc(0) != 0)) {
            ((Boss03*)temp_r4)->unk18 = 0;
            func_ov017_02135e08(arg0, 1);
            func_ov017_021303d4(arg0);
            return;
        }
        goto block_18;
    }
block_18:
    temp_r0 = ((Boss03*)arg0)->unk80;
    switch (temp_r0) { /* irregular */
        case 1:
            if ((((Boss03*)temp_r4)->unk24 == 0) && (((Boss03*)temp_r4)->unk1C != 0) && (func_ov017_02135eb0(arg0, 0) != 0)) {
                ((Boss03*)temp_r4)->unk1C = 0;
                ((Boss03*)temp_r4)->unk18 = 0;
                ((Boss03*)temp_r4)->unk24 = 1;
                func_ov017_02133524(arg0, func_ov017_0213400c);
                func_ov003_020cb578(arg0, 0);
                return;
            }
        default:
        block_28:
            if ((func_ov003_02085dc8(1) == arg0) || (func_ov003_02085e78(1) == arg0) || (func_ov003_02085ec0(1) == arg0) ||
                (func_ov003_02086014(1) == arg0))
            {
                ((Boss03*)arg0)->unk220 = 1;
            }
            if ((((Boss03*)arg0)->unk220 != 0) ||
                (temp_r2 = ((Boss03*)arg0)->unk1C0, ((s32)temp_r2 >= (s32)((Boss03*)arg0)->unk1C2)))
            {
                ((Boss03*)temp_r4)->unk18 = 0;
                if ((((Boss03*)arg0)->unk220 != 0) || (func_ov003_020c4348(arg0) == 0)) {
                    func_ov017_0213354c(arg0);
                    return;
                }
                func_ov017_021335c8(arg0);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
            return;
        case 2:
            if (((Boss03*)temp_r4)->unk24 == 0) {
                func_ov017_02133524(arg0, func_ov017_0213400c);
                func_ov003_020cb578(arg0, 0);
                return;
            }
            goto block_28;
    }
}

void func_ov017_02133ad0(void* arg0) {
    s16 temp_r0;
    s16 temp_r0_2;
    s16 temp_r2;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r4;
    s32 temp_r5;

    temp_r5 = func_ov003_020cb744(0);
    temp_r2 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = func_ov003_020cb7a4(0);
    switch (temp_r2) { /* irregular */
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 | 2);
                ((Boss03*)arg0)->unk1D0 = (s32)(RNG_Next((temp_r5 >> 0xC) + 1) << 0xC);
                ((Boss03*)arg0)->unk1D4 = (s32)(RNG_Next((temp_r4 >> 0xC) + 1) << 0xC);
                ((Boss03*)arg0)->unk1D8 = 0;
                ((Boss03*)arg0)->unk1C2 = func_ov017_02135dac(arg0, 0x4000);
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_02082cc4(arg0 + 0x84);
                    func_ov017_02140a8c(0, arg0 + 0x84, 1, 0);
                    temp_r2_2 = ((Boss03*)arg0)->unk24;
                    if (((temp_r2_2 == 0) && ((s32)((Boss03*)arg0)->unk1D0 < (s32)((Boss03*)arg0)->unk28)) ||
                        ((temp_r2_2 == 1) && ((s32)((Boss03*)arg0)->unk1D0 >= (s32)((Boss03*)arg0)->unk28)))
                    {
                        func_ov003_020824a0(arg0 + 0x84, 0U, 1U);
                    } else {
                        func_ov003_020824a0(arg0 + 0x84, 3U, 1U);
                    }
                } else {
                    temp_r2_3 = ((Boss03*)arg0)->unk24;
                    if (((temp_r2_3 == 0) && ((s32)((Boss03*)arg0)->unk1D0 < (s32)((Boss03*)arg0)->unk28)) ||
                        ((temp_r2_3 == 1) && ((s32)((Boss03*)arg0)->unk1D0 >= (s32)((Boss03*)arg0)->unk28)))
                    {
                        func_ov003_020824a0(arg0 + 0x84, 1U, 1U);
                    } else {
                        func_ov003_020824a0(arg0 + 0x84, 4U, 1U);
                    }
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, (u16)(((Boss03*)arg0)->unkC8 + 1), 0U);
                func_ov003_02087f00(SEIDX_SE_B005_TSUUJYOU_IDOU, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
            ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
            if (((s32)((Boss03*)arg0)->unk1C0 >= (s32)((Boss03*)arg0)->unk1C2) || (func_ov003_020cc300(arg0, 0) == 0)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 2;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 2:
            temp_r0_2 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_2 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_2 + 1);
                func_ov003_020824a0(arg0 + 0x84, (u16)(((Boss03*)arg0)->unkC8 + 1), 1U);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_02133524(arg0, func_ov017_021337e4);
            return;
    }
}

void func_ov017_02133d9c(void* arg0) {
    s32 sp8;
    s32 sp4;
    s16 temp_r1;
    s32 temp_r4;
    s32 var_r1;
    u32 var_r0;
    u32 var_r6;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 & ~2);
            }
            if (func_ov017_02135438(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r4 = (RNG_Next(0x41) + 0x40) << 0xC;
                var_r6  = func_ov003_020cc430(arg0, 0, temp_r4, 0xC00);
                func_02047e84(0);
                var_r0 = RNG_Next(0);
                var_r1 = 0;
            loop_10:
                if (var_r6 & 1) {
                    if (var_r0 != 0) {
                        var_r0 = (u32)(u16)(var_r0 - 1);
                        goto block_13;
                    }
                } else {
                block_13:
                    var_r1 += 1;
                    var_r6 = var_r6 >> 1;
                    if (var_r1 < 0x20) {
                        goto loop_10;
                    }
                }
                func_ov003_020cbcb4(&sp8, &sp4, (u16)(var_r1 << 0xB), temp_r4, 0xC00);
                ((Boss03*)arg0)->unk28 += sp8;
                ((Boss03*)arg0)->unk2C += sp4;
                BtlBoss03_01_RG_PlaySE_BtlFb(arg0, 0);
            }
            if (func_ov017_021354e4(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_02133524(arg0, func_ov017_021337e4);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_02133f74(void* arg0) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg0)->unk188;
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 & ~2);
    }
    if (func_ov017_02135438(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10);
        func_ov017_02133524(arg0, func_ov017_0213368c);
        ((Boss03*)temp_r4)->unk14 = func_ov017_02135ecc(arg0, 1);
        ((Boss03*)temp_r4)->unk8  = 1;
        func_ov003_02082cc4(arg0 + 0x84);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_0213400c(void* arg0) {
    s16   temp_r1;
    u16   var_r0;
    void* temp_r0;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 & ~2);
            }
            if (func_ov017_02135438(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                if (((Boss03*)arg0)->unk80 == 1) {
                    var_r0 = 2;
                } else {
                    var_r0 = 1;
                }
                ((Boss03*)arg0)->unk80 = var_r0;
                func_ov003_020826c8(arg0 + 0x84, ((Boss03*)func_ov003_020c3c6c(arg0))->unkA);
                ((Boss03*)arg0)->unkD4  = (s32)((Boss03*)arg0)->unkC0;
                temp_r0                 = func_ov003_020c3c88(arg0);
                ((Boss03*)arg0)->unk192 = (s16)((Boss03*)temp_r0)->unk10;
                ((Boss03*)arg0)->unk194 = (s16)((Boss03*)temp_r0)->unk12;
                BtlBoss03_01_RG_PlaySE_BtlFb(arg0, 0);
            }
            if (func_ov017_021354e4(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov003_020cb578(arg0, 1);
                func_ov017_02133524(arg0, func_ov017_021337e4);
                func_ov017_02135e08(arg0, 0);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_02134190(void* arg0) {
    s16 temp_r1;
    s16 temp_r1_2;
    s16 temp_r2;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) { /* irregular */
        case 0:
            temp_r1_2 = ((Boss03*)arg0)->unk1C0;
            if (temp_r1_2 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1_2 + 1);
                if (func_ov003_020cb648(arg0, 0x46) != 0) {
                    func_ov017_0213f20c(1);
                } else {
                    func_ov017_0213f20c(0);
                }
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_02082cc4(arg0);
                    func_ov017_02140a8c(0, arg0 + 0x84, 3, 0);
                    func_ov003_020824a0(arg0 + 0x84, 0U, 1U);
                } else {
                    func_ov003_020824a0(arg0 + 0x84, 0xCU, 1U);
                }
                func_ov003_020c4ab4(arg0, 0);
                SndMgrSeIdx seIdx = (RNG_Next(100) < 50) ? SEIDX_MINAMI_BTL_ATCM02 : SEIDX_MINAMI_BTL_ATCS03;
                func_ov003_02087f00(seIdx, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 & ~2);
            }
            if ((((Boss03*)arg0)->unk9A == 4) && (((Boss03*)arg0)->unk8C == 1)) {
                if (func_ov017_0213ef4c() > 0) {
                    do {
                        func_ov017_02140b9c(arg0, 0);
                    } while (func_ov017_0213ef4c() > 0);
                }
                func_ov003_02087f00(SEIDX_SE_B005_BADGE_NAGE01, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(0, arg0 + 0x84, 0, 0);
                func_ov003_020824a0(arg0 + 0x84, 0U, 0U);
                ((Boss03*)arg0)->unk1C2 = 0x1E;
            }
            temp_r2 = ((Boss03*)arg0)->unk1C0;
            if ((s32)temp_r2 < (s32)((Boss03*)arg0)->unk1C2) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
                return;
            }
            func_ov017_02133524(arg0, func_ov017_02133f74);
            return;
    }
}

void func_ov017_02134378(void* arg0) {
    u16   sp92;
    s16   sp8E;
    s32   sp88;
    s32   sp80;
    s32   sp74;
    s16   sp66;
    s16   sp62;
    s32   sp5C;
    s32   sp54;
    s32   sp48;
    s16   sp3A;
    s16   sp36;
    s32   sp30;
    s32   sp28;
    s32   sp1C;
    void* var_r7;
    s32   temp_r4;
    s32   var_r4;
    s32   var_r5;
    s32   var_r6;
    u16   temp_r1;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        SndMgrSeIdx seIdx = (((Boss03*)arg0)->unk80 == 0) ? SEIDX_MINAMI_BTL_ATCM04 : SEIDX_MINAMI_H_BTL_ATCS03;
        func_ov003_02087f00(seIdx, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
        ((Boss03*)arg0)->unk218 = 0x3C;
        ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 | 2);
    }
    temp_r4 = func_ov017_021355a0(arg0, arg0 + 0x218, 0, ((Boss03*)arg0)->unk1C0);
    if (((Boss03*)arg0)->unk1C0 == 0xC) {
        temp_r1 = ((Boss03*)arg0)->unk80;
        var_r5  = 0x30000;
        if (temp_r1 == 0) {
            var_r7 = &data_ov017_021425d8;
        } else {
            var_r7 = &data_ov017_021425e8;
        }
        if (((Boss03*)arg0)->unk24 == 0) {
            var_r6 = 0x8000;
            var_r5 = -0x30000;
        } else {
            var_r6 = 0;
        }
        if (temp_r1 == 0) {
            func_ov003_020cc834(&sp74, arg0, var_r7, 0);
            sp80 += var_r5;
            sp88 -= 0x28000;
            sp8E = 0x20;
            sp92 = ((Boss03*)var_r7)->unk8;
            func_ov003_020d1dc0(&sp74, 0x4000, var_r6, 0x1000, 5);
            func_ov003_02087f00(SEIDX_SE_B005_FIRING_SHOT, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
        } else {
            func_ov003_020cc834(&sp48, arg0, var_r7, 0);
            func_ov003_020cc834(&sp1C, arg0, var_r7, 0xF2, 0);
            sp62 = 0x20;
            sp36 = 0x20;
            sp54 += var_r5;
            sp5C -= 0x28000;
            sp28 += var_r5;
            sp30 -= 0x28000;
            sp66 = ((Boss03*)var_r7)->unk8 - 0x16;
            sp3A = ((Boss03*)var_r7)->unk8 - 0x16;
            func_ov003_02087f00(SEIDX_SE_B005_FIRING_RAPID_SLOW, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            func_ov003_020d0518(&sp48, 0x4000, var_r6, 4, &sp1C, 0x3000, var_r6, 0x1000, 3, 0x1E);
        }
    }
    if ((((Boss03*)arg0)->unk80 != 0) && (((Boss03*)arg0)->unk1C0 == 0x2A)) {
        func_ov003_02087f00(SEIDX_SE_B005_FIRING_RAPID_SLOW, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (temp_r4 == 0) {
        func_ov017_02133524(arg0, func_ov017_021337e4);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_02134600(void* arg0) {
    u16   sp26;
    s16   sp22;
    s32   sp1C;
    s32   sp14;
    s32   sp8;
    void* var_r5_2;
    s16   temp_r1;
    s16   temp_r1_2;
    s32   var_r6;
    s32   var_r9;
    u16   temp_r0;
    u16   var_r10;
    void* temp_r0_2;
    void* temp_r0_3;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                SndMgrSeIdx seIdx;
                if (((Boss03*)arg0)->unk80 == 0) {
                    seIdx = SEIDX_MINAMI_BTL_FB05;
                } else {
                    seIdx = SEIDX_MINAMI_H_BTL_ATCS01;
                }
                func_ov003_02087f00(seIdx, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_02082cc4(arg0);
                    func_ov017_02140a8c(0, arg0 + 0x84, 4, 0);
                    func_ov003_020824a0(arg0 + 0x84, 0U, 1U);
                } else {
                    func_ov003_020824a0(arg0 + 0x84, 0xFU, 1U);
                }
                ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 | 2);
            }
            if ((((Boss03*)arg0)->unk9A == 4) && (((Boss03*)arg0)->unk8C == 1)) {
                var_r6 = 0x30000;
                if (((Boss03*)arg0)->unk80 == 0) {
                    var_r5_2 = &data_ov017_021425d8;
                } else {
                    var_r5_2 = &data_ov017_021425e8;
                }
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r6 = -0x30000;
                }
                func_ov003_020cc834(&sp8, arg0, var_r5_2, 0xF2, 0);
                var_r9 = 0;
                sp22   = 0x20;
                sp14 += var_r6;
                sp1C -= 0x28000;
                var_r10 = 0;
                temp_r0 = ((Boss03*)var_r5_2)->unk8;
                if (((Boss03*)arg0)->unk80 == 0) {
                    sp26 = temp_r0;
                } else {
                    sp26 = temp_r0 - 0x16;
                }
                do {
                    func_ov003_020d23e8(&sp8, var_r10, 0x2000, 0x800, 0x400, 0x800);
                    var_r9 += 1;
                    var_r10 += 0x2000;
                } while (var_r9 < 8);
                func_ov003_02087f00(SEIDX_SE_B005_FIRING_SPIRAL, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r0_2 = arg0 + 0x84;
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_020824a0(temp_r0_2, 2U);
                } else {
                    func_ov003_020824a0(temp_r0_2, 0x11U);
                }
            }
            temp_r1_2 = ((Boss03*)arg0)->unk1C0;
            if ((s32)temp_r1_2 < 0x3C) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1_2 + 1);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 2:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r0_3 = arg0 + 0x84;
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_020824a0(temp_r0_3, 3U);
                } else {
                    func_ov003_020824a0(temp_r0_3, 0x12U);
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_02133524(arg0, func_ov017_021337e4);
            return;
    }
}

void func_ov017_021348c0(void* arg0) {
    SubroutineArgs subroutine_arg0;
    s32            sp100;
    s32            spF8;
    s32            spEC;
    u16            spDE;
    s16            spDA;
    s32            spD4;
    s32            spCC;
    s32            spC0;
    s32            spBC;
    s32            spB8;
    s32            spB4;
    s32            spB0;
    s16            spAE;
    s16            spAC;
    s32            spA8;
    s32            spA4;
    s32            spA0;
    s16            sp9C;
    void*          sp98;
    void*          sp94;
    s16            sp86;
    s16            sp82;
    s32            sp7C;
    s32            sp74;
    s32            sp68;
    s16            sp5A;
    s16            sp56;
    s32            sp50;
    s32            sp48;
    s32            sp3C;
    void*          var_r6;
    void*          var_r7;
    s16            temp_r1;
    s32            var_r4_2;
    s32            var_r5;
    u16            temp_r1_2;
    void*          temp_r0;
    void*          temp_r0_2;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                SndMgrSeIdx seIdx;
                if (((Boss03*)arg0)->unk80 == 0) {
                    seIdx = SEIDX_MINAMI_BTL_FB07;
                } else if (RNG_Next(0x64) < 0x32U) {
                    seIdx = SEIDX_MINAMI_H_BTL_ATCS02;
                } else {
                    seIdx = SEIDX_MINAMI_H_BTL_FB07;
                }

                func_ov003_02087f00(seIdx, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_02082cc4(arg0);
                    func_ov017_02140a8c(0, arg0 + 0x84, 4, 0);
                    func_ov003_020824a0(arg0 + 0x84, 4U, 1U);
                } else {
                    func_ov003_020824a0(arg0 + 0x84, 0x13U, 1U);
                }
                ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 | 2);
            }
            if ((((Boss03*)arg0)->unk9A == 0xA) && (((Boss03*)arg0)->unk8C == 1)) {
                temp_r1_2 = ((Boss03*)arg0)->unk80;
                var_r4_2  = 0x24000;
                if (temp_r1_2 == 0) {
                    var_r6 = &data_ov017_02142608;
                    var_r7 = &data_ov017_021425f8;
                } else {
                    var_r6 = &data_ov017_021425c8;
                    var_r7 = &data_ov017_021425b8;
                }
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r4_2 = -0x24000;
                    var_r5   = -0x60000;
                } else {
                    var_r5 = 0x60000;
                }
                if (temp_r1_2 == 0) {
                    BtlBoss03_ShotEff_CreateTask((s32)arg0);
                    func_ov003_020cc834(&spEC, arg0, var_r6, 0xF2, 0);
                    func_ov003_020cc834(&spC0, arg0, var_r7, 0xF2, 0);
                    ((Boss03DataStruct*)&sp100)->unk6 = 0x20;
                    spDA                              = 0x20;
                    spF8 += var_r4_2;
                    sp100 -= 0x32000;
                    spCC += var_r4_2;
                    spD4 -= 0x32000;
                    ((Boss03DataStruct*)&sp100)->unkA = (u16)((Boss03*)var_r6)->unk8;
                    sp98                              = &spC0;
                    sp94                              = &spEC;
                    sp9C                              = 0x1E;
                    spDE                              = ((Boss03*)var_r7)->unk8;
                    spA0                              = ((Boss03*)arg0)->unk28 + var_r5;
                    spAE                              = 8;
                    spB8                              = 0xCCD;
                    spA4                              = ((Boss03*)arg0)->unk2C;
                    spA8                              = -0x50000;
                    spAC                              = 0x1E;
                    spBC                              = 0xC00;
                    spB0                              = 0x4000;
                    spB4                              = 0x4000;
                    func_ov003_020d2bc0(&sp94);
                    func_ov003_02087f00(SEIDX_SE_B005_FIRING_SPLIT, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                } else {
                    BtlBoss03_ShotEff_CreateTask((s32)arg0);
                    func_ov003_020cc834(&sp68, arg0, var_r6, 0xF2, 0);
                    func_ov003_020cc834(&sp3C, arg0, var_r7, 0xF2, 0);
                    sp82 = 0x20;
                    sp56 = 0x20;
                    sp74 += var_r4_2;
                    sp7C -= 0x32000;
                    sp48 += var_r4_2;
                    sp50 -= 0x32000;
                    if (((Boss03*)arg0)->unk80 == 0) {
                        sp86 = ((Boss03*)var_r6)->unk8 - 0x16;
                        sp5A = ((Boss03*)var_r7)->unk8 - 0x16;
                    }
                    func_ov003_020cd988(&subroutine_arg0, &sp68, &sp3C, 0x1E, ((Boss03*)arg0)->unk28 + var_r5,
                                        ((Boss03*)arg0)->unk2C, -0x50000, 0x1E, 8, 0x4000, 0x4000, 0x800, 0x3C, 8, 0x4000,
                                        0x4000, 0x400, 0xC00);
                    func_ov003_02087f00(SEIDX_SE_B005_FIRING_BURST, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r0 = arg0 + 0x84;
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_020824a0(temp_r0, 5U);
                } else {
                    func_ov003_020824a0(temp_r0, 0x14U);
                }
            }
            if ((SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) && ((s32)((Boss03*)arg0)->unk1C0 >= 0x3C)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 2;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 2:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r0_2 = arg0 + 0x84;
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_020824a0(temp_r0_2, 6U);
                } else {
                    func_ov003_020824a0(temp_r0_2, 0x15U);
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_02133524(arg0, func_ov017_021337e4);
            return;
    }
}

void func_ov017_02134d58(void* arg0) {
    SubroutineArgs subroutine_arg0;
    s16            temp_r1;
    s16            temp_r1_2;
    s16            temp_r1_3;
    s16            temp_r1_4;
    s16            temp_r1_5;
    s32            temp_r4_2;
    void*          temp_r0;
    void*          temp_r0_2;
    void*          temp_r0_3;
    void*          temp_r4;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = ((Boss03*)arg0)->unk188;
    switch (temp_r1) {
        default:
            return;
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                ((Boss03*)arg0)->unk218 = 0x1E;
                ((Boss03*)arg0)->unk44  = 0;
                ((Boss03*)arg0)->unk40  = 0;
                ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 & ~2);
            }
            temp_r4_2 = func_ov017_021355a0(arg0, arg0 + 0x218, 0, ((Boss03*)arg0)->unk1C0);
            if (((Boss03*)arg0)->unk1C0 == 0xC) {
                func_ov017_0213f0bc(0);
            }
            if (temp_r4_2 == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            temp_r1_2 = ((Boss03*)arg0)->unk1C0;
            if (temp_r1_2 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1_2 + 1);
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_02082cc4(arg0);
                    func_ov017_02140a8c(0, arg0 + 0x84, 6, 0);
                    func_ov003_020824a0(arg0 + 0x84, 0U, 1U);
                } else {
                    func_ov003_020824a0(arg0 + 0x84, 0x1AU, 1U);
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 2:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r0 = arg0 + 0x84;
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_020824a0(temp_r0, 1U);
                } else {
                    func_ov003_020824a0(temp_r0, 0x1BU);
                }
            }
            if (((Boss03*)arg0)->unk21C == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 3;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 3:
            temp_r1_3 = ((Boss03*)arg0)->unk1C0;
            if (temp_r1_3 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1_3 + 1);
                temp_r0_2               = arg0 + 0x84;
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_020824a0(temp_r0_2, 2U);
                } else {
                    func_ov003_020824a0(temp_r0_2, 0x1CU);
                }
                func_ov003_02087f00(SEIDX_MINAMI_BTL_OTHER01, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 4;
            return;
        case 4:
            temp_r1_4 = ((Boss03*)arg0)->unk1C0;
            if (temp_r1_4 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1_4 + 1);
                temp_r0_3               = arg0 + 0x84;
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_020824a0(temp_r0_3, 3U);
                } else {
                    func_ov003_020824a0(temp_r0_3, 0x1DU);
                }
                func_ov003_02087f00(SEIDX_SE_B005_NINGEN_LION_HENSHIN, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 5;
            return;
        case 5:
            temp_r1_5 = ((Boss03*)arg0)->unk1C0;
            if (temp_r1_5 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1_5 + 1);
                if (((Boss03*)arg0)->unk80 == 0) {
                    func_ov003_02082cc4(arg0);
                    func_ov017_02140a8c(0, arg0 + 0x84, 7, 0);
                    func_ov003_020824a0(arg0 + 0x84, 0U, 1U);
                } else {
                    func_ov003_020824a0(arg0 + 0x84, 0x1EU, 1U);
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_0213f15c(0);
            EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
            func_ov003_02087ed8(0x2F1);
            subroutine_arg0.unk0 = (s32)arg0;
            subroutine_arg0.unk4 = 0;
            subroutine_arg0.unk8 = 0;
            subroutine_arg0.unkC = 2;
            BtlBoss03_SeqEntry_CreateTask(&subroutine_arg0);
            ((Boss03*)arg0)->unk54    = (s32)(((Boss03*)arg0)->unk54 | 4);
            ((Boss03*)temp_r4)->unk20 = 1;
            ((Boss03*)temp_r4)->unk10 = 1;
            ((Boss03*)arg0)->unk1CC   = 0;
            ((Boss03*)arg0)->unk54    = (s32)(((Boss03*)arg0)->unk54 | 4);
            return;
    }
}

void func_ov017_02135124(void* arg0) {
    void* temp_r5;

    ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 & ~2);
    temp_r5                 = ((Boss03*)arg0)->unk188;
    Boss03_BeginFadeOut(0);
    ((Boss03*)temp_r5)->unkC = 1;
    ((Boss03*)arg0)->unk18C  = (u16)(((Boss03*)arg0)->unk18C | 1);
    ((Boss03*)arg0)->unk54   = (s32)(((Boss03*)arg0)->unk54 | 0x10);
    func_ov003_02084694((void**)((u8*)arg0 + 0x144), 0);
    func_ov017_02133524(arg0, func_ov017_0213368c);
    func_ov003_02082cc4(arg0 + 0x84);
}

void func_ov017_02135190(void* arg0) {
    s16 temp_r1;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 & ~2);
            }
            EasyFade_FadeBothDisplays(0, 0, 0x1000);
            if (((s32)((Boss03*)arg0)->unk1C0 > 0) && (EasyFade_IsFading() == 0)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(0, arg0 + 0x84, 0, 0);
                func_ov003_020824a0(arg0 + 0x84, 0U, 0U);
                ((Boss03*)arg0)->unk1C2 = 0x1E;
            }
            if ((func_ov003_020cb648(arg0, 0x1E) == 0) && (func_ov017_0213eeb8() == 0)) {
                return;
            }
            func_ov017_02133524(arg0, func_ov017_021337e4);
            return;
    }
}

void func_ov017_021352a0(void* arg0) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg0)->unk188;
    if (func_ov003_020c7070() == 0) {
        ((Boss03*)temp_r4)->unk10 = 1;
        ((Boss03*)arg0)->unk1CC   = 0;
        ((Boss03*)arg0)->unk54    = (s32)(((Boss03*)arg0)->unk54 | 4);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_021352ec(void* arg0) {
    s16 temp_r2;
    s32 var_r0;
    s32 var_r4;

    temp_r2 = ((Boss03*)arg0)->unk1C0;
    if (temp_r2 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
        if ((SndMgr_IsSEPlaying(SEIDX_MINAMI_BTL_DMG01) == 0) && (SndMgr_IsSEPlaying(SEIDX_MINAMI_BTL_DMG02) == 0)) {
            if (RNG_Next(0x64) < 0x32U) {
                var_r4 = SEIDX_MINAMI_BTL_DMG01;
            } else {
                var_r4 = SEIDX_MINAMI_BTL_DMG02;
            }
            func_ov003_02087f00(var_r4, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
        }
        if (((Boss03*)arg0)->unk80 == 0) {
            func_ov003_02082cc4(arg0 + 0x84);
            func_ov017_02140a8c(0, arg0 + 0x84, 2, 0);
        }
    }

    if (((Boss03*)arg0)->unk80 == 0) {
        var_r0 = func_ov003_020c6b8c(arg0, 1);
    } else {
        var_r0 = func_ov003_020c6b8c(arg0, 0xA);
    }
    if (var_r0 != 0) {
        return;
    }

    func_ov017_02133524(arg0, func_ov017_021337e4);
}

void func_ov017_021353c4(void* arg0) {
    s16 temp_r1;

    temp_r1 = ((Boss03*)arg0)->unk1C0;
    if (temp_r1 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1 + 1);
        if (((Boss03*)arg0)->unk80 == 0) {
            func_ov003_02082cc4(arg0 + 0x84);
            func_ov017_02140a8c(0, arg0 + 0x84, 0, 0);
        }
    }
    if (func_ov003_020c72b4(arg0, 0, 0) != 0) {
        return;
    }
    func_ov017_02133524(arg0, func_ov017_021337e4);
}

s32 func_ov017_02135438(void* arg0, s16 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10);
        if (((Boss03*)arg0)->unk80 == 0) {
            func_ov003_02082cc4(arg0);
            func_ov017_02140a8c(0, arg0 + 0x84, 1, 0);
            func_ov003_020824a0(arg0 + 0x84, 6U, 1U);
        } else {
            func_ov003_020824a0(arg0 + 0x84, 7U, 1U);
        }
        func_ov003_02087f00(SEIDX_SE_B005_KUUKANN_TENNI_DISAPPEAR, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return 1;
    }
    ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
    return 0;
}

s32 func_ov017_021354e4(void* arg0, s16 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C & ~1);
        if (((Boss03*)arg0)->unk80 == 0) {
            func_ov003_02082cc4(arg0);
            func_ov017_02140a8c(0, arg0 + 0x84, 1, 0);
            func_ov003_020824a0(arg0 + 0x84, 7U, 1U);
        } else {
            func_ov003_020824a0(arg0 + 0x84, 8U, 1U);
        }
        func_ov003_020c4b5c(arg0);
        func_ov003_02087f00(SEIDX_SE_B005_KUUKANN_TENNI_APPEAR, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return 1;
    }
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10);
    func_ov003_02084694(arg0 + 0x144, 0);
    return 0;
}

s32 func_ov017_021355a0(void* arg0, s16* arg1, s32 arg2, s16 arg3) {
    s16 temp_r0;
    s16 temp_r0_2;
    s16 temp_r0_3;
    u16 var_r0;
    u16 var_r0_2;

    if (arg3 == 0) {
        if (((Boss03*)arg0)->unk80 == 0) {
            func_ov003_02082cc4(arg0);
            func_ov017_02140a8c(0, arg0 + 0x84, 4, 0);
            func_ov003_020824a0(arg0 + 0x84, 0U, 1U);
        } else {
            func_ov003_020824a0(arg0 + 0x84, 0xFU, 1U);
        }
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    switch ((s32)temp_r0) { /* irregular */
        case 0:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                if (arg2 == 0) {
                    var_r0 = 1;
                } else {
                    var_r0 = 0;
                }
                func_ov003_020824a0(arg0 + 0x84, 1U, var_r0);
            }
        default:
            return 1;
        case 15:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                if (arg2 == 0) {
                    var_r0_2 = 1;
                } else {
                    var_r0_2 = 0;
                }
                func_ov003_020824a0(arg0 + 0x84, 0x10U, var_r0_2);
            }
            /* Duplicate return node #35. Try simplifying control flow for better match */
            return 1;
        case 1:
            temp_r0_2 = *arg1;
            if ((s32)temp_r0_2 > 0) {
                *arg1 = temp_r0_2 - 1;
            } else {
                func_ov003_020824a0(arg0 + 0x84, 3U, 1U);
            }
            /* Duplicate return node #35. Try simplifying control flow for better match */
            return 1;
        case 16:
            temp_r0_3 = *arg1;
            if ((s32)temp_r0_3 > 0) {
                *arg1 = temp_r0_3 - 1;
            } else {
                func_ov003_020824a0(arg0 + 0x84, 0x12U, 1U);
            }
            /* Duplicate return node #35. Try simplifying control flow for better match */
            return 1;
        case 18:
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                return 0;
            }
            /* Duplicate return node #35. Try simplifying control flow for better match */
            return 1;
    }
}

s32 BtlBoss03_01_UG_Init(void* arg1, void* arg2) {
    s32   var_r6;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov003_020cb744(0);
    func_ov003_020cb7a4(0);
    MI_CpuSet(temp_r4, 0, 0x228);
    if (((Boss03*)arg2)->unk4 == 0) {
        if (data_ov003_020e71b8->unk3D874 == 0) {
            BtlBoss03_KindanMgr_CreateTask();
            func_ov017_0213df24(0);
        }
        func_ov017_0213e17c(0x11, 0);
        func_ov017_0213e17c(0x12, 0);
        func_ov017_0213e088(0, 0);
        func_ov017_0213e088(0xE, 0);
        func_ov017_0213e088(0xF, 0);
        func_ov017_0213e088(0x10, 0);
    }
    func_ov003_020c3efc(temp_r4, arg2);
    func_ov003_020c4520(temp_r4);
    func_ov003_020c4b5c(temp_r4);
    ((Boss03*)temp_r4)->unk188 = (void*)(temp_r4 + 0x1E8);
    var_r6                     = 0;
    ((Boss03*)temp_r4)->unk20C = 0;
    ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 0x40000000);
    do {
        func_ov017_02135e08(temp_r4, var_r6);
        var_r6 += 1;
    } while (var_r6 < 3);
    ((Boss03*)temp_r4)->unk1CC = 1;
    ((Boss03*)temp_r4)->unk224 = (u16)(((Boss03*)temp_r4)->unk224 & ~1);
    ((Boss03*)temp_r4)->unk224 = (u16)(((Boss03*)temp_r4)->unk224 & ~2);
    if (((Boss03*)arg2)->unk4 == 0) {
        func_ov017_02135868(temp_r4);
    } else {
        func_ov017_02135a38(temp_r4);
    }
    return 1;
}

void func_ov017_02135868(void* arg0) {
    void* temp_r0;
    void* temp_r4;

    temp_r4                = func_ov003_020cb42c(0, 0x2B, 0);
    temp_r0                = func_ov003_020cb42c(0, 0x2D, 0);
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x20);
    if (data_ov003_020e71b8->unk3D874 == 0) {
        Input_PollState(&InputStatus);
        if (!(InputStatus.buttonState.currButtons & 1)) {
            func_ov017_02133524(arg0, func_ov017_02133624);
            ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 | 2);
            return;
        }
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
        ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x10);
        func_ov003_02084694(arg0 + 0x144, 1);
        func_ov017_02133524(arg0, func_ov017_0213368c);
        func_ov003_02082cc4(arg0 + 0x84);
        return;
    }
    if (temp_r4 != NULL) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
        ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x10);
        func_ov003_02084694(arg0 + 0x144, 1);
        func_ov017_02133524(arg0, func_ov017_0213368c);
        func_ov003_02082cc4(arg0 + 0x84);
        return;
    }
    ((Boss03*)arg0)->unk6C = (s16)((Boss03*)temp_r0)->unk6C;
    if (func_ov003_020cb648(arg0, 0x46) != 0) {
        func_ov017_0213f20c(1);
    } else {
        func_ov017_0213f20c(0);
    }
    if (!(((Boss03*)temp_r0)->unk18C & 1)) {
        ((Boss03*)arg0)->unk28 = (s32)((Boss03*)temp_r0)->unk28;
        ((Boss03*)arg0)->unk2C = (s32)((Boss03*)temp_r0)->unk2C;
        ((Boss03*)arg0)->unk30 = (s32)((Boss03*)temp_r0)->unk30;
        func_ov017_02133524(arg0, func_ov017_02135190);
        return;
    }
    ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
    ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x10);
    func_ov003_02084694(arg0 + 0x144, 1);
    func_ov017_02133524(arg0, func_ov017_0213368c);
    func_ov003_02082cc4(arg0 + 0x84);
}

void func_ov017_02135a38(void* arg0) {
    func_ov017_02133524(arg0, func_ov017_02133624);
    ((Boss03*)arg0)->unk224 = (u16)(((Boss03*)arg0)->unk224 | 2);
}

s32 BtlBoss03_01_UG_Update(void* arg1) {
    void (*temp_r1_2)(void*);
    s16   temp_r1;
    s32   temp_r0;
    s32   temp_r0_2;
    s32   var_r6;
    void* temp_r4;
    void* temp_r5;

    temp_r4                                       = ((Boss03*)arg1)->unk18;
    temp_r5                                       = temp_r4 + 0x1E8;
    ((Boss03*)((Boss03*)temp_r4)->unk184)->unk188 = temp_r5;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        if (((Boss03*)temp_r4)->unk20 == 6) {
            func_ov003_02082f2c(temp_r4);
        }
        return 1;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk210 - ((Boss03*)temp_r4)->unk6C;
    if ((s32)temp_r1 > 0) {
        ((Boss03*)temp_r4)->unk1E8 = (s16)(((Boss03*)temp_r4)->unk1E8 + temp_r1);
    }
    if ((((Boss03*)temp_r4)->unk80 == 1) && (((u32)(((Boss03*)temp_r4)->unk224 << 0x1F) >> 0x1F) == 0) &&
        (func_ov003_020cb648(temp_r4, 0xA) != 0))
    {
        ((Boss03*)temp_r4)->unk224 = (u16)((((Boss03*)temp_r4)->unk224 & ~1) | 1);
        func_ov003_02087f00(SEIDX_MINAMI_BTL_PNC01, func_ov003_020843b0(0, ((Boss03*)temp_r4)->unk28));
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    switch (temp_r0) { /* irregular */
        case 2:
            if (((u32)(((Boss03*)temp_r4)->unk224 << 0x1E) >> 0x1F) != 1) {
                ((Boss03*)temp_r4)->unk38 = 0;
            } else {
                ((Boss03*)temp_r5)->unk18 = 0;
                func_ov003_02084694((void**)((u8*)temp_r4 + 0x144), 0);
                func_ov017_02133524(temp_r4, func_ov017_021352ec);
            }
            break;
        case 3:
            if (((Boss03*)temp_r4)->unk80 != 0) {
                func_ov003_02084694(temp_r4 + 0x144, 1);
                func_ov017_02133524(temp_r4, func_ov017_021352a0);
            }
            break;
        case 6:
            temp_r0_2                 = ((Boss03*)temp_r4)->unk54 | 1;
            ((Boss03*)temp_r4)->unk54 = temp_r0_2;
            if (temp_r0_2 == 0) {
                func_ov017_02133524(temp_r4, func_ov017_021353c4);
            }
            break;
    }
    if ((data_ov003_020e71b8->unk3D874 == 2) && (func_ov003_020c3bf0(temp_r4) == 0)) {
        var_r6 = 0;
        do {
            func_ov017_02135e7c(temp_r4, var_r6);
            var_r6 += 1;
        } while (var_r6 < 3);
        ((Boss03*)temp_r5)->unk14 = (s16)(((Boss03*)temp_r5)->unk14 - 1);
        if ((s32)((Boss03*)temp_r5)->unk14 < 0) {
            ((Boss03*)temp_r5)->unk14 = 0;
        }
    }
    if ((data_ov003_020e71b8->unk3D874 == 2) && (((Boss03*)temp_r4)->unk80 != 0) && !(((Boss03*)temp_r4)->unk54 & 4) &&
        (func_ov017_02135eb0(temp_r4, 2) != 0))
    {
        func_ov017_02135e08(temp_r4, 2);
        func_ov003_020cb5b0(temp_r4, 0x21, 1);
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk1C8;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    func_ov003_020c4668(temp_r4);
    ((Boss03*)temp_r4)->unk210 = (s16)((Boss03*)temp_r4)->unk6C;
    return ((Boss03*)temp_r4)->unk1CC;
}

s32 BtlBoss03_01_UG_Render(void* arg1) {
    func_ov003_020c48b0(((Boss03*)arg1)->unk18);
    return 1;
}

s32 BtlBoss03_01_UG_Destroy(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov003_020c492c(temp_r4);
    if ((((Boss03*)temp_r4)->unk80 == 0) && (((Boss03*)temp_r4)->unk208 == 0)) {
        func_ov017_0213e074();
    }
    return 1;
}

s32 BtlBoss03_01_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_021426d0 = {
        .initialize = BtlBoss03_01_UG_Init,
        .update     = BtlBoss03_01_UG_Update,
        .render     = BtlBoss03_01_UG_Render,
        .cleanup    = BtlBoss03_01_UG_Destroy,
    };
    return data_ov017_021426d0.iter[stage](pool, task, args);
}

void func_ov017_02135d60(void* arg0) {
    ((Boss03*)arg0)->unk28 = (s32)(RNG_Next((func_ov003_020cb744(0) >> 0xC) + 1) << 0xC);
    ((Boss03*)arg0)->unk2C = (s32)(RNG_Next((func_ov003_020cb7a4(0) >> 0xC) + 1) << 0xC);
    ((Boss03*)arg0)->unk30 = 0;
}

s16 func_ov017_02135dac(void* arg0, s32 arg1) {
    return func_ov003_020cb910(arg0 + 0x1DC, arg0 + 0x1E0, arg0 + 0x1E4, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C,
                               ((Boss03*)arg0)->unk30, ((Boss03*)arg0)->unk1D0, ((Boss03*)arg0)->unk1D4,
                               ((Boss03*)arg0)->unk1D8, arg1);
}

void func_ov017_02135e08(void* arg0, s32 arg1) {
    s16 temp_r7;
    s32 temp_r4;
    s32 var_r1;

    if (((Boss03*)arg0)->unk80 != 0) {
        var_r1 = 1;
    } else {
        var_r1 = 0;
    }
    temp_r4 = var_r1 * 0xC;
    temp_r7 = *((arg1 * 4) + (&data_ov017_021426e2 + temp_r4));
    ((Boss03*)(arg0 + (arg1 * 2)))->unk212 =
        (s16)(*((arg1 * 4) + (&data_ov017_021426e0 + temp_r4)) + ((0 - temp_r7) + RNG_Next((temp_r7 * 2) + 1)));
}

void func_ov017_02135e7c(void* arg0, s32 arg1) {
    s16*  temp_ip;
    s32   temp_r3;
    void* temp_r0;

    temp_ip              = arg0 + 0x212;
    temp_r3              = arg1 * 2;
    temp_r0              = arg0 + (arg1 * 2);
    *(temp_ip + temp_r3) = *(temp_ip + temp_r3) - 1;
    if ((s32)((Boss03*)temp_r0)->unk212 < 0) {
        ((Boss03*)temp_r0)->unk212 = 0;
    }
}

s32 func_ov017_02135eb0(void* arg0, s32 arg1) {
    if (((Boss03*)(arg0 + (arg1 * 2)))->unk212 == 0) {
        return 1;
    }
    return 0;
}

s16 func_ov017_02135ecc(void* arg0, s32 arg1) {
    return ((Boss03*)(arg0 + (arg1 * 2)))->unk212;
}

void func_ov017_02135edc(void* arg0, s32 arg1, s16 arg2) {
    ((Boss03*)(arg0 + (arg1 * 2)))->unk212 = arg2;
}

void func_ov017_02135eec(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1C8 = arg1;
    ((Boss03*)arg0)->unk1C4 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_02135f14(void* arg0) {
    func_ov017_02135eec(arg0, func_ov017_021361ac);
    ((Boss03*)arg0)->unk202 = (s16)(((Boss03*)arg0)->unk202 + 1);
    if ((s32)((Boss03*)arg0)->unk202 < 3) {
        return;
    }
    ((Boss03*)arg0)->unk202 = 0;
    func_ov017_02137848(arg0, 0);
}

void func_ov017_02135f58(void* arg0) {
    s16 temp_r1;
    u32 temp_r0;

    temp_r1 = ((Boss03*)arg0)->unk1F6;
    if (temp_r1 == 3) {
        ((Boss03*)arg0)->unk1F6 = 0;
        func_ov017_02135eec(arg0, func_ov017_021364b8);
        return;
    }
    ((Boss03*)arg0)->unk1F6 = (s16)(temp_r1 + 1);
    temp_r0                 = RNG_Next(0x64);
    if ((s32)((Boss03*)arg0)->unk30 < 0) {
        if ((s32)temp_r0 < 0x32) {
            func_ov017_02135eec(arg0, func_ov017_021362b0);
        } else {
            func_ov017_02135eec(arg0, func_ov017_02136d90);
        }
    } else if ((s32)temp_r0 < 0x1E) {
        func_ov017_02135eec(arg0, func_ov017_021362b0);
    } else if ((s32)temp_r0 < 0x32) {
        func_ov017_02135eec(arg0, func_ov017_021363c4);
    } else {
        func_ov017_02135eec(arg0, func_ov017_02136d90);
    }
    ((Boss03*)arg0)->unk202 = 0;
}

void func_ov017_02136024(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        data_ov003_020e71b8->unk3D875 = 2;
        ((Boss03*)arg0)->unk1C0       = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    }
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_02135eec(arg0, func_ov017_021360ac);
}

void func_ov017_02136084(void* arg0) {
    if (((Boss03*)((Boss03*)arg0)->unk188)->unk4 == 0) {
        return;
    }
    ((Boss03*)arg0)->unk1CC = 0;
    ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 4);
}

void func_ov017_021360ac(void* arg0) {
    s16 temp_r1;
    s16 temp_r2;
    s32 temp_r0;

    func_ov003_020c4b1c(arg0);
    if (((s32)func_ov017_02130630(0) == 0) ||
        (temp_r1 = ((Boss03*)arg0)->unk6E, temp_r0 = ((Boss03*)arg0)->unk6C * 0x64, ((s32)(temp_r0 / temp_r1) < 0x1E)))
    {
        ((Boss03*)arg0)->unk1F8 = (u8)(((Boss03*)arg0)->unk1F8 & ~2);
        func_ov017_02135eec(arg0, func_ov017_02136fa0);
        func_ov017_0213e3b8();
        return;
    }
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk1C2 = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
    }
    func_ov017_02137228(arg0, ((Boss03*)arg0)->unk1C0);
    temp_r2 = ((Boss03*)arg0)->unk1C0;
    if ((s32)temp_r2 >= (s32)((Boss03*)arg0)->unk1C2) {
        if (func_ov003_020c4348(arg0) != 0) {
            func_ov017_02135f58(arg0);
            return;
        }
        if (func_ov017_0213789c(arg0, 0) != 0) {
            func_ov017_02135f14(arg0);
            return;
        }
        ((Boss03*)arg0)->unk1C0 = 1;
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
}

void func_ov017_021361ac(void* arg0) {
    s16 temp_r2;
    s32 var_r0;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r2) {
        default:
            return;
        case 0:
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x40000000);
            return;
        case 1:
            if (func_ov017_0213731c(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 2;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 2:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov017_021302f0(arg0);
                if (func_ov017_021378e4() != 0) {
                    var_r0 = 0;
                } else {
                    var_r0 = -0x40000;
                }
                ((Boss03*)arg0)->unk30 = var_r0;
                func_ov003_020c4b1c(arg0);
            }
            if (func_ov017_021373d8(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 3;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 3:
            func_ov017_02135eec(arg0, func_ov017_021360ac);
            return;
    }
}

void func_ov017_021362b0(void* arg0) {
    s16 temp_r0;
    s16 temp_r2;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r2) { /* irregular */
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                if ((func_ov017_021378e4() != 0) && (((Boss03*)arg0)->unk30 != 0)) {
                    /* Duplicate return node #11. Try simplifying control flow for better match */
                    func_ov017_02135eec(arg0, func_ov017_021361ac);
                    return;
                }
                if ((func_ov017_021378e4() == 0) && (((Boss03*)arg0)->unk30 == 0)) {
                    func_ov017_02135eec(arg0, func_ov017_021361ac);
                    return;
                }
                ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
                goto block_13;
            }
        block_13:
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020c4b1c(arg0);
                func_ov003_02087f00(SEIDX_MINAMI_N_BTL_ATCS02, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
            }
            if (func_ov017_02137494(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_02135eec(arg0, func_ov017_021360ac);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_021363c4(void* arg0) {
    s16 temp_r0;
    s16 temp_r2;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r2) { /* irregular */
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
            }
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020c4b1c(arg0);
                func_ov003_02087f00(SEIDX_MINAMI_N_BTL_ATCS03, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
                func_ov003_02087f00(SEIDX_SE_B006_KINNKYORI_KOUGEKI_L, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
            }
            if (func_ov017_02137600(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_02135eec(arg0, func_ov017_021360ac);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_021364b8(void* arg0) {
    s16   spA;
    s16   sp8;
    s32   var_r1;
    s16   temp_r0;
    s16   temp_r0_11;
    s16   temp_r0_14;
    s16   temp_r0_2;
    s16   temp_r0_3;
    s16   temp_r0_4;
    s16   temp_r0_5;
    s16   temp_r0_7;
    s16   temp_r1;
    s32   temp_r0_10;
    s32   temp_r0_12;
    s32   temp_r0_13;
    s32   temp_r0_15;
    s32   temp_r0_6;
    s32   temp_r0_8;
    s32   temp_r0_9;
    s32   var_r0;
    s32   var_r0_2;
    s32   var_r0_3;
    s32   var_r0_4;
    s32   var_r0_5;
    s32   var_r0_6;
    s32   var_r0_7;
    void* temp_r4;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = data_ov003_020e71b8->unk3D89C;
    switch (temp_r1) {
        default:
            return;
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                ((Boss03*)arg0)->unk1F8 = (u8)(((Boss03*)arg0)->unk1F8 & ~2);
            }
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x40000000);
            return;
        case 1:
            temp_r0_2 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_2 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_2 + 1);
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(1, arg0 + 0x84, 8, 0);
                func_ov003_020824a0(arg0 + 0x84, 3U, 0U);
                ((Boss03*)arg0)->unk1FC = BtlBoss03_EffSign_CreateTask(arg0);
                func_ov003_02087f00(SEIDX_MINAMI_N_BTL_FB06, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
            }
            if (EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, arg0 + 0x1FC) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 2:
            temp_r0_3 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_3 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_3 + 1);
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(1, arg0 + 0x84, 9, 0);
                func_ov003_020824a0(arg0 + 0x84, 2U, 1U);
                ((Boss03*)arg0)->unk1E8 = (s32)((Boss03*)arg0)->unk28;
                ((Boss03*)arg0)->unk1EC = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1F0 = (s32)((Boss03*)arg0)->unk30;
                if (((Boss03*)arg0)->unk24 == 0) {
                    ((Boss03*)arg0)->unk1DC = -0x8000;
                } else {
                    ((Boss03*)arg0)->unk1DC = 0x8000;
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 3;
            return;
        case 3:
            temp_r0_4 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_4 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_4 + 1);
                func_ov003_020824a0(arg0 + 0x84, 3U, 0U);
                BtlBoss03_EffPosture_CreateTask(arg0);
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            if (func_ov017_021377d4(arg0) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 4;
                return;
            }
            func_ov003_02084348(1, &spA, &sp8, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
            if (func_ov003_020cc7c0(spA, sp8, 0x40) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 0xC;
            return;
        case 4:
            temp_r0_5 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_5 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_5 + 1);
                func_ov003_020824a0(arg0 + 0x84, 4U, 1U);
                func_ov003_02087f00(SEIDX_SE_B006_TUKAMIKOUGEKI_01, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 5;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 5:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 5U, 1U);
                BtlBoss03_EffCatch_CreateTask(arg0);
            }
            if ((((u32)(((Boss03*)arg0)->unk1F8 << 0x1F) >> 0x1F) == 0) && (func_ov003_020cc354(temp_r4) == 0) &&
                (func_ov017_021377d4(arg0) != 0))
            {
                ((Boss03*)arg0)->unk1F8 = (u8)((((Boss03*)arg0)->unk1F8 & ~1) | 1);
                func_ov003_020cc784(temp_r4, 0);
                ((Boss03*)temp_r4)->unk38 = 0;
                func_ov003_02082f1c(temp_r4, 2);
                ((Boss03*)temp_r4)->unk54     = (s32)(((Boss03*)temp_r4)->unk54 | 0x20000);
                data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x80000);
                ((Boss03*)temp_r4)->unk10     = 0x12C;
                if (((Boss03*)arg0)->unk24 == 0) {
                    ((Boss03*)temp_r4)->unk24 = 1;
                } else {
                    ((Boss03*)temp_r4)->unk24 = 0;
                }
            }
            if (((u32)(((Boss03*)arg0)->unk1F8 << 0x1F) >> 0x1F) != 0) {
                temp_r0_6 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0 = temp_r0_6 - 0x48000;
                } else {
                    var_r0 = temp_r0_6 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x20000);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 6;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 6:
            temp_r0_7 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_7 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_7 + 1);
                func_ov003_020824a0(arg0 + 0x84, 6U, 1U);
                if (((u32)(((Boss03*)arg0)->unk1F8 << 0x1F) >> 0x1F) != 0) {
                    temp_r0_8 = ((Boss03*)arg0)->unk28;
                    if (((Boss03*)arg0)->unk24 == 0) {
                        var_r0_2 = temp_r0_8 - 0x18000;
                    } else {
                        var_r0_2 = temp_r0_8 + 0x18000;
                    }
                    ((Boss03*)temp_r4)->unk28 = var_r0_2;
                    ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                    ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x70000);
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 7;
            return;
        case 7:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 7U, 1U);
                if (((u32)(((Boss03*)arg0)->unk1F8 << 0x1F) >> 0x1F) != 0) {
                    temp_r0_9 = ((Boss03*)arg0)->unk28;
                    if (((Boss03*)arg0)->unk24 == 0) {
                        var_r0_3 = temp_r0_9 - 0x18000;
                    } else {
                        var_r0_3 = temp_r0_9 + 0x18000;
                    }
                    ((Boss03*)temp_r4)->unk28 = var_r0_3;
                    ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                    ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x70000);
                }
            }
            if ((((u32)(((Boss03*)arg0)->unk1F8 << 0x1F) >> 0x1F) != 0) && (((Boss03*)arg0)->unk1C0 == 0xC)) {
                temp_r0_10 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_4 = temp_r0_10 - 0x48000;
                } else {
                    var_r0_4 = temp_r0_10 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0_4;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x50000);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 8;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 8:
            temp_r0_11 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_11 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_11 + 1);
                func_ov003_020824a0(arg0 + 0x84, 8U, 1U);
            }
            if (((u32)(((Boss03*)arg0)->unk1F8 << 0x1F) >> 0x1F) != 0) {
                temp_r0_12 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_5 = temp_r0_12 - 0x48000;
                } else {
                    var_r0_5 = temp_r0_12 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0_5;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x20000);
                ((Boss03*)temp_r4)->unk38 = 0x20000;
                ((Boss03*)arg0)->unk1F8   = (u8)(((Boss03*)arg0)->unk1F8 & ~1);
                func_ov003_020cc784(temp_r4, 1);
                ((Boss03*)temp_r4)->unk10 = 0;
                func_ov003_020cc784(temp_r4, 1);
                func_ov017_0213e3a4();
                BtlBoss03_FailPly_CreateTask(arg0, temp_r4);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 9;
            return;
        case 9:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 9U, 1U);
                func_ov017_0213778c(arg0, func_ov003_020cb764(1), ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
                temp_r0_13 = ((Boss03*)arg0)->unk1D0;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_6 = temp_r0_13 + 0x50000;
                } else {
                    var_r0_6 = temp_r0_13 - 0x50000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_6;
                ((Boss03*)arg0)->unk1C2 = func_ov017_02137730(arg0, 0x4000);
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
            ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
            if (((s32)((Boss03*)arg0)->unk1C0 >= (s32)((Boss03*)arg0)->unk1C2) || (func_ov003_020cc300(arg0, 1) == 0)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 0xA;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 10:
            temp_r0_14 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_14 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_14 + 1);
                func_ov003_020824a0(arg0 + 0x84, 0xAU, 1U);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_02135eec(arg0, func_ov017_021360ac);
            ((Boss03*)arg0)->unk1F8 = (u8)(((Boss03*)arg0)->unk1F8 | 2);
            return;
        case 12:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov017_0213778c(arg0, func_ov003_020cb764(1), ((Boss03*)arg0)->unk1EC, ((Boss03*)arg0)->unk1F0);
                temp_r0_15 = ((Boss03*)arg0)->unk1D0;
                if ((s32)((Boss03*)arg0)->unk1E8 < temp_r0_15) {
                    var_r0_7 = temp_r0_15 + 0x50000;
                } else {
                    var_r0_7 = temp_r0_15 - 0x50000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_7;
                if ((s32)((Boss03*)arg0)->unk1D0 > (s32)((Boss03*)arg0)->unk28) {
                    var_r1 = 1;
                } else {
                    var_r1 = 0;
                }
                func_ov003_020c4ab4(arg0, var_r1);
            }
            if (func_ov017_0213727c(arg0, 0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_02135eec(arg0, func_ov017_021360ac);
                ((Boss03*)arg0)->unk1F8 = (u8)(((Boss03*)arg0)->unk1F8 | 2);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_02136d90(void* arg0) {
    s16   temp_r0;
    s16   temp_r2;
    s32   temp_r6;
    s32   var_r0;
    s32   var_r1;
    s32   var_r4;
    void* temp_r4;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = func_ov003_020cc750(1);
    switch (temp_r2) {
        default:
            return;
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
            }
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 1:
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 2:
            if (func_ov017_0213731c(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 3;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 3:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r6 = ((Boss03*)temp_r4)->unk28;
                var_r1  = 0x50000;
                if (RNG_Next(0x64) < 0x32U) {
                    var_r1 = -0x50000;
                }
                ((Boss03*)arg0)->unk28 = (s32)(temp_r6 + var_r1);
                ((Boss03*)arg0)->unk2C = (s32)((Boss03*)temp_r4)->unk2C;
                if (((Boss03*)temp_r4)->unk30 == 0) {
                    var_r0 = 0;
                } else {
                    var_r0 = -0x40000;
                }
                ((Boss03*)arg0)->unk30 = var_r0;
                if (((var_r1 < 0) && (((Boss03*)temp_r4)->unk24 == 0)) || ((var_r1 > 0) && (((Boss03*)temp_r4)->unk24 == 1))) {
                    var_r4 = 0x506;
                } else {
                    var_r4 = 0x507;
                }
                func_ov003_02087f00(var_r4, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
                func_ov003_020c4b1c(arg0);
            }
            if (func_ov017_021373d8(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 4;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 4:
            if (func_ov017_02137494(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 5;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 5:
            func_ov017_02135eec(arg0, func_ov017_021360ac);
            return;
    }
}

void func_ov017_02136fa0(void* arg0) {
    SubroutineArgs subroutine_arg0;
    void*          temp_r4;

    temp_r4                = ((Boss03*)arg0)->unk188;
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x40000010);
    EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
    subroutine_arg0.unk0 = (s32)arg0;
    subroutine_arg0.unk4 = 0;
    subroutine_arg0.unk8 = 0;
    subroutine_arg0.unkC = 1;
    BtlBoss03_SeqEntry_CreateTask(&subroutine_arg0);
    ((Boss03*)arg0)->unk1CC  = 0;
    ((Boss03*)temp_r4)->unk8 = 1;
    ((Boss03*)temp_r4)->unk4 = 1;
    ((Boss03*)arg0)->unk54   = (s32)(((Boss03*)arg0)->unk54 | 4);
    func_ov003_02087ed8(0x2DE);
}

void func_ov017_02137020(void* arg0) {
    s16 temp_r1;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(1, arg0 + 0x84, 0xB, 0);
                func_ov003_020824a0(arg0 + 0x84, 2U, 1U);
                ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x40000000);
            }
            EasyFade_FadeBothDisplays(0, 0, 0x1000);
            if (((s32)((Boss03*)arg0)->unk1C0 > 0) && (EasyFade_IsFading() == 0)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1F8 = (u8)(((Boss03*)arg0)->unk1F8 | 2);
            func_ov017_02135eec(arg0, func_ov017_021360ac);
            return;
    }
}

void func_ov017_02137110(void* arg0) {
    s16   temp_r1;
    u32   var_r1;
    u8    temp_r0;
    void* temp_r3;
    void* temp_r3_2;

    temp_r1 = ((Boss03*)arg0)->unk1C0;
    if (temp_r1 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1 + 1);
        temp_r0                 = ((Boss03*)arg0)->unk1F8;
        var_r1                  = (u32)(temp_r0 << 0x1F) >> 0x1F;
        if (var_r1 != 0) {
            ((Boss03*)arg0)->unk1F8 = (u8)(temp_r0 & ~1);
            func_ov003_020cc784(data_ov003_020e71b8->unk3D89C, 1);
            temp_r3                       = data_ov003_020e71b8->unk3D89C;
            ((Boss03*)temp_r3)->unk54     = (s32)(((Boss03*)temp_r3)->unk54 & ~0x20000);
            temp_r3_2                     = data_ov003_020e71b8->unk3D89C;
            ((Boss03*)temp_r3_2)->unk54   = (s32)(((Boss03*)temp_r3_2)->unk54 & ~0x4000);
            data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 & ~0x80000);
            func_ov003_0208afcc(0);
            ((Boss03*)data_ov003_020e71b8->unk3D89C)->unk10 = 0;
            var_r1 = SECOND_REG(func_ov003_020cc784(data_ov003_020e71b8->unk3D89C, 1));
        }
        func_ov003_02082cc4((void**)((u8*)arg0 + 0x84));
        func_ov017_02140a8c(1, arg0 + 0x84, 8, 0);
    }
    if (func_ov003_020c72b4(arg0, 0, 0) != 0) {
        return;
    }
    func_ov017_02135eec(arg0, func_ov017_021360ac);
}

s32 func_ov017_02137228(void* arg0, s16 arg1) {
    if (arg1 == -1) {
        return 0;
    }
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 8, 0);
        func_ov003_020824a0(arg0 + 0x84, 0U, 0U);
    }
    return 1;
}

s32 func_ov017_0213727c(void* arg0, s32 arg1, s16 arg2) {
    if (arg2 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xA, 0);
        func_ov003_020824a0(arg0 + 0x84, 0U, 0U);
    }
    func_ov017_02137730(arg0, 0x4000);
    ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
    ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
    ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
    if (func_ov017_0213779c(arg0) >= 0x4000) {
        return 1;
    }
    func_ov017_02137228(arg0, 0);
    return 0;
}

s32 func_ov017_0213731c(void* arg0, s32 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xB, 0);
        func_ov003_020824a0(arg0 + 0x84, 0, 1);
        if (data_ov003_020e71b8->unk3D8A0 == arg0) {
            data_ov003_020e71b8->unk3D8A0 = 0;
        }
        func_ov003_02087f00(SEIDX_SE_B006_KUUKANN_TENNI_DISAPPEAR, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
        return 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    return 1;
}

s32 func_ov017_021373d8(void* arg0, s32 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C & ~1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xB, 0);
        func_ov003_020824a0(arg0 + 0x84, 1, 1);
        func_ov003_02087f00(SEIDX_SE_B006_KUUKANN_TENNI_APPEAR, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
        ((Boss03*)arg0)->unk1AC = (s32)((Boss03*)arg0)->unk28;
        ((Boss03*)arg0)->unk1B0 = (s32)((Boss03*)arg0)->unk2C;
        ((Boss03*)arg0)->unk1B4 = (s32)((Boss03*)arg0)->unk30;
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10);
        return 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    return 1;
}

s32 func_ov017_02137494(void* arg0, s32 arg1) {
    f32 var_r0;
    s16 temp_r0;
    s32 temp_r0_3;
    s32 var_r4;
    u8  temp_r0_2;

    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xC, 0);
        func_ov003_020824a0(arg0 + 0x84, 0, 1);
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    switch (temp_r0) { /* irregular */
        case 0:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 1, 1);
                func_ov003_02087f00(SEIDX_SE_B006_KINNKYORI_KOUGEKI_S, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
            }
        default:
            return 1;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 2, 1);
            }
            temp_r0_2 = ((Boss03*)func_ov003_0208a114(0xF9))->unk7;
            var_r4    = 0x40000;
            if (((Boss03*)arg0)->unk24 == 0) {
                var_r4 = -0x40000;
            }
            temp_r0_3 = temp_r0_2 << 0xC;
            if (temp_r0_2 != 0) {
                var_r0 = 0.5f + (f32)temp_r0_3;
            } else {
                var_r0 = (f32)temp_r0_3 - 0.5f;
            }
            func_ov003_020c5b2c(0xF90000U >> 0x10, arg0, ((Boss03*)arg0)->unk28 + var_r4, ((Boss03*)arg0)->unk2C,
                                ((Boss03*)arg0)->unk30 - (s32)var_r0);
            /* Duplicate return node #18. Try simplifying control flow for better match */
            return 1;
        case 2:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov017_02137228(arg0, 0);
                return 0;
            }
            /* Duplicate return node #18. Try simplifying control flow for better match */
            return 1;
    }
}

s32 func_ov017_02137600(void* arg0, s32 arg1) {
    s16 temp_r0;
    s32 var_r5;

    var_r5 = -0x40000;
    func_ov003_020cc750(1);
    if (((Boss03*)arg0)->unk24 == 1) {
        var_r5 = 0x40000;
    }
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xD, 0);
        func_ov003_020824a0(arg0 + 0x84, 0, 1);
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    switch (temp_r0) { /* irregular */
        case 0:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 1, 1);
            }
        default:
            return 1;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 2, 1);
            }
            if ((arg1 >= 8) && (arg1 <= 0x17)) {
                func_ov003_020c5b2c(0xFAU, arg0, ((Boss03*)arg0)->unk28 + var_r5, ((Boss03*)arg0)->unk2C,
                                    ((Boss03*)arg0)->unk30 - 0x20000);
            }
            /* Duplicate return node #17. Try simplifying control flow for better match */
            return 1;
        case 2:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov017_02137228(arg0, 0);
                return 0;
            }
            /* Duplicate return node #17. Try simplifying control flow for better match */
            return 1;
    }
}

s16 func_ov017_02137730(void* arg0, s32 arg1) {
    return func_ov003_020cb910(arg0 + 0x1DC, arg0 + 0x1E0, arg0 + 0x1E4, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C,
                               ((Boss03*)arg0)->unk30, ((Boss03*)arg0)->unk1D0, ((Boss03*)arg0)->unk1D4,
                               ((Boss03*)arg0)->unk1D8, arg1);
}

void func_ov017_0213778c(void* arg0, s32 arg1, s32 arg2, s32 arg3) {
    ((Boss03*)arg0)->unk1D0 = arg1;
    ((Boss03*)arg0)->unk1D4 = arg2;
    ((Boss03*)arg0)->unk1D8 = arg3;
}

s32 func_ov017_0213779c(void* arg0) {
    func_ov003_020cba54(((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30, ((Boss03*)arg0)->unk1D0,
                        ((Boss03*)arg0)->unk1D4, ((Boss03*)arg0)->unk1D8);
}

s32 func_ov017_021377d4(void* arg0) {
    s32 var_ip;

    var_ip = 0x48000;
    if (((Boss03*)arg0)->unk24 == 0) {
        var_ip = -0x48000;
    }
    func_ov003_020cc38c(data_ov003_020e71b8->unk3D89C, ((Boss03*)arg0)->unk28 + var_ip, ((Boss03*)arg0)->unk2C,
                        ((Boss03*)arg0)->unk30 - 0x20000);
}

void func_ov017_02137834(void* arg0, s32 arg1) {
    ((Boss03*)(arg0 + (arg1 * 2)))->unk200 = 0;
}

void func_ov017_02137848(void* arg0, s32 arg1) {
    s16 temp_r5;

    temp_r5 = *(s32*)((char*)&data_ov017_021426fa + (arg1 * 4));
    ((Boss03*)(arg0 + (arg1 * 2)))->unk200 =
        (s16)(*(s32*)((char*)&data_ov017_021426f8 + (arg1 * 4)) + ((0 - temp_r5) + RNG_Next((temp_r5 * 2) + 1)));
}

s32 func_ov017_0213789c(s32 arg0, s32 arg1) {
    if (((Boss03*)(arg0 + (arg1 * 2)))->unk200 == 0) {
        return 1;
    }
    return 0;
}

void func_ov017_021378b8(void* arg0, s32 arg1) {
    s16* temp_r1;
    s32  temp_r3;

    temp_r3 = arg1 * 2;
    if ((s32)((Boss03*)(arg0 + (arg1 * 2)))->unk200 <= 0) {
        return;
    }
    temp_r1              = arg0 + 0x200;
    *(temp_r1 + temp_r3) = *(temp_r1 + temp_r3) - 1;
}

s32 func_ov017_021378e4(void) {
    if (((Boss03*)data_ov003_020e71b8->unk3D89C)->unk30 > -0x20000) {
        return 1;
    }
    return 0;
}

s32 func_ov017_02137914(void* arg1, s32 arg2) {
    void* temp_r4;
    void* temp_r5;

    temp_r5 = ((Boss03*)arg1)->unk18;
    temp_r4 = func_ov003_020cb42c(1, 0x2C, 0);
    MI_CpuSet(temp_r5, 0, 0x204);
    func_ov003_020c3efc(temp_r5, arg2);
    func_ov003_020c44ac(temp_r5);
    func_ov003_020c4b1c(temp_r5);
    ((Boss03*)temp_r5)->unk54 = (s32)(((Boss03*)temp_r5)->unk54 | 0x20);
    func_ov003_020cb520(temp_r5, 0);
    func_ov017_02137834(temp_r5, 0);
    ((Boss03*)temp_r5)->unk1CC = 1;
    ((Boss03*)temp_r5)->unk1F8 = (u8)(((Boss03*)temp_r5)->unk1F8 & ~2);
    if (data_ov003_020e71b8->unk3D874 == 0) {
        if (!(InputStatus.buttonState.currButtons & 1)) {
            ((Boss03*)temp_r5)->unk18C = (u16)(((Boss03*)temp_r5)->unk18C | 1);
            ((Boss03*)temp_r5)->unk54  = (s32)(((Boss03*)temp_r5)->unk54 | 0x10);
            func_ov017_02135eec(temp_r5, &func_ov017_02136084);
            func_ov003_02082cc4(temp_r5 + 0x84);
        } else {
            func_ov017_02135eec(temp_r5, &func_ov017_02136024);
            ((Boss03*)temp_r5)->unk28 = 0x100000;
            func_ov017_021302f0(temp_r5);
        }
    } else {
        ((Boss03*)temp_r5)->unk6C = (s16)((Boss03*)temp_r4)->unk6C;
        if (!(((Boss03*)temp_r4)->unk18C & 1)) {
            ((Boss03*)temp_r5)->unk28 = (s32)((Boss03*)temp_r4)->unk28;
            ((Boss03*)temp_r5)->unk2C = (s32)((Boss03*)temp_r4)->unk2C;
            ((Boss03*)temp_r5)->unk30 = (s32)((Boss03*)temp_r4)->unk30;
            func_ov017_02135eec(temp_r5, &func_ov017_02137020);
        } else {
            ((Boss03*)temp_r5)->unk18C = (u16)(((Boss03*)temp_r5)->unk18C | 1);
            ((Boss03*)temp_r5)->unk54  = (s32)(((Boss03*)temp_r5)->unk54 | 0x10);
            func_ov017_02135eec(temp_r5, &func_ov017_02136084);
            func_ov003_02082cc4(temp_r5 + 0x84);
        }
    }
    return 1;
}

s32 func_ov017_02137aa0(void* arg1) {
    void (*temp_r1_2)(void*);
    s16   temp_r1;
    s16*  temp_r5;
    s32   temp_r0;
    s32   temp_r0_2;
    void* temp_r4;

    temp_r4                    = ((Boss03*)arg1)->unk18;
    temp_r5                    = ((Boss03*)((Boss03*)temp_r4)->unk184)->unk188;
    ((Boss03*)temp_r4)->unk188 = temp_r5;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        if (((Boss03*)temp_r4)->unk20 == 6) {
            func_ov003_02082f2c(temp_r4);
        }
        return 1;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk1F4 - ((Boss03*)temp_r4)->unk6C;
    if ((s32)temp_r1 > 0) {
        *temp_r5 += temp_r1;
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    if ((temp_r0 != 3) && (temp_r0 == 6)) {
        temp_r0_2                 = ((Boss03*)temp_r4)->unk54 | 1;
        ((Boss03*)temp_r4)->unk54 = temp_r0_2;
        if (temp_r0_2 == 0) {
            func_ov017_02135eec(temp_r4, &func_ov017_02137110);
        }
    }
    if ((data_ov003_020e71b8->unk3D874 == 2) && (func_ov003_020c3bf0(temp_r4) == 0)) {
        func_ov017_021378b8(temp_r4, 0);
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk1C8;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    func_ov003_020c4628(temp_r4);
    ((Boss03*)temp_r4)->unk1F4 = (s16)((Boss03*)temp_r4)->unk6C;
    return ((Boss03*)temp_r4)->unk1CC;
}

s32 func_ov017_02137ba8(void* arg1) {
    func_ov003_020c4878(((Boss03*)arg1)->unk18);
    return 1;
}

s32 func_ov017_02137bbc(void* arg1) {
    func_ov003_020c48fc(((Boss03*)arg1)->unk18);
    return 1;
}

s32 func_ov017_02137bd0(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_021426fc = {
        .initialize = func_ov017_02137914,
        .update     = func_ov017_02137aa0,
        .render     = func_ov017_02137ba8,
        .cleanup    = func_ov017_02137bbc,
    };
    return data_ov017_021426fc.iter[stage](pool, task, args);
}

void func_ov017_02137c18(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1C8 = arg1;
    ((Boss03*)arg0)->unk1C4 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_02137c40(void* arg0) {
    func_ov017_02137c18(arg0, func_ov017_02137edc);
}

void func_ov017_02137c54(void* arg0) {
    s32 temp_r0;
    u32 temp_r1;
    u8  temp_r2;

    temp_r2 = ((Boss03*)arg0)->unk208;
    temp_r1 = (u32)(temp_r2 << 0x1D) >> 0x1F;
    if (temp_r1 != 0) {
        ((Boss03*)arg0)->unk208 = (u8)(temp_r2 & ~4);
        func_ov017_02137c18(arg0, func_ov017_021389d0);
        return;
    }
    temp_r0 = RNG_Next(0x64);
    if (temp_r0 < 0x23) {
        func_ov017_02137c18(arg0, func_ov017_02137fd0);
        return;
    }
    if (temp_r0 < 0x32) {
        func_ov017_02137c18(arg0, func_ov017_02138048);
        return;
    }
    func_ov017_02137c18(arg0, func_ov017_021389d0);
}

void func_ov017_02137cd4(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        data_ov003_020e71b8->unk3D875 = 2;
        ((Boss03*)arg0)->unk1C0       = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    }
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_02137c18(arg0, func_ov017_02137d64);
}

void func_ov017_02137d34(void* arg0) {
    if (((Boss03*)((Boss03*)arg0)->unk188)->unk4 == 0) {
        return;
    }
    ((Boss03*)arg0)->unk1CC = 0;
    ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 4);
    func_ov017_021394ac(arg0, 0);
}

void func_ov017_02137d64(void* arg0) {
    s16 temp_r2;
    u32 temp_r3;

    func_ov003_020c4b5c(arg0);
    if ((func_ov017_02130630(1) == 0) || ((s32)((s32)(((Boss03*)arg0)->unk6C * 0x64) / (s16)((Boss03*)arg0)->unk6E) < 0x1E)) {
        ((Boss03*)arg0)->unk208 = (u8)(((Boss03*)arg0)->unk208 & ~2);
        func_ov017_02137c18(arg0, func_ov017_02138b70);
        func_ov017_0213e3b8();
        return;
    }
    if (func_ov017_02139534(arg0, 0) != 0) {
        func_ov017_021394ac(arg0, 0);
        func_ov017_02137c18(arg0, func_ov017_021380dc);
        return;
    }
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk1C2 = func_ov003_020c42ec(arg0);
    }
    func_ov017_02138fdc(arg0, ((Boss03*)arg0)->unk1C0);
    if (((func_ov003_02085dc8(1) == arg0) || (func_ov003_02085e78(1) == arg0) || (func_ov003_02085ec0(1) == arg0) ||
         (func_ov003_02086014(1) == arg0)) &&
        ((u32)RNG_Next(0x64) < 0x14U))
    {
        ((Boss03*)arg0)->unk208 = (u8)(((Boss03*)arg0)->unk208 | 4);
    }
    temp_r3 = (u32)(((Boss03*)arg0)->unk208 << 0x1D) >> 0x1F;
    if ((temp_r3 != 0) || (temp_r2 = ((Boss03*)arg0)->unk1C0, ((s32)temp_r2 >= (s32)((Boss03*)arg0)->unk1C2))) {
        if ((temp_r3 != 0) || (func_ov003_020c4348(arg0) != 0)) {
            func_ov017_02137c54(arg0);
            return;
        }
        func_ov017_02137c40(arg0);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
}

void func_ov017_02137edc(void* arg0) {
    s32 var_r1;
    s16 temp_r2;
    s32 temp_r4;
    s32 temp_r6;

    temp_r6 = func_ov003_020cb744(0);
    temp_r2 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = func_ov003_020cb7a4(0);
    switch (temp_r2) { /* irregular */
        case 0:
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            ((Boss03*)arg0)->unk1D0 = (s32)(RNG_Next((temp_r6 >> 0xC) + 1) << 0xC);
            ((Boss03*)arg0)->unk1D4 = (s32)(RNG_Next((temp_r4 >> 0xC) + 1) << 0xC);
            var_r1                  = 0;
            ((Boss03*)arg0)->unk1D8 = 0;
            if ((s32)((Boss03*)arg0)->unk1D0 > (s32)((Boss03*)arg0)->unk28) {
                var_r1 = 1;
            }
            func_ov003_020c4ab4(arg0, var_r1);
            return;
        case 1:
            if (func_ov017_02138eb4(arg0, 1, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 2;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 2:
            func_ov017_02137c18(arg0, func_ov017_02137d64);
            return;
    }
}

void func_ov017_02137fd0(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_020c4b5c(arg0);
        func_ov003_02087f00(SEIDX_MINAMI_N_BTL_ATCS02, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (func_ov017_02139194(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
        func_ov017_02137c18(arg0, func_ov017_02137d64);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_02138048(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_020c4b5c(arg0);
        func_ov003_02087f00(SEIDX_MINAMI_N_BTL_ATCS03, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
        func_ov003_02087f00(SEIDX_SE_B006_KINNKYORI_KOUGEKI_L, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (func_ov017_021392b8(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
        func_ov017_02137c18(arg0, func_ov017_02137d64);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_021380dc(void* arg0) {
    s16   spA;
    s16   sp8;
    s32   var_r1;
    s16   temp_r0;
    s16   temp_r0_10;
    s16   temp_r0_13;
    s16   temp_r0_2;
    s16   temp_r0_3;
    s16   temp_r0_4;
    s16   temp_r0_6;
    s16   temp_r1;
    s32   temp_r0_11;
    s32   temp_r0_12;
    s32   temp_r0_5;
    s32   temp_r0_7;
    s32   temp_r0_8;
    s32   temp_r0_9;
    s32   temp_r2;
    s32   var_r0;
    s32   var_r0_2;
    s32   var_r0_3;
    s32   var_r0_4;
    s32   var_r0_5;
    s32   var_r0_6;
    s32   var_r0_7;
    s32   var_r0_8;
    s32   var_r0_9;
    void* temp_r4;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = data_ov003_020e71b8->unk3D898;
    switch (temp_r1) {
        default:
            return;
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(0, arg0 + 0x84, 8, 0);
                func_ov003_020824a0(arg0 + 0x84, 3, 1);
                ((Boss03*)arg0)->unk204 = BtlBoss03_EffSign_CreateTask(arg0);
                func_ov003_02087f00(SEIDX_MINAMI_N_BTL_FB06, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_00000, arg0 + 0x204);
            if (((Boss03*)arg0)->unk204 != -1) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            temp_r0_2 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_2 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_2 + 1);
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(0, arg0 + 0x84, 9, 0);
                func_ov003_020824a0(arg0 + 0x84, 2, 1);
                ((Boss03*)arg0)->unk1E8 = (s32)((Boss03*)arg0)->unk28;
                ((Boss03*)arg0)->unk1EC = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1F0 = (s32)((Boss03*)arg0)->unk30;
                if (((Boss03*)arg0)->unk24 == 0) {
                    ((Boss03*)arg0)->unk1DC = -0x8000;
                } else {
                    ((Boss03*)arg0)->unk1DC = 0x8000;
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 2:
            temp_r0_3 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_3 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_3 + 1);
                func_ov003_020824a0(arg0 + 0x84, 3, 0);
                BtlBoss03_EffPosture_CreateTask(arg0);
                func_ov003_02084694(arg0 + 0x144, 1);
                ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10000000);
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            if (func_ov017_02139550(arg0) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 3;
                return;
            }
            func_ov003_02084348(0, &spA, &sp8, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
            var_r0 = func_ov003_020cc7c0(spA, sp8, 0x40);
            if ((var_r0 == 0) && (((temp_r2 = ((Boss03*)arg0)->unk24, (temp_r2 == 0)) && ((s32)spA > 0x80)) ||
                                  ((temp_r2 == 1) && ((s32)spA < 0x80))))
            {
                var_r0 = 1;
            }
            if (var_r0 != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 0xB;
            return;
        case 3:
            temp_r0_4 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_4 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_4 + 1);
                func_ov003_020824a0(arg0 + 0x84, 4, 1);
                func_ov003_02087f00(SEIDX_SE_B006_TUKAMIKOUGEKI_01, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 4;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 4:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 5, 1);
                BtlBoss03_EffCatch_CreateTask(arg0);
            }
            if ((((u32)(((Boss03*)arg0)->unk208 << 0x1F) >> 0x1F) == 0) && (func_ov003_020cc354(temp_r4) == 0) &&
                (func_ov017_02139550(arg0) != 0))
            {
                ((Boss03*)arg0)->unk208 = (u8)((((Boss03*)arg0)->unk208 & ~1) | 1);
                func_ov003_020cc784(temp_r4, 0);
                func_ov003_02082f1c(temp_r4, 2);
                ((Boss03*)temp_r4)->unk54     = (s32)(((Boss03*)temp_r4)->unk54 | 0x20000);
                data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x80000);
                ((Boss03*)temp_r4)->unk10     = 0x12C;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_2 = 1;
                } else {
                    var_r0_2 = 0;
                }
                ((Boss03*)temp_r4)->unk24 = var_r0_2;
                func_ov003_020cc784(temp_r4, 0);
                ((Boss03*)temp_r4)->unk38 = 0;
            }
            if (((u32)(((Boss03*)arg0)->unk208 << 0x1F) >> 0x1F) != 0) {
                temp_r0_5 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_3 = temp_r0_5 - 0x48000;
                } else {
                    var_r0_3 = temp_r0_5 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0_3;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x20000);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 5;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 5:
            temp_r0_6 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_6 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_6 + 1);
                func_ov003_020824a0(arg0 + 0x84, 6, 1);
                if (((u32)(((Boss03*)arg0)->unk208 << 0x1F) >> 0x1F) != 0) {
                    temp_r0_7 = ((Boss03*)arg0)->unk28;
                    if (((Boss03*)arg0)->unk24 == 0) {
                        var_r0_4 = temp_r0_7 - 0x18000;
                    } else {
                        var_r0_4 = temp_r0_7 + 0x18000;
                    }
                    ((Boss03*)temp_r4)->unk28 = var_r0_4;
                    ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                    ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x70000);
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 6;
            return;
        case 6:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 7, 1);
                if (((u32)(((Boss03*)arg0)->unk208 << 0x1F) >> 0x1F) != 0) {
                    temp_r0_8 = ((Boss03*)arg0)->unk28;
                    if (((Boss03*)arg0)->unk24 == 0) {
                        var_r0_5 = temp_r0_8 - 0x18000;
                    } else {
                        var_r0_5 = temp_r0_8 + 0x18000;
                    }
                    ((Boss03*)temp_r4)->unk28 = var_r0_5;
                    ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                    ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x70000);
                }
            }
            if ((((u32)(((Boss03*)arg0)->unk208 << 0x1F) >> 0x1F) != 0) && (((Boss03*)arg0)->unk1C0 == 0xC)) {
                temp_r0_9 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_6 = temp_r0_9 - 0x48000;
                } else {
                    var_r0_6 = temp_r0_9 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0_6;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x50000);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 7;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 7:
            temp_r0_10 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_10 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_10 + 1);
                func_ov003_020824a0(arg0 + 0x84, 8, 1);
            }
            if (((u32)(((Boss03*)arg0)->unk208 << 0x1F) >> 0x1F) != 0) {
                temp_r0_11 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_7 = temp_r0_11 - 0x48000;
                } else {
                    var_r0_7 = temp_r0_11 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0_7;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x20000);
                ((Boss03*)temp_r4)->unk38 = 0x20000;
                ((Boss03*)arg0)->unk208   = (u8)(((Boss03*)arg0)->unk208 & ~1);
                func_ov003_020cc784(temp_r4, 1);
                func_ov003_02084694(arg0 + 0x144, 0);
                ((Boss03*)temp_r4)->unk10 = 0;
                func_ov003_020cc784(temp_r4, 1);
                func_ov017_0213e3a4();
                BtlBoss03_FailPly_CreateTask(arg0, temp_r4);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 8;
            return;
        case 8:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 9, 1);
                func_ov017_02139464(arg0, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
                temp_r0_12 = ((Boss03*)arg0)->unk1D0;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_8 = temp_r0_12 + 0x20000;
                } else {
                    var_r0_8 = temp_r0_12 - 0x20000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_8;
                ((Boss03*)arg0)->unk1C2 = func_ov017_02139408(arg0, 0x4000);
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
            ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
            if (((s32)((Boss03*)arg0)->unk1C0 >= (s32)((Boss03*)arg0)->unk1C2) || (func_ov003_020cc300(arg0, 0) == 0)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 9;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 9:
            temp_r0_13 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_13 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_13 + 1);
                func_ov003_020824a0(arg0 + 0x84, 0xA, 1);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_02137c18(arg0, func_ov017_02137d64);
            func_ov003_02084694(arg0 + 0x144, 0);
            ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10000000);
            return;
        case 11:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                if ((s32)((Boss03*)arg0)->unk28 < (s32)((Boss03*)temp_r4)->unk28) {
                    var_r0_9 = func_ov003_0208442c(0, 0x40);
                } else {
                    var_r0_9 = func_ov003_0208442c(0, 0xC0);
                }
                func_ov017_02139464(arg0, var_r0_9, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
                if ((s32)((Boss03*)arg0)->unk1D0 > (s32)((Boss03*)arg0)->unk28) {
                    var_r1 = 1;
                } else {
                    var_r1 = 0;
                }
                func_ov003_020c4ab4(arg0, var_r1);
            }
            if (func_ov017_02138eb4(arg0, 0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_02137c18(arg0, func_ov017_02137d64);
                ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10000000);
                func_ov003_02084694(arg0 + 0x144, 0);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_021389d0(void* arg0) {
    s16   temp_r2;
    s32   temp_r7;
    s32   var_r4;
    s32   var_r5;
    void* temp_r4;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = func_ov003_020cc750(0);
    switch (temp_r2) { /* irregular */
        case 0:
            if (func_ov017_0213903c(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r7 = ((Boss03*)temp_r4)->unk28;
                var_r5  = 0x50000;
                if ((u32)RNG_Next(0x64) < 0x32U) {
                    var_r5 = -0x50000;
                }
                ((Boss03*)arg0)->unk28 = (s32)(temp_r7 + var_r5);
                ((Boss03*)arg0)->unk2C = (s32)((Boss03*)temp_r4)->unk2C;
                ((Boss03*)arg0)->unk30 = (s32)((Boss03*)temp_r4)->unk30;
                if (func_ov003_020cc300(arg0, 0) == 0) {
                    ((Boss03*)arg0)->unk28 = (s32)(temp_r7 - var_r5);
                }
                if (((var_r5 < 0) && (((Boss03*)temp_r4)->unk24 == 0)) || ((var_r5 > 0) && (((Boss03*)temp_r4)->unk24 == 1))) {
                    var_r4 = 0x506;
                } else {
                    var_r4 = 0x507;
                }
                func_ov003_02087f00(var_r4, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                func_ov003_020c4b5c(arg0);
            }
            if (func_ov017_021390e4(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 2;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 2:
            if (func_ov017_02139194(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_02137c18(arg0, func_ov017_02137d64);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_02138b70(void* arg0) {
    SubroutineArgs subroutine_arg0;
    void*          temp_r4;

    temp_r4                = ((Boss03*)arg0)->unk188;
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10);
    func_ov003_02084694(arg0 + 0x144, 1);
    EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
    subroutine_arg0.unk0 = (s32)arg0;
    subroutine_arg0.unk4 = 0;
    subroutine_arg0.unk8 = 0;
    subroutine_arg0.unkC = 1;
    BtlBoss03_SeqEntry_CreateTask(&subroutine_arg0);
    ((Boss03*)arg0)->unk1CC  = 0;
    ((Boss03*)temp_r4)->unk8 = 1;
    ((Boss03*)temp_r4)->unk4 = 1;
    ((Boss03*)arg0)->unk54   = (s32)(((Boss03*)arg0)->unk54 | 4);
    func_ov003_02087ed8(0x2DE);
}

void func_ov017_02138bfc(void* arg0) {
    s16 temp_r1;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(0, arg0 + 0x84, 0xB, 0);
                func_ov003_020824a0(arg0 + 0x84, 2, 1);
            }
            EasyFade_FadeBothDisplays(0, 0, 0x1000);
            if (((s32)((Boss03*)arg0)->unk1C0 > 0) && (EasyFade_IsFading() == 0)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk208 = (u8)(((Boss03*)arg0)->unk208 | 2);
            func_ov017_02137c18(arg0, func_ov017_02137d64);
            return;
    }
}

void func_ov017_02138ce0(void* arg0) {
    s16   temp_r1;
    s32   var_r5;
    u8    temp_r0;
    void* temp_r3;
    void* temp_r3_2;

    temp_r1 = ((Boss03*)arg0)->unk1C0;
    if (temp_r1 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1 + 1);
        func_ov003_020cb520(arg0, 1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 9, 0);
        if ((SndMgr_IsSEPlaying(SEIDX_MINAMI_BTL_DMG01) == 0) && (SndMgr_IsSEPlaying(SEIDX_MINAMI_BTL_DMG02) == 0)) {
            if ((u32)RNG_Next(0x64) < 0x32U) {
                var_r5 = 0x4EF;
            } else {
                var_r5 = 0x4F0;
            }
            func_ov003_02087f00(var_r5, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
        }
        temp_r0 = ((Boss03*)arg0)->unk208;
        if (((u32)(temp_r0 << 0x1F) >> 0x1F) != 0) {
            ((Boss03*)arg0)->unk208 = (u8)(temp_r0 & ~1);
            func_ov003_020cc784(data_ov003_020e71b8->unk3D898, 1);
            func_ov003_02084694(arg0 + 0x144, 0);
            temp_r3                       = data_ov003_020e71b8->unk3D898;
            ((Boss03*)temp_r3)->unk54     = (s32)(((Boss03*)temp_r3)->unk54 & ~0x20000);
            temp_r3_2                     = data_ov003_020e71b8->unk3D898;
            ((Boss03*)temp_r3_2)->unk54   = (s32)(((Boss03*)temp_r3_2)->unk54 & ~0x4000);
            data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 & ~0x80000);
            func_ov003_0208afcc(0);
            ((Boss03*)data_ov003_020e71b8->unk3D898)->unk10 = 0;
        }
    }
    if (func_ov003_020c6b8c(arg0, 0) != 0) {
        return;
    }
    func_ov017_02137c18(arg0, func_ov017_02137d64);
}

void func_ov017_02138e44(void* arg0) {
    s16 temp_r1;

    temp_r1 = ((Boss03*)arg0)->unk1C0;
    if (temp_r1 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1 + 1);
        func_ov003_020cb520(arg0, 1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 8, 0);
    }
    if (func_ov003_020c72b4(arg0, 0, 0) != 0) {
        return;
    }
    func_ov017_02137c18(arg0, func_ov017_02137d64);
}

s32 func_ov017_02138eb4(void* arg0, s32 arg1, s16 arg2) {
    s16 temp_r0;

    if (arg2 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xA, 0);
        func_ov003_020824a0(arg0 + 0x84, 0, 0);
        ((Boss03*)arg0)->unk20A = 0;
        ((Boss03*)arg0)->unk20C = func_ov017_02139408(arg0, 0x4000);
    }
    ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
    ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
    ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
    temp_r0                = ((Boss03*)arg0)->unk9A;
    if ((temp_r0 != 3) && (temp_r0 != 6)) {

    } else {
        func_ov003_02087f00(SEIDX_SE_B006_FOOTSTEP, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (((s32)((Boss03*)arg0)->unk20A >= (s32)((Boss03*)arg0)->unk20C) || (func_ov017_02139474(arg0) < 0x4000) ||
        ((arg1 != 0) && (func_ov003_020cc300(arg0, 0) == 0)))
    {
        func_ov017_02138fdc(arg0, 0);
        return 0;
    }
    ((Boss03*)arg0)->unk20A = (s16)(((Boss03*)arg0)->unk20A + 1);
    return 1;
}

s32 func_ov017_02138fdc(void* arg0, s16 arg1) {
    if (arg1 == -1) {
        return 0;
    }
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 8, 0);
    }
    if (((Boss03*)arg0)->unkC8 != 0) {
        func_ov003_020824a0(arg0 + 0x84, 0, 0);
    }
    return 1;
}

s32 func_ov017_0213903c(void* arg0, s16 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10);
        func_ov003_02084694(arg0 + 0x144, 1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xB, 0);
        func_ov003_020824a0(arg0 + 0x84, 0, 1);
        func_ov003_02087f00(SEIDX_SE_B006_KUUKANN_TENNI_DISAPPEAR, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
        return 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    return 1;
}

s32 func_ov017_021390e4(void* arg0, s16 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C & ~1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xB, 0);
        func_ov003_020824a0(arg0 + 0x84, 1, 1);
        func_ov003_02087f00(SEIDX_SE_B006_KUUKANN_TENNI_APPEAR, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10);
        func_ov003_02084694(arg0 + 0x144, 0);
        return 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    return 1;
}

s32 func_ov017_02139194(void* arg0, s16 arg1) {
    s16 temp_r0;
    s32 var_r5;

    var_r5 = -0x40000;
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xC, 0);
        func_ov003_020824a0(arg0 + 0x84, 0, 1);
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    if (((Boss03*)arg0)->unk24 == 1) {
        var_r5 = 0x40000;
    }
    switch (temp_r0) { /* irregular */
        case 0:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 1, 1);
                func_ov003_02087f00(SEIDX_SE_B006_KINNKYORI_KOUGEKI_S, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
        default:
            return 1;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 2, 1);
            }
            func_ov003_020c5b2c(0xF3U, arg0, ((Boss03*)arg0)->unk28 + var_r5, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
            /* Duplicate return node #15. Try simplifying control flow for better match */
            return 1;
        case 2:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov017_02138fdc(arg0, 0);
                return 0;
            }
            /* Duplicate return node #15. Try simplifying control flow for better match */
            return 1;
    }
}

s32 func_ov017_021392b8(void* arg0, s32 arg1) {
    s16   temp_r0;
    s32   var_r4;
    s32   var_r5;
    void* temp_r6;

    var_r4  = -0x40000;
    var_r5  = -0x4000;
    temp_r6 = func_ov003_020cc750(0);
    if (((Boss03*)arg0)->unk24 == 1) {
        var_r4 = 0x40000;
        var_r5 = 0x4000;
    }
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xD, 0);
        func_ov003_020824a0(arg0 + 0x84, 0, 1);
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    switch (temp_r0) { /* irregular */
        case 0:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 1, 1);
            }
        default:
            return 1;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 2, 1);
            }
            if ((arg1 >= 8) && (arg1 <= 0x17) &&
                (func_ov003_020c5b2c(0xF4U, arg0, ((Boss03*)arg0)->unk28 + var_r4, ((Boss03*)arg0)->unk2C,
                                     ((Boss03*)arg0)->unk30) == 1))
            {
                ((Boss03*)temp_r6)->unk40 = var_r5;
                ((Boss03*)temp_r6)->unk48 = 0x19A;
            }
            /* Duplicate return node #18. Try simplifying control flow for better match */
            return 1;
        case 2:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov017_02138fdc(arg0, 0);
                return 0;
            }
            /* Duplicate return node #18. Try simplifying control flow for better match */
            return 1;
    }
}

s16 func_ov017_02139408(void* arg0, s32 arg1) {
    return func_ov003_020cb910(arg0 + 0x1DC, arg0 + 0x1E0, arg0 + 0x1E4, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C,
                               ((Boss03*)arg0)->unk30, ((Boss03*)arg0)->unk1D0, ((Boss03*)arg0)->unk1D4,
                               ((Boss03*)arg0)->unk1D8, arg1);
}

void func_ov017_02139464(void* arg0, s32 arg1, s32 arg2, s32 arg3) {
    ((Boss03*)arg0)->unk1D0 = arg1;
    ((Boss03*)arg0)->unk1D4 = arg2;
    ((Boss03*)arg0)->unk1D8 = arg3;
}

s32 func_ov017_02139474(void* arg0) {
    func_ov003_020cba54(((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30, ((Boss03*)arg0)->unk1D0,
                        ((Boss03*)arg0)->unk1D4, ((Boss03*)arg0)->unk1D8);
}

void func_ov017_021394ac(void* arg0, s32 arg1) {
    s16 temp_r5;

    temp_r5 = *(s32*)((char*)&data_ov017_0214357e + (arg1 * 4));
    ((Boss03*)(arg0 + (arg1 * 2)))->unk202 =
        (s16)(*(s32*)((char*)&data_ov017_0214357c + (arg1 * 4)) + ((0 - temp_r5) + RNG_Next((temp_r5 * 2) + 1)));
}

void func_ov017_02139500(void* arg0, s32 arg1) {
    s16*  temp_ip;
    s32   temp_r3;
    void* temp_r0;

    temp_ip              = arg0 + 0x202;
    temp_r3              = arg1 * 2;
    temp_r0              = arg0 + (arg1 * 2);
    *(temp_ip + temp_r3) = *(temp_ip + temp_r3) - 1;
    if ((s32)((Boss03*)temp_r0)->unk202 < 0) {
        ((Boss03*)temp_r0)->unk202 = 0;
    }
}

s32 func_ov017_02139534(void* arg0, s32 arg1) {
    if (((Boss03*)(arg0 + (arg1 * 2)))->unk202 == 0) {
        return 1;
    }
    return 0;
}

s32 func_ov017_02139550(void* arg0) {
    s32 var_ip;

    var_ip = 0x48000;
    if (((Boss03*)arg0)->unk24 == 0) {
        var_ip = -0x48000;
    }
    func_ov003_020cc38c(data_ov003_020e71b8->unk3D898, ((Boss03*)arg0)->unk28 + var_ip, ((Boss03*)arg0)->unk2C,
                        ((Boss03*)arg0)->unk30);
}

/// MARK: BtlBoss03_02_UG

s32 BtlBoss03_02_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_02_UG = {"Tsk_BtlBoss03_02_UG", BtlBoss03_02_UG_RunTask, 0x218};

s32 BtlBoss03_02_UG_Init(void* arg1, s32 arg2) {
    u8    temp_r1;
    void* temp_r4;
    void* temp_r5;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r5 = func_ov003_020cb42c(0, 0x2C, 0);
    MI_CpuSet(temp_r4, 0, 0x218);
    if (data_ov003_020e71b8->unk3D874 == 0) {
        func_ov017_0213df24(0);
        BtlBoss03_KindanMgr_CreateTask();
    }
    func_ov017_0213e17c(0xF, 0);
    func_ov017_0213e17c(0x10, 0);
    func_ov017_0213e088(8, 0);
    func_ov017_0213f20c(1);
    func_ov003_020c3efc(temp_r4, arg2);
    func_ov003_020c4520(temp_r4);
    func_ov003_020c4b5c(temp_r4);
    ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 0x40000020);
    ((Boss03*)temp_r4)->unk188 = (void*)(temp_r4 + 0x1F4);
    ((Boss03*)temp_r4)->unk1CC = 1;
    ((Boss03*)temp_r4)->unk210 = -1;
    func_ov017_021394ac(temp_r4, 0);
    temp_r1 = ((Boss03*)temp_r4)->unk208;
    if (data_ov003_020e71b8->unk3D874 == 0) {
        ((Boss03*)temp_r4)->unk208 = (u8)(temp_r1 | 2);
        Input_PollState(&InputStatus);
        if (!(InputStatus.buttonState.currButtons & 1)) {
            func_ov017_02137c18(temp_r4, func_ov017_02137cd4);
            ((Boss03*)temp_r4)->unk28 = -0x500000;
        } else {
            ((Boss03*)temp_r4)->unk18C = (u16)(((Boss03*)temp_r4)->unk18C | 1);
            ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 0x10);
            func_ov003_02084694(temp_r4 + 0x144, 1);
            func_ov017_02137c18(temp_r4, func_ov017_02137d34);
            func_ov003_02082cc4(temp_r4 + 0x84);
        }
    } else {
        ((Boss03*)temp_r4)->unk208 = (u8)(temp_r1 & ~2);
        ((Boss03*)temp_r4)->unk6C  = (s16)((Boss03*)temp_r5)->unk6C;
        if (!(((Boss03*)temp_r5)->unk18C & 1)) {
            ((Boss03*)temp_r4)->unk28 = (s32)((Boss03*)temp_r5)->unk28;
            ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)temp_r5)->unk2C;
            ((Boss03*)temp_r4)->unk30 = (s32)((Boss03*)temp_r5)->unk30;
            func_ov017_02137c18(temp_r4, func_ov017_02138bfc);
        } else {
            ((Boss03*)temp_r4)->unk18C = (u16)(((Boss03*)temp_r4)->unk18C | 1);
            ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 0x10);
            func_ov003_02084694(temp_r4 + 0x144, 1);
            func_ov017_02137c18(temp_r4, func_ov017_02137d34);
            func_ov003_02082cc4(temp_r4 + 0x84);
        }
    }
    return 1;
}

s32 BtlBoss03_02_UG_Update(void* arg1) {
    void (*temp_r1_2)(void*);
    s16   temp_r1;
    s16*  temp_r5;
    s32   temp_r0;
    s32   temp_r0_2;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r5 = ((Boss03*)temp_r4)->unk188;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        if (((Boss03*)temp_r4)->unk20 == 6) {
            func_ov003_02082f2c(temp_r4);
        }
        return 1;
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    switch (temp_r0) { /* irregular */
        case 3:
            break;
        case 2:
            if (((u32)(((Boss03*)temp_r4)->unk208 << 0x1E) >> 0x1F) == 0) {
                ((Boss03*)temp_r4)->unk38 = 0;
            } else {
                func_ov003_02084694(temp_r4 + 0x144, 0);
                ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 & ~0x10000000);
                func_ov017_02137c18(temp_r4, func_ov017_02138ce0);
            }
            break;
        case 6:
            func_ov003_02084694(temp_r4 + 0x144, 0);
            temp_r0_2                 = (((Boss03*)temp_r4)->unk54 & ~0x10000000) | 1;
            ((Boss03*)temp_r4)->unk54 = temp_r0_2;
            if (temp_r0_2 == 0) {
                func_ov017_02137c18(temp_r4, func_ov017_02138e44);
            }
            break;
    }
    if ((data_ov003_020e71b8->unk3D874 == 2) && (func_ov003_020c3bf0(temp_r4) == 0)) {
        func_ov017_02139500(temp_r4, 0);
    }
    if (((Boss03*)temp_r4)->unk210 != -1) {
        ((Boss03*)temp_r4)->unk214 = (s32)(((Boss03*)temp_r4)->unk214 + 1);
    }
    temp_r1 = ((Boss03*)temp_r4)->unk200 - ((Boss03*)temp_r4)->unk6C;
    if ((s32)temp_r1 > 0) {
        *temp_r5 += temp_r1;
    }
    ((Boss03*)temp_r4)->unk200 = (s16)((Boss03*)temp_r4)->unk6C;
    temp_r1_2                  = ((Boss03*)temp_r4)->unk1C8;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    func_ov003_020c4668(temp_r4);
    return ((Boss03*)temp_r4)->unk1CC;
}

s32 BtlBoss03_02_UG_Render(void* arg1) {
    s32   temp_r1;
    s32   var_z;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r1 = ((Boss03*)temp_r4)->unk210;
    var_z   = temp_r1 == -1;
    if (temp_r1 != -1) {
        var_z = ((Boss03*)temp_r4)->unk214 == 0;
    }
    if (var_z) {
        func_ov003_020c48b0();
    } else {
        func_ov003_020c4748();
        func_ov003_0208810c(temp_r4 + 0xE4, temp_r4);
    }
    return 1;
}

s32 BtlBoss03_02_UG_Destroy(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov003_020c492c(temp_r4);
    if (((Boss03*)temp_r4)->unk1FC == 0) {
        func_ov017_0213e074();
    }
    return 1;
}

s32 BtlBoss03_02_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_02142754 = {
        .initialize = BtlBoss03_02_UG_Init,
        .update     = BtlBoss03_02_UG_Update,
        .render     = BtlBoss03_02_UG_Render,
        .cleanup    = BtlBoss03_02_UG_Destroy,
    };
    return data_ov017_02142754.iter[stage](pool, task, args);
}

void func_ov017_021399dc(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1C8 = arg1;
    ((Boss03*)arg0)->unk1C4 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_02139a04(void* arg0) {
    func_ov017_021399dc(arg0, func_ov017_02139d14);
    ((Boss03*)arg0)->unk1F8 = (s16)(((Boss03*)arg0)->unk1F8 + 1);
    if (((Boss03*)arg0)->unk1F8 != 3) {
        return;
    }
    func_ov017_0213acc8(arg0, 3);
    ((Boss03*)arg0)->unk1F8 = 0;
}

void func_ov017_02139a50(void* arg0) {
    s32 temp_r0;

    if ((s32)((Boss03*)arg0)->unk1E8 >= 2) {
        ((Boss03*)arg0)->unk1E8 = 0;
        func_ov017_021399dc(arg0, func_ov017_0213a01c);
        return;
    }
    temp_r0 = RNG_Next(0x64);
    if ((s32)((Boss03*)arg0)->unk30 < 0) {
        func_ov017_021399dc(arg0, func_ov017_02139de0);
    } else if (temp_r0 < 0x41) {
        func_ov017_021399dc(arg0, func_ov017_02139de0);
    } else {
        func_ov017_021399dc(arg0, func_ov017_02139f1c);
    }
    ((Boss03*)arg0)->unk1F8 = 0;
}

void func_ov017_02139ae0(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        data_ov003_020e71b8->unk3D875 = 2;
        ((Boss03*)arg0)->unk1C0       = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    }
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_021399dc(arg0, func_ov017_02139b40);
}

void func_ov017_02139b40(void* arg0) {
    s16 temp_r2;
    u32 temp_r0;

    func_ov003_020c4b1c(arg0);
    if (((Boss03*)arg0)->unk1EA == 2) {
        func_ov017_021399dc(arg0, func_ov017_02139c54);
        return;
    }
    if (func_ov017_0213ef4c() > 0) {
        func_ov017_021399dc(arg0, func_ov017_0213a458);
        return;
    }
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk1C2 = func_ov003_020c42ec(arg0);
        if (((Boss03*)arg0)->unk1EA == 1) {
            temp_r0                 = ((Boss03*)arg0)->unk1C2 * 3;
            ((Boss03*)arg0)->unk1C2 = (s16)((s32)(temp_r0 + (temp_r0 >> 0x1F)) >> 1);
        }
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
    }
    func_ov017_0213a7a0(arg0, ((Boss03*)arg0)->unk1C0);
    temp_r2 = ((Boss03*)arg0)->unk1C0;
    if ((s32)temp_r2 >= (s32)((Boss03*)arg0)->unk1C2) {
        if (func_ov003_020c4348(arg0) != 0) {
            func_ov017_02139a50(arg0);
            return;
        }
        if (func_ov017_0213ad4c(arg0, 3) != 0) {
            func_ov017_02139a04(arg0);
            return;
        }
        ((Boss03*)arg0)->unk1C0 = 1;
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
}

void func_ov017_02139c54(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 8, 0);
        func_ov003_020824a0(arg0 + 0x84, 2, 0);
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
    }
    if (func_ov017_0213ef4c() > 0) {
        func_ov017_021399dc(arg0, func_ov017_0213a458);
        return;
    }
    if (((Boss03*)arg0)->unk1EA != 2) {
        if (func_ov017_0213ef4c() > 0) {
            func_ov017_021399dc(arg0, func_ov017_0213a458);
            return;
        }
        func_ov017_021399dc(arg0, func_ov017_02139b40);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_02139d14(void* arg0) {
    s16 temp_r2;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r2) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x40000000);
                ((Boss03*)arg0)->unk1FA = (u8)(((Boss03*)arg0)->unk1FA & ~8);
            }
            if (func_ov017_0213a818(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (func_ov017_0213a8e0(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_021399dc(arg0, func_ov017_02139b40);
                ((Boss03*)arg0)->unk1FA = (u8)(((Boss03*)arg0)->unk1FA | 8);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_02139de0(void* arg0) {
    s16 temp_r0;
    s16 temp_r2;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r2) { /* irregular */
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                if ((func_ov017_0213ada0() != 0) && (((Boss03*)arg0)->unk30 != 0)) {
                    /* Duplicate return node #11. Try simplifying control flow for better match */
                    func_ov017_021399dc(arg0, func_ov017_02139d14);
                    return;
                }
                if ((func_ov017_0213ada0() == 0) && (((Boss03*)arg0)->unk30 == 0)) {
                    func_ov017_021399dc(arg0, func_ov017_02139d14);
                    return;
                }
                ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
                goto block_13;
            }
        block_13:
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                ((Boss03*)arg0)->unk1E8 = (s16)(((Boss03*)arg0)->unk1E8 + 1);
                func_ov003_020c4b1c(arg0);
                func_ov003_02087f00(SEIDX_MINAMI_N_BTL_ATCM03, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
                func_ov003_02087f00(SEIDX_SE_B007_KINNKYORI_KOUGEKI_S, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
            }
            if (func_ov017_0213a9cc(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_021399dc(arg0, func_ov017_02139b40);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_02139f1c(void* arg0) {
    s16 temp_r0;
    s16 temp_r2;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r2) { /* irregular */
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
            }
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                ((Boss03*)arg0)->unk1E8 = (s16)(((Boss03*)arg0)->unk1E8 + 1);
                func_ov003_020c4b1c(arg0);
                func_ov003_02087f00(SEIDX_MINAMI_N_BTL_ATCM04, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
                func_ov003_02087f00(SEIDX_SE_B007_KINNKYORI_KOUGEKI_L, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
            }
            if (func_ov017_0213ab24(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_021399dc(arg0, func_ov017_02139b40);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_0213a01c(void* arg0) {
    f32   var_r0_2;
    s16   temp_r0;
    s16   temp_r0_2;
    s16   temp_r0_3;
    s16   temp_r0_7;
    s16   temp_r2;
    s16   temp_r2_2;
    s16   temp_r2_3;
    s32   temp_r0_5;
    s32   var_r0;
    s32   var_r0_3;
    s32   var_r0_4;
    u8    temp_r0_4;
    void* temp_r0_6;

    temp_r0 = ((Boss03*)arg0)->unk1C4;
    switch (temp_r0) {
        default:
            return;
        case 0:
            temp_r0_2 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_2 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_2 + 1);
                ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
                ((Boss03*)arg0)->unk1FA = (u8)(((Boss03*)arg0)->unk1FA & ~8);
            }
            if (func_ov003_020c6230(arg0) != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            temp_r0_3 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_3 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_3 + 1);
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(1, arg0 + 0x84, 9, 0);
                func_ov003_020824a0(arg0 + 0x84, 0xC, 1);
                func_ov003_02087f00(SEIDX_MINAMI_N_BTL_FB06, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
                func_ov003_02087f00(SEIDX_SE_B007_BLION_TOSSHIN_KOUGEKI, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 2:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 0xD, 0);
                if ((s32)((Boss03*)arg0)->unk28 < (s32)(func_ov003_020cb744(1) >> 1)) {
                    var_r0 = func_ov003_020cb764(0) + 0x20000;
                } else {
                    var_r0 = func_ov003_020cb764(0) - 0x20000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0;
                ((Boss03*)arg0)->unk1D4 = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1D8 = (s32)((Boss03*)arg0)->unk30;
                ((Boss03*)arg0)->unk1C2 = func_ov017_0213ac5c(arg0, 0x8000);
                ((Boss03*)arg0)->unk1F4 = BtlBoss03_EffRush_CreateTask(arg0);
            }
            temp_r0_4 = ((Boss03*)func_ov003_0208a114(0xFD))->unk7;
            temp_r0_5 = temp_r0_4 << 0xC;
            if (temp_r0_4 != 0) {
                var_r0_2 = 0.5f + (f32)temp_r0_5;
            } else {
                var_r0_2 = (f32)temp_r0_5 - 0.5f;
            }
            func_ov003_020c5b2c(0xFDU, arg0, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C,
                                ((Boss03*)arg0)->unk30 - (s32)var_r0_2);
            temp_r2 = ((Boss03*)arg0)->unk1C0;
            if ((s32)temp_r2 < (s32)((Boss03*)arg0)->unk1C2) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 3;
            return;
        case 3:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 0xE, 1);
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_3 = func_ov003_020cb764(0) - 0x40000;
                } else {
                    var_r0_3 = func_ov003_020cb764(0) + 0x40000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_3;
                ((Boss03*)arg0)->unk1D4 = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1D8 = (s32)((Boss03*)arg0)->unk30;
                ((Boss03*)arg0)->unk1C2 = func_ov017_0213ac5c(arg0, 0x4000);
                EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, arg0 + 0x1F4);
                temp_r0_6 = EasyTask_GetTaskById(&data_ov003_020e71b8->taskPool, ((Boss03*)arg0)->unk1F4);
                if (temp_r0_6 != NULL) {
                    ((Boss03*)temp_r0_6)->unk4 = (u16)(((Boss03*)temp_r0_6)->unk4 | 0x10);
                }
            }
            temp_r2_2 = ((Boss03*)arg0)->unk1C0;
            if ((s32)temp_r2_2 < (s32)((Boss03*)arg0)->unk1C2) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2_2 + 1);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 4;
            return;
        case 4:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 0xF, 0);
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_4 = func_ov003_020cb764(0) - 0x50000;
                } else {
                    var_r0_4 = func_ov003_020cb764(0) + 0x50000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_4;
                ((Boss03*)arg0)->unk1D4 = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1D8 = (s32)((Boss03*)arg0)->unk30;
                ((Boss03*)arg0)->unk1C2 = func_ov017_0213ac5c(arg0, 0x2000);
            }
            temp_r2_3 = ((Boss03*)arg0)->unk1C0;
            if ((s32)temp_r2_3 < (s32)((Boss03*)arg0)->unk1C2) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2_3 + 1);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 5;
            ((Boss03*)arg0)->unk28  = (s32)((Boss03*)arg0)->unk1D0;
            ((Boss03*)arg0)->unk1E4 = 0;
            ((Boss03*)arg0)->unk1E0 = 0;
            ((Boss03*)arg0)->unk1DC = 0;
            return;
        case 5:
            temp_r0_7 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_7 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_7 + 1);
                func_ov003_020824a0(arg0 + 0x84, 0x10, 1);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_021399dc(arg0, func_ov017_02139b40);
            ((Boss03*)arg0)->unk1FA = (u8)(((Boss03*)arg0)->unk1FA | 8);
            return;
    }
}

void func_ov017_0213a458(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 9, 0);
        func_ov003_020824a0(arg0 + 0x84, 0xB, 1);
        ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x40000000);
        ((Boss03*)arg0)->unk1FA = (u8)(((Boss03*)arg0)->unk1FA & ~8);
    }
    if ((((Boss03*)arg0)->unk9A == 0xD) && (((Boss03*)arg0)->unk8C == 1)) {
        if (func_ov017_0213ef4c() > 0) {
            do {
                func_ov017_02140cbc(arg0);
            } while (func_ov017_0213ef4c() > 0);
        }
    }
    if (((Boss03*)arg0)->unk1C0 == 0x16) {
        func_ov003_02087f00(SEIDX_SE_B005_BADGE_NAGE01, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        func_ov017_021399dc(arg0, func_ov017_02139b40);
        ((Boss03*)arg0)->unk1FA = (u8)(((Boss03*)arg0)->unk1FA & ~8);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_0213a558(void* arg0) {
    s16 temp_r1 = ((Boss03*)arg0)->unk1C4;

    switch (temp_r1) { /* irregular */
        case 0:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(1, arg0 + 0x84, 0xB, 0);
                func_ov003_020824a0(arg0 + 0x84, 2, 1);
                ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x40000000);
                ((Boss03*)arg0)->unk1FA = (u8)(((Boss03*)arg0)->unk1FA & ~8);
            }
            EasyFade_FadeBothDisplays(0, 0, 0x1000);
            if (((s32)((Boss03*)arg0)->unk1C0 > 0) && (EasyFade_IsFading() == 0)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 1;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_021399dc(arg0, func_ov017_02139b40);
            if ((s32)((Boss03*)arg0)->unk6C <= 0) {
                ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x2000);
                func_ov003_020c4fc8(arg0);
            }
            ((Boss03*)arg0)->unk1FA = (u8)(((Boss03*)arg0)->unk1FA | 8);
            return;
    }
}

void func_ov017_0213a674(void* arg0) {
    s16   temp_r0;
    void* temp_r4;

    temp_r0 = ((Boss03*)arg0)->unk1C0;
    temp_r4 = ((Boss03*)arg0)->unk188;
    if (temp_r0 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 8, 0);
        func_ov003_020824a0(arg0 + 0x84, 1, 1);
        ((Boss03*)arg0)->unk1E4 = 0;
        ((Boss03*)arg0)->unk1E0 = 0;
        ((Boss03*)arg0)->unk1DC = 0;
    }
    if (func_ov003_020c703c(arg0) == 0) {
        data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x20000000);
        ((Boss03*)temp_r4)->unk4      = 1;
        ((Boss03*)arg0)->unk1CC       = 0;
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_0213a728(void* arg0) {
    s16 temp_r0;

    temp_r0 = ((Boss03*)arg0)->unk1C0;
    if (temp_r0 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 8, 0);
        ((Boss03*)arg0)->unk1E4 = 0;
        ((Boss03*)arg0)->unk1E0 = 0;
        ((Boss03*)arg0)->unk1DC = 0;
    }
    if (func_ov003_020c72b4(arg0, 0, 0) != 0) {
        return;
    }
    func_ov017_021399dc(arg0, func_ov017_02139b40);
}

s32 func_ov017_0213a7a0(void* arg0, s16 arg1) {
    if (arg1 == -1) {
        return 0;
    }
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 8, 0);
        if (((Boss03*)arg0)->unk1EA == 0) {
            func_ov003_020824a0(arg0 + 0x84, 1, 0);
        } else {
            func_ov003_020824a0(arg0 + 0x84, 0, 0);
        }
    }
    return 1;
}

s32 func_ov017_0213a818(void* arg0, s16 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10);
        func_ov003_02084694(arg0 + 0x144, 1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xB, 0);
        func_ov003_020824a0(arg0 + 0x84, 0, 1);
        if (data_ov003_020e71b8->unk3D8A0 == arg0) {
            data_ov003_020e71b8->unk3D8A0 = 0;
        }
        func_ov003_02087f00(SEIDX_SE_B006_KUUKANN_TENNI_DISAPPEAR, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
        return 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    return 1;
}

s32 func_ov017_0213a8e0(void* arg0, s16 arg1) {
    s32 var_r0;

    if (arg1 == 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C & ~1);
        func_ov017_021302f0(arg0);
        if (func_ov017_0213ada0() != 0) {
            var_r0 = 0;
        } else {
            var_r0 = -0x40000;
        }
        ((Boss03*)arg0)->unk30 = var_r0;
        func_ov003_020c4b1c(arg0);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xB, 0);
        func_ov003_020824a0(arg0 + 0x84, 1, 1);
        func_ov003_02087f00(SEIDX_SE_B006_KUUKANN_TENNI_APPEAR, func_ov003_020843b0(1, ((Boss03*)arg0)->unk28));
        ((Boss03*)arg0)->unk1AC = (s32)((Boss03*)arg0)->unk28;
        ((Boss03*)arg0)->unk1B0 = (s32)((Boss03*)arg0)->unk2C;
        ((Boss03*)arg0)->unk1B4 = (s32)((Boss03*)arg0)->unk30;
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        func_ov003_02084694(arg0 + 0x144, 0);
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10);
        return 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    return 1;
}

s32 func_ov017_0213a9cc(void* arg0, s16 arg1) {
    f32 var_r0;
    s16 temp_r0;
    s32 temp_r0_3;
    s32 var_r4;
    u8  temp_r0_2;

    var_r4 = -0x40000;
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xC, 0);
        func_ov003_020824a0(arg0 + 0x84, 3, 1);
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    if (((Boss03*)arg0)->unk24 == 1) {
        var_r4 = 0x40000;
    }
    switch (temp_r0) { /* irregular */
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 4, 1);
            }
        default:
            return 1;
        case 4:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 5, 1);
            }
            temp_r0_2 = ((Boss03*)func_ov003_0208a114(0xF9))->unk7;
            temp_r0_3 = temp_r0_2 << 0xC;
            if (temp_r0_2 != 0) {
                var_r0 = 0.5f + (f32)temp_r0_3;
            } else {
                var_r0 = (f32)temp_r0_3 - 0.5f;
            }
            func_ov003_020c5b2c(0xF90000U >> 0x10, arg0, ((Boss03*)arg0)->unk28 + var_r4, ((Boss03*)arg0)->unk2C,
                                ((Boss03*)arg0)->unk30 - (s32)var_r0);
            /* Duplicate return node #18. Try simplifying control flow for better match */
            return 1;
        case 5:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov017_0213a7a0(arg0, 0);
                return 0;
            }
            /* Duplicate return node #18. Try simplifying control flow for better match */
            return 1;
    }
}

s32 func_ov017_0213ab24(void* arg0, s32 arg1) {
    s16 temp_r0;
    s32 var_r5;

    var_r5 = -0x40000;
    func_ov003_020cc750(1);
    if (((Boss03*)arg0)->unk24 == 1) {
        var_r5 = 0x40000;
    }
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(1, arg0 + 0x84, 0xD, 0);
        func_ov003_020824a0(arg0 + 0x84, 3, 1);
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    switch (temp_r0) { /* irregular */
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 4, 1);
            }
        default:
            return 1;
        case 4:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 5, 1);
            }
            if ((arg1 >= 8) && (arg1 <= 0x17)) {
                func_ov003_0208a114(0xFA);
                func_ov003_020c5b2c(0xFAU, arg0, ((Boss03*)arg0)->unk28 + var_r5, ((Boss03*)arg0)->unk2C,
                                    ((Boss03*)arg0)->unk30 - 0x20000);
            }
            /* Duplicate return node #17. Try simplifying control flow for better match */
            return 1;
        case 5:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov017_0213a7a0(arg0, 0);
                return 0;
            }
            /* Duplicate return node #17. Try simplifying control flow for better match */
            return 1;
    }
}

s16 func_ov017_0213ac5c(void* arg0, s32 arg1) {
    return func_ov003_020cb910(arg0 + 0x1DC, arg0 + 0x1E0, arg0 + 0x1E4, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C,
                               ((Boss03*)arg0)->unk30, ((Boss03*)arg0)->unk1D0, ((Boss03*)arg0)->unk1D4,
                               ((Boss03*)arg0)->unk1D8, arg1);
}

void func_ov017_0213acb8(void* arg0, s32 arg1, s16 arg2) {
    ((Boss03*)(arg0 + (arg1 * 2)))->unk1EC = arg2;
}

void func_ov017_0213acc8(void* arg0, s32 arg1) {
    s16 temp_r5;

    temp_r5 = *(s32*)((char*)&data_ov017_02142772 + (arg1 * 4));
    ((Boss03*)(arg0 + (arg1 * 2)))->unk1EC =
        (s16)(*(s32*)((char*)&data_ov017_02142770 + (arg1 * 4)) + ((0 - temp_r5) + RNG_Next((temp_r5 * 2) + 1)));
}

void func_ov017_0213ad1c(void* arg0, s32 arg1) {
    s32*  temp_ip;
    s32   temp_r3;
    void* temp_r0;

    temp_ip = arg0 + 0x1EC;
    temp_r3 = arg1 * 2;
    temp_r0 = arg0 + (arg1 * 2);
    temp_ip[temp_r3]--;
    if ((s32)((Boss03*)temp_r0)->unk1EC < 0) {
        ((Boss03*)temp_r0)->unk1EC = 0;
    }
}

s32 func_ov017_0213ad4c(void* arg0, s32 arg1) {
    if (((Boss03*)(arg0 + (arg1 * 2)))->unk1EC == 0) {
        return 1;
    }
    return 0;
}

void func_ov017_0213ad68(void* arg0, s32 arg1) {
    ((Boss03*)(arg0 + (arg1 * 2)))->unk1EC = 0;
}

void func_ov017_0213ad7c(void* arg0) {
    u8 temp_r0;
    u8 var_r0;

    temp_r0 = ((Boss03*)arg0)->unk1FA;
    if (func_ov017_0213eeb8() == 1) {
        var_r0 = temp_r0 | 4;
    } else {
        var_r0 = temp_r0 | 2;
    }
    ((Boss03*)arg0)->unk1FA = var_r0;
}

s32 func_ov017_0213ada0(void) {
    if (((Boss03*)data_ov003_020e71b8->unk3D89C)->unk30 > -0x20000) {
        return 1;
    }
    return 0;
}

s32 BtlBoss03_03_RG_Init(void* arg1, s32 arg2) {
    s32   var_r4;
    void* temp_r4;
    void* temp_r5;

    temp_r5 = ((Boss03*)arg1)->unk18;
    temp_r4 = func_ov003_020cb42c(1, 0x2C, 0);
    MI_CpuSet(temp_r5, 0, 0x1FC);
    func_ov003_020c3efc(temp_r5, arg2);
    func_ov003_020c44ac(temp_r5);
    func_ov003_020c4b1c(temp_r5);
    func_ov017_0213ed98((s32)func_ov017_0213ad7c, (s32)temp_r5);
    ((Boss03*)temp_r5)->unk54 = (s32)(((Boss03*)temp_r5)->unk54 | 0x20);
    func_ov003_020cb520(temp_r5, 0);
    ((Boss03*)temp_r5)->unk1CC = 1;
    ((Boss03*)temp_r5)->unk1FA = (u8)(((Boss03*)temp_r5)->unk1FA & ~1);
    if (data_ov003_020e71b8->unk3D874 == 0) {
        func_ov017_021302f0(temp_r5);
        func_ov017_021399dc(temp_r5, func_ov017_02139ae0);
    } else {
        ((Boss03*)temp_r5)->unk6C = (s16)((Boss03*)temp_r4)->unk6C;
        func_ov017_021399dc(temp_r5, func_ov017_0213a558);
        ((Boss03*)temp_r5)->unk28 = (s32)((Boss03*)temp_r4)->unk28;
        ((Boss03*)temp_r5)->unk2C = (s32)((Boss03*)temp_r4)->unk2C;
        ((Boss03*)temp_r5)->unk30 = (s32)((Boss03*)temp_r4)->unk30;
    }
    var_r4 = 0;
    do {
        func_ov017_0213acc8(temp_r5, var_r4);
        var_r4 += 1;
    } while (var_r4 < 4);
    ((Boss03*)temp_r5)->unk62 = 0x32;
    return 1;
}

s32 BtlBoss03_03_RG_Update(void* arg1) {
    void (*temp_r1_2)(void*);
    s16   temp_r0_4;
    s32   temp_r0;
    s32   temp_r0_2;
    s32   var_r5;
    u8    temp_r0_3;
    u8    temp_r1;
    void* temp_r4;

    temp_r4                    = ((Boss03*)arg1)->unk18;
    ((Boss03*)temp_r4)->unk188 = ((Boss03*)((Boss03*)temp_r4)->unk184)->unk188;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        if (((Boss03*)temp_r4)->unk20 == 6) {
            func_ov003_02082f2c(temp_r4);
        }
        return 1;
    }
    if ((((u32)(((Boss03*)temp_r4)->unk1FA << 0x1F) >> 0x1F) == 0) && (func_ov003_020cb648(temp_r4, 0xA) != 0)) {
        ((Boss03*)temp_r4)->unk1FA = (u8)((((Boss03*)temp_r4)->unk1FA & ~1) | 1);
        func_ov003_02087f00(SEIDX_MINAMI_N_BTL_FB05, func_ov003_020843b0(1, ((Boss03*)temp_r4)->unk28));
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    switch (temp_r0) { /* switch 1; irregular */
        case 3:        /* switch 1 */
            func_ov003_02084694(temp_r4 + 0x144, 1);
            func_ov017_021399dc(temp_r4, func_ov017_0213a674);
            break;
        case 6: /* switch 1 */
            temp_r0_2                 = ((Boss03*)temp_r4)->unk54 | 1;
            ((Boss03*)temp_r4)->unk54 = temp_r0_2;
            if (temp_r0_2 == 0) {
                func_ov017_021399dc(temp_r4, func_ov017_0213a728);
            }
            break;
    }
    if (((Boss03*)temp_r4)->unk1EA != 2) {
        temp_r0_3 = ((Boss03*)temp_r4)->unk1FA;
        if ((((u32)(temp_r0_3 << 0x1E) >> 0x1F) != 1) && (((u32)(temp_r0_3 << 0x1D) >> 0x1F) != 1)) {

        } else {
            ((Boss03*)temp_r4)->unk1EA = 1;
            func_ov017_0213ad68(temp_r4, 2);
        }
    }
    if ((data_ov003_020e71b8->unk3D874 == 2) && (func_ov003_020c3bf0(temp_r4) == 0)) {
        var_r5 = 0;
        do {
            func_ov017_0213ad1c(temp_r4, var_r5);
            var_r5 += 1;
        } while (var_r5 < 4);
    }
    temp_r0_4 = ((Boss03*)temp_r4)->unk1EA;
    switch (temp_r0_4) { /* switch 2; irregular */
        case 0:          /* switch 2 */
            if (func_ov017_0213ad4c(temp_r4, 1) != 0) {
                func_ov017_0213acc8(temp_r4, 2);
                ((Boss03*)temp_r4)->unk1EA = 1;
                ((Boss03*)temp_r4)->unk62  = 0x32;
            }
            break;
        case 1: /* switch 2 */
            if (func_ov017_0213ad4c(temp_r4, 2) != 0) {
                temp_r1 = ((Boss03*)temp_r4)->unk1FA;
                if (((u32)(temp_r1 << 0x1E) >> 0x1F) == 1) {
                    ((Boss03*)temp_r4)->unk1FA = (u8)(temp_r1 & ~2);
                    func_ov017_0213acb8(temp_r4, 0, 0xF0);
                } else if (((u32)(temp_r1 << 0x1D) >> 0x1F) == 1) {
                    ((Boss03*)temp_r4)->unk1FA = (u8)(temp_r1 & ~4);
                    func_ov017_0213acb8(temp_r4, 0, 0x12C);
                } else {
                    func_ov017_0213acc8(temp_r4, 0);
                }
                ((Boss03*)temp_r4)->unk1EA = 2;
                ((Boss03*)temp_r4)->unk62  = 0x96;
            }
            break;
        case 2: /* switch 2 */
            if (func_ov017_0213ad4c(temp_r4, 0) != 0) {
                func_ov017_0213acc8(temp_r4, 1);
                ((Boss03*)temp_r4)->unk1EA = 0;
                ((Boss03*)temp_r4)->unk62  = 0x32;
            }
            break;
    }
    ((Boss03*)temp_r4)->unk28 = (s32)(((Boss03*)temp_r4)->unk28 + ((Boss03*)temp_r4)->unk1DC);
    ((Boss03*)temp_r4)->unk2C = (s32)(((Boss03*)temp_r4)->unk2C + ((Boss03*)temp_r4)->unk1E0);
    ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)temp_r4)->unk30 + ((Boss03*)temp_r4)->unk1E4);
    temp_r1_2                 = ((Boss03*)temp_r4)->unk1C8;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    func_ov003_020c4628(temp_r4);
    return ((Boss03*)temp_r4)->unk1CC;
}

s32 BtlBoss03_03_RG_Render(void* arg1) {
    func_ov003_020c48b0(((Boss03*)arg1)->unk18);
    return 1;
}

s32 BtlBoss03_03_RG_Destroy(void* arg1) {
    func_ov003_020c48fc(((Boss03*)arg1)->unk18);
    return 1;
}

s32 BtlBoss03_03_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_03_RG = {"Tsk_BtlBoss03_03_RG", BtlBoss03_03_RG_RunTask, 0x1FC};

s32 BtlBoss03_03_RG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_02142780 = {
        .initialize = BtlBoss03_03_RG_Init,
        .update     = BtlBoss03_03_RG_Update,
        .render     = BtlBoss03_03_RG_Render,
        .cleanup    = BtlBoss03_03_RG_Destroy,
    };
    return data_ov017_02142780.iter[stage](pool, task, args);
}

s32 BtlBoss03_03_UG_Init(void* arg1, s32 arg2) {
    void* temp_r4;
    void* temp_r5;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r5 = func_ov003_020cb42c(0, 0x2C, 0);
    MI_CpuSet(temp_r4, 0, 0x1DC);
    ((Boss03*)temp_r4)->unk188 = (void*)(temp_r4 + 0x1D4);
    if (data_ov003_020e71b8->unk3D874 == 0) {
        func_ov017_0213df24(0);
        BtlBoss03_KindanMgr_CreateTask();
    }
    func_ov017_0213e088(8, 0);
    func_ov017_0213e17c(0xF, 0);
    func_ov017_0213e17c(0x10, 0);
    func_ov017_0213f20c(1);
    func_ov003_020c3efc(temp_r4, arg2);
    func_ov003_020c4520(temp_r4);
    ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x20);
    if (data_ov003_020e71b8->unk3D874 != 0) {
        ((Boss03*)temp_r4)->unk6C = (s16)((Boss03*)temp_r5)->unk6C;
    }
    ((Boss03*)temp_r4)->unk18C = (u16)(((Boss03*)temp_r4)->unk18C | 1);
    ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 0x10);
    func_ov003_02084694(temp_r4 + 0x144, 1);
    func_ov003_02082cc4(temp_r4 + 0x84);
    ((Boss03*)temp_r4)->unk1CC = 1;
}

s32 BtlBoss03_03_UG_Update(void* arg1) {
    void* temp_r4;
    void* temp_r5;

    temp_r4                                       = ((Boss03*)arg1)->unk18;
    temp_r5                                       = temp_r4 + 0x1D4;
    ((Boss03*)((Boss03*)temp_r4)->unk184)->unk188 = temp_r5;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        if (((Boss03*)temp_r4)->unk20 == 6) {
            func_ov003_02082f2c(temp_r4);
        }
        return 1;
    }
    if (((Boss03*)temp_r5)->unk4 != 0) {
        ((Boss03*)temp_r5)->unk4   = 0;
        ((Boss03*)temp_r4)->unk1CC = 0;
        ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 4);
    }
    if ((s32)((Boss03*)temp_r4)->unk1D0 > func_ov017_02130630(0)) {
        ((Boss03*)temp_r4)->unk1D4 = 1;
    }
    ((Boss03*)temp_r4)->unk1D0 = (s16)func_ov017_02130630(0);
    return ((Boss03*)temp_r4)->unk1CC;
}

s32 BtlBoss03_03_UG_Render(void) {
    return 1;
}

s32 BtlBoss03_03_UG_Destroy(void* arg1) {
    func_ov003_020c492c(((Boss03*)arg1)->unk18);
    func_ov017_0213e074();
    return 1;
}

/// MARK: BtlBoss03_03_UG
s32 BtlBoss03_03_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_03_UG = {"Tsk_BtlBoss03_03_UG", BtlBoss03_03_UG_RunTask, 0x1DC};

s32 BtlBoss03_03_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_0214279c = {
        .initialize = BtlBoss03_03_UG_Init,
        .update     = BtlBoss03_03_UG_Update,
        .render     = BtlBoss03_03_UG_Render,
        .cleanup    = BtlBoss03_03_UG_Destroy,
    };
    return data_ov017_0214279c.iter[stage](pool, task, args);
}

void func_ov017_0213b454(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1C8 = arg1;
    ((Boss03*)arg0)->unk1C4 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_0213b47c(void* arg0) {
    func_ov017_0213b454(arg0, func_ov017_0213b6d0);
}

void func_ov017_0213b490(void* arg0) {
    s32 temp_r4;
    u32 temp_r1;
    u8  temp_r2;
    void (*var_r1)(void*);

    temp_r2 = ((Boss03*)arg0)->unk200;
    temp_r1 = (u32)(temp_r2 << 0x1C) >> 0x1F;
    if (temp_r1 != 0) {
        ((Boss03*)arg0)->unk200 = (u8)(temp_r2 & ~8);
        func_ov017_0213b454(arg0, func_ov017_0213c60c);
        return;
    }
    temp_r4 = RNG_Next(0x64);
    if (func_ov017_0213d2f4(arg0) != 0) {
        if (temp_r4 < 0x1E) {
            var_r1 = func_ov017_0213b7c4;
        } else if (temp_r4 < 0x37) {
            var_r1 = func_ov017_0213b83c;
        } else if (temp_r4 < 0x55) {
            var_r1 = func_ov017_0213c60c;
        } else {
            var_r1 = func_ov017_0213c1d4;
        }
    } else if (temp_r4 < 0x28) {
        var_r1 = func_ov017_0213b7c4;
    } else if (temp_r4 < 0x46) {
        var_r1 = func_ov017_0213b83c;
    } else {
        var_r1 = func_ov017_0213c60c;
    }
    func_ov017_0213b454(arg0, var_r1);
}

void func_ov017_0213b534(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        data_ov003_020e71b8->unk3D875 = 2;
        ((Boss03*)arg0)->unk1C0       = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    }
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_0213b454(arg0, func_ov017_0213b594);
}

void func_ov017_0213b594(void* arg0) {
    s16 temp_r2;
    u32 temp_r3;

    func_ov003_020c4b5c(arg0);
    if (func_ov017_0213d27c(arg0, 0) != 0) {
        func_ov017_0213d1e0(arg0, 0);
        func_ov017_0213b454(arg0, func_ov017_0213b8cc);
        return;
    }
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk1C2 = func_ov003_020c42ec(arg0);
    }
    func_ov017_0213cd00(arg0, ((Boss03*)arg0)->unk1C0);
    if (((func_ov003_02085dc8(1) == arg0) || (func_ov003_02085e78(1) == arg0) || (func_ov003_02085ec0(1) == arg0) ||
         (func_ov003_02086014(1) == arg0)) &&
        (func_ov003_020cb648(arg0, 0x28) != 0) && ((u32)RNG_Next(0x64) < 0x21U))
    {
        ((Boss03*)arg0)->unk200 = (u8)(((Boss03*)arg0)->unk200 | 8);
    }
    temp_r3 = (u32)(((Boss03*)arg0)->unk200 << 0x1C) >> 0x1F;
    if ((temp_r3 != 0) || (temp_r2 = ((Boss03*)arg0)->unk1C0, ((s32)temp_r2 >= (s32)((Boss03*)arg0)->unk1C2))) {
        if ((temp_r3 != 0) || (func_ov003_020c4348(arg0) != 0)) {
            func_ov017_0213b490(arg0);
            return;
        }
        func_ov017_0213b47c(arg0);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
}

void func_ov017_0213b6d0(void* arg0) {
    s32 var_r1;
    s16 temp_r2;
    s32 temp_r4;
    s32 temp_r6;

    temp_r6 = func_ov003_020cb744(0);
    temp_r2 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = func_ov003_020cb7a4(0);
    switch (temp_r2) { /* irregular */
        case 0:
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            ((Boss03*)arg0)->unk1D0 = (s32)(RNG_Next((temp_r6 >> 0xC) + 1) << 0xC);
            ((Boss03*)arg0)->unk1D4 = (s32)(RNG_Next((temp_r4 >> 0xC) + 1) << 0xC);
            var_r1                  = 0;
            ((Boss03*)arg0)->unk1D8 = 0;
            if ((s32)((Boss03*)arg0)->unk1D0 > (s32)((Boss03*)arg0)->unk28) {
                var_r1 = 1;
            }
            func_ov003_020c4ab4(arg0, var_r1);
            return;
        case 1:
            if (func_ov017_0213cbcc(arg0, 1, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 2;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 2:
            func_ov017_0213b454(arg0, func_ov017_0213b594);
            return;
    }
}

void func_ov017_0213b7c4(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_020c4b5c(arg0);
        func_ov003_02087f00(SEIDX_HANEKOMA_N_BTL_ATCS01, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (func_ov017_0213ced0(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
        func_ov017_0213b454(arg0, func_ov017_0213b594);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_0213b83c(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_020c4b5c(arg0);
        func_ov003_02087f00(SEIDX_HANEKOMA_N_BTL_ATCM01, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
        func_ov003_02087f00(SEIDX_SE_B006_KINNKYORI_KOUGEKI_L, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (func_ov017_0213cfec(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
        func_ov017_0213b454(arg0, func_ov017_0213b594);
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_0213b8cc(void* arg0) {
    s16   spA;
    s16   sp8;
    s32   var_r1;
    s16   temp_r0;
    s16   temp_r0_10;
    s16   temp_r0_13;
    s16   temp_r0_2;
    s16   temp_r0_3;
    s16   temp_r0_4;
    s16   temp_r0_6;
    s16   temp_r1;
    s32   temp_r0_11;
    s32   temp_r0_12;
    s32   temp_r0_5;
    s32   temp_r0_7;
    s32   temp_r0_8;
    s32   temp_r0_9;
    s32   temp_r2;
    s32   var_r0;
    s32   var_r0_2;
    s32   var_r0_3;
    s32   var_r0_4;
    s32   var_r0_5;
    s32   var_r0_6;
    s32   var_r0_7;
    s32   var_r0_8;
    s32   var_r0_9;
    void* temp_r4;

    temp_r1 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = data_ov003_020e71b8->unk3D898;
    switch (temp_r1) {
        default:
            return;
        case 0:
            temp_r0 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0 + 1);
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(0, arg0 + 0x84, 8, 1);
                func_ov003_020824a0(arg0 + 0x84, 3, 1);
                ((Boss03*)arg0)->unk1FC = BtlBoss03_EffSign_CreateTask(arg0);
                func_ov003_02087f00(SEIDX_HANEKOMA_N_BTL_FB07, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_00000, arg0 + 0x1FC);
            if (((Boss03*)arg0)->unk1FC != -1) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            temp_r0_2 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_2 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_2 + 1);
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(0, arg0 + 0x84, 9, 1);
                func_ov003_020824a0(arg0 + 0x84, 2, 1);
                ((Boss03*)arg0)->unk1E8 = (s32)((Boss03*)arg0)->unk28;
                ((Boss03*)arg0)->unk1EC = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1F0 = (s32)((Boss03*)arg0)->unk30;
                if (((Boss03*)arg0)->unk24 == 0) {
                    ((Boss03*)arg0)->unk1DC = -0x8000;
                } else {
                    ((Boss03*)arg0)->unk1DC = 0x8000;
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 2:
            temp_r0_3 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_3 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_3 + 1);
                func_ov003_020824a0(arg0 + 0x84, 3, 0);
                BtlBoss03_EffPosture_CreateTask(arg0);
                func_ov003_02084694(arg0 + 0x144, 1);
                ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10000000);
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            if (func_ov017_0213d298(arg0) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 3;
                return;
            }
            func_ov003_02084348(0, &spA, &sp8, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
            var_r0 = func_ov003_020cc7c0(spA, sp8, 0x40);
            if ((var_r0 == 0) && (((temp_r2 = ((Boss03*)arg0)->unk24, (temp_r2 == 0)) && ((s32)spA > 0x80)) ||
                                  ((temp_r2 == 1) && ((s32)spA < 0x80))))
            {
                var_r0 = 1;
            }
            if (var_r0 != 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 0xB;
            return;
        case 3:
            temp_r0_4 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_4 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_4 + 1);
                func_ov003_020824a0(arg0 + 0x84, 4, 1);
                func_ov003_02087f00(SEIDX_SE_B006_TUKAMIKOUGEKI_01, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 4;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 4:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 5, 1);
                BtlBoss03_EffCatch_CreateTask(arg0);
            }
            if ((((u32)(((Boss03*)arg0)->unk200 << 0x1F) >> 0x1F) == 0) && (func_ov003_020cc354(temp_r4) == 0) &&
                (func_ov017_0213d298(arg0) != 0))
            {
                ((Boss03*)arg0)->unk200 = (u8)((((Boss03*)arg0)->unk200 & ~1) | 1);
                func_ov003_020cc784(temp_r4, 0);
                func_ov003_02084694(arg0 + 0x144, 1);
                func_ov003_02082f1c(temp_r4, 2);
                ((Boss03*)temp_r4)->unk54     = (s32)(((Boss03*)temp_r4)->unk54 | 0x20000);
                data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x80000);
                ((Boss03*)temp_r4)->unk10     = 0x12C;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_2 = 1;
                } else {
                    var_r0_2 = 0;
                }
                ((Boss03*)temp_r4)->unk24 = var_r0_2;
                func_ov003_020cc784(temp_r4, 0);
                ((Boss03*)temp_r4)->unk38 = 0;
            }
            if (((u32)(((Boss03*)arg0)->unk200 << 0x1F) >> 0x1F) != 0) {
                temp_r0_5 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_3 = temp_r0_5 - 0x48000;
                } else {
                    var_r0_3 = temp_r0_5 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0_3;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x20000);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 5;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 5:
            temp_r0_6 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_6 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_6 + 1);
                func_ov003_020824a0(arg0 + 0x84, 6, 1);
                if (((u32)(((Boss03*)arg0)->unk200 << 0x1F) >> 0x1F) != 0) {
                    temp_r0_7 = ((Boss03*)arg0)->unk28;
                    if (((Boss03*)arg0)->unk24 == 0) {
                        var_r0_4 = temp_r0_7 - 0x18000;
                    } else {
                        var_r0_4 = temp_r0_7 + 0x18000;
                    }
                    ((Boss03*)temp_r4)->unk28 = var_r0_4;
                    ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                    ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x70000);
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 6;
            return;
        case 6:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 7, 1);
                if (((u32)(((Boss03*)arg0)->unk200 << 0x1F) >> 0x1F) != 0) {
                    temp_r0_8 = ((Boss03*)arg0)->unk28;
                    if (((Boss03*)arg0)->unk24 == 0) {
                        var_r0_5 = temp_r0_8 - 0x18000;
                    } else {
                        var_r0_5 = temp_r0_8 + 0x18000;
                    }
                    ((Boss03*)temp_r4)->unk28 = var_r0_5;
                    ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                    ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x70000);
                }
            }
            if ((((u32)(((Boss03*)arg0)->unk200 << 0x1F) >> 0x1F) != 0) && (((Boss03*)arg0)->unk1C0 == 0xC)) {
                temp_r0_9 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_6 = temp_r0_9 - 0x48000;
                } else {
                    var_r0_6 = temp_r0_9 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0_6;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x50000);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 7;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 7:
            temp_r0_10 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_10 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_10 + 1);
                func_ov003_020824a0(arg0 + 0x84, 8, 1);
            }
            if (((u32)(((Boss03*)arg0)->unk200 << 0x1F) >> 0x1F) != 0) {
                temp_r0_11 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_7 = temp_r0_11 - 0x48000;
                } else {
                    var_r0_7 = temp_r0_11 + 0x48000;
                }
                ((Boss03*)temp_r4)->unk28 = var_r0_7;
                ((Boss03*)temp_r4)->unk2C = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)temp_r4)->unk30 = (s32)(((Boss03*)arg0)->unk30 - 0x20000);
                ((Boss03*)temp_r4)->unk38 = 0x20000;
                ((Boss03*)arg0)->unk200   = (u8)(((Boss03*)arg0)->unk200 & ~1);
                func_ov003_020cc784(temp_r4, 1);
                func_ov003_02084694(arg0 + 0x144, 0);
                ((Boss03*)temp_r4)->unk10 = 0;
                func_ov003_020cc784(temp_r4, 1);
                ((Boss03*)arg0)->unk204 = 0;
                func_ov017_0213e3a4();
                BtlBoss03_FailPly_CreateTask(arg0, temp_r4);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 8;
            return;
        case 8:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 9, 1);
                func_ov017_0213d198(arg0, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
                temp_r0_12 = ((Boss03*)arg0)->unk1D0;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_8 = temp_r0_12 + 0x20000;
                } else {
                    var_r0_8 = temp_r0_12 - 0x20000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_8;
                ((Boss03*)arg0)->unk1C2 = func_ov017_0213d13c(arg0, 0x4000);
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
            ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
            if (((s32)((Boss03*)arg0)->unk1C0 >= (s32)((Boss03*)arg0)->unk1C2) || (func_ov003_020cc300(arg0, 0) == 0)) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 9;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 9:
            temp_r0_13 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_13 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_13 + 1);
                func_ov003_020824a0(arg0 + 0x84, 0xA, 1);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_0213b454(arg0, func_ov017_0213b594);
            func_ov003_02084694(arg0 + 0x144, 0);
            ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10000000);
            return;
        case 11:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                if ((s32)((Boss03*)arg0)->unk28 < (s32)((Boss03*)temp_r4)->unk28) {
                    var_r0_9 = func_ov003_0208442c(0, 0x40);
                } else {
                    var_r0_9 = func_ov003_0208442c(0, 0xC0);
                }
                func_ov017_0213d198(arg0, var_r0_9, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
                if ((s32)((Boss03*)arg0)->unk1D0 > (s32)((Boss03*)arg0)->unk28) {
                    var_r1 = 1;
                } else {
                    var_r1 = 0;
                }
                func_ov003_020c4ab4(arg0, var_r1);
            }
            if (func_ov017_0213cbcc(arg0, 0, ((Boss03*)arg0)->unk1C0) == 0) {
                func_ov017_0213b454(arg0, func_ov017_0213b594);
                ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10000000);
                func_ov003_02084694(arg0 + 0x144, 0);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
    }
}

void func_ov017_0213c1d4(void* arg0) {
    f32   var_r0;
    f32   var_r0_3;
    s16   temp_r0;
    s16   temp_r0_10;
    s16   temp_r0_4;
    s16   temp_r1;
    s16   temp_r2;
    s16   temp_r2_2;
    s16   temp_r2_3;
    s32   temp_r0_2;
    s32   temp_r0_3;
    s32   temp_r0_6;
    s32   temp_r0_7;
    s32   temp_r0_9;
    s32   var_r0_2;
    s32   var_r0_4;
    s32   var_r0_5;
    u8    temp_r0_5;
    void* temp_r0_8;

    temp_r0   = ((Boss03*)arg0)->unk70;
    temp_r0_2 = temp_r0 << 0xC;
    if ((s32)temp_r0 > 0) {
        var_r0 = 0.5f + (f32)temp_r0_2;
    } else {
        var_r0 = (f32)temp_r0_2 - 0.5f;
    }
    temp_r0_3 = (s32)var_r0;
    temp_r1   = ((Boss03*)arg0)->unk1C4;
    switch (temp_r1) {
        default:
            return;
        case 0:
            temp_r0_4 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_4 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_4 + 1);
                func_ov003_02082cc4(arg0 + 0x84);
                func_ov017_02140a8c(0, arg0 + 0x84, 9, 1);
                func_ov003_020824a0(arg0 + 0x84, 0xC, 1);
                func_ov003_02087f00(SEIDX_HANEKOMA_N_BTL_ATCM04, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                func_ov003_02087f00(SEIDX_SE_B007_BLION_TOSSHIN_KOUGEKI, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                func_ov003_020c4c5c(arg0);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 0xD, 0);
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_2 = temp_r0_3 + 0x30000;
                } else {
                    var_r0_2 = (func_ov003_020cb744(0) - temp_r0_3) - 0x30000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_2;
                ((Boss03*)arg0)->unk1D4 = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1D8 = (s32)((Boss03*)arg0)->unk30;
                ((Boss03*)arg0)->unk1C2 = func_ov017_0213d13c(arg0, 0x8000);
                ((Boss03*)arg0)->unk1FC = BtlBoss03_EffRush_CreateTask(arg0);
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
            ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
            temp_r0_5              = ((Boss03*)func_ov003_0208a114(0xF7))->unk7;
            temp_r0_6              = temp_r0_5 << 0xC;
            if (temp_r0_5 != 0) {
                var_r0_3 = 0.5f + (f32)temp_r0_6;
            } else {
                var_r0_3 = (f32)temp_r0_6 - 0.5f;
            }
            func_ov003_020c5b2c(0xF7U, arg0, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C,
                                ((Boss03*)arg0)->unk30 - (s32)var_r0_3);
            temp_r2 = ((Boss03*)arg0)->unk1C0;
            if ((s32)temp_r2 < (s32)((Boss03*)arg0)->unk1C2) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 2;
            return;
        case 2:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 0xE, 1);
                temp_r0_7 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_4 = temp_r0_7 - 0x20000;
                } else {
                    var_r0_4 = temp_r0_7 + 0x20000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_4;
                ((Boss03*)arg0)->unk1D4 = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1D8 = (s32)((Boss03*)arg0)->unk30;
                ((Boss03*)arg0)->unk1C2 = func_ov017_0213d13c(arg0, 0x4000);
                EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_00000, arg0 + 0x1FC);
                temp_r0_8 = EasyTask_GetTaskById(&data_ov003_020e71b8->unk_00000, ((Boss03*)arg0)->unk1FC);
                if (temp_r0_8 != NULL) {
                    ((Boss03*)temp_r0_8)->unk4 = (u16)(((Boss03*)temp_r0_8)->unk4 | 0x10);
                }
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
            ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
            temp_r2_2              = ((Boss03*)arg0)->unk1C0;
            if ((s32)temp_r2_2 < (s32)((Boss03*)arg0)->unk1C2) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2_2 + 1);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 3;
            return;
        case 3:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                func_ov003_020824a0(arg0 + 0x84, 0xF, 0);
                temp_r0_9 = ((Boss03*)arg0)->unk28;
                if (((Boss03*)arg0)->unk24 == 0) {
                    var_r0_5 = temp_r0_9 - 0x10000;
                } else {
                    var_r0_5 = temp_r0_9 + 0x10000;
                }
                ((Boss03*)arg0)->unk1D0 = var_r0_5;
                ((Boss03*)arg0)->unk1D4 = (s32)((Boss03*)arg0)->unk2C;
                ((Boss03*)arg0)->unk1D8 = (s32)((Boss03*)arg0)->unk30;
                ((Boss03*)arg0)->unk1C2 = func_ov017_0213d13c(arg0, 0x2000);
            }
            ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
            ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
            ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
            temp_r2_3              = ((Boss03*)arg0)->unk1C0;
            if ((s32)temp_r2_3 < (s32)((Boss03*)arg0)->unk1C2) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2_3 + 1);
                return;
            }
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 4;
            ((Boss03*)arg0)->unk28  = (s32)((Boss03*)arg0)->unk1D0;
            ((Boss03*)arg0)->unk1E4 = 0;
            ((Boss03*)arg0)->unk1E0 = 0;
            ((Boss03*)arg0)->unk1DC = 0;
            return;
        case 4:
            temp_r0_10 = ((Boss03*)arg0)->unk1C0;
            if (temp_r0_10 == 0) {
                ((Boss03*)arg0)->unk1C0 = (s16)(temp_r0_10 + 1);
                func_ov003_020824a0(arg0 + 0x84, 0x10, 1);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_0213b454(arg0, func_ov017_0213b594);
            return;
    }
}

void func_ov017_0213c60c(void* arg0) {
    s16   temp_r2;
    s32   temp_r6;
    s32   var_r7;
    void* temp_r4;

    temp_r2 = ((Boss03*)arg0)->unk1C4;
    temp_r4 = func_ov003_020cc750(0);
    switch (temp_r2) {
        default:
            return;
        case 0:
            ((Boss03*)arg0)->unk1C0 = 0;
            ((Boss03*)arg0)->unk1C4 = 1;
            return;
        case 1:
            if (func_ov017_0213cd60(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 2;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 2:
            if (((Boss03*)arg0)->unk1C0 == 0) {
                temp_r6 = ((Boss03*)temp_r4)->unk28;
                var_r7  = 0x50000;
                if ((u32)RNG_Next(0x64) < 0x32U) {
                    var_r7 = -0x50000;
                }
                ((Boss03*)arg0)->unk28 = (s32)(temp_r6 + var_r7);
                ((Boss03*)arg0)->unk2C = (s32)((Boss03*)temp_r4)->unk2C;
                ((Boss03*)arg0)->unk30 = (s32)((Boss03*)temp_r4)->unk30;
                if (func_ov003_020cc300(arg0, 0) == 0) {
                    ((Boss03*)arg0)->unk28 = (s32)(temp_r6 - var_r7);
                }
                if (func_ov017_0213d27c(arg0, 1) != 0) {
                    func_ov017_0213d1e0(arg0, 1);
                    func_ov003_02087f00(SEIDX_HANEKOMA_N_BTL_FB05, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
                }
                func_ov003_020c4b5c(arg0);
            }
            if (func_ov017_0213ce14(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 3;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 3:
            if (func_ov017_0213ced0(arg0, ((Boss03*)arg0)->unk1C0) == 0) {
                ((Boss03*)arg0)->unk1C0 = 0;
                ((Boss03*)arg0)->unk1C4 = 4;
                return;
            }
            ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
            return;
        case 4:
            func_ov017_0213b454(arg0, func_ov017_0213b594);
            return;
    }
}

void func_ov017_0213c7c0(void* arg0) {
    s16   temp_r1;
    u8    temp_r0_2;
    void* temp_r0;
    void* temp_r3;
    void* temp_r3_2;

    temp_r1 = ((Boss03*)arg0)->unk1C0;
    if (temp_r1 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r1 + 1);
        func_ov003_020cb520(arg0, 1);
        EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_00000, arg0 + 0x1FC);
        temp_r0 = EasyTask_GetTaskById(&data_ov003_020e71b8->unk_00000, ((Boss03*)arg0)->unk1FC);
        if (temp_r0 != NULL) {
            ((Boss03*)temp_r0)->unk4 = (u16)(((Boss03*)temp_r0)->unk4 | 0x10);
        }
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 9, 1);
        if (SndMgr_IsSEPlaying(0x553) == 0) {
            func_ov003_02087f00(SEIDX_HANEKOMA_N_BTL_DMG03, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
        }
        temp_r0_2 = ((Boss03*)arg0)->unk200;
        if (((u32)(temp_r0_2 << 0x1F) >> 0x1F) != 0) {
            ((Boss03*)arg0)->unk200 = (u8)(temp_r0_2 & ~1);
            func_ov003_020cc784(data_ov003_020e71b8->unk3D898, 1);
            func_ov003_02084694(arg0 + 0x144, 0);
            temp_r3                       = data_ov003_020e71b8->unk3D898;
            ((Boss03*)temp_r3)->unk54     = (s32)(((Boss03*)temp_r3)->unk54 & ~0x20000);
            temp_r3_2                     = data_ov003_020e71b8->unk3D898;
            ((Boss03*)temp_r3_2)->unk54   = (s32)(((Boss03*)temp_r3_2)->unk54 & ~0x4000);
            data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 & ~0x80000);
            func_ov003_0208afcc(0);
            ((Boss03*)data_ov003_020e71b8->unk3D898)->unk10 = 0;
        }
    }
    if (func_ov003_020c6b8c(arg0, 0) != 0) {
        return;
    }
    func_ov017_0213b454(arg0, func_ov017_0213b594);
}

void func_ov017_0213c930(void* arg0) {
    u8    temp_r0;
    void* temp_r0_2;
    void* temp_r3;
    void* temp_r3_2;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        temp_r0 = ((Boss03*)arg0)->unk200;
        if (((u32)(temp_r0 << 0x1F) >> 0x1F) != 0) {
            ((Boss03*)arg0)->unk200 = (u8)(temp_r0 & ~1);
            func_ov003_020cc784(data_ov003_020e71b8->unk3D898, 1);
            func_ov003_02084694(arg0 + 0x144, 0);
            temp_r3                       = data_ov003_020e71b8->unk3D898;
            ((Boss03*)temp_r3)->unk54     = (s32)(((Boss03*)temp_r3)->unk54 & ~0x20000);
            temp_r3_2                     = data_ov003_020e71b8->unk3D898;
            ((Boss03*)temp_r3_2)->unk54   = (s32)(((Boss03*)temp_r3_2)->unk54 & ~0x4000);
            data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 & ~0x80000);
            func_ov003_0208afcc(0);
            ((Boss03*)data_ov003_020e71b8->unk3D898)->unk10 = 0;
        }
        EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_00000, arg0 + 0x1FC);
        temp_r0_2 = EasyTask_GetTaskById(&data_ov003_020e71b8->unk_00000, ((Boss03*)arg0)->unk1FC);
        if (temp_r0_2 != NULL) {
            ((Boss03*)temp_r0_2)->unk4 = (u16)(((Boss03*)temp_r0_2)->unk4 | 0x10);
        }
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 8, 1);
    }
    if (func_ov003_020c7070(arg0) == 0) {
        data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x20000000);
        ((Boss03*)arg0)->unk1CC       = 0;
        return;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_0213ca84(void* arg0) {
    s16   temp_r2;
    u8    temp_r0;
    void* temp_r0_2;
    void* temp_r3;
    void* temp_r3_2;

    temp_r2 = ((Boss03*)arg0)->unk1C0;
    if (temp_r2 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s16)(temp_r2 + 1);
        func_ov003_020cb520(arg0, 1);
        temp_r0 = ((Boss03*)arg0)->unk200;
        if (((u32)(temp_r0 << 0x1F) >> 0x1F) != 0) {
            ((Boss03*)arg0)->unk200 = (u8)(temp_r0 & ~1);
            func_ov003_020cc784(data_ov003_020e71b8->unk3D898, 1);
            func_ov003_02084694(arg0 + 0x144, 0);
            temp_r3                       = data_ov003_020e71b8->unk3D898;
            ((Boss03*)temp_r3)->unk54     = (s32)(((Boss03*)temp_r3)->unk54 & ~0x20000);
            temp_r3_2                     = data_ov003_020e71b8->unk3D898;
            ((Boss03*)temp_r3_2)->unk54   = (s32)(((Boss03*)temp_r3_2)->unk54 & ~0x4000);
            data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 & ~0x80000);
            func_ov003_0208afcc(0);
            ((Boss03*)data_ov003_020e71b8->unk3D898)->unk10 = 0;
        }
        EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_00000, arg0 + 0x1FC);
        temp_r0_2 = EasyTask_GetTaskById(&data_ov003_020e71b8->unk_00000, ((Boss03*)arg0)->unk1FC);
        if (temp_r0_2 != NULL) {
            ((Boss03*)temp_r0_2)->unk4 = (u16)(((Boss03*)temp_r0_2)->unk4 | 0x10);
        }
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 8, 1);
    }
    if (func_ov003_020c72b4(arg0, 0, 0) != 0) {
        return;
    }
    func_ov017_0213b454(arg0, func_ov017_0213b594);
}

s32 func_ov017_0213cbcc(void* arg0, s32 arg1, s16 arg2) {
    s16 temp_r0;

    if (arg2 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xA, 1);
        func_ov003_020824a0(arg0 + 0x84, 0, 0);
        ((Boss03*)arg0)->unk208 = 0;
        ((Boss03*)arg0)->unk20A = func_ov017_0213d13c(arg0, 0x4000);
    }
    func_ov017_0213d13c(arg0, 0x4000);
    ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk1DC);
    ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + ((Boss03*)arg0)->unk1E0);
    ((Boss03*)arg0)->unk30 = (s32)(((Boss03*)arg0)->unk30 + ((Boss03*)arg0)->unk1E4);
    temp_r0                = ((Boss03*)arg0)->unk9A;
    if ((temp_r0 != 3) && (temp_r0 != 6)) {

    } else {
        func_ov003_02087f00(SEIDX_SE_B006_FOOTSTEP, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (((s32)((Boss03*)arg0)->unk208 >= (s32)((Boss03*)arg0)->unk20A) || (func_ov017_0213d1a8(arg0) < 0x4000) ||
        ((arg1 != 0) && (func_ov003_020cc300(arg0, 0) == 0)))
    {
        func_ov017_0213cd00(arg0, 0);
        return 0;
    }
    ((Boss03*)arg0)->unk208 = (s16)(((Boss03*)arg0)->unk208 + 1);
    return 1;
}

s32 func_ov017_0213cd00(void* arg0, s16 arg1) {
    if (arg1 == -1) {
        return 0;
    }
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 8, 1);
    }
    if (((Boss03*)arg0)->unkC8 != 0) {
        func_ov003_020824a0(arg0 + 0x84, 0, 0);
    }
    return 1;
}

s32 func_ov017_0213cd60(void* arg0, s16 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x10);
        func_ov003_02084694(arg0 + 0x144, 1);
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x40000000);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xB, 1);
        func_ov003_020824a0(arg0 + 0x84, 0, 1);
        func_ov003_02087f00(SEIDX_SE_B006_KUUKANN_TENNI_DISAPPEAR, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
        return 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    return 1;
}

s32 func_ov017_0213ce14(void* arg0, s16 arg1) {
    if (arg1 == 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C & ~1);
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xB, 1);
        func_ov003_020824a0(arg0 + 0x84, 1, 1);
        func_ov003_02087f00(SEIDX_SE_B006_KUUKANN_TENNI_APPEAR, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x10);
        func_ov003_02084694(arg0 + 0x144, 0);
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x40000000);
        return 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s16)(((Boss03*)arg0)->unk1C0 + 1);
    return 1;
}

s32 func_ov017_0213ced0(void* arg0, s16 arg1) {
    s16 temp_r0;
    s32 var_r4;

    var_r4 = -0x40000;
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xC, 1);
        func_ov003_020824a0(arg0 + 0x84, 3, 1);
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    if (((Boss03*)arg0)->unk24 == 1) {
        var_r4 = 0x40000;
    }
    switch (temp_r0) { /* irregular */
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 4, 1);
                func_ov003_02087f00(SEIDX_SE_B006_KINNKYORI_KOUGEKI_S, func_ov003_020843b0(0, ((Boss03*)arg0)->unk28));
            }
        default:
            return 1;
        case 4:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 5, 1);
            }
            func_ov003_020c5b2c(0xF3U, arg0, ((Boss03*)arg0)->unk28 + var_r4, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30);
            /* Duplicate return node #15. Try simplifying control flow for better match */
            return 1;
        case 5:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov017_0213cd00(arg0, 0);
                return 0;
            }
            /* Duplicate return node #15. Try simplifying control flow for better match */
            return 1;
    }
}

s32 func_ov017_0213cfec(void* arg0, s32 arg1) {
    s16   temp_r0;
    s32   var_r4;
    s32   var_r5;
    void* temp_r6;

    var_r4  = -0x40000;
    var_r5  = -0x4000;
    temp_r6 = func_ov003_020cc750(0);
    if (((Boss03*)arg0)->unk24 == 1) {
        var_r4 = 0x40000;
        var_r5 = 0x4000;
    }
    if (arg1 == 0) {
        func_ov003_02082cc4(arg0 + 0x84);
        func_ov017_02140a8c(0, arg0 + 0x84, 0xD, 1);
        func_ov003_020824a0(arg0 + 0x84, 3, 1);
    }
    temp_r0 = ((Boss03*)arg0)->unkC8;
    switch (temp_r0) { /* irregular */
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 4, 1);
            }
        default:
            return 1;
        case 4:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov003_020824a0(arg0 + 0x84, 5, 1);
            }
            if ((arg1 >= 8) && (arg1 <= 0x17) &&
                (func_ov003_020c5b2c(0xF4, arg0, ((Boss03*)arg0)->unk28 + var_r4, ((Boss03*)arg0)->unk2C,
                                     ((Boss03*)arg0)->unk30) == 1))
            {
                ((Boss03*)temp_r6)->unk40 = var_r5;
                ((Boss03*)temp_r6)->unk48 = 0x19A;
            }
            /* Duplicate return node #18. Try simplifying control flow for better match */
            return 1;
        case 5:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) != 0) {
                func_ov017_0213cd00(arg0, 0);
                return 0;
            }
            /* Duplicate return node #18. Try simplifying control flow for better match */
            return 1;
    }
}

s16 func_ov017_0213d13c(void* arg0, s32 arg1) {
    return func_ov003_020cb910(arg0 + 0x1DC, arg0 + 0x1E0, arg0 + 0x1E4, ((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C,
                               ((Boss03*)arg0)->unk30, ((Boss03*)arg0)->unk1D0, ((Boss03*)arg0)->unk1D4,
                               ((Boss03*)arg0)->unk1D8, arg1);
}

void func_ov017_0213d198(void* arg0, s32 arg1, s32 arg2, s32 arg3) {
    ((Boss03*)arg0)->unk1D0 = arg1;
    ((Boss03*)arg0)->unk1D4 = arg2;
    ((Boss03*)arg0)->unk1D8 = arg3;
}

s32 func_ov017_0213d1a8(void* arg0) {
    func_ov003_020cba54(((Boss03*)arg0)->unk28, ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30, ((Boss03*)arg0)->unk1D0,
                        ((Boss03*)arg0)->unk1D4, ((Boss03*)arg0)->unk1D8);
}

void func_ov017_0213d1e0(void* arg0, s32 arg1) {
    s16 temp_r5;

    temp_r5 = *(s32*)((char*)&data_ov017_021435be + (arg1 * 4));
    ((Boss03*)(arg0 + (arg1 * 2)))->unk1F6 =
        (s16)(*(s32*)((char*)&data_ov017_021435bc + (arg1 * 4)) + ((0 - temp_r5) + RNG_Next((temp_r5 * 2) + 1)));
}

void func_ov017_0213d234(void* arg0, s32 arg1) {
    ((Boss03*)(arg0 + (arg1 * 2)))->unk1F6 = 0;
}

void func_ov017_0213d248(void* arg0, s32 arg1) {
    s32   temp_ip;
    s32   temp_r3;
    void* temp_r0;

    temp_ip = arg0 + 0x1F6;
    temp_r3 = arg1 * 2;
    temp_r0 = arg0 + (arg1 * 2);
    ((s32*)temp_ip)[temp_r3]--;
    if ((s32)((Boss03*)temp_r0)->unk1F6 < 0) {
        ((Boss03*)temp_r0)->unk1F6 = 0;
    }
}

s32 func_ov017_0213d27c(s32 arg0, s32 arg1) {
    if (((Boss03*)(arg0 + (arg1 * 2)))->unk1F6 == 0) {
        return 1;
    }
    return 0;
}

s32 func_ov017_0213d298(void* arg0) {
    s32 var_ip = 0x48000;
    if (((Boss03*)arg0)->unk24 == 0) {
        var_ip = -0x48000;
    }
    func_ov003_020cc38c(data_ov003_020e71b8->unk3D898, ((Boss03*)arg0)->unk28 + var_ip, ((Boss03*)arg0)->unk2C,
                        ((Boss03*)arg0)->unk30);
}

s32 func_ov017_0213d2f4(void* arg0) {
    s32   temp_r1;
    s32   temp_r1_2;
    void* temp_r4;

    temp_r4 = data_ov003_020e71b8->unk3D898;
    temp_r1 = ((Boss03*)arg0)->unk28;
    if (((temp_r1 < func_ov003_020cb764(0)) && ((s32)((Boss03*)temp_r4)->unk28 > temp_r1)) ||
        ((temp_r1_2 = ((Boss03*)arg0)->unk28, (temp_r1_2 > func_ov003_020cb764(0))) &&
         ((s32)((Boss03*)temp_r4)->unk28 < temp_r1_2)))
    {
        return 1;
    }
    return 0;
}

/// MARK: Boss03_04_UG

s32 BtlBoss03_04_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle data_ov017_021427ac = {"Tsk_BtlBoss03_04_UG", BtlBoss03_04_UG_RunTask, 524};

s32 BtlBoss03_04_UG_Init(void* arg1, s32 arg2) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x20C);
    func_ov017_0213df24(1);
    func_ov017_0213e088(8U, 1);
    func_ov003_020c3efc(temp_r4, arg2);
    func_ov003_020c4520(temp_r4);
    func_ov003_020c4b5c(temp_r4);
    ((Boss03*)temp_r4)->unk1CC = 1;
    ((Boss03*)temp_r4)->unk200 = (u8)(((Boss03*)temp_r4)->unk200 & ~2);
    func_ov017_0213d1e0(temp_r4, 0);
    func_ov017_0213d234(temp_r4, 1);
    ((Boss03*)temp_r4)->unk200 = (u8)(((Boss03*)temp_r4)->unk200 | 4);
    func_ov017_0213b454(temp_r4, &func_ov017_0213b534);
    ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x40000000);
    return 1;
}

s32 BtlBoss03_04_UG_Update(void* arg1) {
    void (*temp_r1)(void*);
    s32    temp_r0;
    s32    temp_r0_2;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (func_ov003_020c3bf0(temp_r4) != 0) {
        if (((Boss03*)temp_r4)->unk20 == 6) {
            func_ov003_02082f2c(temp_r4);
        }
        return 1;
    }
    if ((((u32)(((Boss03*)temp_r4)->unk200 << 0x1E) >> 0x1F) == 0) && (func_ov003_020cb648(temp_r4, 0x32) != 0)) {
        ((Boss03*)temp_r4)->unk200 = (u8)(((Boss03*)temp_r4)->unk200 | 2);
        func_ov003_02087f00(SEIDX_HANEKOMA_N_BTL_FB04, func_ov003_020843b0(0, ((Boss03*)temp_r4)->unk28));
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    switch (temp_r0) { /* irregular */
        case 2:
            if (((u32)(((Boss03*)temp_r4)->unk200 << 0x1D) >> 0x1F) == 0) {
                ((Boss03*)temp_r4)->unk38 = 0;
            } else {
                func_ov003_02084694(temp_r4 + 0x144, 0);
                ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 & ~0x10000000);
                func_ov017_0213b454(temp_r4, &func_ov017_0213c7c0);
            }
            break;
        case 3:
            func_ov003_02084694(temp_r4 + 0x144, 1);
            func_ov017_0213b454(temp_r4, &func_ov017_0213c930);
            break;
        case 6:
            func_ov003_02084694(temp_r4 + 0x144, 0);
            temp_r0_2                 = (((Boss03*)temp_r4)->unk54 & ~0x10000000) | 1;
            ((Boss03*)temp_r4)->unk54 = temp_r0_2;
            if (temp_r0_2 == 0) {
                func_ov017_0213b454(temp_r4, &func_ov017_0213ca84);
            }
            break;
    }
    if ((data_ov003_020e71b8->unk3D874 == 2) && (func_ov003_020c3bf0(temp_r4) == 0)) {
        func_ov017_0213d248(temp_r4, 0);
        func_ov017_0213d248(temp_r4, 1);
    }
    temp_r1 = ((Boss03*)temp_r4)->unk1C8;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov003_020c4668(temp_r4);
    ((Boss03*)temp_r4)->unk1F4    = (s16)((Boss03*)temp_r4)->unk6C;
    data_ov003_020e71b8->unk3D8A0 = 0;
    return ((Boss03*)temp_r4)->unk1CC;
}

s32 BtlBoss03_04_UG_Render(void* arg1) {
    func_ov003_020c48b0(((Boss03*)arg1)->unk18);
    return 1;
}

s32 BtlBoss03_04_UG_Destroy(void* arg1) {
    func_ov003_020c492c(((Boss03*)arg1)->unk18);
    func_ov017_0213e074();
    return 1;
}

s32 BtlBoss03_04_UG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_021427b8 = {
        .initialize = BtlBoss03_04_UG_Init,
        .update     = BtlBoss03_04_UG_Update,
        .render     = BtlBoss03_04_UG_Render,
        .cleanup    = BtlBoss03_04_UG_Destroy,
    };
    return data_ov017_021427b8.iter[stage](pool, task, args);
}

/// MARK: Boss03_AppearBlackBall

typedef struct {

} Boss03_AppearBlackBall; // Size: 0xE4

typedef struct {
    /* 0x00 */ s32 unk_0;
    /* 0x04 */ s32 unk_4;
    /* 0x08 */ s32 unk_8;
    /* 0x0C */ s32 unk_C;
    /* 0x10 */ s32 unk_10;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
} Boss03_AppearBlackBall_Args;

s32 BtlBoss03_AppearBlackBall_Init(void* arg0, void* arg1);
s32 BtlBoss03_AppearBlackBall_Update(void* arg0);
s32 BtlBoss03_AppearBlackBall_Render(s32 arg0);
s32 BtlBoss03_AppearBlackBall_Destroy(s32 arg0);
s32 BtlBoss03_AppearBlackBall_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Task_BtlBoss03_AppearBlackBall = {"Task_BtlBoss03_AppearBlackBall", BtlBoss03_AppearBlackBall_RunTask, 0xE4};

s32 BtlBoss03_AppearBlackBall_CreateTask(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    Boss03_AppearBlackBall_Args args;
    args.unk_0  = arg0;
    args.unk_4  = arg1;
    args.unk_8  = arg2;
    args.unk_C  = arg3;
    args.unk_10 = arg4;
    args.unk_14 = arg5;
    args.unk_18 = arg6;

    TaskPool* taskPool;
    if (arg0 != 1) {
        taskPool = &data_ov003_020e71b8->unk_00000;
    } else {
        taskPool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(taskPool, &Task_BtlBoss03_AppearBlackBall, 0, 0, 0, &args);
}

void func_ov017_0213d6b4(s32 arg0, s32 arg1) {
    void* temp_r4 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_00000, arg0);
    void* temp_r0 = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, arg1);

    ((Boss03*)temp_r4)->unk0  = arg1;
    ((Boss03*)temp_r0)->unk0  = arg0;
    ((Boss03*)temp_r4)->unk7C = (void*)(temp_r0 + 4);
    ((Boss03*)temp_r0)->unk7C = (void*)(temp_r4 + 4);
}

s32 BtlBoss03_AppearBlackBall_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    Boss03_AppearBlackBall* appearBlackBall = task->data;

    switch (stage) {
        case 0:
            return BtlBoss03_AppearBlackBall_Init(appearBlackBall, args);
        case 1:
            return BtlBoss03_AppearBlackBall_Update(appearBlackBall);
        case 2:
            return BtlBoss03_AppearBlackBall_Render(appearBlackBall);
        case 3:
            return BtlBoss03_AppearBlackBall_Destroy(appearBlackBall);
    }
    return 1;
}

s32 BtlBoss03_AppearBlackBall_Init(void* arg0, void* arg1) {
    u32 var_r0;

    MI_CpuSet(arg0, 0, 0xE4);
    func_ov003_02083118(arg0 + 4, 2);
    ((Boss03*)arg0)->unk74 = 0xC;
    ((Boss03*)arg0)->unk76 = 0x1E;
    ((Boss03*)arg0)->unk40 = 0x2B8;
    var_r0                 = 0;
    ((Boss03*)arg0)->unk4C = 0;
    ((Boss03*)arg0)->unk50 = 0xF33;
    ((Boss03*)arg0)->unk54 = 0;
    ((Boss03*)arg0)->unk2C = (s32)((Boss03*)arg1)->unk4;
    ((Boss03*)arg0)->unk30 = (s32)((Boss03*)arg1)->unk8;
    ((Boss03*)arg0)->unk34 = (s32)((Boss03*)arg1)->unkC;
    ((Boss03*)arg0)->unk44 = (s32)((Boss03*)arg1)->unk10;
    ((Boss03*)arg0)->unk48 = (s32)((Boss03*)arg1)->unk14;
    ((Boss03*)arg0)->unk3C = (s32)((Boss03*)arg1)->unk18;
    ((Boss03*)arg0)->unk0  = (void*)-1;
    if (((Boss03*)arg1)->unk0 == 1) {
        var_r0 = 1;
    }
    func_ov017_02140b14(var_r0, arg0 + 0x80, 0, 0);
    return 1;
}

s32 BtlBoss03_AppearBlackBall_Update(void* arg0) {
    SubroutineArgs subroutine_arg0;
    s32            sp18;
    s32            sp14;
    s32            sp10;
    s16            temp_r0_2;
    s16            temp_r0_3;
    s16            temp_r0_4;
    s32            temp_r0_5;
    s32*           var_r4;
    void*          var_r5;
    void*          temp_r0;
    void*          temp_r0_6;
    void*          temp_r0_7;
    void*          temp_r1;

    temp_r0 = func_ov003_020cb42c(1, 0x2F, 0);
    if ((data_ov003_020e71b8->unk3D878 & 0x20000000) || ((temp_r0 != NULL) && (((Boss03*)temp_r0)->unk54 & 4))) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02083074(arg0 + 4);
    temp_r0_2 = ((Boss03*)arg0)->unkE2;
    switch (temp_r0_2) { /* irregular */
        case 0:
            temp_r0_3 = ((Boss03*)arg0)->unkE0;
            if (temp_r0_3 == 0) {
                ((Boss03*)arg0)->unkE0 = (s16)(temp_r0_3 + 1);
                func_ov003_020824a0(arg0 + 0x80, 0, 0);
            }
            if ((s32)((Boss03*)arg0)->unk34 > -0x40000) {
                ((Boss03*)arg0)->unkE0 = 0;
                ((Boss03*)arg0)->unkE2 = 1;
            }
        default:
            func_ov003_02082b0c(arg0 + 0x80);
            return 1;
        case 1:
            temp_r0_4 = ((Boss03*)arg0)->unkE0;
            if (temp_r0_4 == 0) {
                ((Boss03*)arg0)->unkE0 = (s16)(temp_r0_4 + 1);
                func_ov003_020824a0(arg0 + 0x80, 6, 0);
            }
            if (((Boss03*)arg0)->unk34 == 0) {
                ((Boss03*)arg0)->unkE0 = 0;
                ((Boss03*)arg0)->unkE2 = 2;
            }
            /* Duplicate return node #32. Try simplifying control flow for better match */
            func_ov003_02082b0c(arg0 + 0x80);
            return 1;
        case 2:
            if (((Boss03*)arg0)->unk34 == 0) {
                if ((((Boss03*)arg0)->unk0 != -1) && (((Boss03*)((Boss03*)arg0)->unk7C)->unk30 != 0)) {
                    /* Duplicate return node #32. Try simplifying control flow for better match */
                    func_ov003_02082b0c(arg0 + 0x80);
                    return 1;
                }
                var_r4    = NULL;
                var_r5    = NULL;
                temp_r0_5 = func_ov003_020c37f8(arg0 + 0x80);
                if (temp_r0_5 != 0) {
                    sp10   = ((Boss03*)arg0)->unk2C;
                    var_r4 = &sp10;
                    sp14   = ((Boss03*)arg0)->unk30;
                    sp18   = ((Boss03*)arg0)->unk34;
                } else {
                    var_r5 = &subroutine_arg0;
                }
                if (((Boss03*)arg0)->unk0 != -1) {
                    temp_r1 = ((Boss03*)arg0)->unk7C;
                    if (temp_r0_5 == 0) {
                        sp10      = ((Boss03*)temp_r1)->unk28;
                        sp14      = ((Boss03*)((Boss03*)arg0)->unk7C)->unk2C;
                        var_r4    = &sp10;
                        sp18      = ((Boss03*)((Boss03*)arg0)->unk7C)->unk30;
                        temp_r0_6 = EasyTask_GetTaskById(&data_ov003_020e71b8->taskPool, ((Boss03*)arg0)->unk0);
                        if (temp_r0_6 != NULL) {
                            ((Boss03*)temp_r0_6)->unk4 = (u16)(((Boss03*)temp_r0_6)->unk4 | 0x10);
                        }
                    } else {
                        var_r5    = &subroutine_arg0;
                        temp_r0_7 = EasyTask_GetTaskById(&data_ov003_020e71b8->unk_00000, ((Boss03*)arg0)->unk0);
                        if (temp_r0_7 != NULL) {
                            ((Boss03*)temp_r0_7)->unk4 = (u16)(((Boss03*)temp_r0_7)->unk4 | 0x10);
                        }
                    }
                }
                func_ov017_0213ee0c();
                func_ov017_0213efa8(var_r4, var_r5);
                return 0;
            }
            /* Duplicate return node #32. Try simplifying control flow for better match */
            func_ov003_02082b0c(arg0 + 0x80);
            return 1;
    }
}

s32 BtlBoss03_AppearBlackBall_Render(s32 arg0) {
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_020831e4(arg0 + 4, arg0 + 0x80);
    return 1;
}

s32 BtlBoss03_AppearBlackBall_Destroy(s32 arg0) {
    func_ov003_02082cc4(arg0 + 0x80);
    return 1;
}

s32 BtlBoss03_BlckBall_Abs_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Task_BtlBoss03_BlckBall_Abs = {"Task_BtlBoss03_BlckBall_Abs", BtlBoss03_BlckBall_Abs_RunTask, 0x98};

s32 BtlBoss03_BlckBall_Abs_CreateTask(void* arg0) {
    void**    sp8;
    TaskPool* taskPool;

    if (func_ov003_020c37f8(arg0 + 0x84) == 0) {
        taskPool = &data_ov003_020e71b8->unk_00000;
    } else {
        taskPool = &data_ov003_020e71b8->taskPool;
    }
    sp8 = arg0;
    return EasyTask_CreateTask(taskPool, &Task_BtlBoss03_BlckBall_Abs, 0, 0, 0, &sp8);
}

s32 BtlBoss03_BlckBall_Abs_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    s16    sp1A;
    s16    sp18;
    s32    var_r0;
    s32    var_r0_2;
    s32    var_r0_3;
    void*  temp_r0;
    void*  temp_r0_2;
    void*  temp_r1;
    void*  temp_r1_2;
    void*  temp_r4;
    void*  temp_r4_2;
    void** temp_r5;

    temp_r5 = ((Boss03*)task)->unk18;
    switch (stage) {
        default:
            return 1;
        case 0:
            if (func_ov003_020c37f8(args + 0x84) != 0) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            temp_r4 = func_ov017_02130408(var_r0);
            MI_CpuSet(temp_r5, 0, 0x98);
            func_ov017_02140b14((u32)(((Boss03*)args)->unk84 << 0x1E) >> 0x1E, (Sprite*)temp_r5, 0, 0);
            func_ov003_020824a0(temp_r5, 6, 0);
            ((Boss03*)temp_r5)->unk60 = (s32)((Boss03*)args)->unk28;
            ((Boss03*)temp_r5)->unk64 = (s32)((Boss03*)args)->unk2C;
            ((Boss03*)temp_r5)->unk68 = (s32)((Boss03*)args)->unk30;
            ((Boss03*)temp_r5)->unk6C = (s32)(((Boss03*)temp_r4)->unk28 + ((RNG_Next(0x31) - 0x18) << 0xC));
            ((Boss03*)temp_r5)->unk70 = (s32)((Boss03*)temp_r4)->unk2C;
            ((Boss03*)temp_r5)->unk74 = (s32)((((Boss03*)temp_r4)->unk30 - 0x20000) + ((RNG_Next(0x41) - 0x20) << 0xC));
            func_ov003_020cb910(temp_r5 + 0x78, temp_r5 + 0x7C, temp_r5 + 0x80, ((Boss03*)temp_r5)->unk60,
                                ((Boss03*)temp_r5)->unk64, ((Boss03*)temp_r5)->unk68, ((Boss03*)temp_r5)->unk6C,
                                ((Boss03*)temp_r5)->unk70, ((Boss03*)temp_r5)->unk74, 0x2000);
            ((Boss03*)temp_r5)->unk90 = 1;
            ((Boss03*)temp_r5)->unk94 = (void*)args;
            ((Boss03*)args)->unk18E   = 0;
            temp_r1                   = args;
            ((Boss03*)temp_r1)->unk54 = (s32)(((Boss03*)temp_r1)->unk54 | 0x20);
            temp_r1_2                 = ((Boss03*)args)->unk184;
            if (temp_r1_2 != NULL) {
                ((Boss03*)temp_r1_2)->unk54 = (s32)(((Boss03*)temp_r1_2)->unk54 | 0x20);
            }
            if (data_ov003_020e71b8->unk3D8A0 == args) {
                data_ov003_020e71b8->unk3D8A0 = 0;
            }
            temp_r0 = ((Boss03*)args)->unk184;
            if (temp_r0 != NULL) {
                ((Boss03*)temp_r0)->unk184                = 0;
                ((Boss03*)((Boss03*)args)->unk184)->unk78 = 0;
                ((Boss03*)args)->unk184                   = NULL;
                ((Boss03*)args)->unk78                    = 0;
            }
            temp_r0_2                    = args;
            ((Boss03*)temp_r0_2)->unk18C = (u16)(((Boss03*)temp_r0_2)->unk18C | 0x1000);
            func_ov003_020c4fc8(args);
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 1;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            if (((Boss03*)temp_r5)->unk88 != 0) {

            } else {
                if (((Boss03*)temp_r5)->unk84 == 0) {
                    if (func_ov003_020c37f8(((Boss03*)temp_r5)->unk94 + 0x84) != 0) {
                        var_r0_2 = 1;
                    } else {
                        var_r0_2 = 0;
                    }
                    temp_r4_2                 = func_ov017_02130408(var_r0_2);
                    ((Boss03*)temp_r5)->unk84 = (s16)(((Boss03*)temp_r5)->unk84 + 1);
                    ((Boss03*)temp_r5)->unk60 = (s32)((Boss03*)((Boss03*)temp_r5)->unk94)->unk28;
                    ((Boss03*)temp_r5)->unk64 = (s32)((Boss03*)((Boss03*)temp_r5)->unk94)->unk2C;
                    ((Boss03*)temp_r5)->unk68 = (s32)((Boss03*)((Boss03*)temp_r5)->unk94)->unk30;
                    ((Boss03*)temp_r5)->unk6C = (s32)(((Boss03*)temp_r4_2)->unk28 + ((RNG_Next(0x31) - 0x18) << 0xC));
                    ((Boss03*)temp_r5)->unk70 = (s32)((Boss03*)temp_r4_2)->unk2C;
                    ((Boss03*)temp_r5)->unk74 =
                        (s32)((((Boss03*)temp_r4_2)->unk30 - 0x20000) + ((RNG_Next(0x41) - 0x20) << 0xC));
                    func_ov003_020cb910(temp_r5 + 0x78, temp_r5 + 0x7C, temp_r5 + 0x80, ((Boss03*)temp_r5)->unk60,
                                        ((Boss03*)temp_r5)->unk64, ((Boss03*)temp_r5)->unk68, ((Boss03*)temp_r5)->unk6C,
                                        ((Boss03*)temp_r5)->unk70, ((Boss03*)temp_r5)->unk74, 0x2000);
                }
                ((Boss03*)temp_r5)->unk60 = (s32)(((Boss03*)temp_r5)->unk60 + ((Boss03*)temp_r5)->unk78);
                ((Boss03*)temp_r5)->unk64 = (s32)(((Boss03*)temp_r5)->unk64 + ((Boss03*)temp_r5)->unk7C);
                ((Boss03*)temp_r5)->unk68 = (s32)(((Boss03*)temp_r5)->unk68 + ((Boss03*)temp_r5)->unk80);
                if (func_ov003_020cba54(((Boss03*)temp_r5)->unk60, ((Boss03*)temp_r5)->unk64, ((Boss03*)temp_r5)->unk68,
                                        ((Boss03*)temp_r5)->unk6C, ((Boss03*)temp_r5)->unk70,
                                        ((Boss03*)temp_r5)->unk74) < 0x2000)
                {
                    ((Boss03*)temp_r5)->unk84 = 0;
                    ((Boss03*)temp_r5)->unk88 = 1;
                }
            }
            func_ov003_02082b0c(temp_r5);
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 1;
        case 2:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            if (func_ov003_020c37f8(temp_r5) != 0) {
                var_r0_3 = 1;
            } else {
                var_r0_3 = 0;
            }
            func_ov003_02084348(var_r0_3, &sp1A, &sp18, ((Boss03*)temp_r5)->unk60);
            func_ov003_02082724(temp_r5, sp1A, sp18);
            func_ov003_02082730(temp_r5, 0x7FFFFFFF - ((Boss03*)temp_r5)->unk64);
            func_ov003_02082b64(temp_r5);
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 1;
        case 3:
            func_ov003_02082cc4(temp_r5);
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 1;
    }
}

void func_ov017_0213df24(s32 arg0) {
    s32* temp_r1;
    s32  temp_hi;
    s32  temp_hi_2;
    s32  var_r10;
    s32  var_r3;
    s32  var_r7;
    u32  temp_r0;
    u32  temp_r0_2;
    u32  temp_r0_3;
    u32  temp_r0_4;
    u32  temp_r4;
    u32  temp_r7;
    u32  temp_r8;
    u32  var_r5;
    u32  var_r6;
    u32  var_r9;

    var_r5  = 0;
    var_r6  = 0;
    var_r9  = 0;
    var_r7  = 0;
    temp_r1 = func_ov017_0213e204(arg0);
    do {
        temp_r0 = func_ov003_020cb368(temp_r1[var_r7]);
        temp_hi = temp_r0 > var_r5;
        var_r7 += 1;
        if (temp_hi) {
            var_r5 = temp_r0;
        }
    } while (var_r7 < 0xE);
    temp_r0_2 = func_ov003_020cb368(temp_r1[14]);
    if (temp_r0_2 > 0U) {
        var_r6 = temp_r0_2;
    }
    temp_r7 = func_ov003_020cb368(temp_r1[15]);
    temp_r8 = func_ov003_020cb368(temp_r1[16]);
    var_r10 = 0x11;
    do {
        temp_r0_3 = func_ov003_020cb368(temp_r1[var_r10]);
        temp_hi_2 = temp_r0_3 > var_r9;
        var_r10 += 1;
        if (temp_hi_2) {
            var_r9 = temp_r0_3;
        }
    } while (var_r10 < 0x13);
    func_ov003_020cb128(&data_ov017_02143a38, 0, 0U);
    func_ov003_020cb128(&data_ov017_02143a40, 0, var_r5);
    temp_r4 = var_r5 + var_r6;
    func_ov003_020cb128(&data_ov017_02143a48, 0, temp_r4);
    func_ov003_020cb128(&data_ov017_02143a50, 0, temp_r7 + temp_r4);
    func_ov003_020cb128(&data_ov017_02143a58, 0, temp_r4);
    temp_r0_4 = temp_r7 + temp_r8;
    if (temp_r0_4 > var_r9) {
        var_r3 = temp_r4 + temp_r0_4;
    } else {
        var_r3 = temp_r4 + var_r9;
    }
    func_ov003_020cb130(&data_ov017_02143a24, &data_ov017_02143a38, 0x13, var_r3, &data_ov017_021436ac);
    func_ov003_020cb150(&data_ov017_02143a24);
}

void func_ov017_0213e074(void) {
    func_ov003_020cb194(&data_ov017_02143a24);
}

void func_ov017_0213e088(u32 arg0, s32 arg1) {
    s32* table;
    s32  temp_r4;
    u16  var_r5;

    var_r5  = 0;
    table   = func_ov017_0213e204(arg1);
    temp_r4 = table[arg0];
    if (arg0 <= 0xDU) {
        var_r5 = 0;
    } else {
        switch (arg0) { /* irregular */
            case 14:
                var_r5 = 1;
                break;
            case 15:
                var_r5 = 2;
                break;
            case 16:
                var_r5 = 3;
                break;
            default:
                var_r5 = 4;
                break;
        }
    }
    if (func_ov003_020cb32c(&data_ov017_02143a24, temp_r4) == 1) {
        return;
    }
    if (*(s32*)((char*)&data_ov017_02143a38 + (var_r5 * 8)) != 0) {
        func_ov003_020cb304(&data_ov017_02143a24, var_r5);
    }
    *(s32*)((char*)&data_ov017_02143a38 + (var_r5 * 8)) = temp_r4;
    func_ov003_020cb200(&data_ov017_02143a24, var_r5);
}

void func_ov017_0213e134(s32 arg0, s32 arg1) {
    s32* table;
    u16  var_r4;

    table  = func_ov017_0213e204(arg1);
    var_r4 = 0;
loop_1:
    if (table[var_r4] == arg0) {
        func_ov017_0213e088((u32)var_r4, arg1);
        return;
    }
    var_r4 += 1;
    if ((s32)var_r4 >= 0x13) {
        return;
    }
    goto loop_1;
}

void func_ov017_0213e17c(u32 arg0, s32 arg1) {
    u16 var_r4;

    var_r4 = 0;
    if (arg0 <= 0xDU) {
        var_r4 = 0;
    } else {
        switch (arg0) { /* irregular */
            case 14:
                var_r4 = 1;
                break;
            case 15:
                var_r4 = 2;
                break;
            case 16:
                var_r4 = 3;
                break;
            default:
                var_r4 = 4;
                break;
        }
    }
    if (*(s32*)((char*)&data_ov017_02143a38 + (var_r4 * 8)) != *(s32*)((char*)&data_ov017_02143660 + (arg0 * 4))) {
        return;
    }
    func_ov003_020cb304(&data_ov017_02143a24, var_r4);
}

s32* func_ov017_0213e204(s32 arg0) {
    if (arg0 == 1) {
        return (s32*)&data_ov017_02143614;
    }
    return (s32*)&data_ov017_02143660;
}

s32 BtlBoss03_EffPress_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
s32 BtlBoss03_EffCatch_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
s32 BtlBoss03_EffPosture_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
s32 BtlBoss03_EffSign_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_EffPress    = {"Tsk_BtlBoss03_EffPress", BtlBoss03_EffPress_RunTask, 0x68};
const TaskHandle Tsk_BtlBoss03_EffCatch    = {"Tsk_BtlBoss03_EffCatch", BtlBoss03_EffCatch_RunTask, 0x64};
const TaskHandle Task_BtlBoss03_EffPosture = {"Task_BtlBoss03_EffPosture", BtlBoss03_EffPosture_RunTask, 0x64};
const TaskHandle Task_BtlBoss03_EffSign    = {"Task_BtlBoss03_EffSign", BtlBoss03_EffSign_RunTask, 0x68};

s32 BtlBoss03_EffSign_CreateTask(void* arg0) {
    void** sp8;
    void*  var_r0;

    sp8 = arg0;
    if (func_ov003_020c37f8(arg0 + 0x84) == 0) {
        var_r0 = &data_ov003_020e71b8->unk_00000;
    } else {
        var_r0 = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(var_r0, &Task_BtlBoss03_EffSign, 0, 0, 0, &sp8);
}

void BtlBoss03_EffPosture_CreateTask(void* arg0) {
    void** sp8;
    void*  var_r0;

    sp8 = arg0;
    if (func_ov003_020c37f8(arg0 + 0x84) == 0) {
        var_r0 = &data_ov003_020e71b8->unk_00000;
    } else {
        var_r0 = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(var_r0, &Task_BtlBoss03_EffPosture, 0, 0, 0, &sp8);
}

void BtlBoss03_EffCatch_CreateTask(void* arg0) {
    void**    sp8;
    TaskPool* taskPool;

    sp8 = arg0;
    if (func_ov003_020c37f8(arg0 + 0x84) == 0) {
        taskPool = &data_ov003_020e71b8->unk_00000;
    } else {
        taskPool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(taskPool, &Tsk_BtlBoss03_EffCatch, 0, 0, 0, &sp8);
}

void BtlBoss03_EffPress_CreateTask(void* arg0) {
    void**    sp8;
    TaskPool* taskPool;

    if (data_ov017_02143a60 != 0) {
        sp8 = arg0;
        if (func_ov003_020c37f8(arg0 + 0x84) == 0) {
            taskPool = &data_ov003_020e71b8->unk_00000;
        } else {
            taskPool = &data_ov003_020e71b8->taskPool;
        }
        EasyTask_CreateTask(taskPool, &Tsk_BtlBoss03_EffPress, 0, 0, 0, &sp8);
    }
}

void func_ov017_0213e3a4(void) {
    data_ov017_02143a60 = 1;
}

void func_ov017_0213e3b8(void) {
    data_ov017_02143a60 = 0;
}

s32 BtlBoss03_EffSign_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    s32    var_r3;
    s32    var_r5;
    void*  temp_r0;
    void*  temp_r1;
    void** temp_r4;

    temp_r4 = ((Boss03*)task)->unk18;
    var_r5  = 1;
    switch (stage) {
        default:
            return var_r5;
        case 0:
            MI_CpuSet(temp_r4, 0, 0x68);
            temp_r1 = args;
            if (((Boss03*)temp_r1)->unk7C == 0x2F) {
                var_r3 = 1;
            } else {
                var_r3 = 0;
            }
            func_ov017_02140b14((u32)(((Boss03*)temp_r1)->unk84 << 0x1E) >> 0x1E, (Sprite*)temp_r4, 3, var_r3);
            func_ov003_020824a0(temp_r4, 3, 1);
            temp_r0                   = args;
            ((Boss03*)temp_r4)->unk60 = temp_r0;
            if (((Boss03*)temp_r0)->unk24 == 1) {
                func_ov003_02082750(temp_r4, 1);
            }
            /* Duplicate return node #23. Try simplifying control flow for better match */
            return var_r5;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            if (((Boss03*)temp_r4)->unk64 == 0x10) {
                func_ov003_02087ed8(0x2F0);
            }
            func_ov003_02082b0c(temp_r4);
            if (SpriteMgr_IsAnimationFinished((Sprite*)temp_r4) != 0) {
                var_r5 = 0;
            } else {
                ((Boss03*)temp_r4)->unk64 = (s16)(((Boss03*)temp_r4)->unk64 + 1);
            }
            /* Duplicate return node #23. Try simplifying control flow for better match */
            return var_r5;
        case 2:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            func_ov003_02082724(temp_r4, 0x80, 0x60);
            func_ov003_02082730(temp_r4, 0x7FFFFFFF - ((Boss03*)((Boss03*)temp_r4)->unk60)->unk2C);
            func_ov003_02082b64(temp_r4);
            /* Duplicate return node #23. Try simplifying control flow for better match */
            return var_r5;
        case 3:
            func_ov003_02082cc4(temp_r4);
            /* Duplicate return node #23. Try simplifying control flow for better match */
            return var_r5;
    }
}

s32 BtlBoss03_EffPosture_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    s16    spA;
    s16    sp8;
    s32    var_r0;
    s32    var_r3;
    s32    var_r5;
    void*  temp_r0;
    void*  temp_r1;
    void*  temp_r6;
    void** temp_r4;

    temp_r4 = ((Boss03*)task)->unk18;
    var_r5  = 1;
    switch (stage) {
        default:
            return var_r5;
        case 0:
            MI_CpuSet(temp_r4, 0, 0x64);
            temp_r1 = args;
            if (((Boss03*)temp_r1)->unk7C == 0x2F) {
                var_r3 = 1;
            } else {
                var_r3 = 0;
            }
            func_ov017_02140b14((u32)(((Boss03*)temp_r1)->unk84 << 0x1E) >> 0x1E, (Sprite*)temp_r4, 3, var_r3);
            func_ov003_020824a0(temp_r4, 0, 1);
            temp_r0                   = args;
            ((Boss03*)temp_r4)->unk60 = temp_r0;
            if (((Boss03*)temp_r0)->unk24 == 1) {
                func_ov003_02082750(temp_r4, 1);
            }
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return var_r5;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            func_ov003_02082b0c(temp_r4);
            if (SpriteMgr_IsAnimationFinished((Sprite*)temp_r4) != 0) {
                var_r5 = 0;
            }
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return var_r5;
        case 2:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            temp_r6 = ((Boss03*)temp_r4)->unk60;
            if (func_ov003_020c37f8(temp_r4) != 0) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            func_ov003_02084348(var_r0, &spA, &sp8, ((Boss03*)temp_r6)->unk28, ((Boss03*)temp_r6)->unk30);
            func_ov003_02082724(temp_r4, spA, sp8);
            func_ov003_02082730(temp_r4, 0x7FFFFFFF - ((Boss03*)((Boss03*)temp_r4)->unk60)->unk2C);
            func_ov003_02082b64(temp_r4);
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return var_r5;
        case 3:
            func_ov003_02082cc4(temp_r4);
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return var_r5;
    }
}

s32 BtlBoss03_EffCatch_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    s16    spA;
    s16    sp8;
    s32    var_r0;
    s32    var_r3;
    s32    var_r5;
    void*  temp_r0;
    void*  temp_r1;
    void*  temp_r6;
    void** temp_r4;

    temp_r4 = ((Boss03*)task)->unk18;
    var_r5  = 1;
    switch (stage) {
        default:
            return var_r5;
        case 0:
            MI_CpuSet(temp_r4, 0, 0x64);
            temp_r1 = args;
            if (((Boss03*)temp_r1)->unk7C == 0x2F) {
                var_r3 = 1;
            } else {
                var_r3 = 0;
            }
            func_ov017_02140b14((u32)(((Boss03*)temp_r1)->unk84 << 0x1E) >> 0x1E, (Sprite*)temp_r4, 3, var_r3);
            func_ov003_020824a0(temp_r4, 1, 1);
            temp_r0                   = args;
            ((Boss03*)temp_r4)->unk60 = temp_r0;
            if (((Boss03*)temp_r0)->unk24 == 1) {
                func_ov003_02082750(temp_r4, 1);
            }
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return var_r5;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            func_ov003_02082b0c(temp_r4);
            if (SpriteMgr_IsAnimationFinished((Sprite*)temp_r4) != 0) {
                var_r5 = 0;
            }
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return var_r5;
        case 2:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            temp_r6 = ((Boss03*)temp_r4)->unk60;
            if (func_ov003_020c37f8(temp_r4) != 0) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            func_ov003_02084348(var_r0, &spA, &sp8, ((Boss03*)temp_r6)->unk28, ((Boss03*)temp_r6)->unk30);
            func_ov003_02082724(temp_r4, spA, sp8);
            func_ov003_02082730(temp_r4, 0x7FFFFFFF - ((Boss03*)((Boss03*)temp_r4)->unk60)->unk2C);
            func_ov003_02082b64(temp_r4);
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return var_r5;
        case 3:
            func_ov003_02082cc4(temp_r4);
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return var_r5;
    }
}

s32 BtlBoss03_EffPress_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    s16    spA;
    s16    sp8;
    s16    temp_r0_2;
    s32    var_r0;
    s32    var_r0_2;
    s32    var_r3;
    s32    var_r5;
    void*  temp_r0;
    void*  temp_r1;
    void*  temp_r1_2;
    void** temp_r4;

    temp_r4 = ((Boss03*)task)->unk18;
    var_r5  = 1;
    switch (stage) {
        default:
            return var_r5;
        case 0:
            MI_CpuSet(temp_r4, 0, 0x68);
            temp_r1 = args;
            if (((Boss03*)temp_r1)->unk7C == 0x2F) {
                var_r3 = 1;
            } else {
                var_r3 = 0;
            }
            func_ov017_02140b14((u32)(((Boss03*)temp_r1)->unk84 << 0x1E) >> 0x1E, (Sprite*)temp_r4, 3, var_r3);
            func_ov003_020824a0(temp_r4, 2, 1);
            temp_r0                   = args;
            ((Boss03*)temp_r4)->unk60 = temp_r0;
            if (((Boss03*)temp_r0)->unk24 == 1) {
                func_ov003_02082750(temp_r4, 1);
            }
            /* Duplicate return node #31. Try simplifying control flow for better match */
            return var_r5;
        case 1:
            temp_r0_2 = ((Boss03*)temp_r4)->unk64;
            if (temp_r0_2 == 0) {
                ((Boss03*)temp_r4)->unk64 = (s16)(temp_r0_2 + 1);
                temp_r1_2                 = ((Boss03*)temp_r4)->unk60;
                if (func_ov003_020c37f8(((Boss03*)temp_r4)->unk60 + 0x84) != 0) {
                    var_r0 = func_ov003_020843b0(1, ((Boss03*)temp_r1_2)->unk28);
                } else {
                    var_r0 = func_ov003_020843b0(0, ((Boss03*)temp_r1_2)->unk28);
                }
                func_ov003_02087f00(SEIDX_SE_B006_TUKAMIKOUGEKI_02, var_r0);
            }
            if (data_ov017_02143a60 == 0) {
                return 0;
            }
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            func_ov003_02082b0c(temp_r4);
            if (SpriteMgr_IsAnimationFinished((Sprite*)temp_r4) != 0) {
                var_r5 = 0;
            }
            /* Duplicate return node #31. Try simplifying control flow for better match */
            return var_r5;
        case 2:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            if (func_ov003_020c37f8(temp_r4) != 0) {
                var_r0_2 = 1;
            } else {
                var_r0_2 = 0;
            }
            func_ov003_02084348(var_r0_2, &spA, &sp8, ((Boss03*)((Boss03*)temp_r4)->unk60)->unk28);
            func_ov003_02082724(temp_r4, spA, sp8);
            func_ov003_02082730(temp_r4, 0x7FFFFFFF - ((Boss03*)((Boss03*)temp_r4)->unk60)->unk2C);
            func_ov003_02082b64(temp_r4);
            /* Duplicate return node #31. Try simplifying control flow for better match */
            return var_r5;
        case 3:
            func_ov003_02082cc4(temp_r4);
            /* Duplicate return node #31. Try simplifying control flow for better match */
            return var_r5;
    }
}

s32 BtlBoss03_EffRush_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Task_BtlBoss03_EffRush = {"Task_BtlBoss03_EffRush", BtlBoss03_EffRush_RunTask, 0x64};

s32 BtlBoss03_EffRush_CreateTask(void* arg0) {
    void** sp8;
    void*  var_r0;

    sp8 = arg0;
    if (func_ov003_020c37f8(arg0 + 0x84) == 0) {
        var_r0 = &data_ov003_020e71b8->unk_00000;
    } else {
        var_r0 = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(var_r0, &Task_BtlBoss03_EffRush, 0, 0, 0, &sp8);
}

s32 BtlBoss03_EffRush_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    s16    spA;
    s16    sp8;
    s32    var_r0;
    s32    var_r3;
    s32    var_r5;
    void*  temp_r1;
    void*  temp_r6;
    void** temp_r4;

    temp_r4 = ((Boss03*)task)->unk18;
    var_r5  = 1;
    switch (stage) {
        default:
            return var_r5;
        case 0:
            MI_CpuSet(temp_r4, 0, 0x64);
            temp_r1 = args;
            if (((Boss03*)temp_r1)->unk7C == 0x2F) {
                var_r3 = 1;
            } else {
                var_r3 = 0;
            }
            func_ov017_02140b14((u32)(((Boss03*)temp_r1)->unk84 << 0x1E) >> 0x1E, (Sprite*)(temp_r4 + 4), 4, var_r3);
            func_ov003_020824a0(temp_r4 + 4, 0, 0);
            if (((Boss03*)args)->unk24 == 1) {
                func_ov003_02082750(temp_r4 + 4, 1);
            }
            *temp_r4 = *(void**)args;
            /* Duplicate return node #25. Try simplifying control flow for better match */
            return var_r5;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            if (Sprite_HasAnimation(*temp_r4 + 0x84) == 0) {
                var_r5 = 0;
            }
            func_ov003_02082b0c(temp_r4 + 4);
            /* Duplicate return node #25. Try simplifying control flow for better match */
            return var_r5;
        case 2:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            temp_r6 = *temp_r4;
            if (func_ov003_020c37f8(temp_r4 + 4) != 0) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            func_ov003_02084348(var_r0, &spA, &sp8, ((Boss03*)temp_r6)->unk28, ((Boss03*)temp_r6)->unk30);
            func_ov003_02082724(temp_r4 + 4, spA, sp8);
            func_ov003_02082730(temp_r4 + 4, 0x7FFFFFFF - ((Boss03*)*temp_r4)->unk2C);
            func_ov003_02082b64(temp_r4 + 4);
            /* Duplicate return node #25. Try simplifying control flow for better match */
            return var_r5;
        case 3:
            func_ov003_02082cc4(temp_r4 + 4);
            /* Duplicate return node #25. Try simplifying control flow for better match */
            return var_r5;
    }
}

/// MARK: BtlBoss03_FailPly

typedef struct {
    /* 0x00 */ char  unk_00[0x28];
    /* 0x28 */ void* unk_28;
    /* 0x2C */ char  unk_2C[0x4];
    /* 0x30 */ s32   unk_30;
    /* 0x34 */ s32   unk_34;
    /* 0x38 */ s32   unk_38;
    /* 0x3C */ s32   unk_3C;
    /* 0x40 */ char  unk_40[0x54 - 0x40];
    /* 0x54 */ s32   unk_54;
} UnkFailPlyStruct;

typedef struct {
    /* 0x00 */ void*             unk_00;
    /* 0x04 */ UnkFailPlyStruct* unk_04;
    /* 0x08 */ u32               unk_08;
    /* 0x0C */ u16               unk_0C;
    /* 0x0E */ u16               unk_0E;
    /* 0x10 */ u16               unk_10;
    /* 0x12 */ u16               unk_12;
} BtlBoss03_FailPly; // Size: 0x14

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} BtlBoss03_FailPly_Args;

s32 BtlBoss03_FailPly_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_FailPly = {"Tsk_BtlBoss03_FailPly", BtlBoss03_FailPly_RunTask, 0x14};

void BtlBoss03_FailPly_CreateTask(s32 arg0, s32 arg1) {
    BtlBoss03_FailPly_Args args;
    TaskPool*              taskPool;

    args.unk_0 = arg0;
    args.unk_4 = arg1;
    if (func_ov003_020c37f8(arg0 + 0x84) == 0) {
        taskPool = &data_ov003_020e71b8->unk_00000;
    } else {
        taskPool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(taskPool, &Tsk_BtlBoss03_FailPly, NULL, 0, NULL, &args);
}

s32 BtlBoss03_FailPly_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlBoss03_FailPly*      failPly     = task->data;
    BtlBoss03_FailPly_Args* failPlyArgs = args;

    void* temp_r4 = ((Boss03*)task)->unk18;
    s32   var_r5  = 1;

    switch (stage) {
        case 0: {
            failPly->unk_04 = failPlyArgs->unk_4;

            void* temp_r3   = failPlyArgs->unk_0;
            failPly->unk_0C = (u16)((Boss03*)temp_r3)->unk4;
            failPly->unk_0E = (u16)((Boss03*)temp_r3)->unk6;
            failPly->unk_10 = (u16)((Boss03*)temp_r3)->unk8;
            failPly->unk_12 = (u16)((Boss03*)temp_r3)->unkA;
            failPly->unk_00 = failPlyArgs->unk_0;

            if (func_ov003_020c37f8(failPlyArgs->unk_0 + 0x84) != 0) {
                failPly->unk_08 = 251;
            } else {
                failPly->unk_08 = 245;
            }
        } break;

        case 1: {
            if (failPly->unk_04->unk_30 + (failPly->unk_04->unk_38 + failPly->unk_04->unk_3C) >= 0) {
                BtlBoss03_EffPress_CreateTask(failPly->unk_00);
                failPly->unk_04->unk_54 &= ~0x20000;
                failPly->unk_04->unk_54 &= ~0x4000;
                data_ov003_020e71b8->unk3D878 &= ~0x80000;
                func_ov003_0208a1e8(func_ov003_0208a114(failPly->unk_08), &failPly->unk_0C, failPly->unk_04->unk_28);
                func_ov003_0208afcc(0);
                var_r5 = 0;
            }
        } break;
    }
    return var_r5;
}

void* func_ov017_0213ece0(u32 arg0) {
    switch (arg0) {
        default:
            return NULL;
        case 0:
            return &data_ov009_02124620;
        case 1:
            return &data_ov009_02123944;
        case 2:
            return func_ov009_020f3448(4);
        case 3:
            return func_ov009_020f6f88(2);
        case 4:
            return &data_ov009_02124174;
    }
}

/// MARK: BtlBoss03_KindanMgr

s32 BtlBoss03_KindanMgr_Init(void* arg0, s32);
s32 BtlBoss03_KindanMgr_Update(void* arg0);
s32 BtlBoss03_KindanMgr_Destroy(s32 arg0);

s32 BtlBoss03_KindanMgr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Task_BtlBoss03_KindanMgr = {"Task_BtlBoss03_KindanMgr", BtlBoss03_KindanMgr_RunTask, 0x88};

void BtlBoss03_KindanMgr_CreateTask(void) {
    s32 sp8;

    data_ov017_02143a64 = EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &Task_BtlBoss03_KindanMgr, 0, 0, 0, &sp8);
}

void func_ov017_0213ed98(s32 arg0, void* arg1) {
    void* temp_r0 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);

    ((Boss03*)temp_r0)->unk8 = arg0;
    ((Boss03*)temp_r0)->unkC = arg1;
}

void func_ov017_0213edd4(void) {
    void* temp_r0 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);

    ((Boss03*)temp_r0)->unk38 = (s16)(((Boss03*)temp_r0)->unk38 + 1);
}

void func_ov017_0213ee0c(void) {
    void* temp_r0 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);

    s16 temp_r1 = ((Boss03*)temp_r0)->unk38;
    if ((s32)temp_r1 > 0) {
        ((Boss03*)temp_r0)->unk38 = (s16)(temp_r1 - 1);
    }
}

void* func_ov017_0213ee48(s32 arg0, s32 arg1) {
    void* temp_r0 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);

    if (arg0 == 1) {
        return EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, ((Boss03*)(temp_r0 + (arg1 * 8)))->unk40);
    }
    return EasyTask_GetTaskData(&data_ov003_020e71b8->unk_00000, ((Boss03*)(temp_r0 + (arg1 * 8)))->unk44);
}

s32 func_ov017_0213eeb8(void) {
    s32   var_r3;
    void* temp_r0;
    void* temp_r2;

    temp_r0 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);
    if ((func_ov003_020cb348(temp_r0 + 0x64, 0) > 1) || (func_ov003_020cb348(temp_r0 + 0x64, 1) > 1)) {
        return 0;
    }
    var_r3 = 0;
loop_4:
    temp_r2 = temp_r0 + (var_r3 * 8);
    if ((((Boss03*)temp_r2)->unk40 == -1) && (((Boss03*)temp_r2)->unk44 == -1)) {
        var_r3 += 1;
        if (var_r3 >= 4) {
            return 1;
        }
        goto loop_4;
    }
    return 0;
}

s32 func_ov017_0213ef4c(void) {
    s16   temp_r1_2;
    s16   temp_r2;
    s32   var_r3;
    s32   var_r4;
    void* temp_r0;
    void* temp_r1;

    var_r4  = 0;
    temp_r0 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);
    var_r3  = 0;
    do {
        temp_r1   = temp_r0 + (var_r3 * 2);
        temp_r2   = ((Boss03*)temp_r1)->unk24;
        temp_r1_2 = ((Boss03*)temp_r1)->unk2E;
        var_r3 += 1;
        var_r4 += temp_r2 - temp_r1_2;
    } while (var_r3 < 5);
    return var_r4 - ((Boss03*)temp_r0)->unk38;
}

void func_ov017_0213efa8(s32* arg0, s32* arg1) {
    void* temp_r0;
    s32   temp_r3;
    s32   var_r0;
    s32   var_r5;
    s32   var_r6;
    u16   temp_r5;
    void* temp_r1;
    void* temp_r1_2;
    void* temp_r2;
    void* temp_r4;

    temp_r4 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);
    func_ov017_0213ef4c();
    var_r0 = RNG_Next(0);
    var_r5 = 0;
loop_1:
    temp_r1 = temp_r4 + (var_r5 * 2);
    temp_r3 = var_r5 * 2;
    var_r0 -= ((Boss03*)temp_r1)->unk24 - ((Boss03*)temp_r1)->unk2E;
    if (var_r0 < 0) {
        temp_r1_2                             = temp_r4 + 0x2E;
        ((Boss03*)(temp_r4 + temp_r3))->unk2E = (s16)(((Boss03*)(temp_r4 + temp_r3))->unk2E + 1);
    } else {
        var_r5 += 1;
        if (var_r5 < 5) {
            goto loop_1;
        }
    }
    if (var_r5 == 5) {
        return;
    }
    temp_r5 = (u16)var_r5;
    var_r6  = 0;
loop_7:
    temp_r2 = temp_r4 + (var_r6 * 8);
    if ((((Boss03*)temp_r2)->unk40 == -1) && (((Boss03*)temp_r2)->unk44 == -1)) {

    } else {
        var_r6 += 1;
        if (var_r6 < 4) {
            goto loop_7;
        }
    }
    if (var_r6 == 4) {
        return;
    }
    temp_r0 = func_ov017_0213ece0((u32)temp_r5);
    if (func_ov003_020cb32c(temp_r4 + 0x64, temp_r0) == 0) {
        func_ov003_020cb264(temp_r4 + 0x64, temp_r0);
    }
    BtlBoss03_NoisAppr_CreateTask(temp_r5, arg0, arg1, temp_r4 + 0x40 + (var_r6 * 8), temp_r4 + 0x44 + (var_r6 * 8));
}

void func_ov017_0213f0bc(s32 arg0) {
    s32   var_r5;
    void* temp_r0;
    void* temp_r0_2;
    void* temp_r4;

    temp_r4 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);
    var_r5  = 0;
    do {
        temp_r0 = func_ov017_0213ee48(arg0, var_r5);
        if ((temp_r0 != NULL) && !(((Boss03*)temp_r0)->unk54 & 4)) {
            if (arg0 == 1) {
                ((Boss03*)(temp_r4 + (var_r5 * 8)))->unk40 = BtlBoss03_BlckBall_Abs_CreateTask(arg0);
            } else {
                ((Boss03*)(temp_r4 + (var_r5 * 8)))->unk44 = BtlBoss03_BlckBall_Abs_CreateTask(arg0);
            }
        } else {
            temp_r0_2 = temp_r4 + (var_r5 * 8);
            if (arg0 == 1) {
                ((Boss03*)temp_r0_2)->unk40 = -1;
            } else {
                ((Boss03*)temp_r0_2)->unk44 = -1;
            }
        }
        var_r5 += 1;
    } while (var_r5 < 4);
}

void func_ov017_0213f15c(s32 arg0) {
    s32   var_r6;
    void* temp_r0;
    void* temp_r0_2;
    void* temp_r5;

    temp_r5 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);
    var_r6  = 0;
    do {
        if (arg0 == 1) {
            temp_r0 = EasyTask_GetTaskById(&data_ov003_020e71b8->taskPool, ((Boss03*)(temp_r5 + (var_r6 * 8)))->unk40);
            if (temp_r0 != NULL) {
                ((Boss03*)temp_r0)->unk4 = (u16)(((Boss03*)temp_r0)->unk4 | 0x10);
            }
            ((Boss03*)(temp_r5 + (var_r6 * 8)))->unk40 = -1;
        } else {
            temp_r0_2 = EasyTask_GetTaskById(&data_ov003_020e71b8->unk_00000, ((Boss03*)(temp_r5 + (var_r6 * 8)))->unk44);
            if (temp_r0_2 != NULL) {
                ((Boss03*)temp_r0_2)->unk4 = (u16)(((Boss03*)temp_r0_2)->unk4 | 0x10);
            }
            ((Boss03*)(temp_r5 + (var_r6 * 8)))->unk44 = -1;
        }
        var_r6 += 1;
    } while (var_r6 < 4);
}

void func_ov017_0213f20c(u8 arg0) {
    void* temp_r0;

    temp_r0 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov017_02143a64);
    if (((u32)(((Boss03*)temp_r0)->unk3C << 0x1F) >> 0x1F) == arg0) {
        return;
    }
    ((Boss03*)temp_r0)->unk3A = 0;
    ((Boss03*)temp_r0)->unk3C = (u8)((((Boss03*)temp_r0)->unk3C & ~1) | (arg0 & 1));
    if (func_ov017_0213eeb8() == 0) {
        return;
    }
    func_ov003_020cb304(temp_r0 + 0x64, 0);
    func_ov003_020cb304(temp_r0 + 0x64, 1);
    func_ov017_0213f394(temp_r0);
    func_ov017_0213f4cc(temp_r0);
}

u32 func_ov017_0213f2a8(void* arg0) {
    void* var_r6_2;
    s32   var_ip;
    s32   var_lr;
    s32   var_r10;
    s32   var_r3;
    u16   var_r6;
    u16*  temp_r8;
    u32   var_r5;
    u32   var_r9;
    void* temp_r1;

    var_r5 = 0;
    var_r6 = 0;
    do {
        func_ov017_0213ece0((u32)var_r6);
        temp_r1 = arg0 + (var_r6 * 4);
        var_r6 += 1;
        ((Boss03*)temp_r1)->unk10 = func_ov003_020cb368();
    } while ((s32)var_r6 < 5);
    var_r3 = 0;
    do {
        if (var_r3 == 0) {
            var_r6_2 = &data_ov017_02143774;
        } else {
            var_r6_2 = &data_ov017_02143790;
        }
        var_ip = 0;
        if (7 > 0) {
            do {
                temp_r8 = *((u16**)((u8*)var_r6_2 + (var_ip * 4)));
                var_r9  = 0;
                var_r10 = 0;
            loop_8:
                var_lr = 0;
                if (*temp_r8 != 5) {
                loop_9:
                    if (var_r10 == *(temp_r8 + (var_lr * 2))) {
                        var_r9 += ((Boss03*)(arg0 + (var_r10 * 4)))->unk10;
                    } else {
                        var_lr += 1;
                        if (*(temp_r8 + (var_lr * 2)) != 5) {
                            goto loop_9;
                        }
                    }
                }
                var_r10 += 1;
                if (var_r10 < 5) {
                    goto loop_8;
                }
                var_ip += 1;
                if (var_r5 < var_r9) {
                    var_r5 = var_r9;
                }
            } while (var_ip < 7);
        }
        var_r3 += 1;
    } while (var_r3 < 2);
    return var_r5;
}

void func_ov017_0213f394(void* arg0) {
    void* var_r4;
    s32   var_r0;
    s32   var_r1;
    s32   var_r4_2;
    u16   temp_r1;
    u16   var_r2;
    u16*  temp_r3;
    void* temp_r0;
    void* temp_r2;

    if (((u32)(((Boss03*)arg0)->unk3C << 0x1F) >> 0x1F) == 0) {
        var_r4 = &data_ov017_02143774;
    } else {
        var_r4 = &data_ov017_02143790;
    }
    if (((Boss03*)arg0)->unk60 == 0xFFFF) {
        ((Boss03*)arg0)->unk60 = (u16)RNG_Next(7);
        ((Boss03*)arg0)->unk3A = (u16)(((Boss03*)arg0)->unk3A | (1 << ((Boss03*)arg0)->unk60));
    } else {
        if (func_02047e84(((Boss03*)arg0)->unk3A) == 7) {
            ((Boss03*)arg0)->unk3A = 0U;
        }
        var_r0 = RNG_Next(7 - func_02047e84(((Boss03*)arg0)->unk3A));
        var_r2 = 0;
        if (7 > 0) {
        loop_9:
            if (!(((Boss03*)arg0)->unk3A & (1 << var_r2))) {
                if (var_r0 == 0) {
                    ((Boss03*)arg0)->unk3A = (u16)(((Boss03*)arg0)->unk3A | (1 << var_r2));
                    ((Boss03*)arg0)->unk60 = var_r2;
                } else {
                    var_r0 = (s32)(u16)(var_r0 - 1);
                    goto block_13;
                }
            } else {
            block_13:
                var_r2 += 1;
                if ((s32)var_r2 < 7) {
                    goto loop_9;
                }
            }
        }
    }
    var_r1   = 0;
    temp_r3  = ((u16**)var_r4)[((Boss03*)arg0)->unk60];
    var_r4_2 = 0;
    do {
        temp_r0                   = arg0 + (var_r1 * 2);
        ((Boss03*)temp_r0)->unk24 = 0;
        var_r1 += 1;
        ((Boss03*)temp_r0)->unk2E = 0;
    } while (var_r1 < 5);
    if (*temp_r3 == 5) {
        return;
    }
    do {
        temp_r1 = *(temp_r3 + (var_r4_2 * 2));
        var_r4_2 += 1;
        temp_r2                   = arg0 + (temp_r1 * 2);
        ((Boss03*)temp_r2)->unk24 = (s16)(((Boss03*)temp_r2)->unk24 + 1);
    } while (*(temp_r3 + (var_r4_2 * 2)) != 5);
}

void func_ov017_0213f4cc(void* arg0) {
    void* var_r4;
    s32   var_r1;
    s32   var_r7;
    u16   var_r6;
    u16*  temp_r5;
    void* var_r8;

    var_r7 = 0;
    if (((u32)(((Boss03*)arg0)->unk3C << 0x1F) >> 0x1F) == 0) {
        var_r4 = &data_ov017_02143774;
    } else {
        var_r4 = &data_ov017_02143790;
    }
    func_ov003_020cb128(arg0 + 0x78, NULL, 0);
    func_ov003_020cb128(arg0 + 0x80, NULL, 0);
    var_r6  = 0;
    var_r8  = arg0 + 0x78;
    temp_r5 = ((u16**)var_r4)[((Boss03*)arg0)->unk60];
    do {
        var_r1 = 0;
        if (*temp_r5 != 5) {
        loop_5:
            if (var_r6 == *(temp_r5 + (var_r1 * 2))) {
                func_ov003_020cb128(var_r8, func_ov017_0213ece0((u32)var_r6), var_r7);
                var_r8 += 8;
                var_r7 += ((Boss03*)(arg0 + (var_r6 * 4)))->unk10;
            } else {
                var_r1 += 1;
                if (*(temp_r5 + (var_r1 * 2)) != 5) {
                    goto loop_5;
                }
            }
        }
        var_r6 += 1;
    } while ((s32)var_r6 < 5);
}

s16 func_ov017_0213f59c(void* arg0) {
    s16 temp_r1;
    s16 var_r3;
    s32 var_r2;

    var_r3 = 0;
    var_r2 = 0;
    do {
        temp_r1 = ((Boss03*)(arg0 + (var_r2 * 2)))->unk24;
        var_r2 += 1;
        var_r3 += temp_r1;
    } while (var_r2 < 5);
    return var_r3;
}

s16 func_ov017_0213f5cc(void* arg0) {
    s16 temp_r1;
    s16 var_r3;
    s32 var_r2;

    var_r3 = 0;
    var_r2 = 0;
    do {
        temp_r1 = ((Boss03*)(arg0 + (var_r2 * 2)))->unk2E;
        var_r2 += 1;
        var_r3 += temp_r1;
    } while (var_r2 < 5);
    return var_r3;
}

s32 func_ov017_0213f5fc(s32 arg0) {
    s32 sp0;

    sp0 = 1;
    if (func_ov003_020cb498(arg0, 0x3C, func_ov017_0213f630, &sp0) == 0) {
        sp0 = 0;
    }
    return sp0;
}

s32 func_ov017_0213f630(void* arg1, s32* arg2) {
    s32   temp_r1;
    void* temp_r0;

    if (*arg2 == 0) {
        return 0;
    }
    if ((s32)((Boss03*)arg1)->unk7C >= 0x20) {
        return 0;
    }
    temp_r0 = func_ov003_020c3c6c(arg1);
    temp_r1 = ((Boss03*)arg1)->unk7C;
    if ((temp_r1 != 3) && (temp_r1 != 0x10)) {
        goto block_11;
    }
    if ((((Boss03*)arg1)->unkC8 != ((Boss03*)temp_r0)->unk8) && (func_ov003_020c59a0(arg1) == 0) &&
        (((Boss03*)arg1)->unk5C != 4))
    {
        *arg2 = 0;
        return 0;
    }
block_11:
    if (((Boss03*)arg1)->unk18C & 0x10) {
        *arg2 = 0;
        return 0;
    }
    return 1;
}

s32 BtlBoss03_KindanMgr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    void* kindanMgr = task->data;

    switch (stage) {
        case 0:
            return BtlBoss03_KindanMgr_Init(kindanMgr, args);
        case 1:
            return BtlBoss03_KindanMgr_Update(kindanMgr);
        case 3:
            return BtlBoss03_KindanMgr_Destroy(kindanMgr);
    }
    return 1;
}

s32 BtlBoss03_KindanMgr_Init(void* arg0, s32) {
    s32   var_r2;
    u32   temp_r5;
    void* temp_r0;

    MI_CpuSet(arg0, 0, 0x88);
    var_r2                 = 0;
    ((Boss03*)arg0)->unk0  = 0;
    ((Boss03*)arg0)->unk38 = 0;
    ((Boss03*)arg0)->unk24 = 0;
    ((Boss03*)arg0)->unk26 = 0;
    ((Boss03*)arg0)->unk28 = 0;
    ((Boss03*)arg0)->unk2A = 0;
    ((Boss03*)arg0)->unk2C = 8;
    ((Boss03*)arg0)->unk2E = 0;
    ((Boss03*)arg0)->unk30 = 0;
    ((Boss03*)arg0)->unk32 = 0;
    ((Boss03*)arg0)->unk34 = 0;
    ((Boss03*)arg0)->unk36 = 0;
    do {
        temp_r0                   = arg0 + (var_r2 * 8);
        ((Boss03*)temp_r0)->unk40 = -1;
        var_r2 += 1;
        ((Boss03*)temp_r0)->unk44 = -1;
    } while (var_r2 < 4);
    ((Boss03*)arg0)->unk60 = 0xFFFF;
    func_ov017_0213f394(arg0);
    temp_r5 = func_ov017_0213f2a8(arg0);
    func_ov017_0213f4cc(arg0);
    func_ov003_020cb130(arg0 + 0x64, arg0 + 0x78, 2, temp_r5, &data_ov017_021437ac);
    func_ov003_020cb150(arg0 + 0x64);
    return 1;
}

s32 BtlBoss03_KindanMgr_Update(void* arg0) {
    s32 sp4;
    s32 sp0;
    void (*temp_r2)(s32, s32);
    void (*temp_r3)(s32, s32, s32);
    s16   temp_r0_4;
    s16   temp_r1;
    s16   temp_r4;
    s32   temp_r0;
    s32   temp_r0_2;
    s32   temp_r0_5;
    s32   temp_r4_2;
    s32   temp_r4_3;
    s32   temp_r4_4;
    s32   temp_r4_5;
    s32   temp_r5;
    s32   temp_r5_2;
    s32   temp_r5_3;
    s32   temp_r5_4;
    s32   var_r11;
    s32   var_r2;
    s32   var_r4;
    s32   var_r5;
    s32   var_r6;
    s32   var_r7;
    s32   var_r8;
    s32   var_r9;
    void* temp_r0_3;

    sp4     = 0;
    sp0     = 0;
    var_r11 = 0;
    var_r5  = 0;
    var_r6  = 0;
    var_r9  = 0;
    var_r7  = arg0 + 0x40;
    var_r8  = arg0 + 0x44;
    do {
        if ((((Boss03*)(arg0 + (var_r9 * 8)))->unk40 != -1) &&
            (EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, var_r7) == 0))
        {
            var_r5 += 1;
        }
        if (((Boss03*)(arg0 + (var_r9 * 8)))->unk40 != -1) {
            sp0 += 1;
        }
        if ((((Boss03*)(arg0 + (var_r9 * 8)))->unk44 != -1) &&
            (EasyTask_ValidateTaskId(&data_ov003_020e71b8->unk_00000, var_r8) == 0))
        {
            var_r6 += 1;
        }
        temp_r0 = ((Boss03*)(arg0 + (var_r9 * 8)))->unk44;
        var_r9 += 1;
        if (temp_r0 != -1) {
            var_r11 += 1;
        }
        var_r7 += 8;
        var_r8 += 8;
    } while (var_r9 < 4);
    if ((var_r5 == 0) && (var_r6 == 0)) {

    } else {
        temp_r3 = ((Boss03*)arg0)->unk8;
        if (temp_r3 != NULL) {
            temp_r3(((Boss03*)arg0)->unkC, var_r5, var_r6);
        }
    }
    temp_r4 = func_ov017_0213f5cc(arg0);
    if ((temp_r4 == func_ov017_0213f59c(arg0)) && ((sp0 + var_r11) == 0) && (func_ov003_020cb348(arg0 + 0x64, 0) <= 1) &&
        (func_ov003_020cb348(arg0 + 0x64, 1) <= 1))
    {
        func_ov003_020cb304(arg0 + 0x64, 0);
        func_ov003_020cb304(arg0 + 0x64, 1);
        func_ov017_0213f394(arg0);
        func_ov017_0213f4cc(arg0);
    }
    temp_r4_2 = func_ov003_020cb3c4(1, 3);
    temp_r4_3 = temp_r4_2 + func_ov003_020cb3c4(1, 8);
    temp_r4_4 = temp_r4_3 + func_ov003_020cb3c4(1, 9);
    temp_r4_5 = temp_r4_4 + func_ov003_020cb3c4(1, 0x10);
    var_r4    = temp_r4_5 + func_ov003_020cb3c4(1, 0x1F);
    temp_r5   = func_ov003_020cb3c4(0, 3);
    temp_r5_2 = temp_r5 + func_ov003_020cb3c4(0, 8);
    temp_r5_3 = temp_r5_2 + func_ov003_020cb3c4(0, 9);
    temp_r5_4 = temp_r5_3 + func_ov003_020cb3c4(0, 0x10);
    temp_r0_2 = temp_r5_4 + func_ov003_020cb3c4(0, 0x1F);
    if (var_r4 <= temp_r0_2) {
        var_r4 = temp_r0_2;
    }
    var_r2 = 0;
    do {
        temp_r0_3 = arg0 + (var_r2 * 2);
        temp_r1   = ((Boss03*)temp_r0_3)->unk24;
        temp_r0_4 = ((Boss03*)temp_r0_3)->unk2E;
        var_r2 += 1;
        temp_r0_5 = sp4 + (temp_r1 - temp_r0_4);
        sp4       = temp_r0_5;
    } while (var_r2 < 5);
    if ((temp_r0_5 > 0) && (var_r4 < 4)) {
        temp_r2 = ((Boss03*)arg0)->unk0;
        if (temp_r2 != NULL) {
            temp_r2(((Boss03*)arg0)->unk4, 4 - var_r4);
        }
    }
    return 1;
}

s32 BtlBoss03_KindanMgr_Destroy(s32 arg0) {
    func_ov003_020cb194(arg0 + 0x64);
    return 1;
}

/// MARK: BtlBoss03_NoisAppr

typedef struct {
    /* 0x00 */ s16                unk_0;
    /* 0x02 */ s16                unk_2;
    /* 0x04 */ Unk_From_NoisAppr* unk_4;
    /* 0x08 */ Unk_From_NoisAppr* unk_8;
} BtlBoss03_NoisAppr;

typedef struct {
    /* 0x0 */ s32   unk_0;
    /* 0x4 */ s32   unk_4;
    /* 0x8 */ void* unk_8;
    /* 0xC */ void* unk_C;
} Appr;

Appr data_ov017_021428f8[5] = {};

typedef struct {
    /* 0x00 */ u16   unk_0;
    /* 0x02 */ char  pad_2[2];
    /* 0x04 */ Vec*  unk_4;
    /* 0x08 */ Vec*  unk_8;
    /* 0x0C */ void* unk_C;
    /* 0x10 */ void* unk_10;
} BtlBoss03_NoisAppr_Args;

typedef struct {
    s32 unk_0;
    s16 unk_4;
    u16 unk_6;
    Vec unk_8;
} ApprBlob;

s32 BtlBoss03_NoisAppr_Init(BtlBoss03_NoisAppr* noisAppr, void*);
s32 BtlBoss03_NoisAppr_Update(BtlBoss03_NoisAppr* noisAppr);
s32 BtlBoss03_NoisAppr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle data_ov017_021428ec = {"Task_BtlBoss03_NoisAppr", BtlBoss03_NoisAppr_RunTask, 0x0C};

void BtlBoss03_NoisAppr_CreateTask(u16 arg0, s32 arg1, s32 arg2, void* arg3, void* arg4) {
    BtlBoss03_NoisAppr_Args args;
    args.unk_4  = arg1;
    args.unk_8  = arg2;
    args.unk_0  = arg0;
    args.unk_C  = arg3;
    args.unk_10 = arg4;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_021428ec, 0, 0, 0, &args);
}

s32 BtlBoss03_NoisAppr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlBoss03_NoisAppr* noisAppr = task->data;

    switch (stage) {
        case 0:
            return BtlBoss03_NoisAppr_Init(noisAppr, args);
        case 1:
            return BtlBoss03_NoisAppr_Update(noisAppr);
        default:
            return 1;
    }
}

s32 BtlBoss03_NoisAppr_Init(BtlBoss03_NoisAppr* noisAppr, void* arg1) {
    BtlBoss03_NoisAppr_Args* noisApprArgs = arg1;

    s32 sp34;
    s32 sp30;
    s32 sp2C;
    u16 sp2A;
    u16 sp28;
    s32 sp20;
    s32 sp1C;
    s32 sp18;
    u16 sp16;

    void* var_r4 = NULL;
    void* var_r5 = NULL;

    ApprBlob blob0;
    ApprBlob blob1;

    MI_CpuSet(noisAppr, 0, sizeof(BtlBoss03_NoisAppr));

    if (noisApprArgs->unk_4 != 0) {
        blob0.unk_0 = data_ov017_021428f8[noisApprArgs->unk_0].unk_0;
        blob0.unk_4 = data_ov017_021428f8[noisApprArgs->unk_0].unk_4;
        blob0.unk_6 &= ~1;
        var_r4        = data_ov017_021428f8[noisApprArgs->unk_0].unk_C;
        blob0.unk_8.x = noisApprArgs->unk_4->x;
        blob0.unk_8.y = noisApprArgs->unk_4->y;
        blob0.unk_8.z = noisApprArgs->unk_4->z;
    }

    if (noisApprArgs->unk_8 != 0) {
        blob1.unk_0 = data_ov017_021428f8[noisApprArgs->unk_0].unk_0;
        blob1.unk_4 = data_ov017_021428f8[noisApprArgs->unk_0].unk_4;
        var_r5      = data_ov017_021428f8[noisApprArgs->unk_0].unk_8;
        blob1.unk_6 &= ~1;
        blob1.unk_8.x = noisApprArgs->unk_8->x;
        blob1.unk_8.y = noisApprArgs->unk_8->y;
        blob1.unk_8.z = noisApprArgs->unk_8->z;
    }

    s32 spC, sp8;
    func_ov003_020c3d28(var_r4, var_r5, &blob0, &blob1, &spC, &sp8);

    noisAppr->unk_4 = NULL;
    noisAppr->unk_8 = NULL;

    if (noisApprArgs->unk_4 != NULL) {
        noisAppr->unk_4 = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, spC);
        ((Boss03*)noisAppr->unk_4)->unk54 |= 0x20;
        ((Boss03*)noisAppr->unk_4)->unk54 |= 0x10;
    }

    if (noisApprArgs->unk_8 != NULL) {
        noisAppr->unk_8 = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_00000, sp8);
        ((Boss03*)noisAppr->unk_8)->unk54 |= 0x20;
        ((Boss03*)noisAppr->unk_8)->unk54 |= 0x10;
    }

    noisApprArgs->unk_C  = spC;
    noisApprArgs->unk_10 = sp8;
    return 1;
}

s32 BtlBoss03_NoisAppr_Update(BtlBoss03_NoisAppr* noisAppr) {
    if ((noisAppr->unk_4 != NULL) && (((Boss03*)noisAppr->unk_4)->unk54 & 4)) {
        noisAppr->unk_4 = NULL;
    }

    if ((noisAppr->unk_8 != NULL) && (((Boss03*)noisAppr->unk_8)->unk54 & 4)) {
        noisAppr->unk_8 = NULL;
    }

    if ((noisAppr->unk_4 == NULL) && (noisAppr->unk_8 == NULL)) {
        return 0;
    }

    if (func_ov003_020c3c28() != 0) {
        return 1;
    }

    switch (noisAppr->unk_2) {
        case 0: {
            if ((noisAppr->unk_4 != NULL) && (noisAppr->unk_4->unk_1A8 == 0) && (noisAppr->unk_4->unk_1A4 == 1)) {
                noisAppr->unk_4->unk_1A6 = 1;
            }

            if ((noisAppr->unk_8 != NULL) && (noisAppr->unk_8->unk_1A8 == 0) && (noisAppr->unk_8->unk_1A4 == 1)) {
                noisAppr->unk_8->unk_1A6 = 1;
            }

            if (((noisAppr->unk_4 != NULL) && (noisAppr->unk_4->unk_1A8 == 3)) ||
                ((noisAppr->unk_8 != NULL) && (noisAppr->unk_8->unk_1A8 == 3)))
            {
                noisAppr->unk_0 = 0;
                noisAppr->unk_2++;
            }
            noisAppr->unk_0++;
        } break;

        case 1: {
            if (noisAppr->unk_0 == 0xA) {
                if (noisAppr->unk_4 != NULL) {
                    ((Boss03*)noisAppr->unk_4)->unk54 &= ~0x20;
                    ((Boss03*)noisAppr->unk_4)->unk54 &= ~0x10;
                }

                if (noisAppr->unk_8 != NULL) {
                    ((Boss03*)noisAppr->unk_8)->unk54 &= ~0x20;
                    ((Boss03*)noisAppr->unk_8)->unk54 &= ~0x10;
                }
                return 0;
            }
            noisAppr->unk_0++;
        } break;
    }

    return 1;
}

/// MARK: BtlBoss03_NoisAttk

s32 BtlBoss03_NoisAttk_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Task_BtlBoss03_NoisAttk = {"Task_BtlBoss03_NoisAttk", BtlBoss03_NoisAttk_RunTask, 0x10C};

void BtlBoss03_NoisAttk_CreateTask(s32* arg0) {
    TaskPool* taskPool;

    if (func_ov003_020c37f8(arg0 + 0x84) == 0) {
        taskPool = &data_ov003_020e71b8->unk_00000;
    } else {
        taskPool = &data_ov003_020e71b8->taskPool;
    }
    EasyTask_CreateTask(taskPool, &Task_BtlBoss03_NoisAttk, 0, 0, 0, arg0);
}

s16 func_ov017_0213fec0(void* arg0, s32 arg1) {
    return func_ov003_020cb910(arg0 + 0xF8, arg0 + 0xFC, arg0 + 0x100, ((Boss03*)arg0)->unk88, ((Boss03*)arg0)->unk8C,
                               ((Boss03*)arg0)->unk90, ((Boss03*)arg0)->unkEC, ((Boss03*)arg0)->unkF0, ((Boss03*)arg0)->unkF4,
                               arg1);
}

s32 BtlBoss03_NoisAttk_Init(TaskPool* pool, Task* task, void* args) {
    void* temp_r4;

    temp_r4 = ((Boss03*)args)->unk18;
    MI_CpuSet(temp_r4, 0, 0x10C);
    func_ov017_02140b14((u32)(((Boss03*)args)->unk84 << 0x1E) >> 0x1E, temp_r4, 0, 0);
    func_ov003_02083118(temp_r4 + 0x60, 2);
    ((Boss03*)temp_r4)->unkD0 = (s16)((Boss03*)args)->unk70;
    ((Boss03*)temp_r4)->unkD2 = (s16)((Boss03*)args)->unk72;
    ((Boss03*)temp_r4)->unkDC = args;
    ((Boss03*)temp_r4)->unkE8 = 1;
}

s32 BtlBoss03_NoisAttk_Update(void* arg1) {
    s32   var_r0_2;
    s16   temp_ip;
    s16   temp_lr;
    s16   temp_r0;
    s16   temp_r0_2;
    s16   temp_r0_5;
    s16   temp_r1_7;
    s16   temp_r1_8;
    s16   temp_r2_3;
    s16   temp_r2_4;
    s32   temp_r1;
    s32   temp_r2_2;
    s32   var_r0;
    u16   var_r0_3;
    u32   temp_r1_6;
    u32   temp_r3_2;
    u32   temp_ret;
    void* temp_r0_3;
    void* temp_r0_4;
    void* temp_r1_2;
    void* temp_r1_3;
    void* temp_r1_4;
    void* temp_r1_5;
    void* temp_r2;
    void* temp_r3;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    temp_r2 = ((Boss03*)temp_r4)->unkDC;
    temp_r1 = ((Boss03*)temp_r2)->unk54;
    if (temp_r1 & 4) {
        ((Boss03*)temp_r2)->unk28                   = (s32)((Boss03*)temp_r4)->unk88;
        ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk2C = (s32)((Boss03*)temp_r4)->unk8C;
        ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk30 = (s32)((Boss03*)temp_r4)->unk90;
        return 0;
    }
    temp_r0 = ((Boss03*)temp_r4)->unkE4;
    switch (temp_r0) {
        default:
            break;
        case 0:
            temp_r0_2 = ((Boss03*)temp_r4)->unkE0;
            if (temp_r0_2 == 0) {
                ((Boss03*)temp_r2)->unk54    = (s32)(temp_r1 | 0x10);
                temp_r1_2                    = ((Boss03*)temp_r4)->unkDC;
                ((Boss03*)temp_r1_2)->unk1AC = (s32)((Boss03*)temp_r1_2)->unk28;
                temp_r1_3                    = ((Boss03*)temp_r4)->unkDC;
                ((Boss03*)temp_r1_3)->unk1B0 = (s32)((Boss03*)temp_r1_3)->unk2C;
                temp_r1_4                    = ((Boss03*)temp_r4)->unkDC;
                ((Boss03*)temp_r1_4)->unk1B4 = (s32)((Boss03*)temp_r1_4)->unk30;
            } else if (temp_r0_2 == 1) {
                ((Boss03*)temp_r2)->unk54 = (s32)(temp_r1 & ~0x10);
                func_ov003_020824a0(temp_r4, 3U, 1);
                temp_r0_3                    = ((Boss03*)temp_r4)->unkDC;
                ((Boss03*)temp_r0_3)->unk18C = (u16)(((Boss03*)temp_r0_3)->unk18C | 1);
                temp_r3                      = ((Boss03*)temp_r4)->unkDC;
                ((Boss03*)temp_r3)->unk54    = (s32)(((Boss03*)temp_r3)->unk54 | 0x10);
                ((Boss03*)temp_r4)->unk104   = 1;
                ((Boss03*)temp_r4)->unk88    = (s32)((Boss03*)((Boss03*)temp_r4)->unkDC)->unk28;
                ((Boss03*)temp_r4)->unk8C    = (s32)((Boss03*)((Boss03*)temp_r4)->unkDC)->unk2C;
                ((Boss03*)temp_r4)->unk90    = 0x8000;
                if (func_ov003_020c37f8(((Boss03*)temp_r4)->unkDC + 0x84) == 0) {
                    temp_r1_5                   = ((Boss03*)temp_r4)->unkDC;
                    ((Boss03*)temp_r1_5)->unk54 = (s32)(((Boss03*)temp_r1_5)->unk54 | 0x10000000);
                }
                func_ov003_020c4a08(((Boss03*)temp_r4)->unkDC);
            }
            if (SpriteMgr_IsAnimationFinished(temp_r4) != 0) {
                ((Boss03*)temp_r4)->unkE0 = 0;
                ((Boss03*)temp_r4)->unkE4 = 1;
            } else {
                ((Boss03*)temp_r4)->unkE0 = (s16)(((Boss03*)temp_r4)->unkE0 + 1);
            }
            break;
        case 1:
            if (((Boss03*)temp_r4)->unkE0 == 0) {
                temp_ret = RNG_Next(0x64);
                func_ov003_020824a0(temp_r4, (u16)((temp_ret >> 0x1F) + (temp_ret / 50) + 4), 0);
                ((Boss03*)temp_r4)->unkE2                    = 0x1E;
                ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk28  = -0x80000;
                ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk1AC = -0x80000;
            }
            temp_lr   = ((Boss03*)temp_r4)->unkE0;
            temp_r2_2 = temp_lr - (0xF * (((u32)temp_lr >> 0x1F) + ((s32)(temp_lr + MULT_HI(0x88888889, temp_lr)) >> 3)));
            temp_r3_2 = temp_r2_2 << 0x10;
            temp_r2_3 = *(
                &data_0205e4e0 +
                (((s32)(u16)((temp_r3_2 >> 0x1F) + ((s32)(MULT_HI(0x88888889, temp_r3_2) + (temp_r2_2 << 0x10)) >> 3)) >> 4) *
                 4));
            temp_r1_6 = (temp_r2_3 << 0xF) + 0x800;
            ((Boss03*)temp_r4)->unk90 =
                (s32)(((Boss03*)((Boss03*)temp_r4)->unkDC)->unk30 +
                      ((temp_r1_6 >> 0xC) |
                       (((((temp_r2_3 >> 0x1F) << 0xF) | ((u32)temp_r2_3 >> 0x11)) + M2C_CARRY(temp_r1_6)) << 0x14)));
            temp_r2_4 = ((Boss03*)temp_r4)->unkE0;
            if ((s32)temp_r2_4 < (s32)((Boss03*)temp_r4)->unkE2) {
                ((Boss03*)temp_r4)->unkE0 = (s16)(temp_r2_4 + 1);
            } else {
                ((Boss03*)temp_r4)->unkE0 = 0;
                ((Boss03*)temp_r4)->unkE4 = 2;
            }
            break;
        case 2:
            if (((Boss03*)temp_r4)->unkE0 == 0) {
                ((Boss03*)temp_r4)->unkE2 = 0x1E;
            }
            temp_r1_7 = ((Boss03*)temp_r4)->unkE0;
            if ((s32)temp_r1_7 < (s32)((Boss03*)temp_r4)->unkE2) {
                ((Boss03*)temp_r4)->unkE0 = (s16)(temp_r1_7 + 1);
            } else {
                ((Boss03*)temp_r4)->unkE0 = 0;
                ((Boss03*)temp_r4)->unkE4 = 3;
            }
            break;
        case 3:
            if (((Boss03*)temp_r4)->unkE0 == 0) {
                if (SndMgr_IsSEPlaying(0x2E0) == 0) {
                    func_ov003_02087ed8(0x2E0);
                }
                if (func_ov003_020cb55c(((Boss03*)temp_r4)->unkDC) == 1) {
                    var_r0 = 1;
                } else {
                    var_r0 = 0;
                }
                if (var_r0 != 0) {
                    var_r0_2 = 1;
                } else {
                    var_r0_2 = 0;
                }
                temp_r0_4                 = func_ov003_020cc750(var_r0_2);
                ((Boss03*)temp_r4)->unkEC = (s32)((((Boss03*)temp_r0_4)->unk28 * 2) - ((Boss03*)temp_r4)->unk88);
                ((Boss03*)temp_r4)->unkF0 = (s32)((((Boss03*)temp_r0_4)->unk2C * 2) - ((Boss03*)temp_r4)->unk8C);
                ((Boss03*)temp_r4)->unkF4 = -0x8000;
                ((Boss03*)temp_r4)->unkE2 = func_ov017_0213fec0(temp_r4, 0x4000);
            }
            ((Boss03*)temp_r4)->unk88 = (s32)(((Boss03*)temp_r4)->unk88 + ((Boss03*)temp_r4)->unkF8);
            ((Boss03*)temp_r4)->unk8C = (s32)(((Boss03*)temp_r4)->unk8C + ((Boss03*)temp_r4)->unkFC);
            ((Boss03*)temp_r4)->unk90 = (s32)(((Boss03*)temp_r4)->unk90 + ((Boss03*)temp_r4)->unk100);
            if (func_ov003_020c37f8(temp_r4) != 0) {
                var_r0_3 = 0xFC;
            } else {
                var_r0_3 = 0xF6;
            }
            func_ov003_020c5b2c(var_r0_3, ((Boss03*)temp_r4)->unkDC, ((Boss03*)temp_r4)->unk88, ((Boss03*)temp_r4)->unk8C);
            temp_r1_8 = ((Boss03*)temp_r4)->unkE0;
            if ((s32)temp_r1_8 < (s32)((Boss03*)temp_r4)->unkE2) {
                ((Boss03*)temp_r4)->unkE0 = (s16)(temp_r1_8 + 1);
            } else {
                ((Boss03*)temp_r4)->unkE0 = 0;
                ((Boss03*)temp_r4)->unkE4 = 4;
            }
            break;
        case 4:
            temp_r0_5 = ((Boss03*)temp_r4)->unkE0;
            if (temp_r0_5 == 0) {
                ((Boss03*)temp_r4)->unkE0 = (s16)(temp_r0_5 + 1);
                func_ov003_020824a0(temp_r4, 9U, 1);
                if (SndMgr_IsSEPlaying(0x2E1) == 0) {
                    func_ov003_02087ed8(0x2E1);
                }
            }
            if (SpriteMgr_IsAnimationFinished(temp_r4) != 0) {
                ((Boss03*)temp_r4)->unk104 = 0;
                ((Boss03*)temp_r4)->unkE8  = 0;
            }
            break;
    }
    func_ov003_02082f1c(((Boss03*)temp_r4)->unkDC, 6);
    temp_ip = ((Boss03*)temp_r4)->unkD0;
    if (func_ov003_02083ca0(temp_r4 + 0x88, temp_r4 + 0x8C, temp_ip << 0xC,
                            ((s32)(temp_ip + ((u32)temp_ip >> 0x1F)) >> 1) << 0xC) != 0)
    {
        ((Boss03*)temp_r4)->unkF8  = 0;
        ((Boss03*)temp_r4)->unkFC  = 0;
        ((Boss03*)temp_r4)->unk100 = 0;
    }
    func_ov003_02082b0c(temp_r4);
    return ((Boss03*)temp_r4)->unkE8;
}

s32 BtlBoss03_NoisAttk_Render(void* arg1) {
    s16   spA;
    s16   sp8;
    s32   var_r0;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    if (((Boss03*)temp_r4)->unk104 != 0) {
        if (((u32)(((Boss03*)temp_r4)->unk0 << 0x1E) >> 0x1E) == 1) {
            var_r0 = 1;
        } else {
            var_r0 = 0;
        }
        func_ov003_02084348(var_r0, &spA, &sp8, ((Boss03*)temp_r4)->unk88);
        func_ov003_02082724(temp_r4, spA, sp8);
        func_ov003_02082730(temp_r4, 0x7FFFFFFF - ((Boss03*)temp_r4)->unk8C);
        func_ov003_02082b64(temp_r4);
    }
    return 1;
}

s32 BtlBoss03_NoisAttk_Destroy(void* arg1) {
    void* temp_r0;
    void* temp_r1;
    void* temp_r1_2;
    void* temp_r1_3;
    void* temp_r1_4;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r1 = ((Boss03*)temp_r4)->unkDC;
    if (!(((Boss03*)temp_r1)->unk54 & 4)) {
        ((Boss03*)temp_r1)->unk28                    = (s32)((Boss03*)temp_r4)->unk88;
        ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk2C  = (s32)((Boss03*)temp_r4)->unk8C;
        ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk30  = 0;
        ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk1AC = (s32)((Boss03*)temp_r4)->unk88;
        ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk1B0 = (s32)((Boss03*)temp_r4)->unk8C;
        ((Boss03*)((Boss03*)temp_r4)->unkDC)->unk1B4 = 0;
        temp_r0                                      = ((Boss03*)temp_r4)->unkDC;
        ((Boss03*)temp_r0)->unk18C                   = (u16)(((Boss03*)temp_r0)->unk18C & ~1);
        if (((u32)(((Boss03*)temp_r4)->unk108 << 0x1F) >> 0x1F) == 0) {
            temp_r1_2                   = ((Boss03*)temp_r4)->unkDC;
            ((Boss03*)temp_r1_2)->unk54 = (s32)(((Boss03*)temp_r1_2)->unk54 & ~0x10);
        }
        if (func_ov003_020c37f8(((Boss03*)temp_r4)->unkDC + 0x84) == 0) {
            temp_r1_3                   = ((Boss03*)temp_r4)->unkDC;
            ((Boss03*)temp_r1_3)->unk54 = (s32)(((Boss03*)temp_r1_3)->unk54 & ~0x10000000);
        }
        func_ov003_020c495c(((Boss03*)temp_r4)->unkDC);
        if (((Boss03*)((Boss03*)temp_r4)->unkDC)->unk5C == 4) {
            func_ov003_02082fc0();
        }
    }
    temp_r1_4                    = ((Boss03*)temp_r4)->unkDC;
    ((Boss03*)temp_r1_4)->unk18C = (u16)(((Boss03*)temp_r1_4)->unk18C & ~1);
    func_ov003_02082cc4(temp_r4);
    return 1;
}

s32 BtlBoss03_NoisAttk_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov017_02142954 = {
        .initialize = BtlBoss03_NoisAttk_Init,
        .update     = BtlBoss03_NoisAttk_Update,
        .render     = BtlBoss03_NoisAttk_Render,
        .cleanup    = BtlBoss03_NoisAttk_Destroy,
    };
    return data_ov017_02142954.iter[stage](pool, task, args);
}

/// MARK: BtlBoss03_SeqEntry

typedef struct {
    /* 0x00 */ void* unk_00;
    /* 0x04 */ void (*unk_04)();
    /* 0x08 */ void (*unk_08)();
    /* 0x0C */ u16 unk_0C;
} BtlBoss03_SeqEntry_Args;

s32 BtlBoss03_SeqEntry_RunTask(TaskPool* pool, Task* task, void* args, s32 arg3);

const TaskHandle Task_BtlBoss03_SeqEntry = {"Task_BtlBoss03_SeqEntry", BtlBoss03_SeqEntry_RunTask, 0x2C};

void BtlBoss03_SeqEntry_CreateTask(void* arg0) {
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &Task_BtlBoss03_SeqEntry, 0, 0, 0, arg0);
}

s32 BtlBoss03_SeqEntry_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    s32   spC;
    s32   sp8;
    s32   temp_r3;
    s32   var_r0;
    u16   temp_r2;
    void* temp_r0_3;
    void* temp_r0_4;
    void* temp_r4;
    void* var_r0_2;
    void* var_r1;

    void*                    temp_r5 = task->data;
    BtlBoss03_SeqEntry_Args* seqArgs = (BtlBoss03_SeqEntry_Args*)args;
    s32                      var_r6  = 1;

    switch (stage) {
        case 0:
            temp_r4                  = seqArgs->unk_00;
            ((Boss03*)temp_r5)->unk0 = seqArgs->unk_0C;
            if (func_ov003_020cb55c(temp_r4) == 1) {
                var_r0 = 1;
            } else {
                var_r0 = 0;
            }
            temp_r3 = ((Boss03*)temp_r5)->unk0 + 0x2B;
            if (var_r0 != 0) {
                var_r1   = temp_r4;
                var_r0_2 = ((Boss03*)temp_r4)->unk184;
            } else {
                var_r1   = ((Boss03*)temp_r4)->unk184;
                var_r0_2 = temp_r4;
            }
            ((Boss03*)temp_r5)->unk18 = temp_r3;
            ((Boss03*)temp_r5)->unk4  = temp_r3;
            ((Boss03*)temp_r5)->unk1C = 0;
            ((Boss03*)temp_r5)->unk8  = 0;
            ((Boss03*)temp_r5)->unkA  = (u16)((((Boss03*)temp_r5)->unkA & ~1) | 1);
            ((Boss03*)temp_r5)->unkC  = (s32)((Boss03*)var_r0_2)->unk28;
            ((Boss03*)temp_r5)->unk10 = (s32)((Boss03*)var_r0_2)->unk2C;
            ((Boss03*)temp_r5)->unk14 = (s32)((Boss03*)var_r0_2)->unk30;
            ((Boss03*)temp_r5)->unk1E = (u16)((((Boss03*)temp_r5)->unk1E & ~1) | 1);
            ((Boss03*)temp_r5)->unk20 = (s32)((Boss03*)var_r1)->unk28;
            ((Boss03*)temp_r5)->unk24 = (s32)((Boss03*)var_r1)->unk2C;
            ((Boss03*)temp_r5)->unk28 = (s32)((Boss03*)var_r1)->unk30;

            if (seqArgs->unk_04 != NULL) {
                seqArgs->unk_04();
            }
            if (seqArgs->unk_08 != NULL) {
                seqArgs->unk_08();
            }

            func_ov003_02082cc4(temp_r4 + 0x84);
            func_ov003_02082cc4(((Boss03*)temp_r4)->unk184 + 0x84);
            temp_r2 = ((Boss03*)temp_r5)->unk0;
            func_ov003_020c3d28(*(s32*)((char*)&data_ov017_02143814 + (temp_r2 * 8)),
                                *(s32*)((char*)&data_ov017_02143810 + (temp_r2 * 8)), temp_r5 + 0x18, temp_r5 + 4, &spC, &sp8);
            temp_r0_3                   = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, spC);
            ((Boss03*)temp_r0_3)->unk54 = (s32)(((Boss03*)temp_r0_3)->unk54 | 0x08000000);
            temp_r0_4                   = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_00000, sp8);
            ((Boss03*)temp_r0_4)->unk54 = (s32)(((Boss03*)temp_r0_4)->unk54 | 0x08000000);
            break;

        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 1;
            }
            var_r6 = 0;
            break;
    }
    return var_r6;
}

/// MARK: BtlBoss03_ShotEff

typedef struct {
    /* 0x00 */ char   unk_00[0x4];
    /* 0x04 */ Sprite unk_04;
} BtlBoss03_ShotEff; // Size: 0x64

s32 BtlBoss03_ShotEff_Init(BtlBoss03_ShotEff* shotEff, void**);
s32 BtlBoss03_ShotEff_Update(BtlBoss03_ShotEff* shotEff);
s32 BtlBoss03_ShotEff_Render(BtlBoss03_ShotEff* shotEff);
s32 BtlBoss03_ShotEff_Destroy(s32);
s32 BtlBoss03_ShotEff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_ShotEff = {"Tsk_BtlBoss03_ShotEff", BtlBoss03_ShotEff_RunTask};

void BtlBoss03_ShotEff_CreateTask(s32 arg0) {
    s32 sp8;

    func_ov003_020c37f8(arg0 + 0x84);
    sp8 = arg0;
    EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_BtlBoss03_ShotEff, 0, 0, 0, &sp8);
}

s32 BtlBoss03_ShotEff_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlBoss03_ShotEff* shotEff = task->data;

    switch (stage) {
        case 0:
            return BtlBoss03_ShotEff_Init(shotEff, args);
        case 1:
            return BtlBoss03_ShotEff_Update(shotEff);
        case 2:
            return BtlBoss03_ShotEff_Render(shotEff);
        case 3:
            return BtlBoss03_ShotEff_Destroy(shotEff);
    }
    return 1;
}

s32 BtlBoss03_ShotEff_Init(BtlBoss03_ShotEff* shotEff, void** arg1) {
    s32   var_r1;
    void* temp_r0;

    MI_CpuSet(shotEff, 0, 0x64);
    temp_r0          = *arg1;
    *(void**)shotEff = temp_r0;
    if (((Boss03*)temp_r0)->unk80 == 0) {
        func_ov017_02140b14((u32)(((Boss03*)*arg1)->unk84 << 0x1E) >> 0x1E, &shotEff->unk_04, 1, 0);
        func_ov003_020824a0(&shotEff->unk_04, 3, 1);
    } else {
        func_ov003_02082a04((u32)(((Boss03*)*arg1)->unk84 << 0x1E) >> 0x1E, &shotEff->unk_04, &data_ov017_02142058,
                            &data_ov017_02142970, 0, 0x12, 0x40);
        func_ov003_020824a0(&shotEff->unk_04, 0, 1);
    }
    if (((Boss03*)*arg1)->unk24 == 1) {
        var_r1 = 1;
    } else {
        var_r1 = 0;
    }
    func_ov003_02082750(&shotEff->unk_04, var_r1);
    return 1;
}

s32 BtlBoss03_ShotEff_Update(BtlBoss03_ShotEff* shotEff) {
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }

    func_ov003_02082b0c(&shotEff->unk_04);
    return SpriteMgr_IsAnimationFinished(&shotEff->unk_04) == FALSE;
}

s32 BtlBoss03_ShotEff_Render(BtlBoss03_ShotEff* shotEff) {
    s16    spA;
    s16    sp8;
    s32    var_r0;
    void** temp_r1;
    void*  temp_ip;

    temp_r1 = (void**)shotEff;
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    temp_ip = *temp_r1;
    if (func_ov003_020c37f8(&shotEff->unk_04) != 0) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    func_ov003_02084348(var_r0, &spA, &sp8, ((Boss03*)temp_ip)->unk28, ((Boss03*)temp_ip)->unk30);
    if (((Boss03*)*temp_r1)->unk24 == 0) {
        func_ov003_02082724(&shotEff->unk_04, (s16)(spA - 0x20), (s16)(sp8 - 0x30));
    } else {
        func_ov003_02082724(&shotEff->unk_04, (s16)(spA + 0x20), (s16)(sp8 - 0x30));
    }
    func_ov003_02082730(&shotEff->unk_04, 0x7FFFFFFF - ((Boss03*)*temp_r1)->unk2C);
    func_ov003_02082b64(&shotEff->unk_04);
    return 1;
}

s32 BtlBoss03_ShotEff_Destroy(s32 arg0) {
    func_ov003_02082cc4(arg0 + 4);
    return 1;
}

void func_ov017_02140a8c(s32 arg0, void* arg1, s32 arg2, s32 arg3) {
    void*  temp_r4;
    void** var_r0;

    if (arg3 != 0) {
        var_r0 = (void**)&data_ov017_021438a8;
    } else {
        var_r0 = (void**)&data_ov017_02143870;
    }
    temp_r4 = var_r0[arg2];
    func_ov017_0213e134(temp_r4, arg3);
    func_ov003_02082a04(arg0, arg1, temp_r4, data_ov017_02143918[arg2], 0, (s32)data_ov017_02142a14[arg2],
                        (s32)data_ov017_02142a68[arg2]);
}

void func_ov017_02140b14(u32 arg0, Sprite* arg1, s32 arg2, s32 arg3) {
    void*  temp_r4;
    void** var_r0;

    if (arg3 != 0) {
        var_r0 = (void**)&data_ov017_02143848;
    } else {
        var_r0 = (void**)&data_ov017_0214385c;
    }
    temp_r4 = var_r0[arg2];
    func_ov017_0213e134(temp_r4, arg3);
    func_ov003_02082a04(arg0, arg1, temp_r4, data_ov017_021438e0[arg2], 0, (s32)data_ov017_02142a30[arg2],
                        (s32)data_ov017_02142a4c[arg2]);
}

/// MARK: BtlBoss03_ThrwBdge

typedef struct {
    /* 0x000 */ s32    unk_000;
    /* 0x004 */ Sprite unk_004;
    /* 0x044 */ char   unk_044[0x48 - 0x44];
    /* 0x048 */ s16    unk_048;
    /* 0x04A */ char   unk_04A[0x64 - 0x4A];
    /* 0x064 */ s32    unk_064;
    /* 0x068 */ char   unk_068[0x8C - 0x68];
    /* 0x08C */ s32    unk_08C;
    /* 0x090 */ s32    unk_090;
    /* 0x094 */ s32    unk_094;
    /* 0x098 */ char   unk_098[0x4];
    /* 0x09C */ s32    unk_09C;
    /* 0x0A0 */ s32    unk_0A0;
    /* 0x0A4 */ s32    unk_0A4;
    /* 0x0A8 */ s32    unk_0A8;
    /* 0x0AC */ s32    unk_0AC;
    /* 0x0B0 */ s32    unk_0B0;
    /* 0x0B4 */ s32    unk_0B4;
    /* 0x0B8 */ char   unk_0B8[0xD0 - 0xB8];
    /* 0x0D0 */ s16    unk_0D0;
    /* 0x0D2 */ char   unk_0D2[2];
    /* 0x0D4 */ s16    unk_0D4;
    /* 0x0D6 */ s16    unk_0D6;
    /* 0x0D8 */ char   unk_0D8[0xE0 - 0xD8];
    /* 0x0E0 */ s16    unk_0E0;
    /* 0x0E2 */ s16    unk_0E2;
    /* 0x0E4 */ s16    unk_0E4;
    /* 0x0E6 */ char   unk_0E6[2];
    /* 0x0E8 */ s32    unk_0E8;
    /* 0x0EC */ s32    unk_0EC;
    /* 0x0F0 */ s32    unk_0F0;
    /* 0x0F4 */ s32    unk_0F4;
    /* 0x0F8 */ s32    unk_0F8;
    /* 0x0FC */ s32    unk_0FC;
    /* 0x100 */ u16    unk_100;
    /* 0x102 */ char   unk_102[2];
    /* 0x104 */ s32    unk_104;
    /* 0x108 */ char   unk_108[0x118 - 0x108];
    /* 0x118 */ s32    unk_118;
} BtlBoss03_ThrwBdge; // Size: 0x11C

typedef struct {
    /* 0x00 */ s32  unk_00;
    /* 0x04 */ u16  unk_04;
    /* 0x06 */ char unk_06[2];
    /* 0x08 */ s32  unk_08;
    /* 0x0C */ s32  unk_0C;
    /* 0x10 */ s32  unk_10;
    /* 0x14 */ s16  unk_14;
    /* 0x16 */ char unk_16[2];
    /* 0x18 */ s32  unk_18;
    /* 0x1C */ s32  unk_1C;
} BtlBoss03_ThrwBdge_Args;

void func_ov017_02140d44(s32 arg0, void* arg1);
s32  BtlBoss03_ThrwBdge_Init(BtlBoss03_ThrwBdge* thrwBdge, BtlBoss03_ThrwBdge_Args* args);
s32  BtlBoss03_ThrwBdge_Update(BtlBoss03_ThrwBdge* thrwBdge);
s32  BtlBoss03_ThrwBdge_Render(BtlBoss03_ThrwBdge* thrwBdge);
s32  BtlBoss03_ThrwBdge_Destroy(BtlBoss03_ThrwBdge* thrwBdge);
s32  BtlBoss03_ThrwBdge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_BtlBoss03_ThrwBdge = {"Tsk_BtlBoss03_ThrwBdge", BtlBoss03_ThrwBdge_RunTask, 0x011C};

void func_ov017_02140b9c(void* arg0, s16 arg1) {
    BtlBoss03_ThrwBdge_Args args;
    void*                   temp_r5;

    func_ov003_020c37f8(arg0 + 0x84);
    args.unk_04 = arg1;
    args.unk_00 = arg0;
    temp_r5     = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x8C));
    args.unk_08 = RNG_Next(0x2001) - 0x1000;
    args.unk_0C = 0;
    args.unk_10 = RNG_Next(0x1001) - 0x5000;
    args.unk_14 = 15;
    func_ov017_02140d44(func_ov003_020cb55c(arg0), &args.unk_18);
    EasyTask_CreateTask(temp_r5 + 0x8000, &Tsk_BtlBoss03_ThrwBdge, 0, 0, 0, &args);
}

s32 func_ov017_02140c40(void* arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4) {
    BtlBoss03_ThrwBdge_Args args;

    func_ov003_020c37f8(arg0 + 0x84);
    args.unk_00 = arg0;
    args.unk_04 = 2;
    args.unk_08 = arg1;
    args.unk_0C = arg2;
    args.unk_10 = arg3;
    args.unk_14 = arg4;
    EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_BtlBoss03_ThrwBdge, 0, 0, 0, &args);
}

void func_ov017_02140cbc(void* arg0) {
    BtlBoss03_ThrwBdge_Args args;
    args.unk_00 = arg0;
    args.unk_04 = 3;
    args.unk_08 = RNG_Next(0x2001) - 0x1000;
    args.unk_0C = 0;
    args.unk_10 = RNG_Next(0x1001) - 0x5000;
    args.unk_14 = 30;
    EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &Tsk_BtlBoss03_ThrwBdge, 0, 0, 0, &args);
}

void func_ov017_02140d44(s32 arg0, void* arg1) {
    s32 temp_r5;

    if (arg0 == 1) {
        temp_r5               = RNG_Next(65) - 32;
        ((Boss03*)arg1)->unk0 = (func_ov003_020cb764(1) + (temp_r5 << 0xC));
        ((Boss03*)arg1)->unk0 = (RNG_Next(100) < 50) ? ((Boss03*)arg1)->unk0 - 0x40000 : ((Boss03*)arg1)->unk0 + 0x40000;
        ((Boss03*)arg1)->unk4 = (RNG_Next((func_ov003_020cb7a4(1) >> 0xC) + 1) << 0xC);
        ((Boss03*)arg1)->unk8 = 0;
        ((Boss03*)arg1)->unkC = 1;
    } else {
        ((Boss03*)arg1)->unk0 = (RNG_Next((func_ov003_020cb744(0) >> 0xC) + 1) << 0xC);
        ((Boss03*)arg1)->unk4 = (RNG_Next((func_ov003_020cb7a4(0) >> 0xC) + 1) << 0xC);
        ((Boss03*)arg1)->unk8 = 0;
        ((Boss03*)arg1)->unkC = 0;
    }
}

s32 BtlBoss03_ThrwBdge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlBoss03_ThrwBdge*      thrwBdge     = task->data;
    BtlBoss03_ThrwBdge_Args* thrwBdgeArgs = args;

    switch (stage) {
        case 0:
            return BtlBoss03_ThrwBdge_Init(thrwBdge, thrwBdgeArgs);
        case 1:
            return BtlBoss03_ThrwBdge_Update(thrwBdge);
        case 2:
            return BtlBoss03_ThrwBdge_Render(thrwBdge);
        case 3:
            return BtlBoss03_ThrwBdge_Destroy(thrwBdge);
    }
    return 1;
}

s32 BtlBoss03_ThrwBdge_Init(BtlBoss03_ThrwBdge* thrwBdge, BtlBoss03_ThrwBdge_Args* args) {
    MI_CpuSet(thrwBdge, 0, sizeof(BtlBoss03_ThrwBdge));

    u32 temp_r0 = (*(Boss03**)args)->unk84 << 0x1E;
    if (args->unk_04 == 3) {
        func_ov017_02140b14(temp_r0 >> 0x1E, &thrwBdge->unk_004, 0, 0);
        func_ov003_020824a0(&thrwBdge->unk_004, 6U, 0);
    } else {
        func_ov017_02140b14(temp_r0 >> 0x1E, &thrwBdge->unk_004, 0, 0);
        if (args->unk_04 == 0) {
            func_ov003_020824a0(&thrwBdge->unk_004, 2U, 0);
        } else {
            u32 temp_ret = RNG_Next(100);
            func_ov003_020824a0(&thrwBdge->unk_004, (u16)((temp_ret >> 0x1F) + (temp_ret / 50)), 0);
        }
    }
    func_ov003_02083118(&thrwBdge->unk_064, 2);
    thrwBdge->unk_0D4 = 0xC;
    thrwBdge->unk_0D6 = 0x1E;
    thrwBdge->unk_0A0 = 0x2B8;
    thrwBdge->unk_0AC = 0xCD;
    thrwBdge->unk_0B0 = 0xF33;
    thrwBdge->unk_0B4 = 0x400;
    thrwBdge->unk_0D0 = 2;
    thrwBdge->unk_0A4 = args->unk_08;
    thrwBdge->unk_0A8 = args->unk_0C;
    thrwBdge->unk_118 = func_ov003_020cb55c(&args->unk_00);

    void* temp_r2 = args->unk_00;

    s32 var_r3 = (args->unk_04 == 3);

    s32 var_r6;
    if (((Boss03*)temp_r2)->unk24 != 0) {
        var_r6 = 1;
    } else {
        var_r6 = 0;
    }

    thrwBdge->unk_08C = (s32)(((Boss03*)temp_r2)->unk28 + *(s32*)((u8*)&data_ov017_02142cc8 + (var_r3 * 0x10) + (var_r6 * 8)));
    thrwBdge->unk_090 = (s32)(*(Boss03**)args)->unk2C;
    thrwBdge->unk_094 = (s32)((*(Boss03**)args)->unk30 + *(s32*)((u8*)&data_ov017_02142ccc + (var_r3 * 0x10) + (var_r6 * 8)));
    thrwBdge->unk_09C = args->unk_10;

    if (args->unk_04 <= 1) {
        s32 temp_r0_2 = func_ov003_020cc5d0(&thrwBdge->unk_064);

        thrwBdge->unk_0A4 = (args->unk_18 - thrwBdge->unk_08C) / temp_r0_2;
        thrwBdge->unk_0A8 = (args->unk_1C - thrwBdge->unk_090) / temp_r0_2;
        thrwBdge->unk_0AC = 0;
    }

    thrwBdge->unk_000 = args->unk_00;
    thrwBdge->unk_100 = args->unk_04;
    thrwBdge->unk_0E2 = args->unk_14;

    if (args->unk_04 != 2) {
        func_ov017_0213edd4();
    }
    return 1;
}

s32 BtlBoss03_ThrwBdge_Update(BtlBoss03_ThrwBdge* thrwBdge) {
    void* temp_r0 = func_ov003_020cb42c(1, 0x2F, 0);

    if ((data_ov003_020e71b8->unk3D878 & 0x20000000) || ((temp_r0 != NULL) && (((Boss03*)temp_r0)->unk54 & 4))) {
        return 0;
    }
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    func_ov003_02082b0c(&thrwBdge->unk_004);
    func_ov003_02083074(&thrwBdge->unk_064);

    switch (thrwBdge->unk_0E4) {
        case 0: {
            thrwBdge->unk_0E0++;

            switch (thrwBdge->unk_100) {
                case 0:
                case 1: {
                    if (thrwBdge->unk_09C >= 0) {
                        if (thrwBdge->unk_118 == 1) {
                            func_ov017_02130444(0, 0, 1, 1, 1, &thrwBdge->unk_064);
                        } else {
                            func_ov017_02130444(0, 0, 1, 1, 0, &thrwBdge->unk_064);
                        }
                        return 0;
                    }
                } break;

                case 3: {
                    if (thrwBdge->unk_0E0 >= thrwBdge->unk_0E2) {
                        func_ov017_02130444(0, 0, 1, 0, 0, NULL);
                        return 0;
                    }
                } break;

                case 2: {
                    if (thrwBdge->unk_094 >= -0x10000) {
                        thrwBdge->unk_0E0 = 0;
                        thrwBdge->unk_0E4 = 1;
                    }
                } break;
            }
        } break;

        case 1: {
            if (thrwBdge->unk_0E0 == 0) {
                thrwBdge->unk_0E2 = 0x5A;
                func_ov003_02082cc4(&thrwBdge->unk_004);
                func_ov017_02140b14((u32)((*(Boss03**)thrwBdge)->unk84 << 0x1E) >> 0x1E, &thrwBdge->unk_004, 0, 0);
                func_ov003_020824a0(&thrwBdge->unk_004, 6, 1);
                thrwBdge->unk_09C = 0x1000;
                thrwBdge->unk_0A0 = 0xFFFFFCCD;
                thrwBdge->unk_104 = (s32)((RNG_Next(0x19) - 0x20) << 0xC);
            }
            if ((thrwBdge->unk_09C < 0) && (thrwBdge->unk_094 < thrwBdge->unk_104)) {
                thrwBdge->unk_09C = 0;
                thrwBdge->unk_0A0 = 0;
                thrwBdge->unk_0A4 = 0;
                thrwBdge->unk_0A8 = 0;
            }
            if ((thrwBdge->unk_048 == 6) && (SpriteMgr_IsAnimationFinished(&thrwBdge->unk_004) != 0)) {
                u16 var_r1;
                if (RNG_Next(100) < 50) {
                    var_r1 = 7;
                } else {
                    var_r1 = 8;
                }
                func_ov003_020824a0(&thrwBdge->unk_004, var_r1, 0);
            }
            if (thrwBdge->unk_0E0 < thrwBdge->unk_0E2) {
                thrwBdge->unk_0E0++;
            } else {
                thrwBdge->unk_0E0 = 0;
                thrwBdge->unk_0E4 = 2;
            }
        } break;

        case 2: {
            if (thrwBdge->unk_0E0 == 0) {
                thrwBdge->unk_0E8 = (s32)((*(Boss03**)thrwBdge)->unk28 + ((RNG_Next(0x31) - 0x18) << 0xC));
                thrwBdge->unk_0EC = (s32)(*(Boss03**)thrwBdge)->unk2C;
                thrwBdge->unk_0F0 = (s32)(((*(Boss03**)thrwBdge)->unk30 - 0x20000) + ((RNG_Next(0x41) - 0x20) << 0xC));
                func_ov003_020cba54(thrwBdge->unk_08C, thrwBdge->unk_090, thrwBdge->unk_094, thrwBdge->unk_0E8,
                                    thrwBdge->unk_0EC, thrwBdge->unk_0F0);
                func_ov003_020cbafc(&thrwBdge->unk_0F4, &thrwBdge->unk_0F8, &thrwBdge->unk_0FC, thrwBdge->unk_08C,
                                    thrwBdge->unk_090, thrwBdge->unk_094, thrwBdge->unk_0E8, thrwBdge->unk_0EC,
                                    thrwBdge->unk_0F0, 0x2000);
            }

            s32 temp_r4 = func_ov003_020cba54(thrwBdge->unk_08C, thrwBdge->unk_090, thrwBdge->unk_094, thrwBdge->unk_0E8,
                                              thrwBdge->unk_0EC, thrwBdge->unk_0F0);
            thrwBdge->unk_08C += thrwBdge->unk_0F4;
            thrwBdge->unk_090 += thrwBdge->unk_0F8;
            thrwBdge->unk_094 += thrwBdge->unk_0FC;

            if (temp_r4 < func_ov003_020cba54(thrwBdge->unk_08C, thrwBdge->unk_090, thrwBdge->unk_094, thrwBdge->unk_0E8,
                                              thrwBdge->unk_0EC, thrwBdge->unk_0F0))
            {
                thrwBdge->unk_0FC = 0;
                thrwBdge->unk_0F8 = 0;
                thrwBdge->unk_0F4 = 0;
            }
            if (thrwBdge->unk_0E0 > 0x258) {
                return 0;
            }
            thrwBdge->unk_0E0++;
        } break;
    }

    return 1;
}

s32 BtlBoss03_ThrwBdge_Render(BtlBoss03_ThrwBdge* thrwBdge) {
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }

    BOOL var_r0 = func_ov003_020c37f8(&thrwBdge->unk_004) != 0;
    s16  spA, sp8;

    func_ov003_02084348(var_r0, &spA, &sp8, thrwBdge->unk_08C, thrwBdge->unk_090, thrwBdge->unk_094);
    func_ov003_02082724(&thrwBdge->unk_004, spA, sp8);
    func_ov003_02082730(&thrwBdge->unk_004, 0x7FFFFFFF - thrwBdge->unk_090);
    func_ov003_02082b64(&thrwBdge->unk_004);
    return 1;
}

s32 BtlBoss03_ThrwBdge_Destroy(BtlBoss03_ThrwBdge* thrwBdge) {
    func_ov003_02082cc4(&thrwBdge->unk_004);
    return 1;
}
