#include "Field/FieldRegion.h"

#include "CriSndMgr.h"
#include "Display.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/System.h"
#include "Engine/IO/Input.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Field/Flash/FlashWrapper.h"
#include "Field/Flash/SeqFlash.h"
#include "Field/Flash/SeqFlashSel.h"
#include "Field/Scene/FieldSeq.h"
#include "Save.h"
#include "SndMgr.h"
#include <nitro/gx.h>
#include <nitro/mi.h>
#include <nitro/mi/cpumem.h>

// Movie bgm values past the real track ids.
#define FLASH_BGM_STOP (ADX_COUNT)
#define FLASH_BGM_KEEP (ADX_COUNT + 1)

void SeqFlash_Init(SeqFlashState* state);
void SeqFlash_Update(SeqFlashState* state);
void SeqFlash_Destroy(SeqFlashState* state);

char        SeqFlash_SequenceName[] = "Seq_Flash()";
const char* SeqFlash_Sequence       = SeqFlash_SequenceName;

char FlashName_catchE6[] = "catchE6";

char FlashName_catchB4[] = "catchB4";

char FlashName_catchE1[] = "catchE1";

char FlashName_b_07_09[] = "b_07_09";

char FlashName_r_07_02[] = "r_07_02";

char FlashName_b_07_08[] = "b_07_08";

char FlashName_r_04_01[] = "r_04_01";

char FlashName_catchR5[] = "catchR5";

char FlashName_catchB5[] = "catchB5";

char FlashName_catchR4[] = "catchR4";

char FlashName_b_07_01[] = "b_07_01";

char FlashName_catchE2[] = "catchE2";

char FlashName_showend[] = "showend";

char FlashName_catchE3[] = "catchE3";

char FlashName_catchB6[] = "catchB6";

char FlashName_b_02_01[] = "b_02_01";

char FlashName_catchE7[] = "catchE7";

char FlashName_e_01_05[] = "e_01_05";

char FlashName_r_02_03[] = "r_02_03";

char FlashName_catchR3[] = "catchR3";

char FlashName_r_05_01[] = "r_05_01";

char FlashName_catchC1[] = "catchC1";

char FlashName_catchE5[] = "catchE5";

char FlashName_r_07_03[] = "r_07_03";

char FlashName_r_02_01[] = "r_02_01";

char FlashName_r_01_02[] = "r_01_02";

char FlashName_e_01_03[] = "e_01_03";

char FlashName_b_07_02[] = "b_07_02";

char FlashName_s_01_01[] = "s_01_01";

char FlashName_e_07_02[] = "e_07_02";

char FlashName_b_07_11[] = "b_07_11";

char FlashName_r_01_01[] = "r_01_01";

char FlashName_e_04_01[] = "e_04_01";

char FlashName_catchR2[] = "catchR2";

char FlashName_e_03_01[] = "e_03_01";

char FlashName_b_07_07[] = "b_07_07";

char FlashName_catchB1[] = "catchB1";

char FlashName_e_07_01[] = "e_07_01";

char FlashName_r_07_01[] = "r_07_01";

char FlashName_e_03_02[] = "e_03_02";

char FlashName_r_02_02[] = "r_02_02";

char FlashName_catchB3[] = "catchB3";

char FlashName_b_07_06[] = "b_07_06";

char FlashName_b_01_01[] = "b_01_01";

char FlashName_b_07_04[] = "b_07_04";

char FlashName_e_01_06[] = "e_01_06";

char FlashName_catchR7[] = "catchR7";

char FlashName_catchB2[] = "catchB2";

char FlashName_e_01_01[] = "e_01_01";

char FlashName_e_01_04[] = "e_01_04";

char FlashName_b_07_03[] = "b_07_03";

char FlashName_catchE4[] = "catchE4";

char FlashName_catchR6[] = "catchR6";

char FlashName_catchR1[] = "catchR1";

char FlashName_catchB7[] = "catchB7";

char FlashName_r_06_end[] = "r_06_end";

char FlashName_r_07_end[] = "r_07_end";

char FlashName_r_03_end[] = "r_03_end";

char FlashName_b_02_end[] = "b_02_end";

char FlashName_b_01_end[] = "b_01_end";

char FlashName_b_03_end[] = "b_03_end";

char FlashName_r_01_end[] = "r_01_end";

char FlashName_b_04_end[] = "b_04_end";

char FlashName_b_05_end[] = "b_05_end";

char FlashName_b_06_end[] = "b_06_end";

char FlashName_r_02_end[] = "r_02_end";

char FlashName_e_01_end[] = "e_01_end";

char FlashName_e_02_end[] = "e_02_end";

char FlashName_e_03_end[] = "e_03_end";

char FlashName_r_05_end[] = "r_05_end";

char FlashName_e_04_end[] = "e_04_end";

char FlashName_e_05_end[] = "e_05_end";

char FlashName_e_06_end[] = "e_06_end";

char FlashName_r_04_end[] = "r_04_end";

char FlashName_b_07_end[] = "b_07_end";

char FlashName_e_07_end[] = "e_07_end";

char FlashName_EndingFlash[] = "EndingFlash";

char FlashName_test_b_07_08[] = "test_b_07_08";

char FlashName_test_r_01_01[] = "test_r_01_01";

char FlashName_test_r_07_02[] = "test_r_07_02";

char FlashName_test_r_02_01[] = "test_r_02_01";

char FlashName_OpeningFlash[] = "OpeningFlash";

char FlashName_test_r_04_01[] = "test_r_04_01";

char FlashPath_op1[] = "Apl_Kit/op1.fds";

char FlashPath_op2[] = "Apl_Kit/op2.fds";

char FlashPath_wait[] = "Apl_Kit/wait.fds";

char FlashPath_movie1[] = "Apl_Kit/movie1.fds";

char FlashPath_movie4[] = "Apl_Kit/movie4.fds";

char FlashPath_movie0[] = "Apl_Kit/movie0.fds";

char FlashPath_b_07_07[] = "Apl_Kit/b_07_07.fds";

char FlashPath_catchE4[] = "Apl_Kit/catchE4.fds";

char FlashPath_e_04_01[] = "Apl_Kit/e_04_01.fds";

char FlashPath_b_07_08[] = "Apl_Kit/b_07_08.fds";

char FlashPath_catchE5[] = "Apl_Kit/catchE5.fds";

char FlashPath_b_07_09[] = "Apl_Kit/b_07_09.fds";

char FlashPath_catchE6[] = "Apl_Kit/catchE6.fds";

char FlashPath_b_07_10[] = "Apl_Kit/b_07_10.fds";

char FlashPath_b_07_12[] = "Apl_Kit/b_07_12.fds";

char FlashPath_catchE7[] = "Apl_Kit/catchE7.fds";

char FlashPath_e_07_01[] = "Apl_Kit/e_07_01.fds";

char FlashPath_b_07_11[] = "Apl_Kit/b_07_11.fds";

char FlashPath_catchR1[] = "Apl_Kit/catchR1.fds";

char FlashPath_r_01_01[] = "Apl_Kit/r_01_01.fds";

char FlashPath_catchC1[] = "Apl_Kit/catchC1.fds";

char FlashPath_r_01_02[] = "Apl_Kit/r_01_02.fds";

char FlashPath_s_01_01[] = "Apl_Kit/s_01_01.fds";

char FlashPath_catchR2[] = "Apl_Kit/catchR2.fds";

char FlashPath_r_02_01[] = "Apl_Kit/r_02_01.fds";

char FlashPath_showend[] = "Apl_Kit/showend.fds";

char FlashPath_r_02_02[] = "Apl_Kit/r_02_02.fds";

char FlashPath_r_02_03[] = "Apl_Kit/r_02_03.fds";

char FlashPath_catchR3[] = "Apl_Kit/catchR3.fds";

char FlashPath_catchR4[] = "Apl_Kit/catchR4.fds";

char FlashPath_r_04_01[] = "Apl_Kit/r_04_01.fds";

char FlashPath_catchR5[] = "Apl_Kit/catchR5.fds";

char FlashPath_r_05_01[] = "Apl_Kit/r_05_01.fds";

char FlashPath_catchR6[] = "Apl_Kit/catchR6.fds";

char FlashPath_catchR7[] = "Apl_Kit/catchR7.fds";

char FlashPath_r_07_01[] = "Apl_Kit/r_07_01.fds";

char FlashPath_r_07_02[] = "Apl_Kit/r_07_02.fds";

char FlashPath_r_07_03[] = "Apl_Kit/r_07_03.fds";

char FlashPath_catchB1[] = "Apl_Kit/catchB1.fds";

char FlashPath_b_01_01[] = "Apl_Kit/b_01_01.fds";

char FlashPath_catchB2[] = "Apl_Kit/catchB2.fds";

char FlashPath_b_02_01[] = "Apl_Kit/b_02_01.fds";

char FlashPath_catchB3[] = "Apl_Kit/catchB3.fds";

char FlashPath_catchB4[] = "Apl_Kit/catchB4.fds";

char FlashPath_catchB5[] = "Apl_Kit/catchB5.fds";

char FlashPath_catchB6[] = "Apl_Kit/catchB6.fds";

char FlashPath_catchE1[] = "Apl_Kit/catchE1.fds";

char FlashPath_e_01_01[] = "Apl_Kit/e_01_01.fds";

char FlashPath_catchB7[] = "Apl_Kit/catchB7.fds";

char FlashPath_e_01_03[] = "Apl_Kit/e_01_03.fds";

char FlashPath_b_07_01[] = "Apl_Kit/b_07_01.fds";

char FlashPath_e_01_05[] = "Apl_Kit/e_01_05.fds";

char FlashPath_b_07_02[] = "Apl_Kit/b_07_02.fds";

char FlashPath_e_01_06[] = "Apl_Kit/e_01_06.fds";

char FlashPath_b_07_03[] = "Apl_Kit/b_07_03.fds";

char FlashPath_catchE2[] = "Apl_Kit/catchE2.fds";

char FlashPath_e_02_01[] = "Apl_Kit/e_02_01.fds";

char FlashPath_e_03_01[] = "Apl_Kit/e_03_01.fds";

char FlashPath_b_07_04[] = "Apl_Kit/b_07_04.fds";

char FlashPath_catchE3[] = "Apl_Kit/catchE3.fds";

char FlashPath_b_07_06[] = "Apl_Kit/b_07_06.fds";

char FlashPath_e_03_02[] = "Apl_Kit/e_03_02.fds";

char FlashPath_b_06_end[] = "Apl_Kit/b_06_end.fds";

char FlashPath_e_01_02a[] = "Apl_Kit/e_01_02a.fds";

char FlashPath_e_01_02b[] = "Apl_Kit/e_01_02b.fds";

char FlashPath_e_01_02c[] = "Apl_Kit/e_01_02c.fds";

char FlashPath_e_01_end[] = "Apl_Kit/e_01_end.fds";

char FlashPath_e_02_end[] = "Apl_Kit/e_02_end.fds";

char FlashPath_e_03_end[] = "Apl_Kit/e_03_end.fds";

char FlashPath_e_04_end[] = "Apl_Kit/e_04_end.fds";

char FlashPath_e_05_end[] = "Apl_Kit/e_05_end.fds";

char FlashPath_e_01_02d[] = "Apl_Kit/e_01_02d.fds";

char FlashPath_e_06_end[] = "Apl_Kit/e_06_end.fds";

char FlashPath_e_07_end[] = "Apl_Kit/e_07_end.fds";

char FlashPath_b_07_end[] = "Apl_Kit/b_07_end.fds";

char FlashPath_r_01_end[] = "Apl_Kit/r_01_end.fds";

char FlashPath_r_02_end[] = "Apl_Kit/r_02_end.fds";

char FlashPath_r_03_end[] = "Apl_Kit/r_03_end.fds";

char FlashPath_r_04_end[] = "Apl_Kit/r_04_end.fds";

char FlashPath_r_05_end[] = "Apl_Kit/r_05_end.fds";

char FlashPath_r_06_end[] = "Apl_Kit/r_06_end.fds";

char FlashPath_r_07_end[] = "Apl_Kit/r_07_end.fds";

char FlashPath_b_01_end[] = "Apl_Kit/b_01_end.fds";

char FlashPath_b_02_end[] = "Apl_Kit/b_02_end.fds";

char FlashPath_b_03_end[] = "Apl_Kit/b_03_end.fds";

char FlashPath_b_04_end[] = "Apl_Kit/b_04_end.fds";

char FlashPath_b_05_end[] = "Apl_Kit/b_05_end.fds";

const FlashMovie FlashMovies_e_03_02[1] = {
    {FlashPath_e_03_02, ADX_E_03_02},
};

const FlashMovie FlashMovies_b_07_06[1] = {
    {FlashPath_b_07_06, ADX_B_07_06},
};

const FlashMovie FlashMovies_r_07_end[1] = {
    {FlashPath_r_07_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_07_07[1] = {
    {FlashPath_b_07_07, ADX_B_07_07},
};

const FlashMovie FlashMovies_e_01_03[1] = {
    {FlashPath_e_01_03, ADX_E_01_03},
};

const FlashMovie FlashMovies_r_07_01[1] = {
    {FlashPath_r_07_01, ADX_R_07_01},
};

const FlashMovie FlashMovies_r_06_end[1] = {
    {FlashPath_r_06_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchR5[1] = {
    {FlashPath_catchR5, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchR7[1] = {
    {FlashPath_catchR7, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_03_01[1] = {
    {FlashPath_e_03_01, ADX_E_03_01},
};

const FlashMovie FlashMovies_b_07_01[1] = {
    {FlashPath_b_07_01, ADX_B_07_01},
};

const FlashMovie FlashMovies_b_06_end[1] = {
    {FlashPath_b_06_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_04_01[1] = {
    {FlashPath_e_04_01, ADX_E_04_01},
};

const FlashMovie FlashMovies_e_04_end[1] = {
    {FlashPath_e_04_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_06_end[1] = {
    {FlashPath_e_06_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchB2[1] = {
    {FlashPath_catchB2, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_07_02[1] = {
    {FlashPath_b_07_02, ADX_B_07_02},
};

const FlashMovie FlashMovies_b_02_end[1] = {
    {FlashPath_b_02_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchR2[1] = {
    {FlashPath_catchR2, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchB3[1] = {
    {FlashPath_catchB3, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchB4[1] = {
    {FlashPath_catchB4, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_r_01_end[1] = {
    {FlashPath_r_01_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchR4[1] = {
    {FlashPath_catchR4, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_test_b_07_08[1] = {
    {FlashPath_b_07_08, ADX_B_07_08},
};

const FlashMovie FlashMovies_catchR3[1] = {
    {FlashPath_catchR3, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_test_r_07_02[1] = {
    {FlashPath_r_07_02, ADX_R_07_02},
};

const FlashMovie FlashMovies_r_02_03[1] = {
    {FlashPath_r_02_03, ADX_R_02_03},
};

const FlashMovie FlashMovies_showend[1] = {
    {FlashPath_showend, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_07_04[1] = {
    {FlashPath_b_07_04, ADX_B_07_04},
};

const FlashMovie FlashMovies_r_01_02[1] = {
    {FlashPath_r_01_02, ADX_E_01_06},
};

const FlashMovie FlashMovies_r_01_01[1] = {
    {FlashPath_r_01_01, ADX_R_01_01},
};

const FlashMovie FlashMovies_catchR1[1] = {
    {FlashPath_catchR1, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_01_05[1] = {
    {FlashPath_e_01_05, ADX_E_01_05},
};

const FlashMovie FlashMovies_e_01_06[1] = {
    {FlashPath_e_01_06, ADX_E_01_06},
};

const FlashMovie FlashMovies_e_07_end[1] = {
    {FlashPath_e_07_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_07_end[1] = {
    {FlashPath_b_07_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_test_r_04_01[1] = {
    {FlashPath_r_04_01, ADX_R_04_01},
};

const FlashMovie FlashMovies_catchB1[1] = {
    {FlashPath_catchB1, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchR6[1] = {
    {FlashPath_catchR6, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchE7[1] = {
    {FlashPath_catchE7, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_07_03[1] = {
    {FlashPath_b_07_03, ADX_B_07_03},
};

const FlashMovie FlashMovies_catchE6[1] = {
    {FlashPath_catchE6, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_test_r_01_01[1] = {
    {FlashPath_r_01_01, ADX_R_01_01},
};

const FlashMovie FlashMovies_r_05_01[1] = {
    {FlashPath_r_05_01, ADX_R_05_01},
};

const FlashMovie FlashMovies_test_r_02_01[1] = {
    {FlashPath_r_02_01, ADX_R_02_01},
};

const FlashMovie FlashMovies_r_02_end[1] = {
    {FlashPath_r_02_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_01_end[1] = {
    {FlashPath_b_01_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_r_05_end[1] = {
    {FlashPath_r_05_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchB7[1] = {
    {FlashPath_catchB7, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_04_end[1] = {
    {FlashPath_b_04_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchE5[1] = {
    {FlashPath_catchE5, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_01_01[1] = {
    {FlashPath_b_01_01, ADX_E_01_06},
};

const FlashMovie FlashMovies_catchB6[1] = {
    {FlashPath_catchB6, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchB5[1] = {
    {FlashPath_catchB5, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_07_01[1] = {
    {FlashPath_e_07_01, ADX_E_07_01},
};

const FlashMovie FlashMovies_catchE2[1] = {
    {FlashPath_catchE2, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_r_03_end[1] = {
    {FlashPath_r_03_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_05_end[1] = {
    {FlashPath_e_05_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchC1[1] = {
    {FlashPath_catchC1, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_02_01[1] = {
    {FlashPath_b_02_01, ADX_B_02_01},
};

const FlashMovie FlashMovies_b_07_11[1] = {
    {FlashPath_b_07_11, ADX_B_07_11},
};

const FlashMovie FlashMovies_EndingFlash[1] = {
    {FlashPath_movie4, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_03_end[1] = {
    {FlashPath_e_03_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_r_02_02[1] = {
    {FlashPath_r_02_02, ADX_R_02_02},
};

const FlashMovie FlashMovies_b_03_end[1] = {
    {FlashPath_b_03_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchE4[1] = {
    {FlashPath_catchE4, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchE3[1] = {
    {FlashPath_catchE3, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_catchE1[1] = {
    {FlashPath_catchE1, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_01_end[1] = {
    {FlashPath_e_01_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_r_04_end[1] = {
    {FlashPath_r_04_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_b_05_end[1] = {
    {FlashPath_b_05_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_01_04[2] = {
    {FlashPath_op1,        ADX_E01},
    {FlashPath_op2, FLASH_BGM_KEEP},
};

const FlashMovie FlashMovies_e_07_02[2] = {
    {    FlashPath_wait, FLASH_BGM_STOP},
    {FlashPath_e_01_02a,   ADX_E_01_02A},
};

const FlashMovie FlashMovies_OpeningFlash[2] = {
    {FlashPath_movie0, FLASH_BGM_STOP},
    {FlashPath_movie1,     ADX_ENDING},
};

const FlashMovie FlashMovies_s_01_01[2] = {
    {FlashPath_s_01_01, ADX_S_01_01},
    {FlashPath_b_07_11, ADX_B_07_11},
};

const FlashMovie FlashMovies_r_02_01[2] = {
    {FlashPath_r_01_01, ADX_R_01_01},
    {FlashPath_r_02_01, ADX_R_02_01},
};

const FlashMovie FlashMovies_r_04_01[2] = {
    {FlashPath_r_01_01, ADX_R_01_01},
    {FlashPath_r_04_01, ADX_R_04_01},
};

const FlashMovie FlashMovies_r_07_03[2] = {
    { FlashPath_r_07_03,  ADX_R_07_03},
    {FlashPath_e_01_02a, ADX_E_01_02A},
};

const FlashMovie FlashMovies_r_07_02[3] = {
    {FlashPath_r_01_01, ADX_R_01_01},
    {FlashPath_r_04_01, ADX_R_04_01},
    {FlashPath_r_07_02, ADX_R_07_02},
};

const FlashMovie FlashMovies_e_02_end[3] = {
    { FlashPath_e_02_01,    ADX_E_02_01},
    { FlashPath_e_03_01,    ADX_E_03_01},
    {FlashPath_e_02_end, FLASH_BGM_STOP},
};

const FlashMovie FlashMovies_e_01_01[4] = {
    { FlashPath_e_01_01,  ADX_E_01_01},
    {FlashPath_e_01_02a, ADX_E_01_02A},
    {FlashPath_e_01_02b, ADX_E_01_02B},
    {FlashPath_e_01_02c, ADX_E_01_02C},
};

const FlashMovie FlashMovies_b_07_09[4] = {
    { FlashPath_b_07_09,  ADX_B_07_09},
    {FlashPath_e_01_02d, ADX_E_01_02A},
    { FlashPath_b_07_10,  ADX_B_07_10},
    { FlashPath_b_07_12,  ADX_B_07_12},
};

const FlashMovie FlashMovies_b_07_08[5] = {
    {FlashPath_r_01_01, ADX_R_01_01},
    {FlashPath_r_04_01, ADX_R_04_01},
    {FlashPath_r_07_02, ADX_R_07_02},
    {FlashPath_b_07_08, ADX_B_07_08},
    {FlashPath_r_02_01, ADX_R_02_01},
};

const OverlayProcess OvlProc_Flash = {
    .init = SeqFlash_Init,
    .main = SeqFlash_Update,
    .exit = SeqFlash_Destroy,
};

const FlashSequence FlashSequences[FLASH_SEQUENCE_COUNT] = {
    {2, FlashMovies_OpeningFlash, FlashName_OpeningFlash},
    {1,  FlashMovies_EndingFlash,  FlashName_EndingFlash},
    {4,      FlashMovies_e_01_01,      FlashName_e_01_01},
    {1,      FlashMovies_e_01_03,      FlashName_e_01_03},
    {2,      FlashMovies_e_01_04,      FlashName_e_01_04},
    {1,      FlashMovies_e_01_05,      FlashName_e_01_05},
    {1,      FlashMovies_e_01_06,      FlashName_e_01_06},
    {1,     FlashMovies_e_01_end,     FlashName_e_01_end},
    {3,     FlashMovies_e_02_end,     FlashName_e_02_end},
    {1,      FlashMovies_e_03_01,      FlashName_e_03_01},
    {1,      FlashMovies_e_03_02,      FlashName_e_03_02},
    {1,     FlashMovies_e_03_end,     FlashName_e_03_end},
    {1,      FlashMovies_e_04_01,      FlashName_e_04_01},
    {1,     FlashMovies_e_04_end,     FlashName_e_04_end},
    {1,     FlashMovies_e_05_end,     FlashName_e_05_end},
    {1,     FlashMovies_e_06_end,     FlashName_e_06_end},
    {1,      FlashMovies_e_07_01,      FlashName_e_07_01},
    {2,      FlashMovies_e_07_02,      FlashName_e_07_02},
    {1,     FlashMovies_e_07_end,     FlashName_e_07_end},
    {1,      FlashMovies_r_01_01,      FlashName_r_01_01},
    {1,      FlashMovies_r_01_02,      FlashName_r_01_02},
    {1,     FlashMovies_r_01_end,     FlashName_r_01_end},
    {2,      FlashMovies_r_02_01,      FlashName_r_02_01},
    {1,      FlashMovies_r_02_02,      FlashName_r_02_02},
    {1,      FlashMovies_r_02_03,      FlashName_r_02_03},
    {1,     FlashMovies_r_02_end,     FlashName_r_02_end},
    {1,     FlashMovies_r_03_end,     FlashName_r_03_end},
    {2,      FlashMovies_r_04_01,      FlashName_r_04_01},
    {1,     FlashMovies_r_04_end,     FlashName_r_04_end},
    {1,      FlashMovies_r_05_01,      FlashName_r_05_01},
    {1,     FlashMovies_r_05_end,     FlashName_r_05_end},
    {1,     FlashMovies_r_06_end,     FlashName_r_06_end},
    {1,      FlashMovies_r_07_01,      FlashName_r_07_01},
    {3,      FlashMovies_r_07_02,      FlashName_r_07_02},
    {2,      FlashMovies_r_07_03,      FlashName_r_07_03},
    {1,     FlashMovies_r_07_end,     FlashName_r_07_end},
    {1,      FlashMovies_b_01_01,      FlashName_b_01_01},
    {1,     FlashMovies_b_01_end,     FlashName_b_01_end},
    {1,      FlashMovies_b_02_01,      FlashName_b_02_01},
    {1,     FlashMovies_b_02_end,     FlashName_b_02_end},
    {1,     FlashMovies_b_03_end,     FlashName_b_03_end},
    {1,     FlashMovies_b_04_end,     FlashName_b_04_end},
    {1,     FlashMovies_b_05_end,     FlashName_b_05_end},
    {1,     FlashMovies_b_06_end,     FlashName_b_06_end},
    {1,      FlashMovies_b_07_01,      FlashName_b_07_01},
    {1,      FlashMovies_b_07_02,      FlashName_b_07_02},
    {1,      FlashMovies_b_07_03,      FlashName_b_07_03},
    {1,      FlashMovies_b_07_04,      FlashName_b_07_04},
    {1,      FlashMovies_b_07_06,      FlashName_b_07_06},
    {1,      FlashMovies_b_07_07,      FlashName_b_07_07},
    {5,      FlashMovies_b_07_08,      FlashName_b_07_08},
    {4,      FlashMovies_b_07_09,      FlashName_b_07_09},
    {1,      FlashMovies_b_07_11,      FlashName_b_07_11},
    {1,     FlashMovies_b_07_end,     FlashName_b_07_end},
    {2,      FlashMovies_s_01_01,      FlashName_s_01_01},
    {1,      FlashMovies_catchE1,      FlashName_catchE1},
    {1,      FlashMovies_catchE2,      FlashName_catchE2},
    {1,      FlashMovies_catchE3,      FlashName_catchE3},
    {1,      FlashMovies_catchE4,      FlashName_catchE4},
    {1,      FlashMovies_catchE5,      FlashName_catchE5},
    {1,      FlashMovies_catchE6,      FlashName_catchE6},
    {1,      FlashMovies_catchE7,      FlashName_catchE7},
    {1,      FlashMovies_catchR1,      FlashName_catchR1},
    {1,      FlashMovies_catchR2,      FlashName_catchR2},
    {1,      FlashMovies_catchR3,      FlashName_catchR3},
    {1,      FlashMovies_catchR4,      FlashName_catchR4},
    {1,      FlashMovies_catchR5,      FlashName_catchR5},
    {1,      FlashMovies_catchR6,      FlashName_catchR6},
    {1,      FlashMovies_catchR7,      FlashName_catchR7},
    {1,      FlashMovies_catchB1,      FlashName_catchB1},
    {1,      FlashMovies_catchB2,      FlashName_catchB2},
    {1,      FlashMovies_catchB3,      FlashName_catchB3},
    {1,      FlashMovies_catchB4,      FlashName_catchB4},
    {1,      FlashMovies_catchB5,      FlashName_catchB5},
    {1,      FlashMovies_catchB6,      FlashName_catchB6},
    {1,      FlashMovies_catchB7,      FlashName_catchB7},
    {1,      FlashMovies_catchC1,      FlashName_catchC1},
    {1,      FlashMovies_showend,      FlashName_showend},
    {1, FlashMovies_test_r_01_01, FlashName_test_r_01_01},
    {1, FlashMovies_test_r_02_01, FlashName_test_r_02_01},
    {1, FlashMovies_test_r_04_01, FlashName_test_r_04_01},
    {1, FlashMovies_test_r_07_02, FlashName_test_r_07_02},
    {1, FlashMovies_test_b_07_08, FlashName_test_b_07_08},
};

s32 gSeqFlashSkip;

extern void func_020218ec(void);
extern s64  func_0203a444(void);
extern void func_0203a4e4(s64 tick);
extern void func_02041134(s32 trigger, s32 arg1, s32 arg2);
extern BOOL Savefile_AcquiredAllSecretReports(void);
extern void func_ov003_0208ec74(void);
extern void func_ov042_020824a0(void);
extern void ProcessOverlay_Result(void);
extern void ProcessOverlay_Save(void);
extern u32  OVERLAY_41_ID;

#ifdef REGION_USA
    #define SEQFLASH_OVL_RESULT    OVERLAY_44_ID
    #define SEQFLASH_OVL_STAFFROLL OVERLAY_42_ID
    #define SEQFLASH_OVL_SAVE      OVERLAY_43_ID
#else
    #define SEQFLASH_OVL_RESULT    OVERLAY_43_ID
    #define SEQFLASH_OVL_STAFFROLL OVERLAY_41_ID
    #define SEQFLASH_OVL_SAVE      OVERLAY_42_ID
#endif

static inline s32 SeqFlash_ClampBrightness(s32 brightness) {
    s32 value = brightness;

    if (brightness > 16) {
        value = 16;
    } else if (brightness < -16) {
        value = -16;
    }
    return value;
}

void SeqFlash_VBlank(void) {
    Flash_VBlank();
}

s32 SeqFlash_LeadsToBattle(void) {
    s32 value = gSaveData.flashSequence;
    if (value == 47) {
        return TRUE;
    }
    if (value == 48) {
        return TRUE;
    }
    return FALSE;
}

s32 SeqFlash_StartsFromWhite(void) {
    return gSaveData.flashSequence == 49;
}

void SeqFlash_InitDisplay(void) {
    Interrupts_ForceVBlank();
    GX_Init();
    s32 brightness = -16;
    if (SeqFlash_StartsFromWhite()) {
        brightness = 16;
    }
    GXx_SetMasterBrightness(&REG_MASTER_BRIGHT, brightness);
    GXx_SetMasterBrightness(&REG_MASTER_BRIGHT_SUB, brightness);
    Display_Init();
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = SeqFlash_ClampBrightness(brightness);
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = SeqFlash_ClampBrightness(brightness);
    Display_Commit();
    Interrupts_Init();
    DMA_Init(0x100);
    Interrupts_RegisterVBlankCallback(SeqFlash_VBlank, TRUE);
}

void SeqFlash_ClearVram(void) {
    GX_SetBankForLcdc(0x1FF);
    MI_CpuFill(0, (void*)0x6800000, 0xA4000);
    MI_CpuFill(0, (void*)0x6000000, 0x80000);
    MI_CpuFill(0, (void*)0x6200000, 0x20000);
    MI_CpuFill(0, (void*)0x6400000, 0x40000);
    MI_CpuFill(0, (void*)0x6600000, 0x20000);
    GX_DisableBankForLcdc();
    MI_CpuFill(0xC0, (void*)0x7000000, 0x400);
    MI_CpuFill(0, (void*)0x5000000, 0x400);
    MI_CpuFill(0xC0, (void*)0x7000400, 0x400);
    MI_CpuFill(0, (void*)0x5000400, 0x400);
}

void SeqFlash_DrawCallback(void) {
    SndMgr_Finalize();
#ifdef REGION_USA
    if (SystemStatusFlags.unk_03) {
        func_020218ec();
    }
    SystemStatusFlags.unk_03 = FALSE;
#endif
}

void SeqFlash_Init(SeqFlashState* state) {
    const char* sequence = SeqFlash_Sequence;

    state = Mem_AllocHeapTail(&gDebugHeap, sizeof(SeqFlashState));
    Mem_SetSequence(&gDebugHeap, state, sequence);
    MainOvlDisp_SetCbArg(state);
    ResourceMgr_ReinitManagers(&state->resources);
    if (gSaveData.flashExitMode == 2) {
        if (Savefile_AcquiredAllSecretReports()) {
            gSaveData.flashSequence = 54;
        } else {
            gSaveData.flashSequence = 52;
        }
    }
    MainOvlDisp_NextProcessStage();
    SeqFlash_InitDisplay();
    SeqFlash_ClearVram();
    gSeqFlashSkip = 0;
}

void SeqFlash_SetupBattle(s32 selector) {
    gSaveData.unk_3130         = 0;
    gSaveData.unk_3134         = selector;
    gSaveData.unk_3142         = 0;
    gSaveData.battleDifficulty = gSaveData.playerStats.difficulty;
    gSaveData.unk_3138         = 0;
    gSaveData.unk_3140         = 1;
    gSaveData.battleDropRate   = gSaveData.playerStats.dropRate;
}

BOOL SeqFlash_ShouldSkip(void) {
    return FALSE;
}

void SeqFlash_Update(SeqFlashState* state) {
    s32               count;
    const FlashMovie* movies;

    u32 index = gSaveData.flashSequence;
    if (index >= FLASH_SEQUENCE_COUNT) {
        index = 0;
    }

    movies = FlashSequences[index].movies;
    count  = FlashSequences[index].count;

    if (SeqFlash_ShouldSkip()) {
        count = 0;
    }
    Interrupts_ForceVBlank();
    for (s32 i = 0; i < count; i++) {
        Flash_Init();
        Flash_SetDrawCallback(SeqFlash_DrawCallback);
        Flash_Load(movies[i].path);
        if (i == 0) {
            GXx_SetMasterBrightness(&REG_MASTER_BRIGHT, 0);
            GXx_SetMasterBrightness(&REG_MASTER_BRIGHT_SUB, 0);
        }
        if (movies[i].bgm != FLASH_BGM_STOP) {
            if (movies[i].bgm == ADX_E01) {
                CriSndMgr_SetLpFlg(FALSE);
                CriSndMgr_PlayFile(ADX_E01);
            } else if (movies[i].bgm != FLASH_BGM_KEEP) {
                CriSndMgr_PlayFile(movies[i].bgm);
            }
        } else {
            CriSndMgr_Stop(ADX_E_01_01);
        }
#ifdef REGION_USA
        Interrupts_ForceVBlank();
#endif
        Flash_Start();
        while (gSeqFlashSkip == 0 && Flash_IsPlaying()) {
            Flash_Update();
            Flash_Draw();
            if (SeqFlash_ShouldSkip()) {
                gSeqFlashSkip = 1;
            }
            if (Input_IsSystemHingeClosed()) {
                s64 tick = func_0203a444();
                func_02041134(12, 0, 0);
                func_0203a4e4(tick);
            }
        }
        Flash_ClearDrawCallback();
        Flash_Shutdown();
        SeqFlash_ClearVram();
        if (gSeqFlashSkip != 0) {
            i = count;
        }
    }
    Interrupts_RegisterVBlankCallback(NULL, TRUE);

    s32 brightness = -16;
    if (SeqFlash_LeadsToBattle()) {
        brightness = 16;
    }
    GXx_SetMasterBrightness(&REG_MASTER_BRIGHT, brightness);
    GXx_SetMasterBrightness(&REG_MASTER_BRIGHT_SUB, brightness);
    Display_Init();
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = SeqFlash_ClampBrightness(brightness);
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = SeqFlash_ClampBrightness(brightness);
    Display_Commit();
    CriSndMgr_Stop(ADX_E_01_01);
    CriSndMgr_SetLpFlg(TRUE);

    if (gSaveData.flashSequence == 47) {
        OverlayTag tag;
        gSaveData.unk_24B4          = 0;
        gSaveData.unk_24B8          = 1;
        gSaveData.currentStoryEvent = 778;
        SeqFlash_SetupBattle(46);
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_3_ID, func_ov003_0208ec74, NULL, PROCESS_STAGE_INIT);
    } else if (gSaveData.flashSequence == 48) {
        OverlayTag tag;
        SeqFlash_SetupBattle(47);
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_3_ID, func_ov003_0208ec74, NULL, PROCESS_STAGE_INIT);
    } else if (gSaveData.flashSequence == 49) {
        OverlayTag tag;
        gSaveData.unk_24B8          = 1;
        gSaveData.currentStoryEvent = 779;
        MainOvlDisp_ReplaceTop(&tag, (s32)&SEQFLASH_OVL_RESULT, ProcessOverlay_Result, NULL, PROCESS_STAGE_INIT);
    } else if (gSaveData.flashSequence == 51) {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, (s32)&SEQFLASH_OVL_STAFFROLL, func_ov042_020824a0, NULL, PROCESS_STAGE_INIT);
    } else if (gSaveData.flashExitMode == 3) {
        OverlayTag tag;
        MainOvlDisp_Pop(&tag);
    } else if (gSaveData.flashExitMode == 1) {
        OverlayTag tag;
#ifndef REGION_USA
        gSaveData.chapter++;
#endif
        MainOvlDisp_ReplaceTop(&tag, (s32)&SEQFLASH_OVL_SAVE, ProcessOverlay_Save, NULL, PROCESS_STAGE_INIT);
        gSaveData.unk_1AB4 |= 2;
    } else if (gSaveData.flashExitMode == 2) {
        OverlayTag tag;
#ifndef REGION_USA
        gSaveData.chapter = 21;
#endif
        MainOvlDisp_ReplaceTop(&tag, (s32)&SEQFLASH_OVL_SAVE, ProcessOverlay_Save, NULL, PROCESS_STAGE_INIT);
        gSaveData.unk_1AB4 |= 0x82;
    } else if (gSaveData.flashExitMode == 4) {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, ProcessOverlay_FlashSel, NULL, PROCESS_STAGE_INIT);
    } else {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, func_ov030_020ae92c, NULL, PROCESS_STAGE_INIT);
    }
}

void SeqFlash_Destroy(SeqFlashState* state) {
    ResourceMgr_ReinitManagers(NULL);
    Mem_Free(&gDebugHeap, state);
}

void ProcessOverlay_Flash(SeqFlashState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        SeqFlash_Destroy(state);
    } else {
        OvlProc_Flash.funcs[stage](state);
    }
}
