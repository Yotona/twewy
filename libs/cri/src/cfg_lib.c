#include <stddef.h>
#include <string.h>

// CRICFG: a name -> value table of library settings (cricfg/cfg_lib.c in the
// Wii tree). ADXM_SetupThrd reads "IMASK_LVL" from it. Nothing in this binary
// installs a table, so the setter was dead-stripped.

typedef struct {
    char name[12];
    int  value;
} CRICFG_ITEM;

int          data_020713ac;
CRICFG_ITEM* data_020713b0;

CRICFG_ITEM* func_0201d478(const char* name) {
    int          i;
    int          num;
    CRICFG_ITEM* item;

    if (name[0] == '\0') {
        return NULL;
    }
    num  = data_020713ac;
    item = data_020713b0;
    for (i = 0; i < num; i++, item++) {
        if (strncmp(item->name, (char*)name, 12) == 0) {
            return item;
        }
    }
    return NULL;
}

int CRICFG_Read(const char* name, int* value) {
    CRICFG_ITEM* item;

    if (data_020713b0 == NULL) {
        return -1;
    }
    item = func_0201d478(name);
    if (item == NULL) {
        return -3;
    }
    *value = item->value;
    return 0;
}
