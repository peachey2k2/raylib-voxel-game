#version 430

// Input vertex attributes
in ivec3 data;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
flat out ivec2 fragSize;
out vec2 fragTexCoord;
// out vec3 debugColor;

// NOTE: Add here your custom variables
// uniform vec3 chunkPos;
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
        (data.z & 0x0F) / 16.0,
        (data.z >> 4) / 16.0
    );
}

void main() {
    unpack();

    // Send vertex attributes to fragment shader
    // fragTexCoord = vertexTexCoord;
    // fragTexCoord = tex;
    // fragTexCoord = vec2(tex.x + 1-(gl_VertexID & 1), tex.y + ((gl_VertexID >> 1) & 1));
    // fragTexCoord = vec2(
    //     tex.x + (1-((gl_VertexID & 2)>>1)),
    //     tex.y + ((gl_VertexID & 3)==3 ? 1 : 0)
    // );
    // debugColor = vec3(
    //     (gl_VertexID & 3) == (0 + normal)%4 ? 1 : 0,
    //     (gl_VertexID & 3) == (1 + normal)%4 ? 1 : 0,
    //     (gl_VertexID & 3) == (2 + normal)%4 ? 1 : 0
    // );
    // fragTexCoord = vec2(
    //     tex.x + 1-(gl_VertexID & 1),
    //     tex.y + ((gl_VertexID & 3) % 3 == 0 ? 1 : 0)
    // );
    int magic = gl_VertexID + normal;
    fragTexCoord = vec2(
        (magic & 2) != 0 ? tex.x : tex.x + 1,
        ((magic & 3)%3) != 0 ? tex.y : tex.y + 1
    );

    fragSize = size;

    // Calculate final vertex position
    // gl_Position = mvp*vec4(vertexPosition, 1.0);
    gl_Position = mvp*vec4(pos + chunkPos, 1.0);
}