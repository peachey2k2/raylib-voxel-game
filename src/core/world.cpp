#define WORLD_CPP
#include "core/world.hpp"

namespace wmac::world {

vec3i getChunkLoc(vec3i p_pos) {
    return {
        .x = p_pos.x / 16,
        .y = p_pos.y / 16,
        .z = p_pos.z / 16,
    };
}

ChunkPos getPosInChunk(vec3i p_pos) {
    return {
        ((p_pos.x & 0x0F)) |
        ((p_pos.y & 0x0F) << 4) |
        ((p_pos.z & 0x0F) << 8)
    };
}

void changeBlock(vec3i p_pos, u64 p_id) {
    vec3i chunkLoc = getChunkLoc(p_pos);
    
    if (chunks.find(chunkLoc) == chunks.end()) {
        say("--Chunk not found");
        return;
    }

    ChunkPos posInChunk = getPosInChunk(p_pos);

    Chunk& chunk = chunks[chunkLoc];
    chunk[posInChunk.xyz] = p_id;
}


};