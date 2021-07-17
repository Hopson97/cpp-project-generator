#version 450

in vec2 passTexCoord;
out vec4 outColour;

uniform sampler2D guiTexture;
uniform sampler2D borderTexture;

void main() { 
    vec4 colour = texture(guiTexture, passTexCoord);
    vec4 border = texture(borderTexture, passTexCoord);

    outColour = colour * border;

   // float depth = texture(colourTexture, passTexCoord).r;
    //depth = 1.0 - (1.0 - depth) * 50.0;
    //outColour = vec4(depth);
}