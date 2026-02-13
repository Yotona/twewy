/**
 * @file Debug/Kitawaki/SoundTest.c
 * @brief Debug Menu K-4 ~ Sound Test
 */

#include "Debug/SoundTest.h"
#include "CriSndMgr.h"
#include "Display.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "SndMgr.h"
#include "System.h"
#include "common_data.h"
#include <registers.h>
#include <types.h>

void func_ov029_020833c4(void);

const char* data_ov029_02083400 = "Seq_SoundTest()";

typedef void (*SoundEffectFunc)(SoundTestState* state);

// Nonmatching: Built order differs
const char* soundEffects[1388] = {
    "SE_battle_in",
    "SE_battle_out",
    "SE_pause",
    "SE_foot01",
    "SE_foot02",
    "SE_field_wo",
    "SE_noise_check",
    "SE_noise_check02",
    "SE_noise_check03",
    "SE_noise_check04",
    "SE_noise_check05",
    "SE_noise_check06",
    "SE_noise_check07",
    "SE_noise_check08",
    "SE_noise_check09",
    "SE_peneffectS",
    "SE_peneffectM",
    "SE_peneffectL",
    "SE_enemy_appear",
    "SE_enemy_disappear",
    "SE_ready01",
    "SE_ready02",
    "SE_noiseerased01",
    "SE_noiseerased02",
    "SE_gameover",
    "SE_quickjump01",
    "SE_quickjump02",
    "SE_quickjump03",
    "SE_flash",
    "SE_warning",
    "SE_finishmove",
    "SE_finishmove_hit",
    "SE_green_light",
    "SE_green_light02",
    "SE_appear_version",
    "SE_obje_car",
    "SE_obje_corn",
    "Btl_result_badgeget",
    "Btl_result_badgeup",
    "Btl_result_counterloop",
    "Btl_result_counterstop",
    "Btl_result_rank",
    "Btl_system_cursor",
    "Btl_system_execute",
    "SE_coin_get",
    "SE_psy_full",
    "SE_psy_empty",
    "SE_obj_SS01",
    "SE_obj_S01",
    "SE_obj_M01",
    "SE_obj_L01",
    "SE_weapon_switch",
    "SE_psy_icepillar01",
    "SE_psy_psycofall01",
    "SE_psy_psycofall01_ground",
    "SE_psy_swordpierce01",
    "SE_psy_swordpierce01_hit",
    "SE_psy_shouryuuken01",
    "SE_psy_shouryuuken01_hit",
    "SE_psy_teleport01_01",
    "SE_psy_teleport01_02",
    "SE_psy_psycoterritory01_01",
    "SE_psy_psycoterritory01_02",
    "SE_psy_psycoterritory01_hit",
    "SE_psy_objbomb01_01",
    "SE_psy_objbomb01_01_cancel",
    "SE_psy_objbomb01_02",
    "SE_psy_objbomb01_03",
    "SE_psy_objbomb01_04",
    "SE_psy_fireball01_01",
    "SE_psy_fireball01_02",
    "SE_psy_fireball01_03",
    "SE_psy_fireball01_hit",
    "SE_psy_fireball02_01",
    "SE_psy_fireball02_02",
    "SE_psy_fireball02_hit",
    "SE_psy_waterball01_01",
    "SE_psy_waterball01_02",
    "SE_psy_waterball01_hit",
    "SE_psy_opticalhold01_01",
    "SE_psy_opticalhold01_02",
    "SE_psy_opticalhold01_hit",
    "SE_psy_fireworks01_01",
    "SE_psy_fireworks01_01_cancel",
    "SE_psy_fireworks01_02",
    "SE_psy_fireworks01_03",
    "SE_psy_fireworks01_hit",
    "SE_psy_boomerang01",
    "SE_psy_boomerang01_hit",
    "SE_psy_psycothunder01",
    "SE_psy_streetlive01",
    "SE_psy_psycokinesis01",
    "SE_psy_psycokinesis01_cancel",
    "SE_psy_psycokinesis02",
    "SE_psy_rockbind01",
    "SE_psy_rockbind02",
    "SE_psy_rockbind03",
    "SE_psy_icebind01",
    "SE_psy_icebind02",
    "SE_psy_icebind03",
    "SE_psy_velocityattack01",
    "SE_psy_velocityattack_hit",
    "SE_psy_rockonthunder_drag01",
    "SE_psy_sycokinesis_s",
    "SE_psy_burstshot01",
    "SE_psy_burstshot01_02",
    "SE_psy_burstshot01_hit",
    "SE_psy_sycoquake01",
    "SE_psy_straightshot01",
    "SE_psy_straightshot01_hit",
    "SE_psy_burstshot01_03",
    "SE_boss_dyingfx01",
    "SE_boss_dyingfx02",
    "SE_btleffect_ug_slash_s",
    "SE_btleffect_ug_slash_l",
    "SE_btleffect_ug_strike_s",
    "SE_btleffect_ug_strike_l",
    "SE_btleffect_ug_poke_s",
    "SE_btleffect_ug_poke_l",
    "SE_btleffect_ug_enemy_s",
    "SE_btleffect_ug_enemy_l",
    "SE_btleffect_ug_bomb_s",
    "SE_btleffect_ug_bomb_l",
    "SE_btleffect_ug_light",
    "SE_btleffect_ug_ice",
    "SE_btleffect_ug_water",
    "SE_btleffect_ug_fire",
    "SE_btleffect_ug_bluefire",
    "SE_btleffect_ug_dark",
    "SE_btleffect_ug_thunder",
    "SE_btleffect_ug_black",
    "Btl_result_evo_flash",
    "Btl_result_evo_lvup",
    "SE_partner_guard",
    "Btl_system_food",
    "SE_psy_pirrar_nobiru01_01",
    "SE_psy_pirrar_tobu01_01",
    "SE_psy_typhoon_blow01_01",
    "SE_psy_typhoon_blow01_hit",
    "SE_psy_beat_stab01_01",
    "SE_psy_downstab_stab01_01",
    "SE_psy_downstab_stab01_02",
    "SE_psy_piercestab_stab01_01",
    "SE_psy_rollingcut_cut01_01",
    "SE_psy_firehold_fire01_01",
    "SE_psy_icehold_boot01_01",
    "SE_psy_icehold_hold01_01",
    "SE_psy_blackhall_hall01_01",
    "SE_psy_darkseald_boot01_01",
    "SE_psy_darkseald_dark01_01",
    "SE_psy_psycobomb_bomb01_01",
    "SE_psy_psycobomb_bomb01_02",
    "SE_psy_psycobomb_bomb01_03",
    "SE_psy_egenerate_boot01_01",
    "SE_psy_hvsword_swish01_01",
    "SE_psy_badge_push",
    "SE_psy_psycokinesis03_01",
    "SE_psy_psycokinesis03_02",
    "SE_psy_psycokinesis03_03",
    "SE_psy_hado_01_01",
    "SE_psy_hado_01_02",
    "SE_psy_rhyme_fly01_01",
    "SE_psy_rhyme_fly01_02",
    "SE_psy_rhyme_fly01_03",
    "SE_psy_straightshot02_01",
    "SE_psy_straightshot02_02",
    "SE_psy_straightshot03",
    "SE_psy_straightshot04",
    "SE_psy_straightshot05",
    "SE_psy_straightshot06",
    "SE_psy_straightshot07",
    "SE_psy_psycofall02",
    "SE_psy_firehold_fire01_02",
    "SE_psy_psycobomb_bomb01_hit",
    "SE_psy_egenerate_shot01_01",
    "SE_psy_rhyme_fly01_hit",
    "SE_psy_waterbarrier_boot01",
    "SE_psy_waterbarrier_fx01",
    "SE_psy_firebarrier_boot01",
    "SE_psy_firebarrier_fx01",
    "SE_psy_badgebeam01",
    "SE_psy_hvsword_tame01_01",
    "SE_psy_hvsword_tame01_02",
    "SE_rensen_whiteout",
    "SE_scanS_open",
    "SE_scanS_close",
    "SE_scanL_open",
    "SE_scanL_close",
    "SE_scanR_open",
    "SE_scanR_hit",
    "SE_scan_stop",
    "SE_dialog_cutin",
    "SE_dialog_box",
    "SE_dialog_boxL",
    "SE_dialog_next",
    "SE_dialog_sign",
    "SE_keyword_switch",
    "SE_item_twinkle_loop",
    "SE_item_twinkle_stop",
    "SE_hachikou_touch",
    "SE_mapjump_guard",
    "SE_get_item",
    "SE_get_keyword",
    "FD_parasite_bomb",
    "FD_game_start",
    "FD_flash",
    "BG_HWV_IMD",
    "BG_A109_IMD",
    "BG_SPAIN_IMD73",
    "EV_se_mail",
    "EV_se_timer_flash",
    "EV_se_timer",
    "EV_se_ene_appear",
    "EV_se_mail_erase",
    "EV_se_barrier_hit",
    "EV_se_timer_disappear",
    "EV_se_trainBG01",
    "EV_se_trainBG02",
    "EV_se_noise_disappear",
    "EV_se_noise_approach_looped",
    "EV_se_timer_looped",
    "EV_se_mail_others",
    "EV_se_lime_disappear",
    "EV_se_tel_ring_looped",
    "EV_se_tel_ring_stop",
    "EV_se_lightnning",
    "EV_se_key_get",
    "EV_se_mainpower01",
    "EV_se_mainpower02",
    "EV_se_cheer",
    "EV_se_footsteps",
    "EV_se_loop_stop",
    "EV_se_badge",
    "EV_se_BGnoise01",
    "EV_se_BGnoise02",
    "EV_se_barrier_cancel",
    "EV_se_mail_keyword",
    "EV_se_exclamation",
    "EV_se_shutter",
    "EV_se_detection_on",
    "EV_se_detection_low",
    "EV_se_detection_high",
    "EV_se_headache",
    "EV_se_agreement",
    "EV_se_shiki_warp",
    "EV_se_battle_allarm",
    "EV_se_bossnoise_diaappear",
    "EV_se_EVpre_expl",
    "EV_se_EV_expl",
    "EV_se_ene_appear_SP",
    "EV_se_NPC_hit",
    "EV_se_pendant",
    "EV_se_entryfee",
    "EV_se_choker",
    "EV_se_negative_people",
    "EV_se_gra082",
    "EV_se_cell_call",
    "EV_se_barriar_break",
    "EV_se_brainwash",
    "EV_se_badge_redskull",
    "BG_city_ambiS",
    "BG_city_ambiM",
    "BG_city_ambiL",
    "BG_city_train",
    "BG_city_train_ea001RtoL",
    "BG_city_train_ea001LtoR",
    "BG_city_train_no010RtoL",
    "BG_city_train_no010LtoR",
    "BG_city_carpass_slow",
    "BG_city_carpass_fast",
    "BG_city_car_slow",
    "BG_city_water_sewer",
    "BG_city_water_room",
    "SN_se_Nzsymbol_appear",
    "SN_se_Nztriger_touch",
    "SN_se_Nzsymbol_activetouch",
    "SN_se_Nz_chain",
    "SN_se_Nz_luck_rise",
    "Menu_Menter",
    "Menu_Mexit",
    "Menu_Mscratch",
    "Menu_Msystem_scroll",
    "Menu_Msystem_cursor",
    "Menu_Msystem_return",
    "Menu_Msystem_execute",
    "EV_se_whiteoutS",
    "EV_se_whiteoutM",
    "EV_se_whiteoutL",
    "EV_se_select_appear",
    "DEV_se_scanbadge_appear01",
    "DEV_se_scanbadge_appear02",
    "DEV_se_scanbadge_susten",
    "DEV_se_scanbadge_susten_touch",
    "BG_city_ele_arrivalsign",
    "BG_city_ele_open",
    "BG_city_ele_close",
    "Menu_Msystem_click01",
    "Menu_Msystem_click02",
    "DEV_se_scanbadge_touch",
    "DEV_se_surikomibadge_appear",
    "DEV_se_surikomibadge_touch",
    "DEV_se_surikomibadge_cursor",
    "DEV_se_surikomibadge_execute",
    "DEV_se_surikomibadge_complete",
    "SE_jyoukyuu01_door",
    "SG_shinigamisan_touch",
    "SG_shinigamisan_release",
    "SG_shinigamisan_kettei",
    "EV_se_reserve01",
    "EV_se_reserve02",
    "EV_se_reserve03",
    "EV_se_reserve04",
    "EV_se_reserve05",
    "EV_se_reserve06",
    "EV_se_reserve07",
    "EV_se_reserve08",
    "EV_se_reserve09",
    "EV_se_reserve10",
    "Menu_shop_ability",
    "Menu_shop_lvup",
    "Menu_save",
    "Menu_Msystem_kinshi",
    "Menu_Msystem_stock_master_in",
    "SN_se_NzsymbolButa",
    "SN_se_NzsymbolButa_appear",
    "SE_boy_swishhit",
    "SE_boy_swishA",
    "SE_boy_swishB",
    "SE_boy_swishHit2",
    "SE_boy_fireS",
    "SE_boy_fireE",
    "SE_boy_thunder",
    "SE_boy_thunder_cursor",
    "SE_boy_dart",
    "SE_boy_recover_drink",
    "SE_boy_levelup",
    "SE_boy_float",
    "SE_girl_dart_throw",
    "SE_girl_dart_hit",
    "SE_girl_dart_hit2",
    "SE_girl_dart_cursor01",
    "SE_girl_dart_cursor02",
    "SE_girl_ESP_full",
    "SE_girl_bear_attack",
    "SE_girl_bear_attackSP",
    "SE_girl_SP_Lv2_01",
    "SE_girl_SP_Lv2_02",
    "SE_girl_SP_Lv3_01",
    "SE_girl_SP_Lv3_02",
    "SE_girl_SP_Lv3_03",
    "SE_girl_SP_Lv3_04",
    "SE_girl_SP_Lv3_05",
    "SE_girl_SP_Lv3_06",
    "SE_girl_airattack",
    "SE_girl_airattack_hit",
    "SE_girl_ayatsu_teleport_UP",
    "SE_girl_ayatsu_teleport_DOWN",
    "SE_girl_appear",
    "SE_girl_ayatsu_blackshadow",
    "SE_joshua_beam",
    "SE_joshua_beam_angel",
    "SE_joshua_beam_SP",
    "SE_joshua_obj_fall",
    "SE_joshua_swish",
    "SE_joshua_teleport_UP",
    "SE_joshua_teleport_DOWN",
    "SE_joshua_SP_common",
    "SE_joshua_SP_Lv2_01",
    "SE_joshua_SP_Lv2_02",
    "SE_joshua_SP_Lv3_01",
    "SE_joshua_SP_Lv3_02",
    "SE_joshua_SP_Lv3_03",
    "SE_joshua_SP_Lv1_01",
    "SE_joshua_gekisha",
    "SE_beat_SP_Lv3_01",
    "SE_beat_SP_Lv3_02",
    "SE_beat_SP_Lv3_03",
    "SE_beat_SP_Lv3_hit",
    "SE_beat_effect_atk0",
    "SE_beat_effect_atk1",
    "SE_beat_effect_atk2",
    "SE_beat_effect_atk3",
    "SE_beat_SP_Lv2_01",
    "SE_beat_SP_Lv2_hit",
    "SE_beat_skate",
    "SE_frog_appear",
    "SE_frog_JumpS",
    "SE_frog_JumpL",
    "SE_frog_Jumphit",
    "SE_frog_Jumpkick",
    "SE_frog_land",
    "SE_frog_voc_attack",
    "SE_frog_voc_damage",
    "SE_frog_voc_dying",
    "SE_frog_voc_idle",
    "SE_frog_babble",
    "SE_otama_redeye",
    "SE_otama_jump01",
    "SE_otama_jump02",
    "SE_otama_rollingdamage",
    "SE_otama_rollingaround",
    "SE_otama_voc_dying",
    "SE_wolf_appear",
    "SE_wolf_transfer",
    "SE_wolf_foot",
    "SE_wolf_tail",
    "SE_wolf_tranfer_hit",
    "SE_wolf_jump_hit",
    "SE_wolf_voc_attack",
    "SE_wolf_voc_damage",
    "SE_wolf_voc_dying",
    "SE_bear_appear",
    "SE_bear_attack",
    "SE_bear_attackSP",
    "SE_bear_attackhit",
    "SE_bear_voc_attack",
    "SE_bear_voc_damage",
    "SE_bear_voc_dying",
    "SE_bear_preattacknail",
    "SE_bear_preattackjump",
    "SE_bear_voc_shout",
    "SE_sbat_spark",
    "SE_sbat_voc_slowattack",
    "SE_sbat_voc_fastattack",
    "SE_sbat_hit",
    "SE_sbat_voc_damage",
    "SE_sbat_voc_dying",
    "SE_sbat_rollingattack01",
    "SE_sbat_rollingattack02",
    "SE_sbat_firstappear",
    "SE_sbat_appear",
    "SE_sbat_appearalone",
    "SE_sbat_spotlight",
    "SE_sbat_appear02",
    "SE_lbat_flap",
    "SE_lbat_flap_small",
    "SE_lbat_flap_large",
    "SE_lbat_flap_large_hit",
    "SE_lbat_beam",
    "SE_lbat_hit",
    "SE_lbat_voc_attack",
    "SE_lbat_voc_damage",
    "SE_lbat_voc_dying",
    "SE_lbat_upper",
    "SE_lbat_strechFX",
    "SE_lbat_flyingattack",
    "SE_lbat_crossflap",
    "SE_lbat_crossinitial",
    "SE_lbat_SP",
    "SE_lbat_Sflying01",
    "SE_lbat_Sflying02",
    "SE_lbat_hang",
    "SE_lbat_dyingfx01",
    "SE_lbat_dyingfx02",
    "SE_006_appear",
    "SE_006_voc_damage",
    "SE_006_voc_dying",
    "SE_006_voc_bite",
    "SE_006_bite",
    "SE_006_splush_SP",
    "SE_006_splush",
    "SE_006_water_sign",
    "SE_006_water_moveS",
    "SE_006_water_moveL",
    "SE_006_water_swirl",
    "SE_006_water_in",
    "SE_010_appear",
    "SE_010_voc_attack",
    "SE_010_voc_damage",
    "SE_010_voc_dying",
    "SE_010_footstep",
    "SE_010_tailattack",
    "SE_010_tailattack_hit",
    "SE_010_bubbleattack",
    "SE_010_bubbleattack_hit",
    "SE_010_fireattack",
    "SE_010_fireattack_hit",
    "SE_010_lazerattack",
    "SE_010_spiralattack",
    "SE_010_tatoo_kiri",
    "SE_010_tatoo_tobashi01",
    "SE_010_tatoo_tobashi02",
    "SE_010_tosshin_kidou",
    "SE_010_tosshin_tobu",
    "SE_010_tosshin_idou",
    "SE_010_lazerattack_tame",
    "SE_011_appear",
    "SE_011_voc_damage",
    "SE_011_voc_dying",
    "SE_011_idle",
    "SE_011_move",
    "SE_011_foot",
    "SE_011_wing",
    "SE_011_move_hit",
    "SE_011_foot_hit",
    "SE_011_wing_hit",
    "SE_011_badge_get",
    "SE_011_obj_fall",
    "SE_011B_sp01",
    "SE_011B_obj_catch",
    "SE_011B_obj_fall",
    "SE_011B_obj_ground",
    "SE_012_appear",
    "SE_012_voc_damage",
    "SE_012_voc_dying",
    "SE_012_idlestart",
    "SE_012_idleend",
    "SE_012_attack00",
    "SE_012_attack00hit",
    "SE_012_attack01",
    "SE_012_attack01hit",
    "SE_012_movestart",
    "SE_012_moveend",
    "SE_012_attack00_01",
    "SE_012_tornade_start",
    "SE_012_tornade_end",
    "SE_013_appear",
    "SE_013_voc_attack",
    "SE_013_voc_damage",
    "SE_013_voc_dying",
    "SE_013_needleFX",
    "SE_013_needlehit",
    "SE_013_bomb_out",
    "SE_013_bomb_drop",
    "SE_013_bomb_stick",
    "SE_013_bomb_targetS",
    "SE_013_bomb_targetM",
    "SE_013_bomb_targetL",
    "SE_013_bomb_timer01",
    "SE_013_bomb_timer02",
    "SE_013_bomb_expS",
    "SE_013_bomb_expM",
    "SE_013_bomb_expL",
    "SE_014_appear",
    "SE_014_voc_damage",
    "SE_014_voc_dying",
    "SE_014_footstep",
    "SE_014_hornattack",
    "SE_014_hornattack_hit",
    "SE_014_tatakitsuke_swish",
    "SE_014_tatakitsuke_hit",
    "SE_014_tatakitsuke_hitSP",
    "SE_014_guard",
    "SE_015_appear",
    "SE_015_voc_attack",
    "SE_015_voc_damage",
    "SE_015_voc_dying",
    "SE_015_footstep",
    "SE_015_noseattack",
    "SE_015_noseattack_hit",
    "SE_015_footstamp01",
    "SE_015_footstamp02",
    "SE_015_footstamp_hit",
    "SE_019_appear",
    "SE_019_voc_damage",
    "SE_019_voc_dying",
    "SE_019_tentcle_open_start",
    "SE_019_tentcle_open_decay",
    "SE_019_move",
    "SE_019_segment_start",
    "SE_019_segment_end",
    "SE_019_needleA",
    "SE_019_needleB01",
    "SE_019_needleB02",
    "SE_019_slowflash",
    "SE_019_tentcle_open_hit",
    "SE_019_needle_hit",
    "SE_019_dengeki",
    "SE_021_appear",
    "SE_021_voc_attack",
    "SE_021_voc_damage",
    "SE_021_voc_dying",
    "SE_021_flipper",
    "SE_021_flipperjump",
    "SE_021_flipperhit",
    "SE_021_gyorai",
    "SE_021_gyoraijump",
    "SE_021_gyoraihit",
    "SE_022_voc_attack",
    "SE_022_voc_damage",
    "SE_022_voc_dying",
    "SE_022_holeout01",
    "SE_022_holeout02",
    "SE_022_holein01",
    "SE_022_holein02",
    "SE_022_bomb_throw",
    "SE_022_bomb_pre",
    "SE_022_bomb_exp",
    "SE_022_swingattack",
    "SE_022_swinghit",
    "SE_022_dril_jump",
    "SE_022_dril_screw",
    "SE_022_dril_hole",
    "SE_022_dril_holein",
    "SE_022_dril_hit",
    "SE_022_dril_grind",
    "SE_023_appear",
    "SE_023_voc_damage",
    "SE_023_voc_dying",
    "SE_023_otakebi",
    "SE_023_mushroom01",
    "SE_023_change01",
    "SE_023_change02",
    "SE_023_moyai_fall",
    "SE_023_moyai_ground",
    "SE_023_gasha_suikomi",
    "SE_023_hitodama_appear",
    "SE_023_jitodama_roll",
    "SE_023_baha01_whiteout",
    "SE_023_baha02_move01",
    "SE_023_baha03_move02",
    "SE_023_baha04_beam01",
    "SE_023_baha05_beam02",
    "SE_023_baha05_beam03",
    "SE_024_appear",
    "SE_024_voc_damage",
    "SE_024_voc_dying",
    "SE_024_child_out",
    "SE_024_child_in",
    "SE_024_jump",
    "SE_024_land",
    "SE_024_jumpkick",
    "SE_024_upjump_jump",
    "SE_024_upjump_preland",
    "SE_024_upjump_land",
    "SE_024_target",
    "SE_024_upper",
    "SE_024_jumpkick_hit",
    "SE_024_upper_hit",
    "SE_026_appear",
    "SE_026_voc_damage",
    "SE_026_voc_dying",
    "SE_026_voc_odoroki",
    "SE_026_voc_ibiki",
    "SE_026_voc_escape",
    "SE_026_fs_walk",
    "SE_026_fs_run",
    "SE_026_timer",
    "SE_026_timer0",
    "SE_026_move_quick",
    "SE_026_tosshin",
    "SE_026_invisible",
    "SE_044_appear",
    "SE_044_voc_damage",
    "SE_044_voc_dying",
    "SE_044_move",
    "SE_044_guard",
    "SE_044_guard_hit",
    "SE_044_attack0",
    "SE_044_attack0_hit",
    "SE_044_attack2_A01",
    "SE_044_attack2_A01_hit",
    "SE_044_attack2_A02",
    "SE_044_attack2_A02_hit",
    "SE_044_attack1_B",
    "SE_044_attack1_B_hit",
    "SE_044_attack3_C01",
    "SE_044_attack3_C02",
    "SE_044_attack3_C_hit",
    "SE_044_attack4_D_start",
    "SE_044_attack4_D_end",
    "SE_044_attack4_D_hit",
    "SE_044_boomerang",
    "SE_B000_appear",
    "SE_B000_shadow",
    "SE_B000_firing_single",
    "SE_B000_firing_wave",
    "SE_B000_firing_machine",
    "SE_B000_firing_shot",
    "SE_B000_move",
    "SE_B001_appear",
    "SE_B001_charge01",
    "SE_B001_charge02",
    "SE_B001_charge03",
    "SE_B001_focus",
    "SE_B001_machine_gun",
    "SE_B001_option01",
    "SE_B001_option02",
    "SE_B001_kage_machine_gun",
    "SE_B001_kuukann_tenni_disappear",
    "SE_B001_kuukann_tenni_appear",
    "SE_B001_move",
    "SE_B001_SP01",
    "SE_B001_SP02",
    "SE_B002_appear",
    "SE_B002_blossom",
    "SE_B002_fuku_machine_gun",
    "SE_B002_homing",
    "SE_B002_kaifuku",
    "SE_B002_kage_blossom",
    "SE_B002_kage_fuku_machine_gun",
    "SE_B002_kage_homing",
    "SE_B002_kuukann_tenni_disappear",
    "SE_B002_kuukann_tenni_appear",
    "SE_B002_move",
    "SE_B003_appear",
    "SE_B003_move",
    "SE_B003_jump_attack",
    "SE_B003_jump_attack_hit",
    "SE_B003_kinkyori_kougeki",
    "SE_B003_tobi_dougu",
    "SE_B003_tsuibi_kougeki",
    "SE_B003_kuukann_tenni_disappear",
    "SE_B003_kuukann_tenni_appear",
    "SE_B003_skate",
    "SE_B004_rain",
    "SE_B004_thunder01",
    "SE_B004_voc_attack01",
    "SE_B004_voc_attack02",
    "SE_B004_voc_dying",
    "SE_B004_voc_idle",
    "SE_B004_voc_pre_attack",
    "SE_B004_punch_ground",
    "SE_B004_punch_ground_hit",
    "SE_B004_punch_big",
    "SE_B004_punch_big_hit",
    "SE_B004_big_swish",
    "SE_B004_big_swish_hit",
    "SE_B004_vacuum",
    "SE_B004_SPattack01",
    "SE_B004_SPattack02",
    "SE_B004_SPattack03",
    "SE_B004_SPattack04",
    "SE_B004_SPattack_hit",
    "SE_B004_flash01",
    "SE_B004_flash02",
    "SE_B005_appear",
    "SE_B005_badge_nage01",
    "SE_B005_firing_split",
    "SE_B005_firing_shot",
    "SE_B005_firing_spiral",
    "SE_B005_kuukann_tenni_disappear",
    "SE_B005_kuukann_tenni_appear",
    "SE_B005_tsuujyou_idou",
    "SE_B005_Lion_ningen_henshin",
    "SE_B005_ningen_lion_henshin",
    "SE_B005_kindan_noise_bullet01",
    "SE_B005_kindan_noise_bullet02",
    "SE_B005_firing_rapid_slow",
    "SE_B005_firing_burst",
    "SE_B006_kinnkyori_kougeki_S",
    "SE_B006_kinnkyori_kougeki_L",
    "SE_B006_tukamikougeki_01",
    "SE_B006_tukamikougeki_02",
    "SE_B006_kuukann_tenni_disappear",
    "SE_B006_kuukann_tenni_appear",
    "SE_B007_kinnkyori_kougeki_S",
    "SE_B007_kinnkyori_kougeki_L",
    "SE_B007_BLion_tosshin_kougeki",
    "SE_B006_appear",
    "SE_B007_appear",
    "SE_B006_footstep",
    "SE_B006_window_noise",
    "SE_B005_ningen_lion_henshin02",
    "SE_B008_appear",
    "SE_B008_kagebunnshinA_hassei",
    "SE_B008_kagebunnshinA_tobashi",
    "SE_B008_kagebunnshinB_hassei",
    "SE_B008_jittaika",
    "SE_B008_jittaika_modoru",
    "SE_B008_kage_fumi",
    "SE_B008_kage_fumare",
    "SE_B008_chaos_fukiagari",
    "SE_B008_chaos_S",
    "SE_B008_chaos_L",
    "SE_B008_kagebunnshinB_shoumetsu",
    "SE_B008_genkakuhoukai",
    "SE_B008_kuukann_tenni_disappear",
    "SE_B008_kuukann_tenni_appear",
    "SE_B008_chaos_S02",
    "SE_B008_chaos_L02",
    "SE_B008_badge",
    "SE_B009_appear",
    "SE_B009_genelator01",
    "SE_B009_genelator02",
    "SE_B009_hebi_dashi01",
    "SE_B009_hebi_dashi02",
    "SE_B009_jikan_teisi01",
    "SE_B009_jikan_teisi02",
    "SE_B009_wind_mill01",
    "SE_B009_wind_mill02",
    "SE_B009_move",
    "SE_B009_shot_lazer",
    "SE_B009_rootlazer01",
    "SE_B009_rootlazer02",
    "SE_B009_rootlazer03",
    "SE_B010_atama_idou",
    "SE_B010_atama_modori",
    "SE_B010_shita_sashi",
    "SE_B010_tama_haki",
    "SE_B010_kamituki",
    "SE_B010_sword_bomb01",
    "SE_B010_sword_bomb02",
    "SE_B010_laser",
    "SE_B010_doutai",
    "SE_B011_sword_bomb01",
    "SE_B011_sword_bomb02",
    "SE_B011_sword_bomb03",
    "SE_B011_sword_bomb04",
    "SE_B011_sword_bomb05",
    "SE_B011_tama_haki",
    "SE_B011_laser",
    "SE_B011_kousoku_dan01",
    "SE_B011_kousoku_dan02",
    "SE_B011_kousoku_dan03",
    "SE_B011_koukyuu_okuri",
    "SE_B011_kamituki",
    "SE_B011_dying_badge01",
    "SE_B011_dying_badge02",
    "SE_B011_dying_badge03",
    "SE_B011_dying_badge04",
    "SE_B011_koukyuu_kyuushuu01",
    "SE_B011_koukyuu_kyuushuu02",
    "SE_baybadge_arrow",
    "SE_baybadge_move",
    "SE_baybadge_hit",
    "SE_baybadge_piyori",
    "SE_baybadge_meteor01",
    "SE_baybadge_meteor02",
    "SE_baybadge_meteor_hit",
    "SE_baybadge_needle01",
    "SE_baybadge_needle02",
    "SE_baybadge_needle03",
    "SE_baybadge_needle_hit",
    "SE_baybadge_hummer01",
    "SE_baybadge_hummer02",
    "SE_baybadge_hummer03",
    "SE_baybadge_hummer_hit",
    "SE_baybadge_disappear",
    "SE_baybadge_piyori_hit",
    "SE_baybadge_countdown",
    "SE_baybadge_alive",
    "SE_baybadge_Bpget",
    "SE_baybadge_meteor_ground",
    "SE_baybadge_turn01",
    "SE_baybadge_warp",
    "SE_baybadge_jump",
    "SE_baybadge_accel",
    "SE_baybadge_menu_cursor",
    "SE_baybadge_menu_execute",
    "SE_baybadge_menu_cancel",
    "SE_baybadge_start01",
    "SE_baybadge_start02",
    "SE_baybadge_finish01",
    "SE_baybadge_finish02",
    "SE_baybadge_move_tame",
    "SE_baybadge_turn02",
    "neku_btl_op01",
    "neku_btl_op03",
    "neku_btl_op_danger01",
    "neku_btl_atc01",
    "neku_btl_atc02",
    "neku_btl_atc03",
    "neku_btl_atc04",
    "neku_btl_atc05",
    "neku_btl_atc06",
    "neku_btl_atc07",
    "neku_btl_atc08",
    "neku_btl_fb01",
    "neku_btl_fb02",
    "neku_btl_fb03",
    "neku_btl_fb04",
    "neku_btl_fb05",
    "neku_btl_fb06",
    "neku_btl_fb07",
    "neku_btl_fb08",
    "neku_btl_fb09",
    "neku_btl_fb_tame01",
    "neku_btl_counter01",
    "neku_btl_cat01",
    "neku_btl_cat02",
    "neku_btl_prs01",
    "neku_btl_prs02",
    "neku_btl_kizukai01",
    "neku_btl_dmg01",
    "neku_btl_dmg02",
    "neku_btl_dmg03",
    "neku_btl_dmg_L01",
    "neku_btl_dmg_L02",
    "neku_btl_pnc01",
    "neku_btl_ovr01",
    "neku_btl_ovr02",
    "neku_btl_win01",
    "neku_btl_win02",
    "neku_btl_win03",
    "neku_btl_win04",
    "neku_btl_rizalt01",
    "neku_btl_rizalt02",
    "shiki_btl_op01",
    "shiki_btl_op03",
    "shiki_btl_op_danger01",
    "shiki_btl_atc01",
    "shiki_btl_atc02",
    "shiki_btl_atc03",
    "shiki_btl_atc04",
    "shiki_btl_atc05",
    "shiki_btl_atc06",
    "shiki_btl_atc07",
    "shiki_btl_atc08",
    "shiki_btl_fb01",
    "shiki_btl_fb02",
    "shiki_btl_fb03",
    "shiki_btl_fb04",
    "shiki_btl_fb05",
    "shiki_btl_fb06",
    "shiki_btl_fb07",
    "shiki_btl_fb08",
    "shiki_btl_fb09",
    "shiki_btl_counter01",
    "shiki_btl_cat01",
    "shiki_btl_cat02",
    "shiki_btl_prs01",
    "shiki_btl_prs02",
    "shiki_btl_kizukai01",
    "shiki_btl_dmg01",
    "shiki_btl_dmg02",
    "shiki_btl_dmg03",
    "shiki_btl_dmg_L01",
    "shiki_btl_dmg_L02",
    "shiki_btl_pnc01",
    "shiki_btl_ovr01",
    "shiki_btl_ovr02",
    "shiki_btl_win01",
    "shiki_btl_win02",
    "shiki_btl_win03",
    "shiki_btl_win04",
    "shiki_btl_rizalt01",
    "shiki_btl_rizalt02",
    "yoshua_btl_op01",
    "yoshua_btl_op03",
    "yoshua_btl_op_danger01",
    "yoshua_btl_atc01",
    "yoshua_btl_atc02",
    "yoshua_btl_atc03",
    "yoshua_btl_atc04",
    "yoshua_btl_atc05",
    "yoshua_btl_atc06",
    "yoshua_btl_atc07",
    "yoshua_btl_atc08",
    "yoshua_btl_fb01",
    "yoshua_btl_fb02",
    "yoshua_btl_fb03",
    "yoshua_btl_fb04",
    "yoshua_btl_fb05",
    "yoshua_btl_fb06",
    "yoshua_btl_fb07",
    "yoshua_btl_fb08",
    "yoshua_btl_fb09",
    "yoshua_btl_counter01",
    "yoshua_btl_cat01",
    "yoshua_btl_cat02",
    "yoshua_btl_prs01",
    "yoshua_btl_prs02",
    "yoshua_btl_kizukai01",
    "yoshua_btl_dmg01",
    "yoshua_btl_dmg02",
    "yoshua_btl_dmg03",
    "yoshua_btl_dmg_L01",
    "yoshua_btl_dmg_L02",
    "yoshua_btl_pnc01",
    "yoshua_btl_ovr01",
    "yoshua_btl_ovr02",
    "yoshua_btl_win01",
    "yoshua_btl_win02",
    "yoshua_btl_win03",
    "yoshua_btl_win04",
    "yoshua_btl_rizalt01",
    "yoshua_btl_rizalt02",
    "biito_btl_op01",
    "biito_btl_op03",
    "biito_btl_op_danger01",
    "biito_btl_atc01",
    "biito_btl_atc02",
    "biito_btl_atc03",
    "biito_btl_atc04",
    "biito_btl_atc05",
    "biito_btl_atc06",
    "biito_btl_atc07",
    "biito_btl_atc08",
    "biito_btl_fb01",
    "biito_btl_fb02",
    "biito_btl_fb03",
    "biito_btl_fb04",
    "biito_btl_fb05",
    "biito_btl_fb06",
    "biito_btl_fb07",
    "biito_btl_fb08",
    "biito_btl_fb09",
    "biito_btl_counter01",
    "biito_btl_cat01",
    "biito_btl_cat02",
    "biito_btl_prs01",
    "biito_btl_prs02",
    "biito_btl_kizukai01",
    "biito_btl_dmg01",
    "biito_btl_dmg02",
    "biito_btl_dmg03",
    "biito_btl_dmg_L01",
    "biito_btl_dmg_L02",
    "biito_btl_pnc01",
    "biito_btl_ovr01",
    "biito_btl_ovr02",
    "biito_btl_win01",
    "biito_btl_win02",
    "biito_btl_win03",
    "biito_btl_win04",
    "biito_btl_rizalt01",
    "biito_btl_rizalt02",
    "shiki_btl_spc_01",
    "neku_btl_spc_shiki01",
    "shiki_btl_spc_02",
    "neku_btl_spc_shiki02",
    "shiki_btl_spc_03",
    "neku_btl_spc_shiki03",
    "yoshua_btl_spc_01",
    "neku_btl_spc_yoshua01",
    "yoshua_btl_spc_02",
    "neku_btl_spc_yoshua02",
    "neku_btl_spc_yoshua03",
    "yoshua_btl_spc_03",
    "biito_btl_spc_01",
    "neku_btl_spc_biito01",
    "biito_btl_spc_02",
    "neku_btl_spc_biito02",
    "neku_btl_spc_biito03",
    "biito_btl_spc_03",
    "neku_fld_01",
    "neku_fld_02",
    "neku_fld_partner01",
    "neku_fld_partner02",
    "shiki_fld_partner01",
    "shiki_fld_partner02",
    "yoshua_fld_partner01",
    "yoshua_fld_partner02",
    "biito_fld_partner01",
    "biito_fld_partner02",
    "neku_fld_goot_L01",
    "neku_fld_goot_S01",
    "neku_fld_average01",
    "neku_fld_notgoot_S01",
    "neku_fld_notgoot_L01",
    "shiki_fld_goot_L01",
    "shiki_fld_goot_S01",
    "shiki_fld_average01",
    "shiki_fld_notgoot_S01",
    "shiki_fld_notgoot_L01",
    "yoshua_fld_goot_L01",
    "yoshua_fld_goot_S01",
    "yoshua_fld_average01",
    "yoshua_fld_notgoot_S01",
    "yoshua_fld_notgoot_L01",
    "biito_fld_goot_L01",
    "biito_fld_goot_S01",
    "biito_fld_average01",
    "biito_fld_notgoot_S01",
    "biito_fld_notgoot_L01",
    "EV_strmse_cheer",
    "EV_strmse_wolf_roar",
    "EV_strmse_ramen",
    "EV_strmse_girls",
    "EV_strmse_badge_throw",
    "EV_strmse_badge_break",
    "M_koukyu01",
    "M_koukyu02",
    "M_koukyu03",
    "M_koukyu04",
    "M_shomin01",
    "M_shomin02",
    "M_shomin03",
    "M_shomin04",
    "M_kawarimono01",
    "M_kawarimono02",
    "M_kawarimono03",
    "M_kawarimono04",
    "M_fastfood01",
    "M_fastfood02",
    "M_fastfood03",
    "M_fastfood04",
    "M_saru01_01",
    "M_saru01_02",
    "M_saru01_03",
    "M_saru02",
    "M_saru03",
    "M_saru04",
    "M_roujin01",
    "M_roujin02",
    "M_roujin03",
    "M_roujin04",
    "W_koukyu01",
    "W_koukyu02",
    "W_koukyu03",
    "W_koukyu04",
    "W_shomin01",
    "W_shomin02",
    "W_shomin03",
    "W_shomin04",
    "W_gal01",
    "W_gal02",
    "W_gal03",
    "W_gal04",
    "W_fastfood01",
    "W_fastfood02",
    "W_fastfood03",
    "W_fastfood04",
    "shopM_ramen01",
    "shopM_ramen02",
    "shopM_ramen03",
    "shopM_ramen04",
    "neku_event_odorokiS01",
    "neku_event_odorokiL01",
    "neku_event_odorokiL02",
    "neku_event_yorokobiS01",
    "neku_event_yorokobiL01",
    "neku_event_ikariL01",
    "neku_event_ikariL02",
    "neku_event_odoroki01",
    "neku_event_kumon01",
    "neku_event_akire01",
    "neku_event_neoki01",
    "neku_event_question01",
    "neku_event_question02",
    "shiki_event_odorokiS01",
    "shiki_event_odorokiS02",
    "shiki_event_odorokiL01",
    "shiki_event_odorokiL02",
    "shiki_event_yorokobiS01",
    "shiki_event_yorokobiL01",
    "shiki_event_ikariL01",
    "shiki_event_ikariL02",
    "shiki_event_odoroki01",
    "shiki_event_kumon01",
    "shiki_event_akire01",
    "shiki_event_question01",
    "shiki_event_question02",
    "yoshua_event_odorokiS01",
    "yoshua_event_odorokiL01",
    "yoshua_event_odorokiL02",
    "yoshua_event_yorokobiS01",
    "yoshua_event_yorokobiL01",
    "yoshua_event_ikariL01",
    "yoshua_event_ikariL02",
    "yoshua_event_odoroki01",
    "yoshua_event_kumon01",
    "yoshua_event_akire01",
    "yoshua_event_question01",
    "yoshua_event_question02",
    "biito_event_odorokiS01",
    "biito_event_odorokiL01",
    "biito_event_odorokiL02",
    "biito_event_yorokobiS01",
    "biito_event_yorokobiL01",
    "biito_event_ikariL01",
    "biito_event_ikariL02",
    "biito_event_odoroki01",
    "biito_event_kumon01",
    "biito_event_akire01",
    "biito_event_question01",
    "biito_event_question02",
    "ryme_event_odorokiS01",
    "ryme_event_yorokobiS01",
    "ryme_event_yorokobiM01",
    "ryme_event_yorokobiL01",
    "ryme_event_akire01",
    "ryme_event_question01",
    "kariya_event_odorokiS01",
    "kariya_event_yorokobiS01",
    "kariya_event_yorokobiM01",
    "kariya_event_yorokobiL01",
    "kariya_event_ikariL01",
    "kariya_event_ikariL02",
    "kariya_event_meirei",
    "kariya_event_akire01",
    "kariya_event_question01",
    "yashiro_event_odorokiS01",
    "yashiro_event_yorokobiS01",
    "yashiro_event_yorokobiM01",
    "yashiro_event_yorokobiL01",
    "yashiro_event_ikariL01",
    "yashiro_event_ikariL02",
    "yashiro_event_meirei",
    "yashiro_event_akire01",
    "yashiro_event_question01",
    "higashi_n_event_odorokiS01",
    "higashi_n_event_yorokobiS01",
    "higashi_n_event_yorokobiM01",
    "higashi_n_event_yorokobiL01",
    "higashi_n_event_ikariL01",
    "higashi_n_event_ikariL02",
    "higashi_n_event_meirei",
    "higashi_n_event_akire01",
    "higashi_n_event_question01",
    "kita_event_odorokiS01",
    "kita_event_yorokobiS01",
    "kita_event_yorokobiM01",
    "kita_event_yorokobiL01",
    "kita_event_ikariL01",
    "kita_event_ikariL02",
    "kita_event_akire01",
    "kita_event_question01",
    "hanekoma_n_event_odorokiS01",
    "hanekoma_n_event_yorokobiS01",
    "hanekoma_n_event_yorokobiM01",
    "hanekoma_n_event_yorokobiL01",
    "hanekoma_n_event_ikariL01",
    "hanekoma_n_event_ikariL02",
    "hanekoma_n_event_akire01",
    "hanekoma_n_event_question01",
    "konishi_n_event_odorokiS01",
    "konishi_n_event_yorokobiS01",
    "konishi_n_event_yorokobiM01",
    "konishi_n_event_yorokobiL01",
    "konishi_n_event_ikariL01",
    "konishi_n_event_ikariL02",
    "konishi_n_event_meirei",
    "konishi_n_event_akire01",
    "konishi_n_event_question01",
    "minami_event_odorokiS01",
    "minami_event_yorokobiS01",
    "minami_event_yorokobiM01",
    "minami_event_yorokobiL01",
    "minami_event_ikariL01",
    "minami_event_ikariL02",
    "minami_event_meirei",
    "minami_event_akire01",
    "minami_event_question01",
    "kita_btl_atcS01",
    "kita_btl_atcM03",
    "kita_btl_atcM02",
    "kita_btl_fb06",
    "kita_btl_fb07",
    "kita_btl_dmg01",
    "kita_btl_dmg02",
    "kita_btl_dmg_L01",
    "kita_btl_dmg_L02",
    "kita_btl_pnc01",
    "kita_btl_win01",
    "kita_btl_ovr01",
    "kita_n_btl_atcS01",
    "kita_n_btl_atcS02",
    "kita_n_btl_atcS03",
    "kita_n_btl_fb05",
    "kita_n_btl_win01",
    "kita_n_btl_ovr01",
    "kita_n_btl_fb06",
    "kita_n_btl_pnc01",
    "konishi_n_btl_atcM04",
    "konishi_n_btl_fb01",
    "konishi_n_btl_atcM03",
    "konishi_n_btl_dmg01",
    "konishi_n_btl_dmg_L01",
    "konishi_n_btl_dmg02",
    "konishi_n_btl_pnc01",
    "konishi_n_btl_win01",
    "konishi_n_btl_other01",
    "konishi_n_btl_fb06",
    "konishi_n_btl_dmg03",
    "konishi_n_btl_fb05",
    "konishi_n_btl_fb07",
    "konishi_n_btl_dmg_L02",
    "konishi_n_btl_other02",
    "minami_btl_atcM04",
    "minami_btl_fb05",
    "minami_btl_fb07",
    "minami_btl_dmg01",
    "minami_btl_dmg02",
    "minami_btl_pnc01",
    "minami_btl_win01",
    "minami_btl_ovr01",
    "minami_btl_fb04",
    "minami_btl_atcM02",
    "minami_btl_atcS03",
    "minami_btl_other01",
    "minami_btl_other02",
    "minami_btl_fb03",
    "minami_btl_fb06",
    "minami_btl_fb01",
    "minami_h_btl_atcS03",
    "minami_h_btl_atcS01",
    "minami_h_btl_atcS02",
    "minami_h_btl_fb07",
    "minami_h_btl_win01",
    "minami_h_btl_ovr01",
    "minami_h_btl_fb06",
    "minami_n_btl_atcS02",
    "minami_n_btl_atcS03",
    "minami_n_btl_fb06",
    "minami_n_btl_fb04",
    "minami_n_btl_fb05",
    "minami_n_btl_dmg01",
    "minami_n_btl_dmg02",
    "minami_n_btl_ovr01",
    "minami_n_btl_atcM03",
    "minami_n_btl_atcM04",
    "higashi_n_btl_fb05",
    "higashi_n_btl_atcM03",
    "higashi_n_btl_atcM04",
    "higashi_n_btl_fb04",
    "higashi_n_btl_fb03",
    "higashi_n_btl_fb06",
    "higashi_n_btl_pnc01",
    "higashi_n_btl_win01",
    "higashi_n_btl_ovr02",
    "higashi_n_btl_fb07",
    "higashi_n_btl_other03",
    "kariya_btl_fb01",
    "kariya_btl_fb05",
    "kariya_btl_fb02",
    "kariya_btl_atcM04",
    "kariya_btl_fb04",
    "kariya_btl_fb06",
    "kariya_btl_fb07",
    "kariya_btl_dmg01",
    "kariya_btl_dmg02",
    "kariya_btl_dmg_L01",
    "kariya_btl_pnc01",
    "kariya_btl_win01",
    "kariya_btl_ovr01",
    "kariya_btl_other01",
    "kariya_btl_other02",
    "kariya_btl_other03",
    "kariya_b_btl_atcS01",
    "kariya_b_btl_atcS02",
    "kariya_b_btl_atcS03",
    "kariya_b_btl_atcS04",
    "kariya_b_btl_atcM04",
    "kariya_b_btl_dmg01",
    "kariya_b_btl_dmg02",
    "kariya_b_btl_dmg03",
    "kariya_b_btl_ovr01",
    "yashiro_btl_fb01",
    "yashiro_btl_fb05",
    "yashiro_btl_atcM04",
    "yashiro_btl_atcM03",
    "yashiro_btl_fb06",
    "yashiro_btl_fb07",
    "yashiro_btl_dmg01",
    "yashiro_btl_dmg02",
    "yashiro_btl_dmg_L02",
    "yashiro_btl_pnc01",
    "yashiro_btl_win01",
    "yashiro_btl_ovr01",
    "yashiro_btl_other01",
    "yashiro_btl_other02",
    "yashiro_btl_fb04",
    "yashiro_btl_other03",
    "yashiro_b_btl_atcS02",
    "yashiro_b_btl_atcS04",
    "yashiro_b_btl_atcS03",
    "yashiro_b_btl_atcM04",
    "yashiro_b_btl_dmg01",
    "yashiro_b_btl_dmg02",
    "yashiro_b_btl_dmg_L02",
    "yashiro_b_btl_ovr01",
    "h_shini_btl_atcS01",
    "h_shini_btl_atcS02",
    "h_shini_btl_atcM02",
    "h_shini_btl_dmg01",
    "h_shini_btl_dmg02",
    "h_shini_btl_dmg_L01",
    "h_shini_btl_dmg_L02",
    "hanekoma_n_btl_atcM01",
    "hanekoma_n_btl_atcM04",
    "hanekoma_n_btl_dmg01",
    "hanekoma_n_btl_dmg03",
    "hanekoma_n_btl_atcS01",
    "hanekoma_n_btl_fb07",
    "hanekoma_n_btl_fb05",
    "hanekoma_n_btl_fb04",
    "hanekoma_n_btl_fb06",
    "shiki_ayatsu_btl_dmg01",
    "shiki_ayatsu_btl_dmg02",
    "shiki_ayatsu_btl_dmg03",
    "MC_01",
    "MC_02_01",
    "MC_02_02",
    "MC_03",
    "MC_04",
    "MC_05",
    "EV_strmse_reserve01",
    "EV_strmse_reserve02",
    "EV_strmse_reserve03",
    "EV_strmse_reserve04",
    "EV_strmse_reserve05",
    "EV_strmse_reserve06",
    "EV_strmse_reserve07",
    "EV_strmse_reserve08",
    "EV_strmse_reserve09",
    "EV_strmse_reserve10",
};

void func_ov029_020824a0(SoundTestState* state) {
    UnkStruct_usedby_02025b68 local_30;

    local_30.unk_00 = 1;
    local_30.unk_04 = 0;
    local_30.unk_08 = state->unk_11584;
    local_30.unk_0C = &data_0205c9b0;
    local_30.unk_10 = 0;
    local_30.unk_14 = NULL;
    local_30.unk_18 = 0;
    local_30.unk_1A = 0;
    local_30.unk_1C = 0x20;
    local_30.unk_1E = 0x18;
    func_02025b68(&state->unk_21594, &local_30);
    Text_RenderToScreen(&state->unk_215A0, 0, 0, "SoundTest");
    Text_RenderToScreen(&state->unk_215A0, 0, 0x98, ADXT_GetVersionInfo());
}

void SoundTest_DrawMenu(SoundTestState* state) {
    func_02010b84(&state->unk_215A0, 0, 0x10, 0x100, 0x50);

    Text_RenderToScreen(&state->unk_215A0, 8, 0x10, CriSndMgr_AdxData[state->adxIdx].adxFile);
    Text_RenderToScreen(&state->unk_215A0, 8, 0x18, func_02006930("seqArc:%d", state->seqArc));
    Text_RenderToScreen(&state->unk_215A0, 8, 0x20, func_02006930("se:%d", state->se));
    Text_RenderToScreen(&state->unk_215A0, 8, 0x28, func_02006930("seIdx %d:%s", state->seIdx, soundEffects[state->seIdx]));
    Text_RenderToScreen(&state->unk_215A0, 8, 0x30, func_02006930("seIdx Volume:%d", state->seIdxVolume));
    Text_RenderToScreen(&state->unk_215A0, 8, 0x38, func_02006930("sePan :%d", state->sePan));

    if (state->adxVolume <= 0) {
        Text_RenderToScreen(&state->unk_215A0, 8, 0x40, func_02006930("adx volume :0(0.0dB)"));
    } else {
        s32 volFloat = state->adxVolume % 10;
        if (volFloat < 0) {
            volFloat = -volFloat;
        }
        Text_RenderToScreen(&state->unk_215A0, 8, 0x40,
                            func_02006930("adx volume :-%d(-%d.%ddB)", state->adxVolume, state->adxVolume / 10, volFloat));
    }

    if (state->adxLoopEnabled) {
        Text_RenderToScreen(&state->unk_215A0, 8, 0x48, func_02006930("adx loop on"));
    } else {
        Text_RenderToScreen(&state->unk_215A0, 8, 0x48, func_02006930("adx loop off"));
    }

    Text_RenderToScreen(&state->unk_215A0, 8, 0x50, func_02006930("sePitch :%d", state->sePitch));

    if (state->noiseNoWaveLoad) {
        Text_RenderToScreen(&state->unk_215A0, 8, 0x58, func_02006930("NoiseNoWaveLoad :ON "));
    } else {
        Text_RenderToScreen(&state->unk_215A0, 8, 0x58, func_02006930("NoiseNoWaveLoad :OFF"));
    }

    // Menu Cursor
    Text_RenderToScreen(&state->unk_215A0, 0, state->menuCurrentRow * 8 + 0x10, ">");
}

void func_ov029_02082824(SoundTestState* state) {
    func_02025e30(&state->unk_21594);
}

void func_ov029_02082838(SoundTestState* state) {
    state->adxIdx          = 0;
    state->unk_219B4       = 0;
    state->unk_219B8       = 0;
    state->menuCurrentRow  = 0;
    state->seqArc          = 0;
    state->se              = 0;
    state->seIdx           = 0;
    state->unk_219D0       = 0;
    state->unk_219E8       = 0;
    state->noiseNoWaveLoad = FALSE;
    state->seIdxVolume     = SndMgr_GetSeIdxVolume(state->seIdx);
    state->sePan           = 128;
    state->adxVolume       = -CriSndMgr_GetAdxIdxVolume(state->adxIdx);
    state->sePitch         = 0;
    state->adxLoopEnabled  = TRUE;
    func_ov029_020824a0(state);
    SoundTest_DrawMenu(state);
}

void func_ov029_020828c0(SoundTestState* state) {
    func_ov029_02082824(state);
}

static void SoundTest_AdjustSePitch(s32* pitch) {
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT) {
        *pitch += 64;
    } else if (SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT) {
        *pitch -= 64;
    }
}

static void SoundTest_AdjustCappedValue(s32* value, s32 upperLimit) {
    s32 iVar1;

    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT) {
        iVar1  = *value;
        *value = iVar1 + 1;
        if (iVar1 + 1 >= upperLimit) {
            *value = 0;
        }
        return;
    }
    if (SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT) {
        iVar1  = *value;
        *value = iVar1 + -1;
        if (iVar1 + -1 < 0) {
            *value = upperLimit - 1;
        }
        return;
    }
}

void SoundTest_PlaySelectedSoundType(SoundTestState* state) {
    if (state->menuCurrentRow < 1) {
        state->unk_219CC = 0;
    }
    switch (state->menuCurrentRow) {
        case MENU_ROW_ADX:
            func_0202733c(state->adxIdx);
            return;
        case MENU_ROW_SEQARC:
        case MENU_ROW_SE:
            SndMgr_StartPlayingSeqWithCustomSeqArc(1, state->seqArc, state->se);
            return;
        case MENU_ROW_SEIDX:
            SndMgr_ResetSequenceHeap();
            SndMgr_StartPlayingSE(state->seIdx);
            return;
        case MENU_ROW_SEVOL:
            SndMgr_ResetSequenceHeap();
            SndMgr_StartPlayingSE(state->seIdx);
            return;
        case MENU_ROW_SEPAN:
            SndMgr_ResetSequenceHeap();
            SndMgr_StartPlayingSE(state->seIdx);
            SndMgr_UpdateSEPan(state->seIdx, state->sePan);
            return;
        case MENU_ROW_SEPITCH:
            SndMgr_ResetSequenceHeap();
            SndMgr_StartPlayingSE(state->seIdx);
            SndMgr_SetSequenceSoundPitch(state->seIdx, state->sePitch);
            return;
    }
}

// Nonmatching: Unreachable code that exists in binary
void func_ov029_02082a38(void) {
    s32 idx = 0;

    // Unreachable code left in original? Possibly a side effect of while loop compilation?
    if (idx >= 5) {
        return;
    }

    while (idx < 5) {
        SndMgr_StopPlaybackForTrack(idx);
        idx++;
    };
}

void SoundTest_StopSelectedSoundType(SoundTestState* state) {
    if (state->menuCurrentRow < 1) {
        state->unk_219CC = 0;
    }
    switch (state->menuCurrentRow) {
        case MENU_ROW_ADX:
            func_02027388(0);
            return;
        case MENU_ROW_SEQARC:
        case MENU_ROW_SE:
            func_ov029_02082a38();
            return;
        case MENU_ROW_SEIDX:
            SndMgr_StopPlayingSE(state->seIdx);
            return;
    }
}

void SoundTest_StopAllSounds(void) {
    func_02027388(0);
    func_02027388(70);
    func_02027388(78);
    func_ov029_02082a38();
}

BOOL SoundTest_ControlMenu(SoundTestState* state) {
    OverlayTag tag;

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_START) {
        state->unk_219E8 ^= 1;
        func_02027200(state->unk_219E8);
    } else if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_A) {
        SoundTest_PlaySelectedSoundType(state);
    } else if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_B)
    { // Stop music/sound effect depending on current menu selection
        SoundTest_StopSelectedSoundType(state);
    } else if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_Y) { // Stop all active sounds
        SoundTest_StopAllSounds();
    } else if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_X) {
        SndMgr_ResetSequenceHeap();
        SndMgr_StartPlayingSE(3);
    } else if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_SELECT) { // Return to main debug menu
        MainOvlDisp_Pop(&tag);
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_UP) {       // Scroll selection up
        state->menuCurrentRow--;
    } else if (SysControl.buttonState.holdButtons == INPUT_BUTTON_DOWN) {     // Scroll selection down
        state->menuCurrentRow++;
    }

    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_L) {
        if (state->seqArc >= 0) {
            SndMgr_StartPlayingSeqWithCustomSeqArc(1, state->seqArc, state->se);
        } else {
            SndMgr_PlayStreamSequenceWithSeqID(state->se);
        }
    }
    if (SysControl.buttonState.pressedButtons & INPUT_BUTTON_R) {
        SndMgr_StopPlaybackForTrack(1);
    }

    if (state->menuCurrentRow < 0) {
        state->menuCurrentRow = 9;
    } else if (state->menuCurrentRow >= 10) {
        state->menuCurrentRow = 0;
    }

    switch (state->menuCurrentRow) {
        case MENU_ROW_ADX:
            SoundTest_AdjustCappedValue(&state->adxIdx, CriSndMgr_AdxDataCount);
            break;
        case MENU_ROW_SEQARC:
            SoundTest_AdjustCappedValue(&state->seqArc, 255);
            break;
        case MENU_ROW_SE:
            SoundTest_AdjustCappedValue(&state->se, 255);
            break;
        case MENU_ROW_SEIDX:
            SoundTest_AdjustCappedValue(&state->seIdx, 1388);
            state->seqArc      = g_SequenceDataTable[state->seIdx].seqArc;
            state->se          = g_SequenceDataTable[state->seIdx].se;
            state->seIdxVolume = SndMgr_GetSeIdxVolume(state->seIdx);
            break;
        case MENU_ROW_SEVOL:
            SoundTest_AdjustCappedValue(&state->seIdxVolume, 128);
            SndMgr_SetSeIdxVolume(state->seIdx, state->seIdxVolume);
            break;
        case MENU_ROW_SEPAN:
            SoundTest_AdjustCappedValue(&state->sePan, 256);
            SndMgr_UpdateSEPan(state->seIdx, state->sePan);
            break;
        case MENU_ROW_ADXVOL:
            SoundTest_AdjustCappedValue(&state->adxVolume, 961);
            CriSndMgr_SetAdxIdxVolume(state->adxIdx, -state->adxVolume);
            break;
        case MENU_ROW_ADXLOOP:
            if ((SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT)) {
                state->adxLoopEnabled = TRUE;
                CriSndMgr_SetLpFlg(TRUE);
            } else if ((SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT)) {
                state->adxLoopEnabled = FALSE;
                CriSndMgr_SetLpFlg(FALSE);
            }
            break;
        case MENU_ROW_SEPITCH:
            SoundTest_AdjustSePitch(&state->sePitch);
            SndMgr_SetSequenceSoundPitch(state->seIdx, state->sePitch);
            break;
        case MENU_ROW_NOISE:
            if ((SysControl.buttonState.holdButtons & INPUT_BUTTON_RIGHT)) {
                state->noiseNoWaveLoad = TRUE;
                SndMgr_SetPriorityLoadFlag(1);
            } else if ((SysControl.buttonState.holdButtons & INPUT_BUTTON_LEFT)) {
                state->noiseNoWaveLoad = FALSE;
                SndMgr_SetPriorityLoadFlag(0);
            }
    }
    if (SysControl.buttonState.holdButtons & (INPUT_MASK_DPAD | INPUT_BUTTON_A)) {
        state->adxVolume = -CriSndMgr_GetAdxIdxVolume(state->adxIdx);
        SoundTest_DrawMenu(state);
    }
    return SndMgr_IsSEPlaying(state->seIdx);
}

// Nonmatching
void func_ov029_02082e40(SoundTestState* param) {
    char*           name  = data_ov029_02083400;
    SoundTestState* state = Mem_AllocHeapTail(&gDebugHeap, sizeof(SoundTestState));
    Mem_SetSequence(&gDebugHeap, state, name);
    MainOvlDisp_SetCbArg(state);
    state->unk_11584 = DatMgr_AllocateSlot();
    func_ov029_020833c4();
    g_DisplaySettings.controls[DISPLAY_MAIN].layers     = LAYER_NONE;
    state->unk_11580                                    = func_0200cef0(state);
    data_02066aec                                       = 0;
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    data_02066eec                                       = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;
    func_ov029_02082838(state);
    MainOvlDisp_IncrementRepeatCount();
}

void func_ov029_02082ee8(SoundTestState* state) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    SoundTest_ControlMenu(state);
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc[0], 0);
    func_0200bf60(data_0206b3cc[1], 0);
}

void func_ov029_02082f68(SoundTestState* state) {
    func_ov029_020828c0(state);
    func_0200cef0(NULL);
    DatMgr_ClearSlot(state->unk_11584);
    Mem_Free(&gDebugHeap, state);
}

void func_ov029_02082f9c(SoundTestState* state) {
    static const SoundEffectFunc funcs[3] = {
        func_ov029_02082e40,
        func_ov029_02082ee8,
        func_ov029_02082f68,
    };

    s32 idx = MainOvlDisp_GetRepeatCount();

    if (idx == 0x7FFFFFFF) {
        func_ov029_02082f68(state);
        return;
    }
    funcs[idx](state);
}

/* Nonmatching */
void func_ov029_02082fdc(void) {
    /* Not yet implemented */
}

void SoundTest_InterruptCallback(void) {
    if (SystemStatusFlags.vblank != FALSE) {
        Display_Commit();
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 0x400);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

void func_ov029_020833c4(void) {
    func_ov029_02082fdc();
    Interrupts_RegisterVBlankCallback(SoundTest_InterruptCallback, TRUE);
}
