#pragma once

#include "utils/utils.hpp"
#include <noise/noise.h>


namespace wmac::world {

inline std::unordered_map<vec3i, Chunk*> m_chunks;

#ifdef WORLD_CPP

noise::module::Perlin* m_noiseMap = nullptr;

std::vector<Chunk> m_chunkPool = {};

#endif

inline std::queue<vec3i> m_chunksToGenerate = {};
inline std::queue<vec3i> m_chunksToRemove = {}; // TODO: implement
inline std::queue<vec3i> m_chunksToGenerateAt = {};

void init();
void threadLoop();
void deinit();

void generateChunk(vec3i p_pos);
Chunk* constructChunk(ChunkLayout& p_layout);
void removeChunk(vec3i p_pos);
void queueGenerationsAt(vec3i p_pos, u32 p_radius);

vec3i getChunkLoc(vec3i p_pos);
vec3i getChunkLoc(vec3 p_pos);
ChunkPos getPosInChunk(vec3i p_pos);
void changeBlock(vec3i p_pos, u64 p_id);

}