#include <Physics/CollisionSolver.h>

template <typename Func, typename ColliderType1, typename ColliderType2>
CollisionTest make_collision_test(Func func, const CollisionSolver* solver) {
    return [solver, func](const Collider* c1, const Collider* c2, const Transform* t1, const Transform* t2) -> CollisionData {
        return (solver->*func)(
            static_cast<const ColliderType1&>(*c1),
            static_cast<const ColliderType2&>(*c2),
            *t1,
            *t2
            );
        };
}

// https://winter.dev/articles/physics-engine
CollisionData CollisionSolver::solve(const Collider* col1, const Collider* col2, const Transform* trans1, const Transform* trans2)
{
    static const CollisionTest tests[ColliderType::N][ColliderType::N] =
    {
        // BOX     // PLANE                                                       // LINE  // SPHERE                
        { nullptr, nullptr,                                                      nullptr, nullptr }, // BOX
        { nullptr, make_collision_test<decltype(&CollisionSolver::testPlanePlane), PlaneCollider, PlaneCollider>(&CollisionSolver::testPlanePlane, this), nullptr, make_collision_test<decltype(&CollisionSolver::testPlaneSphere), PlaneCollider, SphereCollider>(&CollisionSolver::testPlaneSphere, this) }, // PLANE
        { nullptr, nullptr,                                                      nullptr, nullptr }, // LINE
        { nullptr, nullptr,                                                      nullptr, make_collision_test<decltype(&CollisionSolver::testSphereSphere), SphereCollider, SphereCollider>(&CollisionSolver::testSphereSphere, this) }  // SPHERE
    };

    assert(col1->type() < ColliderType::N);
    assert(col2->type() < ColliderType::N);

    bool swap = col1->type() > col2->type();
    if (swap)
    {
        std::swap(col1, col2);
        std::swap(trans1, trans2); // Asegúrate de intercambiar también las transformaciones
    }

    CollisionData data = tests[col1->type()][col2->type()](col1, col2, trans1, trans2);

    if (swap)
    {
        std::swap(data.p1, data.p2);
        data.normal = -data.normal;
    }

    return data;
}

CollisionData CollisionSolver::testPlaneSphere(const PlaneCollider& p, const SphereCollider& s, const Transform& t1, const Transform& t2) const
{
    printf("PLANE vs SPHERE\n");
    return CollisionData();
}

CollisionData CollisionSolver::testPlanePlane(const PlaneCollider& p, const PlaneCollider& s, const Transform& t1, const Transform& t2) const
{
    printf("PLANE vs PLANE\n");
    return CollisionData();
}

CollisionData CollisionSolver::testSphereSphere(const SphereCollider& s1, const SphereCollider& s2, const Transform& t1, const Transform& t2) const
{
    vec3 centerVector = (t2.position + s2.center()) - (t1.position + s1.center());

    printf("pos 1: %f %f %f\n", t1.position.x, t1.position.y, t1.position.z);
    printf("pos 2: %f %f %f\n", t2.position.x, t2.position.y, t2.position.z);
    printf("center: %f %f %f\n", centerVector.x, centerVector.y, centerVector.z);
    printf("Distance: %f, Radi: %f\n", glm::length(centerVector), s1.radius() + s2.radius());

    if (glm::length(centerVector) > s1.radius() + s2.radius()) {
        
        return CollisionData();
    }

    vec3 normal = glm::normalize(centerVector);
    return CollisionData(s1.center() + normal * s1.radius(), s2.center() + normal * s2.radius());
}
