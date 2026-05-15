#ifndef PLAYER_PROGRESS_PROGRESSFLAGS_H
#define PLAYER_PROGRESS_PROGRESSFLAGS_H

#include <types.h>

typedef enum {
    GATE_GAME_CLEARED = 0, /**< The main storyline has been completed. */

} ProgressGate;

/**
 * Marks a progress gate as complete.
 *
 * This sets the bit corresponding to the given gate in the save data's
 * progress flag array.
 *
 * @param gate Progress milestone to mark as reached.
 */
void Progress_SetGate(ProgressGate gate);

/**
 * Clears a previously set progress gate.
 *
 * This clears the bit corresponding to the given gate in the save data's
 * progress flag array.
 *
 * @param gate Progress milestone to clear.
 */
void Progress_ClearGate(ProgressGate gate);

/**
 * Checks whether a progress gate has been set.
 *
 * @param gate Progress milestone to query.
 * @return TRUE if the gate is set, FALSE otherwise.
 */
BOOL Progress_Check(ProgressGate gate);

void func_ov030_020aec1c(s32 arg0);

#endif // PLAYER_PROGRESS_PROGRESSFLAGS_H
