#define WORLD_CPP
#include "core/world.hpp"

#include "core/render.hpp"
#include <math.h>

namespace wmac::world {
    Chunk chunk = {};

void init() {
    say("Initializing world");
    chunks.clear();
    for (i32 x = 0; x < 16; x++) {
        for (i32 y = 0; y < 16; y++) {
            for (i32 z = 0; z < 16; z++) {
                if (pow(x-8, 2) + pow(y-8, 2) + pow(z-8, 2) < 64) {
                    chunk[x + y*16 + z*16*16] = 1;
                }
            }
        }
    }
    addChunk(vec3i{0, 0, 0}, &chunk);
    render::activateChunk(vec3i{0, 0, 0}, chunk);
}

void addChunk(vec3i p_pos, Chunk *p_chunk) {
    chunks[p_pos] = p_chunk;
}

vec3i getChunkLoc(vec3i p_pos) {
    return {
        .x = p_pos.x / 16,
        .y = p_pos.y / 16,
        .z = p_pos.z / 16,
    };
}

ChunkPos getPosInChunk(vec3i p_pos) {
    return {
        scast<u16>((p_pos.x & 0x0F) << 0) |
        scast<u16>((p_pos.y & 0x0F) << 4) |
        scast<u16>((p_pos.z & 0x0F) << 8)
    };
}

void changeBlock(vec3i p_pos, u64 p_id) {
    vec3i chunkLoc = getChunkLoc(p_pos);
    
    if (chunks.find(chunkLoc) == chunks.end()) {
        say("--Chunk not found");
        return;
    }

    ChunkPos posInChunk = getPosInChunk(p_pos);

    Chunk& chunk = *chunks[chunkLoc];
    chunk[posInChunk.xyz] = p_id;
}


};