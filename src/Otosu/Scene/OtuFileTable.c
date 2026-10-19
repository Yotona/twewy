#include "Engine/File/BinMgr.h"
#include "Otosu/OtosuRegion.h"

char OtuFile_BadgePath[]         = "Apl_Tak/Grp_Badge.bin";
char OtuFile_AIPath[]            = "Apl_Sug/BeBadge_AI.bin";
char OtuFile_SingleMatchesPath[] = "data/BeBadge_Single.bin";
char OtuFile_MapDataPath[]       = "data/BeBadge_MapData.bin";
char OtuFile_CommonBgPath[]      = "Apl_Sug/Grp_baycmnbg.bin";
char OtuFile_BadgeParamsPath[]   = "Apl_Tak/BeBadge_Parm.bin";
char OtuFile_Stage1BgPath[]      = "Apl_Sug/Grp_baystg1bg.bin";
char OtuFile_Stage3BgPath[]      = "Apl_Sug/Grp_baystg3bg.bin";
char OtuFile_CommonObjPath[]     = "Apl_Sug/Grp_baycmnobj.bin";
char OtuFile_Stage2BgPath[]      = "Apl_Sug/Grp_baystg2bg.bin";
char OtuFile_Stage2ObjPath[]     = "Apl_Sug/Grp_baystg2obj.bin";
char OtuFile_UnderBgPath[]       = "Apl_Sug/Grp_baymenunbg.bin";
char OtuFile_OverBgPath[]        = "Apl_Sug/Grp_baymenovbg.bin";
char OtuFile_OverObjPath[]       = "Apl_Sug/Grp_baymenovobj.bin";
char OtuFile_UnderObjPath[]      = "Apl_Sug/Grp_baymenuunobj.bin";

const BinIdentifier OtuFile_UnderObj      = {OTOSU_GAME_OVL_NUM, OtuFile_UnderObjPath};
const BinIdentifier OtuFile_OverBg        = {OTOSU_GAME_OVL_NUM, OtuFile_OverBgPath};
const BinIdentifier OtuFile_BadgeParams   = {OTOSU_GAME_OVL_NUM, OtuFile_BadgeParamsPath};
const BinIdentifier OtuFile_Stage2Bg      = {OTOSU_GAME_OVL_NUM, OtuFile_Stage2BgPath};
const BinIdentifier OtuFile_Badge         = {OTOSU_GAME_OVL_NUM, OtuFile_BadgePath};
const BinIdentifier OtuFile_MapData       = {OTOSU_GAME_OVL_NUM, OtuFile_MapDataPath};
const BinIdentifier OtuFile_CommonObj     = {OTOSU_GAME_OVL_NUM, OtuFile_CommonObjPath};
const BinIdentifier OtuFile_AI            = {OTOSU_GAME_OVL_NUM, OtuFile_AIPath};
const BinIdentifier OtuFile_Stage1Bg      = {OTOSU_GAME_OVL_NUM, OtuFile_Stage1BgPath};
const BinIdentifier OtuFile_OverObj       = {OTOSU_GAME_OVL_NUM, OtuFile_OverObjPath};
const BinIdentifier OtuFile_Stage3Bg      = {OTOSU_GAME_OVL_NUM, OtuFile_Stage3BgPath};
const BinIdentifier OtuFile_CommonBg      = {OTOSU_GAME_OVL_NUM, OtuFile_CommonBgPath};
const BinIdentifier OtuFile_Stage2Obj     = {OTOSU_GAME_OVL_NUM, OtuFile_Stage2ObjPath};
const BinIdentifier OtuFile_UnderBg       = {OTOSU_GAME_OVL_NUM, OtuFile_UnderBgPath};
const BinIdentifier OtuFile_SingleMatches = {OTOSU_GAME_OVL_NUM, OtuFile_SingleMatchesPath};
