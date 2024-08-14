#ifndef __PLANE__H_
#define __PLANE__H_

#include "Actor.h"

class Plane : public Actor
{
    public:
        Plane(Shader* shader, glm::vec3 startPoint = glm::vec3(0.f,0.f,0.f), glm::vec3 right = glm::vec3(1.f,0.f,0.f), glm::vec3 up = glm::vec3(0.f,1.f,0.f), glm::vec2 resolution = glm::vec2(1.f,1.f));
        mat3 inertiaTensor() const override;
};

#endif