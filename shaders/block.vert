#version 430

layout(location = 0) in vec3 vertPos;
layout(location = 1) in ivec2 data;

uniform mat4 mvp;

flat out ivec2 fragSize;
out vec2 fragTexCoord;
out vec3 debugColor;

layout(std430, binding = 3) buffer ssbo {
    vec3 chunkPositions[];
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
    normal = (data.x >> 12) & 0x07;
    size = ivec2(
        ((data.x >> 15) & 0x0F) + 1,
        ((data.x >> 19) & 0x0F) + 1
    );
    tex = vec2(
        (data.y & 0x0F) / 16.0,
        (data.y >> 4) / 16.0
    );
}

void main() {
    unpack();

    int magic = gl_VertexID + normal;
    fragTexCoord = vec2(
        (magic & 2) != 0 ? tex.x : tex.x + 1,
        ((magic & 3)%3) != 0 ? tex.y : tex.y + 1
    );

    fragSize = size;
    debugColor = pos / 16.0;

    gl_Position = mvp*vec4(pos + (chunkPositions[gl_InstanceID] * 16.0), 1.0);
    // gl_Position = mvp*vec4(gl_VertexID%2, gl_VertexID, 5-gl_InstanceID, 1.0);
}