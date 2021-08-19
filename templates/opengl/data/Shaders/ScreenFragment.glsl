#version 450

in vec2 passTexCoord;
out vec4 outColour;

uniform sampler2D colourTexture;

void main() { 
    outColour = texture(colourTexture, passTexCoord);



   // float depth = texture(colourTexture, passTexCoord).r;
    //depth = 1.0 - (1.0 - depth) * 50.0;
    //outColour = vec4(depth);
}