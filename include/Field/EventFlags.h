#ifndef FIELD_EVENTFLAGS_H
#define FIELD_EVENTFLAGS_H

#include <nitro/types.h>

typedef enum {
    GATE_GAME_CLEARED = 0, /**< The main storyline has been completed. */

} EventGate;

/**
 * Marks a progression gate as complete.
 *
 * This sets the bit corresponding to the given gate in the save data's
 * progression flag array.
 *
 * @param gate Progression milestone to mark as reached.
 */
void FldGate_Set(EventGate gate);

/**
 * Clears a previously set progression gate.
 *
 * This clears the bit corresponding to the given gate in the save data's
 * progression flag array.
 *
 * @param gate Progression milestone to clear.
 */
void FldGate_Clear(EventGate gate);

/**
 * Checks whether a progression gate has been set.
 *
 * @param gate Progression milestone to query.
 * @return TRUE if the gate is set, FALSE otherwise.
 */
BOOL FldGate_Get(EventGate gate);

void func_ov030_020aec1c(s32 arg0);

#endif // FIELD_EVENTFLAGS_H
