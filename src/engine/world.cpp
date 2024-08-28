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
        delete chunk.small;
        delete chunk.large;
    }
}

ChunkLayout chunkLayout; // used for chunk gen, easier to work with.
void generateChunk(vec3i p_pos) {
    // auto bm = new tools::Benchmark("Generating chunk");
    #pragma omp parallel for
    for (i32 x = 0; x < 16; x++) {
        for (i32 z = 0; z < 16; z++) {
            i32 height = scast<i32>(m_noiseMap->GetValue((x + p_pos.x*16)/160.0, 0, (z + p_pos.z*16)/160.0) * 160);
            if (height < p_pos.y*16) height = 0;
            else if (height >= (p_pos.y+1)*16) height = 16;
            else {height %= 16;}
            
            i32 y;
            for (y = 0; y < height; y++) {
                chunkLayout[x + y*16 + z*16*16] = 1;
            }
            for (; y < 16; y++) {
                chunkLayout[x + y*16 + z*16*16] = 0;
            }
        }
    }
    m_chunks[p_pos] = constructChunk(chunkLayout);;
    render::m_chunksToUpdate.push(p_pos);
    // bm->end();
}

Chunk constructChunk(ChunkLayout& p_layout) {
    static std::unordered_map<u64, u32> blockCounts = {};
    Chunk chunk;

    blockCounts.clear();
    for (auto& block : p_layout) {
        if (blockCounts.contains(block)) {
            blockCounts[block]++;
        } else {
            blockCounts[block] = 1;
        }
    }

    if (blockCounts.size() > MAX_8BIT) {
        if (m_largeChunkPool.empty()) {
            chunk.large = new LargeChunk();
        } else {
            chunk.large = m_largeChunkPool.back();
            m_largeChunkPool.pop_back();
        }
        chunk.small = nullptr;

        std::memcpy(chunk.large->data, p_layout, sizeof(u64) * 16*16*16);
        return chunk;
    }

    if (m_smallChunkPool.empty()) {
        chunk.small = new SmallChunk();
    } else {
        chunk.small = m_smallChunkPool.back();
        m_smallChunkPool.pop_back();
    }
    chunk.large = nullptr;

    u32 i = 1;
    for (auto& [id, count] : blockCounts) {
        // chunk.small->blocks[i++] = id;
        chunk.small->blocks.push_back(id);
        // small->blocks is only a 1-way conversion, so we do this instead
        count = i++; // keep in mind this starts from 1, 0 is reserved for air.
    }

    #pragma omp simd
    for (i = 0; i < 16*16*16; i++) {
        u32 idx = p_layout[i];
        if (idx == 0) {
            chunk.small->data[i] = 0;
        } else {
            chunk.small->data[i] = blockCounts[idx];
        }
    }
    return chunk;
}

void removeChunk(vec3i p_pos) {
    // TODO: implement
    p_pos = p_pos;
}

void queueGenerationsAt(vec3i p_pos, u32 p_radius) {
    auto bm = new tools::Benchmark("Generating chunks");
    i32 radius = scast<i32>(p_radius);
    // tools::say("Generating chunks at", p_pos, "at frame", core::getFrameCount());

    std::erase_if(m_chunks, [&](std::pair<const vec3i, Chunk>& p) {
        auto& [pos, chunk] = p;
        if (
            abs(pos.x - p_pos.x) > radius ||
            abs(pos.y - p_pos.y) > radius ||
            abs(pos.z - p_pos.z) > radius
        ) {
            m_chunksToRemove.push(pos);           
            // if (chunk != nullptr) {
            //     delete chunk;
            //     chunk = nullptr;
            // }
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

    Chunk& chunk = m_chunks[chunkLoc];

    if (chunk.large != nullptr) {
        chunk.large->data[posInChunk.xyz] = p_id;
    } else {
        auto& [data, blocks] = *chunk.small;
        u32 idx = findIn(blocks, p_id);

        if (idx != NOT_FOUND) {
            data[posInChunk.xyz] = idx+1;
        } else {
            if (blocks.size() >= MAX_8BIT) {
                if (m_largeChunkPool.empty()) {
                    chunk.large = new LargeChunk();
                } else {
                    chunk.large = m_largeChunkPool.back();
                    m_largeChunkPool.pop_back();
                }
                
                auto& [oldData, oldBlocks] = *chunk.small;
                auto& newData = chunk.large->data;
                #pragma omp simd
                for (i32 i = 0; i < 16*16*16; i++) {
                    newData[i] = oldBlocks[oldData[i]-1];
                }

                m_smallChunkPool.push_back(chunk.small);
                chunk.small = nullptr;
            } 
            blocks.push_back(p_id);
            data[posInChunk.xyz] = blocks.size();
        }
    }
}


};