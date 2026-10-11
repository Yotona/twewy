#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/TaskHelpers.h"
#include "SndMgr.h"
#include <nitro/fx.h>

// USA 0x020cb284-0x020ccb78: the "Shinigami" (reaper wall) collision helpers
// and the reaper menu task. Positions are 2D fx32 (12-bit fraction). The map
// in data_ov030_02109d84 holds a grid of cells, each listing indices of wall
// triangles (3 vertices + category/attr); wall containment classifies points as
// inside (1), touching (-1) or free (0), and the slide helpers steer movement
// along wall edges.

typedef struct {
    /* 0x0 */ u16 unk_00;
    /* 0x2 */ u16 unk_02;
    /* 0x4 */ u16 width;
    /* 0x6 */ u16 height;
    /* 0x8 */ u16 cellWidth;
    /* 0xA */ u16 cellHeight;
} ShiniGridInfo; // Size: 0xC

typedef struct {
    /* 0x0 */ s32 originX;
    /* 0x4 */ s32 originY;
    /* 0x8 */ u32 count;
    /* 0xC */ s32 offset;
} ShiniCell; // Size: 0x10

typedef struct {
    /* 0x00 */ FieldPos v[3];
    /* 0x18 */ s32      unk_18;
    /* 0x1C */ u16      category;
    /* 0x1E */ u16      attr;
} ShiniWall; // Size: 0x20

typedef struct {
    /* 0x0 */ u8*            indexTable;
    /* 0x4 */ ShiniGridInfo* grid;
    /* 0x8 */ ShiniCell*     cells;
    /* 0xC */ ShiniWall*     walls;
} ShiniWorld;

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 taskId[7];
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ s32 unk_28;
} Fld_Shinigami; // Size: 0x2C

extern ShiniWorld* data_ov030_02109d84;

// Wall indices listed for a cell; cell->offset is a byte offset into the index table.
static inline s32* Shini_GetCellWalls(ShiniCell* cell) {
    return (s32*)(data_ov030_02109d84->indexTable + cell->offset);
}

extern void func_ov030_020af6f0(s32);
extern void func_ov030_020af708(s16);
extern s32  func_ov030_020bfee4(TaskPool*, s32, s32);
extern void func_ov030_020bff68(Task*, s32);
extern s32  func_ov030_020c0184(TaskPool*, s32, s32, s16, s16);
extern void func_ov030_020c01c4(Task*, s32);
extern void func_ov030_020c050c(Task*, s16);
extern s32  func_ov030_020cd4bc(TaskPool*, s32);
extern void func_ov030_020cd4ec(Task*, s32);
extern s16  func_ov030_020cd530(Task*);
extern void func_ov030_020cd550(Task*);
extern s32  func_ov030_020cd560(Task*);
extern s32  func_ov030_020cd734(TaskPool*, s32);
extern s32  func_ov030_020cd764(Task*, s16);
extern s32  func_0202599c(s32);

s32        func_ov030_020cb284(FieldPos* pos, s32 category);
void       func_ov030_020cb354(FieldPos* out, FieldPos* from, FieldPos* to, s32 category);
s32        func_ov030_020cb634(FieldPos* pos, s32 x, s32 y);
s32        func_ov030_020cb6cc(FieldPos* pos, u16* outAttr);
s32        func_ov030_020cb72c(FieldPos* pos);
ShiniWall* func_ov030_020cb770(FieldPos* pos, s32 category);
ShiniCell* func_ov030_020cb81c(FieldPos* pos);
s32        func_ov030_020cb898(ShiniWall* wall, FieldPos* pos);
void       func_ov030_020cb990(FieldPos* out, ShiniWall* wall, FieldPos* from, FieldPos* to);
s32        func_ov030_020cbc00(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2);
s32        func_ov030_020cbe98(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2);
s32        func_ov030_020cc170(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2);
void       func_ov030_020cc340(FieldPos* out, FieldPos* from, FieldPos* to);
s32        func_ov030_020cc494(FieldPos* pos, s32 dx, s32 dy, s32 depth);
s32        func_ov030_020cc524(FieldPos* a, FieldPos* b, FieldPos* c);
s32        func_ov030_020cc580(FieldPos* a, FieldPos* b);
void       func_ov030_020cc5d0(FieldPos* out, s32 x, s32 y);
void       func_ov030_020cc5f0(FieldPos* out, s32 x, s32 y);
s32        func_ov030_020cc610(s32 v);
s32        func_ov030_020cc670(TaskPool* pool, Fld_Shinigami* shini);
void       func_ov030_020cc748(TaskPool* pool, Fld_Shinigami* shini, s32 sel);
s32        func_ov030_020cc7a0(TaskPool* pool, Fld_Shinigami* shini);
s16        func_ov030_020cc7fc(TaskPool* pool, Fld_Shinigami* shini);
s32        func_ov030_020cc854(TaskPool* pool, Fld_Shinigami* shini);
s32        func_ov030_020cc8d0(TaskPool* pool, Task* task, void* args);
s32        func_ov030_020cca64(TaskPool* pool, Task* task, void* args);
s32        func_ov030_020ccab8(TaskPool* pool, Task* task, void* args);
s32        func_ov030_020ccac0(TaskPool* pool, Task* task, void* args);
s32        func_ov030_020ccb00(TaskPool* pool, Task* task, void* args, s32 stage);
s32        func_ov030_020ccb48(TaskPool* pool, s32 kind);

char data_ov030_02108920[] = "Tsk_Fld_Shinigami";
char data_ov030_02108934[] = "Tsk_Fld_ShinigamiCoin";
char data_ov030_0210894c[] = "Apl_Abe/Grp_FldShinigami.bin";

const TaskHandle data_ov030_020ed0e4 = {data_ov030_02108920, func_ov030_020ccb00, 0x2C};

const TaskStages data_ov030_020ed0f0 = {
    {func_ov030_020cc8d0, func_ov030_020cca64, func_ov030_020ccab8, func_ov030_020ccac0}
};

s32 func_ov030_020cb284(FieldPos* pos, s32 category) {
    ShiniCell* cell;
    s32*       list;
    s32        numTouch;
    s32        one;
    s32        i;

    if (data_ov030_02109d84->indexTable == NULL) {
        return 0;
    }
    cell = func_ov030_020cb81c(pos);
    if (cell->count == 0) {
        return 0;
    }
    numTouch = 0;
    i        = 0;
    list     = Shini_GetCellWalls(cell);
    for (i = 0; i < cell->count; i++) {
        ShiniWall* wall = &data_ov030_02109d84->walls[list[i]];
        if (wall->category == category) {
            s32 r = func_ov030_020cb898(wall, pos);
            if (r == 1) {
                return 1;
            }
            if (r == -1) {
                numTouch += 1;
            }
        }
    }
    if (numTouch >= 2) {
        return 1;
    }
    if (numTouch == 1) {
        one = 1;
    } else {
        one = 0;
    }
    return 0 - one;
}

void func_ov030_020cb354(FieldPos* out, FieldPos* from, FieldPos* to, s32 category) {
    Vec        diff;
    FieldPos   cand;
    FieldPos   best;
    FieldPos   origin;
    s32        bestDist;
    ShiniCell* cell;
    s32        rFrom;
    s32        rTo;

    diff.x = to->x - from->x;
    diff.y = to->y - from->y;
    diff.z = 0;
    if (Vec_Magnitude(&diff) < 0x1000) {
        Vec_Normalize(&diff, &diff);
        to->x = (diff.x << 1) + from->x;
        to->y = (diff.y << 1) + from->y;
    }
    from->x = func_ov030_020cc610(from->x);
    from->y = func_ov030_020cc610(from->y);
    to->x   = func_ov030_020cc610(to->x);
    to->y   = func_ov030_020cc610(to->y);
    best    = *to;
    if (data_ov030_02109d84->indexTable == NULL) {
        *out = best;
        return;
    }
    rFrom = func_ov030_020cb284(from, category);
    rTo   = func_ov030_020cb284(to, category);
    if ((rFrom == 1) && (rTo == 1)) {
        *out = best;
        return;
    }
    if ((rFrom == 1) && (rTo == -1)) {
        *out = best;
        return;
    }
    if ((rFrom == -1) && (rTo != 0)) {
        *out = best;
        return;
    }
    if ((rFrom == 0) && (rTo == 0)) {
        *out = best;
        return;
    }
    if (rTo == 1) {
        cell   = func_ov030_020cb81c(from);
        origin = *from;
    } else {
        cell   = func_ov030_020cb81c(to);
        origin = *to;
    }
    if (cell->count == 0) {
        *out = best;
        return;
    }
    bestDist = 0x7FFFFFFF;
    {
        s32* list = Shini_GetCellWalls(cell);
        s32  i;

        for (i = 0; i < cell->count; i++) {
            ShiniWall* wall = &data_ov030_02109d84->walls[list[i]];
            if (wall->category == category) {
                func_ov030_020cb990(&cand, wall, from, to);
                if ((cand.x != to->x) || (cand.y != to->y)) {
                    s32 dist = func_ov030_020cc580(&origin, &cand);
                    if (bestDist > dist) {
                        bestDist = dist;
                        best     = cand;
                    }
                }
            }
        }
    }
    if (func_ov030_020cb284(&best, 0) == 0) {
        func_ov030_020cc340(&best, from, to);
    }
    *out = best;
}

s32 func_ov030_020cb634(FieldPos* pos, s32 x, s32 y) {
    FieldPos from;
    FieldPos to;
    FieldPos res;
    FieldPos conv;

    func_ov030_020cc5d0(&from, pos->x, pos->y);
    func_ov030_020cc5d0(&to, x, y);
    func_ov030_020cb354(&res, &from, &to, 0);
    if ((res.x != to.x) || (res.y != to.y)) {
        func_ov030_020cc5f0(&conv, res.x, res.y);
        *pos = conv;
        return 1;
    }
    pos->x = x;
    pos->y = y;
    return 0;
}

s32 func_ov030_020cb6cc(FieldPos* pos, u16* outAttr) {
    FieldPos   pt;
    ShiniWall* wall;
    s32        i;

    func_ov030_020cc5d0(&pt, pos->x, pos->y);
    for (i = 1; i < 3; i++) {
        wall = func_ov030_020cb770(&pt, i);
        if (wall != NULL) {
            *outAttr = wall->attr;
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020cb72c(FieldPos* pos) {
    FieldPos   pt;
    ShiniWall* wall;
    s32        hit;

    hit = 0;
    func_ov030_020cc5d0(&pt, pos->x, pos->y);
    wall = func_ov030_020cb770(&pt, 3);
    if ((wall != NULL) && (wall->attr == 0x3E0)) {
        hit = 1;
    }
    return hit;
}

ShiniWall* func_ov030_020cb770(FieldPos* pos, s32 category) {
    ShiniCell* cell;
    ShiniWall* wall;
    s32*       list;
    s32        i;

    if (data_ov030_02109d84->indexTable == NULL) {
        return NULL;
    }
    cell = func_ov030_020cb81c(pos);
    if (cell->count == 0) {
        return NULL;
    }
    list = Shini_GetCellWalls(cell);
    for (i = 0; i < cell->count; i++) {
        wall = &data_ov030_02109d84->walls[list[i]];
        if ((wall->category == category) && (func_ov030_020cb898(wall, pos) != 0)) {
            return wall;
        }
    }
    return NULL;
}

ShiniCell* func_ov030_020cb81c(FieldPos* pos) {
    ShiniGridInfo* grid;
    ShiniCell*     cells;
    s32            cx;
    s32            cy;
    s32            dx;
    s32            dy;
    s32            h;

    grid  = data_ov030_02109d84->grid;
    cells = data_ov030_02109d84->cells;
    dx    = pos->x - cells->originX;
    dy    = pos->y - cells->originY;
    cx    = (dx / grid->cellWidth) >> 0xC;
    cy    = (dy / grid->cellHeight) >> 0xC;
    h     = grid->height;
    if (cx < 0) {
        cx = 0;
    }
    if (cx >= (s32)grid->width) {
        cx = grid->width - 1;
    }
    if (cy < 0) {
        cy = 0;
    }
    if (cy >= h) {
        cy = h - 1;
    }
    return &cells[(cy * grid->width) + cx];
}

s32 func_ov030_020cb898(ShiniWall* wall, FieldPos* pos) {
    FieldPos v0;
    FieldPos v1;
    FieldPos v2;
    s32      t1;
    s32      t2;
    s32      t3;

    v0.x = wall->v[0].x & ~0xFFF;
    v0.y = wall->v[0].y & ~0xFFF;
    v1.x = wall->v[1].x & ~0xFFF;
    v1.y = wall->v[1].y & ~0xFFF;
    v2.x = wall->v[2].x & ~0xFFF;
    v2.y = wall->v[2].y & ~0xFFF;
    t1   = func_ov030_020cc524(pos, &v0, &v1);
    t2   = func_ov030_020cc524(pos, &v1, &v2);
    t3   = func_ov030_020cc524(pos, &v2, &v0);
    if ((t1 > 0) && (t2 > 0) && (t3 > 0)) {
        return 1;
    }
    if ((t1 < 0) && (t2 < 0) && (t3 < 0)) {
        return 1;
    }
    if ((t1 >= 0) && (t2 >= 0) && (t3 >= 0)) {
        return -1;
    }
    if ((t1 <= 0) && (t2 <= 0) && (t3 <= 0)) {
        return -1;
    }
    return 0;
}

void func_ov030_020cb990(FieldPos* out, ShiniWall* wall, FieldPos* from, FieldPos* to) {
    FieldPos v0;
    FieldPos v1;
    FieldPos v2;
    FieldPos cand;
    FieldPos best;
    s32      bestDist;

    v0.x     = wall->v[0].x & ~0xFFF;
    v0.y     = wall->v[0].y & ~0xFFF;
    v1.x     = wall->v[1].x & ~0xFFF;
    v1.y     = wall->v[1].y & ~0xFFF;
    v2.x     = wall->v[2].x & ~0xFFF;
    v2.y     = wall->v[2].y & ~0xFFF;
    best     = *to;
    bestDist = 0x7FFFFFFF;
    if (func_ov030_020cbc00(&cand, &v0, &v1, from, to) != 0) {
        func_ov030_020cbe98(&cand, &v0, &v1, from, to);
        if (func_ov030_020cb284(&cand, 0) != 0) {
            s32 dist = func_ov030_020cc580(to, &cand);
            if (dist < 0x7FFFFFFF) {
                bestDist = dist;
                best     = cand;
            }
        } else {
            func_ov030_020cc170(&cand, &v0, &v1, from, to);
            best = cand;
        }
    }
    if (func_ov030_020cbc00(&cand, &v1, &v2, from, to) != 0) {
        func_ov030_020cbe98(&cand, &v1, &v2, from, to);
        if (func_ov030_020cb284(&cand, 0) != 0) {
            s32 dist = func_ov030_020cc580(to, &cand);
            if (bestDist > dist) {
                bestDist = dist;
                best     = cand;
            }
        } else {
            func_ov030_020cc170(&cand, &v1, &v2, from, to);
            best = cand;
        }
    }
    if (func_ov030_020cbc00(&cand, &v2, &v0, from, to) != 0) {
        func_ov030_020cbe98(&cand, &v2, &v0, from, to);
        if (func_ov030_020cb284(&cand, 0) != 0) {
            if (bestDist > func_ov030_020cc580(to, &cand)) {
                best = cand;
            }
        } else {
            func_ov030_020cc170(&cand, &v2, &v0, from, to);
            best = cand;
        }
    }
    *out = best;
}

s32 func_ov030_020cbc00(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2) {
    s32 ix;
    s32 iy;
    s32 slope1;
    s32 slope2;

    /* AABB reject between the wall edge (a1-a2) and the move segment (b1-b2). */
    if (((a1->x < a2->x) ? a1->x : a2->x) > ((b1->x > b2->x) ? b1->x : b2->x)) {
        return 0;
    }
    if (((a1->y < a2->y) ? a1->y : a2->y) > ((b1->y > b2->y) ? b1->y : b2->y)) {
        return 0;
    }
    if (((b1->x < b2->x) ? b1->x : b2->x) > ((a1->x > a2->x) ? a1->x : a2->x)) {
        return 0;
    }
    if (((b1->y < b2->y) ? b1->y : b2->y) > ((a1->y > a2->y) ? a1->y : a2->y)) {
        return 0;
    }
    if (func_ov030_020cc524(b1, a1, a2) * func_ov030_020cc524(b2, a1, a2) > 0) {
        return 0;
    }
    if (func_ov030_020cc524(a1, b1, b2) * func_ov030_020cc524(a2, b1, b2) > 0) {
        return 0;
    }
    if ((a1->x == a2->x) && (b1->x == b2->x)) {
        return 0;
    }
    if (a1->x == a2->x) {
        slope1 = a2->y - a1->y;
    } else {
        slope1 = (a2->y - a1->y) / ((a2->x - a1->x) >> 0xC);
    }
    if (b1->x == b2->x) {
        slope2 = b2->y - b1->y;
    } else {
        slope2 = (b2->y - b1->y) / ((b2->x - b1->x) >> 0xC);
    }
    if (slope1 == slope2) {
        return 0;
    }
    if ((a1->x != a2->x) && (b1->x != b2->x)) {
        s32 t = (b1->y + (((a1->x >> 0xC) * slope1) - a1->y - ((b1->x >> 0xC) * slope2))) / (slope1 - slope2);
        iy    = (slope1 * (t - (a1->x >> 0xC))) + a1->y;
        ix    = t << 0xC;
    } else if ((a1->x == a2->x) && (b1->x != b2->x)) {
        iy = (slope2 * ((a1->x >> 0xC) - (b1->x >> 0xC))) + b1->y;
        ix = a1->x;
    } else if ((a1->x != a2->x) && (b1->x == b2->x)) {
        ix = b1->x;
        iy = (slope1 * ((ix - a1->x) >> 0xC)) + a1->y;
    }
    out->x = ix & ~0xFFF;
    out->y = iy & ~0xFFF;
    return 1;
}

s32 func_ov030_020cbe98(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2) {
    Vec      diff;
    Vec      e1;
    Vec      e2;
    Vec      proj;
    FieldPos saved;
    s32      mag;
    s32      len2;
    s32      scale;
    s32      overflow;
    s64      quot;

    diff.x   = b2->x - b1->x;
    diff.y   = b2->y - b1->y;
    diff.z   = 0;
    overflow = 0;
    mag      = Vec_Magnitude(&diff);
    saved    = *(FieldPos*)b2;
    if (func_ov030_020cc524(out, b1, b2) != 0) {
        b2->x = out->x + diff.x / 2;
        b2->y = out->y + diff.y / 2;
    }
    Vec_Normalize(&diff, &diff);
    if (a2->x != out->x || a2->y != out->y) {
        e1.x = a2->x - out->x;
        e1.y = a2->y - out->y;
        e1.z = 0;
    } else {
        e1.x = a1->x - out->x;
        e1.y = a1->y - out->y;
    }
    {
        s32 ex = e1.x >> 3;
        s32 ey = e1.y >> 3;
        len2   = (s32)(((s64)ex * ex + 0x800) >> 0xC) + (s32)(((s64)ey * ey + 0x800) >> 0xC);
    }
    scale = len2 * 8;
    if ((s32)(len2 * 8) > 0x0FFFFFFF) {
        overflow = 1;
    } else {
        scale *= 8;
    }
    if (b2->x != out->x || b2->y != out->y) {
        e2.x = b2->x - out->x;
        e2.y = b2->y - out->y;
        e2.z = 0;
    } else {
        e2.x = -(b1->x - out->x);
        e2.y = -(b1->y - out->y);
        e2.z = 0;
    }
    {
        s32 dot = Vec_DotProduct(&e1, &e2);
        if (overflow != 0) {
            dot = dot >> 3;
        }
        quot = FX_Divide64(dot, scale);
    }
    proj.x = (s32)((quot * e1.x + 0x80000000) >> 32);
    proj.y = (s32)((quot * e1.y + 0x80000000) >> 32);
    proj.z = 0;
    Vec_Normalize(&proj, &proj);
    if (mag > 0x1000) {
        s32 t  = mag + 0x1000;
        proj.x = (s32)(((s64)proj.x * t + 0x800) >> 0xC);
        proj.y = (s32)(((s64)proj.y * t + 0x800) >> 0xC);
    }
    out->x = b1->x + proj.x;
    out->y = b1->y + proj.y;
    out->x = out->x & ~0xFFF;
    out->y = out->y & ~0xFFF;
    b2->x  = saved.x;
    b2->y  = saved.y;
    return 1;
}

s32 func_ov030_020cc170(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2) {
    s32 dy;
    s32 dx;
    s32 origDx;

    dy     = out->y - b1->y;
    dx     = out->x - b1->x;
    origDx = dx;
    if ((dy < 0x1000) && (dy > 0)) {
        dy += 0x1000;
    } else if ((dy > -0x1000) && (dy < 0)) {
        dy -= 0x1000;
    }
    if ((dx < 0x1000) && (dx > 0)) {
        dx += 0x1000;
    } else if ((dx > -0x1000) && (dx < 0)) {
        dx -= 0x1000;
    }
    if ((dy < 0) && (a1->y > a2->y) && (a1->x > a2->x)) {
        dy *= -1;
    }
    if ((dx < 0) && (a1->x < a2->x) && (a1->y < a2->y)) {
        dx *= -1;
    }
    if ((origDx < 0) && (a1->x > a2->x) && (a1->y < a2->y)) {
        dx = 0x1000;
    }
    if ((dy == 0) && (a1->y > a2->y)) {
        dy += 0x1000;
    }
    if (dx == 0) {
        if (((a1->x > a2->x) && (a1->y > a2->y)) || ((a1->x < a2->x) && (a1->y < a2->y))) {
            dx = 0x1000;
        }
    }
    if ((dx * dx) < (dy * dy)) {
        out->x += dx;
        if (func_ov030_020cb284(out, 0) == 0) {
            out->x -= dx;
            out->y += dy;
        }
    } else {
        out->y += dy;
        if (func_ov030_020cb284(out, 0) == 0) {
            out->y -= dy;
            out->x += dx;
        }
    }
    return 1;
}

void func_ov030_020cc340(FieldPos* out, FieldPos* from, FieldPos* to) {
    FieldPos pt;
    s32      dx;
    s32      dy;
    s32      n1;
    s32      n2;

    pt = *from;
    dx = (to->x - from->x) * 8;
    dy = (to->y - from->y) * 8;
    n1 = func_ov030_020cc494(&pt, 0 - dy, dx, 0);
    n2 = func_ov030_020cc494(&pt, dy, 0 - dx, 0);
    if (n1 < n2) {
        pt.x = from->x + (dy >> 3);
        pt.y = from->y - (dx >> 3);
        if (func_ov030_020cb284(&pt, 0) == 0) {
            pt.x = from->x - (dy >> 3);
            pt.y = from->y + (dx >> 3);
        }
    } else if (n2 < n1) {
        pt.x = from->x - (dy >> 3);
        pt.y = from->y + (dx >> 3);
        if (func_ov030_020cb284(&pt, 0) == 0) {
            pt.x = from->x + (dy >> 3);
            pt.y = from->y - (dx >> 3);
        }
    } else {
        pt = *from;
    }
    if (func_ov030_020cb284(&pt, 0) == 0) {
        pt = *from;
    }
    *out = pt;
}

s32 func_ov030_020cc494(FieldPos* pos, s32 dx, s32 dy, s32 depth) {
    FieldPos pt;
    s32      ny;

    pos->x += dx;
    ny     = pos->y + dy;
    pos->y = ny;
    pt.x   = pos->x & ~0xFFF;
    pt.y   = ny & ~0xFFF;
    depth += 1;
    if ((func_ov030_020cb284(&pt, 0) != 0) && (depth < 8)) {
        depth = func_ov030_020cc494(pos, dx, dy, depth);
    }
    return depth;
}

s32 func_ov030_020cc524(FieldPos* a, FieldPos* b, FieldPos* c) {
    s32 t;
    s32 r;

    t = ((c->x >> 0xC) * (a->y - b->y)) + (((a->x >> 0xC) * (b->y - c->y)) + ((b->x >> 0xC) * (c->y - a->y)));
    if (t > 0) {
        return 1;
    }
    if (t < 0) {
        r = 1;
    } else {
        r = 0;
    }
    return 0 - r;
}

s32 func_ov030_020cc580(FieldPos* a, FieldPos* b) {
    s32 dx;
    s32 dy;

    dx = b->x - a->x;
    dy = b->y - a->y;
    return FX_Sqrt((s32)(((s64)dx * dx + 0x800) >> 0xC) + (s32)(((s64)dy * dy + 0x800) >> 0xC));
}

void func_ov030_020cc5d0(FieldPos* out, s32 x, s32 y) {
    FieldPos pt;

    pt.x = x;
    pt.y = -y;
    *out = pt;
}

void func_ov030_020cc5f0(FieldPos* out, s32 x, s32 y) {
    FieldPos pt;

    pt.x = x;
    pt.y = -y;
    *out = pt;
}

// Nonmatching: neg/round locals land in r2/r3 reversed from target (neg=r3,
// round=r2); ~30 alternative spellings (decl order, initializers, control-flow
// shape, guarding the negation, masking into a local, u16/s16 neighbours, ternary
// sign propagation) are byte-identical, so it is an allocator tie (USA 98.12%;
// JP unvalidated).
s32 func_ov030_020cc610(s32 v) {
    s32 neg;
    s32 round;

    neg = 0;
    if (v < 0) {
        v *= -1;
        neg = 1;
    }
    if ((v & 0xFFF) < 0x800) {
        round = 0;
    } else {
        round = 0x1000;
    }
    v = v & ~0xFFF;
    if (neg != 0) {
        v *= -1;
        round = 0 - round;
    }
    return v + round;
}

s32 func_ov030_020cc670(TaskPool* pool, Fld_Shinigami* shini) {
    s16 sel;

    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (shini->unk_24 == 1) {
        func_ov030_020c050c(EasyTask_GetTaskById(pool, shini->taskId[4]), 2);
        shini->unk_24 = 2;
    }
    if (TouchInput_WasTouchPressed() != 0) {
        sel = func_ov030_020cc7fc(pool, shini);
        if (sel == 0) {
            func_ov030_020cc748(pool, shini, sel);
            func_ov030_020af6f0(3);
            SndMgr_PlaySEWithPan(SEIDX_SE_KEYWORD_SWITCH, 0xE2);
            shini->unk_28 = 1;
            func_ov030_020cd550(EasyTask_GetTaskById(pool, shini->taskId[5]));
        } else if ((sel != -1) && (shini->unk_24 != sel)) {
            shini->unk_24 = sel;
            func_ov030_020cc748(pool, shini, sel);
            func_ov030_020cc7a0(pool, shini);
        }
    }
    return 1;
}

void func_ov030_020cc748(TaskPool* pool, Fld_Shinigami* shini, s32 sel) {
    s32 i;
    s32 v;

    for (i = 0; i < 3; i++) {
        if (shini->taskId[i] != -1) {
            Task* task = EasyTask_GetTaskById(pool, shini->taskId[i]);
            if (i == sel) {
                v = 1;
            } else {
                v = 2;
            }
            func_ov030_020c01c4(task, v);
        }
    }
}

s32 func_ov030_020cc7a0(TaskPool* pool, Fld_Shinigami* shini) {
    if (shini->unk_24 == 1) {
        func_ov030_020bff68(EasyTask_GetTaskById(pool, shini->taskId[3]), 2);
        func_ov030_020af6f0(5);
        func_ov030_020af708(0xD);
        SndMgr_PlaySEWithPan(SEIDX_SE_KEYWORD_SWITCH, 0x23);
        func_ov030_020cd550(EasyTask_GetTaskById(pool, shini->taskId[5]));
    }
    return 1;
}

s16 func_ov030_020cc7fc(TaskPool* pool, Fld_Shinigami* shini) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (shini->taskId[i] != -1) {
            if (func_ov030_020c01ec(EasyTask_GetTaskById(pool, shini->taskId[i])) != 0) {
                return i;
            }
        }
    }
    return -1;
}

s32 func_ov030_020cc854(TaskPool* pool, Fld_Shinigami* shini) {
    TouchCoord coord;
    s16        anim;

    TouchInput_GetCoord(&coord);
    anim = func_ov030_020cd530(EasyTask_GetTaskById(pool, shini->taskId[5]));
    func_ov030_020cd764(EasyTask_GetTaskById(pool, shini->taskId[6]), anim);
    if (anim != 0) {
        if (shini->unk_22 == 0) {
            shini->unk_22 = 1;
            SndMgr_StartPlayingSE(SEIDX_SG_SHINIGAMISAN_KETTEI);
        }
    } else {
        shini->unk_22 = 0;
    }
    return 1;
}

s32 func_ov030_020cc8d0(TaskPool* pool, Task* task, void* args) {
    Fld_Shinigami* shini = task->data;
    s32*           kind  = args;

    shini->taskId[6] = func_ov030_020cd734(pool, *kind);
    shini->taskId[5] = func_ov030_020cd4bc(pool, *kind);
    shini->taskId[0] = func_ov030_020c0184(pool, *kind, 0, 0xE2, 0xA7);
    shini->taskId[1] = -1;
    shini->taskId[2] = -1;
    shini->taskId[4] = -1;
    if (func_0202599c(2) != 0) {
        shini->taskId[1] = func_ov030_020c0184(pool, *kind, 2, 0x23, 0xA0);
    }
    if (func_0202599c(3) != 0) {
        shini->taskId[2] = func_ov030_020c0184(pool, *kind, 4, 0xE2, 0x21);
        func_ov030_020c01c4(EasyTask_GetTaskById(pool, shini->taskId[2]), 1);
        shini->taskId[3] = func_ov030_020bfee4(pool, *kind, 3);
        shini->unk_24    = 2;
        shini->taskId[4] = func_ov030_020c04d8(pool, *kind, 2);
    } else if (func_0202599c(2) != 0) {
        func_ov030_020c01c4(EasyTask_GetTaskById(pool, shini->taskId[1]), 1);
        shini->taskId[3] = func_ov030_020bfee4(pool, *kind, 2);
        shini->unk_24    = 1;
        shini->taskId[4] = func_ov030_020c04d8(pool, *kind, 1);
    } else {
        shini->taskId[3] = -1;
        shini->unk_24    = -1;
    }
    func_ov030_020cd4ec(EasyTask_GetTaskById(pool, shini->taskId[5]), 1);
    shini->unk_20 = -1;
    shini->unk_22 = 0;
    shini->unk_28 = 0;
    return 1;
}

s32 func_ov030_020cca64(TaskPool* pool, Task* task, void* args) {
    Fld_Shinigami* shini = task->data;

    if (shini->unk_28 == 1) {
        return 1;
    }
    if (func_ov030_020cd560(EasyTask_GetTaskById(pool, shini->taskId[5])) != 0) {
        return 1;
    }
    func_ov030_020cc670(pool, shini);
    func_ov030_020cc854(pool, shini);
    return 1;
}

s32 func_ov030_020ccab8(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov030_020ccac0(TaskPool* pool, Task* task, void* args) {
    Fld_Shinigami* shini = task->data;
    s32            i;

    for (i = 0; i < 7; i++) {
        if (shini->taskId[i] != -1) {
            EasyTask_DeleteTask(pool, shini->taskId[i]);
        }
    }
    return 1;
}

s32 func_ov030_020ccb00(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = data_ov030_020ed0f0;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020ccb48(TaskPool* pool, s32 kind) {
    s32 arg = kind;

    return EasyTask_CreateTask(pool, &data_ov030_020ed0e4, NULL, 0, NULL, &arg);
}
