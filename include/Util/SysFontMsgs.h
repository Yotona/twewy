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
    // menu_bdgName.xls
    #define SYSMSG_PIN_NAMES_START 10177 // "Ice Blow"..
    // menu_brand.xls
    #define SYSMSG_BRAND_NAMES_START    10785 // "Mus Rattus"..
    #define SYSMSG_BRAND_ATTACK_DOUBLED 10800 // "Attack <cC>doubled"
    #define SYSMSG_BRAND_ATTACK_UP_50   10801 // "Attack <cC>+50%"
    #define SYSMSG_BRAND_ATTACK_UP_20   10802 // "Attack <cC>+20%"
    #define SYSMSG_BRAND_ATTACK_HALVED  10803 // "Attack <c6>halved"
    // menu_foodHelp.xls
    #define SYSMSG_FOOD_DESC_START 10819 // "A regular hamburger.\nNothing special, b"...
    // menu_foodName.xls
    #define SYSMSG_FOOD_NAMES_START 10861 // "Hamburger"..
    // menu_helpMess.xls
    #define SYSMSG_TOPMENU_HELP_LABELS   10903 // "THE PHONE MENU"..
    #define SYSMSG_TOPMENU_HELP_TEXT     10910 // "Use the Phone Menu to <cC>access game me"...
    #define SYSMSG_EQUIPMENU_HELP_LABELS 10947 // "THE ITEMS MENU"..
    #define SYSMSG_EQUIPMENU_HELP_TEXT   10956 // "Use this menu to change your characters'"...
    // menu_itemHelp.xls
    #define SYSMSG_THREAD_DESC_START 11004 // "This baseball cap bears an "M" patch.\nA"...
    // menu_itemName.xls
    #define SYSMSG_THREAD_NAMES_START 11284 // "M Cap"..
    // menu_map.xls
    #define SYSMSG_AREA_NAMES_START  11564 // "Scramble Crossing"..
    #define SYSMSG_AREA_NAME_UNKNOWN 11606 // "? ? ?"
    // menu_shop.xls
    #define SYSMSG_SHOP_NAMES_START        12216 // "104 Building"..
    #define SYSMSG_SHOP_FRIENDSHIP_UP      12345 // "Friendship level up!"
    #define SYSMSG_SHOP_FRIENDSHIP_UP_TIPS 12346 // "<c8>You've grown on the seller!<cE>\nThe"...
    #define SYSMSG_SHOP_FRIENDSHIP_UP_ITEM 12347 // "<c8>You've grown on the seller!<cE>\nNow"...
    #define SYSMSG_SHOP_ABILITY_UNLOCKED   12348 // "New ability unlocked!"
    #define SYSMSG_SHOP_ABILITY_TIP_STYLE  12349 // "<c8>The seller digs your style!<cE>\nYou"...
    #define SYSMSG_SHOP_ABILITY_TIP_BROWSE 12350 // "<c8>The seller notices you browsing<cE> "...
    // menu_system.xls
    #define SYSMSG_PARTNER_SHIKI                13087 // "Shiki"
    #define SYSMSG_PARTNER_JOSHUA               13088 // "Joshua"
    #define SYSMSG_PARTNER_BEAT                 13089 // "Beat"
    #define SYSMSG_STAT_ATTACK                  13090 // "Attack"
    #define SYSMSG_STAT_DEFENSE                 13091 // "Defense"
    #define SYSMSG_STAT_HP                      13092 // "HP"
    #define SYSMSG_BRAVERY_REQ                  13096 // "Bravery Req."
    #define SYSMSG_STAT_NONE                    13113 // "----"
    #define SYSMSG_STAT_BONUS_FMT               13115 // "<str> <c6>+<s32>"
    #define SYSMSG_STAT_PENALTY_FMT             13116 // "<str> <cC>-<s32>"
    #define SYSMSG_DIVIDED_U32S                 13117 // "<u32>/<u32>"
    #define SYSMSG_THREAD_ATTRIBUTE_LABEL       13151 // "Threads: "
    #define SYSMSG_THREAD_ATTRIBUTE_NAMES_START 13152 // "Headwear"..
    #define SYSMSG_FOOD_ITEM_LABEL              13158 // "Food Item"
    #define SYSMSG_SWAG_LABEL                   13159 // "Swag"
    #define SYSMSG_PARTNER_DAY_FMT              13169 // "<str>, Day <u32>"
    #define SYSMSG_ANOTHER_DAY                  13170 // ""Another Day""
    #define SYSMSG_THREAD_ABILITY_LOCKED        13280 // "Ability Locked"
    #define SYSMSG_THREAD_ABILITY_LOCKED_HELP   13281 // "You haven't acquired this item's ability"...
    #define SYSMSG_FOOD_EFFECT_START            13292 // "Eat this to <cC>boost your sync rate rou"...
    #define SYSMSG_FOOD_DIGEST_BYTES            13298 // "Goes down in <u32> bytes"
    #define SYSMSG_FOOD_DIGEST_BYTE             13299 // "Goes down in <u32> byte"
    #define SYSMSG_SWAG_TIPS_LABEL              13300 // ""
    #define SYSMSG_CANT_EAT_TITLE_START         13301 // "You <cC>can't eat<cE> this item!"..
    #define SYSMSG_CANT_EAT_TEXT_START          13302 // "At least, not if you want to avoid\nthe "...
    #define SYSMSG_SHOP_PRICE_FMT               13331 // "<cC>$ <u32>"
    #define SYSMSG_SHOP_PRICE_THOUSANDS_FMT     13332 // "<cC>$ <u32>,<u32><u32><u32>"
    #define SYSMSG_SHOP_PRICE_MILLIONS_FMT      13333 // "<cC>$ <u32>,<u32><u32><u32>,<u32><u32><u"...
    #define SYSMSG_SHOP_BUY_CONFIRM             13334 // "Buy this merchandise?"
    #define SYSMSG_BRAND_AREA_PROTECTED_FMT     13342 // "A special force field\nsurrounds this ar"...
    #define SYSMSG_BRAND_AREA_UNAFFECTED        13343 // "This area doesn't\nseem to be affected\n"...
    #define SYSMSG_SHOP_QUEST_ITEM              13347 // "<cC>QUEST ITEM<cE>"
    #define SYSMSG_SHOP_TRADE_CONFIRM           13348 // "<cC>Trade in<cE> your items for this?"
    #define SYSMSG_SHOP_BAG_FULL_TITLE          13349 // "You <cC>can't carry<cE> any more!"
    #define SYSMSG_SHOP_BAG_FULL_TEXT           13350 // "One more item to lug around and you\nmig"...
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
    #define SYSMSG_PIN_NAMES_START              10100 // "アイスブロウ"..
    #define SYSMSG_BRAND_NAMES_START            10708 // "ムース・ラットゥス"..
    #define SYSMSG_BRAND_ATTACK_DOUBLED         10723 // "{0710}{070E}{0247}<cC>２{"...
    #define SYSMSG_BRAND_ATTACK_UP_50           10724 // "{0710}{070E}{0247}<cC>１．"...
    #define SYSMSG_BRAND_ATTACK_UP_20           10725 // "{0710}{070E}{0247}<cC>１．"...
    #define SYSMSG_BRAND_ATTACK_HALVED          10726 // "{0710}{070E}{0247}<c6>{0"...
    #define SYSMSG_FOOD_DESC_START              10742 // "{0294}{035D}のハンバーガー\nリーズ"...
    #define SYSMSG_FOOD_NAMES_START             10784 // "ハンバーガー"..
    #define SYSMSG_TOPMENU_HELP_LABELS          10826 // "トップメニュー"..
    #define SYSMSG_TOPMENU_HELP_TEXT            10833 // "　トップメニューでは　<cC>{0283}の{0"...
    #define SYSMSG_EQUIPMENU_HELP_LABELS        10870 // "アイテムメニュー"..
    #define SYSMSG_EQUIPMENU_HELP_TEXT          10879 // "　アイテムメニューでは<cC>キャラクターのそう"...
    #define SYSMSG_THREAD_DESC_START            10927 // "Ｍのワッペンがついたキャップ\nシンプルさを{0"...
    #define SYSMSG_THREAD_NAMES_START           11207 // "Ｍキャップ"..
    #define SYSMSG_AREA_NAMES_START             11487 // "スクランブル{02E6}{02D8}{02A6}"..
    #define SYSMSG_AREA_NAME_UNKNOWN            11529 // "？？？"
    #define SYSMSG_SHOP_NAMES_START             12135 // "１０４ビル"..
    #define SYSMSG_SHOP_FRIENDSHIP_UP           12264 // "ショップ{026E}{02FB}との{027B}"...
    #define SYSMSG_SHOP_FRIENDSHIP_UP_TIPS      12265 // "<c8>ショップ{026E}{02FB}と{02"...
    #define SYSMSG_SHOP_FRIENDSHIP_UP_ITEM      12266 // "<c8>ショップ{026E}{02FB}と{02"...
    #define SYSMSG_SHOP_ABILITY_UNLOCKED        12267 // "ショップ{026E}{02FB}からアビリティ{"...
    #define SYSMSG_SHOP_ABILITY_TIP_STYLE       12268 // "<c8>{025B}{082F}のコーディネイト"...
    #define SYSMSG_SHOP_ABILITY_TIP_BROWSE      12269 // "<c8>アイテムを{0238}ていたら{0264"...
    #define SYSMSG_PARTNER_SHIKI                13006 // "シキ"
    #define SYSMSG_PARTNER_JOSHUA               13007 // "ヨシュア"
    #define SYSMSG_PARTNER_BEAT                 13008 // "ビイト"
    #define SYSMSG_STAT_ATTACK                  13009 // "{0710}{070E}{0247}"
    #define SYSMSG_STAT_DEFENSE                 13010 // "{038A}{082B}{0247}"
    #define SYSMSG_STAT_HP                      13011 // "ＨＰ"
    #define SYSMSG_BRAVERY_REQ                  13015 // "{0286}{0292}{04FD}{0236}"
    #define SYSMSG_STAT_NONE                    13032 // "－－－－"
    #define SYSMSG_STAT_BONUS_FMT               13034 // "<str>　<c6>＋<s32>"
    #define SYSMSG_STAT_PENALTY_FMT             13035 // "<str>　<cC>－<s32>"
    #define SYSMSG_DIVIDED_U32S                 13036 // "<u32>／<u32>"
    #define SYSMSG_THREAD_ATTRIBUTE_LABEL       13069 // "そうび{062A}{026B}："
    #define SYSMSG_THREAD_ATTRIBUTE_NAMES_START 13070 // "{02A7}"..
    #define SYSMSG_FOOD_ITEM_LABEL              13076 // "フードアイテム"
    #define SYSMSG_SWAG_LABEL                   13077 // "{0517}{02E2}{02C6}"
    #define SYSMSG_PARTNER_DAY_FMT              13084 // "<str>{052C}　<u32>{0898}{"...
    #define SYSMSG_ANOTHER_DAY                  13085 // "アナザーデイ"
    #define SYSMSG_THREAD_ABILITY_LOCKED        13177 // "アビリティ{0302}{026F}{070D}"
    #define SYSMSG_THREAD_ABILITY_LOCKED_HELP   13178 // "このアイテムのアビリティはまだ{026F}{07"...
    #define SYSMSG_FOOD_EFFECT_START            13189 // "{0273}べると<cC>シンクロ{035C}が"...
    #define SYSMSG_FOOD_DIGEST_BYTES            13195 // "{0250}{0322}メモリ：<u32>"
    #define SYSMSG_SWAG_TIPS_LABEL              13196 // "ＴＩＰＳ"
    #define SYSMSG_CANT_EAT_TITLE_START         13197 // "<cC>このアイテム<cE>はたべられません！"..
    #define SYSMSG_CANT_EAT_TEXT_START          13198 // "このアイテムをたべると\nおなかをこわしてしまい"...
    #define SYSMSG_SHOP_PRICE_FMT               13221 // "<cC>{0225}<u32>"
    #define SYSMSG_SHOP_BUY_CONFIRM             13222 // "この{041D}{02C6}を{065E}{02"...
    #define SYSMSG_BRAND_AREA_PROTECTED_FMT     13230 // "この{025A}{026B}は{02E9}{06"...
    #define SYSMSG_BRAND_AREA_UNAFFECTED        13231 // "この{025A}{026B}には\nブランドラン"...
    #define SYSMSG_SHOP_QUEST_ITEM              13235 // "<cC>クエストアイテム<cE>"
    #define SYSMSG_SHOP_TRADE_CONFIRM           13236 // "お{070D}{0733}ちのアイテムと<cC>"...
    #define SYSMSG_SHOP_BAG_FULL_TITLE          13237 // "<cC>{026B}{0733}{0290}オー"...
    #define SYSMSG_SHOP_BAG_FULL_TEXT           13238 // "お{070D}{0733}ちがいっぱいなので\n"...
    #define SYSMSG_SWAG_DESC_START              13240 // "{059A}{0272}の{024F}{0723"...
    #define SYSMSG_SWAG_NAMES_START             13390 // "あざやかな{05E8}"..
    #define SYSMSG_SWAG_TIPS_START              13540 // "あるアイテムを、もっとステキで{073A}{02"...
#endif

#endif // TWEWY_UTIL_SYSFONTMSGS_H
