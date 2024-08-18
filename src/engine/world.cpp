#define WORLD_CPP
#include "./world.hpp"

#include "./render.hpp"
#include "utils/tools.hpp"
#include "./core.hpp"

#include <math.h>

namespace wmac::world {

void init() {
    auto bm = new tools::Benchmark("World init");
    m_noiseMap = new noise::module::Perlin();
    m_chunks.clear();
    generateChunksAt({0,0,0}, core::RENDER_DISTANCE);
    bm->end();
}

void deinit() {
    delete m_noiseMap;
    for (auto& [pos, chunk] : m_chunks) {
        delete chunk;
    }
}

void generateChunk(vec3i p_pos) {
    Chunk* chunkPtr = rcast<Chunk*>(new Chunk());
    Chunk& chunk = *chunkPtr;
    for (i32 x = 0; x < 16; x++) {
        for (i32 z = 0; z < 16; z++) {
            i32 height = scast<i32>(m_noiseMap->GetValue((x + p_pos.x*16)/160.0, 0, (z + p_pos.z*16)/160.0) * 160);
            i32 y;
            for (y = 0; (y < 16) && (y + p_pos.y*16 < height); y++) {
                chunk[x + y*16 + z*16*16] = 1;
            }
            for (; y < 16; y++) {
                chunk[x + y*16 + z*16*16] = 0;
            }
        }
    }
    m_chunks[p_pos] = &chunk;
}

void generateChunksAt(vec3i p_pos, u32 p_radius) {
    i32 radius = scast<i32>(p_radius);

    std::erase_if(m_chunks, [&](auto& p) {
        auto& [pos, chunk] = p;
        if (
            abs(pos.x - p_pos.x) > radius ||
            abs(pos.y - p_pos.y) > radius ||
            abs(pos.z - p_pos.z) > radius
        ) {
            render::deactivateChunk(pos);
            // tools::say(*chunk);
            if (chunk != nullptr) {
                delete chunk;
                chunk = nullptr;
            }
            return true;
        }
        return false;
    });

    for (i32 x = p_pos.x-radius; x <= p_pos.x+radius; x++) {
        for (i32 y = p_pos.y-radius; y <= p_pos.y+radius; y++) {
            for (i32 z = p_pos.z-radius; z <= p_pos.z+radius; z++) {
                vec3i pos = {x, y, z};
                if (m_chunks.find(pos) == m_chunks.end()) {
                    generateChunk(pos);
                    render::activateChunk(pos);
                }
            }
        }
    }
}

vec3i getChunkLoc(vec3i p_pos) {
    return {
        .x = scast<i32>(floor(p_pos.x / 16.0)),
        .y = scast<i32>(floor(p_pos.y / 16.0)),
        .z = scast<i32>(floor(p_pos.z / 16.0)),
    };
}

vec3i getChunkLoc(vec3 p_pos) {
    return {
        .x = scast<i32>(floor(p_pos.x / 16.0)),
        .y = scast<i32>(floor(p_pos.y / 16.0)),
        .z = scast<i32>(floor(p_pos.z / 16.0)),
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
        tools::say("--Chunk not found");
        return;
    }

    ChunkPos posInChunk = getPosInChunk(p_pos);

    Chunk& chunk = *m_chunks[chunkLoc];
    chunk[posInChunk.xyz] = p_id;
}


};