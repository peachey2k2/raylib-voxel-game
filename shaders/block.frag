#version 430

flat in ivec2 fragSize;
in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main() {
    vec4 texelColor = texture(
        texture0,
        (floor(fragTexCoord) + fract(fragTexCoord * fragSize)) / 16.0
    );

    finalColor = texelColor*colDiffuse;
}