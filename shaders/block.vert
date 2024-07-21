#version 330

// Input vertex attributes
// in vec3 vertexPosition;
in ivec3 data;

// Input uniform values
uniform mat4 mvp;
uniform int side;

// Output vertex attributes (to fragment shader)
out vec2 fragTexCoord;

// NOTE: Add here your custom variables
vec3 vertexPosition;
vec2 vertexTexCoord;

int extract4(int at) {
    return (data.x >> at) & 0xF;
}

int extract10(int at) {
    return (data.x >> at) & 0x3FF;
}

void unpackData() {
    vertexPosition = vec3(
        extract4(0),
        extract4(4),
        extract4(8)
    );

    vertexTexCoord = vec2(
        extract10(12),
        extract10(22)
    );
}

void main() {
    // Send vertex attributes to fragment shader
    unpackData();
    fragTexCoord = vertexTexCoord;

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}