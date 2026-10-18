#include "Field/Tasks/Fld_Shinigami.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/FieldTypes.h"
#include "Field/Tasks/Fld_ShinigamiCoin.h"
#include "Field/Tasks/Fld_ShinigamiON.h"
#include "Field/Tasks/Fld_TouchCommand.h"
#include "Field/Tasks/Fld_TouchCursor.h"
#include "Field/Tasks/Fld_TouchSeal.h"
#include "SndMgr.h"
#include <nitro/fx.h>

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 taskId[7];
    /* 0x20 */ s16 unk_20;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
    /* 0x28 */ s32 unk_28;
} Fld_Shinigami; // Size: 0x2C

static ShiniWorld* Fld_Shinigami_World;

// Wall indices listed for a cell; cell->offset is a byte offset into the index table.
static inline s32* Shini_GetCellWalls(ShiniCell* cell) {
    return (s32*)(Fld_Shinigami_World->indexTable + cell->offset);
}

void Fld_Shinigami_SetWorld(ShiniWorld* world, void* arg1, u8* file) {
    Fld_Shinigami_World             = world;
    Fld_Shinigami_World->indexTable = file;
    if (Fld_Shinigami_World->indexTable == NULL) {
        return;
    }
    Fld_Shinigami_World->grid   = (ShiniGridInfo*)Fld_Shinigami_World->indexTable;
    Fld_Shinigami_World->cells  = (ShiniCell*)&Fld_Shinigami_World->indexTable[sizeof(ShiniGridInfo)];
    Fld_Shinigami_World->walls  = (ShiniWall*)&Fld_Shinigami_World->indexTable[Fld_Shinigami_World->grid->wallsOffset];
    Fld_Shinigami_World->unk_10 = &Fld_Shinigami_World->indexTable[Fld_Shinigami_World->grid->unk_1C];
}

extern s32        func_0202599c(s32);
static ShiniWall* Fld_Shinigami_FindWall(FieldPos* pos, s32 category);
static ShiniCell* Fld_Shinigami_GetCell(FieldPos* pos);
static s32        Fld_Shinigami_TestWall(ShiniWall* wall, FieldPos* pos);
static void       Fld_Shinigami_SlideAlongWall(FieldPos* out, ShiniWall* wall, FieldPos* from, FieldPos* to);
static s32        Fld_Shinigami_IntersectSegments(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2);
static s32        Fld_Shinigami_ProjectOntoEdge(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2);
static s32        Fld_Shinigami_PushOutOfWall(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2);
static void       Fld_Shinigami_PushSideways(FieldPos* out, FieldPos* from, FieldPos* to);
static s32        Fld_Shinigami_StepUntilFree(FieldPos* pos, s32 dx, s32 dy, s32 depth);
static s32        Fld_Shinigami_SideOfLine(FieldPos* a, FieldPos* b, FieldPos* c);
static s32        Fld_Shinigami_Distance(FieldPos* a, FieldPos* b);
static void       Fld_Shinigami_ToWallSpace(FieldPos* out, s32 x, s32 y);
static void       Fld_Shinigami_FromWallSpace(FieldPos* out, s32 x, s32 y);
static s32        Fld_Shinigami_RoundToPixel(s32 v);
static void       Fld_Shinigami_SelectSeal(TaskPool* pool, Fld_Shinigami* shini, s32 sel);
static s32        Fld_Shinigami_ApplySelection(TaskPool* pool, Fld_Shinigami* shini);
static s16        Fld_Shinigami_FindTouchedSeal(TaskPool* pool, Fld_Shinigami* shini);
static s32        Fld_Shinigami_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Shinigami = {"Tsk_Fld_Shinigami", Fld_Shinigami_RunTask, sizeof(Fld_Shinigami)};

static s32 Fld_Shinigami_TestPoint(FieldPos* pos, s32 category) {
    ShiniCell* cell;
    s32*       list;
    s32        numTouch;
    s32        one;
    s32        i;

    if (Fld_Shinigami_World->indexTable == NULL) {
        return 0;
    }
    cell = Fld_Shinigami_GetCell(pos);
    if (cell->count == 0) {
        return 0;
    }
    numTouch = 0;
    i        = 0;
    list     = Shini_GetCellWalls(cell);
    for (i = 0; i < cell->count; i++) {
        ShiniWall* wall = &Fld_Shinigami_World->walls[list[i]];
        if (wall->category == category) {
            s32 r = Fld_Shinigami_TestWall(wall, pos);
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

static void Fld_Shinigami_ResolveMove(FieldPos* out, FieldPos* from, FieldPos* to, s32 category) {
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
    from->x = Fld_Shinigami_RoundToPixel(from->x);
    from->y = Fld_Shinigami_RoundToPixel(from->y);
    to->x   = Fld_Shinigami_RoundToPixel(to->x);
    to->y   = Fld_Shinigami_RoundToPixel(to->y);
    best    = *to;
    if (Fld_Shinigami_World->indexTable == NULL) {
        *out = best;
        return;
    }
    rFrom = Fld_Shinigami_TestPoint(from, category);
    rTo   = Fld_Shinigami_TestPoint(to, category);
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
        cell   = Fld_Shinigami_GetCell(from);
        origin = *from;
    } else {
        cell   = Fld_Shinigami_GetCell(to);
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
            ShiniWall* wall = &Fld_Shinigami_World->walls[list[i]];
            if (wall->category == category) {
                Fld_Shinigami_SlideAlongWall(&cand, wall, from, to);
                if ((cand.x != to->x) || (cand.y != to->y)) {
                    s32 dist = Fld_Shinigami_Distance(&origin, &cand);
                    if (bestDist > dist) {
                        bestDist = dist;
                        best     = cand;
                    }
                }
            }
        }
    }
    if (Fld_Shinigami_TestPoint(&best, 0) == 0) {
        Fld_Shinigami_PushSideways(&best, from, to);
    }
    *out = best;
}

s32 Fld_Shinigami_MoveWithCollision(FieldPos* pos, s32 x, s32 y) {
    FieldPos from;
    FieldPos to;
    FieldPos res;
    FieldPos conv;

    Fld_Shinigami_ToWallSpace(&from, pos->x, pos->y);
    Fld_Shinigami_ToWallSpace(&to, x, y);
    Fld_Shinigami_ResolveMove(&res, &from, &to, 0);
    if ((res.x != to.x) || (res.y != to.y)) {
        Fld_Shinigami_FromWallSpace(&conv, res.x, res.y);
        *pos = conv;
        return 1;
    }
    pos->x = x;
    pos->y = y;
    return 0;
}

s32 Fld_Shinigami_GetFloorAttr(FieldPos* pos, u16* outAttr) {
    FieldPos   pt;
    ShiniWall* wall;
    s32        i;

    Fld_Shinigami_ToWallSpace(&pt, pos->x, pos->y);
    for (i = 1; i < 3; i++) {
        wall = Fld_Shinigami_FindWall(&pt, i);
        if (wall != NULL) {
            *outAttr = wall->attr;
            return 1;
        }
    }
    return 0;
}

s32 Fld_Shinigami_IsOccluded(FieldPos* pos) {
    FieldPos   pt;
    ShiniWall* wall;
    s32        hit;

    hit = 0;
    Fld_Shinigami_ToWallSpace(&pt, pos->x, pos->y);
    wall = Fld_Shinigami_FindWall(&pt, 3);
    if ((wall != NULL) && (wall->attr == 0x3E0)) {
        hit = 1;
    }
    return hit;
}

static ShiniWall* Fld_Shinigami_FindWall(FieldPos* pos, s32 category) {
    ShiniCell* cell;
    ShiniWall* wall;
    s32*       list;
    s32        i;

    if (Fld_Shinigami_World->indexTable == NULL) {
        return NULL;
    }
    cell = Fld_Shinigami_GetCell(pos);
    if (cell->count == 0) {
        return NULL;
    }
    list = Shini_GetCellWalls(cell);
    for (i = 0; i < cell->count; i++) {
        wall = &Fld_Shinigami_World->walls[list[i]];
        if ((wall->category == category) && (Fld_Shinigami_TestWall(wall, pos) != 0)) {
            return wall;
        }
    }
    return NULL;
}

static ShiniCell* Fld_Shinigami_GetCell(FieldPos* pos) {
    ShiniGridInfo* grid;
    ShiniCell*     cells;
    s32            cx;
    s32            cy;
    s32            dx;
    s32            dy;
    s32            h;

    grid  = Fld_Shinigami_World->grid;
    cells = Fld_Shinigami_World->cells;
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

static s32 Fld_Shinigami_TestWall(ShiniWall* wall, FieldPos* pos) {
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
    t1   = Fld_Shinigami_SideOfLine(pos, &v0, &v1);
    t2   = Fld_Shinigami_SideOfLine(pos, &v1, &v2);
    t3   = Fld_Shinigami_SideOfLine(pos, &v2, &v0);
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

static void Fld_Shinigami_SlideAlongWall(FieldPos* out, ShiniWall* wall, FieldPos* from, FieldPos* to) {
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
    if (Fld_Shinigami_IntersectSegments(&cand, &v0, &v1, from, to) != 0) {
        Fld_Shinigami_ProjectOntoEdge(&cand, &v0, &v1, from, to);
        if (Fld_Shinigami_TestPoint(&cand, 0) != 0) {
            s32 dist = Fld_Shinigami_Distance(to, &cand);
            if (dist < 0x7FFFFFFF) {
                bestDist = dist;
                best     = cand;
            }
        } else {
            Fld_Shinigami_PushOutOfWall(&cand, &v0, &v1, from, to);
            best = cand;
        }
    }
    if (Fld_Shinigami_IntersectSegments(&cand, &v1, &v2, from, to) != 0) {
        Fld_Shinigami_ProjectOntoEdge(&cand, &v1, &v2, from, to);
        if (Fld_Shinigami_TestPoint(&cand, 0) != 0) {
            s32 dist = Fld_Shinigami_Distance(to, &cand);
            if (bestDist > dist) {
                bestDist = dist;
                best     = cand;
            }
        } else {
            Fld_Shinigami_PushOutOfWall(&cand, &v1, &v2, from, to);
            best = cand;
        }
    }
    if (Fld_Shinigami_IntersectSegments(&cand, &v2, &v0, from, to) != 0) {
        Fld_Shinigami_ProjectOntoEdge(&cand, &v2, &v0, from, to);
        if (Fld_Shinigami_TestPoint(&cand, 0) != 0) {
            if (bestDist > Fld_Shinigami_Distance(to, &cand)) {
                best = cand;
            }
        } else {
            Fld_Shinigami_PushOutOfWall(&cand, &v2, &v0, from, to);
            best = cand;
        }
    }
    *out = best;
}

static s32 Fld_Shinigami_IntersectSegments(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2) {
    s32 ix;
    s32 iy;
    s32 slope1;
    s32 slope2;

    // AABB reject between the wall edge (a1-a2) and the move segment (b1-b2).
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
    if (Fld_Shinigami_SideOfLine(b1, a1, a2) * Fld_Shinigami_SideOfLine(b2, a1, a2) > 0) {
        return 0;
    }
    if (Fld_Shinigami_SideOfLine(a1, b1, b2) * Fld_Shinigami_SideOfLine(a2, b1, b2) > 0) {
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

static s32 Fld_Shinigami_ProjectOntoEdge(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2) {
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
    if (Fld_Shinigami_SideOfLine(out, b1, b2) != 0) {
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

static s32 Fld_Shinigami_PushOutOfWall(FieldPos* out, FieldPos* a1, FieldPos* a2, FieldPos* b1, FieldPos* b2) {
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
        if (Fld_Shinigami_TestPoint(out, 0) == 0) {
            out->x -= dx;
            out->y += dy;
        }
    } else {
        out->y += dy;
        if (Fld_Shinigami_TestPoint(out, 0) == 0) {
            out->y -= dy;
            out->x += dx;
        }
    }
    return 1;
}

static void Fld_Shinigami_PushSideways(FieldPos* out, FieldPos* from, FieldPos* to) {
    FieldPos pt;
    s32      dx;
    s32      dy;
    s32      n1;
    s32      n2;

    pt = *from;
    dx = (to->x - from->x) * 8;
    dy = (to->y - from->y) * 8;
    n1 = Fld_Shinigami_StepUntilFree(&pt, 0 - dy, dx, 0);
    n2 = Fld_Shinigami_StepUntilFree(&pt, dy, 0 - dx, 0);
    if (n1 < n2) {
        pt.x = from->x + (dy >> 3);
        pt.y = from->y - (dx >> 3);
        if (Fld_Shinigami_TestPoint(&pt, 0) == 0) {
            pt.x = from->x - (dy >> 3);
            pt.y = from->y + (dx >> 3);
        }
    } else if (n2 < n1) {
        pt.x = from->x - (dy >> 3);
        pt.y = from->y + (dx >> 3);
        if (Fld_Shinigami_TestPoint(&pt, 0) == 0) {
            pt.x = from->x + (dy >> 3);
            pt.y = from->y - (dx >> 3);
        }
    } else {
        pt = *from;
    }
    if (Fld_Shinigami_TestPoint(&pt, 0) == 0) {
        pt = *from;
    }
    *out = pt;
}

static s32 Fld_Shinigami_StepUntilFree(FieldPos* pos, s32 dx, s32 dy, s32 depth) {
    FieldPos pt;
    s32      ny;

    pos->x += dx;
    ny     = pos->y + dy;
    pos->y = ny;
    pt.x   = pos->x & ~0xFFF;
    pt.y   = ny & ~0xFFF;
    depth += 1;
    if ((Fld_Shinigami_TestPoint(&pt, 0) != 0) && (depth < 8)) {
        depth = Fld_Shinigami_StepUntilFree(pos, dx, dy, depth);
    }
    return depth;
}

static s32 Fld_Shinigami_SideOfLine(FieldPos* a, FieldPos* b, FieldPos* c) {
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

static s32 Fld_Shinigami_Distance(FieldPos* a, FieldPos* b) {
    s32 dx;
    s32 dy;

    dx = b->x - a->x;
    dy = b->y - a->y;
    return FX_Sqrt((s32)(((s64)dx * dx + 0x800) >> 0xC) + (s32)(((s64)dy * dy + 0x800) >> 0xC));
}

static void Fld_Shinigami_ToWallSpace(FieldPos* out, s32 x, s32 y) {
    FieldPos pt;

    pt.x = x;
    pt.y = -y;
    *out = pt;
}

static void Fld_Shinigami_FromWallSpace(FieldPos* out, s32 x, s32 y) {
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
static s32 Fld_Shinigami_RoundToPixel(s32 v) {
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

static s32 Fld_Shinigami_UpdateSeals(TaskPool* pool, Fld_Shinigami* shini) {
    s16 sel;

    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
    if (shini->unk_24 == 1) {
        Fld_TouchCursor_SetAnimation(EasyTask_GetTaskById(pool, shini->taskId[4]), 2);
        shini->unk_24 = 2;
    }
    if (TouchInput_WasTouchPressed() != 0) {
        sel = Fld_Shinigami_FindTouchedSeal(pool, shini);
        if (sel == 0) {
            Fld_Shinigami_SelectSeal(pool, shini, sel);
            Field_SetTouchEventResult(3);
            SndMgr_PlaySEWithPan(SEIDX_SE_KEYWORD_SWITCH, 0xE2);
            shini->unk_28 = 1;
            Fld_ShinigamiCoin_Suppress(EasyTask_GetTaskById(pool, shini->taskId[5]));
        } else if ((sel != -1) && (shini->unk_24 != sel)) {
            shini->unk_24 = sel;
            Fld_Shinigami_SelectSeal(pool, shini, sel);
            Fld_Shinigami_ApplySelection(pool, shini);
        }
    }
    return 1;
}

static void Fld_Shinigami_SelectSeal(TaskPool* pool, Fld_Shinigami* shini, s32 sel) {
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
            ((void (*)())Fld_TouchSeal_SetAnimation)(task, v);
        }
    }
}

static s32 Fld_Shinigami_ApplySelection(TaskPool* pool, Fld_Shinigami* shini) {
    if (shini->unk_24 == 1) {
        Fld_TouchCommand_SetCommand(EasyTask_GetTaskById(pool, shini->taskId[3]), 2);
        Field_SetTouchEventResult(5);
        Field_SetTouchEventTarget(0xD);
        SndMgr_PlaySEWithPan(SEIDX_SE_KEYWORD_SWITCH, 0x23);
        Fld_ShinigamiCoin_Suppress(EasyTask_GetTaskById(pool, shini->taskId[5]));
    }
    return 1;
}

static s16 Fld_Shinigami_FindTouchedSeal(TaskPool* pool, Fld_Shinigami* shini) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (shini->taskId[i] != -1) {
            if (Fld_TouchSeal_IsTouched(EasyTask_GetTaskById(pool, shini->taskId[i])) != 0) {
                return i;
            }
        }
    }
    return -1;
}

static s32 Fld_Shinigami_UpdateOnIndicator(TaskPool* pool, Fld_Shinigami* shini) {
    TouchCoord coord;
    s16        anim;

    TouchInput_GetCoord(&coord);
    anim = Fld_ShinigamiCoin_GetOnAnim(EasyTask_GetTaskById(pool, shini->taskId[5]));
    Fld_ShinigamiON_SetAnimation(EasyTask_GetTaskById(pool, shini->taskId[6]), anim);
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

static s32 Fld_Shinigami_Init(TaskPool* pool, Task* task, void* args) {
    Fld_Shinigami* shini = task->data;
    s32*           kind  = args;

    shini->taskId[6] = Fld_ShinigamiON_CreateTask(pool, *kind);
    shini->taskId[5] = Fld_ShinigamiCoin_CreateTask(pool, *kind);
    shini->taskId[0] = Fld_TouchSeal_CreateTask(pool, *kind, 0, 0xE2, 0xA7);
    shini->taskId[1] = -1;
    shini->taskId[2] = -1;
    shini->taskId[4] = -1;
    if (func_0202599c(2) != 0) {
        shini->taskId[1] = Fld_TouchSeal_CreateTask(pool, *kind, 2, 0x23, 0xA0);
    }
    if (func_0202599c(3) != 0) {
        shini->taskId[2] = Fld_TouchSeal_CreateTask(pool, *kind, 4, 0xE2, 0x21);
        Fld_TouchSeal_SetAnimation(EasyTask_GetTaskById(pool, shini->taskId[2]), 1);
        shini->taskId[3] = Fld_TouchCommand_CreateTask(pool, *kind, 3);
        shini->unk_24    = 2;
        shini->taskId[4] = Fld_TouchCursor_CreateTask(pool, *kind, 2);
    } else if (func_0202599c(2) != 0) {
        Fld_TouchSeal_SetAnimation(EasyTask_GetTaskById(pool, shini->taskId[1]), 1);
        shini->taskId[3] = Fld_TouchCommand_CreateTask(pool, *kind, 2);
        shini->unk_24    = 1;
        shini->taskId[4] = Fld_TouchCursor_CreateTask(pool, *kind, 1);
    } else {
        shini->taskId[3] = -1;
        shini->unk_24    = -1;
    }
    Fld_ShinigamiCoin_Reset(EasyTask_GetTaskById(pool, shini->taskId[5]), 1);
    shini->unk_20 = -1;
    shini->unk_22 = 0;
    shini->unk_28 = 0;
    return 1;
}

static s32 Fld_Shinigami_Update(TaskPool* pool, Task* task, void* args) {
    Fld_Shinigami* shini = task->data;

    if (shini->unk_28 == 1) {
        return 1;
    }
    if (Fld_ShinigamiCoin_IsSuppressed(EasyTask_GetTaskById(pool, shini->taskId[5])) != 0) {
        return 1;
    }
    Fld_Shinigami_UpdateSeals(pool, shini);
    Fld_Shinigami_UpdateOnIndicator(pool, shini);
    return 1;
}

static s32 Fld_Shinigami_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Fld_Shinigami_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_Shinigami* shini = task->data;
    s32            i;

    for (i = 0; i < 7; i++) {
        if (shini->taskId[i] != -1) {
            EasyTask_DeleteTask(pool, shini->taskId[i]);
        }
    }
    return 1;
}

static s32 Fld_Shinigami_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Shinigami_Init,
        .update     = Fld_Shinigami_Update,
        .render     = Fld_Shinigami_Render,
        .cleanup    = Fld_Shinigami_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Shinigami_CreateTask(TaskPool* pool, s32 kind) {
    s32 arg = kind;

    return EasyTask_CreateTask(pool, &Tsk_Fld_Shinigami, NULL, 0, NULL, &arg);
}
