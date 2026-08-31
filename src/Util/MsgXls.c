#include <nitro/types.h>

typedef struct {
    /* 0x0 */ s32         mesNum;
    /* 0x4 */ const char* xlsName;
} XlsFileEntry;

static const XlsFileEntry sMsgXlsTable[60] = {
    {    0,                  "b_message.xls"},
    { 3437,                "b_message_y.xls"},
    { 3908,           "e05_test_message.xls"},
    { 3916,                  "e_message.xls"},
    { 7322,                "e_message_w.xls"},
    { 7904,                "e_message_y.xls"},
    { 8334,       "for_check_new_font_w.xls"},
    { 8351,         "for_test_staffroll.xls"},
    { 8362,                  "g_message.xls"},
    { 8603,           "menu_abilityHelp.xls"},
    { 8899,           "menu_abilityName.xls"},
    { 9195,              "menu_baybadge.xls"},
    { 9228,         "menu_baybadge_name.xls"},
    { 9264,              "menu_bdgHelp1.xls"},
    { 9568,              "menu_bdgHelp2.xls"},
    { 9872,              "menu_bdgHelpM.xls"},
    {10177,               "menu_bdgName.xls"},
    {10481,                "menu_bdgPsy.xls"},
    {10785,                 "menu_brand.xls"},
    {10804,               "menu_deldata.xls"},
    {10819,              "menu_foodHelp.xls"},
    {10861,              "menu_foodName.xls"},
    {10903,              "menu_helpMess.xls"},
    {11004,              "menu_itemHelp.xls"},
    {11284,              "menu_itemName.xls"},
    {11564,                   "menu_map.xls"},
    {11621,             "menu_noizeHelp.xls"},
    {11717,             "menu_noizeName.xls"},
    {11813,              "menu_noizeVal.xls"},
    {11824,                "menu_report.xls"},
    {12022,           "menu_reportQuest.xls"},
    {12155,                "menu_result.xls"},
    {12216,                  "menu_shop.xls"},
    {13086,                "menu_system.xls"},
    {13352,          "menu_treasureHelp.xls"},
    {13502,          "menu_treasureName.xls"},
    {13652,          "menu_treasureTips.xls"},
    {13802,          "message_KeyWord_w.xls"},
    {14064,             "message_scan_w.xls"},
    {14406,             "m_message_1211.xls"},
    {16853,                "N_staffroll.xls"},
    {17404,                  "p_message.xls"},
    {17813,                "p_message_y.xls"},
    {17936,                  "r_message.xls"},
    {22145,           "r_message_scan_w.xls"},
    {22159,                "r_message_w.xls"},
    {22178,                "r_message_y.xls"},
    {22756,     "scan_message_beat_free.xls"},
    {22894,         "scan_message_brand.xls"},
    {23204,   "scan_message_joshua_free.xls"},
    {23344, "scan_message_kaiketsu_free.xls"},
    {23480,           "scan_message_map.xls"},
    {23874,    "scan_message_shiki_free.xls"},
    {24012,             "system_message.xls"},
    {24022,        "TEST_message_scan_w.xls"},
    {24030,             "TEST_message_w.xls"},
    {24229,    "tgs_touch_message060914.xls"},
    {24260,              "touch_message.xls"},
    {24611,            "tutrial_message.xls"},
    {24688,               "wall_message.xls"},
};

s32 MsgXls_GetMesNum(s32 index) {
    return sMsgXlsTable[index].mesNum;
}

s32 MsgXls_GetXlsFileCount(void) {
    return ARRAY_LEN(sMsgXlsTable);
}

s32 MsgXls_GetMesNumEnd(s32 index) {
    s32 next = index + 1;

    if (index < 0 || next >= MsgXls_GetXlsFileCount()) {
        return 0;
    }
    return sMsgXlsTable[next].mesNum;
}

const char* MsgXls_GetXlsName(s32 index) {
    return sMsgXlsTable[index].xlsName;
}
