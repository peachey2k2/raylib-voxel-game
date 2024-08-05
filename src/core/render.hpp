#pragma once

#include "misc/utils.hpp"
#include "core/blocks.hpp"
#include "core/world.hpp"

namespace wmac::render {

extern const i32 TILE_SIZE;
extern const i32 TILE_PER_ROW;
extern const std::pair<vec3i, std::array<vec3, 4>> DIRECTIONS[6];

typedef struct RenderChunk {
    Chunk* chunk;
    Mesh mesh;
} RenderChunk;

#ifdef RENDER_CPP

Shader shader;
Material material;

std::unordered_map<vec3i, RenderChunk> renderChunks;

Image atlasImage;
Texture2D atlas;
u32 atlasIndex = 0;

std::vector<f32> vertices;
std::vector<f32> texcoords;
std::vector<u16> indices;

#endif

void initAtlas();
void initMesh();
void draw();
void addTextureToAtlas(InitBlockInfo& p_block);

void activateChunk(vec3i p_pos);
void updateChunk(vec3i p_pos);
u16 populateMesh(vec3i p_pos);
void deactivateChunk(vec3i p_pos);

};