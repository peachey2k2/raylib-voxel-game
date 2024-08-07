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

i64 accum = 0;

void activateChunk(vec3i p_pos) {
    Chunk& chunk = *(world::chunks[p_pos]);
        renderChunks[p_pos] = {
        .chunk = &chunk,
        .mesh = {
            .vertexCount = 8 * 16*16*16,
            .triangleCount = 12 * 16*16*16,
            .vertices = new f32[3*4*6 * 16*16*16],
            .texcoords = new f32[2*4*6 * 16*16*16],
            .indices = new u16[6*6 * 16*16*16],
        }
    };

    u16 count = populateMesh(p_pos);
    accum += count;

    auto& mesh = renderChunks[p_pos].mesh;

    UploadMesh(&mesh, true);

    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_POSITION, mesh.vertices, 12*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_TEXCOORD01, mesh.texcoords, 8*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, 6 /* indices location */ , mesh.indices, 6*count * sizeof(u16), 0);
}

void updateChunk(vec3i p_pos) {
    u16 count = populateMesh(p_pos);

    auto& mesh = renderChunks[p_pos].mesh;

    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_POSITION, mesh.vertices, 12*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, SHADER_LOC_VERTEX_TEXCOORD01, mesh.texcoords, 8*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, 6 /* indices location */ , mesh.indices, 6*count * sizeof(u16), 0);
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

    #define BLOCK(x, y, z) chunk[16*16*(z) + 16*(y) + (x)]

    say(BLOCK(15,15,15));

    u8 faces[3][17][16][16];
    // todo: make it work with chunk boundaries
    for (i32 i = 0; i < 15; i++) {
        for (i32 j = 0; j < 16; j++) {
            for (i32 k = 0; k < 16; k++) {
                if (BLOCK(i, j, k) && BLOCK(i+1, j, k)) {
                    faces[0][i+1][j][k] = 0;
                } else {
                    faces[0][i+1][j][k] = BLOCK(i, j, k) | BLOCK(i+1, j, k);
                }

                if (BLOCK(j, k, i) && BLOCK(j, k, i+1)) {
                    faces[1][i+1][j][k] = 0;
                } else {
                    faces[1][i+1][j][k] = BLOCK(j, k, i) | BLOCK(j, k, i+1);
                }

                if (BLOCK(k, i, j) && BLOCK(k, i+1, j)) {
                    faces[2][i+1][j][k] = 0;
                } else {
                    faces[2][i+1][j][k] = BLOCK(k, i, j) | BLOCK(k, i+1, j);
                }
            }
        }
    }

    // GREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEED
    u8 greedyMask[17][16][16] = {0};
    for (i32 x = 1; x < 17; x++) {
        for (i32 y = 0; y < 16; y++) {
            for (i32 z = 0; z < 16; z++) {
                u64 id = faces[0][x][y][z];
                if (id == 0) continue;
                if (greedyMask[x][y][z]) continue;

                greedyMask[x][y][z] = 1;

                i32 yi = y+1;
                for (; yi < 16; yi++) {
                    if (faces[0][x][yi][z] != id) break;
                    if (greedyMask[x][yi][z]) break;
                    greedyMask[x][yi][z] = 1;
                }

                i32 zi = z+1;
                for (; zi < 16; zi++) {
                    for (i32 ytoyi = y; ytoyi <= yi; ytoyi++) {
                        if (faces[0][x][ytoyi][zi] != id) goto endloop;
                        if (greedyMask[x][ytoyi][zi]) goto endloop;
                    }
                    for (i32 ytoyi = y; ytoyi <= yi; ytoyi++) {
                        greedyMask[x][ytoyi][zi] = 1;
                    }
                }
                endloop:

                mesh.vertices[12*count + 0] = x + offset.x;
                mesh.vertices[12*count + 1] = y + offset.y;
                mesh.vertices[12*count + 2] = z + offset.z;

                mesh.vertices[12*count + 3] = x + offset.x;
                mesh.vertices[12*count + 4] = yi + offset.y;
                mesh.vertices[12*count + 5] = z + offset.z;

                mesh.vertices[12*count + 6] = x + offset.x;
                mesh.vertices[12*count + 7] = yi + offset.y;
                mesh.vertices[12*count + 8] = zi + offset.z;

                mesh.vertices[12*count + 9] = x + offset.x;
                mesh.vertices[12*count + 10] = y + offset.y;
                mesh.vertices[12*count + 11] = zi + offset.z;

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
    }

    std::cout << "Chunk Position: " << p_pos.x << ", " << p_pos.y << ", " << p_pos.z << std::endl;
    std::cout << "Vertex Count: " << count*4 << std::endl;
    for (u16 i = 0; i < count*4; i++) {
        std::cout << "Vertex " << i << ": (" 
                  << mesh.vertices[3*i + 0] << ", " 
                  << mesh.vertices[3*i + 1] << ", " 
                  << mesh.vertices[3*i + 2] << ")" << std::endl;
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
