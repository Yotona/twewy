#ifndef INVENTORY_PIN_H
#define INVENTORY_PIN_H

#include <types.h>

typedef union {
    struct {
        u8 level  : 7;
        u8 unk_07 : 1;
    } bits;
    u8 raw;
} PinFlags;

/**
 * @brief Pin information as stored from within an external binary data file.
 */
typedef struct {
    /* 0x00 */ char unk_00[0x8];
    /* 0x08 */ u32  unk_08;
    /* 0x0C */ s16  unk_0C;
    /* 0x0E */ char unk_0E[0x25 - 0x0E];
    /* 0x25 */ u8   unk_25;
    /* 0x26 */ char unk_26[0x34 - 0x26];
} RawPinData; // Size: 0x34

typedef struct {
    /* 0x00 */ u16      pinID;
    /* 0x02 */ u16      battlePP;
    /* 0x04 */ u16      minglePP;
    /* 0x06 */ u16      shutdownPP;
    /* 0x08 */ u8       unk_08;
    /* 0x09 */ PinFlags flags;
} EquippedPin;

typedef struct {
    /* 0x00 */ u16      pinID;
    /* 0x02 */ u16      battlePP;
    /* 0x04 */ u16      minglePP;
    /* 0x06 */ u16      shutdownPP;
    /* 0x08 */ PinFlags flags;
} StockpilePin;

typedef struct {
    /* 0x0 */ u16      pinID;
    /* 0x2 */ u8       count;
    /* 0x3 */ PinFlags flags;
} MasteredPin;

typedef enum {
    // D+B
    PIN_ICE_BLOW      = 0,
    PIN_ICE_RISERS    = 1,
    PIN_FROZEN_COOL   = 2,
    PIN_BLIZZARD_COOL = 3,
    PIN_EARTHSHAKE    = 4,
    PIN_SEXY_D        = 5,
    PIN_SEXY_PLUS     = 6,
    PIN_SEXY_B        = 7,
    PIN_STRAITJACKET  = 8,
    PIN_BLOWN_KISS    = 9,
    PIN_D_PLUS_B      = 10,

    // Natural Puppy
    PIN_HAPPY_BEAM         = 11,
    PIN_NATURAL_MAGNUM     = 12,
    PIN_WONDER_MAGNUM      = 13,
    PIN_BEAUTY_LAUNCHER    = 14,
    PIN_ONE_LOVE_MAGNUM    = 15,
    PIN_HOUNDER_MAGNUM     = 16,
    PIN_BEAR_HUG_MAGNUM    = 17,
    PIN_SPARKLE_BEAM       = 18,
    PIN_LIL_TERROR_BAZOOKA = 19,
    PIN_ANGEL_MAGNUM       = 20,
    PIN_CUTIE_BEAM         = 21,
    PIN_LOVELY_BEAM        = 22,
    PIN_SEXY_BEAM          = 23,
    PIN_SUPERFINE_BEAM     = 24,
    PIN_PLAYMATE_BEAM      = 25,
    PIN_PURITY_LAUNCHER    = 26,
    PIN_INNOCENCE_BEAM     = 27,
    PIN_POP_PENDULUM       = 28,
    PIN_CASUAL_PENDULUM    = 29,
    PIN_LOVE_ME_TETHER     = 30,
    PIN_PAMPER_ME_TETHER   = 31,
    PIN_SWEET_TALK_TETHER  = 32,
    PIN_NATURAL_PUPPY      = 33,

    // Pavo Real
    PIN_FRANTIC         = 34,
    PIN_BLINK_STEP      = 35,
    PIN_FIRST_GEAR      = 36,
    PIN_TOP_GEAR        = 37,
    PIN_EGG_BOMB        = 38,
    PIN_METEOR_MAGNET   = 39,
    PIN_METEOR_HOOK     = 40,
    PIN_METEOR_SPIKE    = 41,
    PIN_STRONG_HEART    = 42,
    PIN_STRONG_SPIRIT   = 43,
    PIN_STRONG_BODY     = 44,
    PIN_STRONG_N_PROUD  = 45,
    PIN_POISON_SCORPION = 46,
    PIN_POISON_SKULL    = 47,
    PIN_POISON_BONES    = 48,
    PIN_POISON_CHAINS   = 49,
    PIN_RAPIER_GLANCE   = 50,
    PIN_LOVE_CHARGE     = 51,
    PIN_GUARDIAN_ANGEL  = 52,
    PIN_ARCHANGEL       = 53,
    PIN_ENERGY_FACTOR   = 54,
    PIN_SPEED_FACTOR    = 55,
    PIN_SUPPLY_FACTOR   = 56,
    PIN_CHAOS           = 57,
    PIN_PAVO_REAL       = 58,

    // Mus Rattus
    PIN_LIGHTNING_MOON   = 59,
    PIN_LIGHTNING_STORM  = 60,
    PIN_BURNING_CHERRY   = 61,
    PIN_BURNING_MELON    = 62,
    PIN_BURNING_BERRY    = 63,
    PIN_AQUA_MONSTER     = 64,
    PIN_AQUA_GHOST       = 65,
    PIN_AQUA_DEMON       = 66,
    PIN_CORNERED_RAT     = 67,
    PIN_BLAST_WARNING    = 68,
    PIN_CANDLE_SERVICE   = 69,
    PIN_SCHOCK_WARNING   = 70,
    PIN_KINETIC_WARNING  = 71,
    PIN_IMPACT_WARNING   = 72,
    PIN_STORM_WARNING    = 73,
    PIN_ELECTRIC_WARNING = 74,
    PIN_TELEPORT_WARNING = 75,
    PIN_SHOUT            = 76,
    PIN_PSYCH_SUPPORT    = 77,
    PIN_STOPPER_SPARK    = 78,
    PIN_MUS_RATTUS       = 79,

    // Jupiter of the Monkey
    PIN_MASAMUNE              = 80,
    PIN_ONIKIRI               = 81,
    PIN_NENEKIRI              = 82,
    PIN_KANESADA              = 83,
    PIN_ENJU                  = 84,
    PIN_YOSHIMITSU            = 85,
    PIN_ICHIMONJI             = 86,
    PIN_NIKKARI               = 87,
    PIN_MIKAZUKI              = 88,
    PIN_SHIRO                 = 89,
    PIN_UNJO                  = 90,
    PIN_IZANAGI               = 91,
    PIN_MURASAME              = 92,
    PIN_RAKUYO                = 93,
    PIN_KUSANAGI              = 94,
    PIN_OHABARI               = 95,
    PIN_ZANTETSU              = 96,
    PIN_MITAMA                = 97,
    PIN_IZANAMI               = 98,
    PIN_ONMYO                 = 99,
    PIN_KONOHANA              = 100,
    PIN_KONOHANA_SAKUYA       = 101,
    PIN_JUPITER_OF_THE_MONKEY = 102,

    // Hip Snake
    PIN_LONG_LIVE_THE_ROCK = 103,
    PIN_LONG_LIVE_THE_ICE  = 104,
    PIN_LONG_LIVE_THE_FLOE = 105,
    PIN_EYES_FULL_OF_HOPE  = 106,
    PIN_EYES_FULL_OF_LIGHT = 107,
    PIN_HOT_GAZE           = 108,
    PIN_SIZZLING_GAZE      = 109,
    PIN_COLD_STARE         = 110,
    PIN_ICY_STARE          = 111,
    PIN_WASSUP_THUNDER     = 112,
    PIN_WASSUP_LIGHTNING   = 113,
    PIN_WASSUP_VOLTAGE     = 114,
    PIN_INDESTRUCTO_MAN    = 115,
    PIN_HIP_SNAKE          = 116,

    // Sheep Heavenly
    PIN_OCTO_SQUEEZE          = 117,
    PIN_SNAIL_SQUEEZE         = 118,
    PIN_VACU_SQUEEZE          = 119,
    PIN_GIMME_DAT_SHEEP       = 120,
    PIN_GIMME_DAT_ELEPHANT    = 121,
    PIN_GIMME_DAT_HIPPO       = 122,
    PIN_CRACKLE_POP_BARRIER   = 123,
    PIN_SPLISH_SPLASH_BARRIER = 124,
    PIN_CREEPY_WEEPY_BARRIER  = 125,
    PIN_WHIRLYGIG_JUGGLE      = 126,
    PIN_HEALING_BUNNY         = 127,
    PIN_HEALING_FLAMINGO      = 128,
    PIN_HEALING_WHALE         = 129,
    PIN_CROC_LEISURE          = 130,
    PIN_MONKEY_LEISURE        = 131,
    PIN_BEAR_LEISURE          = 132,
    PIN_SQUIRREL_LEISURE      = 133,
    PIN_SPARKLE_CHARGE        = 134,
    PIN_HANDSFREE_HEALING     = 135,
    PIN_SHEEP_HEAVENLY        = 136,

    // Tigre Punks
    PIN_DISTORTION      = 137,
    PIN_VELOCITY_ATTACK = 138,
    PIN_VELOCITY_TACKLE = 139,
    PIN_DEMONS_HATRED   = 140,
    PIN_GO_2_HELL       = 141,
    PIN_GO_2_HEAVEN     = 142,
    PIN_LIGHTNING_ANGER = 143,
    PIN_LIGHTNING_RAGE  = 144,
    PIN_LIVE            = 145,
    PIN_PEACE_FULL      = 146,
    PIN_THANX           = 147,
    PIN_D_I_Y           = 148,
    PIN_TIGRE_PUNKS     = 149,

    // Dragon Couture
    PIN_ONE_STONE_MANY_BIRDS       = 150,
    PIN_ONE_STROKE_VAST_WEALTH     = 151,
    PIN_ONE_GRAIN_INFINITE_PROMISE = 152,
    PIN_SELF_FOUND_OTHERS_LOST     = 153,
    PIN_FIERY_SPIRIT_SPIRITED_FIRE = 154,
    PIN_FLAMES_AFAR_FOES_AFLAME    = 155,
    PIN_FINAL_PYRE_ALL_EXPIRED     = 156,
    PIN_BLUE_BLOOD_BURNS_BLUE      = 157,
    PIN_LIVE_SLOW_DIE_FAST         = 158,
    PIN_LOCAL_FIRE_DISTANT_SEA     = 159,
    PIN_JUNE_HEARTH_JANUARY_FAN    = 160,
    PIN_SWIFT_STORM_SWIFT_END      = 161,
    PIN_BLACK_SKY_WHITE_BOLT       = 162,
    PIN_FOLLOW_SUIT_FALL_IN_TURN   = 163,
    PIN_DRAGON_COUTURE             = 164,

    // Lapin Angelique
    PIN_LOLITA_BAT      = 165,
    PIN_SKULL_RABBIT    = 166,
    PIN_KALEIDOSCOPE    = 167,
    PIN_LOLITA_EMBLEM   = 168,
    PIN_LOLITA_CHOPPER  = 169,
    PIN_LINK_SPIDER     = 170,
    PIN_WEB_SPIDER      = 171,
    PIN_NETWORK_SPIDER  = 172,
    PIN_LOLITA_MIC      = 173,
    PIN_SPIDERS_SILK    = 174,
    PIN_LEOPARD         = 175,
    PIN_LOLITA_SKULL    = 176,
    PIN_BLACK_ROSE      = 177,
    PIN_LAPIN_ANGELIQUE = 178,

    // Pegaso
    PIN_THUNDER_PAWN       = 179,
    PIN_LIGHTNING_PAWN     = 180,
    PIN_AQUA_PAWN          = 181,
    PIN_SWING_BISHOP       = 182,
    PIN_PEGASO_BISHOP      = 183,
    PIN_KING_ARTHUR        = 184,
    PIN_EXCALIBUR          = 185,
    PIN_THUNDER_ROOK       = 186,
    PIN_LIGHTNING_ROOK     = 187,
    PIN_JACKS_KNIGHT       = 188,
    PIN_KINGS_KNIGHT       = 189,
    PIN_QUEENS_KNIGHT      = 190,
    PIN_HER_ROYAL_HIGHNESS = 191,
    PIN_PEGASO             = 192,

    // Wild Boar
    PIN_LAZY_BOMBER      = 193,
    PIN_FLOWER_OF_FLAME  = 194,
    PIN_FLOWER_OF_FIRE   = 195,
    PIN_KEWL_LINE        = 196,
    PIN_DOPE_LINE        = 197,
    PIN_WILD_LINE        = 198,
    PIN_FLY_LINE         = 199,
    PIN_FRESH_LINE       = 200,
    PIN_MICROCOSMIC_PULL = 201,
    PIN_COSMIC_PULL      = 202,
    PIN_MACROCOSMIC_PULL = 203,
    PIN_DISS             = 204,
    PIN_WILD_BOAR        = 205,

    // Gatito
    PIN_BIG_BANG              = 206,
    PIN_BIG_CRUNCH            = 207,
    PIN_SWIFT_AS_THE_WIND     = 208,
    PIN_HUSHED_AS_THE_WOOD    = 209,
    PIN_FIERCE_AS_THE_FLAME   = 210,
    PIN_STALWART_AS_THE_MOUNT = 211,
    PIN_BLACK_MARS            = 212,
    PIN_BLACK_JUPITER         = 213,
    PIN_BLACK_URANUS          = 214,
    PIN_BLACK_VENUS           = 215,
    PIN_BLACK_SATURN          = 216,
    PIN_BLACK_MERCURY         = 217,
    PIN_RIGHTY_CAT            = 218,
    PIN_BRAINY_CAT            = 219,
    PIN_LEFTY_CAT             = 220,
    PIN_ONE_JUMP_FROM_EDEN    = 221,
    PIN_ONE_SKIP_FROM_EDEN    = 222,
    PIN_ONE_HOP_FROM_EDEN     = 223,
    PIN_ONE_STEP_FROM_EDEN    = 224,
    PIN_EDENS_DOOR            = 225,
    PIN_VISIONARY_BLEND       = 226,
    PIN_RED_SKULL             = 227,
    PIN_GATITO                = 228,

    // Unbranded - Tin Pin
    PIN_TIN_PIN_FIRE      = 229,
    PIN_TIN_PIN_CUSTOM    = 230,
    PIN_TIN_PIN_WIND      = 231,
    PIN_TIN_PIN_BLADE     = 232,
    PIN_TIN_PIN_WHEEL     = 233,
    PIN_TIN_PIN_DASH      = 234,
    PIN_TIN_PIN_THRIFT    = 235,
    PIN_TIN_PIN_HELLFIRE  = 236,
    PIN_TIN_PIN_SUNSCORCH = 237,
    PIN_TIN_PIN_ROCKER    = 238,
    PIN_TIN_PIN_ARTIST    = 239,
    PIN_TIN_PIN_DEVIL     = 240,
    PIN_TIN_PIN_IFRIT     = 241,
    PIN_TIN_PIN_SHIVA     = 242,
    PIN_TIN_PIN_GOLEM     = 243,
    PIN_TIN_PIN_BAHAMUT   = 244,

    // Unbranded - Yen
    PIN_1_YEN     = 245,
    PIN_5_YEN     = 246,
    PIN_10_YEN    = 247,
    PIN_50_YEN    = 248,
    PIN_100_YEN   = 249,
    PIN_500_YEN   = 250,
    PIN_1000_YEN  = 251,
    PIN_5000_YEN  = 252,
    PIN_10000_YEN = 253,

    // Unbranded - Metals
    PIN_SCARLETITE    = 254,
    PIN_RARE_METAL    = 255,
    PIN_TEKTITE       = 256,
    PIN_ADAMANTITE    = 257,
    PIN_ORICHALCUM    = 258,
    PIN_SHADOW_MATTER = 259,
    PIN_DARK_MATTER   = 260,

    // Unbranded - Special Pins
    PIN_RHYME         = 261,
    PIN_LUCKY_STAR    = 262,
    PIN_LADY_LUCK     = 263,
    PIN_LUCKY_PANDA   = 264,
    PIN_PYROKINESIS   = 265,
    PIN_SHOCKWAVE     = 266,
    PIN_CURE_DRINK    = 267,
    PIN_FORCE_ROUNDS  = 268,
    PIN_PSYCHOKINESIS = 269,
    PIN_THUNDERBOLT   = 270,

    // Unbranded - Miscellaneous
    PIN_THE_EYES_HAVE_IT     = 271,
    PIN_I_LIVE_FOR_FOOD      = 272,
    PIN_WALK_ON_WALK_OFF     = 273,
    PIN_ME_TIME_IS_FREE_TIME = 274,
    PIN_FUJI_YAMA            = 275,
    PIN_DARUMA               = 276,
    PIN_SUSHI                = 277,
    PIN_SHINOBI              = 278,

    // Unbranded - Noise Pins
    PIN_TIGRIS   = 279,
    PIN_COREHOG  = 280,
    PIN_JELLY    = 281,
    PIN_SHARK    = 282,
    PIN_LEO      = 283,
    PIN_OVIS     = 284,
    PIN_CORNIX   = 285,
    PIN_POPGUIN  = 286,
    PIN_PTEROPUS = 287,
    PIN_MINK     = 288,
    PIN_BOOMER   = 289,
    PIN_SPROG    = 290,
    PIN_FOX      = 291,
    PIN_BAT      = 292,
    PIN_RHINO    = 293,
    PIN_SHREW    = 294,
    PIN_WOOLLY   = 295,
    PIN_RAVEN    = 296,
    PIN_DRAKE    = 297,
    PIN_ANGUIS   = 298,
    PIN_PIG      = 299,
    PIN_CARCIN   = 300,
    PIN_GRIZZLY  = 301,
    PIN_WOLF     = 302,
    PIN_FROG     = 303,
} PinID;

#define GLOBAL_ITEMID_PIN_OFFSET 0

#endif // INVENTORY_PIN_H