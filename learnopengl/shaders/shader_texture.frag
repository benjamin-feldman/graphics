#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// Defaults to 0, and reads texture bound to unit 0 at draw time
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixAmount;

void main()
{
    // exercise 1
    vec2 texture2Coord = vec2(1.0f - TexCoord.x, TexCoord.y);

    FragColor = mix(texture(texture1, TexCoord), texture(texture2, texture2Coord), mixAmount);
}
