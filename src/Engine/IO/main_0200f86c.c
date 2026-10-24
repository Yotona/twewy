#include "Engine/IO/Str.h"

#define STRLIST_MAX_LEN 0x100

typedef struct StrListNode {
    /* 0x00 */ struct StrListNode* next;
    /* 0x04 */ u8                  unk_04[0x18];
    /* 0x1C */ const char*         str;
} StrListNode;

typedef struct {
    /* 0x00 */ u8           unk_00[0x20];
    /* 0x20 */ StrListNode* listA;
    /* 0x24 */ u8           unk_24[0x1C];
    /* 0x40 */ StrListNode* listB;
    /* 0x44 */ u8           unk_44[0x1C];
    /* 0x60 */ StrListNode* listC;
} StrListOwner;

void func_0200f86c(StrListOwner* owner) {
    StrListNode* node;

    for (node = owner->listB; node != NULL; node = node->next) {
        if (node->str != NULL) {
            Str_Validate(node->str, STRLIST_MAX_LEN);
        }
    }
}

void func_0200f8a4(StrListOwner* owner) {
    StrListNode* node;

    for (node = owner->listA; node != NULL; node = node->next) {
    }
}

void func_0200f8c0(StrListOwner* owner) {
    StrListNode* node;

    for (node = owner->listC; node != NULL; node = node->next) {
        if (node->str != NULL) {
            Str_Validate(node->str, STRLIST_MAX_LEN);
        }
    }
}

void func_0200f8f8(StrListOwner* owner) {
    func_0200f86c(owner);
    func_0200f8a4(owner);
    func_0200f8c0(owner);
}

s32 func_0200f918(StrListNode* node) {
    s32 count = 0;

    for (; node != NULL; node = node->next) {
        count++;
    }
    return count;
}

s32 func_0200f93c(StrListOwner* owner) {
    return func_0200f918(owner->listB);
}
