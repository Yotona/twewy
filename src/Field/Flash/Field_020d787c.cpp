#include "Field/FlashRegion.h"

extern "C" {
#include "Engine/Core/Memory.h"
#include <nitro/mi/cpumem.h>
#include <nitro/os/cache.h>
}
#include "Field/FlashPlayer.h"

extern "C" {
void Interrupts_ForceVBlank(void);
u64  func_0203a444(void);
void func_02028084(void* transfer, s32 count);
void func_020280a8(void* transfer);
void func_02028108(s32 type, u32 dst, const void* src, u32 size);
void func_0203b928(void* context, void* dest, const void* header);
s32  func_0203b958(void* context, const void* src, u32 size);
}

FlashManager::FlashManager() :
    m_player(NULL), m_drawCallback(NULL), m_unk_0C(0), m_drawn(TRUE), m_updated(TRUE) {
    m_loader = new FlashVramLoader();
}

FlashManager::~FlashManager() {
    if (m_player != NULL) {
        delete m_player;
        m_player = NULL;
    }
    delete m_loader;
    m_loader = NULL;
}

void FlashManager::InitGraphics() {
    func_ov030_020d5a2c();
    FlashTexture::InitHeaps();
}

void FlashManager::Load(const char* path) {
    if (m_player != NULL) {
        delete m_player;
    }
    m_player = new FlashPlayer(this);
    m_player->Load(path);
}

void FlashManager::Update() {
    m_updated = FALSE;
    m_player->Advance();
    m_updated = TRUE;
}

void FlashManager::Draw() {
    Interrupts_ForceVBlank();
    m_drawn = FALSE;
    m_player->Render();
    m_drawn = TRUE;
    if (m_drawCallback != NULL) {
        m_drawCallback();
    }
}

void FlashManager::VBlank() {
    func_ov030_020d5c90();
#ifdef REGION_USA
    if (m_updated && m_drawn && IsPlaying() == TRUE) {
#else
    if (IsPlaying() == TRUE) {
#endif
        m_loader->Update();
    }
}

void FlashManager::AddListener(FlashListener* listener) {
    if (m_player != NULL) {
        m_player->AddListener(this, listener);
    }
}

void FlashManager::RemoveListener(FlashListener* listener) {
    if (m_player != NULL) {
        m_player->RemoveListener(listener);
    }
}

void FlashManager::AddFrameListener(FlashFrameListener* listener) {
    if (m_player != NULL) {
        m_player->AddFrameListener(this, listener);
    }
}

void FlashManager::RemoveFrameListener(FlashFrameListener* listener) {
    if (m_player != NULL) {
        m_player->RemoveFrameListener(listener);
    }
}

void FlashManager::SetDrawCallback(void (*callback)(void)) {
    m_drawCallback = callback;
}

void FlashManager::ClearDrawCallback() {
    m_drawCallback = NULL;
}

BOOL FlashManager::Start() {
    return m_player->Play();
}

BOOL FlashManager::IsPlaying() {
    if (m_player == NULL) {
        return FALSE;
    }
    return m_player->IsPlaying();
}

FlashPlayer::FlashPlayer(FlashManager* manager) :
    m_state(FLASH_STATE_IDLE),
    m_unk_04(0),
    m_nextTick(0),
    m_charDefs(NULL),
    m_charDefCount(0),
    m_data(NULL),
    m_pc(NULL),
    m_preloadPc(NULL),
    m_loopPc(NULL),
    m_lastCharId(0),
    m_listenerCount(0),
    m_extraData(NULL),
    m_extraSize(0),
    m_frameListenerCount(0),
    m_stream(NULL) {
    s32 i;

    m_width   = 256;
    m_height  = 384;
    m_manager = manager;
    for (i = 0; i < 256; i++) {
        m_objects[i] = NULL;
    }
    m_displayList.Clear();
}

FlashPlayer::~FlashPlayer() {
    s32 i;

    if (m_stream != NULL) {
        if (m_stream->IsOpen()) {
            m_stream->Close();
        }
        delete m_stream;
        m_stream = NULL;
    }
    for (i = 0; i < 256; i++) {
        if (m_objects[i] != NULL) {
            if (m_objects[i]->HasResources() == TRUE) {
                m_objects[i]->Release();
            }
            delete m_objects[i];
            m_objects[i] = NULL;
        }
    }
    if (m_charDefs != NULL) {
        delete[] m_charDefs;
        m_charDefs = NULL;
    }
    FlashFreeArray(m_data);
    m_data = NULL;
}

void FlashPlayer::SetSize(u16 width, u16 height) {
    m_width  = width;
    m_height = height;
}

void FlashPlayer::AllocCharDefs(u16 count) {
    m_charDefCount = count;
    if (count != 0) {
        m_charDefs = new FlashCharDef[count];
    } else {
        m_charDefs = NULL;
    }
}

void FlashPlayer::SetCharDef(u16 index, const FlashCharDef* def) {
    m_charDefs[index] = *def;
}

FlashCharDef* FlashPlayer::FindCharDef(u16 id) {
    FlashCharDef* def = m_charDefs;
    u16           i;

    for (i = 0; i < m_charDefCount; def++, i++) {
        if (def->m_id == id) {
            return def;
        }
    }
    return NULL;
}

void FlashPlayer::SetObject(FlashObject* obj) {
    u16 id = obj->m_id;

    if (m_objects[id] != NULL) {
        delete m_objects[id];
    }
    m_objects[id] = obj;
}

void FlashPlayer::SetData(u8* data) {
    m_data      = data;
    m_pc        = data;
    m_preloadPc = data;
}

void FlashPlayer::Preload() {
    s32 i;

    for (i = 0; i < 2; i++) {
        RunPreloadFrame();
    }
}

BOOL FlashPlayer::Advance() {
    u64 now;
    u16 i;

    switch (m_state) {
        case FLASH_STATE_IDLE:
        case FLASH_STATE_LOADED:
        case FLASH_STATE_ENDED:
            m_displayList.Init();
            return FALSE;
        case FLASH_STATE_PLAYING:
            break;
    }
    now = func_0203a444();
    if (m_nextTick == 0 || now >= m_nextTick) {
        if (m_nextTick != 0) {
            m_nextTick += m_frameTicks;
        } else {
            m_nextTick = now + m_frameTicks;
        }
    } else {
        return FALSE;
    }
    if (*m_pc == 0) {
        m_state = FLASH_STATE_ENDED;
        return FALSE;
    }
    for (i = 0; i < 256; i++) {
        FlashObject* obj = m_objects[i];

        if (obj != NULL && obj->m_useCount == 0 && obj->HasResources() == TRUE) {
            obj->Release();
        }
    }
    RunPreloadFrame();
    RunFrame();
    return TRUE;
}

void FlashPlayer::RunFrame() {
    while (*m_pc != 0) {
        switch (*m_pc++) {
            case 0:
                m_pc--;
                m_state = FLASH_STATE_ENDED;
                return;
            case 1:
                TagPlace();
                break;
            case 2:
                TagPlaceEx();
                break;
            case 3:
                TagRemove();
                break;
            case 4:
                TagRemoveDepth();
                break;
            case 5:
                return;
            case 6:
                TagSetBgColor();
                break;
            case 7:
                TagEvent();
                break;
            case 8:
                TagJump();
                break;
            case 12:
                m_pc = m_loopPc;
                break;
            case 13:
                break;
        }
    }
}

void FlashPlayer::RunPreloadFrame() {
    while (*m_preloadPc != 0) {
        switch (*m_preloadPc++) {
            case 0:
                m_preloadPc--;
                return;
            case 1:
                PreloadPlace();
                break;
            case 2:
                PreloadPlaceEx();
                break;
            case 3:
                PreloadSkip4();
                break;
            case 4:
                PreloadSkip2();
                break;
            case 5:
                if (m_frame >= 0) {
                    m_frame++;
                }
                return;
            case 6:
                PreloadSkip2b();
                break;
            case 7:
                PreloadSkipEvent();
                break;
            case 8:
                PreloadJump();
                break;
            case 12:
                break;
            case 13:
                break;
        }
    }
}

u16 FlashPlayer::ReadU16() {
    u8* pc    = m_pc;
    u16 value = pc[0] | (pc[1] << 8);

    m_pc = pc + 2;
    return value;
}

u16 FlashPlayer::ReadPreloadU16() {
    u8* pc    = m_preloadPc;
    u16 value = pc[0] | (pc[1] << 8);

    m_preloadPc = pc + 2;
    return value;
}

u32 FlashPlayer::ReadU32() {
    u8* pc    = m_pc;
    u32 value = pc[0] | (pc[1] << 8) | (pc[2] << 16) | (pc[3] << 24);

    m_pc = pc + 4;
    return value;
}

u32 FlashPlayer::ReadPreloadU32() {
    u8* pc    = m_preloadPc;
    u32 value = pc[0] | (pc[1] << 8) | (pc[2] << 16) | (pc[3] << 24);

    m_preloadPc = pc + 4;
    return value;
}

void FlashPlayer::ReadMatrix(FlashMatrix* mtx) {
    s32  i;
    s32  j;
    s32* dst = &mtx->m[0][0];

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            *dst++ = m_pc[0] | (m_pc[1] << 8) | (m_pc[2] << 16) | (m_pc[3] << 24);
            m_pc += 4;
        }
    }
}

const char* FlashPlayer::ReadString() {
    const char* str = (const char*)m_pc;

    m_pc += strlen((char*)str) + 1;
    return str;
}

void FlashPlayer::TagPlace() {
    FlashMatrix mtx;
    u16         id    = ReadU16();
    u16         depth = ReadU16();

    m_pc++;
    ReadMatrix(&mtx);
    m_displayList.Insert(m_objects[id], depth, &mtx);
    m_objects[id]->OnPlace(this);
}

void FlashPlayer::PreloadPlace() {
    u16 id = ReadPreloadU16();

    m_objects[id]->m_useCount++;
    m_objects[id]->Load(this);
    m_preloadPc += sizeof(u16);
    m_preloadPc += sizeof(u8);
    m_preloadPc += sizeof(FlashMatrix);
}

// Nonmatching: regalloc, this in sl instead of r5
void FlashPlayer::TagPlaceEx() {
    FlashColorTransform cxform;
    FlashMatrix         mtx;
    BOOL                hasCxform;
    u8                  flags;
    u16                 depth;
    u8                  alpha;
    BOOL                hasMatrix;
    u16                 id;

    cxform.alpha = alpha = 0xFF;
    flags                = *m_pc++;
    hasMatrix            = FALSE;
    hasCxform            = FALSE;
    depth                = ReadU16();
    if (flags & 2) {
        id           = ReadU16();
        m_lastCharId = id;
    } else {
        id = m_lastCharId;
    }
    if (flags & 4) {
        ReadMatrix(&mtx);
        hasMatrix = TRUE;
    }
    if (flags & 8) {
        u8 data;

        hasCxform    = TRUE;
        data         = *m_pc++;
        cxform.flags = data & 0xC0;
        cxform.alpha = alpha = data & 0x1F;
        if (cxform.flags & 0x80) {
            cxform.mul[0] = *m_pc++;
            cxform.mul[1] = *m_pc++;
            cxform.mul[2] = *m_pc++;
        } else {
            cxform.mul[0] = 31;
            cxform.mul[1] = 31;
            cxform.mul[2] = 31;
        }
        if (cxform.flags & 0x40) {
            cxform.add[0] = *m_pc++;
            cxform.add[1] = *m_pc++;
            cxform.add[2] = *m_pc++;
        } else {
            cxform.add[0] = 0;
            cxform.add[1] = 0;
            cxform.add[2] = 0;
        }
    }
    flags &= 3;
    if (flags == 3) {
        if (!hasMatrix) {
            MI_CpuCopyU8(m_displayList.FindMatrix(depth), &mtx, sizeof(FlashMatrix));
        }
        m_displayList.RemoveDepth(depth);
        m_objects[id]->SetAlpha(alpha);
        if (hasCxform) {
            m_objects[id]->SetColorTransform(&cxform);
        }
        m_displayList.Insert(m_objects[id], depth, &mtx);
        m_objects[id]->OnPlace(this);
    }
    if (flags == 1) {
        m_displayList.Modify(depth, alpha, hasCxform ? &cxform : NULL, hasMatrix ? &mtx : NULL);
    }
    if (flags == 2) {
        m_objects[id]->SetAlpha(alpha);
        if (hasCxform) {
            m_objects[id]->SetColorTransform(&cxform);
        }
        m_displayList.Insert(m_objects[id], depth, &mtx);
        m_objects[id]->OnPlace(this);
    }
}

void FlashPlayer::PreloadPlaceEx() {
    u8  flags;
    u16 id = 0;

    flags = *m_preloadPc++;
    m_preloadPc += sizeof(u16);
    if (flags & 2) {
        id = ReadPreloadU16();
    }
    if (flags & 4) {
        m_preloadPc += sizeof(FlashMatrix);
    }
    if (flags & 8) {
        u8 data = *m_preloadPc++;

        if (data & 0x80) {
            m_preloadPc += 3;
        }
        if (data & 0x40) {
            m_preloadPc += 3;
        }
    }
    flags &= 3;
    if (flags == 3) {
        m_objects[id]->m_useCount++;
        m_objects[id]->Load(this);
    }
    if (flags == 2) {
        m_objects[id]->m_useCount++;
        m_objects[id]->Load(this);
    }
}

void FlashPlayer::TagRemove() {
    u16 id    = ReadU16();
    u16 depth = ReadU16();

    m_displayList.Remove(id, depth);
}

void FlashPlayer::PreloadSkip4() {
    m_preloadPc += sizeof(u16);
    m_preloadPc += sizeof(u16);
}

void FlashPlayer::TagRemoveDepth() {
    m_displayList.RemoveDepth(ReadU16());
}

void FlashPlayer::PreloadSkip2() {
    m_preloadPc += sizeof(u16);
}

void FlashPlayer::TagSetBgColor() {
    m_bgColor = ReadU16();
}

void FlashPlayer::PreloadSkip2b() {
    m_preloadPc += sizeof(u16);
}

void FlashPlayer::AddListener(FlashManager* manager, FlashListener* listener) {
    m_listeners[m_listenerCount] = listener;
    m_listenerCount++;
    m_manager = manager;
}

void FlashPlayer::RemoveListener(FlashListener* listener) {
    u32 count = 0;
    u32 i;

    for (i = 0; i < m_listenerCount; i++) {
        if (m_listeners[i] != listener) {
            m_listeners[count] = m_listeners[i];
            count++;
        }
    }
    m_listenerCount = count;
}

void FlashPlayer::TagEvent() {
    const char* name = ReadString();
    const char* arg  = ReadString();
    u32         i;

    for (i = 0; i < m_listenerCount; i++) {
        if (m_listeners[i]->Matches(name)) {
            m_listeners[i]->OnEvent(m_manager, name, arg);
        }
    }
}

void FlashPlayer::PreloadSkipEvent() {
    m_preloadPc += strlen((char*)m_preloadPc) + 1;
    m_preloadPc += strlen((char*)m_preloadPc) + 1;
}

void FlashPlayer::AddFrameListener(FlashManager* manager, FlashFrameListener* listener) {
    m_frameListeners[m_frameListenerCount] = listener;
    m_frameListenerCount++;
    m_manager = manager;
}

void FlashPlayer::RemoveFrameListener(FlashFrameListener* listener) {
    u32 count = 0;
    u32 i;

    // BUG: bounded by the event listener count instead of the frame listener count
    for (i = 0; i < m_listenerCount; i++) {
        if (m_frameListeners[i] != listener) {
            m_frameListeners[count] = m_frameListeners[i];
            count++;
        }
    }
    m_frameListenerCount = count;
}

void FlashPlayer::TagJump() {
    u32 offset = ReadU32();

    m_pc = m_data + offset + 1;
    m_displayList.Init();
}

void FlashPlayer::PreloadJump() {
    u32 offset = ReadPreloadU32();

    m_preloadPc = m_data + offset + 1;
    m_frame     = -1;
}

BOOL FlashPlayer::Load(const char* path) {
    FlashParser parser;

    if (m_unk_04 == 1) {
        return FALSE;
    }
    m_stream = new FlashFileStream();
    m_stream->Open(path);
    parser.Parse(m_stream, this);
    m_state = FLASH_STATE_LOADED;
    return TRUE;
}

void FlashPlayer::Rewind() {
    FlashCharDef* def;
    s32           i;
    u16           j;

    if (m_state == FLASH_STATE_IDLE) {
        return;
    }
    m_displayList.Clear();
    m_preloadPc = m_pc = m_data;
    m_manager->m_loader->Reset();
    if (m_charDefCount != 0) {
        def = m_charDefs;
        i   = 0;
        while (i < m_charDefCount) {
            if (def->m_state == 1 && def->m_data != NULL) {
                FlashFreeArray(def->m_data);
                def->m_data = NULL;
            }
            def->m_state = 0;
            def++;
            i++;
        }
    }
    for (j = 0; j < 256; j++) {
        if (m_objects[j] != NULL) {
            m_objects[j]->Reset();
        }
    }
    m_state = FLASH_STATE_LOADED;
}

BOOL FlashPlayer::Play() {
    switch (m_state) {
        case FLASH_STATE_IDLE:
            return FALSE;
        case FLASH_STATE_LOADED:
            m_nextTick = 0;
            m_state    = FLASH_STATE_PLAYING;
            m_frame    = 0;
            Preload();
            break;
        case FLASH_STATE_PLAYING:
            break;
        case FLASH_STATE_ENDED:
            Rewind();
            m_state = FLASH_STATE_PLAYING;
            Preload();
            break;
    }
    return TRUE;
}

BOOL FlashPlayer::IsPlaying() {
    return m_state == FLASH_STATE_PLAYING;
}

void* FlashAlloc(u32 size) {
    return Mem_AllocZeroed(size, "Flash");
}

void* FlashAllocArray(u32 size) {
    return Mem_AllocZeroed(size, "Flash");
}

void FlashFree(void* ptr) {
    Mem_FreeMainHeap(ptr);
}

void FlashFreeArray(void* ptr) {
    Mem_FreeMainHeap(ptr);
}

void FlashParser::Parse(FlashStream* stream, FlashPlayer* player) {
    u16 size;
    u8* data;
    u32 length;

    ParseHeader(stream, player);
    ParseCharDefs(stream, player);
    ParseObjects(stream, player);
    stream->Read(&size, sizeof(size));
    if (size - 2 != 0) {
        u8* extra = (u8*)FlashAllocArray(size - 2);

        stream->Read(extra, size - 2);
        player->m_extraData = extra;
        player->m_extraSize = size;
    }
    length = stream->GetLength() - stream->Tell();
    data   = (u8*)FlashAllocArray(length);
    stream->Read(data, length);
    player->SetData(data);
}

void FlashParser::ParseHeader(FlashStream* stream, FlashPlayer* player) {
    u32 frameTicks;
    u16 width;
    u16 height;

    stream->Read(&width, sizeof(width));
    stream->Read(&height, sizeof(height));
    player->SetSize(width, height);
    stream->Read(&frameTicks, sizeof(frameTicks));
    player->m_frameTicks = frameTicks;
}

void FlashParser::ParseCharDefs(FlashStream* stream, FlashPlayer* player) {
    u16 count;
    u16 i;

    stream->Read(&count, sizeof(count));
    player->AllocCharDefs(count);
    for (i = 0; i < count; i++) {
        FlashCharDef def;
        u32          skip;
        u8           plttCount;

        def.m_widthLog  = 0;
        def.m_heightLog = 0;
        stream->Read(&def.m_id, 2);
        stream->Read(&def.m_widthLog, 1);
        stream->Read(&def.m_heightLog, 1);
        stream->Read(&def.m_unk_16, 2);
        stream->Read(&def.m_unk_18, 2);
        stream->Read(&def.m_format, 1);
        stream->Read(&def.m_dataSize, 4);
        stream->Read(&plttCount, 1);
        def.m_plttCount  = plttCount + 1;
        def.m_dataOffset = stream->Tell();
        stream->Read(&skip, 4);
        stream->Seek(skip + stream->Tell());
        def.m_plttOffset = stream->Tell();
        stream->Seek(stream->Tell() + def.m_plttCount * 2);
        player->SetCharDef(i, &def);
    }
}

void FlashParser::ParseObjects(FlashStream* stream, FlashPlayer* player) {
    u16 count;
    u16 i;

    stream->Read(&count, sizeof(count));
    for (i = 0; i < count; i++) {
        u8  type;
        s16 x;
        s16 y;
        s16 w;
        s16 h;
        u16 id;

        stream->Read(&type, 1);
        stream->Read(&id, 2);
        stream->Read(&x, 2);
        stream->Read(&y, 2);
        stream->Read(&w, 2);
        stream->Read(&h, 2);
        switch (type) {
            case 0: {
                u16        color;
                u8         verts[16];
                FlashRect* rect;

                stream->Read(&color, 2);
                stream->Read(verts, sizeof(verts));
                rect = new FlashRect();
                rect->Init(id, verts, color);
                rect->SetBounds(x, y, w, h);
                player->SetObject(rect);
                break;
            }
            case 1: {
                u16         charId;
                FlashMatrix mtx;
                FlashImage* image;
                s32         j;
                s32         k;

                stream->Read(&charId, 2);
                for (j = 0; j < 2; j++) {
                    for (k = 0; k < 3; k++) {
                        stream->Read(&mtx.m[j][k], 4);
                    }
                }
                image = new FlashImage();
                image->Init(id, charId, &mtx);
                image->SetBounds(x, y, w, h);
                player->SetObject(image);
                break;
            }
        }
    }
}

FlashUploadQueue::FlashUploadQueue() {
    Init();
}

FlashUploadQueue::~FlashUploadQueue() {}

void FlashUploadQueue::Init() {
    m_head    = 0;
    m_tail    = 0;
    m_count   = 0;
    m_current = -1;
#ifdef REGION_USA
    m_maxCount = 0;
#endif
    m_ready = TRUE;
}

void FlashUploadQueue::Push(void* src, u32 dst, u32 size, FlashCharDef* def) {
    Entry* entry;

    m_ready     = FALSE;
    entry       = &m_entries[m_tail];
    entry->src  = src;
    entry->dst  = dst;
    entry->size = size;
    entry->def  = def;
    m_count++;
#ifdef REGION_USA
    if (m_maxCount < m_count) {
        m_maxCount = m_count;
    }
#endif
    m_tail++;
    if (m_tail == FLASH_UPLOAD_QUEUE_LEN) {
        m_tail = 0;
    }
    m_ready = TRUE;
}

void FlashUploadQueue::Process() {
    FlashCharDef* def;

    if (m_current != -1 && (def = m_entries[m_current].def) != NULL) {
        if (def->m_data != NULL) {
            FlashFreeArray(def->m_data);
            def->m_data = NULL;
        }
        def->m_state = 2;
    }
    m_current = -1;
    if (m_ready && m_count != 0) {
        func_02028108(0, m_entries[m_head].dst, m_entries[m_head].src, m_entries[m_head].size);
        m_current = m_head;
        m_head++;
        if (m_head == FLASH_UPLOAD_QUEUE_LEN) {
            m_head = 0;
        }
        m_count--;
    }
}

FlashVramLoader::FlashVramLoader() {
    Reset();
}

FlashVramLoader::~FlashVramLoader() {}

void FlashVramLoader::Reset() {
    func_02028084(m_transfer, 16);
    m_queue.Init();
}

void FlashVramLoader::Load(FlashStream* stream, FlashCharDef* def, u32 texAddr, u32 plttAddr) {
    u8  buf[0x200];
    u8  context[0x10];
    u32 header;
    u32 remaining;
    u32 chunk;
    u32 size;
    u8* src;

    def->m_data = (u8*)FlashAllocArray(def->m_dataSize);
    stream->Seek(def->m_dataOffset);
    stream->Read(&remaining, 4);
    stream->Read(buf, 4);
    func_0203b928(context, def->m_data, buf);
    do {
        chunk = remaining;
        if (chunk >= 0x200) {
            chunk = 0x200;
        }
        remaining -= chunk;
        stream->Read(buf, chunk);
    } while (func_0203b958(context, buf, chunk) != 0);
#ifdef REGION_USA
    DC_PurgeRange(def->m_data, def->m_dataSize);
#endif
    size = def->m_dataSize;
    src  = def->m_data;
    while (size != 0) {
        chunk = size > 0x6000 ? 0x6000 : size;
        size -= chunk;
        m_queue.Push(src, texAddr, chunk, size == 0 ? def : NULL);
        src += chunk;
        texAddr += chunk;
    }
    def->m_palette = (u16*)FlashAllocArray(0x200);
    stream->Seek(def->m_plttOffset);
    stream->Read(def->m_palette, def->m_plttCount * 2);
#ifdef REGION_USA
    DC_PurgeRange(def->m_palette, def->m_plttCount * 2);
#endif
    func_02028108(1, plttAddr, def->m_palette, 0x200);
    def->m_state = 1;
}

void FlashVramLoader::Update() {
    func_020280a8(m_transfer);
    m_queue.Process();
}
