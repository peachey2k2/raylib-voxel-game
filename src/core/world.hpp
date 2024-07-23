#pragma once

#include "misc/utils.hpp"

namespace wmac {

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

// these could be a more optimized alternative
// but i cba to implement everything for them

// // takes up 8 + 2 = 10 bytes
// typedef struct BlcokFew {
//     u64 id;
//     ChunkPos position;
// } BlockFew;

// // takes up 8 + 16*16*2 = 520 bytes
// typedef struct BlockMany {
//     u64 id;
//     u16 map[16*16];
// } BlockMany;

// typedef struct Chunk {
//     // vec3i position;
//     std::vector<BlockFew> few;
//     std::vector<BlockMany> many;
// } Chunk;

// takes up 16*16*16*8 = 32KB
typedef u64 Chunk[16*16*16];

};

namespace wmac::world {

#ifdef WORLD_CPP

std::unordered_map<vec3i, Chunk*> chunks;

#endif

void init();
void addChunk(vec3i p_pos, Chunk *p_chunk);
std::unordered_map<vec3i, Chunk*> getChunks();

vec3i getChunkLoc(vec3i p_pos);
ChunkPos getPosInChunk(vec3i p_pos);
void changeBlock(vec3i p_pos, u64 p_id);

}