#ifndef COMBAT_CORE_COMBATAPI_H
#define COMBAT_CORE_COMBATAPI_H

/// Functions of overlay 3 that are called from other translation units.
/// Grouped by the unit that owns them; declarations that need a unit-private type live in that unit.

#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"

// CombatSprite.c

// CombatActor.c

// ov003_02083300.c
s32 func_ov003_02083470(void);

// ov003_02083950.c
/// Sets up the camera projection/position for one engine from a camera path entry.
void         func_ov003_02083a2c(s32, s32, s32, s32, s32);
void         func_ov003_02083ab0(s32, s32, s32, s32);
s32          func_ov003_02083bfc(s32*, s32, s32);
s32          func_ov003_02083c50(s32*, s32);
BOOL         func_ov003_02083ca0(s32*, s32*, s32, s32);
void         func_ov003_02083d34(void);
void         func_ov003_02083e50(s32);
void         func_ov003_020842e0(void);
void         func_ov003_02084678(Ov003Collider*);
void         func_ov003_020846ac(Ov003Collider*, s32, s32, s32);
void         func_ov003_020846c0(Ov003Collider*, s32*, s32*, s32*);
void         func_ov003_020847f0(void);
void         func_ov003_0208481c(void);
void*        func_ov003_02084998(CombatListNode*);
void         func_ov003_02084b88(void);
s32          func_ov003_02084d80(s32);
s32          func_ov003_02084db0(void);
void         func_ov003_02085054(void);
void         func_ov003_0208508c(void);
void         func_ov003_02085c78(void);
BOOL         func_ov003_02085cfc(u16);
BOOL         func_ov003_02085d3c(s32);
BOOL         func_ov003_02085d60(CombatActor*);
CombatActor* func_ov003_02085e78(s32);
CombatActor* func_ov003_02085ec0(s32);
CombatActor* func_ov003_02086014(s32);
void         func_ov003_020860c4(void);
void         func_ov003_02086130(void);
void         func_ov003_020862a4(s32);
void         func_ov003_020862f8(s32, s32);

void         func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
s32          func_ov003_020843b0(s32, s32);
s32          func_ov003_020843ec(s32, s32, s32);
s32          func_ov003_0208442c(s32, s32);
s32          func_ov003_02084460(s32, s32, s32);
void         func_ov003_02084634(Ov003Collider*, s32, s32, s32);
void         func_ov003_02084694(Ov003Collider*, s32);
CombatList*  func_ov003_020846f0(s32);
void         func_ov003_020846fc(CombatList*, CombatList*);
void*        func_ov003_0208495c(CombatList*);
void*        func_ov003_02084984(CombatListNode*);
void         func_ov003_020849ac(CombatList*);
void         func_ov003_020849bc(CombatList*, CombatListNode*, void*);
void*        func_ov003_020849dc(CombatList*, CombatListNode*);
CombatActor* func_ov003_02085dc8(s32);
// ov003_02086404.c
s32 func_ov003_02086404(s32 engine, CombatSprite* sprite, u32 id);

// ov003_020864c0.c
void func_ov003_020864c0(s32 arg0, CombatSprite* arg1, s32 arg2);
/// Plays the actor record's sound: the normal SE when `arg0 == 0`, the alternate one otherwise.
void func_ov003_02086540(s32 arg0, s32 arg1, s32 arg2);
/// Spawns the battle effect task for actor record `id`, writing the task id through `out`.
s32  func_ov003_020865a0(u16 id, s32 arg1, s32 arg2, s32* out);
void func_ov003_020867a0(void);
/// Picks the hit effect/SE combo for whatever the main engine's camera is currently showing.
void func_ov003_020867c0(void);
/// Refills the effect command's queue with a fresh shuffle.
void func_ov003_02086cc0(void);
void func_ov003_020871cc(s32 arg0);
/// Pushes a battle actor towards `x`/`y`/`z` through the shared action-effect pool.
void func_ov003_02087250(Ov003EffectState* state, CombatActor* target, s32 angle);
void func_ov003_02087310(void);
void func_ov003_02087378(void);
/// Retires the action effects whose target actor is gone or defeated.
void func_ov003_020873e0(void);
s32  func_ov003_020876c0(void);
/// TRUE when the main display actor is far enough from the projected sub-screen position.
s32 func_ov003_02087704(void);
/// Plays one of the two hit-reaction SE sets, picked at random.
void func_ov003_02087804(s32 sub);
/// Moves the display actor of one engine to a new position and tells the HUD about it.
void func_ov003_02087b00(s32 mode, s32 x, s32 y, s32 z);

void func_ov003_020864dc(CombatActor*, u16);
s32  func_ov003_0208656c(void);
void func_ov003_02086c78(s32*, s16*);
s16* func_ov003_02086e94(void);
s32  func_ov003_02086f20(s32, s32);
void func_ov003_02087094(Ov003EffectCmd*, u8, s32);
void func_ov003_020871b0(Ov003EffectCmd*);
void func_ov003_0208722c(void);
// ov003_0208a488.c
/// True for the two encounters that are allowed to take damage while the battle is paused.
BOOL func_ov003_0208b690(u16 encounterId);
/// Bumps the battle's combo-length counter.
void func_ov003_0208cbac(void);
/// Extends the battle's longest combo.
void func_ov003_0208cbc8(void);
/// Picks the character that joins the battle for this chapter and tracks that it was used.
u16 func_ov003_0208d608(void);
/// Waits for the current icon animation to finish, then plays the idle one.
void func_ov003_0208ed98(BtlIcon* icon);
/// Plays the icon's damage animation for the current battle state.
void func_ov003_0208edcc(BtlIcon* icon);

void             func_ov003_0208afcc(s32);
void             func_ov003_0208b3cc(s32, s16);
void             func_ov003_0208b518(s32, Ov003EnemyActor*, s16);
BOOL             func_ov003_0208c79c(Ov003EnemyActor*, s32);
Ov003EnemyActor* func_ov003_0208c810(s32, s32, s32);
void             func_ov003_0208ec74(BtlMgrWork*);
// BtlBarBS.c
/// Spawns the BS bar task for the actor that just entered the battle.
void func_ov003_0209ab9c(CombatActor* actor);

// BtlPop_Combo.c
/// Posts a combo increase through the mailbox and records the longest combo of the battle.
void func_ov003_0209c150(s16 combo);

// BtlPop_State.c
s32 func_ov003_0209cb70(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209cc44(TaskPool* pool, Task* task, void* args);
s32 func_ov003_0209cc58(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlPop_Star.c
/// Splits `value` (clamped to 0..99) into decimal digits, least significant first, and returns
/// how many digits are needed. A value of -1 stores a lone `-` glyph.
s32 func_ov003_0209cd0c(s32* digits, s32 value);
/// Loads the popup sprite on the star-digit animation set and parks it under the number.
void func_ov003_0209cd74(CombatSprite* sprite, s32 variant);
s32  func_ov003_0209cdd4(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209ce80(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209cef0(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209cfdc(TaskPool* pool, Task* task, void* args);
/// Task dispatcher: calls the stage function for `stage`.
s32 func_ov003_0209cff0(TaskPool* pool, Task* task, void* args, s32 stage);
/// Spawns a star popup showing `value` at (`x`, `y`).
s32 func_ov003_0209d038(s32 value, s16 x, s32 y);

// BtlPop_Tech.c
/// Creates the popup task if a tech was used this frame. The task parameter is a bare `s16` tech
/// id (a `BtlPop_Tech_Args`), passed by address.
void func_ov003_0209d4b0(void);

// BtlSandstorm.c
s32 func_ov003_0209e70c(TaskPool*, Task*, void*, s32);

// BtlEff.c
/// Loads the effect sprite from the record `args->id` selects, positions it and accounts for it in
/// the per-screen live effect count. Returns 0 when the sprite could not be loaded.
s32 func_ov003_0209e754(TaskPool* pool, Task* task, void* args);
/// Advances the effect and stops it once its animation has played out.
s32 func_ov003_0209e864(TaskPool* pool, Task* task, void* args);
/// Draws the effect at its world position, depth-sorted by Y.
s32 func_ov003_0209e890(TaskPool* pool, Task* task, void* args);
/// Releases the effect sprite and drops it from the per-screen live effect count.
s32 func_ov003_0209e928(TaskPool* pool, Task* task, void* args);
/// Task dispatcher: runs `stage` of the effect's stage table unless the encounter cutscene is
/// running, which freezes every effect task.
s32 func_ov003_0209e990(TaskPool* pool, Task* task, void* args, s32 stage);
/// Spawns a hit effect of id `id` on the display engine `mode` at world position (`x`, `y`, `z`),
/// flipped to match `flip`. Ignored once ten effects are already live on that screen.
void func_ov003_0209e9f8(s32 mode, s32 id, s32 x, s32 y, s32 z, s32 flip);

// BtlEff_Hit.c
/// Loads the hit sprite from record `args->id`, positions it, plays its SE and accounts for it in
/// the per-screen live effect count. Returns 0 when the sprite could not be loaded.
s32 func_ov003_0209ea94(TaskPool* pool, Task* task, void* args);
/// Advances the hit sprite and stops it once its animation has played out.
s32 func_ov003_0209ebd0(TaskPool* pool, Task* task, void* args);
/// Draws the hit sprite at its world position, depth-sorted by Y. The main-screen half is drawn a
/// little lower than its projected Y.
s32 func_ov003_0209ebfc(TaskPool* pool, Task* task, void* args);
/// Releases the hit sprite and drops it from the per-screen live effect count.
s32 func_ov003_0209ec9c(TaskPool* pool, Task* task, void* args);
/// Task dispatcher: runs `stage` of the hit effect's stage table unless the encounter cutscene is
/// running, which freezes every effect task.
s32 func_ov003_0209ed08(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlEff_Dead.c
/// Spawns the death effect on `engine`'s screen at the given world position.
void func_ov003_0209f444(s32 engine, s32 x, s32 y, s32 z, s32 hasPartner);

// BtlEff_Warp.c
/// Builds the previous-position half and current-position half, both flipped like the actor.
s32 func_ov003_0209f5e4(TaskPool* pool, Task* task, void* args);
/// Task update: advances both halves, keeping the task alive while either is still playing.
s32 func_ov003_0209f694(TaskPool* pool, Task* task, void* args);
/// Task render: draws both halves.
s32 func_ov003_0209f6c8(TaskPool* pool, Task* task, void* args);
/// Task destroy: releases both halves.
s32 func_ov003_0209f6e8(TaskPool* pool, Task* task, void* args);
/// Task dispatcher: calls the stage function for `stage` unless the encounter cutscene is running.
s32 func_ov003_0209f708(TaskPool* pool, Task* task, void* args, s32 stage);
/// Spawns the warp effect that trails the display actor from (`x`, `y`, `z`) to its new position.
void func_ov003_0209f770(s32 mode, s32 x, s32 y, s32 z);

// BtlEff_Splash.c
void func_ov003_0209f7c4(CombatSprite* sprite, u16 anim);
s32  func_ov003_0209fb1c(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209fb8c(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209fbec(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209fc2c(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209fc40(TaskPool* pool, Task* task, void* args, s32 stage);

void func_ov003_0209fc88(s32, s32, s32, s32, s16, s32);
// BtlEff_Star.c
void func_ov003_0209fd48(s32 engine, CombatSprite* sprite);
s32  func_ov003_0209ff9c(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a00e8(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a0180(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a01cc(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a01fc(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlEff_LvUp.c
s32 func_ov003_020a0430(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020a0490(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020a0548(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020a0568(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020a05a4(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlDbg.c
void func_ov003_020a29b8(void);
/// Posts a debug rectangle for `life` frames. Declared `s32` because a caller forwards the (undefined) return register.
s32 func_ov003_020a2a04(u16 kind, u16 life, s32 x, s32 y, s16 width, s16 height);

// BtlInf_ShowVal.c
void func_ov003_020a300c(void);
void func_ov003_020a303c(const u16* items, s32 touchIndex);

// ov003_020a30d4.c
s32  func_ov003_020a3340(s32 actionId);
s32  func_ov003_020a3350(s32 actionId);
s32  func_ov003_020a3478(s32 actionId);
s32  func_ov003_020a3488(s32 actionId);
void func_ov003_020a35b8(CombatSprite* cSprite, s32 index, s32 arg2);
/// Drops every touch action actor of the battle.
void func_ov003_020a35d8(void);
/// Publishes the actor positions to the 3D layer's slot table.
void func_ov003_020a3608(void);
/// Queues one touch event for every equipped pin of the current deck (`arg0` selects the deck
/// of the active player).
void func_ov003_020a3728(s32 arg0);
/// Spawns the touch icon task and queues one action per equipped pin of the current deck.
void func_ov003_020a37a8(s32 arg0);
/// Runs one update of every touch action actor.
void func_ov003_020a3924(void);
/// TRUE while at least one non-player actor is alive.
s32 func_ov003_020a3980(void);
/// Picks the actor under the touch/stylus and moves it there; SELECT resets the positions.
void func_ov003_020a3a80(void);

s32 func_ov003_020a3330(s32);
s32 func_ov003_020a3468(s32);
// ov003_020a3b9c.c
/// TRUE while the actor can be pushed around by the other actors.
s32 func_ov003_020a3e50(Ov003TouchActor* actor);
/// TRUE while the actor's kind matches the current battle mode.
s32 func_ov003_020a3edc(Ov003TouchActor* actor);
/// TRUE when a touch is inside one of the actor's active touch areas.
s32 func_ov003_020a3f20(Ov003TouchActor* actor);
/// TRUE when the release point of the current touch is inside the actor.
s32  func_ov003_020a401c(Ov003TouchActor* actor);
void func_ov003_020a40b0(Ov003TouchActor* actor);
s16  func_ov003_020a40bc(Ov003TouchActor* actor);
/// Pushes overlapping actors apart, then keeps every actor inside the touch area.
void func_ov003_020a4158(void);
/// Requests state `request` from the actor; a "revive" request while the actor is still fading
/// turns into the death state instead.
void func_ov003_020a4390(Ov003TouchActor* actor, s32 request);
/// Loads the raw pin record `pinId` into `out`.
s32 func_ov003_020a474c(u16 pinId, void* out);
/// The scaled duration of psych kind `params->unk_05` at level `level`.
s32 func_ov003_020a4778(Ov003PsychParams* params, s32 level);
/// Sets up the two sprite slots and the trail of the psych effect object.
void func_ov003_020a4a34(Ov003TrailEffect* effect);
/// Updates the psych effect trail; returns 0 when it is finished.
s32 func_ov003_020a4aac(Ov003TrailEffect* effect);
/// Renders the psych trail.
void func_ov003_020a4c8c(Ov003TrailEffect* effect);
void func_ov003_020a4d50(Ov003TrailEffect* effect);
/// Checks that the trail is long and straight enough to fire the psych.
s32 func_ov003_020a4d6c(Ov003TrailEffect* effect);
/// Binds camera `index` to the animation driven by `obj`.
void func_ov003_020a4e84(Ov003CameraAnim* obj, s32 index, s32 arg2);
void func_ov003_020a4ed0(Ov003CameraAnim* obj, s32 period, s32 offset);
/// Steps the camera shake animation; returns 1 when it hit a limit this frame.
s32  func_ov003_020a4ee8(Ov003CameraAnim* obj, s32 delta, s32 min, s32 max);
void func_ov003_020a4fd0(Ov003CameraAnim* obj);
/// Sets up the 3D layer the touch action sprites are rendered through.
void func_ov003_020a4ff0(void);

// BtlMic.c
void func_ov003_020a5080(void);
s32  func_ov003_020a50cc(void);

// ov003_020a5250.c
/// Integrates one touch actor's velocity, bouncing it off the battle bounds and pushing it away
/// from overlapping actors; also ramps the effect speed of actors near the effect origin.
void func_ov003_020a5250(Ov003TouchActor* actor);
/// Starts effect `effect`: resets its per-frame counters and spawns its task/callbacks.
u16 func_ov003_020a5660(Ov003TouchActor* actor);
/// Whether `actor` should be considered by the touch-effect selection.
s32 func_ov003_020a56c8(Ov003TouchActor* actor);
/// Collects the live touch actors into `slots`; `mode != 0` keeps only the two chain-eligible
/// effect kinds. Returns the number collected.
s32 func_ov003_020a5738(Ov003TouchActor** slots, s32 count, s32 mode);
/// Picks the highest-priority live touch actor and starts its effect; returns the effect's
/// animation id (0 if nothing was picked).
u16 func_ov003_020a5804(s32 mode);
/// Records the touch-slot origins used by the effect selection.
void func_ov003_020a593c(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
/// Fires the shared effect for all five touch slots and arms its cooldown.
void func_ov003_020a5974(Ov003EffectState* effect);
void func_ov003_020a59dc(void);
s32  func_ov003_020a59f8(void);
u8   func_ov003_020a5a1c(void);
/// Finds the touch actor that owns the effect state at `effect`.
Ov003TouchActor* func_ov003_020a5a34(Ov003EffectState* effect);
void             func_ov003_020a5a94(Ov003EffectState* effect);
s32              func_ov003_020a5aa4(Ov003EffectState* effect);
s32              func_ov003_020a5ab4(Ov003EffectState* effect);
/// Sums the per-actor amount of every actor running effect `state`.
s32 func_ov003_020a5b18(s32 state);
/// Number of slots effect `state` occupies.
s32 func_ov003_020a5b80(s32 state);
/// Fills `slots` with the actors participating in `effect`, one per effect slot.
void  func_ov003_020a5bac(Ov003TouchActor** slots, s32 count, Ov003EffectState* effect, s32 arg3);
void* func_ov003_020a5d2c(s32 size, const char* name);
void  func_ov003_020a5d50(void* ptr);
/// Staggers the countdown timers of `count` sprite slots.
void func_ov003_020a5d88(Ov003AnimSprite* sprites, s32 count, s32 step);

// ov003_020a5db8.c
/// Advances the spin/fade timers of `count` sprite slots and restarts their animations on demand.
void func_ov003_020a5db8(Ov003AnimSprite* sprites, s32 count, s32 animSet, s32 arg3, s32 arg4, s32 arg5);
/// Spins the live sprite slots around (x, y) and renders them.
void func_ov003_020a5ed4(Ov003AnimSprite* sprites, s32 count, s32 x, s32 y);
/// Releases the live sprite slots.
void  func_ov003_020a5fc0(Ov003AnimSprite* sprites, s32 count);
BOOL  func_ov003_020a603c(u32* id);
void  func_ov003_020a6060(u32 id);
void* func_ov003_020a6084(u32 id);
/// Whether `unit` can take the pending command tagged `arg1`.
s32 func_ov003_020a60a8(Ov003EnemyActor* unit, s32 arg1);
/// Whether `unit` can be given `arg1`; the busy/idle and status bits gate it.
s32 func_ov003_020a60ec(Ov003EnemyActor* unit, s32 arg1);
/// Applies pending command 4 to `unit` and records `arg1`.
s32 func_ov003_020a6164(Ov003EnemyActor* unit, u16 arg1);
/// Applies pending command 5 to `unit` and clears its velocity.
void func_ov003_020a6198(Ov003EnemyActor* unit, s32 arg1);
/// Applies pending command 5 and launches `unit` at `speed` in direction `angle`.
void func_ov003_020a61e4(Ov003EnemyActor* unit, u16 arg1, s32 attackId, Ov003AttackArgs* args, s32 angle, s32 speed,
                         s32 zVelocity);
s32  func_ov003_020a62d0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32  func_ov003_020a62ec(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
/// Whether a (0..0x100, 0..0xC0) point lies inside a square of half-size `arg2`.
s32 func_ov003_020a6374(s32 arg0, s32 arg1, s32 arg2);
/// Whether the current battle lets the effect run.
s32 func_ov003_020a63a8(Ov003EffectState* effect);
/// Whether the battle is in the post-boss state that unlocks the extra effects.
s32 func_ov003_020a63e4(void);
/// The player's battle actor.
CombatActor* func_ov003_020a6438(void);
void         func_ov003_020a6450(void);
void         func_ov003_020a6464(s32 arg0);
void         func_ov003_020a647c(s32 arg0);
void         func_ov003_020a6494(s16 arg0);
void         func_ov003_020a64ac(void);
/// Initialises the effect's sprite anchor from the owning touch actor.
void func_ov003_020a64d0(Ov003EffectState* effect, s16 arg1);
/// Queues a sweep attack against `unit`, resetting the streak when the info record says so.
s32 func_ov003_020a6568(Ov003AttackStreak* streak, s32 attackId, Ov003EnemyActor* unit, s32 x, s32 force, u16 amount);
/// Queues a point attack at (x, y, z) and resets the streak when the info record says so.
s32 func_ov003_020a6628(Ov003AttackStreak* streak, s32 attackId, s32 x, s32 y, s32 z, s32 force, u16 amount);
/// Segment-segment intersection test; both segments are axis-aligned bounding boxes first.
s32 func_ov003_020a67d4(s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 x4, s32 y4);
/// Whether `unit`'s screen-space bounding box overlaps (x1, y1)-(x2, y2).
s32 func_ov003_020a6924(Ov003EnemyActor* unit, s32 x1, s32 y1, s32 x2, s32 y2);
/// Queues a sweep attack against every unit whose box overlaps (x1, y1)-(x2, y2).
s32 func_ov003_020a6acc(Ov003AttackStreak* streak, s32 attackId, s32 x1, s32 y1, s32 x2, s32 y2, s32 x);
/// Clears `*out` when the current encounter uses the same hit kind as `unit`.
void func_ov003_020a6bb8(s16* out, Ov003EnemyActor* unit);
/// Story-progress gate for the pin effects: 0 = none, 1 = early, 2 = late.
s32 func_ov003_020a6c14(void);

// BtlBadge.c
void func_ov003_020a6d24(void*);

// BtlBadge_ExecEffect.c
void func_ov003_020a7284(Ov003TouchActor*);

// BtlBadge_Gauge.c
void func_ov003_020a7640(void*);

// BtlBadge_Stock.c
void func_ov003_020a7e68(void*);

// BtlBadge_RankingEffect.c
void func_ov003_020a7ba8(Ov003TouchActor*);

// BtlBadge_TimeBoot.c
void func_ov003_020a8300(Ov003TouchActor*, s32, s32);

// BtlPsy_FldShake.c
s32  func_ov003_020a8718(TaskPool* pool, Task* task, void* args, s32 stage);
void func_ov003_020a8768(void);
s32  func_ov003_020a8780(TaskPool* pool, Task* task, void* args, s32 stage);

// CmnTsk_BtlPsy_EffBrilliant.c
s32   func_ov003_020a8ec4(TaskPool* pool, Task* task, void* args, s32 stage);
Vec2p func_ov003_020a904c(s32 radius, s32 fixedRadius);

// SubTsk_BtlPsy_FireChild.c
s32 func_ov003_020a9530(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlPsy_FireWave_D.c
s32 func_ov003_020a9ac4(TaskPool* pool, Task* task, void* args, s32 stage);

// SubTsk_BtlPsy_Barrier.c
void func_ov003_020aa860(Ov003Barrier* barrier);
void func_ov003_020aa990(Ov003Barrier* barrier);
void func_ov003_020aa9f4(Ov003EffectState* effect, s32 percent);

// BtlPsy_Barrier.c
s32 func_ov003_020aaad4(Ov003EffectContext* context, Ov003EffectState* effect, s32 automatic, s32 mode, s32 kind);

// SubTsk_BtlPsy_BlackHoll.c
s32 func_ov003_020aad24(TaskPool* pool, Task* task, void* args, s32 stage);

// SubTsk_BtlPsy_Bomb.c
s32 func_ov003_020ab56c(TaskPool* pool, Task* task, void* args, s32 stage);

// SubTsk_BtlPsy_FireWorks.c
s32 func_ov003_020ab828(TaskPool* pool, Task* task, void* args, s32 stage);

// SubTsk_BtlPsy_ObsBomb.c
/// Adjusts the contact attack's dimensions to the selected object's dimensions.
s32 func_ov003_020abdb8(Ov003ObsBomb* self);

// SubTsk_BtlPsy_PsychoBomb.c
s32 func_ov003_020abe60(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlPsy_Blur.c
void func_ov003_020acd04(void);

// BtlPsy_DragAttack_Effect.c
s32 func_ov003_020acd6c(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020acdc0(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020ace54(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020acebc(TaskPool* pool, Task* task, void* args);

// SubTsk_BtlPsy_Spark.c
s32 func_ov003_020ad364(TaskPool* pool, Task* task, void* args, s32 stage);

// SubTsk_BtlPsy_ElementK.c
void func_ov003_020adbec(Ov003ElementK* self);
void func_ov003_020adc8c(Ov003ElementK* self);
void func_ov003_020adcb4(Ov003ElementKRay* ray);
void func_ov003_020adcd8(Ov003ElementKRay* ray, s32 level, s32 direction);
void func_ov003_020add40(Ov003ElementKRay* ray, s16 x, s16 y, Ov003EffectState* effect);
void func_ov003_020ade3c(Ov003ElementKRay* ray, s32 x, s32 y);
void func_ov003_020ade74(Ov003ElementKRay* ray);

// SubTsk_BtlPsy_ElementR.c
void func_ov003_020ae324(Ov003ElementR* self);
void func_ov003_020ae350(Ov003ElementR* self);
void func_ov003_020ae3a4(Ov003ElementR* self);

// SubTsk_BtlPsy_Element.c
s32  func_ov003_020ae46c(TaskPool* pool, Task* task, void* args, s32 stage);
void func_ov003_020ae890(Ov003Element* self);
void func_ov003_020ae8b4(Ov003Element* self);

// BtlPsy_Element_S.c
s32 func_ov003_020af2d8(TaskPool* pool, Task* task, void* args, s32 stage);

// SubTsk_BtlPsy_EnemyAirFlow.c
s32 func_ov003_020b01d8(TaskPool* pool, Task* task, void* args, s32 stage);
s32 func_ov003_020b03b4(Ov003EnemyAirFlow* self);
s32 func_ov003_020b0470(Ov003EnemyAirFlow* self);

// BtlPsy_FireHold.c
/// The actor currently held by the touch, if it is a live enemy.
Ov003EnemyActor* func_ov003_020b0fa0(void);
void             func_ov003_020b0fdc(Ov003EnemyActor* unit);
void             func_ov003_020b1024(Ov003EnemyActor* unit);
void             func_ov003_020b1094(Ov003EnemyActor* unit, s32 charged);

// BtlPsy_IceHold.c
/// The actor tracked by the ice psych.
Ov003EnemyActor* func_ov003_020b15a0(void);

// BtlPsy_OpticalHold.c
/// Returns the held actor while the optical-hold touch is active, or NULL when it is not.
Ov003EnemyActor* func_ov003_020b1c50(void);

// BtlPsy_IcePillar_R.c
/// Sways every pillar centre sideways and flips the sway direction for the
/// next step, so the three beams weave around the touch point.
void func_ov003_020b26cc(Ov003IcePillarR* self);

// BtlPsy_LockOnThunder_D.c
void func_ov003_020b3464(Ov003Thunder* self);
void func_ov003_020b35c0(Ov003Thunder* self);
void func_ov003_020b3698(Ov003Thunder* self);
s32  func_ov003_020b38d4(Ov003LockOnThunder* self);
s32  func_ov003_020b39ac(Ov003LockOnThunder* self);

// BtlPsy_LockOnThunder_P.c
s32 func_ov003_020b3bdc(Ov003LockOnThunderP* self);
s32 func_ov003_020b3d5c(Ov003LockOnThunderP* self);

// BtlPsy_LockOnThunder_T.c
s32 func_ov003_020b3edc(Ov003LockOnThunderT* self);
s32 func_ov003_020b3fac(Ov003LockOnThunderT* self);

// SubTsk_BtlPsy_PsychoFall_T.c
s32 func_ov003_020b42f4(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlPsy_PsychoFall_T.c
/// Spawns `effect->params[0]` falling shards around the anchored position; the first one lands
/// dead centre and the rest are scattered along a random horizontal angle.
s32 func_ov003_020b4a44(Ov003PsychoFallT* self);

// SubTsk_BtlPsy_Psycho_T.c
void func_ov003_020b4e48(Ov003EnemyActor* unit, Ov003PsychoHitContext* context);
s32  func_ov003_020b4e8c(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlPsy_Psycho_D.c
s32 func_ov003_020b5480(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlPsy_Psycho_S.c
Ov003EnemyActor* func_ov003_020b5f04(s32 index);

// BtlPsy_Psycho_T.c
Ov003EnemyActor* func_ov003_020b6308(Ov003EffectState* effect);

// BtlPsy_Heal.c
s32  func_ov003_020b6724(TaskPool* pool, Task* task, void* args, s32 stage);
void func_ov003_020b67dc(Ov003EffectState* effect, CombatActor* actor);
s32  func_ov003_020b68ec(Ov003EffectState* effect, CombatActor* actor);

// ov003_020b6b20.c
s32 func_ov003_020b6b20(Ov003ShockWaveArgs* args);

// BtlPsy_ShockWave_Body.c
s32  func_ov003_020b6b38(s32 kind);
u32  func_ov003_020b6b50(u32 angle, u32 limit);
s32  func_ov003_020b6ba4(TaskPool* pool, Task* task, void* args, s32 stage);
void func_ov003_020b7b34(s32 x, s32 y, s32 z, s32 kind, s32 flip, u32* out);

// BtlPsy_BoomerangShot_S.c
s32 func_ov003_020b7e68(Ov003BoomerangShotS* self);

// BtlPsy_BoomerangShot_T.c
s32 func_ov003_020b81cc(Ov003BoomerangShotT* self);

// BtlPsy_BurstShot_T.c
s32 func_ov003_020b83ec(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlPsy_StraightShot_S.c
s32 func_ov003_020b87f8(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlPsy_LineThunder_S.c
void func_ov003_020b96d8(Ov003LineThunder* self);
void func_ov003_020b9764(s32* outX, s32* outY, s32 x1, s32 y1, s32 x2, s32 y2);

// BtlPsy_Psychithunder_T.c
void func_ov003_020b9a14(Ov003Psychithunder* self);

// BtlPsy_StreetLive.c
void func_ov003_020ba68c(Ov003StreetLive* self);

// BtlPrizeItem.c
void func_ov003_020bb190(s32, u32, s32);
s32  func_ov003_020bb47c(void);
void func_ov003_020bb580(void*);

s32 func_ov003_020bb48c(s32, s8, u16, s32, s32, s32, s32);
// ov003_020bd1bc.c
void func_ov003_020be210(Ov003CutinTask* self);
void func_ov003_020be2a8(Ov003CutinTask* self);
void func_ov003_020be3d0(Ov003CutinTask* self);

// BtlCutIn00.c
s32 func_ov003_020be658(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020be660(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020be688(TaskPool* pool, Task* task, void* args, s32 stage);

// BtlActSp00.c
void func_ov003_020c1f18(void*, u16);

// ov003_020c2064.c
/// Scales `value` by `scale / total` while the cut-in slow-motion flag is off.
void func_ov003_020c228c(s32 value, s32 total);
void func_ov003_020c2324(s32 value, s32 total);
/// Default cut-in layout, used when the battle does not select a scripted one.
void func_ov003_020c23bc(void);
/// Scripted cut-in layout: walks the 12-bit command list in `data_ov003_020e53f4[index]`.
void func_ov003_020c2624(u16* script);
/// Starts cut-in `index` (0 = the default layout) and kicks off its actors.
void func_ov003_020c2a34(s32 index);
void func_ov003_020c2a7c(void);
/// (Re)initialises the shared save-resident cut-in slot table.
void func_ov003_020c2af0(void);
u32  func_ov003_020c2b3c(s32 index);
void func_ov003_020c2b4c(s32 index, s32 value);
void func_ov003_020c2b5c(Ov003CutinActor* actor);
void func_ov003_020c2b68(Ov003CutinActor* actor);
/// True while the actor is safely inside the camera bounds.
s32 func_ov003_020c2bbc(Ov003CutinActor* actor);
/// Three-phase approach/attack/leave state machine used by the normal cut-in attack.
s32 func_ov003_020c2c30(Ov003CutinActor* actor, s32 animBase, s32 targetX, s32 targetY, s32 targetZ, s32 arg5);
/// Projectile-like cut-in attack state machine (fly toward the target, then leave).
s32  func_ov003_020c2f68(Ov003CutinActor* actor, s32 animBase, s32 speed, s32 spread);
void func_ov003_020c32a4(Ov003CutinActor* src, Ov003CutinActor* dst);
/// Rotates the main/sub/third actor chain when the third actor is present.
void func_ov003_020c34dc(Ov003CutinActor* actor);
void func_ov003_020c353c(Ov003CutinActor* actor);
/// Third-actor entrance/exit state machine.
s32  func_ov003_020c3570(Ov003CutinActor* actor, u16 animBase);
void func_ov003_020c36d4(s32 scale, Vec* src, Vec* dst);
void func_ov003_020c3700(Ov003EnemyActor* actor);
void func_ov003_020c3794(Ov003EnemyActor* actor);
void func_ov003_020c37bc(Ov003EnemyActor* actor);
/// Fills an animation descriptor from a cut-in actor parameter table and loads it into a sprite.
void func_ov003_020c3814(s32 arg0, CombatSprite* sprite, s32 unused, const u8* table, u16 index, u16 arg4, u16 arg5, s16 arg6,
                         u16 arg7, s16 arg8);
s32  func_ov003_020c38b8(CombatSprite* sprite, u16 anim, s32 arg2);
s32  func_ov003_020c3920(CombatSprite* sprite);
s32  func_ov003_020c3938(CombatSprite* sprite);
s32  func_ov003_020c3950(u32 index);
/// Clamps `*value` to the camera's horizontal bounds.
s32 func_ov003_020c3960(s32* value, s32 margin);
/// Rotates a 2D offset so it points along the vector between the cut-in actor and its target.
void         func_ov003_020c39b0(Vec* src, Vec* dst);
TaskPool*    func_ov003_020c3a10(u32 index);
Ov003Camera* func_ov003_020c3a48(u32 index);
s32          func_ov003_020c3a70(u32 index);
void         func_ov003_020c3acc(s32 engine, void* arg1, s32 arg2);
void         func_ov003_020c3b1c(u16* tracks);
/// Returns the first index whose cumulative weight exceeds `target`.
s32       func_ov003_020c3b30(u16* weights, s32 count, s32 target);
void      func_ov003_020c3b6c(u16* tracks);
void      func_ov003_020c3ba8(s32 x, s32 y);
s32       func_ov003_020c3bf0(void* actor);
TaskPool* func_ov003_020c3ca4(Ov003CutinActor* actor);
s32       func_ov003_020c3cbc(Ov003CutinActor* actor);
void      func_ov003_020c3ccc(Ov003CutinActor* actor);
void      func_ov003_020c3cdc(Ov003CutinActor* actor);
void      func_ov003_020c3e64(Ov003EnemyActor*);
void      func_ov003_020c3e68(Ov003EnemyActor*, Ov003EnemyActor*);
void      func_ov003_020c3e88(Ov003CutinActor* actor, s32 arg1);

s32                  func_ov003_020c37f8(CombatSprite*);
s32                  func_ov003_020c38e8(CombatSprite*);
s32                  func_ov003_020c3908(CombatSprite*);
u32                  func_ov003_020c3aac(u32);
s32                  func_ov003_020c3c28(void);
void                 func_ov003_020c3c44(Ov003CutinScript*, void*, s16);
const BtlSpriteDesc* func_ov003_020c3c6c(Ov003EnemyActor*);
Ov003ActorParams*    func_ov003_020c3c88(Ov003EnemyActor*);
void                 func_ov003_020c3cec(void*, void*, void*, void*);
s32                  func_ov003_020c3d28(void*, void*, void*, void*, s32*, s32*);
s32                  func_ov003_020c3ec0(CombatActor*);
void                 func_ov003_020c3efc(Ov003EnemyActor*, Ov003CutinCmdData*);
// ov003_020c41e8.c
/// Resets a cut-in actor to its idle defaults (a fresh 0x1C0-byte actor block).
void func_ov003_020c41e8(Ov003CutinActor* actor);
void func_ov003_020c4264(Ov003EnemyActor* actor);
/// Releases the main sprite's resources.
void func_ov003_020c4268(Ov003CutinActor* actor);
void func_ov003_020c4288(Ov003EnemyActor* actor);
void func_ov003_020c42d4(Ov003CutinActor* actor);
s32  func_ov003_020c44a4(Ov003EnemyActor* actor);
/// Same as `func_ov003_020c4520`, using the secondary load template.
s32  func_ov003_020c45a4(Ov003CutinActor* actor);
void func_ov003_020c49c8(Ov003EnemyActor* actor);
void func_ov003_020c4a08(Ov003CutinActor* actor);
s32  func_ov003_020c4a34(Ov003EnemyActor* actor);
s32  func_ov003_020c4a74(Ov003EnemyActor* actor);
void func_ov003_020c4b9c(Ov003CutinActor* actor);
void func_ov003_020c4bdc(Ov003CutinActor* actor);
/// Returns 1 if `value` is already in `table`.
s32 func_ov003_020c4cf0(s32 value, s32* table, s32 count);
/// Stores `value` into the first free slot of `table`.
s32 func_ov003_020c4d20(s32 value, s32* table, s32 count);
/// Clears every slot of `table` holding `value`.
void func_ov003_020c4d54(s32 value, s32* table, s32 count);
void func_ov003_020c4d84(void);
void func_ov003_020c4da8(void);
s32  func_ov003_020c4dcc(s32 value);
s32  func_ov003_020c4dec(s32 value);
s32  func_ov003_020c4e0c(s32 value);
s32  func_ov003_020c4e7c(s32 value);
void func_ov003_020c4eb4(Ov003CutinActor* actor);
/// Retires the actor's main-screen command slot.
void func_ov003_020c4f00(Ov003EnemyActor* actor);
/// True if any actor on `list` has cut-in instance id `id`.
s32  func_ov003_020c4f20(s32 id, CombatList* list);
void func_ov003_020c4f60(s32 id);
void func_ov003_020c4f80(s32 id);
/// Knocks the actor out: zero HP, defeat command, "defeated" flag.
void func_ov003_020c4fa0(Ov003EnemyActor* actor);
s32  func_ov003_020c4ff4(Ov003EnemyActor* actor);
s32  func_ov003_020c5058(Ov003EnemyActor* actor);
void func_ov003_020c50bc(void);
/// Pushes the actor's hit effects to the combo counters.
void func_ov003_020c5130(Ov003EnemyActor* actor);
/// Nudges the actor one eighth of the way towards (x, y), clamped to +/-0x8000.
void func_ov003_020c51a4(Ov003CutinActor* actor, s32 x, s32 y);
/// True if the actor and its partner swapped sides of the sub-screen centre.
s32 func_ov003_020c5220(Ov003EnemyActor* actor);
/// True if the actor's box does not overlap any actor on the sub-screen list.
s32 func_ov003_020c5288(Ov003EnemyActor* actor, s32 x, s32 y);
/// Counts the sub-screen cut-in actors, and how many of them are "large" (width > 0x40).
void func_ov003_020c54e0(s32* over, s32* total);
/// Picks a randomised rise speed for the cut-in, scaled by how crowded the sub screen is.
void func_ov003_020c554c(Ov003EnemyActor* actor, s32* outX, s32* outY, u16 arg3);
s32  func_ov003_020c5864(Ov003CutinActor* actor, s32* outX, s32* outY);
s32  func_ov003_020c58c8(Ov003CutinActor* actor, s32* outX, s32* outY, u16 arg3);
s32  func_ov003_020c59cc(Ov003EnemyActor* actor);
s32  func_ov003_020c59e0(Ov003EnemyActor* actor);
s32  func_ov003_020c59f4(Ov003EnemyActor* actor);
s32  func_ov003_020c5a00(Ov003CutinActor* actor);
s32  func_ov003_020c5a14(Ov003CutinActor* actor);
s32  func_ov003_020c5a28(Ov003CutinActor* actor);
s32  func_ov003_020c5a60(Ov003CutinActor* actor);
void func_ov003_020c5a98(Ov003CutinActor* actor);
void func_ov003_020c5ac0(s32 arg0, s32 arg1, Ov003CutinActor* actor, s32 arg3, s32 arg4, s32 arg5);
s32  func_ov003_020c5b0c(u16 id, void* owner, s32 arg2);
s32  func_ov003_020c5b64(u16 id, void* owner, s32 x, s32 y);
s32  func_ov003_020c5b8c(const Ov003AttackInfo* info, void* owner, s32 x, s32 y, s32 z);
s32  func_ov003_020c5ba4(u16 id, void* args, s32 arg2);
/// Third cut-in state machine: dash in, bounce and recover.
s32  func_ov003_020c6920(Ov003EnemyActor* actor, u16 anim, s32 arg2);
void func_ov003_020c6b9c(Ov003CutinActor* actor, u16 anim);
void func_ov003_020c6bbc(Ov003CutinActor* actor, u16 anim);
void func_ov003_020c6e0c(Ov003EnemyActor* actor);
void func_ov003_020c6e58(Ov003EnemyActor* actor);
/// Final cut-in sequence: camera fade-out and sprite release.
s32 func_ov003_020c6e64(Ov003EnemyActor* actor);
s32 func_ov003_020c6f40(Ov003EnemyActor*);
s32 func_ov003_020c6f68(Ov003EnemyActor*);
/// Drives the end-of-cut-in teardown task until the fade completes.
s32 func_ov003_020c6f90(Ov003EnemyActor* actor);

void func_ov003_020c427c(Ov003EnemyActor*);
s16  func_ov003_020c42ec(Ov003EnemyActor*);
s32  func_ov003_020c4348(Ov003EnemyActor*);
s32  func_ov003_020c44ac(Ov003EnemyActor*);
s32  func_ov003_020c4520(Ov003EnemyActor*);
void func_ov003_020c4628(Ov003EnemyActor*);
void func_ov003_020c4668(Ov003EnemyActor*);
void func_ov003_020c4748(Ov003EnemyActor*);
s32  func_ov003_020c4830(Ov003EnemyActor*);
void func_ov003_020c48b0(Ov003EnemyActor*);
void func_ov003_020c492c(Ov003EnemyActor*);
void func_ov003_020c495c(Ov003EnemyActor*);
void func_ov003_020c4988(Ov003EnemyActor*);
void func_ov003_020c4ab4(Ov003EnemyActor*, s32);
void func_ov003_020c4af0(Ov003EnemyActor*);
void func_ov003_020c4b1c(Ov003EnemyActor*);
void func_ov003_020c4b5c(Ov003EnemyActor*);
void func_ov003_020c4c1c(Ov003EnemyActor*);
void func_ov003_020c4c5c(Ov003EnemyActor*);
void func_ov003_020c4c9c(Ov003EnemyActor*);
void func_ov003_020c4cc4(Ov003EnemyActor*, s32);
void func_ov003_020c4ee0(Ov003EnemyActor*);
void func_ov003_020c4fc8(Ov003EnemyActor*);
s32  func_ov003_020c5924(Ov003EnemyActor*, s32*);
s32  func_ov003_020c59a0(Ov003EnemyActor*);
s32  func_ov003_020c5b2c(u16, void*, s32, s32, s32);
s32  func_ov003_020c5bc4(u16, Ov003AttackArgs*, s32, s32, s32);
s32  func_ov003_020c5bfc(Ov003EnemyActor*);
s32  func_ov003_020c5e98(Ov003EnemyActor*);
s32  func_ov003_020c6230(Ov003EnemyActor*);
s32  func_ov003_020c62c4(Ov003EnemyActor*, u16);
s32  func_ov003_020c65cc(Ov003EnemyActor*, u16);
s32  func_ov003_020c6b8c(Ov003EnemyActor*, u16);
s32  func_ov003_020c6bac(Ov003EnemyActor*, u16);
s32  func_ov003_020c6bc8(Ov003EnemyActor*, u16);
s32  func_ov003_020c6c2c(Ov003EnemyActor*, u16);
// BossExp1.c
s32 func_ov003_020c703c(Ov003EnemyActor*);
s32 func_ov003_020c7070(Ov003EnemyActor*);
s32 func_ov003_020c72b4(Ov003EnemyActor*, s32, s32);
s32 func_ov003_020c9008(void);

// BtlEff_SideMove.c
void func_ov003_020c9118(Ov003EnemyActor*);
s32  func_ov003_020c9338(Ov003EnemyActor*);

void func_ov003_020c9064(const SpriteAnimationEx*);
// BtlEnmShot_Blossom.c
void             func_ov003_020cb128(s32*, s32, s32);
void             func_ov003_020cb130(BtlBinSet*, const BtlBinEntry*, u16, s32, const char*);
s32              func_ov003_020cb150(BtlBinSet*);
void             func_ov003_020cb194(BtlBinSet*);
u16              func_ov003_020cb3c4(s32, s32);
Ov003EnemyActor* func_ov003_020cb42c(s32, s32, s32);
s32              func_ov003_020cb498(s32, s32, BtlActorFilter, void*);
void             func_ov003_020cb594(Ov003EnemyActor*, s32);
s32              func_ov003_020cb764(s32);
s16              func_ov003_020cb888(u16*, s32, s32);
s32              func_ov003_020cb910(s32*, s32*, s32*, s32, s32, s32, s32, s32, s32, s32);
s32              func_ov003_020cba14(s32, s32, s32, s32);
s32              func_ov003_020cba2c(s32, s32, s32, s32);
s32              func_ov003_020cba54(s32, s32, s32, s32, s32, s32);
void             func_ov003_020cbd30(BtlVec2*, BtlVec2, BtlVec2, BtlVec2, s32);

s32   func_ov003_020cb200(BtlBinSet*, u16);
s32   func_ov003_020cb264(BtlBinSet*, u32);
void  func_ov003_020cb304(BtlBinSet*, u16);
s32   func_ov003_020cb32c(BtlBinSet*, s32);
u16   func_ov003_020cb348(BtlBinSet*, u16);
s32   func_ov003_020cb368(BinIdentifier*);
void  func_ov003_020cb520(CombatActor*, s32);
s32   func_ov003_020cb55c(Ov003EnemyActor*);
void  func_ov003_020cb578(CombatActor*, s32);
void  func_ov003_020cb5b0(Ov003EnemyActor*, s32, s32);
s32   func_ov003_020cb648(Ov003EnemyActor*, s32);
s32   func_ov003_020cb744(s32);
s32   func_ov003_020cb784(s32);
s32   func_ov003_020cb7a4(s32);
s32   func_ov003_020cbafc(s32*, s32*, s32*, s32, s32, s32, s32, s32, s32, s32);
void  func_ov003_020cbc50(s32*, s32*, u16, s32);
void  func_ov003_020cbcb4(s32*, s32*, u16, s32, s32);
s32   func_ov003_020cc300(Ov003EnemyActor*, s32);
s32   func_ov003_020cc354(CombatActor*);
s32   func_ov003_020cc38c(Ov003CutinActor*, s32, s32, s32, s32, s32, s32);
u32   func_ov003_020cc430(Ov003CutinActor*, s32, s32, s32);
s32   func_ov003_020cc5d0(CombatActor*);
s32   func_ov003_020cc5fc(s32, s32);
void* func_ov003_020cc750(s32);
void  func_ov003_020cc784(CombatActor*, s32);
s32   func_ov003_020cc7c0(s32, s32, s32);
s32   func_ov003_020cc7f4(s32, s32, s32, s32);
void  func_ov003_020ccea8(CombatActor*, CombatSprite*);
void  func_ov003_020ccec0(CombatActor*, s32);
s32   func_ov003_020ccedc(s32);
s32   func_ov003_020ccefc(s32);
void  func_ov003_020ccf1c(s32*, s32);
void  func_ov003_020ccf58(s32, Ov003EnemyActor*);
void  func_ov003_020ccf94(s32, CombatSprite*, const BtlSpriteDesc*);
s32   func_ov003_020ccfc8(Ov003EnemyActor*, u16);
s32   func_ov003_020ccfec(Ov003EnemyActor*);
s32   func_ov003_020cd010(Ov003EnemyActor*);
s32   func_ov003_020cd11c(s32);
// ov003_02087b7c.c
void                   func_ov003_02087ed8(SndMgrSeIdx);
void                   func_ov003_02087f28(SndMgrSeIdx, s32);
void                   func_ov003_02087f64(SndMgrSeIdx, s32);
void                   func_ov003_02087fe0(s32, CombatSprite*, s32);
void                   func_ov003_02088064(CombatSprite*, s32, s32);
void                   func_ov003_020880e4(s32, CombatSprite*, Ov003EnemyActor*);
void                   func_ov003_0208810c(CombatSprite*, CombatActor*);
s32                    func_ov003_02088130(void);
s32                    func_ov003_0208a08c(s32, Ov003EnemyActor*, s32);
const Ov003AttackInfo* func_ov003_0208a114(u16);
s32                    func_ov003_0208a128(const Ov003AttackInfo*, void*, void*, s32);
s32                    func_ov003_0208a164(const Ov003AttackInfo*, Ov003AttackArgs*, s32, s32, s32);
s32                    func_ov003_0208a1a4(const Ov003AttackInfo*, void*, s32, s32);
s32                    func_ov003_0208a1e8(const Ov003AttackInfo*, void*, s32);

// BtlAuraSub.c
void func_ov003_020974f8(s32, s32);

// BtlPop_Num.c
void func_ov003_0209b564(s32, s32, CombatActor*, s32);
void func_ov003_0209b61c(s32, s32, CombatActor*);

// BtlFeat_SplashEnm.c
void func_ov003_020bc690(u16, u16, s32);

// BtlFeat_SplashBg.c
void func_ov003_020bce18(s32, u16, u16, u16);

// ov003_020bce6c.c
void func_ov003_020bd150(s32, Ov003SnapshotCallback, void*);

// BtlEnmShot_Charge.c
s32 func_ov003_020cd988(Ov003EnemyShotChargeSplitArgs*);

// BtlEnmShot_RapidSlow.c
s32 func_ov003_020d0518(Ov003EnemyShotSource*, s32, u16, u16, u16, Ov003EnemyShotSource*, s32, u16, u16, u16, s16);

// BtlEnmShot_RootLaser.c
void func_ov003_020d1dc0(Ov003EnemyShotSource*, s32, u16, u16, u16);

// BtlEnmShot_Spiral.c
s32 func_ov003_020d23e8(Ov003EnemyShotSource*, u16, s32, s32, u16, s32);

// BtlEnmShot_Split.c
s32 func_ov003_020d2bc0(Ov003EnemyShotSplitArgs*);

// ov003_020d36fc.c
void func_ov003_020d3b6c(Ov003AnimLayer*, Data*, s16, u32, u32, u32);
void func_ov003_020d3bec(Ov003AnimLayer*, BinIdentifier*, s32, void**, const Ov003AnimFrame*);
void func_ov003_020d3c2c(Ov003AnimLayer*, const Ov003AnimFrame*);
void func_ov003_020d3c98(Ov003AnimLayer*, Ov003AnimLayer*);
void func_ov003_020d3cac(Ov003AnimLayer*);
void func_ov003_020d3cf8(Ov003AnimLayer*);

#endif // COMBAT_CORE_COMBATAPI_H
