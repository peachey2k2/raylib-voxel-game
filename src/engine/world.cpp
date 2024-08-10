#define WORLD_CPP
#include "./world.hpp"

#include "./render.hpp"
#include <math.h>

namespace wmac::world {

void init() {
    say("Initializing world");
    m_noiseMap = new noise::module::Perlin();
    m_chunks.clear();
    for (i32 x = 0; x < 10; x++) {
        for (i32 y = 0; y < 10; y++) {
            for (i32 z = 0; z < 10; z++) {
                generateChunk(vec3i{x, y, z});
                render::activateChunk(vec3i{x, y, z});
            }
        }
    }
    say("vertex count:", 4*render::m_accum);
    say("triangle count:", 2*render::m_accum);
}

void deinit() {
    delete m_noiseMap;
    for (auto& [pos, chunk] : m_chunks) {
        delete chunk;
    }
}

void addChunk(vec3i p_pos, Chunk *p_chunk) {
    m_chunks[p_pos] = p_chunk;
}

void generateChunk(vec3i p_pos) {
    Chunk* chunk = rcast<Chunk*>(new Chunk());
    for (i32 x = 0; x < 16; x++) {
        for (i32 z = 0; z < 16; z++) {
            i32 height = scast<i32>(m_noiseMap->GetValue((x + p_pos.x*16)/160.0, 0, (z + p_pos.z*16)/160.0) * 160);
            for (i32 y = 0; (y < 16) && (y + p_pos.y*16 < height); y++) {
                (*chunk)[x + y*16 + z*16*16] = 1;
            }
        }
    }
    addChunk(p_pos, chunk);
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
    
    if (m_chunks.find(chunkLoc) == m_chunks.end()) {
        say("--Chunk not found");
        return;
    }

    ChunkPos posInChunk = getPosInChunk(p_pos);

    Chunk& chunk = *m_chunks[chunkLoc];
    chunk[posInChunk.xyz] = p_id;
}


};