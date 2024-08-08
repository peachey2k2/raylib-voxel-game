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

raylib::Shader shader;
raylib::Material material;

std::unordered_map<vec3i, RenderChunk> renderChunks;

raylib::Image atlasImage;
raylib::Texture2D atlas;
u32 atlasIndex = 0;

std::vector<f32> vertices;
std::vector<f32> texcoords;
std::vector<u16> indices;

#endif

extern i64 accum;

void initAtlas();
void initMesh();
void draw();
void addTextureToAtlas(InitBlockInfo& p_block);

void activateChunk(vec3i p_pos);
void updateChunk(vec3i p_pos);
u16 populateMesh(vec3i p_pos);
void deactivateChunk(vec3i p_pos);

};