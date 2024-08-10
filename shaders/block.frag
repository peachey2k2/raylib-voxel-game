#version 330

// Input vertex attributes (from vertex shader)
flat in ivec2 fragSize;
in vec2 fragTexCoord;
// in vec3 debugColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // Texel color fetching from texture sampler
    // vec4 texelColor = texture(texture0, fragTexCoord);
    vec4 texelColor = texture(
        texture0,
        (floor(fragTexCoord) + fract(fragTexCoord * fragSize)) / 16.0
    );

    // NOTE: Implement here your fragment shader code

    finalColor = texelColor*colDiffuse;
    // finalColor = vec4(debugColor, 1.0);
}