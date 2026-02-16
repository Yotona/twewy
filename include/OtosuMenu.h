#ifndef OTOSUMENU_H
#define OTOSUMENU_H

#include "BgResMgr.h"
#include "EasyTask.h"
#include "Memory.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00000 */ char        unk_00000[0x11580];
    /* 0x11580 */ s32         unk_11580;
    /* 0x11584 */ s32         unk_11584;
    /* 0x11588 */ s32         unk_11588;
    /* 0x1158C */ s32         unk_1158C;
    /* 0x11590 */ MemPool     memPool;
    /* 0x1159C */ u8          memPoolBuffer[0x30000];
    /* 0x4159C */ TaskPool    taskPool;
    /* 0x4161C */ s32         unk_4161C;
    /* 0x41620 */ char        unk_41620[0x41804 - 0x41620];
    /* 0x41804 */ s32         unk_41804;
    /* 0x41808 */ s32         unk_41808;
    /* 0x4180C */ s32         unk_4180C;
    /* 0x41810 */ s32         unk_41810;
    /* 0x41814 */ s32         unk_41814;
    /* 0x41818 */ s32         unk_41818;
    /* 0x4181C */ s32         unk_4181C;
    /* 0x41820 */ char        unk_41820[0x41832 - 0x41820];
    /* 0x41832 */ u16         unk_41832;
    /* 0x41834 */ u16         unk_41834;
    /* 0x41836 */ u16         unk_41836;
    /* 0x41838 */ char        unk_41838[0x41862 - 0x41838];
    /* 0x41862 */ s16         unk_41862;
    /* 0x41864 */ char        unk_41864[0x41950 - 0x41864];
    /* 0x41950 */ s32         unk_41950;
    /* 0x41954 */ s32         unk_41954;
    /* 0x41958 */ s16         unk_41958;
    /* 0x4195A */ s16         unk_4195A;
    /* 0x4195C */ char        unk_4195C[0x41962 - 0x4195C];
    /* 0x41962 */ s16         unk_41962;
    /* 0x41964 */ char        unk_41964[0x4196A - 0x41964];
    /* 0x4196A */ s16         unk_4196A;
    /* 0x4196C */ s16         unk_4196C;
    /* 0x4196E */ s16         unk_4196E;
    /* 0x41970 */ char        unk_41970[0x4198A - 0x41970];
    /* 0x4198A */ s16         unk_4198A;
    /* 0x4198C */ s32         unk_4198C;
    /* 0x41990 */ s32         unk_41990;
    /* 0x41994 */ s32         unk_41994;
    /* 0x41998 */ char        unk_41998[0x419A0 - 0x41998];
    /* 0x419A0 */ s32         unk_419A0;
    /* 0x419A4 */ char        unk_419A4[0x419F0 - 0x419A4];
    /* 0x419F0 */ u16*        unk_419F0;
    /* 0x419F4 */ char        unk_419F4[0x41E38 - 0x419F4];
    /* 0x41E38 */ s32         unk_41E38;
    /* 0x41E3C */ char        unk_41E3C[0x41EF0 - 0x41E3C];
    /* 0x41EF0 */ s32         unk_41EF0;
    /* 0x41EF4 */ char        unk_41EF4[0x41FB6 - 0x41EF4];
    /* 0x41FB6 */ s16         unk_41FB6;
    /* 0x41FB8 */ char        unk_41FB8[0x41FD5 - 0x41FB8];
    /* 0x41FD5 */ u8          unk_41FD5;
    /* 0x41FD6 */ char        unk_41FD6[0x41FDD - 0x41FD6];
    /* 0x41FDD */ u8          unk_41FDD;
    /* 0x41FDE */ char        unk_41FDE[0x41FE9 - 0x41FDE];
    /* 0x41FE9 */ u8          unk_41FE9;
    /* 0x41FEA */ char        unk_41FEA[0x41FF0 - 0x41FEA];
    /* 0x41FF0 */ s32         unk_41FF0;
    /* 0x41FF4 */ char        unk_41FF4[0x45FF4 - 0x41FF4];
    /* 0x45FF4 */ s32         unk_45FF4;
    /* 0x45FF8 */ char        unk_45FF8[0x46070 - 0x45FF8];
    /* 0x46070 */ s16         unk_46070;
    /* 0x46072 */ char        unk_46072[0x46074 - 0x46072];
    /* 0x46074 */ u32         unk_46074;
    /* 0x46078 */ Sprite      unk_46078;
    /* 0x460B8 */ s32         unk_460B8;
    /* 0x460BC */ s32         unk_460BC;
    /* 0x460C0 */ Sprite      unk_460C0;
    /* 0x46100 */ s32         unk_46100;
    /* 0x46104 */ char        unk_46104[0x462E8 - 0x46104];
    /* 0x462E8 */ s32         unk_462E8;
    /* 0x462EC */ s32         unk_462EC;
    /* 0x462F0 */ Data*       unk_462F0;
    /* 0x462F4 */ Data*       unk_462F4;
    /* 0x462F8 */ Data*       unk_462F8;
    /* 0x462FC */ Data*       unk_462FC;
    /* 0x46300 */ s32         unk_46300;
    /* 0x46304 */ void*       unk_46304;
    /* 0x46308 */ void*       unk_46308;
    /* 0x4630C */ void*       unk_4630C;
    /* 0x46310 */ void*       unk_46310;
    /* 0x46314 */ void*       unk_46314;
    /* 0x46318 */ void*       unk_46318;
    /* 0x4631C */ void*       unk_4631C;
    /* 0x46320 */ s32         unk_46320;
    /* 0x46324 */ char        unk_46324[0x47000 - 0x46324];
    /* 0x47000 */ s32         unk_47000;
    /* 0x47004 */ char        unk_47004[0x47320 - 0x47004];
    /* 0x47320 */ BgResource* unk_47320;
    /* 0x47324 */ BgResource* unk_47324;
    /* 0x47328 */ BgResource* unk_47328;
    /* 0x4732C */ BgResource* unk_4732C;
    /* 0x47330 */ BgResource* unk_47330;
    /* 0x47334 */ BgResource* unk_47334;
    /* 0x47338 */ BgResource* unk_47338;
    /* 0x4733C */ BgResource* unk_4733C;
    /* 0x47340 */ void*       unk_47340;
    /* 0x47344 */ void*       unk_47344;
    /* 0x47348 */ char        unk_47348[0x47370 - 0x47348];
    /* 0x47370 */ s32         unk_47370;
    /* 0x47374 */ s32         unk_47374;
    /* 0x47378 */ char        unk_47378[0x47398 - 0x47378];
    /* 0x47398 */ s32         unk_47398;
    /* 0x4739C */ s32         unk_4739C;
    /* 0x473A0 */ char        unk_473A0[0x473C0 - 0x473A0];
    /* 0x473C0 */ s32         unk_473C0;
    /* 0x473C4 */ char        unk_473C4[0x473E8 - 0x473C4];
    /* 0x473E8 */ s32         unk_473E8;
    /* 0x473EC */ char        unk_473EC[0x47410 - 0x473EC];
    /* 0x47410 */ s32         unk_47410;
    /* 0x47414 */ char        unk_47414[0x47438 - 0x47414];
    /* 0x47438 */ s32         unk_47438;
    /* 0x4743C */ char        unk_4743C[0x47460 - 0x4743C];
    /* 0x47460 */ s32         unk_47460;
    /* 0x47464 */ char        unk_47464[0x47488 - 0x47464];
    /* 0x47488 */ s32         unk_47488;
    /* 0x4748C */ char        unk_4748C[0x47490 - 0x4748C];
    /* 0x47490 */ s32         unk_47490;
    /* 0x47494 */ char        unk_47494[0x47498 - 0x47494];
    /* 0x47498 */ s32         unk_47498;
    /* 0x4749C */ s32         unk_4749C;
    /* 0x474A0 */ s32         unk_474A0;
    /* 0x474A4 */ char        unk_474A4[0x474A8 - 0x474A4];
    /* 0x474A8 */ s32         unk_474A8;
    /* 0x474AC */ s32         unk_474AC;
    /* 0x474B0 */ s32         unk_474B0;
    /* 0x474B4 */ char        unk_474B4[0x474B8 - 0x474B4];
    /* 0x474B8 */ s32         unk_474B8;
    /* 0x474BC */ s32         unk_474BC;
    /* 0x474C0 */ s32         unk_474C0;
    /* 0x474C4 */ char        unk_474C4[0x474C8 - 0x474C4];
    /* 0x474C8 */ u16         unk_474C8;
    /* 0x474CA */ u16         unk_474CA;
    /* 0x474CC */ u16         unk_474CC;
    /* 0x474D0 */ char        unk_474D0[0x474D8 - 0x474CE];
    /* 0x474D8 */ s32         unk_474D8;
    /* 0x474DC */ s32         unk_474DC;
    /* 0x474E0 */ s32         unk_474E0;
    /* 0x474E4 */ s32         unk_474E4;
    /* 0x474E8 */ s32         unk_474E8;
    /* 0x474EC */ char        unk_474EC[0x476D0 - 0x474EC];
    /* 0x476D0 */ s32         unk_476D0;
    /* 0x476D4 */ char        unk_476D4[0x478B8 - 0x476D4];
    /* 0x478B8 */ s32         unk_478B8;
    /* 0x478BC */ char        unk_478BC[0x47AA0 - 0x478BC];
    /* 0x47AA0 */ s32         unk_47AA0;
    /* 0x47AA4 */ char        unk_47AA4[0x47C88 - 0x47AA4];
    /* 0x47C88 */ s32         unk_47C88;
    /* 0x47C8C */ char        unk_47C8C[0x48058 - 0x47C8C];
    /* 0x48058 */ s32         unk_48058;
} OtosuMenuObj; // Size: 0x48068

/**
 * @brief Launcher for displaying a test screen that shows pin symbols
 * and a cyclable text field containing either battle tutorials or generic samples
 */
void ProcessOverlay_OtosuMenu_FontList(void* menuObj);

/**
 * @brief Launcher for starting single player mode of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_SinglePlayerEnter(void* menuObj);

/**
 * @brief Launcher for displaying single player rankings of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_SinglePlayerRanking(void* menuObj);

/**
 * @brief Launcher for starting multiplayer mode of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_MultiplayerEnter(void* menuObj);

/**
 * @brief Launcher for displaying multiplayer rankings of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_MultiplayerRanking(void* menuObj);

/**
 * @brief Launcher for displaying connection error screen of Tin Pin Slammer
 */
void ProcessOverlay_OtosuMenu_ConnectionError(void* menuObj);

#endif // OTOSUMENU_H