#ifndef ENGINE_CORE_OAMMANAGER_H
#define ENGINE_CORE_OAMMANAGER_H

#include "Display.h"
#include <types.h>

/**
 * @brief 2D vertex with s16 x/y coordinates.
 *
 * Used for quad vertex positions sent to the 3D engine via GFX_FIFO_VERTEX_16.
 */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} VtxXY; // Size: 0x4

/**
 * @brief 2D texture coordinate pair (s, t).
 */
typedef struct {
    /* 0x0 */ u32 s;
    /* 0x4 */ u32 t;
} TexCoordST; // Size: 0x8

/**
 * @brief Quad texture coordinate set for 3D sprite rendering.
 *
 * Contains 4 (s, t) texture coordinate pairs, one for each vertex
 * of a textured quad. Indexed by flip state (0-3).
 */
typedef struct {
    /* 0x00 */ TexCoordST vtx[4];
} TexCoordQuad; // Size: 0x20

/**
 * @brief Sprite cell piece descriptor.
 *
 * Defines one piece of a multi-piece sprite cell. Arrays of these
 * descriptors define all pieces; terminated by charName == 0xFFFF.
 */
typedef struct {
    /* 0x0 */ u16 charName; ///< Character/tile index (0xFFFF = terminator)
    /* 0x2 */ u16 attr0;    ///< Y position, shape, color mode
    /* 0x4 */ u16 attr1;    ///< X position, size, flip flags
    /* 0x6 */ u16 attr2;    ///< Palette number, priority
} OamCellPiece;             // Size: 0x8

/**
 * @brief Tile mapping configuration for OBJ character data.
 *
 * Describes how OBJ tiles are mapped for a given mapping mode.
 * charBases[] in OamManager points to these.
 */
typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 tileShift; ///< Right-shift applied to tile indices
    /* 0x0C */ s32 unk_0C;
} OamCharConfig;              // Size: 0x10

/**
 * @brief Shape dimensions for an OBJ shape/size combination.
 *
 * Describes pixel dimensions and rendering parameters for one of the
 * NDS OBJ shape/size combinations. Indexed from data_020593c0.
 */
typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 width;    ///< Pixel width
    /* 0x0C */ s32 height;   ///< Pixel height
    /* 0x10 */ s32 texSizeS; ///< Texture S dimension parameter
    /* 0x14 */ s32 texSizeT; ///< Texture T dimension parameter
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
} OamCellShape; // Size: 0x20

/**
 * @brief Cell frame info for 3D sprite rendering.
 *
 * Contains dimensions and texture parameters for rendering a sprite cell
 * as a textured quad via the 3D engine. Stride 0x18.
 */
typedef struct {
    /* 0x00 */ u32 width;    ///< Frame width
    /* 0x04 */ u32 height;   ///< Frame height
    /* 0x08 */ s32 centerX;  ///< X center offset
    /* 0x0C */ s32 centerY;  ///< Y center offset
    /* 0x10 */ s32 texSizeS; ///< Texture S size parameter
    /* 0x14 */ s32 texSizeT; ///< Texture T size parameter
} OamCellFrame;              // Size: 0x18

/**
 * @brief NDS hardware OAM (Object Attribute Memory) entry.
 *
 * Each entry describes one hardware sprite. 128 entries exist per engine.
 * Affine parameters are interleaved at the affineParam field of every 4th entry group.
 */
typedef struct {
    /* 0x0 */ u16 attr0;       ///< Y position, OBJ mode, mosaic, color mode, shape
    /* 0x2 */ u16 attr1;       ///< X position, flip flags or affine index, size
    /* 0x4 */ u16 attr2;       ///< Tile index, priority, palette number
    /* 0x6 */ s16 affineParam; ///< Rotation/scaling parameter (PA/PB/PC/PD interleaved)
} OamAttr;                     // Size: 0x8

/**
 * @brief Stored affine transformation parameters for one affine group.
 */
typedef struct {
    /* 0x00 */ s32 rotation; ///< Rotation angle
    /* 0x04 */ s32 scaleX;   ///< Horizontal scale
    /* 0x08 */ s32 scaleY;   ///< Vertical scale
    /* 0x0C */ s32 unk_0C;   ///< Unused / padding
} OamAffineParam;            // Size: 0x10

/**
 * @brief Sprite render command node.
 *
 * Used in both BST-sorted and linked-list-sorted priority command queues.
 */
typedef struct OamSpriteCmd {
    /* 0x00 */ struct OamSpriteCmd* left;       ///< BST left child / alternate link
    /* 0x04 */ struct OamSpriteCmd* right;      ///< BST right child / linked list next
    /* 0x08 */ u32                  sortKey;    ///< Priority sort key
    /* 0x0C */ OamCellPiece*        spriteData; ///< Pointer to sprite cell piece data
} OamSpriteCmd;                                 // Size: 0x10

/**
 * @brief Priority group for ordering sprite render commands.
 *
 * Each OAM priority level (0-3) has one group. Supports both insertion-sorted
 * linked lists and BST-sorted command ordering.
 */
typedef struct {
    /* 0x00 */ s32           mode;       ///< Non-zero = use insertion-sorted linked list
    /* 0x04 */ OamSpriteCmd* sortedRoot; ///< BST root for sorted insertion
    /* 0x08 */ OamSpriteCmd* sentinel;   ///< Linked list sentinel (self-referencing)
    /* 0x0C */ OamSpriteCmd* head;       ///< First entry (sentinel->next)
    /* 0x10 */ u8            _pad[8];    ///< Padding to 0x18 stride
} OamPriorityGroup;                      // Size: 0x18

/**
 * @brief OAM Manager for one display engine.
 *
 * Manages hardware sprite attributes, affine transformations, and
 * priority-ordered sprite render commands for a single NDS display engine.
 * Three instances exist: main, sub, and extended.
 */
typedef struct {
    /* 0x0000 */ s32              engine;          ///< Display engine ID
    /* 0x0004 */ s32              type;            ///< Engine type (7=main/sub, 1=extended)
    /* 0x0008 */ OamCharConfig*   charBases[4];    ///< Character data base pointers
    /* 0x0018 */ s32              oamCount;        ///< Next free OAM entry index
    /* 0x001C */ s32              affineCount;     ///< Next free affine group index
    /* 0x0020 */ OamAttr          oam[128];        ///< Hardware OAM attribute buffer
    /* 0x0420 */ OamAffineParam   affine[32];      ///< Affine transformation storage
    /* 0x0620 */ s32              renderMode;      ///< Render dispatch table index
    /* 0x0624 */ s32              cmdCount;        ///< Active sprite render commands
    /* 0x0628 */ OamPriorityGroup prioGroups[4];   ///< Priority-based command groups
    /* 0x0688 */ OamSpriteCmd     cmds[128];       ///< Sprite command pool
    /* 0x0E88 */ OamSpriteCmd*    sortBuffer[129]; ///< Sort/heap buffer (1-indexed)
} OamManager;                                      // Size: 0x108C

extern OamManager g_OamMgr[3];

/**
 * @brief Initialize an engine-specific OAM manager configuration.
 *
 * Selects tile/bmp mapping settings from display configuration and updates
 * the corresponding OamManager instance for the selected display engine.
 *
 * @param unused  Unused parameter (artifact from original calling convention).
 * @param engine  Display engine to initialize (DISPLAY_MAIN, DISPLAY_SUB, or DISPLAY_EXTENDED).
 */
void OamMgr_Init(s32 unused, DisplayEngine engine);

/**
 * @brief Reset OAM and affine usage counters for a manager.
 *
 * Also initializes remaining OAM/affine entries to default hidden/identity values.
 *
 * @param mgr                OAM manager to reset.
 * @param initialOamCount    First OAM entry to hide (entries [initialOamCount..127] are disabled).
 * @param initialAffineCount First affine group to reset (groups [initialAffineCount..31] are set to identity).
 */
void OamMgr_Reset(OamManager* mgr, u32 initialOamCount, u32 initialAffineCount);

/**
 * @brief Set current affine group count and reset remaining affine params.
 *
 * @param mgr   OAM manager to update.
 * @param count Next free affine group index; groups [count..31] are reset to identity.
 */
void OamMgr_SetAffineCount(OamManager* mgr, u32 count);

/**
 * @brief Append a transformed cell-piece list directly into the OAM buffer.
 *
 * @param mgr        OAM manager to write into.
 * @param x          Screen X offset added to each piece's X position.
 * @param y          Screen Y offset added to each piece's Y position.
 * @param cellPieces Null-terminated array of cell piece descriptors to append.
 * @param attrs      Packed sprite attribute flags (flip bits, palette bank, etc.).
 * @param charOffset Base tile/character offset added to each piece's tile index.
 */
void OamMgr_AddCellToOam(OamManager* mgr, s32 x, s32 y, OamCellPiece* cellPieces, u16 attrs, s32 charOffset);

/**
 * @brief Copy a prebuilt cell-piece list into OAM entries.
 *
 * @param mgr        OAM manager whose hardware OAM buffer is written.
 * @param cellPieces Null-terminated array of cell piece descriptors to copy.
 */
void OamMgr_CopyCellPiecesToOam(OamManager* mgr, OamCellPiece* cellPieces);

/**
 * @brief Allocate and configure one affine transform group.
 *
 * @param mgr       OAM manager to allocate from.
 * @param rotation  Rotation angle (fixed-point, same unit as OAM hardware).
 * @param scaleX    Horizontal scale factor (fixed-point 1.0 = 0x1000).
 * @param scaleY    Vertical scale factor (fixed-point 1.0 = 0x1000).
 * @param flipFlags Bit 0 = negate scaleX, bit 1 = negate scaleY.
 * @return          Allocated affine group index, or 0 if the pool is full.
 */
s32 OamMgr_AllocAffineGroup(OamManager* mgr, u16 rotation, s32 scaleX, s32 scaleY, s32 flipFlags);

/**
 * @brief Resolve shape/size metadata for a single cell piece.
 *
 * @param piece Cell piece whose attr0/attr1 shape and size fields are decoded.
 * @return      Pointer to the matching OamCellShape entry describing pixel dimensions
 *              and texture parameters for that shape/size combination.
 */
OamCellShape* OamMgr_GetCellShape(OamCellPiece* piece);

/**
 * @brief Build a temporary visible-only cell-piece list after 2D transform/cull.
 *
 * Iterates the source piece array, applies the position offset and flip flags,
 * culls pieces outside the screen (256~192), and writes survivors into a
 * scratch-allocated output list.
 *
 * @param mgr        OAM manager providing char-base configuration.
 * @param x          Screen X origin added to each piece's X coordinate.
 * @param y          Screen Y origin added to each piece's Y coordinate.
 * @param cellPieces Source piece array (terminated by charName == 0xFFFF).
 * @param attrs      Packed attribute flags: flip bits, palette bank, affine flag.
 * @param charOffset Base character/tile offset added to each piece's tile index.
 * @return           Pointer to the newly allocated visible piece list, or the
 *                   shared empty-sentinel if all pieces were culled.
 */
OamCellPiece* OamMgr_BuildVisibleCellPieces(OamManager* mgr, s32 x, s32 y, OamCellPiece* cellPieces, u16 attrs,
                                            s32 charOffset);

/**
 * @brief Build a temporary visible-only cell-piece list using affine transform.
 *
 * Applies the given rotation/scale matrix to each piece center before culling,
 * then writes survivors into a scratch-allocated output list.
 *
 * @param mgr        OAM manager providing char-base configuration.
 * @param x          Screen X origin of the sprite.
 * @param y          Screen Y origin of the sprite.
 * @param cellPieces Source piece array (terminated by charName == 0xFFFF).
 * @param attrs      Packed attribute flags; bit 0 must be set (affine mode).
 * @param charOffset Base character/tile offset added to each piece's tile index.
 * @param transform  Pointer to a rotation/scale parameter block (OamAffineParam-compatible);
 *                   pass NULL to use a default identity-like transform.
 * @return           Pointer to the newly allocated visible piece list, or the
 *                   shared empty-sentinel if all pieces were culled.
 */
OamCellPiece* OamMgr_BuildVisibleCellPiecesAffine(OamManager* mgr, s32 x, s32 y, OamCellPiece* cellPieces, u16 attrs,
                                                  s32 charOffset, void* transform);

/**
 * @brief Queue character/tile transfer work needed by marked cell pieces.
 *
 * Iterates pieces whose charName has bit 15 set, indicating pending VRAM
 * transfer, and submits the corresponding DMA work.
 *
 * @param mgr          OAM manager providing engine-type context.
 * @param transferBase Base destination address for VRAM character transfers.
 * @param charOffset   Running character offset within the destination bank.
 * @param charDataBase Base source address of character data to transfer.
 * @param cellPieces   Source piece array (terminated by charName == 0xFFFF).
 */
void OamMgr_QueueCellCharTransfers(OamManager* mgr, s32 transferBase, s32 charOffset, s32 charDataBase,
                                   OamCellPiece* cellPieces);

/**
 * @brief Clone or patch cell-piece tile indices into a destination list.
 *
 * If @p dstCellPieces is NULL, a new temporary list is scratch-allocated and
 * fully populated from @p srcCellPieces. Otherwise only charName fields are
 * updated in the existing destination list.
 *
 * @param dstCellPieces Destination piece array, or NULL to allocate a new one.
 * @param srcCellPieces Source piece array (terminated by charName == 0xFFFF).
 * @return              Pointer to the destination list (allocated or provided),
 *                      or the shared empty-sentinel if the source was empty and
 *                      a new list was allocated.
 */
OamCellPiece* OamMgr_CloneCellPieces(OamCellPiece* dstCellPieces, OamCellPiece* srcCellPieces);

/**
 * @brief Clear all command queues/trees used for sprite submission.
 *
 * Resets the command count, reinitializes linked-list sentinels for all four
 * priority groups, and clears their BST roots.
 *
 * @param mgr OAM manager whose command state is cleared.
 */
void OamMgr_ResetCommandQueues(OamManager* mgr);

/**
 * @brief Submit one sprite command using the active render-mode strategy.
 *
 * Dispatches to the heap-sort, priority-list, or priority-tree submit
 * implementation according to mgr->renderMode.
 *
 * @param mgr        OAM manager to submit the command to.
 * @param sortKey    Priority/depth key used to order this sprite relative to others.
 * @param spriteData Prebuilt visible cell-piece list for this sprite.
 */
void OamMgr_SubmitCommand(OamManager* mgr, u32 sortKey, OamCellPiece* spriteData);

/**
 * @brief Flush all queued sprite commands to OAM using active render mode.
 *
 * Dispatches to the appropriate flush implementation (heap-sort, priority-list,
 * or priority-tree) and copies all visible cell pieces into the hardware OAM buffer.
 *
 * @param mgr OAM manager whose pending sprite commands are flushed.
 */
void OamMgr_FlushCommands(OamManager* mgr);

/**
 * @brief Configure 3D pipeline state used by 3D-backed sprite rendering.
 */
void OamMgr_Init3DSpritePipeline(void);

/**
 * @brief Reset 3D sprite renderer transient state.
 */
void OamMgr_Reset3DState(void);

/**
 * @brief Request buffer swap for the 3D engine.
 */
void OamMgr_Swap3DBuffers(void);

#endif // ENGINE_CORE_OAMMANAGER_H
