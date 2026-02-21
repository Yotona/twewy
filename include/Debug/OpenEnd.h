#ifndef OPENEND_H
#define OPENEND_H

#include "DatMgr.h"
#include "EasyTask.h"
#include "Input.h"
#include "Interrupts.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "SpriteMgr.h"
#include "System.h"
#include "registers.h"

typedef struct {
    /* 0x00000 */ Data*    dataList[3];
    /* 0x0000C */ s32      dataType;
    /* 0x00010 */ s32      unk_10;
    /* 0x11588 */ char     pad_14[0x1157C];
    /* 0x11590 */ TaskPool taskPool;
    /* 0x11610 */ MemPool  memPool;
    /* 0x1161C */ void*    unk_1161C;
    /* 0x11620 */ char     pad_11620[0x3FC];
    /* 0x11A1C */ s16      introFrameCount;
    /* 0x11A1E */ s16      pad_11A1E;
    /* 0x11A20 */ s16      sequenceStep;
    /* 0x11A22 */ s16      pad_11A22;
    /* 0x11A24 */ s32      titleFrameCount;
    /* 0x11A28 */ s32      selectedOption;
    /* 0x11A2C */ s32      alreadySelected;
    /* 0x11A30 */ s32      fadeBrightness;
    /* 0x11A34 */ s32      fadeRate;
    /* 0x11A38 */ u16      unk_11A38;
    /* 0x11A3A */ s16      pad_11A3A;
    /* 0x11A3C */ s32      isSaveValid;
    /* 0x11A40 */ s32      unk_11A40;
    /* 0x11A44 */ s32      isThereExistingSaveData;
    /* 0x11A48 */ s32      unk_11A48;
} OpenEndState; // Size: 0x11A4C

/**
 * @brief Loads character (tile) graphics data for a screen into VRAM.
 *
 * @param r0
 * @param r1
 * @param buffer   Pointer to the source character data buffer.
 * @param r3
 * @param dataSize Size in bytes of the character data to load.
 */
void OpenEnd_LoadScreenCharacterData(s32 r0, s32 r1, void* buffer, s32 r3, s32 dataSize);

/**
 * @brief Validates the save data and updates the state with the result.
 *
 * Checks whether existing save data is present and marks it as valid or
 * invalid in @p state. Sets @c isSaveValid and @c isThereExistingSaveData
 * accordingly.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_ValidateSaveData(OpenEndState* state);

/**
 * @brief Loads all graphical assets required for a screen.
 *
 * Orchestrates loading of character, palette, and sprite data for the
 * opening/title sequence screens.
 *
 * @param state Pointer to the current OpenEnd state.
 * @param r1
 * @param r2
 * @param r3
 */
void OpenEnd_LoadScreenAssets(OpenEndState* state, s32 r1, s32 r2, s32 r3);

/**
 * @brief Loads palette data for a screen into palette RAM.
 *
 * @param r0
 * @param buffer   Pointer to the source palette data buffer.
 * @param r2
 * @param dataSize Size in bytes of the palette data to load.
 */
void OpenEnd_LoadScreenPaletteData(s32 r0, void* buffer, s32 r2, s32 dataSize);

/**
 * @brief Loads sprite (OAM) data for a screen.
 *
 * @param r0
 * @param r1
 * @param buffer   Pointer to the source sprite data buffer.
 * @param r3
 * @param dataSize Size in bytes of the sprite data to load.
 */
void OpenEnd_LoadScreenSpriteData(s32 r0, s32 r1, void* buffer, s32 r3, s32 dataSize);

/**
 * @brief Advances the opening/title screen sequence by one step.
 *
 * Dispatches to the appropriate init, update, or destroy function based on
 * @c sequenceStep stored in @p state.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_PlayScreenSequence(OpenEndState* state);

/**
 * @brief Creates and registers a badge task (New Game / Continue).
 *
 * @param r0 Badge index; see @c BadgeIndex enum (@c BADGE_NEWGAME or @c BADGE_CONTINUE).
 * @return   Non-zero on success, zero on failure.
 */
s32 OpenEnd_CreateBadgeTask(u32 r0);

/**
 * @brief VBlank interrupt handler for the OpenEnd module.
 *
 * Called each VBlank to perform display-synchronised updates such as
 * DMA transfers and palette/OAM commits.
 */
void OpenEnd_VBlankHandler(void);

/**
 * @brief Task callback: initialises a badge sprite task.
 *
 * @param unused_r0 Unused task pool pointer (required by task callback signature).
 * @param r1        Pointer to the Task being initialised.
 * @param r2        Pointer to the task parameter data.
 * @return          0 to keep the task alive, non-zero to destroy it.
 */
s32 OpenEnd_TaskBadge_Init(struct TaskPool* unused_r0, struct Task* r1, void* r2);

/**
 * @brief Task callback: updates a badge sprite task each frame.
 *
 * Handles badge animation and input polling for the New Game / Continue
 * selection on the title screen.
 *
 * @param unused_r0 Unused task pool pointer (required by task callback signature).
 * @param r1        Pointer to the Task being updated.
 * @param r2        Pointer to the task parameter data.
 * @return          0 to keep the task alive, non-zero to destroy it.
 */
s32 OpenEnd_TaskBadge_Update(struct TaskPool* unused_r0, struct Task* r1, void* r2);

/**
 * @brief Task callback: renders a badge sprite to the screen.
 *
 * Submits OAM entries for the badge sprite corresponding to the
 * New Game or Continue option.
 *
 * @param unused_r0 Unused task pool pointer (required by task callback signature).
 * @param r1        Pointer to the Task being rendered.
 * @param r2        Pointer to the task parameter data.
 * @return          0 to keep the task alive, non-zero to destroy it.
 */
s32 OpenEnd_TaskBadge_Render(struct TaskPool* unused_r0, struct Task* r1, void* r2);

/**
 * @brief Task callback: cleans up a badge sprite task.
 *
 * Releases any resources allocated during @c OpenEnd_TaskBadge_Init.
 *
 * @param unused_r0 Unused task pool pointer (required by task callback signature).
 * @param r1        Pointer to the Task being destroyed.
 * @param r2        Pointer to the task parameter data.
 * @return          0 on success.
 */
s32 OpenEnd_TaskBadge_CleanUp(struct TaskPool* unused_r0, struct Task* r1, void* r2);

const BinIdentifier OpenEnd_FileList[] = {
    {0x25,          "Apl_Mor/Grp_Title.bin"},
    {0x25,        "Apl_Mor/COPY_RG_SE.nbfc"}, //"Square Enix"
    {0x25,        "Apl_Mor/COPY_RG_SE.nbfp"},
    {0x25,        "Apl_Mor/COPY_RG_SE.nbfs"},
    {0x25,        "Apl_Mor/COPY_UG_JP.nbfc"}, //"Jupiter"
    {0x25,        "Apl_Mor/COPY_UG_JP.nbfp"},
    {0x25,        "Apl_Mor/COPY_UG_JP.nbfs"},
    {0x25,     "Apl_Mor/N_COPY_RG_NIN.nbfc"}, //"LICENSED BY NINTENDO"
    {0x25,     "Apl_Mor/N_COPY_RG_NIN.nbfp"},
    {0x25,     "Apl_Mor/N_COPY_RG_NIN.nbfs"},
    {0x25,       "Apl_Mor/COPY_UG_CRI.nbfc"}, // "CRIWARE (Technology by ADX, SofDec)"
    {0x25,       "Apl_Mor/COPY_UG_CRI.nbfp"},
    {0x25,       "Apl_Mor/COPY_UG_CRI.nbfs"},
    {0x25,      "Apl_Mor/OpeningBG_RG.nbfc"}, // Title screen (Main display)
    {0x25,      "Apl_Mor/OpeningBG_RG.nbfp"},
    {0x25,      "Apl_Mor/OpeningBG_RG.nbfs"},
    {0x25, "Apl_Mor/OpeningBG_UGstart.nbfc"}, // Title screen (Sub display)
    {0x25, "Apl_Mor/OpeningBG_UGstart.nbfp"},
    {0x25, "Apl_Mor/OpeningBG_UGstart.nbfs"},
};

extern s32 data_ov037_02083a74[];
extern s32 data_ov037_02083a7c[];

/**
 * @brief Initialises the OpenEnd module and loads initial assets.
 *
 * Sets up the task pool, memory pool, and kicks off the intro screen
 * sequence. Should be called once when the overlay is loaded.
 *
 * @param state Pointer to the OpenEnd state to initialise.
 */
void OpenEnd_Init(OpenEndState* state);

/**
 * @brief Updates the OpenEnd module for the current frame.
 *
 * Dispatches per-frame logic to whichever screen is currently active
 * (intro or title) via @c screenFunctions.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_Update(OpenEndState* state);

/**
 * @brief Destroys the OpenEnd module and releases all resources.
 *
 * Tears down the active screen, frees memory pool allocations, and
 * unregisters the VBlank handler.
 *
 * @param state Pointer to the OpenEnd state to destroy.
 */
void OpenEnd_Destroy(OpenEndState* state);

/** @brief Function pointer type for OpenEnd screen lifecycle callbacks. */
typedef void (*OpenEndFunc)(OpenEndState*);

/**
 * @brief Initialises the intro (copyright / splash) screen.
 *
 * Loads copyright screen assets (Square Enix, Jupiter, Nintendo, CRI)
 * and resets @c introFrameCount.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_IntroScreen_Init(OpenEndState* state);

/**
 * @brief Updates the intro screen each frame.
 *
 * Advances @c introFrameCount and transitions to the title screen
 * once the intro duration has elapsed or the screen is touched.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_IntroScreen_Update(OpenEndState* state);

/**
 * @brief Destroys the intro screen and unloads its assets.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_IntroScreen_Destroy(OpenEndState* state);

/**
 * @brief Initialises the title screen.
 *
 * Loads the opening background graphics, creates badge tasks for
 * New Game and Continue, and resets @c titleFrameCount.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_TitleScreen_Init(OpenEndState* state);

/**
 * @brief Updates the title screen each frame.
 *
 * Polls touch input, updates @c selectedOption, handles the
 * @c alreadySelected flag, and triggers a fade-to-black when an
 * option is confirmed.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_TitleScreen_Update(OpenEndState* state);

/**
 * @brief Destroys the title screen and unloads its assets.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_TitleScreen_Destroy(OpenEndState* state);

/**
 * @brief Begins a fade-to-black transition.
 *
 * Decrements @c fadeBrightness each frame by @c fadeRate until the
 * screen is fully black.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_FadeToBlack(OpenEndState* state);

/**
 * @brief Begins a fade-from-black transition.
 *
 * Increments @c fadeBrightness each frame by @c fadeRate until the
 * screen is at full brightness.
 *
 * @param state Pointer to the current OpenEnd state.
 */
void OpenEnd_FadeFromBlack(OpenEndState* state);

const OpenEndFunc screenFunctions[2][3] = {
    {OpenEnd_IntroScreen_Init, OpenEnd_IntroScreen_Update, OpenEnd_IntroScreen_Destroy},
    {OpenEnd_TitleScreen_Init, OpenEnd_TitleScreen_Update, OpenEnd_TitleScreen_Destroy}
};

/**
 * @brief Dispatches a badge task to the appropriate callback.
 *
 * Selects the init, update, render, or cleanup callback from the badge
 * task jump table based on @p index and invokes it.
 *
 * @param pool      Pointer to the owning TaskPool.
 * @param task      Pointer to the Task to dispatch.
 * @param taskParam Pointer to the task parameter data.
 * @param index     Callback index (0=Init, 1=Update, 2=Render, 3=CleanUp).
 * @return          Return value from the dispatched callback.
 */
s32 OpenEnd_TaskBadge_RunTask(struct TaskPool* pool, struct Task* task, void* taskParam, s32 index);

enum {
    BADGE_NEWGAME,
    BADGE_CONTINUE,
} BadgeIndex;

extern SpriteAnimation g_BadgeAnim;

TaskPool*     g_taskPool;
s32           flag_screenTouched;
OpenEndState* g_OpenEndstate;
#endif // OPENEND_H