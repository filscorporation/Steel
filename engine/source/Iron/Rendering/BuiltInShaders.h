#pragma once

#include "Shader.h"

namespace BuiltInShaders
{
    const GLchar* VertexShader =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3  position;\n"
"layout (location = 1) in vec4  color;\n"
"layout (location = 2) in vec2  texCoordAtr;\n"
"layout (location = 3) in float texIDAtr;\n"
"\n"
"out vec4  vertexColor;\n"
"out vec2  texCoord;\n"
"flat out float texID;\n"
"\n"
"uniform mat4 view_projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = view_projection * vec4(position, 1.0f);\n"
"    vertexColor = color;\n"
"    texCoord = texCoordAtr;\n"
"    texID = texIDAtr;\n"
"}";

    const GLchar* FragmentShader =
"#version 330 core\n"
"\n"
"in vec4 vertexColor;\n"
"in vec2 texCoord;\n"
"flat in float texID;\n"
"\n"
"out vec4 color;\n"
"\n"
"uniform sampler2D images[32];\n"
"uniform int draw_mode;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 texel = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"    switch(int(texID))\n"
"    {\n"
"        case  0: texel = texture(images[ 0], texCoord); break;\n"
"        case  1: texel = texture(images[ 1], texCoord); break;\n"
"        case  2: texel = texture(images[ 2], texCoord); break;\n"
"        case  3: texel = texture(images[ 3], texCoord); break;\n"
"        case  4: texel = texture(images[ 4], texCoord); break;\n"
"        case  5: texel = texture(images[ 5], texCoord); break;\n"
"        case  6: texel = texture(images[ 6], texCoord); break;\n"
"        case  7: texel = texture(images[ 7], texCoord); break;\n"
"        case  8: texel = texture(images[ 8], texCoord); break;\n"
"        case  9: texel = texture(images[ 9], texCoord); break;\n"
"        case 10: texel = texture(images[10], texCoord); break;\n"
"        case 11: texel = texture(images[11], texCoord); break;\n"
"        case 12: texel = texture(images[12], texCoord); break;\n"
"        case 13: texel = texture(images[13], texCoord); break;\n"
"        case 14: texel = texture(images[14], texCoord); break;\n"
"        case 15: texel = texture(images[15], texCoord); break;\n"
"        case 16: texel = texture(images[16], texCoord); break;\n"
"        case 17: texel = texture(images[17], texCoord); break;\n"
"        case 18: texel = texture(images[18], texCoord); break;\n"
"        case 19: texel = texture(images[19], texCoord); break;\n"
"        case 20: texel = texture(images[20], texCoord); break;\n"
"        case 21: texel = texture(images[21], texCoord); break;\n"
"        case 22: texel = texture(images[22], texCoord); break;\n"
"        case 23: texel = texture(images[23], texCoord); break;\n"
"        case 24: texel = texture(images[24], texCoord); break;\n"
"        case 25: texel = texture(images[25], texCoord); break;\n"
"        case 26: texel = texture(images[26], texCoord); break;\n"
"        case 27: texel = texture(images[27], texCoord); break;\n"
"        case 28: texel = texture(images[28], texCoord); break;\n"
"        case 29: texel = texture(images[29], texCoord); break;\n"
"        case 30: texel = texture(images[30], texCoord); break;\n"
"        case 31: texel = texture(images[31], texCoord); break;\n"
"    }\n"
"    switch(draw_mode)\n"
"    {\n"
"        case 0: color = vec4(0.0, 0.0, 0.0, 1.0); break;\n"
"        case 1: color = texel * vertexColor; break;\n"
"        case 2: color = vec4(vertexColor.rgb, texel.r); break;\n"
"    }\n"
"    if (color.a < 0.01) discard;\n"
"}";
}