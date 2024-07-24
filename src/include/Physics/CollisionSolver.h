#ifndef __COLLISIONSOLVER__H_
#define __COLLISIONSOLVER__H_

#include <Physics/Collider.h>
#include <Actors/Actor.h>  // Incluye el encabezado de Transform
#include <functional>

using CollisionTest = std::function<CollisionData(const Collider*, const Collider*, const Transform*, const Transform*)>;

class CollisionSolver {
public:
    CollisionData solve(const Collider* col1, const Collider* col2, const Transform* trans1, const Transform* trans2);

    CollisionData testPlaneSphere(const PlaneCollider& p, const SphereCollider& s, const Transform& t1, const Transform& t2) const;
    CollisionData testPlanePlane(const PlaneCollider& p, const PlaneCollider& s, const Transform& t1, const Transform& t2) const;
    CollisionData testSphereSphere(const SphereCollider& s1, const SphereCollider& s2, const Transform& t1, const Transform& t2) const;
};

#endif
