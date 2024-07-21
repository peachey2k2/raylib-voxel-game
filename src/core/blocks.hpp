#pragma once

#include "misc/utils.hpp"

namespace wmac::blocks {

typedef struct InitBlockInfo {
    const char* name;
    const char* tooltip;
    const char* texture;
    const char* model;
} InitBlockInfo;

const i32 TILE_SIZE = 16;
const i32 TILE_PER_ROW = 16;

#ifdef BLOCKS_CPP

std::vector<InitBlockInfo> blocks(256);
Mesh mesh;
Image atlasImage;
Texture2D atlas;
Material material;

Shader shader;
i32 shaderParam;

std::vector<f32> vertices;

#endif

void createAtlas();
void init();
void update();
void draw();
void add(InitBlockInfo p_block);
void add(std::vector<InitBlockInfo> p_blocks);

};
