#ifndef __SPHERE__H_
#define __SPHERE__H_

#include "Actor.h"
#include <Physics/Collider.h>

class Sphere : public Actor
{
public:
    Sphere(Shader* shader, uint16_t resolution = 8, float radius = 1.f);
};

#endif