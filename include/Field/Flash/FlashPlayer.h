#ifndef FIELD_FLASH_FLASHPLAYER_H
#define FIELD_FLASH_FLASHPLAYER_H

#include <nitro/types.h>
extern "C" {
#include <nitro/fs/file.h>
}
#include <nitro/fx.h>
#include <string.h>

typedef struct {
    /* 0x00 */ s32 m[2][3];
} FlashMatrix; // Size: 0x18

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} FlashPoint; // Size: 0x4

typedef struct {
    /* 0x0 */ u8 alpha;
    /* 0x1 */ u8 flags;
    /* 0x2 */ u8 mul[3];
    /* 0x5 */ u8 add[3];
} FlashColorTransform; // Size: 0x8

class FlashPlayer;
class FlashManager;
class FlashTexture;

typedef struct FlashHeapNode {
    /* 0x0 */ s32                   state;
    /* 0x4 */ u32                   addr;
    /* 0x8 */ u32                   size;
    /* 0xC */ struct FlashHeapNode* next;
} FlashHeapNode; // Size: 0x10

void* FlashAlloc(u32 size);
void* FlashAllocArray(u32 size);
void  FlashFree(void* ptr);
void  FlashFreeArray(void* ptr);

class FlashAllocated {
public:
    static void* operator new(size_t size) {
        return FlashAlloc(size);
    }
    static void operator delete(void* ptr) {
        FlashFree(ptr);
    }
};

class FlashCharDef {
public:
    FlashCharDef() {
        m_state    = 0;
        m_data     = NULL;
        m_palette  = NULL;
        m_texNode  = NULL;
        m_plttNode = NULL;
        m_texRefs  = 0;
    }
    ~FlashCharDef() {
        if (m_palette != NULL) {
            FlashFreeArray(m_palette);
            m_palette = NULL;
        }
    }

    /* 0x00 */ u16            m_id;
    /* 0x04 */ s32            m_widthLog;
    /* 0x08 */ s32            m_heightLog;
    /* 0x0C */ u8             m_format;
    /* 0x10 */ u32            m_dataSize;
    /* 0x14 */ u16            m_plttCount;
    /* 0x16 */ u16            m_unk_16;
    /* 0x18 */ u16            m_unk_18;
    /* 0x1C */ u8*            m_data;
    /* 0x20 */ u16*           m_palette;
    /* 0x24 */ s32            m_dataOffset;
    /* 0x28 */ s32            m_plttOffset;
    /* 0x2C */ s32            m_state;
    /* 0x30 */ u16            m_texRefs;
    /* 0x34 */ FlashHeapNode* m_texNode;
    /* 0x38 */ FlashHeapNode* m_plttNode;
}; // Size: 0x3C

class FlashStream : public FlashAllocated {
public:
    FlashStream() {
        m_isOpen = FALSE;
    }
    virtual ~FlashStream() {
        if (m_isOpen) {
            Close();
        }
    }
    virtual BOOL Open(const char* path) {
        m_isOpen = TRUE;
        return FALSE;
    }
    virtual BOOL Read(void* buf, u32 size) = 0;
    virtual BOOL Seek(s32 pos)             = 0;
    virtual u32  GetLength()               = 0;
    virtual void Close() {
        m_isOpen = FALSE;
    }
    virtual BOOL IsOpen() {
        return m_isOpen;
    }
    virtual u32 Tell() = 0;

    /* 0x04 */ BOOL m_isOpen;
}; // Size: 0x8

class FlashFileStream : public FlashStream {
public:
    FlashFileStream();
    virtual ~FlashFileStream() {}
    virtual BOOL Open(const char* path);
    virtual BOOL Read(void* buf, u32 size);
    virtual BOOL Seek(s32 pos);
    virtual u32  GetLength();
    virtual void Close();
    virtual BOOL IsOpen() {
        return m_isOpen;
    }
    virtual u32 Tell();

    /* 0x08 */ FS_File m_file;
}; // Size: 0x50

class FlashObject : public FlashAllocated {
public:
    FlashObject();
    virtual ~FlashObject();
    virtual void Draw(const FlashMatrix* mtx, fx32 z, u16 yOffset, u16 polygonId) = 0;
    virtual void OnPlace(FlashPlayer* player) {}
    virtual void Load(FlashPlayer* player) {}
    virtual void Release() {}
    virtual BOOL HasResources() = 0;
    virtual void SetColorTransform(const FlashColorTransform* cxform) {}
    virtual void Reset();

    void SetBounds(s16 x, s16 y, s16 w, s16 h);
    void SetAlpha(u8 alpha) {
        if (alpha != 0xFF) {
            m_alpha = alpha;
        }
    }

    /* 0x04 */ u16                 m_id;
    /* 0x06 */ u16                 m_useCount;
    /* 0x08 */ u8                  m_alpha;
    /* 0x09 */ FlashColorTransform m_cxform;
    /* 0x14 */ fx32                m_x;
    /* 0x18 */ fx32                m_y;
    /* 0x1C */ fx32                m_w;
    /* 0x20 */ fx32                m_h;
    /* 0x24 */ u16*                m_palette;
}; // Size: 0x28

class FlashRect : public FlashObject {
public:
    virtual ~FlashRect() {}
    virtual void Draw(const FlashMatrix* mtx, fx32 z, u16 yOffset, u16 polygonId);
    virtual BOOL HasResources() {
        return FALSE;
    }

    void Init(u16 id, const void* verts, u16 color);

    /* 0x28 */ FlashPoint m_verts[4];
    /* 0x38 */ u16        m_color;
}; // Size: 0x3C

class FlashImage : public FlashObject {
public:
    FlashImage() {
        m_texture = NULL;
    }
    virtual ~FlashImage();
    virtual void Draw(const FlashMatrix* mtx, fx32 z, u16 yOffset, u16 polygonId);
    virtual void OnPlace(FlashPlayer* player);
    virtual void Load(FlashPlayer* player);
    virtual void Release();
    virtual BOOL HasResources() {
        return TRUE;
    }
    virtual void SetColorTransform(const FlashColorTransform* cxform);
    virtual void Reset();

    void Init(u16 id, u16 charId, const FlashMatrix* mtx);

    /* 0x28 */ u16           m_charId;
    /* 0x2C */ FlashTexture* m_texture;
    /* 0x30 */ FlashMatrix   m_mtx;
}; // Size: 0x48

class FlashHeap : public FlashAllocated {
public:
    FlashHeap() {
        m_head = NULL;
        m_free = NULL;
    }

    void           Init(u32 size);
    FlashHeapNode* AllocNode();
    FlashHeapNode* Alloc(u32 size);
    void           Free(FlashHeapNode* node);
    void           FreeNode(FlashHeapNode* node);

    /* 0x000 */ u32            m_size;
    /* 0x004 */ FlashHeapNode  m_nodes[66];
    /* 0x424 */ FlashHeapNode* m_head;
    /* 0x428 */ FlashHeapNode* m_free;
}; // Size: 0x42C

class FlashTexture : public FlashAllocated {
public:
    FlashTexture();
    ~FlashTexture();

    static void InitHeaps();
    static void FreeHeaps();

    void Init(FlashPlayer* player, FlashCharDef* def);
    void Draw(const FlashMatrix* nodeMtx, const FlashMatrix* mtx, fx32 z, u8 alpha, u16 yOffset, const fx32* bounds,
              u16 polygonId);

    /* 0x00 */ FlashCharDef* m_def;
    /* 0x04 */ s32           m_widthLog;
    /* 0x08 */ s32           m_heightLog;
    /* 0x0C */ s32           m_format;
    /* 0x10 */ u16           m_id;
    /* 0x12 */ u16           m_unk_12;
    /* 0x14 */ u16           m_unk_14;
}; // Size: 0x18

typedef struct FlashDisplayNode {
    /* 0x00 */ u16                      depth;
    /* 0x04 */ FlashMatrix              mtx;
    /* 0x1C */ FlashObject*             obj;
    /* 0x20 */ struct FlashDisplayNode* next;
} FlashDisplayNode; // Size: 0x24

class FlashDisplayList {
public:
    FlashDisplayList() {
        m_head = NULL;
        m_free = NULL;
    }
    ~FlashDisplayList();

    void              Clear();
    void              Init();
    FlashDisplayNode* AllocNode();
    void              Insert(FlashObject* obj, u16 depth, const FlashMatrix* mtx);
    void              Modify(u16 depth, u8 alpha, const FlashColorTransform* cxform, const FlashMatrix* mtx);
    void              Remove(u16 id, u16 depth);
    void              RemoveDepth(u16 depth);
    FlashMatrix*      FindMatrix(u16 depth);

    /* 0x0000 */ FlashDisplayNode  m_nodes[258];
    /* 0x2448 */ FlashDisplayNode* m_head;
    /* 0x244C */ FlashDisplayNode* m_free;
}; // Size: 0x2450

class FlashListener : public FlashAllocated {
public:
    FlashListener() {
        m_name = NULL;
    }
    virtual ~FlashListener() {}
    virtual BOOL Matches(const char* name) {
        return strcmp((char*)name, (char*)m_name) == 0;
    }
    virtual void OnEvent(FlashManager* manager, const char* name, const char* arg) {}

    /* 0x4 */ const char* m_name;
}; // Size: 0x8

class FlashFrameHandler : public FlashAllocated {
public:
    virtual ~FlashFrameHandler() {}
    virtual void OnEvent(FlashManager* manager, const char* name, const char* arg) {}
    virtual void OnEvent(FlashManager* manager, s32 frame) {}
}; // Size: 0x4

class FlashEventCallback : public FlashListener {
public:
    FlashEventCallback() {
        m_callback = NULL;
    }
    virtual void OnEvent(FlashManager* manager, const char* name, const char* arg) {
        if (m_callback != NULL) {
            m_callback(name, arg);
        }
    }
    virtual BOOL Matches(const char* name) {
        return TRUE;
    }

    /* 0x8 */ void (*m_callback)(const char* name, const char* arg);
}; // Size: 0xC

class FlashFrameListener : public FlashFrameHandler {
public:
    FlashFrameListener() {
        m_onEvent = NULL;
        m_onFrame = NULL;
    }
    virtual void OnEvent(FlashManager* manager, const char* name, const char* arg) {
        if (m_onEvent != NULL) {
            m_onEvent(name, arg);
        }
    }
    virtual void OnEvent(FlashManager* manager, s32 frame) {
        if (m_onFrame != NULL) {
            m_onFrame(frame);
        }
    }

    /* 0x4 */ void (*m_onEvent)(const char* name, const char* arg);
    /* 0x8 */ void (*m_onFrame)(s32 frame);
}; // Size: 0xC

#ifdef REGION_USA
    #define FLASH_UPLOAD_QUEUE_LEN 32
#else
    #define FLASH_UPLOAD_QUEUE_LEN 16
#endif

class FlashUploadQueue {
public:
    FlashUploadQueue();
    ~FlashUploadQueue();

    void Init();
    void Push(void* src, u32 dst, u32 size, FlashCharDef* def);
    void Process();

    struct Entry {
        /* 0x0 */ void*         src;
        /* 0x4 */ u32           dst;
        /* 0x8 */ u32           size;
        /* 0xC */ FlashCharDef* def;
    };

    /* 0x000 */ Entry m_entries[FLASH_UPLOAD_QUEUE_LEN];
    /* 0x200 */ s16   m_head;
    /* 0x202 */ s16   m_tail;
    /* 0x204 */ s16   m_current;
    /* 0x206 */ u16   m_count;
#ifdef REGION_USA
    /* 0x208 */ u16 m_maxCount;
#endif
    /* 0x20C */ BOOL m_ready;
}; // Size: 0x210 (JP: 0x10C)

class FlashVramLoader : public FlashAllocated {
public:
    FlashVramLoader();
    ~FlashVramLoader();

    void Reset();
    void Load(FlashStream* stream, FlashCharDef* def, u32 texAddr, u32 plttAddr);
    void Update();

    /* 0x000 */ u8               m_transfer[0x100];
    /* 0x100 */ FlashUploadQueue m_queue;
}; // Size: 0x310 (JP: 0x20C)

class FlashParser {
public:
    void Parse(FlashStream* stream, FlashPlayer* player);
    void ParseHeader(FlashStream* stream, FlashPlayer* player);
    void ParseCharDefs(FlashStream* stream, FlashPlayer* player);
    void ParseObjects(FlashStream* stream, FlashPlayer* player);
};

typedef enum FlashPlayerState {
    FLASH_STATE_IDLE    = 0,
    FLASH_STATE_LOADED  = 1,
    FLASH_STATE_PLAYING = 2,
    FLASH_STATE_ENDED   = 3,
} FlashPlayerState;

class FlashPlayer : public FlashAllocated {
public:
    FlashPlayer(FlashManager* manager);
    ~FlashPlayer();

    void          SetSize(u16 width, u16 height);
    void          AllocCharDefs(u16 count);
    void          SetCharDef(u16 index, const FlashCharDef* def);
    FlashCharDef* FindCharDef(u16 id);
    void          SetObject(FlashObject* obj);
    void          SetData(u8* data);
    void          DrawDisplayList(u16 yOffset);
    void          Render();
    void          Preload();
    BOOL          Advance();
    void          RunFrame();
    void          RunPreloadFrame();
    u16           ReadU16();
    u16           ReadPreloadU16();
    u32           ReadU32();
    u32           ReadPreloadU32();
    void          ReadMatrix(FlashMatrix* mtx);
    const char*   ReadString();
    void          TagPlace();
    void          PreloadPlace();
    void          TagPlaceEx();
    void          PreloadPlaceEx();
    void          TagRemove();
    void          PreloadSkip4();
    void          TagRemoveDepth();
    void          PreloadSkip2();
    void          TagSetBgColor();
    void          PreloadSkip2b();
    void          AddListener(FlashManager* manager, FlashListener* listener);
    void          RemoveListener(FlashListener* listener);
    void          TagEvent();
    void          PreloadSkipEvent();
    void          AddFrameListener(FlashManager* manager, FlashFrameListener* listener);
    void          RemoveFrameListener(FlashFrameListener* listener);
    void          TagJump();
    void          PreloadJump();
    BOOL          Load(const char* path);
    void          Rewind();
    BOOL          Play();
    BOOL          IsPlaying();

    /* 0x0000 */ s32                 m_state;
    /* 0x0004 */ s32                 m_unk_04;
    /* 0x0008 */ u16                 m_width;
    /* 0x000A */ u16                 m_height;
    /* 0x000C */ u64                 m_frameTicks;
    /* 0x0014 */ u64                 m_nextTick;
    /* 0x001C */ u16                 m_bgColor;
    /* 0x0020 */ FlashCharDef*       m_charDefs;
    /* 0x0024 */ u16                 m_charDefCount;
    /* 0x0028 */ u8*                 m_data;
    /* 0x002C */ u8*                 m_pc;
    /* 0x0030 */ u8*                 m_preloadPc;
    /* 0x0034 */ u8*                 m_loopPc;
    /* 0x0038 */ FlashObject*        m_objects[256];
    /* 0x0438 */ FlashDisplayList    m_displayList;
    /* 0x2888 */ u16                 m_lastCharId;
    /* 0x288C */ FlashListener*      m_listeners[32];
    /* 0x290C */ u32                 m_listenerCount;
    /* 0x2910 */ FlashManager*       m_manager;
    /* 0x2914 */ u8*                 m_extraData;
    /* 0x2918 */ u16                 m_extraSize;
    /* 0x291C */ FlashFrameListener* m_frameListeners[32];
    /* 0x299C */ u32                 m_frameListenerCount;
    /* 0x29A0 */ FlashStream*        m_stream;
    /* 0x29A4 */ s32                 m_frame;
}; // Size: 0x29A8

class FlashManager : public FlashAllocated {
public:
    FlashManager();
    ~FlashManager();

    static void InitGraphics();

    void Load(const char* path);
    void Update();
    void Draw();
    void VBlank();
    void AddListener(FlashListener* listener);
    void RemoveListener(FlashListener* listener);
    void AddFrameListener(FlashFrameListener* listener);
    void RemoveFrameListener(FlashFrameListener* listener);
    void SetDrawCallback(void (*callback)(void));
    void ClearDrawCallback();
    BOOL Start();
    BOOL IsPlaying();

    /* 0x00 */ FlashPlayer*     m_player;
    /* 0x04 */ FlashVramLoader* m_loader;
    /* 0x08 */ void (*m_drawCallback)(void);
    /* 0x0C */ s32  m_unk_0C;
    /* 0x10 */ BOOL m_drawn;
    /* 0x14 */ BOOL m_updated;
}; // Size: 0x18

#endif // FIELD_FLASH_FLASHPLAYER_H
