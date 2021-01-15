#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D image;

void main()
{
    vec4 texel = texture(image, texCoord);
    if (texel.a < 0.05)
        discard;
    color = texel * vertexColor;
}