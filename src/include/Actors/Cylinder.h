#ifndef __CYLINDER__H_
#define __CYLINDER__H_

#include "Actor.h"
#include <Physics/Collider.h>
#include "Utils.h"


class Cylinder : public Actor
{
public:
    Cylinder(Shader* shader, float baseRadius, float topRadius, float height, int sectorCount);
};

#endif