#pragma once

#include <utils.hpp>
#include <noise/noise.h>


namespace wmac::world {

inline std::unordered_map<vec3i, Chunk*> m_chunks;

#ifdef WORLD_CPP

noise::module::Perlin* m_noiseMap = nullptr;

#endif

void init();
void deinit();
void addChunk(vec3i p_pos, Chunk *p_chunk);

void generateChunk(vec3i p_pos);

vec3i getChunkLoc(vec3i p_pos);
ChunkPos getPosInChunk(vec3i p_pos);
void changeBlock(vec3i p_pos, u64 p_id);

}