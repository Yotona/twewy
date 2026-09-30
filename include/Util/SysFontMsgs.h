#ifndef TWEWY_UTIL_SYSFONTMSGS_H
#define TWEWY_UTIL_SYSFONTMSGS_H

// Message indices into Apl_Fuk/mestxt.bin. The JP and USA tables diverge, so every
// literal index must go through one of these. USA entries are grouped by the source .xls
// file named in src/Util/MsgXls.c (a USA-only table). *_START entries are the first
// message of a contiguous list;
// SYSMSG_CANT_EAT_* interleave as title/text pairs, so they step by two.

#ifdef REGION_USA
    // for_check_new_font_w.xls
    #define SYSMSG_FONT_CHECK_TEXT 8334 // "How does this look on screen?!\n(I'm wor"...
    // for_test_staffroll.xls
    #define SYSMSG_FONT_PAGE_FMT 8354 // ""
    // menu_abilityHelp.xls
    #define SYSMSG_THREAD_ABILITY_HELP_START 8603 // "Increases your <cC>drop rate<cE> by 1."..
    // menu_abilityName.xls
    #define SYSMSG_THREAD_ABILITY_NAME_START 8899 // "<ic_ab>Mother Lode"..
    // menu_bdgHelp1.xls
    #define SYSMSG_PIN_EFFECT_HELP_START 9264 // "<cC>Slash vertically up on empty space<c"...
    // menu_bdgHelp2.xls
    #define SYSMSG_PIN_GROWTH_HELP_START 9568 // "Growth Bonus:   Power:<cC> Y    <cE>Uses"...
    // menu_bdgHelpM.xls; USA's first entry is the Tin Pin stat labels, JP's are per-pin texts.
    #define SYSMSG_PIN_TINPIN_HELP_START 9872 // "Weight:                     Spin:       "...
    // menu_bdgName.xls
    #define SYSMSG_PIN_NAMES_START 10177 // "Ice Blow"..
    // menu_bdgPsy.xls
    #define SYSMSG_PSYCH_NAMES_START 10481 // "<ic_ne>Spark Core<ic_ci>"
    // menu_brand.xls
    #define SYSMSG_BRAND_NAMES_START    10785 // "Mus Rattus"..
    #define SYSMSG_BRAND_UNBRANDED      10799 // "Unbranded"
    #define SYSMSG_BRAND_ATTACK_DOUBLED 10800 // "Attack <cC>doubled"
    #define SYSMSG_BRAND_ATTACK_UP_50   10801 // "Attack <cC>+50%"
    #define SYSMSG_BRAND_ATTACK_UP_20   10802 // "Attack <cC>+20%"
    #define SYSMSG_BRAND_ATTACK_HALVED  10803 // "Attack <c6>halved"
    // menu_foodHelp.xls
    #define SYSMSG_FOOD_DESC_START 10819 // "A regular hamburger.\nNothing special, b"...
    // menu_foodName.xls
    #define SYSMSG_FOOD_NAMES_START 10861 // "Hamburger"..
    // menu_helpMess.xls
    #define SYSMSG_TOPMENU_HELP_LABELS    10903 // "THE PHONE MENU"..
    #define SYSMSG_TOPMENU_HELP_TEXT      10910 // "Use the Phone Menu to <cC>access game me"...
    #define SYSMSG_SAVEMENU_HELP_LABELS   10917 // "THE SAVE MENU"..
    #define SYSMSG_SAVEMENU_HELP_TEXT     10921 // "Use this menu to <cC>save your game<cE> a"...
    #define SYSMSG_BADGEMENU_HELP_LABELS  10925 // "THE PINS MENU"
    #define SYSMSG_BADGEMENU_HELP_TEXT    10936 // "Use this menu to <cC>find out more about"...
    #define SYSMSG_EQUIPMENU_HELP_LABELS  10947 // "THE ITEMS MENU"..
    #define SYSMSG_EQUIPMENU_HELP_TEXT    10956 // "Use this menu to change your characters'"...
    #define SYSMSG_MINGLEMENU_HELP_LABELS 10965 // "MINGLE MODE"..
    #define SYSMSG_MINGLEMENU_HELP_TEXT   10972 // "From this menu you can enter Mingle Mode"...
    #define SYSMSG_FRIENDMENU_HELP_LABELS 10989 // "THE FRIENDS MENU"..
    #define SYSMSG_FRIENDMENU_HELP_TEXT   10992 // "From this menu you can <cC>look over<cE> "...
    // menu_itemHelp.xls
    #define SYSMSG_THREAD_DESC_START 11004 // "This baseball cap bears an "M" patch.\nA"...
    // menu_itemName.xls
    #define SYSMSG_THREAD_NAMES_START 11284 // "M Cap"..
    // menu_map.xls
    #define SYSMSG_AREA_NAMES_START  11564 // "Scramble Crossing"..
    #define SYSMSG_AREA_NAME_UNKNOWN 11606 // "? ? ?"
    // menu_result.xls
    #define SYSMSG_RESULT_PIN_OBTAINED_FMT    12155 // "Obtained <cC><str><cE> x<u32>."
    #define SYSMSG_RESULT_FOOD_DIGESTED_FMT   12156 // "<str> digested the <str>."
    #define SYSMSG_RESULT_PARTNER_STAT_UP_FMT 12157 // "<str><str> <cC>+<u32><cE>"
    // USA-only: JP names the partner too and uses SYSMSG_RESULT_PARTNER_STAT_UP_FMT.
    #define SYSMSG_RESULT_STAT_UP_FMT           12158 // "<str> <cC>+<u32><cE>"
    #define SYSMSG_RESULT_SPECIAL_BONUS_START   12159 // "No damage taken."..
    #define SYSMSG_RESULT_MINGLE_COUNT_PLURAL   12207 // "You encountered <u32> people while you w"...
    #define SYSMSG_RESULT_MINGLE_COUNT_SINGULAR 12208 // "You encountered <u32> person while you w"...
    #define SYSMSG_RESULT_SLEEP_PP_INFO         12209 // "You receive PP for the amount of time yo"...
    #define SYSMSG_RESULT_SLEEP_SINCE           12210 // "A total of"
    #define SYSMSG_RESULT_SLEEP_ELAPSED         12211 // "have passed."
    #define SYSMSG_RESULT_SLEEP_DHM             12212 // "<u32>d <u32>h <u32>m"
    #define SYSMSG_RESULT_SLEEP_HMS             12213 // "<u32>h <u32>m <u32>s"
    // USA-only: JP has no combined sentence and draws the pieces above instead.
    #define SYSMSG_RESULT_SLEEP_TOTAL_DHM 12214 // "A total of  <u32>d <u32>h <u32>m  have p"...
    #define SYSMSG_RESULT_SLEEP_TOTAL_HMS 12215 // "A total of  <u32>h <u32>m <u32>s  have p"...
    // menu_shop.xls
    #define SYSMSG_SHOP_NAMES_START        12216 // "104 Building"..
    #define SYSMSG_SHOP_CLERK_NAMES_START  12310 // "Ayu Hamaguchi"..
    #define SYSMSG_SHOP_FRIENDSHIP_UP      12345 // "Friendship level up!"
    #define SYSMSG_SHOP_FRIENDSHIP_UP_TIPS 12346 // "<c8>You've grown on the seller!<cE>\nThe"...
    #define SYSMSG_SHOP_FRIENDSHIP_UP_ITEM 12347 // "<c8>You've grown on the seller!<cE>\nNow"...
    #define SYSMSG_SHOP_ABILITY_UNLOCKED   12348 // "New ability unlocked!"
    #define SYSMSG_SHOP_ABILITY_TIP_STYLE  12349 // "<c8>The seller digs your style!<cE>\nYou"...
    #define SYSMSG_SHOP_ABILITY_TIP_BROWSE 12350 // "<c8>The seller notices you browsing<cE> "...
    // menu_system.xls
    #define SYSMSG_PARTNER_NEKU              13086 // "Neku"
    #define SYSMSG_PARTNER_SHIKI             13087 // "Shiki"
    #define SYSMSG_PARTNER_JOSHUA            13088 // "Joshua"
    #define SYSMSG_PARTNER_BEAT              13089 // "Beat"
    #define SYSMSG_STAT_ATTACK               13090 // "Attack"
    #define SYSMSG_STAT_DEFENSE              13091 // "Defense"
    #define SYSMSG_STAT_HP                   13092 // "HP"
    #define SYSMSG_BRAVERY_REQ               13096 // "Bravery Req."
    #define SYSMSG_TOPMENU_ENTRY_NAMES_START 13097 // "Friends"..
    #define SYSMSG_PIN_CLASS_NAMES_START     13105 // "<cC>Angel<cE>"
    #define SYSMSG_STAT_NONE                 13113 // "----"
    #define SYSMSG_SLASH                     13114 // "/"
    #define SYSMSG_STAT_BONUS_FMT            13115 // "<str> <c6>+<s32>"
    #define SYSMSG_STAT_PENALTY_FMT          13116 // "<str> <cC>-<s32>"
    #define SYSMSG_DIVIDED_U32S              13117 // "<u32>/<u32>"
    #define SYSMSG_PIN_INPUT_TYPES_START     13119 // "Touch the pin"
    #define SYSMSG_PIN_STAT_NONE             13143 // "\u2014\u2014\u2014\u2014" (em dashes)
    #define SYSMSG_PIN_DURATION_TIME_FMT     13144 // "Lasts <cC><u32>.<u32><cE>s"
    #define SYSMSG_PIN_DURATION_USES_FMT     13145 // "Lasts <cC><u32><cE> uses"
    // USA-only: JP has no singular form.
    #define SYSMSG_PIN_DURATION_USE_FMT    13146 // "Lasts <cC><u32><cE> use"
    #define SYSMSG_PIN_BOOT_INSTANT        13147 // "Instant boot"
    #define SYSMSG_PIN_BOOT_TIME_FMT       13148 // "Ready to use in <cC><u32>.<u32><cE>s"
    #define SYSMSG_PIN_REBOOT_NONE         13149 // "\u2014\u2014\u2014\u2014" (em dashes)
    #define SYSMSG_PIN_REBOOT_TIME_FMT     13150 // "Reboots in <cC><u32>.<u32><cE>s"
    #define SYSMSG_MINGLE_ESPERS           13197 // "ESP'ers"
    #define SYSMSG_MINGLE_CIVVIES          13198 // "Civvies"
    #define SYSMSG_MINGLE_ALIENS           13199 // "Aliens"
    #define SYSMSG_CARD_ESPER_RANK         13200 // "ESP'er Rank"
    #define SYSMSG_CARD_ESPER_POINTS       13201 // "ESP'er Points"
    #define SYSMSG_CARD_NOISE_REPORT       13202 // "Noise Report"
    #define SYSMSG_CARD_PIN_MASTERY        13203 // "Pin Mastery"
    #define SYSMSG_CARD_ITEM_COLLECTION    13204 // "Item Collection"
    #define SYSMSG_CARD_TIME_ATTACK        13205 // "Final Time Attack"
    #define SYSMSG_CARD_POINTS_PLURAL      13206 // "<u32> pts."
    #define SYSMSG_CARD_POINTS_SINGULAR    13207 // "<u32> pt."
    #define SYSMSG_CARD_TYPES_PLURAL       13208 // "<u32> types"
    #define SYSMSG_CARD_TYPES_SINGULAR     13209 // "<u32> type"
    #define SYSMSG_CARD_PERCENT_FMT        13210 // "<u32>.<u32> %"
    #define SYSMSG_CARD_TIME_FMT           13212 // "<u32>'<u32><u32>\"<u32><u32>"
    #define SYSMSG_COUNT_FMT               13213 // "<u32>"
    #define SYSMSG_COUNT_NONE              13214 // "\u2014" (em dash)
    #define SYSMSG_CARD_GRADES_START       13215 // "\u2606" (star), then "A".."E"
    #define SYSMSG_CARD_ESPER_RANKS_START  13221 // "God".."Only Human"
    #define SYSMSG_SAVE_COMPLETE           13186 // "Save complete."
    #define SYSMSG_MINGLE_REVIEW_INFO      13232 // "Review your information."
    #define SYSMSG_MINGLE_SEND_NOTICE      13233 // "The contents shown above will be sent.\n"...
    #define SYSMSG_MINGLE_POWER_LIGHT_HINT 13234 // "End communications when the\npower light"...
    #define SYSMSG_MINGLE_COUNT_HINT       13235 // "Communications end when this\nmingle cou"...
    #define SYSMSG_MINGLE_END_AND_SAVE     13236 // "End communications and save"
    #define SYSMSG_MINGLE_POWER_LIGHT_RED  13237 // "The power light is red!\nCommunications "...
    #define SYSMSG_MINGLE_REMAINING_PLURAL 13238 // "You can mingle with <u32> more people."
    // USA-only: JP has no singular form.
    #define SYSMSG_MINGLE_REMAINING_SINGULAR    13239 // "You can mingle with <u32> more person."
    #define SYSMSG_MINGLE_NOW_MINGLING          13240 // "Now mingling!\nFeel free to close your D"...
    #define SYSMSG_MINGLE_ENDING                13241 // "Ending communications."
    #define SYSMSG_MINGLE_QUOTA_MET             13242 // "You've met your mingle quota!\nCommunica"...
    #define SYSMSG_MINGLE_SAVING                13243 // "<cC>Saving... Don't turn power OFF or re"...
    #define SYSMSG_MINGLE_RAN_INTO_FMT          13244 // "You ran into <cC><str><cE>!"
    #define SYSMSG_MINGLE_SHOP_SETUP            13249 // "<cC>Set up your Mingle Mode shop<cE>"
    #define SYSMSG_MINGLE_RAKED_IN_FMT          13245 // "You raked in <cC>$<u32><cE>!"
    #define SYSMSG_MINGLE_RAN_INTO_OTHER_FMT    13248 // "You ran into <cC><str><cE>..."
    #define SYSMSG_MINGLE_ALIEN_NAMES_START     13250 // "a cuckoo"
    #define SYSMSG_THREAD_ATTRIBUTE_LABEL       13151 // "Threads: "
    #define SYSMSG_THREAD_ATTRIBUTE_NAMES_START 13152 // "Headwear"..
    #define SYSMSG_FOOD_ITEM_LABEL              13158 // "Food Item"
    #define SYSMSG_SWAG_LABEL                   13159 // "Swag"
    #define SYSMSG_PARTNER_DAY_FMT              13169 // "<str>, Day <u32>"
    #define SYSMSG_ANOTHER_DAY                  13170 // ""Another Day""
    #define SYSMSG_SAVE_DATE_LABEL              13160 // "Date Saved"
    #define SYSMSG_SAVE_TIME_LABEL              13161 // "Time Saved"
    #define SYSMSG_SAVE_CHAPTER_LABEL           13162 // "Chapter"
    #define SYSMSG_SAVE_AREA_LABEL              13163 // "Area"
    // USA-only: JP writes the date and a 24-hour time as plain digits (SYSMSG_SAVE_DATE_FMT/TIME_FMT).
    #define SYSMSG_SAVE_DATE_MONTH_FMT        13165 // "<str> <u32>, <u32>"
    #define SYSMSG_SAVE_TIME_AM_FMT           13167 // "<u32>:<u32><u32> a.m."
    #define SYSMSG_SAVE_TIME_PM_FMT           13168 // "<u32>:<u32><u32> p.m."
    #define SYSMSG_SAVE_PLAY_TIME_FMT         13171 // "<cC>Play Time:  <u32>h <u32>m"
    #define SYSMSG_SAVE_MONTH_NAMES_START     13172 // "Jan."..
    #define SYSMSG_SAVE_PROMPT                13184 // "Save your progress?\nYou can pick up from"...
    #define SYSMSG_SAVE_SAVING                13185 // "<c6>Saving... Don't turn power OFF or rem"...
    #define SYSMSG_THREAD_ABILITY_LOCKED      13280 // "Ability Locked"
    #define SYSMSG_THREAD_ABILITY_LOCKED_HELP 13281 // "You haven't acquired this item's ability"...
    #define SYSMSG_FOOD_EFFECT_START          13292 // "Eat this to <cC>boost your sync rate rou"...
    #define SYSMSG_FOOD_DIGEST_BYTES          13298 // "Goes down in <u32> bytes"
    #define SYSMSG_FOOD_DIGEST_BYTE           13299 // "Goes down in <u32> byte"
    #define SYSMSG_SWAG_TIPS_LABEL            13300 // ""
    #define SYSMSG_CANT_EAT_TITLE_START       13301 // "You <cC>can't eat<cE> this item!"..
    #define SYSMSG_CANT_EAT_TEXT_START        13302 // "At least, not if you want to avoid\nthe "...
    #define SYSMSG_PIN_CANNOT_SELL            13314 // "<cC>You can't cash in this pin!<cE>\nIt's"...
    #define SYSMSG_PIN_STOCKPILE_FULL         13315 // "Your stockpile is full!\nYou can only ho"...
    // USA-only: JP has no singular form.
    #define SYSMSG_PIN_STOCKPILE_FULL_SINGULAR 13316 // "Your stockpile is full!\nYou can only ho"...
    #define SYSMSG_PIN_CLASS_LIMIT_START       13317 // "You're over your <cC>class limit<cE>!\nYo"...
    // USA-only: JP has no money cap warning.
    #define SYSMSG_PIN_WALLET_FULL 13322 // "Warning:\nYour <cC>wallet<cE> is full, so"...
    // USA-only: JP formats the plain number with a local template.
    #define SYSMSG_PIN_PRICE_FMT            13328 // "$ <u32>"
    #define SYSMSG_PIN_PRICE_THOUSANDS_FMT  13329 // "$ <u32>,<u32><u32><u32>"
    #define SYSMSG_PIN_PRICE_MILLIONS_FMT   13330 // "$ <u32>,<u32><u32><u32>,<u32><u32><u32>"
    #define SYSMSG_SHOP_PRICE_FMT           13331 // "<cC>$ <u32>"
    #define SYSMSG_SHOP_PRICE_THOUSANDS_FMT 13332 // "<cC>$ <u32>,<u32><u32><u32>"
    #define SYSMSG_SHOP_PRICE_MILLIONS_FMT  13333 // "<cC>$ <u32>,<u32><u32><u32>,<u32><u32><u"...
    #define SYSMSG_SHOP_BUY_CONFIRM         13334 // "Buy this merchandise?"
    #define SYSMSG_PIN_SELL_CONFIRM         13335 // "Cash in this pin?"
    #define SYSMSG_PIN_ARRANGE_TITLE        13336 // "<cC>Organize your pins"
    #define SYSMSG_PIN_ARRANGE_BY_NUMBER    13337 // "Arrange by number."
    #define SYSMSG_PIN_ARRANGE_BY_PSYCH     13339 // "Arrange by psych."
    #define SYSMSG_PIN_ALWAYS_ARRANGE       13341 // "Always organize your pins."
    #define SYSMSG_BRAND_AREA_PROTECTED_FMT 13342 // "A special force field\nsurrounds this ar"...
    #define SYSMSG_BRAND_AREA_UNAFFECTED    13343 // "This area doesn't\nseem to be affected\n"...
    #define SYSMSG_SHOP_QUEST_ITEM          13347 // "<cC>QUEST ITEM<cE>"
    #define SYSMSG_SHOP_BGM_NAMES_START     13344 // "Track 1: \"Economical Shoppers\""..
    #define SYSMSG_SHOP_TRADE_CONFIRM       13348 // "<cC>Trade in<cE> your items for this?"
    #define SYSMSG_SHOP_BAG_FULL_TITLE      13349 // "You <cC>can't carry<cE> any more!"
    #define SYSMSG_SHOP_BAG_FULL_TEXT       13350 // "One more item to lug around and you\nmig"...
    #define SYSMSG_PIN_RECOVERY_FMT         13351 // "Recovery <cC><u32><cE>%"
    // menu_treasureHelp.xls
    #define SYSMSG_SWAG_DESC_START 13352 // "Awe-inspiring thread, hand-dyed in Nishi"...
    // menu_treasureName.xls
    #define SYSMSG_SWAG_NAMES_START 13502 // "Colorful Thread"..
    // menu_treasureTips.xls
    #define SYSMSG_SWAG_TIPS_START 13652              // "Materials like this transform certain it"...
#elif defined(REGION_JP)
    #define SYSMSG_FONT_CHECK_TEXT              8266  // "{0753}{0754}{0755}{0756}"...
    #define SYSMSG_FONT_PAGE_FMT                8279  // "フォントセット :　<str>\n<u32>/<"...
    #define SYSMSG_THREAD_ABILITY_HELP_START    8527  // "ドロップレートが１アップする"..
    #define SYSMSG_THREAD_ABILITY_NAME_START    8823  // "<ic_ab>ビッグトレジャー"..
    #define SYSMSG_PIN_EFFECT_HELP_START        9188  // "<cC>{03B2}{070A}を{0728}か"...
    #define SYSMSG_PIN_GROWTH_HELP_START        9492  // "{02BB}{02B6}{07D9}{0262}"...
    #define SYSMSG_PIN_TINPIN_HELP_START        9796  // "おもさ<cC>９　　<cE>カーブ<cC>０　　"...
    #define SYSMSG_PIN_NAMES_START              10100 // "アイスブロウ"..
    #define SYSMSG_PSYCH_NAMES_START            10404 // "<ic_ne>スパークポイント<ic_ci>"
    #define SYSMSG_BRAND_NAMES_START            10708 // "ムース・ラットゥス"..
    #define SYSMSG_BRAND_UNBRANDED              10722 // "ノーブランド"
    #define SYSMSG_BRAND_ATTACK_DOUBLED         10723 // "{0710}{070E}{0247}<cC>２{"...
    #define SYSMSG_BRAND_ATTACK_UP_50           10724 // "{0710}{070E}{0247}<cC>１．"...
    #define SYSMSG_BRAND_ATTACK_UP_20           10725 // "{0710}{070E}{0247}<cC>１．"...
    #define SYSMSG_BRAND_ATTACK_HALVED          10726 // "{0710}{070E}{0247}<c6>{0"...
    #define SYSMSG_FOOD_DESC_START              10742 // "{0294}{035D}のハンバーガー\nリーズ"...
    #define SYSMSG_FOOD_NAMES_START             10784 // "ハンバーガー"..
    #define SYSMSG_TOPMENU_HELP_LABELS          10826 // "トップメニュー"..
    #define SYSMSG_TOPMENU_HELP_TEXT            10833 // "　トップメニューでは　<cC>{0283}の{0"...
    #define SYSMSG_SAVEMENU_HELP_LABELS         10840 // "セーブメニュー"..
    #define SYSMSG_SAVEMENU_HELP_TEXT           10844 // "　セーブメニューでは、<cC>これまでのゲームの"...
    #define SYSMSG_BADGEMENU_HELP_LABELS        10848 // "バッジメニュー"
    #define SYSMSG_BADGEMENU_HELP_TEXT          10859 // "　バッジメニューでは{0733}っている<cC>"...
    #define SYSMSG_EQUIPMENU_HELP_LABELS        10870 // "アイテムメニュー"..
    #define SYSMSG_EQUIPMENU_HELP_TEXT          10879 // "　アイテムメニューでは<cC>キャラクターのそう"...
    #define SYSMSG_MINGLEMENU_HELP_LABELS       10888 // "すれ{0266}いメニュー"..
    #define SYSMSG_MINGLEMENU_HELP_TEXT         10895 // "　すれ{0266}いメニューでは{0729}{02BC}の<cC>セッティングや"...
    #define SYSMSG_FRIENDMENU_HELP_LABELS       10912 // "フレンドリストメニュー"..
    #define SYSMSG_FRIENDMENU_HELP_TEXT         10915 // "　フレンドリストでは　これまでにすれ{0266}い{0729}{02BC}で\n"...
    #define SYSMSG_THREAD_DESC_START            10927 // "Ｍのワッペンがついたキャップ\nシンプルさを{0"...
    #define SYSMSG_THREAD_NAMES_START           11207 // "Ｍキャップ"..
    #define SYSMSG_AREA_NAMES_START             11487 // "スクランブル{02E6}{02D8}{02A6}"..
    #define SYSMSG_AREA_NAME_UNKNOWN            11529 // "？？？"
    #define SYSMSG_RESULT_PIN_OBTAINED_FMT      12078 // "<str>×<cC><u32><cE>ゲット！"
    #define SYSMSG_RESULT_FOOD_DIGESTED_FMT     12079 // "<str>が<str>{0250}{0322}！"
    #define SYSMSG_RESULT_PARTNER_STAT_UP_FMT   12080 // "<str>　<str><cC>＋<u32><cE>"
    #define SYSMSG_RESULT_SPECIAL_BONUS_START   12081 // "ノーダメージで{070E}{0381}した！"..
    #define SYSMSG_RESULT_MINGLE_COUNT          12129 // "{0826}{070F}のすれ{0266}い{0729}{02BC}の"...
    #define SYSMSG_RESULT_SLEEP_PP_INFO         12130 // "ゲームをプレイしていなかった"...
    #define SYSMSG_RESULT_SLEEP_SINCE           12131 // "{024D}{070F}から"
    #define SYSMSG_RESULT_SLEEP_ELAPSED         12132 // "{03D7}{042A}"
    #define SYSMSG_RESULT_SLEEP_DHM             12133 // "<u32>{0898}<u32>{070C}{070A}<u32>{082F}"
    #define SYSMSG_RESULT_SLEEP_HMS             12134 // "<u32>{070C}{070A}<u32>{082F}<u32>{03A9}"
    #define SYSMSG_SHOP_NAMES_START             12135 // "１０４ビル"..
    #define SYSMSG_SHOP_CLERK_NAMES_START       12229 // "{067A}{031A}　アユカ"..
    #define SYSMSG_SHOP_FRIENDSHIP_UP           12264 // "ショップ{026E}{02FB}との{027B}"...
    #define SYSMSG_SHOP_FRIENDSHIP_UP_TIPS      12265 // "<c8>ショップ{026E}{02FB}と{02"...
    #define SYSMSG_SHOP_FRIENDSHIP_UP_ITEM      12266 // "<c8>ショップ{026E}{02FB}と{02"...
    #define SYSMSG_SHOP_ABILITY_UNLOCKED        12267 // "ショップ{026E}{02FB}からアビリティ{"...
    #define SYSMSG_SHOP_ABILITY_TIP_STYLE       12268 // "<c8>{025B}{082F}のコーディネイト"...
    #define SYSMSG_SHOP_ABILITY_TIP_BROWSE      12269 // "<c8>アイテムを{0238}ていたら{0264"...
    #define SYSMSG_PARTNER_NEKU                 13005 // "ネク"
    #define SYSMSG_PARTNER_SHIKI                13006 // "シキ"
    #define SYSMSG_PARTNER_JOSHUA               13007 // "ヨシュア"
    #define SYSMSG_PARTNER_BEAT                 13008 // "ビイト"
    #define SYSMSG_STAT_ATTACK                  13009 // "{0710}{070E}{0247}"
    #define SYSMSG_STAT_DEFENSE                 13010 // "{038A}{082B}{0247}"
    #define SYSMSG_STAT_HP                      13011 // "ＨＰ"
    #define SYSMSG_BRAVERY_REQ                  13015 // "{0286}{0292}{04FD}{0236}"
    #define SYSMSG_TOPMENU_ENTRY_NAMES_START    13016 // "フレンド"..
    #define SYSMSG_PIN_CLASS_NAMES_START        13024 // "<cC>{02FF}{0265}<cE>クラス"
    #define SYSMSG_STAT_NONE                    13032 // "－－－－"
    #define SYSMSG_SLASH                        13033 // "／"
    #define SYSMSG_STAT_BONUS_FMT               13034 // "<str>　<c6>＋<s32>"
    #define SYSMSG_STAT_PENALTY_FMT             13035 // "<str>　<cC>－<s32>"
    #define SYSMSG_DIVIDED_U32S                 13036 // "<u32>／<u32>"
    #define SYSMSG_PIN_INPUT_TYPES_START        13038 // "バッジをタッチ"
    #define SYSMSG_PIN_STAT_NONE                13062 // "－－－－"
    #define SYSMSG_PIN_DURATION_TIME_FMT        13063 // "{0265}{0711}{070C}{070A}"...
    #define SYSMSG_PIN_DURATION_USES_FMT        13064 // "{0265}{0711}{070F}{0290}"...
    #define SYSMSG_PIN_BOOT_INSTANT             13065 // "{05A4}{070C}{0741}{0252}"
    #define SYSMSG_PIN_BOOT_TIME_FMT            13066 // "<cC><u32>.<u32><cE>{03A9"...
    #define SYSMSG_PIN_REBOOT_NONE              13067 // "－－－－"
    #define SYSMSG_PIN_REBOOT_TIME_FMT          13068 // "<cC><u32>.<u32><cE>{03A9"...
    #define SYSMSG_MINGLE_ESPERS                13098 // "エスパー"
    #define SYSMSG_MINGLE_CIVVIES               13099 // "いっぱんじん"
    #define SYSMSG_MINGLE_ALIENS                13100 // "エイリアン"
    #define SYSMSG_CARD_ESPER_RANK              13101 // "エスパーランク"
    #define SYSMSG_CARD_ESPER_POINTS            13102 // "エスパーポイント"
    #define SYSMSG_CARD_NOISE_REPORT            13103 // "ノイズレポート"
    #define SYSMSG_CARD_PIN_MASTERY             13104 // "マスターバッジ"
    #define SYSMSG_CARD_ITEM_COLLECTION         13105 // "アイテムコンプ"
    #define SYSMSG_CARD_TIME_ATTACK             13106 // "ファイナルタイムアタック"
    #define SYSMSG_CARD_POINTS_FMT              13107 // "<u32>ポイント"
    #define SYSMSG_CARD_TYPES_FMT               13108 // "<u32>{03D6}{0389}"
    #define SYSMSG_CARD_PERCENT_FMT             13109 // "<u32>.<u32>{020D}"
    #define SYSMSG_CARD_TIME_FMT                13110 // "<u32><u32>'<u32><u32>"<u32><u32>"
    #define SYSMSG_COUNT_FMT                    13111 // "<u32>{085D}"
    #define SYSMSG_COUNT_NONE                   13112 // "－"
    #define SYSMSG_CARD_GRADES_START            13113 // "Ｓ"..
    #define SYSMSG_CARD_ESPER_RANKS_START       13119 // "{023F}"..
    #define SYSMSG_SAVE_COMPLETE                13089 // "セーブ{0738}{0343}しました"
    #define SYSMSG_MINGLE_REVIEW_INFO           13130 // "すれ{0266}い{0729}{02BC}　セッティング"
    #define SYSMSG_MINGLE_SEND_NOTICE           13131 // "この{02C7}{03FF}は{0729}{02BC}{0713}{070D}に"...
    #define SYSMSG_MINGLE_POWER_LIGHT_HINT      13132 // "{02A1}{060A}ランプが{036E}いときは\n{0729}{02BC}"...
    #define SYSMSG_MINGLE_COUNT_HINT            13133 // "{0729}{02BC}する{029C}り{085D}{0290}\n０{085"...
    #define SYSMSG_MINGLE_END_AND_SAVE          13134 // "セーブして{0729}{02BC}を{0738}{0343}する"
    #define SYSMSG_MINGLE_POWER_LIGHT_RED       13135 // "{02A1}{060A}ランプが{036E}くなったので\n{0729}{02B"...
    #define SYSMSG_MINGLE_REMAINING             13136 // "あと<u32>{085D}まですれ{0266}い{0729}{02BC}できます"
    #define SYSMSG_MINGLE_NOW_MINGLING          13137 // "すれ{0266}い{0729}{02BC}{024F}です！\nこの{02F8}"...
    #define SYSMSG_MINGLE_ENDING                13138 // "{0729}{02BC}を{0738}{0343}します"
    #define SYSMSG_MINGLE_QUOTA_MET             13139 // "{0375}{0294}の{085D}{0290}に{0715}したので\n{0"...
    #define SYSMSG_MINGLE_SAVING                13140 // "<cC>セーブ{024F}…　カードを{0360}いたり\n{02A1}{060"...
    #define SYSMSG_MINGLE_RAN_INTO_FMT          13141 // "<cC><str><cE>とすれ{0266}いました！"
    #define SYSMSG_MINGLE_SHOP_SETUP            13146 // "<cC>すれ{0266}い{0729}{02BC}のショップを{0379}{02"...
    #define SYSMSG_MINGLE_RAKED_IN_FMT          13142 // "<cC>{0225}<u32><cE>のお{028E}い{0255}げです！"
    #define SYSMSG_MINGLE_RAN_INTO_OTHER_FMT    13145 // "<cC><str><cE>とすれ{0266}いました…"
    #define SYSMSG_MINGLE_ALIEN_NAMES_START     13147 // "かんこどり"
    #define SYSMSG_THREAD_ATTRIBUTE_LABEL       13069 // "そうび{062A}{026B}："
    #define SYSMSG_THREAD_ATTRIBUTE_NAMES_START 13070 // "{02A7}"..
    #define SYSMSG_FOOD_ITEM_LABEL              13076 // "フードアイテム"
    #define SYSMSG_SWAG_LABEL                   13077 // "{0517}{02E2}{02C6}"
    #define SYSMSG_PARTNER_DAY_FMT              13084 // "<str>{052C}　<u32>{0898}{"...
    #define SYSMSG_ANOTHER_DAY                  13085 // "アナザーデイ"
    #define SYSMSG_SAVE_DATE_LABEL              13078 // "セーブした{0898}{02F4}"
    #define SYSMSG_SAVE_TIME_LABEL              13079 // "セーブした{070C}{070A}"
    #define SYSMSG_SAVE_CHAPTER_LABEL           13080 // "チャプター"
    #define SYSMSG_SAVE_AREA_LABEL              13081 // "{025A}{026B}"
    #define SYSMSG_SAVE_DATE_FMT                13082 // "<u32>／<u32><u32>／<u32><u32>"
    #define SYSMSG_SAVE_TIME_FMT                13083 // "<u32><u32>：<u32><u32>"
    #define SYSMSG_SAVE_PLAY_TIME_FMT           13086 // "<cC>プレイタイム<u32>{070C}{070A}<u32>{082F}"
    #define SYSMSG_SAVE_PROMPT                  13087 // "これまでの{02C7}{03FF}をセーブしますか？\n{0737}{070F}から"...
    #define SYSMSG_SAVE_SAVING                  13088 // "<cC>セーブ{024F}…　カードを{0360}いたり\n{02A1}{060A}を"...
    #define SYSMSG_THREAD_ABILITY_LOCKED        13177 // "アビリティ{0302}{026F}{070D}"
    #define SYSMSG_THREAD_ABILITY_LOCKED_HELP   13178 // "このアイテムのアビリティはまだ{026F}{07"...
    #define SYSMSG_FOOD_EFFECT_START            13189 // "{0273}べると<cC>シンクロ{035C}が"...
    #define SYSMSG_FOOD_DIGEST_BYTES            13195 // "{0250}{0322}メモリ：<u32>"
    #define SYSMSG_SWAG_TIPS_LABEL              13196 // "ＴＩＰＳ"
    #define SYSMSG_CANT_EAT_TITLE_START         13197 // "<cC>このアイテム<cE>はたべられません！"..
    #define SYSMSG_CANT_EAT_TEXT_START          13198 // "このアイテムをたべると\nおなかをこわしてしまい"...
    #define SYSMSG_PIN_CANNOT_SELL              13210 // "<cC>このバッジは{056B}{0330}でき"...
    #define SYSMSG_PIN_STOCKPILE_FULL           13211 // "<cC>ストックバッジ<cE>がいっぱいです！"...
    #define SYSMSG_PIN_CLASS_LIMIT_START        13212 // "<cC>クラス<cE>{02AB}{028A}オ"...
    #define SYSMSG_SHOP_PRICE_FMT               13221 // "<cC>{0225}<u32>"
    #define SYSMSG_SHOP_BUY_CONFIRM             13222 // "この{041D}{02C6}を{065E}{02"...
    #define SYSMSG_PIN_SELL_CONFIRM             13223 // "このバッジを{056B}{0330}しますか？"
    #define SYSMSG_PIN_ARRANGE_TITLE            13224 // "<cC>バッジをならびかえます"
    #define SYSMSG_PIN_ARRANGE_BY_NUMBER        13225 // "ナンバーでならべる"
    #define SYSMSG_PIN_ARRANGE_BY_PSYCH         13227 // "サイキックでならべる"
    #define SYSMSG_PIN_ALWAYS_ARRANGE           13229 // "{0312}{070F}オートでならべる"
    #define SYSMSG_BRAND_AREA_PROTECTED_FMT     13230 // "この{025A}{026B}は{02E9}{06"...
    #define SYSMSG_BRAND_AREA_UNAFFECTED        13231 // "この{025A}{026B}には\nブランドラン"...
    #define SYSMSG_SHOP_QUEST_ITEM              13235 // "<cC>クエストアイテム<cE>"
    #define SYSMSG_SHOP_BGM_NAMES_START         13232 // "ＢＧＭ１「Economical Shoppers」"..
    #define SYSMSG_SHOP_TRADE_CONFIRM           13236 // "お{070D}{0733}ちのアイテムと<cC>"...
    #define SYSMSG_SHOP_BAG_FULL_TITLE          13237 // "<cC>{026B}{0733}{0290}オー"...
    #define SYSMSG_SHOP_BAG_FULL_TEXT           13238 // "お{070D}{0733}ちがいっぱいなので\n"...
    #define SYSMSG_PIN_RECOVERY_FMT             13239 // "{070F}{0824}{0247}<cC><u"...
    #define SYSMSG_SWAG_DESC_START              13240 // "{059A}{0272}の{024F}{0723"...
    #define SYSMSG_SWAG_NAMES_START             13390 // "あざやかな{05E8}"..
    #define SYSMSG_SWAG_TIPS_START              13540 // "あるアイテムを、もっとステキで{073A}{02"...
#endif

#endif // TWEWY_UTIL_SYSFONTMSGS_H
