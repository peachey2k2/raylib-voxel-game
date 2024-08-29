#pragma once

#include "raylib/raylib.h"

#define WMAC_API extern "C"

#ifndef u8
typedef __UINT8_TYPE__ u8;
#endif

#ifndef u16
typedef __UINT16_TYPE__ u16;
#endif

#ifndef u32
typedef __UINT32_TYPE__ u32;
#endif

#ifndef u64
typedef __UINT64_TYPE__ u64;
#endif

#ifndef i8
typedef __INT8_TYPE__ i8;
#endif

#ifndef i16
typedef __INT16_TYPE__ i16;
#endif

#ifndef i32
typedef __INT32_TYPE__ i32;
#endif

#ifndef i64
typedef __INT64_TYPE__ i64;
#endif

#ifndef f32
typedef float f32;
#endif

#ifndef f64
typedef double f64;
#endif

#ifndef size_t
typedef __SIZE_TYPE__ size_t;
#endif

typedef u32 ModID;

typedef Vector2 vec2;
typedef Vector3 vec3;
typedef Vector4 vec4;
typedef Matrix mat4;

typedef struct vec2i {
    i32 x;
    i32 y;
} vec2i;

typedef struct vec3i {
    i32 x;
    i32 y;
    i32 z;
} vec3i;

typedef struct vec4i {
    i32 x;
    i32 y;
    i32 z;
    i32 w;
} vec4i;

typedef struct vec2d {
    f64 x;
    f64 y;
} vec2d;

typedef struct vec3d {
    f64 x;
    f64 y;
    f64 z;
} vec3d;

typedef struct vec4d {
    f64 x;
    f64 y;
    f64 z;
    f64 w;
} vec4d;

typedef struct IndirectCommand {
    u32 count; // indices to render (4 for quad)
    u32 instanceCount; // data size
    u32 first; // data start (0 for our case)
    u32 baseInstance; // base vertex attribute index
} IndirectCommand;

typedef struct Block {
    u64 itemId;
    u32 textureId;
    const char* name;
    const char* tooltip;
    const char* texture;
} Block;

// takes up 2 bytes
typedef struct ChunkPos {
    u16 xyz;

    constexpr u8 x() { return xyz & 0x0F; }
    constexpr u8 y() { return (xyz >> 4) & 0x0F; }
    constexpr u8 z() { return (xyz >> 8) & 0x0F; }

    constexpr void x(u8 x) { xyz = (xyz & 0xFFF0) | x; }
    constexpr void y(u8 y) { xyz = (xyz & 0xFF0F) | (y << 4); }
    constexpr void z(u8 z) { xyz = (xyz & 0xF0FF) | (z << 8); }
} ChunkPos;

// takes up 16*16*16 + 256 * 8 = 4KB + 2KB = 6KB for worst case
typedef struct SmallChunk {
    u8 data[16*16*16];
    std::vector<u64> blocks;
} SmallChunk;

// takes up 16*16*16*8 = 32KB, used for when there are more than 256 unique blocks
typedef struct LargeChunk {
    u64 data[16*16*16];
} LargeChunk;

typedef struct Chunk {
    SmallChunk* small;
    LargeChunk* large;
} Chunk;

// easier to work with for generation
typedef u64 ChunkLayout[16*16*16];

WMAC_API typedef struct InitBlockInfo {
    const char* name;
    const char* tooltip;
    const char* texture;
} InitBlockInfo;

WMAC_API typedef struct InitEntityInfo {
    
}

WMAC_API typedef struct ApiFunctions {
    void (*blocks__add)(InitBlockInfo &p_block);
    void (*entities__add)(InitEntityInfo &p_entity);
} ApiFunctions;

WMAC_API typedef struct ModInfo {
    const char* name;
    const char* version;
    const char* author;
    const char* description;
    const char* license;
    const char* source;
    const char* dependencies;
    const char* conflicts;
    const int loadPriority;
} ModInfo;

WMAC_API typedef struct Mod {
    ModInfo info;
    std::string path; // auto-filled by game, no need to set
    void (*initFunctions)(ApiFunctions &p_api);
    void (*initItems)();
    void (*initBlocks)();
} Mod;

