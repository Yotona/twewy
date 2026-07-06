#ifndef SAVE_H
#define SAVE_H

#include "Save/FriendData.h"
#include "Save/MainData.h"

typedef struct SaveFooter {
    /* 0x00 */ u8  magic[27];  // Magic signature "SubarAsikiKonosEkai070613b" used to validate the save file
    /* 0x1B */ u8  nibbles[4]; // Nibble values used for additional validation, typically encoding the size of the save data
    /* 0x1F */ u8  _pad;
    /* 0x20 */ u16 checksum;   // Checksum of the save data, used to verify integrity
    /* 0x22 */ u8  reserved[2];
} SaveFooter;                  // Size: 0x24

typedef struct {
    /* 0x0000 */ MainData   mainData;
    /* 0x3420 */ SaveFooter footer;
} MainSaveImage; // Size: 0x3444

typedef struct {
    /* 0x0000*/ GlobalFriendData friendData;
    /* 0x2008 */ SaveFooter      footer;
} FriendSaveImage; // Size: 0x202C

struct SaveState {
    /* 0x00 */ u16               savePipelineStepIndex; // Current step in save pipeline
    /* 0x02 */ u16               saveIoErrorFlags;
    /* 0x04 */ GlobalFriendData* globalFriendData;      // Heap buffer used for backup data of registered friends
    /* 0x08 */ s32               unk_08;
    /* 0x0C */ s32               lockID;
    /* 0x10 */ FriendSaveImage*  friendImage;
    /* 0x14 */ MainSaveImage*    mainImage;
    /* 0x18 */ s64               unk_18;
    /* 0x20 */ MainData          unk_20;
};

extern struct SaveState gSaveState;
extern MainData         gSaveData;

/**
 * @brief Execute the current step in the save pipeline.
 *
 * @return The result of the save pipeline step.
 */
s32 Savefile_RunSavePipelineStep(void);

/**
 * @brief Execute the current save step from an alternate pipeline.
 *
 * @return The result of the alternate save pipeline step.
 */
s32 Savefile_RunAlternatePipelineStep(void);

/**
 * @brief Reset all gameplay-related data to their initial state. This includes all player and friend stats, all pins, all
 * equipment, all story progress, and all collections.
 */
void Savefile_ResetAllGameplay(MainData* arg0);

#endif // SAVE_H