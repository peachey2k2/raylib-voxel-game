#define RENDER_CPP
#include "core/render.hpp"
#include "core/ticks.hpp"
#include "core/world.hpp"

namespace wmac::render {

const i32 TILE_SIZE = 32;
const i32 TILE_PER_ROW = 16;

// const std::pair<vec3i, std::array<vec3, 4>> DIRECTIONS[6] = {
//     {vec3i{1, 0, 0}, std::array<vec3, 4>{{{0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, -0.5f}}}},
//     {vec3i{-1, 0, 0}, std::array<vec3, 4>{{{-0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, 0.5f}}}},
//     {vec3i{0, 1, 0}, std::array<vec3, 4>{{{0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}}}},
//     {vec3i{0, -1, 0}, std::array<vec3, 4>{{{-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}}}},
//     {vec3i{0, 0, 1}, std::array<vec3, 4>{{{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}}}},
//     {vec3i{0, 0, -1}, std::array<vec3, 4>{{{0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}}}},
// };

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
    shaderParam = GetShaderLocation(shader, "side");
    atlas = LoadTextureFromImage(atlasImage);
    material = LoadMaterialDefault();
    material.shader = shader;
    SetMaterialTexture(&material, MATERIAL_MAP_ALBEDO, atlas);
    ticks::addCallback(update);

    /* hardcoded block, "necessary" to hack raylib */
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
        .vertexCount = scast<i32>(8*1000),
        .triangleCount = scast<i32>(12*1000),
        .vertices = vertices.data(),
        .texcoords = texcoords.data(),
        .indices = indices.data(),
    };
    // indices[0] = 0;
    // indices[1] = 0;
    // indices[2] = 0;
    // vertices[0] = 0.0f;
    // vertices[1] = 0.0f;
    // vertices[2] = 0.0f;
    UploadMesh(&mesh, true);
}

void update() {
    vertices.clear();
    texcoords.clear();
    indices.clear();
    u16 a = 0, b;

    for (auto& [chunkPos, chunk] : world::chunks) {
        u8 cullMask[16][16][16];

        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < 16; z++) {
                    cullMask[x][y][z] = ((*chunk)[16*16*z + 16*y + x] != 0);
                }
            }
        }

        for (int i = 0; i < 16*16*16; i++) {
            u64& blockId = (*chunk)[i];
            if (blockId == 0) continue;
            vec3i posInChunk = {
                /*;)*/ (i & 0x0F),
                ((i >> 4) & 0x0F),
                ((i >> 8) & 0x0F),
            };
            vec3 pos = {
                posInChunk.x + chunkPos.x,
                posInChunk.y + chunkPos.y,
                posInChunk.z + chunkPos.z,
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
                
                vertices.insert(vertices.end(), {
                    pos.x + vertArr[0].x, pos.y + vertArr[0].y, pos.z + vertArr[0].z,
                    pos.x + vertArr[1].x, pos.y + vertArr[1].y, pos.z + vertArr[1].z,
                    pos.x + vertArr[2].x, pos.y + vertArr[2].y, pos.z + vertArr[2].z,
                    pos.x + vertArr[3].x, pos.y + vertArr[3].y, pos.z + vertArr[3].z,
                });

                texcoords.insert(texcoords.end(), {
                    0.0f, 0.0f,
                    1.0f/TILE_PER_ROW, 0.0f,
                    1.0f/TILE_PER_ROW, 1.0f/TILE_PER_ROW,
                    0.0f, 1.0f/TILE_PER_ROW,
                });

                b = a;
                indices.insert(indices.end(), {
                    a, ++a, ++a,
                    a++, a++, b,
                });
                say("Block at ", posInChunk.x, posInChunk.y, posInChunk.z);
                say("vertices:", vertices[vertices.size()-12], vertices[vertices.size()-11], vertices[vertices.size()-10], vertices[vertices.size()-9], vertices[vertices.size()-8], vertices[vertices.size()-7], vertices[vertices.size()-6], vertices[vertices.size()-5], vertices[vertices.size()-4], vertices[vertices.size()-3], vertices[vertices.size()-2], vertices[vertices.size()-1]);
                say("indices:", indices[indices.size()-6], indices[indices.size()-5], indices[indices.size()-4], indices[indices.size()-3], indices[indices.size()-2], indices[indices.size()-1]);
            }
            

            // // vertices.insert(vertices.end(), {
            // //     pos.x + posInChunk.x - 0.5f, pos.y + posInChunk.y + 0.5f, pos.z + posInChunk.z - 0.5f,
            // //     pos.x + posInChunk.x + 0.5f, pos.y + posInChunk.y + 0.5f, pos.z + posInChunk.z - 0.5f,
            // //     pos.x + posInChunk.x + 0.5f, pos.y + posInChunk.y - 0.5f, pos.z + posInChunk.z - 0.5f,
            // //     pos.x + posInChunk.x - 0.5f, pos.y + posInChunk.y - 0.5f, pos.z + posInChunk.z - 0.5f,
            // // });
            // // texcoords.insert(texcoords.end(), {
            // //     0.0f, 0.0f,
            // //     1.0f/TILE_PER_ROW, 0.0f,
            // //     1.0f/TILE_PER_ROW, 1.0f/TILE_PER_ROW,
            // //     0.0f, 1.0f/TILE_PER_ROW,
            // // });
            // b = a;
            // indices.insert(indices.end(), {
            //     // a, a+1, a+2, // end my suffering
            //     // a+2, a+3, a,
            //     a, ++a, ++a,
            //     a++, a++, b,
            // });
        }
    }

    mesh.vertexCount = scast<i32>(vertices.size() / 3);
    mesh.triangleCount = scast<i32>(indices.size() / 3);
    say(mesh.vertexCount, mesh.triangleCount);

    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_POSITION, vertices.data(), scast<i32>(vertices.size() * sizeof(f32)), 0);
    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_TEXCOORD01, texcoords.data(), scast<i32>(texcoords.size() * sizeof(f32)), 0);
    UpdateMeshBuffer(mesh, 6 /* indices location */ , indices.data(), scast<i32>(indices.size() * sizeof(u16)), 0);
}

void draw() {
    DrawMesh(mesh, material, MatrixIdentity());
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

};
