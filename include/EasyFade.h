/**
 * @file EasyFade.h
 * @brief Display brightness fade control system.
 *
 * Provides functionality to smoothly fade display brightness using various interpolation modes.
 * Manages fade operations for main display, sub display, or both displays simultaneously.
 */

#ifndef EASYFADE_H
#define EASYFADE_H

#include "EasyTask.h"

/**
 * @enum FaderMode
 * @brief Specifies the interpolation method used for brightness transitions.
 */
typedef enum {
    FADER_LINEAR      = 0, ///< Linear fade: Update fader with constant step size
    FADER_INTERPOLATE = 1, ///< Frame-based interpolation: Smooth transition based on frame count
    FADER_SMOOTH      = 2, ///< Smooth easing: Accelerated/decelerated step size for natural motion
    FADER_INSTANT     = 3, ///< Immediate: Instantly set fader to target brightness without transition
} FaderMode;

/**
 * @struct Fader
 * @brief Manages the state and parameters of a single display's fade operation.
 */
typedef struct {
    /* 0x00 */ FaderMode mode;              ///< Current fade interpolation mode
    /* 0x04 */ s32       targetBrightness;  ///< Target brightness level (in 0x1000 fixed-point format)
    /* 0x08 */ s32       currentBrightness; ///< Current brightness level (in 0x1000 fixed-point format)
    /* 0x0C */ s32       rate;              ///< Fade speed/rate parameter
    /* 0x10 */ u16       framesRemaining;   ///< Frames remaining for frame-based interpolation mode
} Fader;

/**
 * @brief Task handle for the EasyFade system.
 *
 * Manages background fade operations for display brightness transitions.
 */
extern const TaskHandle Task_EasyFade;

/**
 * @brief Fade both displays to the specified brightness level.
 *
 * @param mode      The interpolation mode to use for the fade transition
 * @param brightness Target brightness level to fade to
 * @param rate      Fade speed/rate parameter (interpretation depends on mode)
 */
void EasyFade_FadeBothDisplays(FaderMode mode, s32 brightness, s32 rate);

/**
 * @brief Fade only the main display to the specified brightness level.
 *
 * @param mode      The interpolation mode to use for the fade transition
 * @param brightness Target brightness level to fade to
 * @param rate      Fade speed/rate parameter (interpretation depends on mode)
 */
void EasyFade_FadeMainDisplay(FaderMode mode, s32 brightness, s32 rate);

/**
 * @brief Fade only the sub display to the specified brightness level.
 *
 * @param mode      The interpolation mode to use for the fade transition
 * @param brightness Target brightness level to fade to
 * @param rate      Fade speed/rate parameter (interpretation depends on mode)
 */
void EasyFade_FadeSubDisplay(FaderMode mode, s32 brightness, s32 rate);

/**
 * @brief Check if either display is currently fading.
 *
 * @return TRUE if either the main or sub display is still fading to its target brightness.
 * @return FALSE if both displays have reached their respective target brightness levels.
 */
BOOL EasyFade_IsFading(void);

#endif // EASYFADE_H