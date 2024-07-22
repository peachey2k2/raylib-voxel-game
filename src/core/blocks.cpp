#define BLOCKS_CPP
#include "core/blocks.hpp"

namespace wmac::blocks {

void createAtlas() {
    atlasImage = GenImageColor(TILE_SIZE*TILE_PER_ROW, TILE_SIZE*TILE_PER_ROW, WHITE);
}

void init() {
    Shader shader = LoadShader("shaders/block.vert", "shaders/block.frag");
    shaderParam = GetShaderLocation(shader, "side");
    atlas = LoadTextureFromImage(atlasImage);
    material = LoadMaterialDefault();
    material.shader = shader;
    SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, atlas);

    /* hardcoded block */
    vertices = {
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
    };
    indices = {
        0, 1, 2,
        2, 3, 0,
    };
    texcoords = {
        0.0f, 0.0f,
        1.0f/TILE_PER_ROW, 0.0f,
        1.0f/TILE_PER_ROW, 1.0f/TILE_PER_ROW,
        0.0f, 1.0f/TILE_PER_ROW,
    };

    mesh = {
        .vertexCount = scast<i32>(vertices.size() / 3),
        .triangleCount = scast<i32>(indices.size() / 3),
        .vertices = vertices.data(),
        .texcoords = texcoords.data(),
        .indices = indices.data(),
    };
    UploadMesh(&mesh, true);
}

void update() {
    mesh.vertexCount = scast<i32>(vertices.size() / 3);
    mesh.triangleCount = scast<i32>(indices.size() / 3);

    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_POSITION, vertices.data(), scast<i32>(vertices.size() * sizeof(f32)), 0);
    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_TEXCOORD01, texcoords.data(), scast<i32>(texcoords.size() * sizeof(f32)), 0);
    UpdateMeshBuffer(mesh, 6, indices.data(), scast<i32>(indices.size() * sizeof(u16)), 0);

}

void draw() {
    for (auto& matrix : transforms) {
        DrawMesh(mesh, material, matrix);
    }
}

void add(InitBlockInfo& p_block) {
    addTextureToAtlas(p_block);
    p_block.id = idCounter++;
    blocks.push_back(p_block);
}

void add(std::vector<InitBlockInfo>& p_blocks) {
    for (auto& block : p_blocks) {
        addTextureToAtlas(block);
        block.id = idCounter++;
    }
    blocks.insert(blocks.end(), p_blocks.begin(), p_blocks.end());
}

void addTextureToAtlas(InitBlockInfo& p_block) {
    p_block.texCoords = {
        (float)(atlasIndex % TILE_PER_ROW) * TILE_SIZE,
        (float)(atlasIndex / TILE_PER_ROW) * TILE_SIZE
    };
    atlasIndex++;

    Image image = LoadImage(p_block.texture);
    ImageResize(&image, TILE_SIZE, TILE_SIZE);
    ImageAlphaClear(&image, WHITE, 0.0f);
    ImageDraw(
        &atlasImage, 
        image,
        {
            0, 0, 
            (float)image.width, (float)image.height
        }, {
            p_block.texCoords.x, p_block.texCoords.y,
            TILE_SIZE, TILE_SIZE
        },
        WHITE
    );
    UnloadImage(image);
}

void addDefaultBlocks() {
    InitBlockInfo block {
        .name = "grass",
        .tooltip = "Grass",
        .texture = "texture.png",
    };
    add(block);
}

const mat4 transforms[6] = {
    MatrixRotateX(PI/2),
    MatrixRotateX(-PI/2),
    MatrixRotateY(PI/2),
    MatrixRotateY(-PI/2),
    MatrixRotateY(PI),
    MatrixIdentity(),
};

};