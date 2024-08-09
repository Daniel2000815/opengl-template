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
    _renderMode = Shader::RenderMode::Normal;
    _collider = new SphereCollider(vec3(0.0f), radius);

    bindResources();

}

float Sphere::radius() const
{
    return std::max( _radius * _transform->scale.x, std::max(_radius * _transform->scale.y, _radius * _transform->scale.z));
}
