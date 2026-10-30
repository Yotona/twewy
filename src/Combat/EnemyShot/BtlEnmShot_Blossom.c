#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include <nitro/fs/file.h>
#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

// The enemy shot toolbox and the `Blossom` shot (`Tsk_BtlEnmShot_Blossom`). The first half of this unit is a grab bag
// of helpers shared by every enemy shot and boss: a loader for sets of packed resources, queries over the enemy lists,
// camera clamps, fixed-point curves and the check that decides when a shot has left play. The Blossom task at the end
// flings a petal out along an arc and leaves a trail of blurred copies.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

typedef s32 (*BtlItemFilter)(s32 count, void* item, s32 arg);

/// A petal's task parameter.
typedef struct Ov003EnemyShotBlossomArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ u16                   angle;
    /* 0x06 */ u16                   reserved06;
    /* 0x08 */ s32                   speed;
    /* 0x0C */ s32                   curveSpeed;
    /* 0x10 */ s32                   yScale;
} Ov003EnemyShotBlossomArgs; // Size: 0x14

typedef struct Ov003EnemyShotBlossom {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ CombatSprite         sprite;
    /* 0x8C */ u16                  angle;
    /* 0x8E */ u16                  reserved8E;
    /* 0x90 */ s32                  velocityX;
    /* 0x94 */ s32                  velocityY;
    /* 0x98 */ s32                  decayX;
    /* 0x9C */ s32                  decayY;
    /* 0xA0 */ s32                  offsetX;
    /* 0xA4 */ s32                  offsetY;
    /* 0xA8 */ s32                  curveStepX;
    /* 0xAC */ s32                  curveStepY;
    /* 0xB0 */ s32                  curveX;
    /* 0xB4 */ s32                  curveY;
    /* 0xB8 */ s16                  frame;
    /* 0xBA */ s16                  curveFrames;
    /* 0xBC */ s16                  curvePhase;
    /* 0xBE */ u16                  reservedBE;
} Ov003EnemyShotBlossom; // Size: 0xC0

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern u8 data_ov003_020d4231[];

extern void func_ov003_02082a04(s32 engine, Sprite* sprite, const BinIdentifier* iden, const SpriteAnimEntry* table, u16 entry,
                                s32 palette, s32 paletteCount);
extern void func_ov003_02082a90(s32 engine, Sprite* sprite, const BinIdentifier* iden, u16 entry, u16 arg4, u16 arg5,
                                s32 palette, s32 anim);

s32 func_ov003_020cd1c8(Ov003EnemyShotBlossom* self, Ov003EnemyShotBlossomArgs* args);
s32 func_ov003_020cd44c(Ov003EnemyShotBlossom* self);
s32 func_ov003_020cd618(Ov003EnemyShotBlossom* self);
s32 func_ov003_020cd690(Ov003EnemyShotBlossom* self);
s32 func_ov003_020cd6a4(TaskPool* pool, Task* task, void* args, s32 stage);

char             data_ov003_020e6be8[0x18] = "Tsk_BtlEnmShot_Blossom";
const TaskHandle data_ov003_020e0c0c       = {data_ov003_020e6be8, func_ov003_020cd6a4, sizeof(Ov003EnemyShotBlossom)};

// ---------------------------------------------------------------------------------------------
// Resource sets
// ---------------------------------------------------------------------------------------------

void func_ov003_020cb128(s32* out, s32 a, s32 b) {
    out[0] = a;
    out[1] = b;
}

void func_ov003_020cb130(BtlBinSet* set, const BtlBinEntry* entries, u16 count, s32 size, const char* tag) {
    set->buffer  = NULL;
    set->entries = entries;
    set->count   = count;
    set->size    = size;
    set->tag     = tag;
}

s32 func_ov003_020cb150(BtlBinSet* set) {
    const char* tag    = set->tag;
    void*       buffer = Mem_AllocHeapTail(&gMainHeap, set->size);

    Mem_SetSequence(&gMainHeap, buffer, tag);
    set->buffer = buffer;
    return buffer != NULL;
}

void func_ov003_020cb194(BtlBinSet* set) {
    s32 i;

    for (i = 0; i < set->count; i++) {
        func_ov003_020cb304(set, (u16)i);
    }
    if (set->buffer != NULL) {
        Mem_Free(&gMainHeap, set->buffer);
    }
    set->buffer  = NULL;
    set->entries = NULL;
    set->count   = 0;
    set->size    = 0;
    set->tag     = NULL;
}

s32 func_ov003_020cb200(BtlBinSet* set, u16 index) {
    const BtlBinEntry* entries = set->entries;
    const BtlBinEntry* entry   = &entries[index];
    u32                size    = set->size - entry->offset;
    u8*                data    = (u8*)set->buffer + entry->offset;

    BinMgr_LoadRawData((Bin*)data, NULL, (BinIdentifier*)entries[index].id, 0, &size);
    BinMgr_CreateFromData(entries[index].id, data, size);
    return 1;
}

s32 func_ov003_020cb264(BtlBinSet* set, u32 id) {
    const BtlBinEntry* entry = NULL;
    s32                count = set->count;
    u32                size;
    u8*                data;
    s32                i = 0;

    if (count > 0) {
        const BtlBinEntry* cursor = set->entries;

        for (; i < count; i++, cursor++) {
            entry = cursor;
            if (id == cursor->id) {
                break;
            }
        }
    }
    if (i == count) {
        return 0;
    }
    size = set->size - entry->offset;
    data = (u8*)set->buffer + entry->offset;
    BinMgr_LoadRawData((Bin*)data, NULL, (BinIdentifier*)entry->id, 0, &size);
    BinMgr_CreateFromData(entry->id, data, size);
    return 1;
}

void func_ov003_020cb304(BtlBinSet* set, u16 index) {
    u32 id = set->entries[index].id;

    if (id == 0) {
        return;
    }
    {
        Bin* bin = BinMgr_FindById(id);

        if (bin == NULL) {
            return;
        }
        BinMgr_ReleaseBin(bin);
    }
}

s32 func_ov003_020cb32c(BtlBinSet* set, BinIdentifier* iden) {
    return BinMgr_FindById((s32)iden) != NULL;
}

s32 func_ov003_020cb348(BtlBinSet* set, u16 index) {
    Bin* bin = BinMgr_FindById(set->entries[index].id);

    if (bin != NULL) {
        return bin->refCount;
    }
    return 0;
}

/// The size of the file of `iden` on the card.
s32 func_ov003_020cb368(BinIdentifier* iden) {
    FS_FileIdentifier fileIden;
    FS_File           file;

    if (iden == NULL) {
        return 0;
    }
    FS_FilePathAsIden(&fileIden, iden->path);
    FS_FileInit(&file);
    FS_FileOpenFromIden(&file, fileIden);
    {
        s32 size = file.endPosition - file.startPosition;

        FS_FileClose(&file);
        return size;
    }
}

// ---------------------------------------------------------------------------------------------
// Queries over the enemy lists
// ---------------------------------------------------------------------------------------------

/// How many enemies of the screen `side` belong to `group` (0x3C counts all).
u16 func_ov003_020cb3c4(s32 side, s32 group) {
    u16             count = 0;
    CombatListNode* node;

    if (side == 1) {
        node = data_ov003_020e71b8->unk3D8AC.head;
    } else {
        node = data_ov003_020e71b8->unk3D8A4.head;
    }
    if (node == NULL) {
        return count;
    }
    do {
        Ov003CutinActor* actor = node->item;

        node = node->next;
        if (group == 0x3C || actor->base.group == group) {
            count++;
        }
    } while (node != NULL);
    return count;
}

/// The `n`-th enemy of the screen `side` that belongs to `group`.
Ov003EnemyActor* func_ov003_020cb42c(s32 side, s32 group, s32 n) {
    Ov003EnemyActor* actor;
    CombatListNode*  node;

    if (side == 1) {
        node = data_ov003_020e71b8->unk3D8AC.head;
    } else {
        node = data_ov003_020e71b8->unk3D8A4.head;
    }
    n++;
    if (n == 0) {
        return NULL;
    }
    do {
        if (node == NULL) {
            return NULL;
        }
        actor = node->item;
        node  = node->next;
        if (group == 0x3C || actor->group == group) {
            n--;
        }
    } while (n != 0);
    return actor;
}

/// How many enemies of `group` on the screen `side` pass `filter`.
s32 func_ov003_020cb498(s32 side, s32 group, BtlActorFilter filter, void* arg) {
    s32             count = 0;
    CombatListNode* node;

    if (side == 1) {
        node = data_ov003_020e71b8->unk3D8AC.head;
    } else {
        node = data_ov003_020e71b8->unk3D8A4.head;
    }
    if (node == NULL) {
        return count;
    }
    do {
        Ov003EnemyActor* actor = node->item;

        if (group == 0x3C || actor->group == group) {
            if (filter(count, actor, arg) != 0) {
                count++;
            }
        }
        node = node->next;
    } while (node != NULL);
    return count;
}

/// Reads the gravity of a fresh actor (what the actor falls with when it is thrown).
void func_ov003_020cb520(CombatActor* actor, s32 flag) {
    CombatActor fresh;

    if (flag == 0) {
        actor->zGravity = 0;
        return;
    }
    CombatActor_Init(&fresh, 1);
    actor->zGravity = fresh.zGravity;
}

s32 func_ov003_020cb55c(Ov003EnemyActor* actor) {
    return actor->sprite.sprite.bits_0_1 == 1;
}

void func_ov003_020cb578(CombatActor* actor, s32 flag) {
    if (flag != 0) {
        actor->flags &= ~0x20;
        actor->flags &= ~0x10;
    } else {
        actor->flags |= 0x30;
    }
}

void func_ov003_020cb594(Ov003EnemyActor* actor, s32 flag) {
    func_ov003_02084694(&actor->collider, flag == 0);
}

/// Heals `actor` and its partner by `amount` (or just plays the effect for `flag`).
void func_ov003_020cb5b0(Ov003EnemyActor* actor, s16 amount, s32 flag) {
    s32 engine = func_ov003_020c37f8(&actor->sprite) ? 1 : 0;

    func_ov003_0209b564(engine, amount, &actor->actor, 1);
    if (flag != 0) {
        func_ov003_0208b518(engine, actor, amount);
        return;
    }
    actor->actor.currentHp = actor->actor.currentHp + amount;
    if (actor->actor.currentHp > actor->actor.maxHp) {
        actor->actor.currentHp = actor->actor.maxHp;
    }
    actor->partner->actor.currentHp = actor->partner->actor.currentHp + amount;
    if (actor->partner->actor.currentHp > actor->partner->actor.maxHp) {
        actor->partner->actor.currentHp = actor->partner->actor.maxHp;
    }
}

/// Whether the actor's HP percentage is at most `percent`.
s32 func_ov003_020cb648(Ov003EnemyActor* actor, s32 percent) {
    return actor->actor.currentHp * 100 / actor->actor.maxHp <= percent;
}

// ---------------------------------------------------------------------------------------------
// Camera
// ---------------------------------------------------------------------------------------------

/// Pushes `*value` past the camera's left edge shifted by the view angle.
s32 func_ov003_020cb674(s32 index, s32* value, s32 margin, s32 scale) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];
    s32          limit  = camera->posX + FX_Mul(FX_SinIdx(camera->rotation), scale);

    if (*value - margin < limit) {
        *value = limit + margin;
        return 1;
    }
    return 0;
}

s32 func_ov003_020cb6e8(s32 index, s32* value, s32 margin, s32 scale) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];
    s32          moved  = 0;

    if (func_ov003_020cb674(index, value, margin, scale) != 0) {
        moved = 1;
    }
    if (*value + margin > camera->unk_10) {
        *value = camera->unk_10 - margin;
        moved  = 1;
    }
    return moved;
}

s32 func_ov003_020cb744(s32 index) {
    return data_ov003_020e71b8->unk3D7C0[index].unk_0C;
}

s32 func_ov003_020cb764(s32 index) {
    return data_ov003_020e71b8->unk3D7C0[index].unk_20;
}

s32 func_ov003_020cb784(s32 index) {
    return data_ov003_020e71b8->unk3D7C0[index].unk_24;
}

s32 func_ov003_020cb7a4(s32 index) {
    return data_ov003_020e71b8->unk3D7C0[index].unk_10;
}

/// Whether (`x`, `y`) lies inside the camera's field.
s32 func_ov003_020cb7c4(s32 index, s32 x, s32 y) {
    if (x < 0 || x > data_ov003_020e71b8->unk3D7C0[index].unk_0C || y < 0 || y > data_ov003_020e71b8->unk3D7C0[index].unk_10) {
        return 0;
    }
    return 1;
}

/// Keeps (`*x`, `*y`) at least a margin inside the camera's field. Returns whether it had to move them.
s32 func_ov003_020cb814(s32 index, s32* x, s32* y, s32 marginX, s32 marginY) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];
    s32          moved  = 0;

    if (*x - marginX < 0) {
        *x    = marginX;
        moved = 1;
    } else if (*x + marginX > camera->unk_0C) {
        *x    = camera->unk_0C - marginX;
        moved = 1;
    }
    if (func_ov003_020cb6e8(index, y, marginY, *x) != 0) {
        moved = 1;
    }
    return moved;
}

/// Index of the weight that `target` falls into.
s16 func_ov003_020cb888(u16* weights, s32 count, s32 target) {
    s16 i    = 0;
    s32 last = count - 1;

    target++;
    while (last > 0) {
        target -= weights[i];
        if (target <= 0) {
            break;
        }
        i++;
        if (!(last > i)) {
            break;
        }
    }
    return i;
}

// ---------------------------------------------------------------------------------------------
// Fixed-point geometry
// ---------------------------------------------------------------------------------------------

void func_ov003_020cb8cc(s32* vx, s32* vy, s32 x0, s32 y0, s32 x1, s32 y1, s32 steps) {
    s32 vz;

    func_ov003_020cb910(vx, vy, &vz, x0, y0, 0, x1, y1, 0, steps);
}

/// Velocity that carries (`x0`,`y0`,`z0`) to (`x1`,`y1`,`z1`) at `speed`, rounded to whole frames. Returns the frame count.
s32 func_ov003_020cb910(s32* vx, s32* vy, s32* vz, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1, s32 speed) {
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 dz = z1 - z0;
    s32 frames;
    s32 whole;
    s32 length = func_ov003_020cba54(0, 0, 0, dx, dy, dz);

    if (length == 0) {
        *vz = 0;
        *vy = 0;
        *vx = 0;
        return 0;
    }
    frames = (FX_Divide(length, speed) * 0x10) >> 16;
    whole  = FX32_CONVERT(frames);
    if (whole != 0) {
        *vx = FX_Divide(dx, whole);
        *vy = FX_Divide(dy, whole);
        *vz = FX_Divide(dz, whole);
    } else {
        *vz = 0;
        *vy = 0;
        *vx = 0;
    }
    return frames;
}

/// The angle from (`x0`,`y0`) to (`x1`,`y1`).
s32 func_ov003_020cba14(s32 x0, s32 y0, s32 x1, s32 y1) {
    return FX_Atan2Idx(y1 - y0, x1 - x0);
}

s32 func_ov003_020cba2c(s32 x0, s32 y0, s32 x1, s32 y1) {
    return func_ov003_020cba54(x0, y0, 0, x1, y1, 0);
}

/// The distance between two points.
s32 func_ov003_020cba54(s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1) {
    s32 dy = y1 - y0;
    s32 dx = x1 - x0;
    s32 dz = z1 - z0;

    return FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy) + FX_Mul(dz, dz));
}

void func_ov003_020cbab8(s32* vx, s32* vy, s32 x0, s32 y0, s32 x1, s32 y1, s32 speed) {
    s32 vz;

    func_ov003_020cbafc(vx, vy, &vz, x0, y0, 0, x1, y1, 0, speed);
}

/// Velocity of length `speed` that points from the first point to the second.
s32 func_ov003_020cbafc(s32* vx, s32* vy, s32* vz, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1, s32 speed) {
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 dz = z1 - z0;
    s32 length;

    length = func_ov003_020cba54(0, 0, 0, dx, dy, dz);
    if (length == 0) {
        *vz = 0;
        *vy = 0;
        *vx = 0;
        return 0;
    }
    *vx = FX_Divide(FX_Mul(dx, speed), length);
    *vy = FX_Divide(FX_Mul(dy, speed), length);
    *vz = FX_Divide(FX_Mul(dz, speed), length);
    return length;
}

void func_ov003_020cbc10(s32* mx, s32* my, s32* mz, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1);

void func_ov003_020cbbd4(s32* mx, s32* my, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 mz;

    func_ov003_020cbc10(mx, my, &mz, x0, y0, 0, x1, y1, 0);
}

/// The middle of two points.
void func_ov003_020cbc10(s32* mx, s32* my, s32* mz, s32 x0, s32 y0, s32 z0, s32 x1, s32 y1, s32 z1) {
    *mx = x0 + ((x1 - x0) >> 1);
    *my = y0 + ((y1 - y0) >> 1);
    *mz = z0 + ((z1 - z0) >> 1);
}

/// (cos, sin) of `angle` times `scale`.
void func_ov003_020cbc50(s32* outCos, s32* outSin, u16 angle, s32 scale) {
    *outCos = FX_Mul(FX_CosIdx(angle), scale);
    *outSin = FX_Mul(FX_SinIdx(angle), scale);
}

/// The same on an ellipse that is `yScale` as tall.
void func_ov003_020cbcb4(s32* outX, s32* outY, u16 angle, s32 scale, s32 yScale) {
    s32 cosv = FX_Mul(FX_CosIdx(angle), scale);
    s32 sinv = FX_Mul(FX_SinIdx(angle), scale);

    *outX = cosv;
    *outY = FX_Mul(sinv, yScale);
}

/// Curve evaluation: quadratic Bezier through three 3D points.
void func_ov003_020cbdd8(Vec* out, Vec p0, Vec p1, Vec p2, s32 t) {
    s32 u = 0x1000 - t;
    s32 a = FX_Mul(u, t);
    s32 b = FX_Mul(u, u);
    s32 c = FX_Mul(t, t);

    out->x = FX_Mul(p0.x, b) + FX_Mul(p1.x, a) * 2 + FX_Mul(p2.x, c);
    out->y = FX_Mul(p0.y, b) + FX_Mul(p1.y, a) * 2 + FX_Mul(p2.y, c);
    out->z = FX_Mul(p0.z, b) + FX_Mul(p1.z, a) * 2 + FX_Mul(p2.z, c);
}

/// The same in 2D.
void func_ov003_020cbd30(BtlVec2* out, BtlVec2 p0, BtlVec2 p1, BtlVec2 p2, s32 t) {
    Vec result;
    Vec a;
    Vec b;
    Vec c;

    c.x = p0.x;
    c.y = p0.y;
    c.z = 0;
    a.x = p1.x;
    a.y = p1.y;
    a.z = 0;
    b.x = p2.x;
    b.y = p2.y;
    b.z = 0;
    func_ov003_020cbdd8(&result, c, a, b, t);
    out->x = result.x;
    out->y = result.y;
}

/// Cubic Bezier through four 3D points.
void func_ov003_020cc020(Vec* out, Vec p0, Vec p1, Vec p2, Vec p3, s32 t) {
    s32 u  = 0x1000 - t;
    s32 uu = FX_Mul(u, u);
    s32 tt = FX_Mul(t, t);
    s32 w0 = FX_Mul(uu, u);
    s32 w1 = FX_Mul(uu, t);
    s32 w2 = FX_Mul(u, tt);
    s32 w3 = FX_Mul(tt, t);

    out->x = (FX_Mul(p1.x, w1) + FX_Mul(p2.x, w2)) * 3 + FX_Mul(p0.x, w0) + FX_Mul(p3.x, w3);
    out->y = (FX_Mul(p1.y, w1) + FX_Mul(p2.y, w2)) * 3 + FX_Mul(p0.y, w0) + FX_Mul(p3.y, w3);
    out->z = (FX_Mul(p1.z, w1) + FX_Mul(p2.z, w2)) * 3 + FX_Mul(p0.z, w0) + FX_Mul(p3.z, w3);
}

/// The same in 2D.
void func_ov003_020cbf54(BtlVec2* out, BtlVec2 p0, BtlVec2 p1, BtlVec2 p2, BtlVec2 p3, s32 t) {
    Vec result;
    Vec a;
    Vec b;
    Vec c;
    Vec d;

    a.x = p0.x;
    a.y = p0.y;
    a.z = 0;
    b.x = p1.x;
    b.y = p1.y;
    b.z = 0;
    c.x = p2.x;
    c.y = p2.y;
    c.z = 0;
    d.x = p3.x;
    d.y = p3.y;
    d.z = 0;
    func_ov003_020cc020(&result, a, b, c, d, t);
    out->x = result.x;
    out->y = result.y;
}

// ---------------------------------------------------------------------------------------------
// Actor tests
// ---------------------------------------------------------------------------------------------

/// Whether the actor is on the left half of the camera.
s32 func_ov003_020cc2d0(CombatActor* actor, s32 index) {
    return actor->position.x < (data_ov003_020e71b8->unk3D7C0[index].unk_0C >> 1);
}

/// Whether the actor sits fully inside the camera's field.
s32 func_ov003_020cc300(Ov003EnemyActor* actor, s32 index) {
    s32 x      = actor->actor.position.x;
    s32 y      = actor->actor.position.y;
    s16 radius = actor->actor.unk_70;

    return func_ov003_020cb814(index, &x, &y, radius << 12, (radius / 2) << 12) == 0;
}

/// Whether the actor is busy (hit, down or flagged).
s32 func_ov003_020cc354(CombatActor* actor) {
    if (actor->unk_5A > 0 || (actor->flags & 0x100) || (actor->flags & 4) || (actor->flags & 0x10)) {
        return 1;
    }
    return 0;
}

/// Whether the actor's box overlaps the box around (`x`,`y`,`z`).
s32 func_ov003_020cc38c(Ov003CutinActor* actor, s32 x, s32 y, s32 z, s32 marginX, s32 marginY, s32 marginZ) {
    s16 width  = actor->base.actor.unk_70;
    s16 height = actor->base.actor.unk_72;
    s32 half   = width / 2;

    if (actor->base.actor.position.x - (width << 12) > x + marginX) {
        return 0;
    }
    if (actor->base.actor.position.x + (width << 12) < x - marginX) {
        return 0;
    }
    if (actor->base.actor.position.y - (half << 12) > y + marginY) {
        return 0;
    }
    if (actor->base.actor.position.y + (half << 12) < y - marginY) {
        return 0;
    }
    if (actor->base.actor.position.z < z - marginZ) {
        return 0;
    }
    return actor->base.actor.position.z - (height << 12) <= z + marginZ;
}

/// A mask of the 32 points around the actor (at `radius`, `yScale` as tall) that are inside the camera's field.
u32 func_ov003_020cc430(Ov003CutinActor* actor, s32 index, s32 radius, s32 yScale) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];
    s16          size   = actor->base.actor.unk_70;
    s32          left   = FX32_CONVERT(size);
    s32          right  = camera->unk_0C - FX32_CONVERT(size);
    s32          top    = FX32_CONVERT(size) / 2;
    s32          bottom = camera->unk_10 - FX32_CONVERT(size) / 2;
    u32          mask   = 0;
    s32          i;
    s32          px;
    s32          py;

    for (i = 0; i < 0x20; i++) {
        func_ov003_020cbcb4(&px, &py, (u16)(i * 0x800), radius, yScale);
        px += actor->base.actor.position.x;
        py += actor->base.actor.position.y;
        if (px >= left && px <= right && py >= top / 2 && py <= bottom) {
            mask |= 1 << i;
        }
    }
    return mask;
}

/// How many frames until `z` becomes non-negative, falling with the actor's gravity.
s32 func_ov003_020cc5d0(CombatActor* actor) {
    s32 gravity  = actor->zGravity;
    s32 velocity = actor->zVelocity;
    s32 z        = actor->position.z;
    s32 frames   = 0;

    if (gravity <= 0) {
        return 0;
    }
    do {
        z += velocity;
        velocity += gravity;
        frames++;
    } while (z < 0);
    return frames;
}

/// How many entries of `id` the touch list holds (0x19 counts all).
s32 func_ov003_020cc5fc(s32 unused, s32 id) {
    s32             count = 0;
    CombatListNode* node  = data_ov003_020e71b8->unk3D8B4.head;

    if (node == NULL) {
        return count;
    }
    do {
        u16* item = node->item;

        node = node->next;
        if (id == 0x19 || item[1] == id) {
            count++;
        }
    } while (node != NULL);
    return count;
}

/// The `n`-th entry of `id` in the touch list (nothing on the sub screen).
void* func_ov003_020cc644(s32 side, s32 id, s32 n) {
    void*           item = NULL;
    CombatListNode* node;

    if (side == 1) {
        return item;
    }
    node = data_ov003_020e71b8->unk3D8B4.head;
    if (node == NULL) {
        return item;
    }
    n++;
    if (n == 0) {
        return item;
    }
    do {
        u16* entry;

        if (node == NULL) {
            return NULL;
        }
        item  = node->item;
        entry = item;
        node  = node->next;
        if (id == 0x19 || entry[1] == id) {
            n--;
        }
    } while (n != 0);
    return item;
}

/// How many entries of `id` in the touch list pass `filter`.
s32 func_ov003_020cc6a4(s32 unused, s32 id, BtlItemFilter filter, s32 arg) {
    CombatListNode* node  = data_ov003_020e71b8->unk3D8B4.head;
    s32             count = 0;

    if (node == NULL) {
        return count;
    }
    do {
        void* item  = node->item;
        u16*  entry = item;

        if (id == 0x19 || entry[1] == id) {
            if (filter(count, item, arg) != 0) {
                count++;
            }
        }
        node = node->next;
    } while (node != NULL);
    return count;
}

void func_ov003_020cc71c(CombatActor* actor, s32 flag) {
    CombatActor fresh;
    s32         value = 0;

    if (flag != 0) {
        func_ov003_020864dc(&fresh, ((u16*)actor)[1]);
        value = fresh.zGravity;
    }
    actor->xVelocity = value;
}

/// The gauge actor of the screen `side`.
void* func_ov003_020cc750(s32 side) {
    if (side == 1) {
        return data_ov003_020e71b8->unk3D89C;
    }
    return data_ov003_020e71b8->unk3D898;
}

void func_ov003_020cc784(CombatActor* actor, s32 flag) {
    CombatActor fresh;

    if (flag == 0) {
        actor->zGravity = 0;
        return;
    }
    CombatActor_Init(&fresh, 0);
    actor->zGravity = fresh.zGravity;
}

/// Whether (`x`,`y`) is on the screen, `margin` pixels of slack.
s32 func_ov003_020cc7c0(s32 x, s32 y, s32 margin) {
    if (x < -margin || y < -margin || x > margin + 0x100 || y > margin + 0xC0) {
        return 0;
    }
    return 1;
}

/// Whether (`x`,`y`) is on the screen for a box of `width` by `height`.
s32 func_ov003_020cc7f4(s32 x, s32 y, s32 width, s32 height) {
    s32 halfW = width >> 1;

    if (x < -halfW || y < -(height >> 1) || x > halfW + 0x100 || y > (height >> 1) + 0xC0) {
        return 0;
    }
    return 1;
}

/// Starts a shot source: `owner` fires the visual `visual` with the attack `attackId`.
void func_ov003_020cc834(Ov003EnemyShotSource* source, Ov003CutinActor* owner, const void* visual, u16 attackId) {
    MI_CpuSet(source, 0, 0x2C);
    source->core.owner      = owner;
    source->visual          = visual;
    source->core.attackId   = attackId;
    source->animation       = 0;
    source->animationMode   = 0;
    source->core.position.x = owner->base.actor.position.x;
    source->core.position.y = owner->base.actor.position.y;
    source->core.position.z = owner->base.actor.position.z;
    source->core.timer      = 0;
    ((s8*)&source->kind)[0] = 0;
    ((s8*)&source->kind)[1] = 0;
    source->core.sprite     = NULL;
    source->sound           = 0x56C;
    source->core.flags |= 1;
}

/// Copies the core of a shot source.
void func_ov003_020cc8b0(Ov003EnemyShotCore* dst, const Ov003EnemyShotSource* src) {
    dst->owner      = src->core.owner;
    dst->sprite     = src->core.sprite;
    dst->hitResult  = src->core.hitResult;
    dst->attackId   = (s16)src->core.attackId;
    dst->position.x = src->core.position.x;
    dst->position.y = src->core.position.y;
    dst->position.z = src->core.position.z;
    dst->timer      = (s16)src->core.timer;
    dst->flags      = src->core.flags;
}

/// Whether a shot is over: it left the field, ran out of time, finished its animation, hit something, or its owner
/// went down, depending on the `flags` of the shot.
s32 func_ov003_020cc8fc(u16 flags, s32 engine, s32 x, s32 y, s32 z, s32 marginX, s32 marginY, Ov003CutinActor* owner,
                        CombatSprite* sprite, s32 hitResult, s16* timer) {
    if (func_ov003_020c3c28() != 0) {
        return 1;
    }
    if (func_ov003_020cce34(engine) == 0) {
        return 1;
    }
    if ((flags & 1) || (flags & 0x20)) {
        s32 screen = func_ov003_020c37f8(sprite) ? 1 : 0;

        if (z > 0) {
            return 1;
        }
        if (func_ov003_020ccd50(screen, x, y, FX32_CONVERT(marginX), FX32_CONVERT(marginY)) != 0) {
            if (func_ov003_020ccdb8(screen, x, y, z, FX32_CONVERT(marginX), FX32_CONVERT(marginY)) != 0) {
                return 1;
            }
        }
    }
    if (flags & 0x40) {
        if (timer != NULL) {
            (*timer)--;
        }
        if (*timer < 0) {
            return 1;
        }
    } else {
        if (timer != NULL) {
            (*timer)++;
        }
    }
    if (flags & 2) {
        if (SpriteMgr_IsAnimationFinished(&sprite->sprite) != 0) {
            return 1;
        }
    }
    if ((flags & 4) && hitResult != 0) {
        return 1;
    }
    if (flags & 8) {
        if ((owner->base.actor.flags & 2) || (owner->base.actor.flags & 0x4000) || (owner->base.actor.flags & 0x200)) {
            return 1;
        }
    }
    if (flags & 0x10) {
        if (owner->base.actor.flags & 4) {
            return 1;
        }
    }
    return 0;
}

s32 func_ov003_020ccb9c(Ov003EnemyShotSource* source, CombatSprite* sprite) {
    return func_ov003_020cc8fc(source->core.flags, sprite->sprite.bits_0_1, source->core.position.x, source->core.position.y,
                               source->core.position.z, ((s8*)&source->kind)[0], ((s8*)&source->kind)[1], source->core.owner,
                               sprite, source->core.hitResult, &source->core.timer);
}

s32 func_ov003_020ccc00(Ov003EnemyShotCore* core, CombatSprite* sprite) {
    return func_ov003_020cc8fc(core->flags, sprite->sprite.bits_0_1, core->position.x, core->position.y, core->position.z, 8,
                               8, core->owner, sprite, core->hitResult, &core->timer);
}

/// The sprite of the shot task `id` running on screen `side`.
s32 func_ov003_020ccc60(s32 side, s32 id) {
    s32* data;

    if (id == -1) {
        return 0;
    }
    data = EasyTask_GetTaskData(side == 0 ? &data_ov003_020e71b8->unk_00000 : &data_ov003_020e71b8->taskPool, id);
    return data[1];
}

/// The hit result of the shot task `id` running on screen `side`.
s32 func_ov003_020ccca4(s32 side, u32 id) {
    s8* data;

    if (id == -1) {
        return 0;
    }
    data = EasyTask_GetTaskData(side == 0 ? &data_ov003_020e71b8->unk_00000 : &data_ov003_020e71b8->taskPool, id);
    if (data == NULL) {
        return 0;
    }
    return data[8];
}

/// Plays the sound of a shot source unless it is already playing.
void func_ov003_020cccf0(s32 engine, Ov003EnemyShotSource* source) {
    s32 pan;

    if (SndMgr_IsSEPlaying(source->sound) != 0) {
        return;
    }
    if (source->sound == 0x56C) {
        return;
    }
    if (engine == 1) {
        pan = func_ov003_020843b0(1, source->core.position.x);
    } else {
        pan = func_ov003_020843b0(0, source->core.position.x);
    }
    func_ov003_02087f00(source->sound, pan);
}

/// Whether a box of `marginX` by `marginY` around (`x`,`y`) lies outside the camera's field.
s32 func_ov003_020ccd50(s32 index, s32 x, s32 y, s32 marginX, s32 marginY) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];

    if (x + marginX < 0) {
        return 1;
    }
    if (x - marginX > camera->unk_0C) {
        return 1;
    }
    if (y + marginY < 0) {
        return 1;
    }
    return y - marginY > camera->unk_10;
}

/// Whether a box around the world point lies outside the screen.
s32 func_ov003_020ccdb8(s32 engine, s32 x, s32 y, s32 z, s32 marginX, s32 marginY) {
    s16 screenX;
    s16 screenY;

    func_ov003_02084348(engine, &screenX, &screenY, x, y, z);
    if (screenX + (marginX >> 12) < 0) {
        return 1;
    }
    if (screenX - (marginX >> 12) > 0x100) {
        return 1;
    }
    if (screenY + (marginY >> 12) < 0) {
        return 1;
    }
    return screenY - (marginY >> 12) > 0xC0;
}

/// Whether the screen `side` still has a live enemy.
s32 func_ov003_020cce34(s32 side) {
    Ov003CutinActor* actor;

    if (side == 0) {
        actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    } else {
        actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    }
    if (actor != NULL) {
        do {
            if (actor->base.group < 0x3C && actor->base.group >= 0) {
                return 1;
            }
            actor = func_ov003_02084984(&actor->base.node);
        } while (actor != NULL);
    }
    return 0;
}

void func_ov003_020ccea8(CombatActor* actor, CombatSprite* sprite) {
    actor->isFlipped = (sprite->flags46 & 1) ? 1 : 0;
}

void func_ov003_020ccec0(CombatActor* actor, s32 flag) {
    if (flag != 0) {
        actor->flags |= 0x30;
    } else {
        actor->flags &= ~0x20;
        actor->flags &= ~0x10;
    }
}

s32 func_ov003_020ccedc(s32 index) {
    return data_ov003_020e71b8->unk3D7C0[index].unk_0C;
}

s32 func_ov003_020ccefc(s32 index) {
    return data_ov003_020e71b8->unk3D7C0[index].unk_10;
}

/// Brings `*value` toward 0 by `step` and stops at 0.
void func_ov003_020ccf1c(s32* value, s32 step) {
    if (*value > 0) {
        *value += step;
        if (*value < 0) {
            *value = 0;
        }
    } else if (*value < 0) {
        *value -= step;
        if (*value > 0) {
            *value = 0;
        }
    }
}

void func_ov003_020ccf58(s32 side, Ov003EnemyActor* actor) {
    if (side == 0) {
        func_ov003_020c4520(actor);
        func_ov003_020c4b5c(actor);
    } else {
        func_ov003_020c44ac(actor);
        func_ov003_020c4b1c(actor);
        func_ov003_020c3e64(actor);
    }
}

/// Loads a sprite from a descriptor.
void func_ov003_020ccf94(s32 engine, CombatSprite* sprite, const BtlSpriteDesc* desc) {
    func_ov003_02082a04(engine, &sprite->sprite, desc->iden, desc->table, desc->entry, desc->palette, desc->paletteCount);
}

s32 func_ov003_020ccfc8(Ov003EnemyActor* actor, u16 anim) {
    if (actor->sprite.sprite.bits_0_1 == 0) {
        return func_ov003_020c65cc(actor, anim);
    } else {
        return func_ov003_020c62c4(actor, anim);
    }
}

s32 func_ov003_020ccfec(Ov003EnemyActor* actor) {
    if (actor->sprite.sprite.bits_0_1 == 0) {
        return func_ov003_020c7070(actor);
    } else {
        return func_ov003_020c703c(actor);
    }
}

/// One frame of an enemy's defeat sequence. Returns 0 once it is over.
s32 func_ov003_020cd010(Ov003EnemyActor* actor) {
    if (actor->unk_1A4 == 0) {
        if (actor->sprite.sprite.bits_0_1 == 0) {
            func_ov003_020c4f00(actor);
        } else {
            actor->flags18C |= 0x40;
            actor->flags18C &= ~0x20;
            func_ov003_020c4ee0(actor);
            func_ov003_020c4a34(actor);
            if (data_ov003_020e71b8->unk3D8A0 == actor) {
                data_ov003_020e71b8->unk3D8A0 = NULL;
            }
        }
        actor->actor.flags |= 4;
        if (!(actor->flags18C & 0x1000)) {
            s32 hasPartner = (actor->actor.flags & 0x2000) && actor->partner != NULL ? 1 : 0;

            func_ov003_0209f444(actor->sprite.sprite.bits_0_1, actor->actor.position.x, actor->actor.position.y,
                                actor->actor.position.z, hasPartner);
            actor->sprite.flags46 |= 2;
        }
    }
    if (actor->unk_1A4 == 1) {
        actor->flags18C |= 1;
    }
    if (actor->unk_1A4 == 0x3C) {
        return 0;
    }
    actor->unk_1A4++;
    return 1;
}

s32 func_ov003_020cd11c(s32 index) {
    return data_ov003_020d4231[index * 12];
}

// ---------------------------------------------------------------------------------------------
// Blossom
// ---------------------------------------------------------------------------------------------

/// Starts a petal.
s32 func_ov003_020cd134(Ov003EnemyShotSource* source, u16 angle, s32 speed, s32 curveSpeed, s32 yScale) {
    Ov003EnemyShotBlossomArgs args;
    TaskPool*                 pool;

    MI_CpuSet(&args, 0, sizeof(args));
    args.source     = source;
    args.angle      = angle;
    args.speed      = speed;
    args.curveSpeed = curveSpeed;
    args.yScale     = yScale;
    if (source->core.owner->base.sprite.sprite.bits_0_1 == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    return EasyTask_CreateTask(pool, &data_ov003_020e0c0c, NULL, 0, NULL, &args);
}

s32 func_ov003_020cd1c8(Ov003EnemyShotBlossom* self, Ov003EnemyShotBlossomArgs* args) {
    const Ov003EnemyShotTableVisual* visual = args->source->tableVisual;
    const SpriteAnimEntry*           entry  = &visual->base.entries[visual->base.entry];
    s32                              engine = args->source->core.owner->base.sprite.sprite.bits_0_1;

    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, self, sizeof(Ov003EnemyShotSource));
    self->angle       = args->angle;
    self->velocityX   = FX_Mul(FX_CosIdx(args->angle), args->speed);
    self->velocityY   = FX_Mul(FX_SinIdx(args->angle), args->speed);
    self->decayX      = FX_Mul(FX_CosIdx(args->angle), args->curveSpeed);
    self->decayY      = FX_Mul(FX_SinIdx(args->angle), args->curveSpeed);
    self->curveFrames = args->speed / args->curveSpeed;
    self->curvePhase  = 0x10000 / self->curveFrames;
    {
        u16 side  = (u16)(args->angle + 0x4000);
        s32 big   = FX_Mul(args->curveSpeed, args->yScale);
        s32 small = FX_Mul(args->speed, args->yScale);

        self->curveStepX = FX_Mul(FX_CosIdx(side), small);
        self->curveStepY = FX_Mul(FX_SinIdx(side), small);
        self->curveX     = FX_Mul(FX_CosIdx(side), big);
        self->curveY     = FX_Mul(FX_SinIdx(side), big);
    }
    func_ov003_02082a90(engine, &self->sprite.sprite, visual->base.file, entry->charDataIndex, entry->paletteDataIndex,
                        entry->frameDataIndex, visual->base.paletteSlot, entry->animDataIndex);
    CombatSprite_SetAnim(&self->sprite, self->source.animation, self->source.animationMode);
    self->source.core.sprite = &self->sprite;
    func_ov003_020cdfbc(&self->source, args->source->core.owner->base.sprite.flags46 & 1, 0);
    func_ov003_020cccf0(engine, args->source);
    return 1;
}

s32 func_ov003_020cd44c(Ov003EnemyShotBlossom* self) {
    s16 sx;
    s16 sy;

    if (func_ov003_020ccb9c(&self->source, &self->sprite) != 0) {
        return 0;
    }
    func_ov003_020cdfbc(&self->source, 0, 1);
    self->source.core.position.x = self->source.core.position.x + (self->velocityX + self->offsetX);
    self->source.core.position.y = self->source.core.position.y + (self->velocityY + self->offsetY);
    self->velocityX -= self->decayX;
    self->velocityY -= self->decayY;
    if (self->frame < self->curveFrames * 2) {
        s32 wave = FX_SinCosTable_[(u16)((self->curvePhase >> 1) * self->frame) / 16 * 2 + 1];

        self->offsetX = FX_Mul(wave, self->curveX);
        self->offsetY = FX_Mul(wave, self->curveY);
    } else {
        self->offsetX += self->curveStepX;
        self->offsetY += self->curveStepY;
    }
    if (self->frame % 5 == 0) {
        func_ov003_020cd6f4(&self->source, 0xC00, 0x180);
    }
    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &sy, &sx, self->source.core.position.x,
                        self->source.core.position.y, self->source.core.position.z);
    self->source.core.hitResult = func_ov003_0208a1a4(func_ov003_0208a114(self->source.core.attackId),
                                                      &self->source.core.owner->base.actor.baseArgs, sx, sy);
    self->frame++;
    CombatSprite_Update(&self->sprite);
    return 1;
}

s32 func_ov003_020cd618(Ov003EnemyShotBlossom* self) {
    s16 x;
    s16 y;

    func_ov003_02084348(func_ov003_020c37f8(&self->sprite) ? 1 : 0, &x, &y, self->source.core.position.x,
                        self->source.core.position.y, self->source.core.position.z);
    CombatSprite_SetPosition(&self->sprite, y, x);
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - self->source.core.position.y);
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020cd690(Ov003EnemyShotBlossom* self) {
    CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020cd6a4(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotBlossom* self = task->data;

    switch (stage) {
        case 0:
            return func_ov003_020cd1c8(self, args);
        case 1:
            return func_ov003_020cd44c(self);
        case 2:
            return func_ov003_020cd618(self);
        case 3:
            return func_ov003_020cd690(self);
        default:
            return 1;
    }
}
