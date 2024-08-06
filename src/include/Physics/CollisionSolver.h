#ifndef __COLLISIONSOLVER__H_
#define __COLLISIONSOLVER__H_

#include <Physics/Collider.h>
#include <Actors/Actor.h>  // Incluye el encabezado de Transform
#include <functional>

using CollisionTest = std::function<CollisionData(const Actor*, const Actor*)>;

class Cube;
class Sphere;

class CollisionSolver {
public:
    CollisionData solve(const Actor* col1, const Actor* col2);

    CollisionData testCubeSphere(const Cube& p, const Sphere& s) const;
    CollisionData testCubeCube(const Cube& p, const Cube& s) const;
    CollisionData testSphereSphere(const Sphere& s1, const Sphere& s2) const;

private:
    std::vector<vec3> boxSeparatingAxes(const BoxCollider& boxA, const BoxCollider& boxB);
};

#endif
