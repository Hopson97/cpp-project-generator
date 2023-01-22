#include "Mesh.h"

#include <glm/gtc/noise.hpp>
#include <iostream>

Mesh createQuadMesh()
{
    // clang-format off
    Mesh mesh;
    mesh.vertices = {
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };
    // clang-format on
    mesh.indices = {0, 1, 2, 2, 3, 0};
    return mesh;
}

Mesh createCubeMesh(const glm::vec3& dimensions)
{
    Mesh mesh;

    float w = dimensions.x;
    float h = dimensions.y;
    float d = dimensions.z;

    // clang-format off
    mesh.vertices = {
        {{w, h, d}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},  
        {{0, h, d}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0, 0, d}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},  
        {{w, 0, d}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

        {{0, h, d}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, 
        {{0, h, 0}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{0, 0, 0}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}}, 
        {{0, 0, d}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

        {{0, h, 0}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, 
        {{w, h, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{w, 0, 0}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}}, 
        {{0, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},

        {{w, h, 0}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},  
        {{w, h, d}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{w, 0, d}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},  
        {{w, 0, 0}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},

        {{w, h, 0}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  
        {{0, h, 0}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0, h, d}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},  
        {{w, h, d}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

        {{0, 0, 0}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, 
        {{w, 0, 0}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{w, 0, d}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}, 
        {{0, 0, d}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
    };
    // clang-format on

    int currIndex = 0;
    for (int i = 0; i < 6; i++) {
        mesh.indices.push_back(currIndex);
        mesh.indices.push_back(currIndex + 1);
        mesh.indices.push_back(currIndex + 2);
        mesh.indices.push_back(currIndex + 2);
        mesh.indices.push_back(currIndex + 3);
        mesh.indices.push_back(currIndex);
        currIndex += 4;
    }

    return mesh;
}

namespace {

    struct TerrainGenOptions {
        float roughness;
        float smoothness;
        float amplitude;

        int octaves;
        float offset;
    };

    float getNoiseAt(const glm::ivec2& position, int seed, TerrainGenOptions& options)
    {
        float value = 0;
        float acc = 0;
        for (int i = 0; i < options.octaves; i++) {
            float frequency = glm::pow(2.0f, i);
            float amplitude = glm::pow(options.roughness, i);

            float x = position.x * frequency / options.smoothness;
            float z = position.y * frequency / options.smoothness;

            float noiseValue = glm::simplex(glm::vec3{x, z, seed});
            noiseValue = (noiseValue + 1.0f) / 2.0f;
            value += noiseValue * amplitude;
            acc += amplitude;
        }
        return value / acc * options.amplitude + options.offset;
    }

} // namespace

Mesh createTerrainMesh(int size, int edgeVertices, bool isFlat)
{
    int heightMapWidth = edgeVertices + 2;
    float fEdgeVertexCount = static_cast<float>(edgeVertices);

    TerrainGenOptions terrainNoise;
    terrainNoise.roughness = 0.7;
    terrainNoise.smoothness = 350.0f;
    terrainNoise.octaves = 5;
    terrainNoise.amplitude = 45.0f;
    terrainNoise.offset = -33;

    TerrainGenOptions bumpNoise;
    bumpNoise.roughness = 0.6;
    bumpNoise.smoothness = 50.0f;
    bumpNoise.octaves = 5;
    bumpNoise.amplitude = 5.0f;
    bumpNoise.offset = 0;

    std::vector<float> heights(heightMapWidth * heightMapWidth);
    if (!isFlat) {
        for (int z = 0; z < heightMapWidth; z++) {
            for (int x = 0; x < heightMapWidth; x++) {
                int tx = x + (heightMapWidth - 1);
                int tz = z + (heightMapWidth - 1);
                float height = getNoiseAt({tx, tz}, 123, terrainNoise);
                float bumps = getNoiseAt({tx, tz}, 123, bumpNoise);
                heights[z * heightMapWidth + x] = height + bumps;
            }
        }
    }

    auto getHeight = [&](int x, int y) {
        if (x < 0 || x >= heightMapWidth || y < 0 || y >= heightMapWidth) {
            return 0.0f;
        }
        else {
            return heights[y * heightMapWidth + x];
        }
    };

    Mesh mesh;
    for (int z = 0; z < edgeVertices; z++) {
        for (int x = 0; x < edgeVertices; x++) {
            GLfloat fz = static_cast<GLfloat>(z);
            GLfloat fx = static_cast<GLfloat>(x);

            int hx = x + 1;
            int hz = z + 1;
            float height = isFlat ? 0 : getHeight(hx, hz);

            Vertex vertex;
            vertex.position.x = fx / fEdgeVertexCount * size;
            vertex.position.y = height;
            vertex.position.z = fz / fEdgeVertexCount * size;

            vertex.textureCoord.s = (fx / fEdgeVertexCount) * edgeVertices / 4;
            vertex.textureCoord.t = (fz / fEdgeVertexCount) * edgeVertices / 4;

            float h1 = getHeight(hx - 1, hz);
            float h2 = getHeight(hx + 1, hz);
            float h3 = getHeight(hx, hz - 1);
            float h4 = getHeight(hx, hz + 1);
            vertex.normal = glm::normalize(glm::vec3{h1 - h2, 2, h3 - h4});

            mesh.vertices.push_back(vertex);
        }
    }

    for (int z = 0; z < edgeVertices - 1; z++) {
        for (int x = 0; x < edgeVertices - 1; x++) {
            int topLeft = (z * edgeVertices) + x;
            int topRight = topLeft + 1;
            int bottomLeft = ((z + 1) * edgeVertices) + x;
            int bottomRight = bottomLeft + 1;

            mesh.indices.push_back(topLeft);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(topRight);
            mesh.indices.push_back(topRight);
            mesh.indices.push_back(bottomLeft);
            mesh.indices.push_back(bottomRight);
        }
    }
    return mesh;
}