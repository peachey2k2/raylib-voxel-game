#define RENDER_CPP
#include "./render.hpp"
#include "./ticks.hpp"
#include "./world.hpp"

#include "./core.hpp"
#include <rlgl.h>
namespace wmac::render {

const i32 TILE_SIZE = 32;
const i32 TILE_PER_ROW = 16;

void initAtlas() {
    m_atlasImage = GenImageColor(TILE_SIZE*TILE_PER_ROW, TILE_SIZE*TILE_PER_ROW, WHITE);
}

void initMesh() {
    m_material = LoadMaterialDefault();
    m_material.shader = LoadShader("shaders/block.vert", "shaders/block.frag");
    m_uniformChunkPos = GetShaderLocation(m_material.shader, "chunkPos");

    m_atlas = LoadTextureFromImage(m_atlasImage);
    SetMaterialTexture(&m_material, MATERIAL_MAP_ALBEDO, m_atlas);
}

void draw() {
    vec3 offset;
    for (auto& [chunkPos, renderChunk] : m_renderChunks) {
        Mesh& mesh = *(renderChunk.mesh);
        offset = {
            (f32)(chunkPos.x * 16),
            (f32)(chunkPos.y * 16),
            (f32)(chunkPos.z * 16),
        };
        SetShaderValue(m_material.shader, m_uniformChunkPos, &offset, SHADER_UNIFORM_VEC3);
        DrawMesh(mesh, m_material, IDENTITY_MATRIX);
    }
}

u32 addTextureToAtlas(const char* p_texture) {
    u32 idx = m_atlasIndex++;

    Image image = LoadImage(p_texture);
    ASSERT(image.data != nullptr, "Failed to load image " + std::string(p_texture) + ", is it in the right place?");
    if (image.width != TILE_SIZE || image.height != TILE_SIZE) {
        say("[WARNING] Image " + std::string(p_texture) + " is not " + std::to_string(TILE_SIZE) + "x" + std::to_string(TILE_SIZE) + " pixels. Resizing.");
        ImageResize(&image, TILE_SIZE, TILE_SIZE);
    }
    ImageAlphaClear(&image, WHITE, 0.0f);
    ImageDraw(
        &m_atlasImage,
        image,
        {
            0, 0, 
            (float)image.width, (float)image.height
        }, {
            (float)(idx % TILE_PER_ROW) * TILE_SIZE, (float)(idx / TILE_PER_ROW) * TILE_SIZE,
            TILE_SIZE, TILE_SIZE
        },
        WHITE
    );
    UnloadImage(image);
    return idx;
}

void activateChunk(vec3i p_pos) {
    Mesh* mesh = getNewMesh();
    Chunk& chunk = *(world::m_chunks[p_pos]);
        m_renderChunks[p_pos] = {
        .chunk = &chunk,
        .mesh = mesh,
    };

    u16 count = populateMesh(p_pos);
    m_accum += count;

    m_renderChunks[p_pos].mesh = mesh;

    mesh->vertexCount = 4*count;
    mesh->triangleCount = 2*count;

    UpdateMeshBuffer(*mesh, SHADER_LOC_VERTEX_POSITION, mesh->vertices, 12*count * sizeof(f32), 0);
    UpdateMeshBuffer(*mesh, 6 /* indices location */ , mesh->indices, 6*count * sizeof(u16), 0);
}

void updateChunk(vec3i p_pos) {
    u16 count = populateMesh(p_pos);

    auto& mesh = m_renderChunks[p_pos].mesh;

    mesh->vertexCount = 4*count;
    mesh->triangleCount = 2*count;

    UpdateMeshBuffer(*mesh, SHADER_LOC_VERTEX_POSITION, mesh->vertices, 12*count * sizeof(f32), 0);
    UpdateMeshBuffer(*mesh, 6 /* indices location */ , mesh->indices, 6*count * sizeof(u16), 0);
}

u16 populateMesh(vec3i p_pos) {
    u16 count = 0;

    auto& chunk = *(m_renderChunks[p_pos].chunk);
    auto& mesh = m_renderChunks[p_pos].mesh;

    #define BLOCK(x, y, z) chunk[16*16*(z) + 16*(y) + (x)]

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
                     * Heck, we don't even need most of the 32 bits on each axis, so
                     * we use some of those bits for other purposes.
                     * x: 
                     *  0-4: x
                     *  5-9: y
                     *  10-14: z
                     *  15-17: normal-ish
                     *  18-21: size-u
                     *  22-25: size-v
                     *  26-31: unused
                     * y:
                     *  0-31: unused
                     * z:
                     *  0-15: tex-x
                     *  16-31: tex-y
                     */

                    i32 du = w << (5*u);
                    i32 dv = h << (5*v);

                    u8 normal;
                    switch (d + flip[n]*3) {
                        case 0: normal = 0; break;
                        case 1: normal = 1; break; // top
                        case 2: normal = 3; break;
                        case 3: normal = 0; break;
                        case 4: normal = 1; break; // bottom
                        case 5: normal = 1; break;
                    }

                    i32 w2, h2;

                    if (normal%2) {
                        w2 = w;
                        h2 = h;
                    } else {
                        w2 = h;
                        h2 = w;
                    }

                    vec3i p = {
                        x[0] + (x[1]<<5) + (x[2]<<10) + (normal<<15) + ((w2-1)<<18) + ((h2-1)<<22),
                        0,
                        0,
                    };

                    u32* verts = rcast<u32*>(mesh->vertices);

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

                    mesh->indices[6*count + 0] = 4*count + 0;
                    mesh->indices[6*count + 1] = 4*count + 1;
                    mesh->indices[6*count + 2] = 4*count + 2;
                    mesh->indices[6*count + 3] = 4*count + 2;
                    mesh->indices[6*count + 4] = 4*count + 3;
                    mesh->indices[6*count + 5] = 4*count + 0;

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
    dropMesh(m_renderChunks[p_pos].mesh);
    m_renderChunks.erase(p_pos);
}

Mesh* getNewMesh() {
    if (m_meshPool.size() == 0) {
        Mesh* mesh = new Mesh();
        mesh -> vertexCount = 8 * 16*16*16;
        mesh -> triangleCount = 12 * 16*16*16;
        mesh -> vertices = new f32[3*4*6 * 16*16*16];
        mesh -> indices = new u16[6*6 * 16*16*16];
        UploadMesh(mesh, true);
        return mesh;
    }
    Mesh* mesh = m_meshPool.back();
    m_meshPool.pop_back();
    return mesh;
}

void dropMesh(Mesh* p_mesh) {
    m_meshPool.push_back(p_mesh);
}

};

