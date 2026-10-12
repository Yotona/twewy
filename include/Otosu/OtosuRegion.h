#ifndef OTOSU_REGION_H
#define OTOSU_REGION_H

// BinIdentifier ids are the number of the overlay that owns the file.
#ifdef REGION_USA
    #define OTOSU_GAME_OVL_NUM 39
    #define OTOSU_MENU_OVL_NUM 43
#else
    #define OTOSU_GAME_OVL_NUM 38
    #define OTOSU_MENU_OVL_NUM 42
#endif

// Placeholder names follow USA addresses; JP builds map them to the JP addresses.
// func_0200d954 is left out: its JP counterpart is named func_0200d8f0, which is aliased below.
#ifndef REGION_USA
    #define data_0205c9b0 data_0205d03c
    #define data_02066cec data_0206754c
    #define data_020670ec data_0206794c
    #define data_020672ec data_02067b4c
    #define data_0206aa78 data_0206b2d8
    #define data_0206aa7c data_0206b2dc
    #define func_02001b44 func_02001ae0
    #define func_0200d1d8 func_0200d174
    #define func_0200d858 func_0200d7f4
    #define func_0200d898 func_0200d834
    #define func_0200d8f0 func_0200d88c
    #define func_0200d90c func_0200d8a8
    #define func_02025b68 func_02025b10
    #define func_02025e30 func_02025dd8
    #define func_020265d4 func_0202657c
    #define func_02027170 func_02027118
    #define func_0202b878 func_0202b820
    #define func_0203a96c func_0203b2d0
    #define func_0203aafc func_0203b460
    #define func_0203ab7c func_0203b4e0
    #define func_0203abec func_0203b550
    #define func_020442f8 func_02044ba0
    #define func_020471ec func_02047f30
    #define func_020472a8 func_02047ffc
    #define func_02047338 func_020480c0
    #define func_0204737c func_02048704
    #define func_02047e84 func_02048f24

    #define func_ov040_0209b8b8 func_ov039_0209c238
    #define func_ov040_0209ba04 func_ov039_0209c384
    #define func_ov040_0209c158 func_ov039_0209cad8
    #define func_ov040_0209caac func_ov039_0209d42c
    #define func_ov040_0209cabc func_ov039_0209d43c
    #define func_ov040_0209cb08 func_ov039_0209d488
    #define func_ov040_0209cb5c func_ov039_0209d4dc
    #define func_ov040_0209cb68 func_ov039_0209d4e8
    #define func_ov040_0209cb78 func_ov039_0209d4f8
    #define func_ov040_0209cb98 func_ov039_0209d518
    #define func_ov040_0209cb9c func_ov039_0209d51c
    #define func_ov040_0209cde4 func_ov039_0209d764
    #define func_ov040_0209cf20 func_ov039_0209d8a0
    #define func_ov040_0209d0a8 func_ov039_0209da28
    #define func_ov040_0209d290 func_ov039_0209dc10
    #define func_ov040_0209d40c func_ov039_0209dd8c
    #define func_ov040_0209d420 func_ov039_0209dda0
    #define func_ov040_0209d434 func_ov039_0209ddb4
    #define func_ov040_0209d48c func_ov039_0209de0c
    #define func_ov040_0209d4a4 func_ov039_0209de24
    #define func_ov040_0209d4c4 func_ov039_0209de44
    #define func_ov040_0209d540 func_ov039_0209dec0
    #define func_ov040_0209d588 func_ov039_0209df08
    #define func_ov040_0209d6cc func_ov039_0209e04c
    #define func_ov040_0209d728 func_ov039_0209e0a8
    #define func_ov040_0209d788 func_ov039_0209e108
    #define func_ov040_0209d818 func_ov039_0209e198
    #define func_ov040_0209d848 func_ov039_0209e1c8
    #define func_ov040_0209d970 func_ov039_0209e2f0
    #define func_ov040_0209d990 func_ov039_0209e310
    #define func_ov040_0209ec5c func_ov039_0209f5dc
    #define func_ov040_0209ece4 func_ov039_0209f664
    #define func_ov040_0209ece8 func_ov039_0209f668
    #define func_ov040_0209ed20 func_ov039_0209f6a0
    #define func_ov040_0209ed30 func_ov039_0209f6b0
    #define func_ov040_0209ed58 func_ov039_0209f6d8
    #define func_ov040_0209edc0 func_ov039_0209f740
    #define func_ov040_0209eef4 func_ov039_0209f874
    #define func_ov040_0209ef48 func_ov039_0209f8c8
    #define func_ov040_0209ef6c func_ov039_0209f8ec
    #define func_ov040_0209ef88 func_ov039_0209f908
    #define func_ov040_0209efc0 func_ov039_0209f940
#endif

#endif // OTOSU_REGION_H
