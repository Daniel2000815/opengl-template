#include <Actors/Sphere.h>
#include <Actors/Cube.h>

Sphere::Sphere(Shader* shader, uint16_t resolution, float radius) : Actor(shader, "Sphere") {
    Cube c(shader, resolution);
    _radius = radius;
    _vertices = c.vertices();
    _normals.assign(_vertices.size(), 0.0f);

    for (int i = 0; i < _vertices.size() / 3; i++) {
        glm::vec3 p(_vertices[3 * i], _vertices[3 * i + 1], _vertices[3 * i + 2]);
        p = glm::normalize(p);

        _vertices[3 * i] = p[0] * radius;
        _vertices[3 * i + 1] = p[1] * radius;
        _vertices[3 * i + 2] = p[2] * radius;

        _normals[3 * i] = p[0];
        _normals[3 * i + 1] = p[1];
        _normals[3 * i + 2] = p[2];
    }

    _indices = c.indices();
    _texCoords = c.uv();
    _colors = c.colors();
    _renderMode = Shader::RenderMode::Color;
    _collider = new SphereCollider(vec3(0.0f), radius);

    bindResources();

}

float Sphere::radius() const
{
    return std::max( _radius * _transform->scale.x, std::max(_radius * _transform->scale.y, _radius * _transform->scale.z));
}

glm::mat3 Sphere::inertiaTensor() const
{
    float I = (2.0f / 5.0f) * _mass * _radius * _radius;
    
    glm::mat3 inertiaMatrixLocal = glm::mat3(
        I, 0.0f, 0.0f,
        0.0f, I, 0.0f,
        0.0f, 0.0f, I
    );

    return rotationMatrix() * inertiaMatrixLocal * glm::transpose(rotationMatrix());
}

void Sphere::setRadius(float r)
{
    for (int i = 0; i < _vertices.size() / 3; i++) {
        glm::vec3 p(_vertices[3 * i], _vertices[3 * i + 1], _vertices[3 * i + 2]);
        p = glm::normalize(p);

        _vertices[3 * i] = p[0] * r;
        _vertices[3 * i + 1] = p[1] * r;
        _vertices[3 * i + 2] = p[2] * r;
    }
}

