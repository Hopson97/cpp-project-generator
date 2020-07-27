#version 330

in vec2 passTexCoord;
in vec3 passFragPosition;
in vec3 passNormal;

out vec4 outColour;

uniform sampler2D tex;
uniform vec3 lightPosition;

void main() {
    vec3 normal = normalize(passNormal);
    vec3 lightDirection = normalize(lightPosition - passFragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);

    outColour = texture(tex, passTexCoord) * vec4(diff, diff, diff, 1.0);
}
