#ifndef __LINE__H_
#define __LINE__H_

#include <Actors/Actor.h>

class Line : public Actor
{
private:
    glm::vec3 _startPoint;
    glm::vec3 _endPoint;
    glm::vec3 _color;
    float _width;

public:
    Line(Shader* shader, glm::vec3 color = glm::vec3(1.0f), float width = 1.f);
    Line(Shader* shader, glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(1.0f), float width = 1.f);

    void addPoint(glm::vec3 p);
    void setWidth(float width) { _width = width; }
    uint16_t nPoints() const { return _vertices.size() / 3; }

    mat3 inertiaTensor() const override;

    void tick(float deltaTime = 0.0f) override;

};

#endif