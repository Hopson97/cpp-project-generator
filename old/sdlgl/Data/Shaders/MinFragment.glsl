#version 450 core

in vec2 passTextureCoord;

out vec4 outColour;

uniform sampler2D tex;

void main()
{
    outColour = texture(tex, passTextureCoord);
}