#define WORLD_CPP
#include "./world.hpp"

#include "./render.hpp"
#include <math.h>

namespace wmac::world {

void init() {
    say("Initializing world");
    m_noiseMap = new noise::module::Perlin();
    m_chunks.clear();
    // for (i32 x = -3; x < 4; x++) {
    //     for (i32 y = -3; y < 4; y++) {
    //         for (i32 z = -3; z < 4; z++) {
    //             generateChunk(vec3i{x, y, z});
    //             render::activateChunk(vec3i{x, y, z});
    //         }
    //     }
    // }
    // say("vertex count:", 4*render::m_accum);
    // say("triangle count:", 2*render::m_accum);
    generateChunksAt({0,0,0}, 16);
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

void generateChunksAt(vec3i p_pos, u32 p_radius) {
    i32 radius = scast<i32>(p_radius);
    std::vector<vec3i> toDisable = {};

    for (auto& [pos, chunk] : m_chunks) {
        if (
            abs(pos.x - p_pos.x) > radius ||
            abs(pos.y - p_pos.y) > radius ||
            abs(pos.z - p_pos.z) > radius
        ) {
            toDisable.push_back(pos);
        }
    }

    for (auto& pos : toDisable) {
        render::deactivateChunk(pos);
        delete m_chunks[pos];
        m_chunks.erase(pos);
    }

    for (i32 x = p_pos.x-radius; x <= p_pos.x+radius; x++) {
        for (i32 y = p_pos.y-radius; y <= p_pos.y+radius; y++) {
            for (i32 z = p_pos.z-radius; z <= p_pos.z+radius; z++) {
                if (m_chunks.find(vec3i{x, y, z}) == m_chunks.end()) {
                    generateChunk(vec3i{x, y, z});
                    render::activateChunk(vec3i{x, y, z});
                }
            }
        }
    }
}

vec3i getChunkLoc(vec3i p_pos) {
    return {
        .x = scast<int>(floor(p_pos.x / 16.0)),
        .y = scast<int>(floor(p_pos.y / 16.0)),
        .z = scast<int>(floor(p_pos.z / 16.0)),
    };
}

vec3i getChunkLoc(vec3 p_pos) {
    return {
        .x = scast<int>(floor(p_pos.x / 16.0)),
        .y = scast<int>(floor(p_pos.y / 16.0)),
        .z = scast<int>(floor(p_pos.z / 16.0)),
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