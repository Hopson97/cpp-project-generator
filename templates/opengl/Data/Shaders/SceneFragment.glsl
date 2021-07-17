#version 450 core

in vec2 passTextureCoord;
in vec3 passNormal;
in vec3 passFragPosition;

out vec4 outColour;

uniform sampler2D diffuseTexture;

uniform vec3 lightColour;
uniform vec3 lightPosition;
uniform vec3 eyePosition;

uniform bool isLight;

void main()
{
    outColour = texture(diffuseTexture, passTextureCoord);
    if (!isLight) {
        vec3 ambient = 0.1 * lightColour;

        vec3 normal = normalize(passNormal);
        vec3 lightDir = normalize(lightPosition - passFragPosition);
        vec3 eyeDirection = normalize(eyePosition - passFragPosition);

        vec3 reflectDirection = reflect(-lightDir, normal);

        float spec = pow(max(dot(eyeDirection, reflectDirection), 0.0), 32);
        vec3 specular = 0.5 * spec * lightColour;

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = lightColour * diff;

        vec4 result = vec4(ambient + specular + diffuse, 1.0); 
        outColour *= result;
    }
}