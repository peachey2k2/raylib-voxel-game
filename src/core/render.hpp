#pragma once

#include "misc/utils.hpp"
#include "core/blocks.hpp"

namespace wmac::render {

extern const i32 TILE_SIZE;
extern const i32 TILE_PER_ROW;

#ifdef RENDER_CPP

Mesh mesh;
Material material;

Image atlasImage;
Texture2D atlas;
u32 atlasIndex = 0;

Shader shader;
i32 shaderParam;

std::vector<f32> vertices;
std::vector<f32> texcoords;
std::vector<u16> indices;

const extern mat4 transforms[6];

#endif


void initAtlas();
void initMesh();
void update();
void draw();
void addTextureToAtlas(InitBlockInfo& p_block);

};