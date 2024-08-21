#version 460

// layout(location = 0) in vec3 vertPos;
layout(location = 1) in ivec2 data;

uniform mat4 mvp;

flat out ivec2 fragSize;
out vec2 fragTexCoord;
out vec3 debugColor;

layout(std430, binding = 3) buffer ssbo {
    ivec4 chunkPositions[];
};

vec3 pos;
int normal;
ivec2 size;
vec2 tex; 

void unpack() {
    pos = vec3(
        data.x & 0x0F,
        (data.x >> 4) & 0x0F,
        (data.x >> 8) & 0x0F
    );
    size = ivec2(
        ((data.x >> 12) & 0x0F) + 1,
        ((data.x >> 16) & 0x0F) + 1
    );
    normal = (data.x >> 20) & 0x07;
    tex = vec2(
        (data.y & 0x0F) / 16.0,
        (data.y >> 4) / 16.0
    );
    if (normal < 3) {
        pos[normal] += 1;
    }
}

void main() {
    unpack();

//     const f32 QUAD_VERTICES[6*4] = {
//     1, 0, 0,
//     0, 0, 0,
//     1, 1, 0,
//     0, 1, 0,
// };
    vec3 vertPos = vec3(
        1-(gl_VertexID & 1),
        (gl_VertexID>>1 & 1),
        0
    );

    int magic = gl_VertexID + normal;
    fragTexCoord = vec2(
        (magic & 2) != 0 ? tex.x : tex.x + 1,
        ((magic & 3)%3) != 0 ? tex.y : tex.y + 1
    );

    fragSize = size;
    // debugColor = vec3(normal&1, (normal>>1)&1, (normal>>2)&1);
    debugColor = pos/16.0;

    vec3 newVertPos = vec3(vertPos.x*size.x, vertPos.y*size.y, vertPos.z);
    switch (normal) {
        case 0:
            newVertPos = newVertPos.zyx;
            break;
        case 1:
            newVertPos = newVertPos.xzy;
            break;
        case 2:
            newVertPos = newVertPos.yxz;
            break;
        case 3:
            newVertPos = newVertPos.zxy;
            break;
        case 4:
            newVertPos = newVertPos.yzx;
            break;
        case 5:
            newVertPos = newVertPos.xyz;
            break;
    }

    gl_Position = mvp*vec4(pos + newVertPos + (chunkPositions[gl_DrawID].xyz * 16.0), 1.0);
    // gl_Position = mvp*vec4(pos + newVertPos + (chunkPositions[1].xyz * 16.0), 1.0);
    // gl_Position = mvp*vec4(pos + vertPos + gl_InstanceID, 1.0);
    // gl_Position = mvp*vec4(gl_VertexID%2, gl_VertexID, 5-gl_InstanceID, 1.0);
}