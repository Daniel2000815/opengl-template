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
    std::vector<vec3> cubeSeparatingAxes (const Cube& boxA, const Cube& boxB) const;
    std::pair<float, float> projectCube(const Cube& box, const vec3& axis) const;
    float computeCubePenetration(const Cube& boxA, const Cube& boxB, const vec3& axis) const;
    std::pair<vec3, vec3> computeCubeContactPoints(const Cube& boxA, const Cube& boxB, const vec3& bestAxis) const;


};

#endif
