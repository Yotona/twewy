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
#include "Field/FlashPlayer.h"
#include "Field/FlashRegion.h"
#include "Save.h"
#include "SndMgr.h"
#include "common_data.h"
#include <nitro/gx.h>
#include <nitro/mi.h>

typedef struct {
    /* 0x0 */ const char*    path;
    /* 0x4 */ CriSndMgrAdxId bgm;
} FlashMovie; // Size: 0x8

typedef struct {
    /* 0x0 */ s32               count;
    /* 0x4 */ const FlashMovie* movies;
    /* 0x8 */ const char*       name;
} FlashSequence; // Size: 0xC

#define FLASH_SEQUENCE_COUNT 83

// Movie bgm values past the real track ids.
#define FLASH_BGM_STOP (ADX_COUNT)
#define FLASH_BGM_KEEP (ADX_COUNT + 1)

typedef struct {
    /* 0x00000 */ ResourceManager resources;
} SeqFlashState; // Size: 0x11580

void func_ov030_020d4a14(SeqFlashState* state);
void func_ov030_020d4b14(SeqFlashState* state);
void func_ov030_020d4f50(SeqFlashState* state);

char        data_ov030_02109430[] = "Seq_Flash()";
const char* data_ov030_0210916c   = data_ov030_02109430;

char data_ov030_02109318[] = "catchE6";

char data_ov030_02109320[] = "catchB4";

char data_ov030_02109180[] = "catchE1";

char data_ov030_02109240[] = "b_07_09";

char data_ov030_02109248[] = "r_07_02";

char data_ov030_02109238[] = "b_07_08";

char data_ov030_021092c8[] = "r_04_01";

char data_ov030_021092c0[] = "catchR5";

char data_ov030_021092e0[] = "catchB5";

char data_ov030_021092d0[] = "catchR4";

char data_ov030_021092b0[] = "b_07_01";

char data_ov030_021091b0[] = "catchE2";

char data_ov030_021092b8[] = "showend";

char data_ov030_021091e8[] = "catchE3";

char data_ov030_02109300[] = "catchB6";

char data_ov030_02109208[] = "b_02_01";

char data_ov030_021092f8[] = "catchE7";

char data_ov030_021091f0[] = "e_01_05";

char data_ov030_02109308[] = "r_02_03";

char data_ov030_02109218[] = "catchR3";

char data_ov030_02109210[] = "r_05_01";

char data_ov030_02109220[] = "catchC1";

char data_ov030_02109310[] = "catchE5";

char data_ov030_02109228[] = "r_07_03";

char data_ov030_021091a0[] = "r_02_01";

char data_ov030_02109198[] = "r_01_02";

char data_ov030_02109200[] = "e_01_03";

char data_ov030_02109230[] = "b_07_02";

char data_ov030_02109258[] = "s_01_01";

char data_ov030_02109190[] = "e_07_02";

char data_ov030_02109260[] = "b_07_11";

char data_ov030_02109178[] = "r_01_01";

char data_ov030_021091b8[] = "e_04_01";

char data_ov030_021092f0[] = "catchR2";

char data_ov030_02109268[] = "e_03_01";

char data_ov030_021091c0[] = "b_07_07";

char data_ov030_02109270[] = "catchB1";

char data_ov030_02109188[] = "e_07_01";

char data_ov030_02109278[] = "r_07_01";

char data_ov030_021091c8[] = "e_03_02";

char data_ov030_02109170[] = "r_02_02";

char data_ov030_02109250[] = "catchB3";

char data_ov030_021091f8[] = "b_07_06";

char data_ov030_02109280[] = "b_01_01";

char data_ov030_02109288[] = "b_07_04";

char data_ov030_021092e8[] = "e_01_06";

char data_ov030_02109290[] = "catchR7";

char data_ov030_021091d8[] = "catchB2";

char data_ov030_021091d0[] = "e_01_01";

char data_ov030_021091a8[] = "e_01_04";

char data_ov030_02109298[] = "b_07_03";

char data_ov030_021091e0[] = "catchE4";

char data_ov030_021092a0[] = "catchR6";

char data_ov030_021092d8[] = "catchR1";

char data_ov030_021092a8[] = "catchB7";

char data_ov030_02109340[] = "r_06_end";

char data_ov030_0210934c[] = "r_07_end";

char data_ov030_02109418[] = "r_03_end";

char data_ov030_02109358[] = "b_02_end";

char data_ov030_02109364[] = "b_01_end";

char data_ov030_02109370[] = "b_03_end";

char data_ov030_0210940c[] = "r_01_end";

char data_ov030_0210937c[] = "b_04_end";

char data_ov030_02109388[] = "b_05_end";

char data_ov030_02109394[] = "b_06_end";

char data_ov030_02109400[] = "r_02_end";

char data_ov030_021093a0[] = "e_01_end";

char data_ov030_021093ac[] = "e_02_end";

char data_ov030_021093b8[] = "e_03_end";

char data_ov030_02109328[] = "r_05_end";

char data_ov030_021093c4[] = "e_04_end";

char data_ov030_021093d0[] = "e_05_end";

char data_ov030_021093dc[] = "e_06_end";

char data_ov030_02109334[] = "r_04_end";

char data_ov030_021093e8[] = "b_07_end";

char data_ov030_021093f4[] = "e_07_end";

char data_ov030_02109424[] = "EndingFlash";

char data_ov030_0210943c[] = "test_b_07_08";

char data_ov030_0210945c[] = "test_r_01_01";

char data_ov030_0210946c[] = "test_r_07_02";

char data_ov030_0210947c[] = "test_r_02_01";

char data_ov030_0210944c[] = "OpeningFlash";

char data_ov030_0210948c[] = "test_r_04_01";

char data_ov030_0210949c[] = "Apl_Kit/op1.fds";

char data_ov030_021094ac[] = "Apl_Kit/op2.fds";

char data_ov030_021094bc[] = "Apl_Kit/wait.fds";

char data_ov030_021094d0[] = "Apl_Kit/movie1.fds";

char data_ov030_021094e4[] = "Apl_Kit/movie4.fds";

char data_ov030_021094f8[] = "Apl_Kit/movie0.fds";

char data_ov030_0210964c[] = "Apl_Kit/b_07_07.fds";

char data_ov030_02109660[] = "Apl_Kit/catchE4.fds";

char data_ov030_02109674[] = "Apl_Kit/e_04_01.fds";

char data_ov030_02109688[] = "Apl_Kit/b_07_08.fds";

char data_ov030_0210969c[] = "Apl_Kit/catchE5.fds";

char data_ov030_021096b0[] = "Apl_Kit/b_07_09.fds";

char data_ov030_021096c4[] = "Apl_Kit/catchE6.fds";

char data_ov030_021096d8[] = "Apl_Kit/b_07_10.fds";

char data_ov030_021096ec[] = "Apl_Kit/b_07_12.fds";

char data_ov030_02109700[] = "Apl_Kit/catchE7.fds";

char data_ov030_02109714[] = "Apl_Kit/e_07_01.fds";

char data_ov030_02109728[] = "Apl_Kit/b_07_11.fds";

char data_ov030_0210973c[] = "Apl_Kit/catchR1.fds";

char data_ov030_02109750[] = "Apl_Kit/r_01_01.fds";

char data_ov030_02109764[] = "Apl_Kit/catchC1.fds";

char data_ov030_02109778[] = "Apl_Kit/r_01_02.fds";

char data_ov030_0210978c[] = "Apl_Kit/s_01_01.fds";

char data_ov030_021097a0[] = "Apl_Kit/catchR2.fds";

char data_ov030_021097b4[] = "Apl_Kit/r_02_01.fds";

char data_ov030_021097c8[] = "Apl_Kit/showend.fds";

char data_ov030_021097dc[] = "Apl_Kit/r_02_02.fds";

char data_ov030_021097f0[] = "Apl_Kit/r_02_03.fds";

char data_ov030_02109804[] = "Apl_Kit/catchR3.fds";

char data_ov030_02109818[] = "Apl_Kit/catchR4.fds";

char data_ov030_0210982c[] = "Apl_Kit/r_04_01.fds";

char data_ov030_02109840[] = "Apl_Kit/catchR5.fds";

char data_ov030_02109854[] = "Apl_Kit/r_05_01.fds";

char data_ov030_02109868[] = "Apl_Kit/catchR6.fds";

char data_ov030_0210987c[] = "Apl_Kit/catchR7.fds";

char data_ov030_02109890[] = "Apl_Kit/r_07_01.fds";

char data_ov030_021098a4[] = "Apl_Kit/r_07_02.fds";

char data_ov030_021098b8[] = "Apl_Kit/r_07_03.fds";

char data_ov030_021098cc[] = "Apl_Kit/catchB1.fds";

char data_ov030_021098e0[] = "Apl_Kit/b_01_01.fds";

char data_ov030_021098f4[] = "Apl_Kit/catchB2.fds";

char data_ov030_02109908[] = "Apl_Kit/b_02_01.fds";

char data_ov030_0210991c[] = "Apl_Kit/catchB3.fds";

char data_ov030_02109930[] = "Apl_Kit/catchB4.fds";

char data_ov030_02109944[] = "Apl_Kit/catchB5.fds";

char data_ov030_02109958[] = "Apl_Kit/catchB6.fds";

char data_ov030_0210950c[] = "Apl_Kit/catchE1.fds";

char data_ov030_02109520[] = "Apl_Kit/e_01_01.fds";

char data_ov030_02109534[] = "Apl_Kit/catchB7.fds";

char data_ov030_02109548[] = "Apl_Kit/e_01_03.fds";

char data_ov030_0210955c[] = "Apl_Kit/b_07_01.fds";

char data_ov030_02109570[] = "Apl_Kit/e_01_05.fds";

char data_ov030_02109584[] = "Apl_Kit/b_07_02.fds";

char data_ov030_02109598[] = "Apl_Kit/e_01_06.fds";

char data_ov030_021095ac[] = "Apl_Kit/b_07_03.fds";

char data_ov030_021095c0[] = "Apl_Kit/catchE2.fds";

char data_ov030_021095d4[] = "Apl_Kit/e_02_01.fds";

char data_ov030_021095e8[] = "Apl_Kit/e_03_01.fds";

char data_ov030_021095fc[] = "Apl_Kit/b_07_04.fds";

char data_ov030_02109610[] = "Apl_Kit/catchE3.fds";

char data_ov030_02109624[] = "Apl_Kit/b_07_06.fds";

char data_ov030_02109638[] = "Apl_Kit/e_03_02.fds";

char data_ov030_0210996c[] = "Apl_Kit/b_06_end.fds";

char data_ov030_02109984[] = "Apl_Kit/e_01_02a.fds";

char data_ov030_0210999c[] = "Apl_Kit/e_01_02b.fds";

char data_ov030_021099b4[] = "Apl_Kit/e_01_02c.fds";

char data_ov030_021099cc[] = "Apl_Kit/e_01_end.fds";

char data_ov030_021099e4[] = "Apl_Kit/e_02_end.fds";

char data_ov030_021099fc[] = "Apl_Kit/e_03_end.fds";

char data_ov030_02109a14[] = "Apl_Kit/e_04_end.fds";

char data_ov030_02109a2c[] = "Apl_Kit/e_05_end.fds";

char data_ov030_02109a44[] = "Apl_Kit/e_01_02d.fds";

char data_ov030_02109a5c[] = "Apl_Kit/e_06_end.fds";

char data_ov030_02109a74[] = "Apl_Kit/e_07_end.fds";

char data_ov030_02109a8c[] = "Apl_Kit/b_07_end.fds";

char data_ov030_02109aa4[] = "Apl_Kit/r_01_end.fds";

char data_ov030_02109abc[] = "Apl_Kit/r_02_end.fds";

char data_ov030_02109ad4[] = "Apl_Kit/r_03_end.fds";

char data_ov030_02109aec[] = "Apl_Kit/r_04_end.fds";

char data_ov030_02109b04[] = "Apl_Kit/r_05_end.fds";

char data_ov030_02109b1c[] = "Apl_Kit/r_06_end.fds";

char data_ov030_02109b34[] = "Apl_Kit/r_07_end.fds";

char data_ov030_02109b4c[] = "Apl_Kit/b_01_end.fds";

char data_ov030_02109b64[] = "Apl_Kit/b_02_end.fds";

char data_ov030_02109b7c[] = "Apl_Kit/b_03_end.fds";

char data_ov030_02109b94[] = "Apl_Kit/b_04_end.fds";

char data_ov030_02109bac[] = "Apl_Kit/b_05_end.fds";

const FlashMovie data_ov030_020fd848[1] = {
    {data_ov030_02109638, ADX_E_03_02},
};

const FlashMovie data_ov030_020fd850[1] = {
    {data_ov030_02109624, ADX_B_07_06},
};

const FlashMovie data_ov030_020fd7c8[1] = {
    {data_ov030_02109b34, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd858[1] = {
    {data_ov030_0210964c, ADX_B_07_07},
};

const FlashMovie data_ov030_020fd7d0[1] = {
    {data_ov030_02109548, ADX_E_01_03},
};

const FlashMovie data_ov030_020fd7a8[1] = {
    {data_ov030_02109890, ADX_R_07_01},
};

const FlashMovie data_ov030_020fd768[1] = {
    {data_ov030_02109b1c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd860[1] = {
    {data_ov030_02109840, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd738[1] = {
    {data_ov030_0210987c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd798[1] = {
    {data_ov030_021095e8, ADX_E_03_01},
};

const FlashMovie data_ov030_020fd880[1] = {
    {data_ov030_0210955c, ADX_B_07_01},
};

const FlashMovie data_ov030_020fd740[1] = {
    {data_ov030_0210996c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd868[1] = {
    {data_ov030_02109674, ADX_E_04_01},
};

const FlashMovie data_ov030_020fd870[1] = {
    {data_ov030_02109a14, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd7e0[1] = {
    {data_ov030_02109a5c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd748[1] = {
    {data_ov030_021098f4, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd8d0[1] = {
    {data_ov030_02109584, ADX_B_07_02},
};

const FlashMovie data_ov030_020fd7e8[1] = {
    {data_ov030_02109b64, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd7f0[1] = {
    {data_ov030_021097a0, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd728[1] = {
    {data_ov030_0210991c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd730[1] = {
    {data_ov030_02109930, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd770[1] = {
    {data_ov030_02109aa4, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd940[1] = {
    {data_ov030_02109818, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd930[1] = {
    {data_ov030_02109688, ADX_B_07_08},
};

const FlashMovie data_ov030_020fd920[1] = {
    {data_ov030_02109804, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd918[1] = {
    {data_ov030_021098a4, ADX_R_07_02},
};

const FlashMovie data_ov030_020fd908[1] = {
    {data_ov030_021097f0, ADX_R_02_03},
};

const FlashMovie data_ov030_020fd8f8[1] = {
    {data_ov030_021097c8, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd8e8[1] = {
    {data_ov030_021095fc, ADX_B_07_04},
};

const FlashMovie data_ov030_020fd8e0[1] = {
    {data_ov030_02109778, ADX_E_01_06},
};

const FlashMovie data_ov030_020fd7f8[1] = {
    {data_ov030_02109750, ADX_R_01_01},
};

const FlashMovie data_ov030_020fd8c8[1] = {
    {data_ov030_0210973c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd8b8[1] = {
    {data_ov030_02109570, ADX_E_01_05},
};

const FlashMovie data_ov030_020fd8b0[1] = {
    {data_ov030_02109598, ADX_E_01_06},
};

const FlashMovie data_ov030_020fd8a0[1] = {
    {data_ov030_02109a74, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd898[1] = {
    {data_ov030_02109a8c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd778[1] = {
    {data_ov030_0210982c, ADX_R_04_01},
};

const FlashMovie data_ov030_020fd840[1] = {
    {data_ov030_021098cc, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd7b8[1] = {
    {data_ov030_02109868, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd7a0[1] = {
    {data_ov030_02109700, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd800[1] = {
    {data_ov030_021095ac, ADX_B_07_03},
};

const FlashMovie data_ov030_020fd808[1] = {
    {data_ov030_021096c4, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd810[1] = {
    {data_ov030_02109750, ADX_R_01_01},
};

const FlashMovie data_ov030_020fd760[1] = {
    {data_ov030_02109854, ADX_R_05_01},
};

const FlashMovie data_ov030_020fd818[1] = {
    {data_ov030_021097b4, ADX_R_02_01},
};

const FlashMovie data_ov030_020fd780[1] = {
    {data_ov030_02109abc, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd7b0[1] = {
    {data_ov030_02109b4c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd7d8[1] = {
    {data_ov030_02109b04, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd788[1] = {
    {data_ov030_02109534, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd790[1] = {
    {data_ov030_02109b94, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd878[1] = {
    {data_ov030_0210969c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd720[1] = {
    {data_ov030_021098e0, ADX_E_01_06},
};

const FlashMovie data_ov030_020fd750[1] = {
    {data_ov030_02109958, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd718[1] = {
    {data_ov030_02109944, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd888[1] = {
    {data_ov030_02109714, ADX_E_07_01},
};

const FlashMovie data_ov030_020fd928[1] = {
    {data_ov030_021095c0, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd910[1] = {
    {data_ov030_02109ad4, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd8f0[1] = {
    {data_ov030_02109a2c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd8d8[1] = {
    {data_ov030_02109764, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd8c0[1] = {
    {data_ov030_02109908, ADX_B_02_01},
};

const FlashMovie data_ov030_020fd8a8[1] = {
    {data_ov030_02109728, ADX_B_07_11},
};

const FlashMovie data_ov030_020fd890[1] = {
    {data_ov030_021094e4, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd7c0[1] = {
    {data_ov030_021099fc, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd900[1] = {
    {data_ov030_021097dc, ADX_R_02_02},
};

const FlashMovie data_ov030_020fd938[1] = {
    {data_ov030_02109b7c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd758[1] = {
    {data_ov030_02109660, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd838[1] = {
    {data_ov030_02109610, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd830[1] = {
    {data_ov030_0210950c, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd828[1] = {
    {data_ov030_021099cc, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd820[1] = {
    {data_ov030_02109aec, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd948[1] = {
    {data_ov030_02109bac, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd95c[2] = {
    {data_ov030_0210949c,        ADX_E01},
    {data_ov030_021094ac, FLASH_BGM_KEEP},
};

const FlashMovie data_ov030_020fd96c[2] = {
    {data_ov030_021094bc, FLASH_BGM_STOP},
    {data_ov030_02109984,   ADX_E_01_02A},
};

const FlashMovie data_ov030_020fd97c[2] = {
    {data_ov030_021094f8, FLASH_BGM_STOP},
    {data_ov030_021094d0,     ADX_ENDING},
};

const FlashMovie data_ov030_020fd98c[2] = {
    {data_ov030_0210978c, ADX_S_01_01},
    {data_ov030_02109728, ADX_B_07_11},
};

const FlashMovie data_ov030_020fd99c[2] = {
    {data_ov030_02109750, ADX_R_01_01},
    {data_ov030_021097b4, ADX_R_02_01},
};

const FlashMovie data_ov030_020fd9ac[2] = {
    {data_ov030_02109750, ADX_R_01_01},
    {data_ov030_0210982c, ADX_R_04_01},
};

const FlashMovie data_ov030_020fd9bc[2] = {
    {data_ov030_021098b8,  ADX_R_07_03},
    {data_ov030_02109984, ADX_E_01_02A},
};

const FlashMovie data_ov030_020fd9e4[3] = {
    {data_ov030_02109750, ADX_R_01_01},
    {data_ov030_0210982c, ADX_R_04_01},
    {data_ov030_021098a4, ADX_R_07_02},
};

const FlashMovie data_ov030_020fd9cc[3] = {
    {data_ov030_021095d4,    ADX_E_02_01},
    {data_ov030_021095e8,    ADX_E_03_01},
    {data_ov030_021099e4, FLASH_BGM_STOP},
};

const FlashMovie data_ov030_020fd9fc[4] = {
    {data_ov030_02109520,  ADX_E_01_01},
    {data_ov030_02109984, ADX_E_01_02A},
    {data_ov030_0210999c, ADX_E_01_02B},
    {data_ov030_021099b4, ADX_E_01_02C},
};

const FlashMovie data_ov030_020fda1c[4] = {
    {data_ov030_021096b0,  ADX_B_07_09},
    {data_ov030_02109a44, ADX_E_01_02A},
    {data_ov030_021096d8,  ADX_B_07_10},
    {data_ov030_021096ec,  ADX_B_07_12},
};

const FlashMovie data_ov030_020fda3c[5] = {
    {data_ov030_02109750, ADX_R_01_01},
    {data_ov030_0210982c, ADX_R_04_01},
    {data_ov030_021098a4, ADX_R_07_02},
    {data_ov030_02109688, ADX_B_07_08},
    {data_ov030_021097b4, ADX_R_02_01},
};

const OverlayProcess data_ov030_020fd950 = {
    .init = func_ov030_020d4a14,
    .main = func_ov030_020d4b14,
    .exit = func_ov030_020d4f50,
};

const FlashSequence data_ov030_020fda64[FLASH_SEQUENCE_COUNT] = {
    {2, data_ov030_020fd97c, data_ov030_0210944c},
    {1, data_ov030_020fd890, data_ov030_02109424},
    {4, data_ov030_020fd9fc, data_ov030_021091d0},
    {1, data_ov030_020fd7d0, data_ov030_02109200},
    {2, data_ov030_020fd95c, data_ov030_021091a8},
    {1, data_ov030_020fd8b8, data_ov030_021091f0},
    {1, data_ov030_020fd8b0, data_ov030_021092e8},
    {1, data_ov030_020fd828, data_ov030_021093a0},
    {3, data_ov030_020fd9cc, data_ov030_021093ac},
    {1, data_ov030_020fd798, data_ov030_02109268},
    {1, data_ov030_020fd848, data_ov030_021091c8},
    {1, data_ov030_020fd7c0, data_ov030_021093b8},
    {1, data_ov030_020fd868, data_ov030_021091b8},
    {1, data_ov030_020fd870, data_ov030_021093c4},
    {1, data_ov030_020fd8f0, data_ov030_021093d0},
    {1, data_ov030_020fd7e0, data_ov030_021093dc},
    {1, data_ov030_020fd888, data_ov030_02109188},
    {2, data_ov030_020fd96c, data_ov030_02109190},
    {1, data_ov030_020fd8a0, data_ov030_021093f4},
    {1, data_ov030_020fd7f8, data_ov030_02109178},
    {1, data_ov030_020fd8e0, data_ov030_02109198},
    {1, data_ov030_020fd770, data_ov030_0210940c},
    {2, data_ov030_020fd99c, data_ov030_021091a0},
    {1, data_ov030_020fd900, data_ov030_02109170},
    {1, data_ov030_020fd908, data_ov030_02109308},
    {1, data_ov030_020fd780, data_ov030_02109400},
    {1, data_ov030_020fd910, data_ov030_02109418},
    {2, data_ov030_020fd9ac, data_ov030_021092c8},
    {1, data_ov030_020fd820, data_ov030_02109334},
    {1, data_ov030_020fd760, data_ov030_02109210},
    {1, data_ov030_020fd7d8, data_ov030_02109328},
    {1, data_ov030_020fd768, data_ov030_02109340},
    {1, data_ov030_020fd7a8, data_ov030_02109278},
    {3, data_ov030_020fd9e4, data_ov030_02109248},
    {2, data_ov030_020fd9bc, data_ov030_02109228},
    {1, data_ov030_020fd7c8, data_ov030_0210934c},
    {1, data_ov030_020fd720, data_ov030_02109280},
    {1, data_ov030_020fd7b0, data_ov030_02109364},
    {1, data_ov030_020fd8c0, data_ov030_02109208},
    {1, data_ov030_020fd7e8, data_ov030_02109358},
    {1, data_ov030_020fd938, data_ov030_02109370},
    {1, data_ov030_020fd790, data_ov030_0210937c},
    {1, data_ov030_020fd948, data_ov030_02109388},
    {1, data_ov030_020fd740, data_ov030_02109394},
    {1, data_ov030_020fd880, data_ov030_021092b0},
    {1, data_ov030_020fd8d0, data_ov030_02109230},
    {1, data_ov030_020fd800, data_ov030_02109298},
    {1, data_ov030_020fd8e8, data_ov030_02109288},
    {1, data_ov030_020fd850, data_ov030_021091f8},
    {1, data_ov030_020fd858, data_ov030_021091c0},
    {5, data_ov030_020fda3c, data_ov030_02109238},
    {4, data_ov030_020fda1c, data_ov030_02109240},
    {1, data_ov030_020fd8a8, data_ov030_02109260},
    {1, data_ov030_020fd898, data_ov030_021093e8},
    {2, data_ov030_020fd98c, data_ov030_02109258},
    {1, data_ov030_020fd830, data_ov030_02109180},
    {1, data_ov030_020fd928, data_ov030_021091b0},
    {1, data_ov030_020fd838, data_ov030_021091e8},
    {1, data_ov030_020fd758, data_ov030_021091e0},
    {1, data_ov030_020fd878, data_ov030_02109310},
    {1, data_ov030_020fd808, data_ov030_02109318},
    {1, data_ov030_020fd7a0, data_ov030_021092f8},
    {1, data_ov030_020fd8c8, data_ov030_021092d8},
    {1, data_ov030_020fd7f0, data_ov030_021092f0},
    {1, data_ov030_020fd920, data_ov030_02109218},
    {1, data_ov030_020fd940, data_ov030_021092d0},
    {1, data_ov030_020fd860, data_ov030_021092c0},
    {1, data_ov030_020fd7b8, data_ov030_021092a0},
    {1, data_ov030_020fd738, data_ov030_02109290},
    {1, data_ov030_020fd840, data_ov030_02109270},
    {1, data_ov030_020fd748, data_ov030_021091d8},
    {1, data_ov030_020fd728, data_ov030_02109250},
    {1, data_ov030_020fd730, data_ov030_02109320},
    {1, data_ov030_020fd718, data_ov030_021092e0},
    {1, data_ov030_020fd750, data_ov030_02109300},
    {1, data_ov030_020fd788, data_ov030_021092a8},
    {1, data_ov030_020fd8d8, data_ov030_02109220},
    {1, data_ov030_020fd8f8, data_ov030_021092b8},
    {1, data_ov030_020fd810, data_ov030_0210945c},
    {1, data_ov030_020fd818, data_ov030_0210947c},
    {1, data_ov030_020fd778, data_ov030_0210948c},
    {1, data_ov030_020fd918, data_ov030_0210946c},
    {1, data_ov030_020fd930, data_ov030_0210943c},
};

s32 data_ov030_02109dec;

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

// JP has no counterpart to USA's ov036, so every later overlay number is one lower there.
#ifdef REGION_USA
    #define SEQFLASH_OVL_RESULT    OVERLAY_44_ID
    #define SEQFLASH_OVL_STAFFROLL OVERLAY_42_ID
    #define SEQFLASH_OVL_SAVE      OVERLAY_43_ID
#else
    #define SEQFLASH_OVL_RESULT    OVERLAY_43_ID
    #define SEQFLASH_OVL_STAFFROLL OVERLAY_41_ID
    #define SEQFLASH_OVL_SAVE      OVERLAY_42_ID
#endif
extern void func_ov030_020ae92c(void);
extern void func_ov030_020d5370(void);

static inline s32 SeqFlash_ClampBrightness(s32 brightness) {
    s32 value = brightness;

    if (brightness > 16) {
        value = 16;
    } else if (brightness < -16) {
        value = -16;
    }
    return value;
}

void func_ov030_020d4830(void) {
    func_ov030_020d65b4();
}

s32 func_ov030_020d483c(void) {
    s32 value = gSaveData.unk_3124;
    if (value == 47) {
        return TRUE;
    }
    if (value == 48) {
        return TRUE;
    }
    return FALSE;
}

s32 func_ov030_020d4864(void) {
    return gSaveData.unk_3124 == 49;
}

void func_ov030_020d4880(void) {
    Interrupts_ForceVBlank();
    GX_Init();
    s32 brightness = -16;
    if (func_ov030_020d4864()) {
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
    Interrupts_RegisterVBlankCallback(func_ov030_020d4830, TRUE);
}

void func_ov030_020d492c(void) {
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

void func_ov030_020d49dc(void) {
    SndMgr_Finalize();
#ifdef REGION_USA
    if (SystemStatusFlags.unk_03) {
        func_020218ec();
    }
    SystemStatusFlags.unk_03 = FALSE;
#endif
}

void func_ov030_020d4a14(SeqFlashState* state) {
    const char* sequence = data_ov030_0210916c;

    state = Mem_AllocHeapTail(&gDebugHeap, sizeof(SeqFlashState));
    Mem_SetSequence(&gDebugHeap, state, sequence);
    MainOvlDisp_SetCbArg(state);
    ResourceMgr_ReinitManagers(&state->resources);
    if (gSaveData.unk_3128 == 2) {
        if (Savefile_AcquiredAllSecretReports()) {
            gSaveData.unk_3124 = 54;
        } else {
            gSaveData.unk_3124 = 52;
        }
    }
    MainOvlDisp_NextProcessStage();
    func_ov030_020d4880();
    func_ov030_020d492c();
    data_ov030_02109dec = 0;
}

void func_ov030_020d4ab8(s32 selector) {
    gSaveData.unk_3130         = 0;
    gSaveData.unk_3134         = selector;
    gSaveData.unk_3142         = 0;
    gSaveData.battleDifficulty = gSaveData.playerStats.difficulty;
    gSaveData.unk_3138         = 0;
    gSaveData.unk_3140         = 1;
    gSaveData.battleDropRate   = gSaveData.playerStats.dropRate;
}

BOOL func_ov030_020d4b0c(void) {
    return FALSE;
}

void func_ov030_020d4b14(SeqFlashState* state) {
    s32               count;
    const FlashMovie* movies;

    u32 index = gSaveData.unk_3124;
    if (index >= FLASH_SEQUENCE_COUNT) {
        index = 0;
    }

    movies = data_ov030_020fda64[index].movies;
    count  = data_ov030_020fda64[index].count;

    if (func_ov030_020d4b0c()) {
        count = 0;
    }
    Interrupts_ForceVBlank();
    for (s32 i = 0; i < count; i++) {
        func_ov030_020d63c8();
        func_ov030_020d6610(func_ov030_020d49dc);
        func_ov030_020d6548(movies[i].path);
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
        func_ov030_020d65e0();
        while (data_ov030_02109dec == 0 && func_ov030_020d65f8()) {
            func_ov030_020d6584();
            func_ov030_020d659c();
            if (func_ov030_020d4b0c()) {
                data_ov030_02109dec = 1;
            }
            if (Input_IsSystemHingeClosed()) {
                s64 tick = func_0203a444();
                func_02041134(12, 0, 0);
                func_0203a4e4(tick);
            }
        }
        func_ov030_020d662c();
        func_ov030_020d6470();
        func_ov030_020d492c();
        if (data_ov030_02109dec != 0) {
            i = count;
        }
    }
    Interrupts_RegisterVBlankCallback(NULL, TRUE);

    s32 brightness = -16;
    if (func_ov030_020d483c()) {
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

    if (gSaveData.unk_3124 == 47) {
        OverlayTag tag;
        gSaveData.unk_24B4          = 0;
        gSaveData.unk_24B8          = 1;
        gSaveData.currentStoryEvent = 778;
        func_ov030_020d4ab8(46);
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_3_ID, func_ov003_0208ec74, NULL, PROCESS_STAGE_INIT);
    } else if (gSaveData.unk_3124 == 48) {
        OverlayTag tag;
        func_ov030_020d4ab8(47);
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_3_ID, func_ov003_0208ec74, NULL, PROCESS_STAGE_INIT);
    } else if (gSaveData.unk_3124 == 49) {
        OverlayTag tag;
        gSaveData.unk_24B8          = 1;
        gSaveData.currentStoryEvent = 779;
        MainOvlDisp_ReplaceTop(&tag, (s32)&SEQFLASH_OVL_RESULT, ProcessOverlay_Result, NULL, PROCESS_STAGE_INIT);
    } else if (gSaveData.unk_3124 == 51) {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, (s32)&SEQFLASH_OVL_STAFFROLL, func_ov042_020824a0, NULL, PROCESS_STAGE_INIT);
    } else if (gSaveData.unk_3128 == 3) {
        OverlayTag tag;
        MainOvlDisp_Pop(&tag);
    } else if (gSaveData.unk_3128 == 1) {
        OverlayTag tag;
#ifndef REGION_USA
        gSaveData.chapter++;
#endif
        MainOvlDisp_ReplaceTop(&tag, (s32)&SEQFLASH_OVL_SAVE, ProcessOverlay_Save, NULL, PROCESS_STAGE_INIT);
        gSaveData.unk_1AB4 |= 2;
    } else if (gSaveData.unk_3128 == 2) {
        OverlayTag tag;
#ifndef REGION_USA
        gSaveData.chapter = 21;
#endif
        MainOvlDisp_ReplaceTop(&tag, (s32)&SEQFLASH_OVL_SAVE, ProcessOverlay_Save, NULL, PROCESS_STAGE_INIT);
        gSaveData.unk_1AB4 |= 0x82;
    } else if (gSaveData.unk_3128 == 4) {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, func_ov030_020d5370, NULL, PROCESS_STAGE_INIT);
    } else {
        OverlayTag tag;
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, func_ov030_020ae92c, NULL, PROCESS_STAGE_INIT);
    }
}

void func_ov030_020d4f50(SeqFlashState* state) {
    ResourceMgr_ReinitManagers(NULL);
    Mem_Free(&gDebugHeap, state);
}

void func_ov030_020d4f74(SeqFlashState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        func_ov030_020d4f50(state);
    } else {
        data_ov030_020fd950.funcs[stage](state);
    }
}
