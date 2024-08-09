#version 330

// Input vertex attributes
// in vec3 vertexPosition;
// in vec2 vertexTexCoord;
in ivec3 data;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;
out vec4 debugColor;

// NOTE: Add here your custom variables
uniform vec3 chunkPos;

vec3 pos;
vec2 tex; 

void unpack() {
    pos = vec3(
        data.x & 0x1F,
        (data.x >> 5) & 0x1F,
        (data.x >> 10) & 0x1F
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
    fragTexCoord = tex;

    // Calculate final vertex position
    // gl_Position = mvp*vec4(vertexPosition, 1.0);
    gl_Position = mvp*vec4(pos + chunkPos, 1.0);
    debugColor = vec4(
        fract(chunkPos.x/160),
        fract(chunkPos.y/160),
        fract(chunkPos.z/160),
        1.0
    );
}