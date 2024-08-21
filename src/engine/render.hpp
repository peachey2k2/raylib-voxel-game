#pragma once

#include "utils/utils.hpp"
#include "./blocks.hpp"
#include "./world.hpp"

namespace wmac::render {

extern const u32 TILE_SIZE;
extern const u32 TILE_PER_ROW;
extern const u32 MESH_BUNCH_SIZE;

extern const f32 QUAD_VERTICES[6*4];

#ifdef RENDER_CPP

Material m_material;
// i32 m_uniformChunkPos;
i32 m_uniformMVP;
i32 m_uniformSampler;

// std::unordered_map<vec3i, RenderChunk> m_renderChunks;
std::unordered_map<vec3i, IndirectCommand*> m_renderChunks;

Image m_atlasImage;
Texture2D m_atlas;
u32 m_atlasIndex = 0;

u32 m_vao;

// this holds the vertex data. just a single quad, instanced
u32 m_vertexPosBuffer;

// ssbo to send the chunk positions
u32 m_shaderStorageBuffer;
std::vector<vec4i> m_shaderStorageArray = {};

// actual per-quad data, sent as vertex attributes
u32 m_attribBuffer;
std::vector<u64> m_attribArray = {};
u64 m_attribArraySize = 0;

// indirect draw commands. specifically in ascending order.
u32 m_indirectBuffer;
std::vector<IndirectCommand> m_indirectCmds = {};

#endif

inline i64 m_accum;

void initAtlas();
void initMesh();
void draw();
u32 addTextureToAtlas(const char* p_texture);

void activateChunk(vec3i p_pos);
void updateChunk(vec3i p_pos);
void deactivateChunk(vec3i p_pos);

u32 calculateVertexData(vec3i p_chunkPos, u64* &p_data);
void editMesh(vec3i p_chunkPos, u64* p_data, u32 p_size);

u32 createCommand(IndirectCommand* &p_cmd, u32 p_size);
u32 resizeCommand(IndirectCommand* &p_cmd, u32 p_newSize);

};