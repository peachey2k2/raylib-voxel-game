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
    // queueGenerationsAt({0,0,0}, core::RENDER_DISTANCE);
    m_chunksToGenerateAt.push({0,0,0});
    bm->end();
}

void threadLoop() {
    while (core::terrainShouldGenerate()) {
        // restartLoop:
        while (not m_chunksToGenerateAt.empty()) {
            if (not (m_chunksToGenerate.empty() && m_chunksToRemove.empty() && render::m_chunksToUpdate.empty())) break;
            vec3i chunkPos = m_chunksToGenerateAt.front();
            m_chunksToGenerateAt.pop();
            queueGenerationsAt(chunkPos, core::RENDER_DISTANCE);
        }
        while (not m_chunksToGenerate.empty()) {
            // if (not m_chunksToGenerateAt.empty()) goto restartLoop;
            vec3i chunkPos = m_chunksToGenerate.front();
            m_chunksToGenerate.pop();
            generateChunk(chunkPos);
        }
        while (not m_chunksToRemove.empty()) {
            // if (not m_chunksToGenerateAt.empty()) goto restartLoop;
            vec3i chunkPos = m_chunksToRemove.front();
            m_chunksToRemove.pop();
            removeChunk(chunkPos);
        }
        i64 sleepTime = 5;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

void deinit() {
    delete m_noiseMap;
    for (auto& [pos, chunk] : m_chunks) {
        delete chunk;
    }
}

void generateChunk(vec3i p_pos) {
    // auto bm = new tools::Benchmark("Generating chunk");
    Chunk* chunkPtr = rcast<Chunk*>(new Chunk());
    Chunk& chunk = *chunkPtr;
    #pragma omp parallel for
    for (i32 x = 0; x < 16; x++) {
        for (i32 z = 0; z < 16; z++) {
            i32 height = scast<i32>(m_noiseMap->GetValue((x + p_pos.x*16)/160.0, 0, (z + p_pos.z*16)/160.0) * 160);
            if (height < p_pos.y*16) height = 0;
            else if (height >= (p_pos.y+1)*16) height = 16;
            else {height %= 16;}
            
            i32 y;
            for (y = 0; y < height; y++) {
                chunk[x + y*16 + z*16*16] = 1;
            }
            for (; y < 16; y++) {
                chunk[x + y*16 + z*16*16] = 0;
            }
        }
    }
    m_chunks[p_pos] = chunkPtr;
    render::m_chunksToUpdate.push(p_pos);
    // bm->end();
}

void removeChunk(vec3i p_pos) {
    // TODO: implement
    p_pos = p_pos;
    render::m_chunksToRemove.push(p_pos);
}

void queueGenerationsAt(vec3i p_pos, u32 p_radius) {
    auto bm = new tools::Benchmark("Generating chunks");
    i32 radius = scast<i32>(p_radius);
    // tools::say("Generating chunks at", p_pos, "at frame", core::getFrameCount());

    std::erase_if(m_chunks, [&](auto& p) {
        auto& [pos, chunk] = p;
        if (
            abs(pos.x - p_pos.x) > radius ||
            abs(pos.y - p_pos.y) > radius ||
            abs(pos.z - p_pos.z) > radius
        ) {
            m_chunksToRemove.push(pos);           
            if (chunk != nullptr) {
                delete chunk;
                chunk = nullptr;
            }
            return true;
        }
        return false;
    });

    for (i32 x = p_pos.x-radius; x <= p_pos.x+radius; x++) {
        for (i32 y = max(p_pos.y-radius, 0); y <= max(p_pos.y+radius, 0); y++) {
            for (i32 z = p_pos.z-radius; z <= p_pos.z+radius; z++) {
                vec3i pos = {x, y, z};
                if (not m_chunks.contains(pos)) {
                    m_chunksToGenerate.push(pos);
                }
            }
        }
    }
    bm->end();
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
        // not a real error, just vscode tripping. it still compiles.
        scast<u16>((p_pos.x & 0x0F) << 0) |
        scast<u16>((p_pos.y & 0x0F) << 4) |
        scast<u16>((p_pos.z & 0x0F) << 8)
    };
}

void changeBlock(vec3i p_pos, u64 p_id) {
    vec3i chunkLoc = getChunkLoc(p_pos);
    
    if (m_chunks.contains(chunkLoc) == false) {
        tools::say("--Chunk not found");
        return;
    }

    ChunkPos posInChunk = getPosInChunk(p_pos);

    Chunk& chunk = *m_chunks[chunkLoc];
    chunk[posInChunk.xyz] = p_id;
}


};