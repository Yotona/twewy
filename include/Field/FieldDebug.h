#ifndef FIELD_FIELDDEBUG_H
#define FIELD_FIELDDEBUG_H

#include "Engine/Resources/ResourceMgr.h"
#include "Engine/Text.h"

#include <nitro/types.h>

// Shared declarations for the field related debug menu sequences: FieldSelect.c ("Seq_FldSel()"),
// FieldFight.c and EventSelect.c ("Seq_EventSel()").

// One selectable event inside an EventSelectCategory.
typedef struct {
    /* 0x0 */ s16         eventId;
    /* 0x2 */ s16         unk_2;
    /* 0x4 */ const char* name;
} EventSelectEntry; // Size: 0x8

// One row of the event select menu: a named list of events.
typedef struct {
    /* 0x0 */ const EventSelectEntry* entries;
    /* 0x4 */ s16                     count;
    /* 0x8 */ const char*             name;
} EventSelectCategory; // Size: 0xC

#define EVENT_SELECT_CATEGORY_COUNT 51

typedef struct {
    /* 0x00000 */ ResourceManager  resources;
    /* 0x11580 */ ResourceManager* prevResources;
    /* 0x11584 */ s32              unk_11584;
    /* 0x11588 */ char             pad_11588[0x1000C];
    /* 0x21594 */ s32              unk_21594;
    /* 0x21598 */ char             pad_21598[0x8];
    /* 0x215A0 */ TextObject       text;
    /* 0x216B4 */ char             pad_216B4[0x300];
    /* 0x219B4 */ s32              unk_219B4;
    /* 0x219B8 */ s16              unk_219B8;
    /* 0x219BA */ char             pad_219BA[0x2];
} FieldSelectObject; // Size: 0x219BC

typedef struct {
    /* 0x00000 */ ResourceManager  resources;
    /* 0x11580 */ ResourceManager* prevResources;
    /* 0x11584 */ s32              unk_11584;
    /* 0x11588 */ char             pad_11588[0x1000C];
    /* 0x21594 */ s32              unk_21594;
    /* 0x21598 */ char             pad_21598[0x8];
    /* 0x215A0 */ TextObject       text;
    /* 0x216B4 */ char             pad_216B4[0x300];
} EventSelectObject; // Size: 0x219B4

void func_ov030_020b20c0(void);

extern void func_ov030_020ae92c(void);
extern void func_ov030_020c26ec(void);

#endif // FIELD_FIELDDEBUG_H
