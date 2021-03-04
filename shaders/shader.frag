#version 330 core

in vec4 vertexColor;
in vec2 texCoord;
in float texID;

out vec4 color;

uniform sampler2D images[32];

void main()
{
    vec4 texel = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    switch(int(texID))
    {
        case  0: texel = texture(images[ 0], texCoord); break;
        case  1: texel = texture(images[ 1], texCoord); break;
        case  2: texel = texture(images[ 2], texCoord); break;
        case  3: texel = texture(images[ 3], texCoord); break;
        case  4: texel = texture(images[ 4], texCoord); break;
        case  5: texel = texture(images[ 5], texCoord); break;
        case  6: texel = texture(images[ 6], texCoord); break;
        case  7: texel = texture(images[ 7], texCoord); break;
        case  8: texel = texture(images[ 8], texCoord); break;
        case  9: texel = texture(images[ 9], texCoord); break;
        case 10: texel = texture(images[10], texCoord); break;
        case 11: texel = texture(images[11], texCoord); break;
        case 12: texel = texture(images[12], texCoord); break;
        case 13: texel = texture(images[13], texCoord); break;
        case 14: texel = texture(images[14], texCoord); break;
        case 15: texel = texture(images[15], texCoord); break;
        case 16: texel = texture(images[16], texCoord); break;
        case 17: texel = texture(images[17], texCoord); break;
        case 18: texel = texture(images[18], texCoord); break;
        case 19: texel = texture(images[19], texCoord); break;
        case 20: texel = texture(images[20], texCoord); break;
        case 21: texel = texture(images[21], texCoord); break;
        case 22: texel = texture(images[22], texCoord); break;
        case 23: texel = texture(images[23], texCoord); break;
        case 24: texel = texture(images[24], texCoord); break;
        case 25: texel = texture(images[25], texCoord); break;
        case 26: texel = texture(images[26], texCoord); break;
        case 27: texel = texture(images[27], texCoord); break;
        case 28: texel = texture(images[28], texCoord); break;
        case 29: texel = texture(images[29], texCoord); break;
        case 30: texel = texture(images[30], texCoord); break;
        case 31: texel = texture(images[31], texCoord); break;
    }
    if (texel.a < 0.05)
        discard;
    color = texel * vertexColor;
}