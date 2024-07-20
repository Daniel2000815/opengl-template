#ifndef __COLLISIONSOLVER__H_
#define __COLLISIONSOLVER__H_

#include <Physics/Collider.h>
#include <functional>

using CollisionTest = std::function<CollisionData(const Collider*, const Collider*)>;

class CollisionSolver {
public:
    CollisionData solve(const Collider* col1, const Collider* col2);

    CollisionData testPlaneSphere(const PlaneCollider& p, const SphereCollider& s) const;
    CollisionData testPlanePlane(const PlaneCollider& p, const PlaneCollider& s) const;
    CollisionData testSphereSphere(const SphereCollider& s1, const SphereCollider& s2) const;
};

#endif
