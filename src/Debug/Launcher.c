#include "Debug/Launcher.h"
#include "Input.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayManager.h"
#include "System.h"
#include "common_data.h"
#include "game.h"

extern void func_ov000_02082854(u32* state);

extern void func_ov001_02082b34(GameState* state);

extern void func_ov002_0208688c(GameState* state);
extern void func_ov002_020868cc(GameState* state);
extern void func_ov002_0208690c(GameState* state);
extern void func_ov002_0208694c(GameState* state);
extern void func_ov002_0208698c(GameState* state);
extern void func_ov002_020869cc(GameState* state);
extern void func_ov002_02086a0c(GameState* state);
extern void func_ov002_02086a4c(GameState* state);
extern void func_ov002_02086a8c(GameState* state);
extern void func_ov002_02086b0c(GameState* state);
extern void func_ov002_02086b4c(GameState* state);

extern void func_ov025_020e82c8(GameState* state);

extern void func_ov026_020e7f98(GameState* state);

extern void func_ov027_020e860c(GameState* state);
extern void func_ov027_020e9670(GameState* state);

extern void func_ov028_020e82d0(GameState* state);

extern void func_ov029_02082f9c(GameState* state);

extern void func_ov030_020b0fe8(GameState* state);
extern void func_ov030_020b0e0c(GameState* state);
extern void func_ov030_020b17b4(GameState* state);
extern void func_ov030_020b6dec(GameState* state);
extern void func_ov030_020c9950(GameState* state);
extern void func_ov030_020ca1b4(GameState* state);
extern void func_ov030_020d2d6c(GameState* state);
extern void func_ov030_020d5370(GameState* state);

extern void func_ov037_0208370c(GameState* state);

extern void func_ov038_02085674(GameState* state);
extern void func_ov038_020856b4(GameState* state);

extern void func_ov041_02082bc4(GameState* state);

extern void func_ov042_020824a0(GameState* state);

extern void func_ov043_020826ac(GameState* state);
extern void func_ov043_02084040(GameState* state);
extern void func_ov043_0208f44c(GameState* state);
extern void func_ov043_0209bce4(GameState* state);
extern void func_ov043_020a15e8(GameState* state);
extern void func_ov043_020aeee0(GameState* state);
extern void func_ov043_020bd414(GameState* state);
extern void func_ov043_020c04f0(GameState* state);
extern void func_ov043_020c6644(GameState* state);

extern void func_ov044_02084a88(GameState* state);

extern void func_ov045_02083c78(GameState* state);
extern void func_ov045_02088700(GameState* state);
extern void func_ov045_02091034(GameState* state);

void func_ov046_020835b4(DebugLauncherState* state);
void func_ov046_020834c0(DebugLauncherState* state);
void func_ov046_02083454(DebugLauncherState* state);

void func_ov046_020839a0();
void func_ov046_020839b4();
void func_ov046_020839c4();
void func_ov046_020839d8();

static const DebugLauncherFunc funcs[3] = {
    func_ov046_020835b4,
    func_ov046_020834c0,
    func_ov046_02083454,
};

const DebugLauncherOption Options_Horii[2] = {
    // "Feature: Noise Report", "Description: Menu screen noise report"
    {1, "機能\:ノイズレポート", "説明:メニュー画面のノイズレポート",         28, func_ov028_020e82d0},
    {0,                   NULL,                                NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Suyama[2] = {
    // "Feature: Staff Roll", "Description: Staff roll screen"
    {1, "機能\:スタッフロール", "説明:スタッフロール画面",         42, func_ov042_020824a0},
    {0,                   NULL,                      NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Takami[14] = {
    // "Feature: test2", "Description: test2"
    {1,            "機能\:test2",                      "説明:test2",         43, func_ov043_020826ac},
    // "Feature: Menu Top", "Description: First screen of the menu"
    {2,   "機能\:メニュートップ",       "説明:メニューの最初の画面",         43, func_ov043_02084040},
    // "Feature: Badge Select", "Description: Screen for organizing badges"
    {3,   "機能\:バッジセレクト",       "説明:バッジを編成する画面",         43, func_ov043_0208f44c},
    // "Feature: Scenario Select", "Description: Scenario selection screen"
    {4, "機能\:シナリオセレクト",     "説明:シナリオを選択する画面",         43, func_ov043_0209bce4},
    // "Feature: Equipment", "Description: Equipment selection screen"
    {5,           "機能\:そうび",       "説明:そうびを選択する画面",         43, func_ov043_020a15e8},
    // "Feature: Result", "Description: Result screen after battle"
    {6,         "機能\:リザルト", "説明:バトル終了後のリザルト画面",         44, func_ov044_02084a88},
    // "Feature: Shop", "Description: Shop screen"
    {7,         "機能\:ショップ",               "説明:ショップ画面",         43, func_ov043_020aeee0},
    // "Feature: Department Store", "Description: Department store screen"
    {8,         "機能\:デパート",               "説明:デパート画面",         43, func_ov043_020bd414},
    // "Feature: Save", "Description: Save screen"
    {9,           "機能\:セーブ",                 "説明:セーブ画面",         43, func_ov043_020c04f0},
    // "Feature: Test2", "Description: Test2"
    {1,          "機能\:テスト2",                    "説明:テスト2",         43, func_ov043_020c6644},
    // "Feature: Communication Menu", "Description: Communication menu screen"
    {2,   "機能\:通信中メニュー",         "説明:通信中メニュー画面",         45, func_ov045_02083c78},
    // "Feature: Communication Setup", "Description: Communication setup screen"
    {3,       "機能\:通信セット",             "説明:通信セット画面",         45, func_ov045_02088700},
    // "Feature: Friend List", "Description: Friend list screen"
    {4,   "機能\:フレンドリスト",         "説明:フレンドリスト画面",         45, func_ov045_02091034},
    {0,                     NULL,                              NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Fukuda[13] = {
    // "Features: Font", "Description: Test"
    {1,           "機能\:フォント",                         "説明:テスト",          1, func_ov001_02082b34},
    // "Feature: Font", "Description: Font list"
    {2,           "機能\:フォント",                   "説明:フォント一覧",          2, func_ov002_02086a0c},
    // "Feature: Title (?)", "Description: Data Deletion Screen"
    {3,     "機能\:タイトル（？）",                 "説明:データ削除画面",          2, func_ov002_02086a4c},
    // "Feature: Title (?)", "Description: Data Corruption Screen"
    {4,     "機能\:タイトル（？）",                 "説明:データ破損画面",          2, func_ov002_02086a8c},
    // "Feature: BayBadge Menu", "Description: Single Player Entrance"
    {5, "機能\:ベイバッジメニュー",                 "説明:１人用の入り口",          2, func_ov002_0208688c},
    // "Feature: BayBadge Menu", "Description: Single Player Return (Ranking)"
    {6, "機能\:ベイバッジメニュー",     "説明:１人用の戻り（ランキング）",          2, func_ov002_0208694c},
    // "Feature: BayBadge Menu", "Description: Gateway to online battles"
    {7, "機能\:ベイバッジメニュー",               "説明:通信対戦の入り口",          2, func_ov002_020868cc},
    // "Feature: BayBadge Menu", "Description: Return from game (Ranking)"
    {8, "機能\:ベイバッジメニュー", "説明:ゲームからの戻り（ランキング）",          2, func_ov002_0208690c},
    // "Feature: BayBadge Menu", "Description: Error entry point"
    {9, "機能\:ベイバッジメニュー",               "説明:エラー用の入り口",          2, func_ov002_0208698c},
    // "Feature: Title (?)", "Description: Data Load Failure Screen"
    {1,     "機能\:タイトル（？）",         "説明:データ読み込み失敗画面",          2, func_ov002_02086b0c},
    // "Feature: Title (?)", "Description: Data Save Failure Screen"
    {2,     "機能\:タイトル（？）",         "説明:データ書き込み失敗画面",          2, func_ov002_02086b4c},
    // "Feature: BayBadge Menu", "Description: Role Selection Screen"
    {3, "機能\:ベイバッジメニュー",                   "説明:役割選択画面",          2, func_ov002_020869cc},
    {0,                       NULL,                                  NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Bul[2] = {
    // "Feature: Opening and Title", "Description:"
    {3, "機能\:オープニングとタイトル", "説明:",         37, func_ov037_0208370c},
    {0,                           NULL,    NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Abe[2] = {
    {1, "mini108", "\0",          0, func_ov000_02082854},
    {0,      NULL, NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Sugata[2] = {
    // "Feature: StreetPass Sample", "Description: StreetPass mode"
    {3, "機能\:すれちがいサンプル", "説明:すれちがい通信モード",         41, func_ov041_02082bc4},
    {0,                       NULL,                        NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Kitawaki[9] = {
    {1,  "機能\:フィールド<->戦闘",    "説明:フィールドで戦闘起こる",         30, func_ov030_020b0fe8},
    {2,     "機能\:Fld/フィールド",        "説明:3Dフィールドテスト",         30, func_ov030_020b0e0c},
    {3,                "機能\:imd",             "説明:imd disp test",         30, func_ov030_020b6dec},
    {4,         "機能\:sound test",                          "説明:",         29, func_ov029_02082f9c},
    {5,       "機能\:event select", "説明:debug用のイベントセレクト",         30, func_ov030_020b17b4},
    {6,              "機能\:flash",              "説明:flash select",         30, func_ov030_020d5370},
    {7, "機能\:NPC message select",        "説明:NPC message select",         30, func_ov030_020c9950},
    {8,     "機能\:message select",            "説明:message select",         30, func_ov030_020ca1b4},
    {0,                       NULL,                             NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Furukawa[4] = {
    {1,       "機能\:ChkBtl/バトルチェック",         "説明:バトルチェック",         27, func_ov027_020e860c},
    {2, "機能\:Continue/コンティニュー画面", "説明:コンティニューメニュー",         25, func_ov025_020e82c8},
    {3, "機能\:Tutorial/チュートリアル画面",         "説明:チュートリアル",         26, func_ov026_020e7f98},
    {0,                                NULL,                          NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Mori[6] = {
    {1,   "機能\:グラフィックチェック",  "説明:ハードウェアスプライト",         38, func_ov038_02085674},
    {2,   "機能\:グラフィックチェック", "説明:ソ\フトウェアスプライト",         38, func_ov038_020856b4},
    {3,  "機能\:ChkBtl/バトルチェック",          "説明:バトルチェック",         27, func_ov027_020e9670},
    {4,           "機能\:ロケタイトル",                    "説明:test",         30, func_ov030_020d2d6c},
    {5, "機能\:オープニングとタイトル",                        "説明:",         37, func_ov037_0208370c},
    {0,                           NULL,                           NULL, 0x80000000,                NULL},
};

const DebugLauncherCategory Categories[11] = {
    {21,     "作成：北脇", "説明：Apl_Kitディレクトリ", Options_Kitawaki},
    {16,     "作成：古川", "説明：Apl_Furディレクトリ", Options_Furukawa},
    {12, "作成：バレット", "説明：Apl_Bulディレクトリ",      Options_Bul},
    {23,       "作成：森", "説明：Apl_Morディレクトリ",     Options_Mori},
    {11,     "作成：安倍", "説明：Apl_Abeディレクトリ",      Options_Abe},
    {30,     "作成：高見", "説明：Apl_Takディレクトリ",   Options_Takami},
    {29,     "作成：菅田", "説明：Apl_Sugディレクトリ",   Options_Sugata},
    {16,     "作成：福田", "説明：Apl_Fukディレクトリ",   Options_Fukuda},
    {18,     "作成：堀居", "説明：Apl_Horディレクトリ",    Options_Horii},
    {29,     "作成：須山", "説明：Apl_Suyディレクトリ",   Options_Suyama},
    { 0,             NULL,                        NULL,             NULL},
};

static const char* Sequence = "Seq_Launcher(void *)";

const s32 data_ov046_02083a34[3] = {0, 0x1000, 0};
const s32 data_ov046_02083a40[3] = {0, 0, -0x1000};

static const DebugLauncherFunc data_ov046_02083f7c[] = {
    func_ov046_020839a0,
    func_ov046_020839b4,
    func_ov046_020839c4,
    func_ov046_020839d8,
};

void func_ov046_020824a0(void) {
    /* Not yet implemented */
}

void func_ov046_02082720(void) {
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0)) {
        func_02006380();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 0x400);
        func_02037264(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        func_020372b8(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        func_02037108(&data_02066aec, 0, 0x200);
        func_020371b4(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        func_0203715c(&data_02066eec, 0, 0x200);
        func_0203720c(&data_020670ec, 0, 0x200);
    }
}

void func_ov046_020827f0(void) {
    func_ov046_020824a0();
    Interrupts_RegisterVBlankCallback(func_ov046_02082720, TRUE);
}

void func_ov046_0208280c(void) {
    func_ov046_020824a0();
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov046_02082824(void) {
    func_02026180(0, 0, 0x800);
    if (func_0202623c() == FALSE) {
        func_02007328();
    }
}

void func_ov046_0208284c(void) {
    func_02026180(0, -0x20, 0x800);
    if (func_0202623c() == FALSE) {
        func_02007328();
    }
}

void func_ov046_020834c0(DebugLauncherState* state) {
    u16 uVar1             = InputStatus.pressedButtons;
    state->currButtons    = InputStatus.currButtons;
    state->pressedButtons = uVar1;
    uVar1                 = InputStatus.prevButtons;
    state->holdButtons    = InputStatus.holdButtons;
    state->prevButtons    = uVar1;

    func_02006ba0();
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_02007390();

    if (func_0200737c() == TRUE) {
        state->active = TRUE;
    }

    if (state->active == TRUE) {
        OverlayTag tag;
        func_020071f4(&tag, state->overlay, state->overlayCB, NULL, 0);
    } else {
        func_0200f514(&state->unk_11650);
        func_020034b0(&data_020676ec);
        func_020034b0(&data_02068778);
        func_0200bf60(data_0206b3cc.unk_00, 0);
        func_0200bf60(data_0206b3cc.unk_04, 0);
    }
}

void func_ov046_020835b4(DebugLauncherState* state) {
    if (state == NULL) {
        const char* seq = Sequence;
        state           = Mem_AllocHeapTail(&gDebugHeap, sizeof(DebugLauncherState));
        Mem_SetSequence(&gDebugHeap, state, seq);
        func_02007260(state);
    }

    func_0203b2d0(0, state, Mem_GetBlockSize(&gDebugHeap, state));
    func_ov046_02082c78(state);
    func_ov046_02083368(state);
    func_020072a4();
}

void func_ov046_020834c0();

void func_ov046_02083630(DebugLauncherState* state) {

    s32 sVar1 = func_02007278();
    if (sVar1 == 0x7fffffff) {
        func_ov046_02083454(state);
    } else {
        funcs[sVar1](state);
    }
}
