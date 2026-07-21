#include "Engine/Math/Rect.h"

// Nonmatching
BOOL Rect_Intersect(s32* out, const s32* a, const s32* b) {
    BOOL isValid = FALSE;

    if ((((a[2] > 0) && (a[3] > 0)) && (b[2] > 0)) && (b[3] > 0)) {
        s32 x2 = a[0] + a[2];
        s32 x1 = (a[0] < b[0]) ? (b[0]) : (a[0]);
        s32 y2 = a[1] + a[3];
        s32 y1 = (a[1] < b[1]) ? (b[1]) : (a[1]);

        if (x2 > (b[0] + b[2])) {
            x2 = b[0] + b[2];
        }
        if (y2 > (b[1] + b[3])) {
            y2 = b[1] + b[3];
        }

        out[0] = x1;
        out[1] = y1;
        out[2] = x2;
        out[2] -= x1;
        out[3] = y2 - y1;

        if ((out[2] > 0) && (out[3] > 0)) {
            isValid = TRUE;
        }
    }
    return isValid;
}

BOOL Rect_ContainsPoint(const s32* rect, s32 x, s32 y) {
    BOOL isWithin = FALSE;

    if (x < rect[0]) {
        isWithin = FALSE;
    } else if (x > rect[2]) {
        isWithin = FALSE;
    } else if (y < rect[1]) {
        isWithin = FALSE;
    } else if (y <= rect[3]) {
        isWithin = TRUE;
    }
    return isWithin;
}

void Rect_Union(s32* out, const s32* a, const s32* b) {
    if ((a[0] < b[0])) {
        out[0] = a[0];
    } else {
        out[0] = b[0];
    }

    if ((a[1] < b[1])) {
        out[1] = a[1];
    } else {
        out[1] = b[1];
    }

    if ((a[2] > b[2])) {
        out[2] = a[2];
    } else {
        out[2] = b[2];
    }

    if ((a[3] > b[3])) {
        out[3] = a[3];
    } else {
        out[3] = b[3];
    }
}
