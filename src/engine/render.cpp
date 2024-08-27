#define RENDER_CPP
#include "./render.hpp"
#include "./ticks.hpp"
#include "./world.hpp"
#include "./core.hpp"

#include "utils/tools.hpp"

#include "raylib/rlgl.h"

#include "gl/glad.h"
#include <GL/gl.h>

namespace wmac::render {

const u32 TILE_SIZE = 32;
const u32 TILE_PER_ROW = 16;

void initAtlas() {
    m_atlasImage = GenImageColor(TILE_SIZE*TILE_PER_ROW, TILE_SIZE*TILE_PER_ROW, WHITE);
}

void initMesh() {
    auto bm = new tools::Benchmark("initMesh");

    m_material = LoadMaterialDefault();
    m_material.shader = LoadShader("shaders/block.vert", "shaders/block.frag");

    m_uniformMVP = GetShaderLocation(m_material.shader, "mvp");
    m_uniformSampler = GetShaderLocation(m_material.shader, "texture0");

    m_atlas = LoadTextureFromImage(m_atlasImage);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_attribBuffer);

    glGenBuffers(1, &m_indirectBuffer);
    // glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBuffer);

    glGenBuffers(1, &m_shaderStorageBuffer);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shaderStorageBuffer);

    glBindVertexArray(0);
    GL_CHECK_ERROR("init mesh");

    bm->end();
}

void update() {
    auto start = std::chrono::high_resolution_clock::now();

    int i = 0;
    do {
        if (m_chunksToUpdate.empty()) break;
        vec3i chunkPos = m_chunksToUpdate.front();
        m_chunksToUpdate.pop();
        updateChunk(chunkPos);
        i++;
    } while (std::chrono::duration<f64>(std::chrono::high_resolution_clock::now() - start).count() < 0.005);

    do {
        if (m_chunksToRemove.empty()) break;
        vec3i chunkPos = m_chunksToRemove.front();
        m_chunksToRemove.pop();
        deactivateChunk(chunkPos);
    } while (std::chrono::duration<f64>(std::chrono::high_resolution_clock::now() - start).count() < 0.005);

    if (m_indirectCmds.size() > 0) draw();
}

void draw() {
    glBindVertexArray(m_vao);
    // GL_CHECK_ERROR("bind vao");

    glUseProgram(m_material.shader.id);
    
    mat4 model = IDENTITY_MATRIX;
    mat4 view = rlGetMatrixModelview();
    mat4 proj = rlGetMatrixProjection();

    rlSetMatrixModelview(view);
    rlSetMatrixProjection(proj);
    view = rlGetMatrixTransform() * view;
    mat4 mvp = model * view * proj;
    glUniformMatrix4fv(m_uniformMVP, 1, GL_FALSE, rcast<f32*>(&mvp));
    // GL_CHECK_ERROR("set mvp");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_atlas.id);
    glUniform1i(m_uniformSampler, 0);

    if (m_updateAttribs) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_shaderStorageBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_shaderStorageArray.size() * sizeof(vec4i), m_shaderStorageArray.data(), GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_shaderStorageBuffer);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        // GL_CHECK_ERROR("bind ssbo");

        glBindBuffer(GL_ARRAY_BUFFER, m_attribBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_attribArraySize * sizeof(u64), m_attribArray.data(), GL_DYNAMIC_DRAW);
        glVertexAttribIPointer(0, 2, GL_UNSIGNED_INT, sizeof(u64), nullptr);
        glVertexAttribDivisor(0, 1);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // GL_CHECK_ERROR("bind attrib buffer");

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_indirectBuffer);
        glBufferData(GL_DRAW_INDIRECT_BUFFER, m_indirectCmds.size() * sizeof(IndirectCommand), m_indirectCmds.data(), GL_DYNAMIC_DRAW);
        // GL_CHECK_ERROR("bind indirect buffer");

        m_updateAttribs = false;
        tools::say(m_indirectCmds.size(), m_attribArraySize);
    }

    glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, m_indirectCmds.size(), 0);
    GL_CHECK_ERROR("draw");

    glBindVertexArray(0);
}

u32 addTextureToAtlas(const char* p_texture) {
    u32 idx = m_atlasIndex++;

    Image image = LoadImage(p_texture);
    ASSERT(image.data != nullptr, "Failed to load image " + std::string(p_texture) + ", is it in the right place?");
    if (image.width != TILE_SIZE || image.height != TILE_SIZE) {
        tools::say("[WARNING] Image", p_texture, "is not", TILE_SIZE, "x", TILE_SIZE, " pixels. Resizing.");
        ImageResize(&image, TILE_SIZE, TILE_SIZE);
    }
    ImageAlphaClear(&image, WHITE, 0.0f);
    ImageDraw(
        &m_atlasImage,
        image,
        {
            0, 0,
            (f32)image.width, (f32)image.height
        }, {
            (f32)(idx % TILE_PER_ROW) * TILE_SIZE, (f32)(idx / TILE_PER_ROW) * TILE_SIZE,
            TILE_SIZE, TILE_SIZE
        },
        WHITE
    );
    UnloadImage(image);
    return idx;
}

void activateChunk(vec3i p_pos) {
    u64* data;
    u32 dataSize = calculateVertexData(p_pos, data);
    if (dataSize == 0) return;
    editMesh(p_pos, data, dataSize);

    m_updateAttribs = true;
}

void updateChunk(vec3i p_pos) {
    u64* data;
    u32 dataSize = calculateVertexData(p_pos, data);
    if (dataSize == 0) {
        deactivateChunk(p_pos);
        return;
    }
    editMesh(p_pos, data, dataSize);

    m_updateAttribs = true;
}

void deactivateChunk(vec3i p_pos) {
    bool exists = false;
    u32 idx = 0;
    for (auto& e : m_shaderStorageArray) {
        if (e.x == p_pos.x && e.y == p_pos.y && e.z == p_pos.z) {
            exists = true;
            idx = &e - m_shaderStorageArray.data();
            break;
        }
    }

    if (exists) {
        m_indirectCmds.erase(m_indirectCmds.begin() + idx);
        m_shaderStorageArray.erase(m_shaderStorageArray.begin() + idx);
        m_updateAttribs = true;
    }
}

u64 tmpVertBuffer[6*16*16*16];
u32 calculateVertexData(vec3i p_chunkPos, u64* &p_data) {
    // make everything static to gain a bit more performance
    static i32 i, j, k, l, w, h, u, v, x[3], q[3], d, w2, h2;
    static u32 n, size;
    static u8 normal;
    static bool mask[16*16], flip[16*16];
    static vec3i pos;
    static u64 vertData;

    size = 0;

    if (world::m_chunks.contains(p_chunkPos) == false) {
        tools::say("Chunk at", p_chunkPos, "does not exist");
        // idk if this works 100% of the time, TODO: make sure it does
        return 0; // if it's removed, don't bother
    }
    // auto bm = new tools::Benchmark("greedy mesher");
    // TODO: add a signal system for you know what

    auto& chunk = *(world::m_chunks[p_chunkPos]);

    // TODO: make this work wit the new chunk layout

    #define BLOCK(x, y, z) chunk[16*16*(z) + 16*(y) + (x)]

    for (d = 0; d < 3; d++) {
        u = (d+1) % 3;
        v = (d+2) % 3;
        
        #pragma omp simd
        for (i = 0; i < 3; i++) {
            x[i] = 0;
            q[i] = 0;
        }

        q[d] = 1;
        x[d] = -1;

        while (x[d] < 16) {

            // cook le' mask
            n = 0;
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

                    /*
                     * we use 64 bits for everything. here's the layout:
                     * 
                     * 0-3: pos-x
                     * 4-7: pos-y
                     * 8-11: pos-z
                     * 12-15: size-u
                     * 16-19: size-v
                     * 20-23: normal
                     * 23-31: unused
                     * 32-47: tex-x
                     * 48-63: tex-y
                     */

                    normal = d + flip[n]*3;

                    if (flip[n]) {
                        w2 = w;
                        h2 = h;
                    } else {
                        w2 = h;
                        h2 = w;
                    }

                    // to prevent an overflow, TODO: find a better way
                    pos = {
                        x[0] - (normal == 0 ? 1 : 0),
                        x[1] - (normal == 1 ? 1 : 0),
                        x[2] - (normal == 2 ? 1 : 0),
                    };

                    vertData = 
                        pos.x |
                        (pos.y<<4) |
                        (pos.z<<8) |
                        ((w2-1)<<12) |
                        ((h2-1)<<16) |
                        (normal<<20) |
                        (0UL << 32) |
                        (0UL << 48);
                    
                    tmpVertBuffer[size++] = vertData;

                    #pragma omp simd // 20% improvement just with that? insane.
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
    p_data = tmpVertBuffer;

    // bm->end();
    return size;
}

void editMesh(vec3i p_chunkPos, u64* p_data, u32 p_size) {
    // auto bm = new tools::Benchmark("adding the mesh");
    vec4i ssboData = { p_chunkPos.x, p_chunkPos.y, p_chunkPos.z, 0 };
    IndirectCommand* cmd;

    bool exists = false;
    u32 idx = 0;
    for (auto& e : m_shaderStorageArray) {
        if (e.x == p_chunkPos.x && e.y == p_chunkPos.y && e.z == p_chunkPos.z) {
            exists = true;
            idx = &e - m_shaderStorageArray.data();
            break;
        }
    }

    // create if it doesn't exist
    if (exists == false) {
        idx = createCommand(cmd, p_size);
        m_shaderStorageArray.insert(m_shaderStorageArray.begin() + idx, ssboData);

    // resize and/or move if it does
    } else {
        cmd = &(m_indirectCmds[idx]);
        u32 idxNew = resizeCommand(cmd, p_size);
        if (idx != idxNew) {
            m_shaderStorageArray.erase(m_shaderStorageArray.begin() + idx);
            m_shaderStorageArray.insert(m_shaderStorageArray.begin() + idxNew, ssboData);
        }
    }

    if (m_attribArraySize > m_attribArray.size()) {
        m_attribArray.resize(m_attribArraySize + 1024);
    }

    // despacito
    u64* dataLoc = m_attribArray.data() + cmd->baseInstance;
    std::memcpy(dataLoc, p_data, p_size * sizeof(u64));

    // bm->end();
}

u32 createCommand(IndirectCommand* &p_cmd, u32 p_size) {
    IndirectCommand cmd = {
        .count = 4,
        .instanceCount = p_size,
        .first = 0,
        .baseInstance = 0, // we'll find the right place later
    };

    u32 i = 0;

    // check if empty. this might seem redundant, but it prevents a crash
    if (m_indirectCmds.size() == 0) {
        m_attribArraySize = p_size;
        m_indirectCmds.insert(m_indirectCmds.begin(), cmd);
        p_cmd = m_indirectCmds.data();
        return 0;
    }

    // check the start
    if (m_indirectCmds[0].baseInstance >= p_size) {
        m_indirectCmds.insert(m_indirectCmds.begin(), cmd);
        p_cmd = m_indirectCmds.data();
        return 0;
    }

    // check any other gaps
    for (i = 1; i < m_indirectCmds.size(); i++) {
        if (m_indirectCmds[i].baseInstance - (m_indirectCmds[i-1].baseInstance + m_indirectCmds[i-1].instanceCount) >= p_size) {
            cmd.baseInstance = m_indirectCmds[i-1].baseInstance + m_indirectCmds[i-1].instanceCount;
            m_indirectCmds.insert(m_indirectCmds.begin() + i, cmd);
            p_cmd = &(m_indirectCmds[i]);
            return i;
        }
    }

    // place it in the end
    cmd.baseInstance = m_indirectCmds.back().baseInstance + m_indirectCmds.back().instanceCount;
    m_attribArraySize = cmd.baseInstance + p_size;
    m_indirectCmds.push_back(cmd);
    p_cmd = &(m_indirectCmds.back());
    return m_indirectCmds.size() - 1;
}

u32 resizeCommand(IndirectCommand* &p_cmd, u32 p_newSize) {
    // if we're shrinking, we can just shrink
    if (p_cmd->instanceCount >= p_newSize) {
        p_cmd->instanceCount = p_newSize;
        return p_cmd - m_indirectCmds.data();
    }

    // if at the end, we mignt need to increase the buffer size
    if (p_cmd == m_indirectCmds.data() + m_indirectCmds.size() - 1) {
        if (p_cmd->baseInstance + p_newSize > m_attribArraySize) {
            m_attribArraySize = p_cmd->baseInstance + p_newSize;
        }
        p_cmd->instanceCount = p_newSize;
        return p_cmd - m_indirectCmds.data();
    };

    // keep in mind vectors aren't null-terminated
    // but the last check should prevent us from going out of bounds
    IndirectCommand* next = p_cmd + 1;

    // if we can expand, we expand
    if (p_cmd->baseInstance + p_newSize <= next->baseInstance) {
        p_cmd->instanceCount = p_newSize;
        return p_cmd - m_indirectCmds.data();
    }

    IndirectCommand cmdCopy = *p_cmd;

    // else, we move it. first check if it fits to the front
    if (p_cmd->instanceCount <= m_indirectCmds[0].baseInstance) {
        for (auto cur = p_cmd; cur > m_indirectCmds.data(); cur--) {
            *cur = *(cur-1);
        }
        m_indirectCmds[0] = cmdCopy;
        m_indirectCmds[0].instanceCount = p_newSize;
        p_cmd = m_indirectCmds.data();
        return 0;
    }

    // if not, scan the rest of the array for a gap
    for (u32 i = 1; i < m_indirectCmds.size(); i++) {
        if (m_indirectCmds[i].baseInstance - (m_indirectCmds[i-1].baseInstance + m_indirectCmds[i-1].instanceCount) >= p_newSize) {
            p_cmd->baseInstance = m_indirectCmds[i].baseInstance + m_indirectCmds[i].instanceCount;
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
            p_cmd = m_indirectCmds.data() + i;
            return i;
        }
    }

    // if we're here, we couldn't find any gaps to fit the mesh,
    // so we need to move it to the end
    m_indirectCmds.erase(m_indirectCmds.begin() + (p_cmd - m_indirectCmds.data())); // iterator nonsense
    
    // here lies a bug, six feet under.
    // ripbozo, #packwatch, whatever
    cmdCopy.baseInstance = m_indirectCmds.back().baseInstance + m_indirectCmds.back().instanceCount;
    cmdCopy.instanceCount = p_newSize;
    m_attribArraySize = cmdCopy.baseInstance + cmdCopy.instanceCount;
    
    m_indirectCmds.push_back(cmdCopy);
    p_cmd = m_indirectCmds.data() + m_indirectCmds.size() - 1;
    return m_indirectCmds.size() - 1;
}

};

