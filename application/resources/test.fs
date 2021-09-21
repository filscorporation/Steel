#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D main_tex;
uniform sampler2D secondary_tex;
uniform vec4 in_color;
uniform int in_int;

void main()
{
    vec4 texel = vec4(texture(main_tex, texCoord).rgb, texture(secondary_tex, texCoord).a);
    color = texel * in_color;
    if (color.a < 0.01) discard;
};