#include <Physics/CollisionSolver.h>
#include <Actors/Sphere.h>
#include <Actors/Cube.h>
#include <Actors/Line.h>

template <typename Func, typename ColliderType1, typename ColliderType2>
CollisionTest make_collision_test(Func func, const CollisionSolver* solver) {
    return [solver, func](const Actor* c1, const Actor* c2) -> CollisionData {
        return (solver->*func)(
            static_cast<const ColliderType1&>(*c1),
            static_cast<const ColliderType2&>(*c2)
            );
        };
}

// https://winter.dev/articles/physics-engine
CollisionData CollisionSolver::solve(const Actor* col1, const Actor* col2)
{
    static const CollisionTest tests[ColliderType::N][ColliderType::N] =
    {
        // Cube     // LINE  // SPHERE                
        { make_collision_test<decltype(&CollisionSolver::testCubeCube), Cube, Cube>(&CollisionSolver::testCubeCube, this), nullptr, make_collision_test<decltype(&CollisionSolver::testCubeSphere), Cube, Sphere>(&CollisionSolver::testCubeSphere, this) }, // Cube
        { nullptr, nullptr, nullptr }, // LINE
        { nullptr, nullptr, make_collision_test<decltype(&CollisionSolver::testSphereSphere), Sphere, Sphere>(&CollisionSolver::testSphereSphere, this) }  // SPHERE
    };

    assert(col1->collider()->type() < ColliderType::N);
    assert(col2->collider()->type() < ColliderType::N);

    bool swap = col1->collider()->type() > col2->collider()->type();
    if (swap)
    {
        std::swap(col1, col2);
    }

    CollisionData data = tests[col1->collider()->type()][col2->collider()->type()](col1, col2);

    if (swap)
    {
        std::swap(data.p1, data.p2);
        data.normal = -data.normal;
    }

    return data;
}

CollisionData CollisionSolver::testCubeSphere(const Cube& p, const Sphere& s) const
{
    printf("Cube vs SPHERE\n");
    return CollisionData();
}

CollisionData CollisionSolver::testCubeCube(const Cube& p, const Cube& s) const
{
    printf("Cube vs Cube\n");
    return CollisionData();
}

CollisionData CollisionSolver::testSphereSphere(const Sphere& s1, const Sphere& s2) const
{
    const Transform* t1 = s1.transform();
    const Transform* t2 = s2.transform();
    const SphereCollider* c1 = dynamic_cast<const SphereCollider*>(s1.collider());
    const SphereCollider* c2 = dynamic_cast<const SphereCollider*>(s2.collider());

    vec3 centervector = (t2->position + c2->center()) - (t1->position + c1->center());

    // todo: work with non uniform scale
    if (glm::length(centervector) > t1->scale.x * c1->radius() + t2->scale.x * c2->radius()) {
        
        return CollisionData();
    }

    vec3 normal = glm::normalize(centervector);
    printf("normal1: (%f, %f, %f)\n", normal.x, normal.y, normal.z);
    return CollisionData(c1->center() + normal * c1->radius(), c2->center() - normal * c2->radius());
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
