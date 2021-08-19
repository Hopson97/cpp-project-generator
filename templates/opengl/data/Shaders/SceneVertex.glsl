#version 450 core

layout (location = 0) in vec3 inVertexCoord;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

out vec2 passTextureCoord;
out vec3 passNormal;
out vec3 passFragPosition;

void main()
{
    vec4 worldPos = modelMatrix * vec4(inVertexCoord, 1.0);
    
    gl_Position = projectionViewMatrix * worldPos;
    passTextureCoord = inTexCoord;

    passNormal = mat3(transpose(inverse(modelMatrix))) * inNormal;
    passFragPosition = vec3(worldPos);
}