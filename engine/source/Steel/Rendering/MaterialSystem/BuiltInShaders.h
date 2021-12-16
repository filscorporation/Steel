#pragma once

#include "Shader.h"

namespace BuiltInShaders
{
    const char* DefaultSpriteVS =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec4 color;\n"
"layout (location = 2) in vec2 texCoordAtr;\n"
"\n"
"out vec4 vertexColor;\n"
"out vec2 texCoord;\n"
"\n"
"uniform mat4 view_projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = view_projection * vec4(position, 1.0f);\n"
"    vertexColor = color;\n"
"    texCoord = texCoordAtr;\n"
"}";

    const char* DefaultSpriteFS =
"#version 330 core\n"
"\n"
"in vec4 vertexColor;\n"
"in vec2 texCoord;\n"
"\n"
"out vec4 color;\n"
"\n"
"uniform sampler2D main_tex;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 texel = texture(main_tex, texCoord);\n"
"    color = texel * vertexColor;\n"
"    if (color.a < 0.01) discard;\n"
"}";

    const char* DefaultMeshVS =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec4 color;\n"
"\n"
"out vec4 vertexColor;\n"
"\n"
"uniform mat4 view_projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = view_projection * vec4(position, 1.0f);\n"
"    vertexColor = color;\n"
"}";

    const char* DefaultMeshFS =
"#version 330 core\n"
"\n"
"in vec4 vertexColor;\n"
"\n"
"out vec4 color;\n"
"\n"
"void main()\n"
"{\n"
"    color = vertexColor;\n"
"    if (color.a < 0.01) discard;\n"
"}";

    const char* DefaultUIVS =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec4 color;\n"
"layout (location = 2) in vec2 texCoordAtr;\n"
"\n"
"out vec4 vertexColor;\n"
"out vec2 texCoord;\n"
"\n"
"uniform mat4 view_projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = view_projection * vec4(position, 1.0f);\n"
"    vertexColor = color;\n"
"    texCoord = texCoordAtr;\n"
"}";

    const char* DefaultUIFS =
"#version 330 core\n"
"\n"
"in vec4 vertexColor;\n"
"in vec2 texCoord;\n"
"\n"
"out vec4 color;\n"
"\n"
"uniform sampler2D main_tex;\n"
"\n"
"void main()\n"
"{\n"
"    vec4 texel = texture(main_tex, texCoord);\n"
"    color = texel * vertexColor;\n"
"    if (color.a < 0.01) discard;\n"
"}";

    const char* DefaultUIClippingVS =
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 position;\n"
"\n"
"uniform mat4 view_projection;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = view_projection * vec4(position, 1.0f);\n"
"}";

    const char* DefaultUIClippingFS =
"#version 330 core\n"
"\n"
"out vec4 color;\n"
"\n"
"void main()\n"
"{\n"
"}";
}
