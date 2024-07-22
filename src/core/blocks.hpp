#pragma once

#include "misc/utils.hpp"

namespace wmac::blocks {

typedef struct InitBlockInfo {
    u64 id;
    const char* name;
    const char* tooltip;
    const char* texture;
    const char* model;
    vec2 texCoords;
} InitBlockInfo;

const i32 TILE_SIZE = 32;
const i32 TILE_PER_ROW = 16;

#ifdef BLOCKS_CPP

std::vector<InitBlockInfo> blocks(256);
Mesh mesh;
Material material;

Image atlasImage;
Texture2D atlas;
u32 atlasIndex = 0;
u64 idCounter = 1; // 0 is reserved for air

Shader shader;
i32 shaderParam;

std::vector<f32> vertices;
std::vector<f32> texcoords;
std::vector<u16> indices;

const extern mat4 transforms[6];

#endif

void createAtlas();
void init();
void update();
void draw();
void add(InitBlockInfo &p_block);
void add(std::vector<InitBlockInfo> &p_blocks);
void addTextureToAtlas(InitBlockInfo &p_block);

void addDefaultBlocks();


};
