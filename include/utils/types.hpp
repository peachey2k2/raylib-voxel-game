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

WMAC_API typedef u32 ModID;
WMAC_API typedef u64 ItemID;
WMAC_API typedef u32 EntityID;
WMAC_API typedef u32 BlockID;

WMAC_API typedef u64 EntityToken;

// typedef Vector2 vec2;
// typedef Vector3 vec3;
// typedef Vector4 vec4;
typedef Matrix mat4;

typedef struct vec2 {
    f32 x;
    f32 y;

    constexpr vec2() : x(0), y(0) {}
    constexpr vec2(f32 p_x, f32 p_y) : x(p_x), y(p_y) {}
    constexpr vec2(Vector2 p_vec) : x(p_vec.x), y(p_vec.y) {}
    constexpr operator Vector2() const { return { x, y }; }
    constexpr operator vec2i() const { return { (i32)x, (i32)y }; }
    constexpr operator vec2d() const { return { (f64)x, (f64)y }; }
    vec3 toVec3(f32 z) const { return { x, y, z }; }
    vec4 toVec4(f32 z, f32 w) const { return { x, y, z, w }; }
} vec2;

typedef struct vec3 {
    f32 x;
    f32 y;
    f32 z;

    constexpr vec3() : x(0), y(0), z(0) {}
    constexpr vec3(f32 p_x, f32 p_y, f32 p_z) : x(p_x), y(p_y), z(p_z) {}
    constexpr vec3(Vector2 p_vec, f32 p_z) : x(p_vec.x), y(p_vec.y), z(p_z) {}
    constexpr vec3(Vector3 p_vec) : x(p_vec.x), y(p_vec.y), z(p_vec.z) {}
    constexpr operator Vector3() const { return { x, y, z }; }
    constexpr operator vec3i() const { return { (i32)x, (i32)y, (i32)z }; }
    constexpr operator vec3d() const { return { (f64)x, (f64)y, (f64)z }; }
    vec4 toVec4(f32 w) const { return { x, y, z, w }; }
} vec3;

typedef struct vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;

    constexpr vec4() : x(0), y(0), z(0), w(0) {}
    constexpr vec4(f32 p_x, f32 p_y, f32 p_z, f32 p_w) : x(p_x), y(p_y), z(p_z), w(p_w) {}
    constexpr vec4(Vector2 p_vec, f32 p_z, f32 p_w) : x(p_vec.x), y(p_vec.y), z(p_z), w(p_w) {}
    constexpr vec4(Vector3 p_vec, f32 p_w) : x(p_vec.x), y(p_vec.y), z(p_vec.z), w(p_w) {}
    constexpr vec4(Vector4 p_vec) : x(p_vec.x), y(p_vec.y), z(p_vec.z), w(p_vec.w) {}
    constexpr operator Vector4() const { return { x, y, z, w }; }
    constexpr operator vec4i() const { return { (i32)x, (i32)y, (i32)z, (i32)w }; }
    constexpr operator vec4d() const { return { (f64)x, (f64)y, (f64)z, (f64)w }; }
} vec4;

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

    constexpr vec2d() : x(0), y(0) {}
    constexpr vec2d(f64 p_x, f64 p_y) : x(p_x), y(p_y) {}
    constexpr operator Vector2() const { return { (f32)x, (f32)y }; }
    constexpr operator vec2i() const { return { (i32)x, (i32)y }; }
    constexpr operator vec2() const { return { (f32)x, (f32)y }; }
} vec2d;

typedef struct vec3d {
    f64 x;
    f64 y;
    f64 z;

    constexpr vec3d() : x(0), y(0), z(0) {}
    constexpr vec3d(f64 p_x, f64 p_y, f64 p_z) : x(p_x), y(p_y), z(p_z) {}
    constexpr operator Vector3() const { return { (f32)x, (f32)y, (f32)z }; }
    constexpr operator vec3i() const { return { (i32)x, (i32)y, (i32)z }; }
    constexpr operator vec3() const { return { (f32)x, (f32)y, (f32)z }; }
} vec3d;

typedef struct vec4d {
    f64 x;
    f64 y;
    f64 z;
    f64 w;

    constexpr vec4d() : x(0), y(0), z(0), w(0) {}
    constexpr vec4d(f64 p_x, f64 p_y, f64 p_z, f64 p_w) : x(p_x), y(p_y), z(p_z), w(p_w) {}
    constexpr operator Vector4() const { return { (f32)x, (f32)y, (f32)z, (f32)w }; }
    constexpr operator vec4i() const { return { (i32)x, (i32)y, (i32)z, (i32)w }; }
    constexpr operator vec4() const { return { (f32)x, (f32)y, (f32)z, (f32)w }; }
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

WMAC_API typedef struct MobStats {
    f64 health;
    f64 speed;
    f64 attack;
    f64 defense;
    f64 attackSpeed;
    f64 attackRange;
} MobStats;

WMAC_API typedef struct InitItemInfo {
    const char* name;
    const char* tooltip;
    const char* texture;
    const char* model;
} InitItemInfo;

WMAC_API typedef struct InitBlockInfo {
    const char* name;
    const char* tooltip;
    const char* texture;
    const char* model;
} InitBlockInfo;

WMAC_API typedef struct InitEntityInfo {
    const char* name;
    const char* texture;
    const char* model;
    const void (*spawnCallback)(EntityToken &p_token);
    MobStats stats;
} InitEntityInfo;

WMAC_API typedef struct EntityData {
    vec3d position;
    vec3d velocity;
    MobStats stats;
} EntityData;

WMAC_API typedef struct PlayerData {
    vec3d position;
    vec3d velocity;
    MobStats stats;
} PlayerData;

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
    const i32 loadPriority;
} ModInfo;

WMAC_API typedef struct Mod {
    ModInfo info;
    std::string path; // auto-filled by game, no need to set
    void (*initFunctions)(ApiFunctions &p_api);
    void (*initItems)();
    void (*initBlocks)();
    void (*initEntities)();
} Mod;

