#ifndef TOUCH_H
#define TOUCH_H

#include <types.h>

typedef struct {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
} TouchCoord;

/**
 * @brief Initialize the touch input system.
 */
void TouchInput_Init(void);

/**
 * @brief Update the touch input state.
 *
 * @return Pointer to raw touch input data, or NULL if touch is disabled.
 */
u16* TouchInput_Update(void);

/**
 * @brief Check if the touch screen is currently being touched.
 *
 * @return TRUE if the screen is being touched, FALSE otherwise.
 */
BOOL TouchInput_IsTouchActive(void);

/**
 * @brief Check if the touch screen was just pressed this frame.
 *
 * @return TRUE if the screen was just pressed, FALSE otherwise.
 */
BOOL TouchInput_WasTouchPressed(void);

/**
 * @brief Check if the touch screen was just released this frame.
 *
 * @return TRUE if the screen was just released, FALSE otherwise.
 */
BOOL TouchInput_WasTouchReleased(void);

/**
 * @brief Check if the touch screen is being held (touched for more than 1 frame).
 *
 * @return TRUE if the screen is being held, FALSE otherwise.
 */
BOOL TouchInput_IsBeingHeld(void);

/**
 * @brief Get the current touch coordinates.
 *
 * @param out Pointer to a TouchCoord structure to receive the coordinates.
 */
void TouchInput_GetCoord(TouchCoord* out);

/**
 * @brief Get the change in touch coordinates since the last frame.
 *
 * @param out Pointer to a TouchCoord structure to receive the delta values.
 */
void TouchInput_GetDelta(TouchCoord* out);

/**
 * @brief Get the duration the screen has been continuously touched.
 *
 * @return Hold duration in frames.
 */
s32 TouchInput_GetHoldDuration(void);

/**
 * @brief Set the hold duration threshold for tap detection.
 *
 * @param threshold Hold duration threshold in frames.
 */
void TouchInput_SetHoldThreshold(s32 threshold);

#endif // TOUCH_H