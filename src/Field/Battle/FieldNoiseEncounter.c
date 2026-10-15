#include "Engine/Math/Random.h"
#include "Save.h"

// USA: 0x020c5744-0x020c59b0. Tables remain linked from the incomplete
// target object. See docs/decomp/ov030/README.md for ownership and semantics.
typedef struct {
    s16 encounter;
    s16 weight;
} FieldNoiseEntry;

typedef struct {
    s8 first;
    s8 second;
} FieldNoiseSequenceEntry;

// 41-word (0xA4) per-area selector table; retail snapshots it to a local
// before indexing.
typedef struct {
    /* 0x00 */ const FieldNoiseEntry* const* tables[41];
} FieldNoiseTableSet; // Size: 0xA4

extern const FieldNoiseTableSet              data_ov030_020e98a0;
extern s32                                   func_ov030_020c5f5c(s32 encounter);
extern const s32                             data_ov030_020ea394[41];
extern const FieldNoiseSequenceEntry* const* data_ov030_02108408[];

// Snapshot-copies the 41-word table and returns the entry for the current
// area (NULL when the area index is out of range).
const FieldNoiseEntry* const* func_ov030_020c5744(void) {
    FieldNoiseTableSet local = data_ov030_020e98a0;
    if ((u32)gSaveData.unk_2458 >= 41) {
        return NULL;
    }
    return local.tables[gSaveData.unk_2458];
}

BOOL func_ov030_020c5798(s32 kind, s32 encounter) {
    s32 category = func_ov030_020c5f5c(encounter);
    if (kind == 0) {
        return category < 0x18;
    }
    if (kind == 1) {
        if (category >= 0x17 && category < 0x1F) {
            return 1;
        }
        return 0;
    }
    if (kind == 2) {
        return category == 7;
    }
    return 0;
}

s16 func_ov030_020c5804(s32 kind) {
    const FieldNoiseEntry* const* chapters = func_ov030_020c5744();
    s32                           i;
    u32                           chapter;
    s16                           roll;
    const FieldNoiseEntry*        entries;

    if (chapters == NULL) {
        return -1;
    }
    i       = 0;
    chapter = gSaveData.chapter;
    if (chapter >= 22) {
        chapter -= 22;
    }
    entries = chapters[chapter];
    roll    = (s16)RNG_Next(100);
    for (;;) {
        if (entries[i].encounter == -1) {
            return -1;
        }
        if (func_ov030_020c5798(kind, entries[i].encounter)) {
            if (entries[i].weight > roll) {
                return entries[i].encounter;
            }
            roll -= entries[i].weight;
        }
        i++;
    }
}

s32 func_ov030_020c58a0(void) {
    s16 area = gSaveData.unk_2458;
    if ((u32)area >= 41) {
        return 0;
    }
    return data_ov030_020ea394[area];
}

s32 func_ov030_020c58c4(s32 component) {
    u32                                   chapter = gSaveData.chapter;
    const FieldNoiseSequenceEntry* const* chapterTable;
    const FieldNoiseSequenceEntry*        sequence;
    // This normalization deliberately differs from the encounter selector; both
    // arms materialize the table base separately (retail duplicates the pool
    // load into each arm), while the unk_3104 read stays common.
    if (chapter < 23) {
        chapterTable = data_ov030_02108408[chapter];
    } else {
        chapterTable = data_ov030_02108408[chapter - 22];
    }
    sequence = chapterTable[gSaveData.unk_3104];
    if (component == 0) {
        return sequence[func_ov030_020c58a0()].first;
    }
    if (component != 1) {
        return 0;
    }
    return sequence[func_ov030_020c58a0()].second;
}

s16 func_ov030_020c5930(void) {
    s16 sum = 0;
    sum += func_ov030_020c58c4(0);
    sum += func_ov030_020c58c4(1);
    // The retail function really asks for component 2; the accessor returns 0.
    return sum + func_ov030_020c58c4(2);
}

void func_ov030_020c596c(void) {
    u32                                   chapter = gSaveData.chapter;
    const FieldNoiseSequenceEntry* const* sequence;
    s32                                   next;
    if (chapter < 23) {
        sequence = data_ov030_02108408[chapter];
    } else {
        sequence = data_ov030_02108408[chapter - 22];
    }
    next = gSaveData.unk_3104 + 1;
    if (sequence[next] != NULL) {
        gSaveData.unk_3104 = next;
    }
}
