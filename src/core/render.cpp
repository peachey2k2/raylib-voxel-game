#define RENDER_CPP
#include "core/render.hpp"
#include "core/ticks.hpp"
#include "core/world.hpp"

#include "core/core.hpp"
#include "rlgl.hpp"
namespace wmac::render {

const i32 TILE_SIZE = 32;
const i32 TILE_PER_ROW = 16;

void initAtlas() {
    atlasImage = raylib::GenImageColor(TILE_SIZE*TILE_PER_ROW, TILE_SIZE*TILE_PER_ROW, raylib::WHITE);
}

void initMesh() {
    material = raylib::LoadMaterialDefault();
    material.shader = raylib::LoadShader("shaders/block.vert", "shaders/block.frag");
    uniformChunkPos = raylib::GetShaderLocation(material.shader, "chunkPos");

    atlas = raylib::LoadTextureFromImage(atlasImage);
    raylib::SetMaterialTexture(&material, raylib::MATERIAL_MAP_ALBEDO, atlas);
}

void draw() {
    vec3 offset;
    for (auto& [chunkPos, renderChunk] : renderChunks) {
        raylib::Mesh& mesh = renderChunk.mesh;
        offset = {
            (f32)(chunkPos.x * 16),
            (f32)(chunkPos.y * 16),
            (f32)(chunkPos.z * 16),
        };
        raylib::SetShaderValue(material.shader, uniformChunkPos, &offset, raylib::SHADER_UNIFORM_VEC3);
        raylib::DrawMesh(mesh, material, IDENTITY_MATRIX);
    }
}

void addTextureToAtlas(InitBlockInfo& p_block) {
    p_block.texCoords = {
        (float)(atlasIndex % TILE_PER_ROW) * TILE_SIZE,
        (float)(atlasIndex / TILE_PER_ROW) * TILE_SIZE
    };
    atlasIndex++;

    raylib::Image image = raylib::LoadImage(p_block.texture);
    raylib::ImageResize(&image, TILE_SIZE, TILE_SIZE);
    raylib::ImageAlphaClear(&image, raylib::WHITE, 0.0f);
    raylib::ImageDraw(
        &atlasImage,
        image,
        {
            0, 0, 
            (float)image.width, (float)image.height
        }, {
            p_block.texCoords.x, p_block.texCoords.y,
            TILE_SIZE, TILE_SIZE
        },
        raylib::WHITE
    );
    raylib::UnloadImage(image);

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
            .indices = new u16[6*6 * 16*16*16],
        }
    };

    u16 count = populateMesh(p_pos);
    accum += count;

    auto& mesh = renderChunks[p_pos].mesh;

    UploadMesh(&mesh, true);

    renderChunks[p_pos].mesh.vertexCount = 4*count;
    renderChunks[p_pos].mesh.triangleCount = 2*count;

    UpdateMeshBuffer(mesh, raylib::SHADER_LOC_VERTEX_POSITION, mesh.vertices, 12*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, 6 /* indices location */ , mesh.indices, 6*count * sizeof(u16), 0);
}

void updateChunk(vec3i p_pos) {
    u16 count = populateMesh(p_pos);

    auto& mesh = renderChunks[p_pos].mesh;

    renderChunks[p_pos].mesh.vertexCount = 4*count;
    renderChunks[p_pos].mesh.triangleCount = 2*count;

    UpdateMeshBuffer(mesh, raylib::SHADER_LOC_VERTEX_POSITION, mesh.vertices, 12*count * sizeof(f32), 0);
    UpdateMeshBuffer(mesh, 6 /* indices location */ , mesh.indices, 6*count * sizeof(u16), 0);
}

u16 populateMesh(vec3i p_pos) {
    u16 count = 0;
    // vec3 offset = {
    //     p_pos.x * 16,
    //     p_pos.y * 16,
    //     p_pos.z * 16,
    // };

    auto& chunk = *(renderChunks[p_pos].chunk);
    auto& mesh = renderChunks[p_pos].mesh;

    #define BLOCK(x, y, z) chunk[16*16*(z) + 16*(y) + (x)]

    // say(BLOCK(15,15,15));

    for (u32 d = 0; d < 3; d++) {
        i32 i, j, k, l, w, h;
        i32 u = (d+1) % 3;
        i32 v = (d+2) % 3;
        i32 x[3] = {0};
        i32 q[3] = {0};

        bool mask[16*16];
        bool flip[16*16];

        q[d] = 1;
        x[d] = -1;

        while (x[d] < 16) {

            // cook le' mask
            u32 n = 0;
            for (x[v] = 0; x[v] < 16; x[v]++) {
                for (x[u] = 0; x[u] < 16; x[u]++) {
                    bool a = (x[d] >= 0) ? BLOCK(x[0],        x[1],        x[2]       ) > 0 : false;
                    bool b = (x[d] < 15) ? BLOCK(x[0] + q[0], x[1] + q[1], x[2] + q[2]) > 0 : false;

                    mask[n] = a != b;
                    flip[n] = b;
                    n++;
                }
            }

            x[d]++;
            n = 0;

            for (j = 0; j < 16; j++) {
                for (i = 0; i < 16;) {
                    if (!mask[n]) {
                        i++;
                        n++;
                        continue;
                    }

                    for (w = 1; i + w < 16 && mask[n + w]; w++);

                    for (h = 1; j + h < 16; h++) {
                        for (k = 0; k < w; k++) {
                            if (!mask[n + k + h*16]) goto endloop;
                            if (flip[n] != flip[n + k + h*16]) goto endloop;
                        }
                    } endloop:

                    x[u] = i;
                    x[v] = j;

                    // vec3i p = {
                    //     x[0] + offset.x,
                    //     x[1] + offset.y,
                    //     x[2] + offset.z,
                    // };

                    /*
                     * We don't need floats for vertices since it's all grid aligned
                     * Heck, we don't even need all 32 bits on each axis, so we use
                     * some of those bits for other purposes.
                     * x: 
                     *  0-4: x
                     *  5-9: y
                     *  10-14: z
                     *  24-31: unused
                     * y:
                     *  0-31: unused
                     * z:
                     *  0-15: tex-x
                     *  16-31: tex-y
                     */

                    // i32 du[3] = {0};
                    // i32 dv[3] = {0};
                    // du[u] = w;
                    // dv[v] = h;

                    i32 du = w << (5*u);
                    i32 dv = h << (5*v);

                    vec3i p = {
                        x[0] + (x[1]<<5) + (x[2]<<10),
                        0,
                        0,
                    };

                    u32* verts = rcast<u32*>(mesh.vertices);

                    if (flip[n]) {
                        verts[12*count + 0] = p.x + dv;
                        verts[12*count + 1] = p.y;
                        verts[12*count + 2] = p.z;

                        verts[12*count + 3] = p.x + du + dv;
                        verts[12*count + 4] = p.y;
                        verts[12*count + 5] = p.z;

                        verts[12*count + 6] = p.x + du;
                        verts[12*count + 7] = p.y;
                        verts[12*count + 8] = p.z;

                        verts[12*count + 9] = p.x;
                        verts[12*count + 10] = p.y;
                        verts[12*count + 11] = p.z;
                    } else {
                        verts[12*count + 0] = p.x;
                        verts[12*count + 1] = p.y;
                        verts[12*count + 2] = p.z;

                        verts[12*count + 3] = p.x + du;
                        verts[12*count + 4] = p.y;
                        verts[12*count + 5] = p.z;

                        verts[12*count + 6] = p.x + du + dv;
                        verts[12*count + 7] = p.y;
                        verts[12*count + 8] = p.z;

                        verts[12*count + 9] = p.x + dv;
                        verts[12*count + 10] = p.y;
                        verts[12*count + 11] = p.z;
                        
                    }

                    // mesh.texcoords[8*count + 0] = 0.0f;
                    // mesh.texcoords[8*count + 1] = 0.0f;
                    // mesh.texcoords[8*count + 2] = 1.0f/TILE_PER_ROW * w;
                    // mesh.texcoords[8*count + 3] = 0.0f;

                    // mesh.texcoords[8*count + 4] = 1.0f/TILE_PER_ROW * w;
                    // mesh.texcoords[8*count + 5] = 1.0f/TILE_PER_ROW * h;
                    // mesh.texcoords[8*count + 6] = 0.0f;
                    // mesh.texcoords[8*count + 7] = 1.0f/TILE_PER_ROW * h;

                    mesh.indices[6*count + 0] = 4*count + 0;
                    mesh.indices[6*count + 1] = 4*count + 1;
                    mesh.indices[6*count + 2] = 4*count + 2;
                    mesh.indices[6*count + 3] = 4*count + 2;
                    mesh.indices[6*count + 4] = 4*count + 3;
                    mesh.indices[6*count + 5] = 4*count + 0;

                    count++;

                    for (l = 0; l < h; l++) {
                        for (k = 0; k < w; k++) {
                            mask[n + k + l*16] = false;
                        }
                    }

                    i += w;
                    n += w;
                }
            }
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
