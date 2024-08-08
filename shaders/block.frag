#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

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
    vec4 texelColor = texture(texture0, fract(fragTexCoord * 16) / 16);

    // NOTE: Implement here your fragment shader code

    finalColor = texelColor*colDiffuse;
}