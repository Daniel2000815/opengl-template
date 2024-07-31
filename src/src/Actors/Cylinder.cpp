#include "Actors/Cylinder.h"

// https://gist.github.com/Pikachuxxxx/a2454e98bb7723afd7251a891c18f194
Cylinder::Cylinder(Shader* shader, float baseRadius, float topRadius, float height, int sectorCount) : Actor(shader, "Cylinder")
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float heightStep = height / sectorCount;
    float radiusStep = (topRadius - baseRadius) / sectorCount;

    float sectorAngle, sectorHeight, sectorRadius;
    float x, y, z;

    // Genera vértices y normales para el cilindro
    for (int height_i = 0; height_i <= sectorCount; ++height_i) {
        float y = -height * 0.5f + height_i * heightStep;
        float radius = baseRadius + (topRadius - baseRadius) * (height_i / float(sectorCount));
        float dR_dy = (topRadius - baseRadius) / height;

        for (int angle_i = 0; angle_i <= sectorCount; ++angle_i) {
            float theta = angle_i * sectorStep;

            // Coordenadas del punto en la superficie
            float x = cos(theta) * radius;
            float z = sin(theta) * radius;

            // Calcula la normal
            glm::vec3 normal(cos(theta), -dR_dy, sin(theta));
            normal = glm::normalize(normal);

            vertices.push_back(glm::vec3(x, y, z));
            normals.push_back(normal);
        }
    }

    // Genera vértices y normales para las tapas
    for (int angle_i = 0; angle_i <= sectorCount; ++angle_i) {
        float sectorAngle = angle_i * sectorStep;

        // Tapa inferior
        float xBase = cos(sectorAngle) * baseRadius;
        float zBase = sin(sectorAngle) * baseRadius;
        vertices.push_back(glm::vec3(xBase, -height * 0.5f, zBase));
        normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

        // Tapa superior
        float xTop = cos(sectorAngle) * topRadius;
        float zTop = sin(sectorAngle) * topRadius;
        vertices.push_back(glm::vec3(xTop, height * 0.5f, zTop));
        normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // Centro de las tapas
    vertices.push_back(glm::vec3(0.0f, -height * 0.5f, 0.0f));
    normals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    vertices.push_back(glm::vec3(0.0f, height * 0.5f, 0.0f));
    normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

    // Genera índices para unir los vértices en triángulos
    for (int height_i = 0; height_i < sectorCount; ++height_i) {
        for (int angle_i = 0; angle_i < sectorCount; ++angle_i) {
            int current = height_i * (sectorCount + 1) + angle_i;
            int next = current + 1;
            int above = (height_i + 1) * (sectorCount + 1) + angle_i;
            int aboveNext = above + 1;

            // Dos triángulos por sector
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(above);

            indices.push_back(above);
            indices.push_back(next);
            indices.push_back(aboveNext);
        }
    }

    // Índices para las tapas
    int baseCenterIndex = vertices.size() - 2;
    int topCenterIndex = vertices.size() - 1;
    for (int angle_i = 0; angle_i < sectorCount; ++angle_i) {
        int currentBase = (sectorCount + 1) * (sectorCount + 1) + angle_i * 2;
        int nextBase = currentBase + 2;

        int currentTop = currentBase + 1;
        int nextTop = nextBase + 1;

        // Tapa inferior
        indices.push_back(baseCenterIndex);
        indices.push_back(currentBase);
        indices.push_back(nextBase);

        // Tapa superior
        indices.push_back(topCenterIndex);
        indices.push_back(nextTop);
        indices.push_back(currentTop);
    }

    // Convertir los datos a arrays de OpenGL
    Utils::copyVecToArray(vertices, _vertices);
    Utils::copyVecToArray(normals, _normals);
    _indices = indices;
    _colors.assign(_vertices.size(), 1.f);
    _renderMode = Shader::RenderMode::Normal;

    bindResources();
}
