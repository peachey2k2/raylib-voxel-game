#define RENDER_CPP
#include "core/render.hpp"
#include "core/ticks.hpp"
#include "core/world.hpp"

namespace wmac::render {

const i32 TILE_SIZE = 32;
const i32 TILE_PER_ROW = 16;

const std::pair<vec3i, std::array<vec3, 4>> DIRECTIONS[6] = {
    {vec3i{1, 0, 0}, std::array<vec3, 4>{{{0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, -0.5f}}}},
    {vec3i{-1, 0, 0}, std::array<vec3, 4>{{{-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}}}},
    {vec3i{0, 1, 0}, std::array<vec3, 4>{{{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}}}},
    {vec3i{0, -1, 0}, std::array<vec3, 4>{{{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}}}},
    {vec3i{0, 0, 1}, std::array<vec3, 4>{{{0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}}}},
    {vec3i{0, 0, -1}, std::array<vec3, 4>{{{-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}}}},
};

void initAtlas() {
    atlasImage = GenImageColor(TILE_SIZE*TILE_PER_ROW, TILE_SIZE*TILE_PER_ROW, WHITE);
}

void initMesh() {
    Shader shader = LoadShader("shaders/block.vert", "shaders/block.frag");
    // shaderParam = GetShaderLocation(shader, "side");
    atlas = LoadTextureFromImage(atlasImage);
    material = LoadMaterialDefault();
    material.shader = shader;
    SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, atlas);
}

void draw() {
    for (auto& [chunkPos, renderChunk] : renderChunks) {
        DrawMesh(renderChunk.mesh, material, MatrixIdentity());
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

void activateChunk(vec3i p_pos, Chunk& p_chunk) {
    renderChunks[p_pos] = {
        .chunk = &p_chunk,
        .mesh = {
            .vertexCount = 8 * 16*16*16,
            .triangleCount = 12 * 16*16*16,
            .vertices = new f32[3*4*6 * 16*16*16],
            .texcoords = new f32[2*4*6 * 16*16*16],
            .indices = new u16[6*6 * 16*16*16],
        }
    };

    u16 count = populateMesh(p_pos);

    auto& mesh = renderChunks[p_pos].mesh;

    UploadMesh(&mesh, true);

    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_POSITION, mesh.vertices, 12*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_TEXCOORD01, mesh.texcoords, 8*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, 6 /* indices location */ , mesh.indices, 4*count * sizeof(u16), 0);
}

void updateChunk(vec3i p_pos) {
    u16 count = populateMesh(p_pos);

    auto& mesh = renderChunks[p_pos].mesh;

    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_POSITION, mesh.vertices, 12*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_TEXCOORD01, mesh.texcoords, 8*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, 6 /* indices location */ , mesh.indices, 4*count * sizeof(u16), 0);
}

u16 populateMesh(vec3i p_pos) {
    u16 count = 0;
    vec3 offset = {
        p_pos.x * 16,
        p_pos.y * 16,
        p_pos.z * 16,
    };

    auto& chunk = *(renderChunks[p_pos].chunk);
    auto& mesh = renderChunks[p_pos].mesh;

    u8 cullMask[16][16][16];
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                cullMask[x][y][z] = (chunk[16*16*z + 16*y + x] != 0);
            }
        }
    }

    for (int i = 0; i < 16*16*16; i++) {
        u64& blockId = chunk[i];
        if (blockId == 0) continue;
        vec3i posInChunk = {
            /*;)*/ (i & 0x0F),
            ((i >> 4) & 0x0F),
            ((i >> 8) & 0x0F),
        };
        vec3 pos = {
            posInChunk.x + offset.x,
            posInChunk.y + offset.y,
            posInChunk.z + offset.z,
        };
        for (auto& [direction, vertArr] : DIRECTIONS) {
            vec3i checkPos = {
                posInChunk.x + direction.x,
                posInChunk.y + direction.y,
                posInChunk.z + direction.z,
            };

            if (
                !(checkPos.x & 0x10) && // check if the block is in the same chunk
                !(checkPos.y & 0x10) && // if it goes out of bounds, checkPos will
                !(checkPos.z & 0x10) && // either be -1 or 16, which this catches
                cullMask[checkPos.x][checkPos.y][checkPos.z]
            ) continue;
            
            mesh.vertices[12*count + 0] = pos.x + vertArr[0].x;
            mesh.vertices[12*count + 1] = pos.y + vertArr[0].y;
            mesh.vertices[12*count + 2] = pos.z + vertArr[0].z;

            mesh.vertices[12*count + 3] = pos.x + vertArr[1].x;
            mesh.vertices[12*count + 4] = pos.y + vertArr[1].y;
            mesh.vertices[12*count + 5] = pos.z + vertArr[1].z;

            mesh.vertices[12*count + 6] = pos.x + vertArr[2].x;
            mesh.vertices[12*count + 7] = pos.y + vertArr[2].y;
            mesh.vertices[12*count + 8] = pos.z + vertArr[2].z;

            mesh.vertices[12*count + 9] = pos.x + vertArr[3].x;
            mesh.vertices[12*count + 10] = pos.y + vertArr[3].y;
            mesh.vertices[12*count + 11] = pos.z + vertArr[3].z;        

            mesh.texcoords[8*count + 0] = 0.0f;
            mesh.texcoords[8*count + 1] = 0.0f;
            mesh.texcoords[8*count + 2] = 1.0f/TILE_PER_ROW;
            mesh.texcoords[8*count + 3] = 0.0f;
            mesh.texcoords[8*count + 4] = 1.0f/TILE_PER_ROW;
            mesh.texcoords[8*count + 5] = 1.0f/TILE_PER_ROW;
            mesh.texcoords[8*count + 6] = 0.0f;
            mesh.texcoords[8*count + 7] = 1.0f/TILE_PER_ROW;

            mesh.indices[6*count + 0] = 4*count + 0;
            mesh.indices[6*count + 1] = 4*count + 1;
            mesh.indices[6*count + 2] = 4*count + 2;
            mesh.indices[6*count + 3] = 4*count + 2;
            mesh.indices[6*count + 4] = 4*count + 3;
            mesh.indices[6*count + 5] = 4*count + 0;

            count++;
        }
    }
    return count;
}

void deactivateChunk(vec3i p_pos) {
    delete renderChunks[p_pos].mesh.vertices;
    delete renderChunks[p_pos].mesh.texcoords;
    delete renderChunks[p_pos].mesh.indices;
    renderChunks.erase(p_pos);
}

};
