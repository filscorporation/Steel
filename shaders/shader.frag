#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D image;

void main()
{
    color = texture(image, texCoord) * vertexColor;
}