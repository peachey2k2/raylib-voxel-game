#define BLOCKS_CPP
#include "core/blocks.hpp"

namespace wmac::blocks {

void createAtlas() {
    // atlasImage = GenImageColor(TILE_SIZE*TILE_PER_ROW, TILE_SIZE*TILE_PER_ROW, WHITE);
    atlasImage = LoadImage("texture.png");
}

u32 packVertexData(vec3i p_pos, u32 p_tex) {
    u32 vertex = 0;
    vertex |= p_pos.x & 0xF;
    vertex |= (p_pos.y & 0xf) << 4;
    vertex |= (p_pos.z & 0xf) << 8;
    vertex |= (p_tex & 0x3F) << 12;
    return vertex;

}

void init() {
    Shader shader = LoadShader("shaders/block.vert", "shaders/block.frag");
    shaderParam = GetShaderLocation(shader, "side");
    atlas = LoadTextureFromImage(atlasImage);
    material = LoadMaterialDefault();
    material.shader = shader;
    SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, atlas);

    /* hardcoded block */
    // vertices = {
    //     -0.5f, 0.5f, -0.5f,
    //     0.5f, 0.5f, -0.5f,
    //     0.5f, -0.5f, -0.5f,
    //     -0.5f, -0.5f, -0.5f,
    //     -0.5f, 0.5f, 0.5f,
    //     0.5f, 0.5f, 0.5f,
    //     0.5f, -0.5f, 0.5f,
    //     -0.5f, -0.5f, 0.5f,
    // };
    vertices.push_back(packVertexData({0, 0, 0}, 0));

    mesh = {
        .vertexCount = scast<i32>(vertices.size() / 3),
        .vertices = vertices.data(),
    };
    UploadMesh(&mesh, true);
}

void update() {
    mesh.vertexCount = scast<i32>(vertices.size() / 3);

    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_POSITION, vertices.data(), scast<i32>(vertices.size() * sizeof(f32)), 0);

}

void draw() {
    for (int i = 0; i < 6; i++) {
        SetShaderValue(shader, shaderParam, &i, SHADER_UNIFORM_INT);
        DrawMesh(mesh, material, MatrixIdentity());
    }
}

void add(InitBlockInfo p_block) {
    blocks.push_back(p_block);
}

void add(std::vector<InitBlockInfo> p_blocks) {
    blocks.insert(blocks.end(), p_blocks.begin(), p_blocks.end());
}

};