#include "Debug/Launcher.h"
#include "Interrupts.h"
#include "OverlayManager.h"
#include "System.h"
#include "common_data.h"
#include "game.h"

extern void func_ov000_02082854(GameState* state);

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
    {1, "�@�\\:�m�C�Y���|�[�g", "����:���j���[��ʂ̃m�C�Y���|�[�g",         28, func_ov028_020e82d0},
    {0,                   NULL,                                NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Suyama[2] = {
    // "Feature: Staff Roll", "Description: Staff roll screen"
    {1, "�@�\\:�X�^�b�t���[��", "����:�X�^�b�t���[�����",         42, func_ov042_020824a0},
    {0,                   NULL,                      NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Takami[14] = {
    // "Feature: test2", "Description: test2"
    {1,            "�@�\\:test2",                      "����:test2",         43, func_ov043_020826ac},
    // "Feature: Menu Top", "Description: First screen of the menu"
    {2,   "�@�\\:���j���[�g�b�v",       "����:���j���[�̍ŏ��̉��",         43, func_ov043_02084040},
    // "Feature: Badge Select", "Description: Screen for organizing badges"
    {3,   "�@�\\:�o�b�W�Z���N�g",       "����:�o�b�W��Ґ�������",         43, func_ov043_0208f44c},
    // "Feature: Scenario Select", "Description: Scenario selection screen"
    {4, "�@�\\:�V�i���I�Z���N�g",     "����:�V�i���I��I��������",         43, func_ov043_0209bce4},
    // "Feature: Equipment", "Description: Equipment selection screen"
    {5,           "�@�\\:������",       "����:�����т�I��������",         43, func_ov043_020a15e8},
    // "Feature: Result", "Description: Result screen after battle"
    {6,         "�@�\\:���U���g", "����:�o�g���I����̃��U���g���",         44, func_ov044_02084a88},
    // "Feature: Shop", "Description: Shop screen"
    {7,         "�@�\\:�V���b�v",               "����:�V���b�v���",         43, func_ov043_020aeee0},
    // "Feature: Department Store", "Description: Department store screen"
    {8,         "�@�\\:�f�p�[�g",               "����:�f�p�[�g���",         43, func_ov043_020bd414},
    // "Feature: Save", "Description: Save screen"
    {9,           "�@�\\:�Z�[�u",                 "����:�Z�[�u���",         43, func_ov043_020c04f0},
    // "Feature: Test2", "Description: Test2"
    {1,          "�@�\\:�e�X�g2",                    "����:�e�X�g2",         43, func_ov043_020c6644},
    // "Feature: Communication Menu", "Description: Communication menu screen"
    {2,   "�@�\\:�ʐM�����j���[",         "����:�ʐM�����j���[���",         45, func_ov045_02083c78},
    // "Feature: Communication Setup", "Description: Communication setup screen"
    {3,       "�@�\\:�ʐM�Z�b�g",             "����:�ʐM�Z�b�g���",         45, func_ov045_02088700},
    // "Feature: Friend List", "Description: Friend list screen"
    {4,   "�@�\\:�t�����h���X�g",         "����:�t�����h���X�g���",         45, func_ov045_02091034},
    {0,                     NULL,                              NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Fukuda[13] = {
    // "Features: Font", "Description: Test"
    {1,           "�@�\\:�t�H���g",                         "����:�e�X�g",          1, func_ov001_02082b34},
    // "Feature: Font", "Description: Font list"
    {2,           "�@�\\:�t�H���g",                   "����:�t�H���g�ꗗ",          2, func_ov002_02086a0c},
    // "Feature: Title (?)", "Description: Data Deletion Screen"
    {3,     "�@�\\:�^�C�g���i�H�j",                 "����:�f�[�^�폜���",          2, func_ov002_02086a4c},
    // "Feature: Title (?)", "Description: Data Corruption Screen"
    {4,     "�@�\\:�^�C�g���i�H�j",                 "����:�f�[�^�j�����",          2, func_ov002_02086a8c},
    // "Feature: BayBadge Menu", "Description: Single Player Entrance"
    {5, "�@�\\:�x�C�o�b�W���j���[",                 "����:�P�l�p�̓����",          2, func_ov002_0208688c},
    // "Feature: BayBadge Menu", "Description: Single Player Return (Ranking)"
    {6, "�@�\\:�x�C�o�b�W���j���[",     "����:�P�l�p�̖߂�i�����L���O�j",          2, func_ov002_0208694c},
    // "Feature: BayBadge Menu", "Description: Gateway to online battles"
    {7, "�@�\\:�x�C�o�b�W���j���[",               "����:�ʐM�ΐ�̓����",          2, func_ov002_020868cc},
    // "Feature: BayBadge Menu", "Description: Return from game (Ranking)"
    {8, "�@�\\:�x�C�o�b�W���j���[", "����:�Q�[������̖߂�i�����L���O�j",          2, func_ov002_0208690c},
    // "Feature: BayBadge Menu", "Description: Error entry point"
    {9, "�@�\\:�x�C�o�b�W���j���[",               "����:�G���[�p�̓����",          2, func_ov002_0208698c},
    // "Feature: Title (?)", "Description: Data Load Failure Screen"
    {1,     "�@�\\:�^�C�g���i�H�j",         "����:�f�[�^�ǂݍ��ݎ��s���",          2, func_ov002_02086b0c},
    // "Feature: Title (?)", "Description: Data Save Failure Screen"
    {2,     "�@�\\:�^�C�g���i�H�j",         "����:�f�[�^�������ݎ��s���",          2, func_ov002_02086b4c},
    // "Feature: BayBadge Menu", "Description: Role Selection Screen"
    {3, "�@�\\:�x�C�o�b�W���j���[",                   "����:�����I�����",          2, func_ov002_020869cc},
    {0,                       NULL,                                  NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Bul[2] = {
    // "Feature: Opening and Title", "Description:"
    {3, "�@�\\:�I�[�v�j���O�ƃ^�C�g��", "����:",         37, func_ov037_0208370c},
    {0,                           NULL,    NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Abe[2] = {
    {1, "mini108", "\0",          0, func_ov000_02082854},
    {0,      NULL, NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Sugata[2] = {
    // "Feature: StreetPass Sample", "Description: StreetPass mode"
    {3, "�@�\\:���ꂿ�����T���v��", "����:���ꂿ�����ʐM���[�h",         41, func_ov041_02082bc4},
    {0,                       NULL,                        NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Kitawaki[9] = {
    {1,  "�@�\\:�t�B�[���h<->�퓬",    "����:�t�B�[���h�Ő퓬�N����",         30, func_ov030_020b0fe8},
    {2,     "�@�\\:Fld/�t�B�[���h",        "����:3D�t�B�[���h�e�X�g",         30, func_ov030_020b0e0c},
    {3,                "�@�\\:imd",             "����:imd disp test",         30, func_ov030_020b6dec},
    {4,         "�@�\\:sound test",                          "����:",         29, func_ov029_02082f9c},
    {5,       "�@�\\:event select", "����:debug�p�̃C�x���g�Z���N�g",         30, func_ov030_020b17b4},
    {6,              "�@�\\:flash",              "����:flash select",         30, func_ov030_020d5370},
    {7, "�@�\\:NPC message select",        "����:NPC message select",         30, func_ov030_020c9950},
    {8,     "�@�\\:message select",            "����:message select",         30, func_ov030_020ca1b4},
    {0,                       NULL,                             NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Furukawa[4] = {
    {1,       "�@�\\:ChkBtl/�o�g���`�F�b�N",         "����:�o�g���`�F�b�N",         27, func_ov027_020e860c},
    {2, "�@�\\:Continue/�R���e�B�j���[���", "����:�R���e�B�j���[���j���[",         25, func_ov025_020e82c8},
    {3, "�@�\\:Tutorial/�`���[�g���A�����",         "����:�`���[�g���A��",         26, func_ov026_020e7f98},
    {0,                                NULL,                          NULL, 0x80000000,                NULL},
};

const DebugLauncherOption Options_Mori[6] = {
    {1,   "�@�\\:�O���t�B�b�N�`�F�b�N",  "����:�n�[�h�E�F�A�X�v���C�g",         38, func_ov038_02085674},
    {2,   "�@�\\:�O���t�B�b�N�`�F�b�N", "����:�\\�t�g�E�F�A�X�v���C�g",         38, func_ov038_020856b4},
    {3,  "�@�\\:ChkBtl/�o�g���`�F�b�N",          "����:�o�g���`�F�b�N",         27, func_ov027_020e9670},
    {4,           "�@�\\:���P�^�C�g��",                    "����:test",         30, func_ov030_020d2d6c},
    {5, "�@�\\:�I�[�v�j���O�ƃ^�C�g��",                        "����:",         37, func_ov037_0208370c},
    {0,                           NULL,                           NULL, 0x80000000,                NULL},
};

const DebugLauncherCategory Categories[11] = {
    {21,     "�쐬�F�k�e", "�����FApl_Kit�f�B���N�g��", Options_Kitawaki},
    {16,     "�쐬�F�Ð�", "�����FApl_Fur�f�B���N�g��", Options_Furukawa},
    {12, "�쐬�F�o���b�g", "�����FApl_Bul�f�B���N�g��",      Options_Bul},
    {23,       "�쐬�F�X", "�����FApl_Mor�f�B���N�g��",     Options_Mori},
    {11,     "�쐬�F���{", "�����FApl_Abe�f�B���N�g��",      Options_Abe},
    {30,     "�쐬�F����", "�����FApl_Tak�f�B���N�g��",   Options_Takami},
    {29,     "�쐬�F���c", "�����FApl_Sug�f�B���N�g��",   Options_Sugata},
    {16,     "�쐬�F���c", "�����FApl_Fuk�f�B���N�g��",   Options_Fukuda},
    {18,     "�쐬�F�x��", "�����FApl_Hor�f�B���N�g��",    Options_Horii},
    {29,     "�쐬�F�{�R", "�����FApl_Suy�f�B���N�g��",   Options_Suyama},
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

extern InputState data_0206a9e0;

void func_ov046_020824a0(void) {
    /* Not yet implemented */
}

void func_ov046_02082720(void) {
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0)) {
        func_02006380();
        func_020019ac();
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
    u16 uVar1             = data_0206a9e0.pressedButtons;
    state->currButtons    = data_0206a9e0.currButtons;
    state->pressedButtons = uVar1;
    uVar1                 = data_0206a9e0.prevButtons;
    state->holdButtons    = data_0206a9e0.holdButtons;
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
        state           = func_02004618(&data_0206a9b0, sizeof(DebugLauncherState));
        func_020049a8(&data_0206a9b0, state, seq);
        func_02007260(state);
    }

    func_0203b2d0(0, state, func_0200498c(&data_0206a9b0, state));
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
