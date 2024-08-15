#define RENDER_CPP
#include "./render.hpp"
#include "./ticks.hpp"
#include "./world.hpp"

#include "./core.hpp"
#include <rlgl.h>

#include <GL/glew.h>
#include <GL/gl.h>
namespace wmac::render {

const u32 TILE_SIZE = 32;
const u32 TILE_PER_ROW = 16;

const f32 QUAD_VERTICES[6*4] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};

void initAtlas() {
    m_atlasImage = GenImageColor(TILE_SIZE*TILE_PER_ROW, TILE_SIZE*TILE_PER_ROW, WHITE);
}

void initMesh() {
    m_material = LoadMaterialDefault();
    m_material.shader = LoadShader("shaders/block.vert", "shaders/block.frag");
    m_uniformChunkPos = GetShaderLocation(m_material.shader, "chunkPos");

    m_atlas = LoadTextureFromImage(m_atlasImage);
    SetMaterialTexture(&m_material, MATERIAL_MAP_ALBEDO, m_atlas);

    // hmmmmm
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, 6*4 * sizeof(f32), QUAD_VERTICES, GL_STATIC_DRAW);

    glGenBuffers(1, &m_dataBuffer);
    
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

    mat4 model = IDENTITY_MATRIX * rlGetMatrixTransform();
    mat4 view = rlGetMatrixModelview();
    mat4 proj = rlGetMatrixProjection();
    mat4 mvp = model * view * proj;

    glBindVertexArray(m_vertexBuffer);
    glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, m_indirectCmds.data(), m_indirectCmds.size(), 0);

    rlSetMatrixModelview(view);
    rlSetMatrixProjection(proj);
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

    // m_renderChunks[p_pos].mesh = mesh;

    // mesh->vertexCount = 4*count;
    // mesh->triangleCount = 2*count;

    // UpdateMeshBuffer(*mesh, SHADER_LOC_VERTEX_POSITION, mesh->vertices, 12*count * sizeof(f32), 0);
    // UpdateMeshBuffer(*mesh, 6 /* indices location */ , mesh->indices, 6*count * sizeof(u16), 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_dataBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_dataArray.size() * sizeof(u64), m_dataArray.data(), GL_DYNAMIC_DRAW);
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

                    verts[]

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

IndirectCommand* createCommand(u32 p_size) {
    IndirectCommand cmd = {
        .count = 4,
        .instanceCount = p_size,
        .first = 0, // we'll find the right place later
        .baseInstance = 0,
    };
    IndirectCommand* cmdPtr; // for return value

    u32 i = 0;
    // check the start
    if (m_indirectCmds[0].first >= p_size) {
        m_indirectCmds.insert(m_indirectCmds.begin(), cmd);
        return m_indirectCmds.data();
    }

    // check any other gaps
    for (i = 1; i < m_indirectCmds.size(); i++) {
        if (m_indirectCmds[i].first - (m_indirectCmds[i-1].first + m_indirectCmds[i-1].instanceCount) >= p_size) {
            cmd.first = m_indirectCmds[i].first + m_indirectCmds[i].instanceCount;
            m_indirectCmds.insert(m_indirectCmds.begin() + i, cmd);
            return &(m_indirectCmds[i]);
        }
    }

    // place it in the end
    cmd.first = m_indirectCmds.back().first + m_indirectCmds.back().instanceCount;
    m_dataArraySize = cmd.first + p_size;
    m_indirectCmds.push_back(cmd);
    return &(m_indirectCmds.back());
}

// warning: this might move the command, making the pointer invalid
// so always use the returned pointer
IndirectCommand* resizeCommand(IndirectCommand* p_cmd, u32 p_newSize) {
    // no shrinking, at least for now
    if (p_cmd->instanceCount > p_newSize) return p_cmd;

    // if at the end, we mignt need to increase the buffer size
    if (p_cmd == m_indirectCmds.data() + m_indirectCmds.size() - 1) {
        if (p_cmd->first + p_newSize > m_dataArraySize) {
            m_dataArraySize = p_cmd->first + p_newSize;
        }
        p_cmd->instanceCount = p_newSize;
        return p_cmd;
    };

    // keep in mind vectors aren't null-terminated
    // but the last check should prevent us from going out of bounds
    IndirectCommand* next = p_cmd + 1;

    // if we can expand, we expand
    if (p_cmd->first + p_newSize <= next->first) {
        p_cmd->instanceCount = p_newSize;
        return p_cmd;
    }

    IndirectCommand cmdCopy = *p_cmd;

    // else, we move it. first check if it fits to the front
    if (p_cmd->instanceCount <= m_indirectCmds[0].first) {
        for (auto cur = p_cmd; cur > m_indirectCmds.data(); cur--) {
            *cur = *(cur-1);
        }
        m_indirectCmds[0] = cmdCopy;
        m_indirectCmds[0].instanceCount = p_newSize;
        return m_indirectCmds.data();
    }

    // if not, scan the rest of the array for a gap
    for (u32 i = 1; i < m_indirectCmds.size(); i++) {
        if (m_indirectCmds[i].first - (m_indirectCmds[i-1].first + m_indirectCmds[i-1].instanceCount) >= p_newSize) {
            p_cmd->first = m_indirectCmds[i].first + m_indirectCmds[i].instanceCount;
            p_cmd->instanceCount = p_newSize;

            // we moved it, so we need to move it in the array to keep the order correct
            if (p_cmd < m_indirectCmds.data() + i) {
                // move forward (move everything back)
                for (auto cur = p_cmd; cur < m_indirectCmds.data() + i; cur++) {
                    *cur = *(cur+1);
                }
            } else {
                // move backward (move everything forward)
                for (auto cur = p_cmd; cur > m_indirectCmds.data() + i; cur--) {
                    *cur = *(cur-1);
                }
            }
            m_indirectCmds[i] = cmdCopy;
            m_indirectCmds[i].instanceCount = p_newSize;
            return &(m_indirectCmds[i]);
        }
    }

    // if we're here, we couldn't find any gaps to fit the mesh,
    // so we need to move it to the end
    m_indirectCmds.erase(m_indirectCmds.begin() + (p_cmd - m_indirectCmds.data())); // iterator nonsense
    u32 temp = m_dataArraySize;
    m_dataArraySize = cmdCopy.first + cmdCopy.instanceCount;

    cmdCopy.first = temp;
    cmdCopy.instanceCount = p_newSize;
    m_indirectCmds.push_back(cmdCopy);
    return m_indirectCmds.data() + m_indirectCmds.size() - 1;
}

};

