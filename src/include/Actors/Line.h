#ifndef __LINE__H_
#define __LINE__H_

#include <Actors/Actor.h>

class Line : public Actor
{
private:
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    glm::vec3 color;
    float width;

public:
    Line(Shader* shader, glm::vec3 color = glm::vec3(1.0f), float width = 1.f);
    Line(Shader* shader, glm::vec3 start, glm::vec3 end, glm::vec3 color = glm::vec3(1.0f), float width = 1.f);

    void tick(float deltaTime) override;
    void addPoint(glm::vec3 p);

};

#endif