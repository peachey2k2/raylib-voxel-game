#pragma once

#include "misc/utils.hpp"
#include "core/blocks.hpp"
#include "core/world.hpp"

namespace wmac::render {

extern const i32 TILE_SIZE;
extern const i32 TILE_PER_ROW;

typedef struct RenderChunk {
    Chunk* chunk;
    raylib::Mesh mesh;
} RenderChunk;

#ifdef RENDER_CPP

// raylib::Shader shader; // FUCK YOU
raylib::Material m_material;
i32 m_uniformChunkPos;

std::unordered_map<vec3i, RenderChunk> renderChunks;

raylib::Image m_atlasImage;
raylib::Texture2D m_atlas;
u32 m_atlasIndex = 0;

#endif

inline i64 m_accum;

void initAtlas();
void initMesh();
void draw();
void addTextureToAtlas(InitBlockInfo& p_block);

void activateChunk(vec3i p_pos);
void updateChunk(vec3i p_pos);
u16 populateMesh(vec3i p_pos);
void deactivateChunk(vec3i p_pos);

};