#pragma once

#include <utils.hpp>
#include "./blocks.hpp"
#include "./world.hpp"

namespace wmac::render {

extern const u32 TILE_SIZE;
extern const u32 TILE_PER_ROW;
extern const u32 MESH_BUNCH_SIZE;

extern const f32 QUAD_VERTICES[6*4];

#ifdef RENDER_CPP

Material m_material;
i32 m_uniformChunkPos;

// std::unordered_map<vec3i, RenderChunk> m_renderChunks;
std::unordered_map<vec3i, IndirectCommand*> m_renderChunks;

Image m_atlasImage;
Texture2D m_atlas;
u32 m_atlasIndex = 0;

u32 m_vertexBuffer;
u32 m_dataBuffer;
std::vector<u64> m_dataArray;
u64 m_dataArraySize = 0;
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
u16 populateMesh(vec3i p_pos);
void deactivateChunk(vec3i p_pos);

Mesh* getNewMesh();
void dropMesh(Mesh*);

IndirectCommand* createMesh(u64* p_data, u32 p_size, vec3i p_chunk);
IndirectCommand* resizeMesh(u64* p_data, u32 p_newSize);

IndirectCommand* createCommand(u32 p_size);
IndirectCommand* resizeCommand(IndirectCommand* p_cmd, u32 p_newSize);

};