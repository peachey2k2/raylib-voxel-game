#pragma once

#include "raylib/raylib.h"

#define WMAC_API extern "C"

#define MAX_8BIT 255
#define MAX_16BIT 65535
#define MAX_32BIT 4294967295
#define MAX_64BIT 18446744073709551615

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

typedef Matrix mat4;

typedef struct vec2i {
    i32 x;
    i32 y;

    constexpr vec2i() : x(0), y(0) {}
    constexpr vec2i(i32 p_x, i32 p_y) : x(p_x), y(p_y) {}
    constexpr operator Vector2() const { return { (f32)x, (f32)y }; }
    constexpr i32& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const i32& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    static const vec2i zero;
} vec2i;

inline const vec2i vec2i::zero = { 0, 0 };

typedef struct vec2d {
    f64 x;
    f64 y;

    constexpr vec2d() : x(0), y(0) {}
    constexpr vec2d(f64 p_x, f64 p_y) : x(p_x), y(p_y) {}
    constexpr operator Vector2() const { return { (f32)x, (f32)y }; }
    constexpr operator vec2i() const { return { (i32)x, (i32)y }; }
    constexpr f64& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const f64& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr vec2d normalize() const {
        f64 len = sqrt(x*x + y*y);
        return { x / len, y / len };
    }
    static const vec2d zero;
} vec2d;

inline const vec2d vec2d::zero = { 0, 0 };

typedef struct vec2 {
    f32 x;
    f32 y;

    constexpr vec2() : x(0), y(0) {}
    constexpr vec2(f32 p_x, f32 p_y) : x(p_x), y(p_y) {}
    constexpr vec2(Vector2 p_vec) : x(p_vec.x), y(p_vec.y) {}
    constexpr vec2(vec2d p_vec) : x(p_vec.x), y(p_vec.y) {}
    constexpr operator Vector2() const { return { x, y }; }
    constexpr operator vec2i() const { return { (i32)x, (i32)y }; }
    constexpr operator vec2d() const { return { (f64)x, (f64)y }; }
    constexpr f32& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const f32& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr vec2 normalize() const {
        f32 len = sqrtf(x*x + y*y);
        return { x / len, y / len };
    }
    static const vec2 zero;
} vec2;

inline const vec2 vec2::zero = { 0, 0 };

typedef struct vec3i {
    i32 x;
    i32 y;
    i32 z;

    constexpr vec3i() : x(0), y(0), z(0) {}
    constexpr vec3i(i32 p_x, i32 p_y, i32 p_z) : x(p_x), y(p_y), z(p_z) {}
    constexpr operator Vector3() const { return { (f32)x, (f32)y, (f32)z }; }
    constexpr i32& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const i32& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    static const vec3i zero;
} vec3i;

inline const vec3i vec3i::zero = { 0, 0, 0 };

typedef struct vec3d {
    f64 x;
    f64 y;
    f64 z;

    constexpr vec3d() : x(0), y(0), z(0) {}
    constexpr vec3d(f64 p_x, f64 p_y, f64 p_z) : x(p_x), y(p_y), z(p_z) {}
    constexpr operator Vector3() const { return { (f32)x, (f32)y, (f32)z }; }
    constexpr operator vec3i() const { return { (i32)x, (i32)y, (i32)z }; }
    constexpr f64& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const f64& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr vec3d normalize() const {
        f64 len = sqrt(x*x + y*y + z*z);
        return { x / len, y / len, z / len };
    }
    static const vec3d zero;
} vec3d;

inline const vec3d vec3d::zero = { 0, 0, 0 };

typedef struct vec3 {
    f32 x;
    f32 y;
    f32 z;

    constexpr vec3() : x(0), y(0), z(0) {}
    constexpr vec3(f32 p_x, f32 p_y, f32 p_z) : x(p_x), y(p_y), z(p_z) {}
    constexpr vec3(Vector2 p_vec, f32 p_z) : x(p_vec.x), y(p_vec.y), z(p_z) {}
    constexpr vec3(Vector3 p_vec) : x(p_vec.x), y(p_vec.y), z(p_vec.z) {}
    constexpr vec3(vec3d p_vec) : x(p_vec.x), y(p_vec.y), z(p_vec.z) {}
    constexpr operator Vector3() const { return { x, y, z }; }
    constexpr operator vec3i() const { return { (i32)x, (i32)y, (i32)z }; }
    constexpr operator vec3d() const { return { (f64)x, (f64)y, (f64)z }; }
    constexpr f32& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const f32& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr vec3 normalize() const {
        f32 len = sqrtf(x*x + y*y + z*z);
        return { x / len, y / len, z / len };
    }
    static const vec3 zero;
} vec3;

inline const vec3 vec3::zero = { 0, 0, 0 };

typedef struct vec4i {
    i32 x;
    i32 y;
    i32 z;
    i32 w;

    constexpr vec4i() : x(0), y(0), z(0), w(0) {}
    constexpr vec4i(i32 p_x, i32 p_y, i32 p_z, i32 p_w) : x(p_x), y(p_y), z(p_z), w(p_w) {}
    constexpr operator Vector4() const { return { (f32)x, (f32)y, (f32)z, (f32)w }; }
    constexpr i32& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const i32& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    static const vec4i zero;
} vec4i;

inline const vec4i vec4i::zero = { 0, 0, 0, 0 };

typedef struct vec4d {
    f64 x;
    f64 y;
    f64 z;
    f64 w;

    constexpr vec4d() : x(0), y(0), z(0), w(0) {}
    constexpr vec4d(f64 p_x, f64 p_y, f64 p_z, f64 p_w) : x(p_x), y(p_y), z(p_z), w(p_w) {}
    constexpr operator Vector4() const { return { (f32)x, (f32)y, (f32)z, (f32)w }; }
    constexpr operator vec4i() const { return { (i32)x, (i32)y, (i32)z, (i32)w }; }
    constexpr f64& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const f64& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr vec4d normalize() const {
        f64 len = sqrt(x*x + y*y + z*z + w*w);
        return { x / len, y / len, z / len, w / len };
    }
    static const vec4d zero;
} vec4d;

inline const vec4d vec4d::zero = { 0, 0, 0, 0 };

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
    constexpr vec4(vec3d p_vec, f32 p_w) : x(p_vec.x), y(p_vec.y), z(p_vec.z), w(p_w) {}
    constexpr operator Vector4() const { return { x, y, z, w }; }
    constexpr operator vec4i() const { return { (i32)x, (i32)y, (i32)z, (i32)w }; }
    constexpr operator vec4d() const { return { (f64)x, (f64)y, (f64)z, (f64)w }; }
    constexpr f32& operator[] (u32 p_index) {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr const f32& operator[] (u32 p_index) const {
        switch (p_index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Index " + std::to_string(p_index) + " is out of bounds");
        }
    }
    constexpr vec4 normalize() const {
        f32 len = sqrtf(x*x + y*y + z*z + w*w);
        return { x / len, y / len, z / len, w / len };
    }
    static const vec4 zero;
} vec4;

inline const vec4 vec4::zero = { 0, 0, 0, 0 };

template <typename T>
struct Range {
    T min;
    T max;
};

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

WMAC_API typedef enum Axis {
    AXIS_X = 0,
    AXIS_Y = 1,
    AXIS_Z = 2,
    AXIS_W = 3,
} Axis;

WMAC_API typedef enum ColorChannel {
    COLOR_R = 0,
    COLOR_G = 1,
    COLOR_B = 2,
    COLOR_A = 3,
} ColorChannel;

WMAC_API typedef enum BlockFace {
    FACE_NORTH = 0,
    FACE_EAST = 1,
    FACE_TOP = 2,
    FACE_SOUTH = 3,
    FACE_WEST = 4,
    FACE_BOTTOM = 5,
} BlockFace;

WMAC_API typedef enum ObjectType {
    OBJECT_BLOCK = 1 << 0,
    OBJECT_ENTITY = 1 << 1,
    OBJECT_ALL = MAX_32BIT,
    OBJECT_NONE = 0,
} ObjectType;

WMAC_API typedef struct RayTarget {
    u64 id;
    vec3d pos;
    BlockFace face;
    ObjectType type;
} RayTarget;

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

