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
        // BOX     // LINE  // SPHERE                
        { make_collision_test<decltype(&CollisionSolver::testBoxBox), BoxCollider, BoxCollider>(&CollisionSolver::testBoxBox, this), nullptr, make_collision_test<decltype(&CollisionSolver::testBoxSphere), BoxCollider, SphereCollider>(&CollisionSolver::testBoxSphere, this) }, // BOX
        { nullptr, nullptr, nullptr }, // LINE
        { nullptr, nullptr, make_collision_test<decltype(&CollisionSolver::testSphereSphere), SphereCollider, SphereCollider>(&CollisionSolver::testSphereSphere, this) }  // SPHERE
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

CollisionData CollisionSolver::testBoxSphere(const BoxCollider& p, const SphereCollider& s, const Transform& t1, const Transform& t2) const
{
    printf("Box vs SPHERE\n");
    return CollisionData();
}

CollisionData CollisionSolver::testBoxBox(const BoxCollider& p, const BoxCollider& s, const Transform& t1, const Transform& t2) const
{
    printf("Box vs Box\n");
    return CollisionData();
}

CollisionData CollisionSolver::testSphereSphere(const SphereCollider& s1, const SphereCollider& s2, const Transform& t1, const Transform& t2) const
{
    vec3 centerVector = (t2.position + s2.center()) - (t1.position + s1.center());

    // TODO: work with non uniform scale
    if (glm::length(centerVector) > t1.scale.x * s1.radius() + t2.scale.x *s2.radius()) {
        
        return CollisionData();
    }

    vec3 normal = glm::normalize(centerVector);
    printf("Normal1: (%f, %f, %f)\n", normal.x, normal.y, normal.z);
    return CollisionData(s1.center() + normal * s1.radius(), s2.center() - normal * s2.radius());
}

std::vector<vec3> CollisionSolver::boxSeparatingAxes(const BoxCollider& boxA, const BoxCollider& boxB)
{
    std::vector<vec3> axes;

    //auto orientationA = boxA.eulerToRotationMatrix();
    //auto orientationB = boxB.eulerToRotationMatrix();

    //// Face normals of boxA (also the edge vectors)
    //axes.push_back(orientationA[0]);
    //axes.push_back(orientationA[1]);
    //axes.push_back(orientationA[2]);

    //// Face normals of boxB (also the edge vectors)
    //axes.push_back(orientationB[0]);
    //axes.push_back(orientationB[1]);
    //axes.push_back(orientationB[2]);

    //// Cross products of edges from boxA and boxB
    //for (const auto& edgeA : orientationA) {
    //    for (const auto& edgeB : orientationB) {
    //        Vec3 crossProduct = edgeA.cross(edgeB);
    //        if (crossProduct.x != 0 || crossProduct.y != 0 || crossProduct.z != 0) {
    //            axes.push_back(crossProduct.normalize());
    //        }
    //    }
    //}

    return axes;
}
