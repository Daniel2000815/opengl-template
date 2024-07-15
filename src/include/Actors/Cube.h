#ifndef __CUBE__H_
#define __CUBE__H_

#include "Actor.h"

class Cube : public Actor
{
    public:
        Cube(Shader* shader, uint16_t resolution = 1);

    private:
        void drawFace(glm::vec3 startPoint, glm::vec3 v1, glm::vec3 v2, glm::vec2 resolution);
};

#endif