#version 330 core

layout (location = 0) in vec3  position;
layout (location = 1) in vec4  color;
layout (location = 2) in vec2  texCoordAtr;
layout (location = 3) in float texIDAtr;

out vec4  vertexColor;
out vec2  texCoord;
out float texID;

uniform mat4 view_projection;

void main()
{
    gl_Position = view_projection * vec4(position, 1.0f);
    vertexColor = color;
    texCoord = texCoordAtr;
    texID = texIDAtr;
}