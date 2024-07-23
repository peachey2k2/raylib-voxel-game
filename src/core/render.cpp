#define RENDER_CPP
#include "core/render.hpp"
#include "core/ticks.hpp"
#include "core/world.hpp"

namespace wmac::render {

const i32 TILE_SIZE = 32;
const i32 TILE_PER_ROW = 16;

void initAtlas() {
    atlasImage = GenImageColor(TILE_SIZE*TILE_PER_ROW, TILE_SIZE*TILE_PER_ROW, BLANK);
}

void initMesh() {
    Shader shader = LoadShader("shaders/block.vert", "shaders/block.frag");
    shaderParam = GetShaderLocation(shader, "side");
    atlas = LoadTextureFromImage(atlasImage);
    material = LoadMaterialDefault();
    material.shader = shader;
    SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, atlas);
    ticks::addCallback(update);

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
    vertices.clear();
    texcoords.clear();
    indices.clear();
    for (auto& [pos, chunk] : world::getChunks()) {
        for (int i = 0; i < 16*16*16; i++) {
            u64& blockId = (*chunk)[i];
            if (blockId == 0) continue;
            vec3 posInChunk = {
                (i % 16),
                (i / 256),
                (i % 256) / 16,
            };
            say(posInChunk.x, posInChunk.y, posInChunk.z, blockId);
            vertices.insert(vertices.end(), {
                pos.x + posInChunk.x - 0.5f, pos.y + posInChunk.y + 0.5f, pos.z + posInChunk.z - 0.5f,
                pos.x + posInChunk.x + 0.5f, pos.y + posInChunk.y + 0.5f, pos.z + posInChunk.z - 0.5f,
                pos.x + posInChunk.x + 0.5f, pos.y + posInChunk.y - 0.5f, pos.z + posInChunk.z - 0.5f,
                pos.x + posInChunk.x - 0.5f, pos.y + posInChunk.y - 0.5f, pos.z + posInChunk.z - 0.5f,
            });
            texcoords.insert(texcoords.end(), {
                0.0f, 0.0f,
                1.0f/TILE_PER_ROW, 0.0f,
                1.0f/TILE_PER_ROW, 1.0f/TILE_PER_ROW,
                0.0f, 1.0f/TILE_PER_ROW,
            });
            u16 a, b = scast<u16>(vertices.size() - 4);
            indices.insert(indices.end(), {
                // a, a+1, a+2, // end my suffering
                // a+2, a+3, a,
                a, a++, a++,
                a, a++, b,
            });
        }
    }

    // mesh.vertexCount = scast<i32>(vertices.size() / 3);
    // mesh.triangleCount = scast<i32>(indices.size() / 3);
    mesh = {
        .vertexCount = scast<i32>(vertices.size() / 3),
        .triangleCount = scast<i32>(indices.size() / 3),
        .vertices = vertices.data(),
        .texcoords = texcoords.data(),
        .indices = indices.data(),
    };
    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_POSITION, vertices.data(), scast<i32>(vertices.size() * sizeof(f32)), 0);
    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_TEXCOORD01, texcoords.data(), scast<i32>(texcoords.size() * sizeof(f32)), 0);
    UpdateMeshBuffer(mesh, 6, indices.data(), scast<i32>(indices.size() * sizeof(u16)), 0);
}

void draw() {
    for (auto& matrix : transforms) {
        DrawMesh(mesh, material, matrix);
    }
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

const mat4 transforms[6] = {
    MatrixRotateX(PI/2),
    MatrixRotateX(-PI/2),
    MatrixRotateY(PI/2),
    MatrixRotateY(-PI/2),
    MatrixRotateY(PI),
    MatrixIdentity(),
};

};
