#include "Engine/File/BinMgr.h"

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

const BinIdentifier OtuFile_UnderObj      = {0x27, OtuFile_UnderObjPath};
const BinIdentifier OtuFile_OverBg        = {0x27, OtuFile_OverBgPath};
const BinIdentifier OtuFile_BadgeParams   = {0x27, OtuFile_BadgeParamsPath};
const BinIdentifier OtuFile_Stage2Bg      = {0x27, OtuFile_Stage2BgPath};
const BinIdentifier OtuFile_Badge         = {0x27, OtuFile_BadgePath};
const BinIdentifier OtuFile_MapData       = {0x27, OtuFile_MapDataPath};
const BinIdentifier OtuFile_CommonObj     = {0x27, OtuFile_CommonObjPath};
const BinIdentifier OtuFile_AI            = {0x27, OtuFile_AIPath};
const BinIdentifier OtuFile_Stage1Bg      = {0x27, OtuFile_Stage1BgPath};
const BinIdentifier OtuFile_OverObj       = {0x27, OtuFile_OverObjPath};
const BinIdentifier OtuFile_Stage3Bg      = {0x27, OtuFile_Stage3BgPath};
const BinIdentifier OtuFile_CommonBg      = {0x27, OtuFile_CommonBgPath};
const BinIdentifier OtuFile_Stage2Obj     = {0x27, OtuFile_Stage2ObjPath};
const BinIdentifier OtuFile_UnderBg       = {0x27, OtuFile_UnderBgPath};
const BinIdentifier OtuFile_SingleMatches = {0x27, OtuFile_SingleMatchesPath};
