#include <algorithm>
#include <Physics/CollisionSolver.h>
#include <Actors/Sphere.h>
#include <Actors/Cube.h>
#include <Actors/Line.h>
#include <Debug.h>
#include <Utils.h>

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
std::pair<vec3, float> CollisionSolver::solve(const Actor* col1, const Actor* col2)
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
    if (!data.hit)
        return std::make_pair(vec3(0.0f), 0.0f);

    printf("COLLISION %s %s", col1->name(), col2->name());
    if (swap)
    {
        std::swap(data.p1, data.p2);
        data.normal = -data.normal;
        data.mtv = -data.mtv;
    }

    float velocityAlongNormal = (swap ? -1 : 1) * glm::dot(col2->transform()->velocity - col1->transform()->velocity, data.normal);
    float elasticity = (col2->elasticity() + col1->elasticity()) / 2.0f;
    printf("ELASTIC %f", elasticity);
    float impulseMagnitude = (-(1.0f + elasticity) * velocityAlongNormal) / ((1 / col1->mass() + 1 / col2->mass()));

    Utils::printVec("final", impulseMagnitude * data.normal);
    return std::make_pair(impulseMagnitude * data.normal, 0.0f);
}

CollisionData CollisionSolver::testCubeSphere(const Cube& c, const Sphere& s) const
{
    /*const SphereCollider* sCol = dynamic_cast<const SphereCollider*>(s.collider());
    const BoxCollider* bCol = dynamic_cast<const BoxCollider*>(c.collider());*/

    glm::vec3 closestPoint = Utils::closestPointToCube(s.transform()->position, c);

    //Debug::drawSphere(c.shader(), closestPoint, 0.1f, vec3(0.0f));

    glm::vec3 vectorToCenter =  -closestPoint + s.transform()->position;
    float distanceSquared = glm::dot(vectorToCenter, vectorToCenter);

    //Debug::drawLine(c.shader(), closestPoint, closestPoint + vectorToCenter);
    if (distanceSquared > s.radius() * s.radius()) {
        printf("FFFFFFFF %f, %f\n", distanceSquared, s.radius() * s.radius());
        return CollisionData(); // No hay colisión
    }

    float distance = glm::sqrt(distanceSquared);
    glm::vec3 normal = vectorToCenter / distance;
    float penetrationDepth = s.radius() - distance;
    glm::vec3 mtv = normal * penetrationDepth;

    return CollisionData(normal, mtv);
}

CollisionData CollisionSolver::testCubeCube(const Cube& boxA, const Cube& boxB) const
{
    auto axes = cubeSeparatingAxes(boxA, boxB);
    float minPenetration = std::numeric_limits<float>::infinity();
    vec3 bestAxis;

    for (const auto& axis : axes) {
        float penetration = computeCubePenetration(boxA, boxB, axis);
        if (penetration < 0) {
            return CollisionData();  // No intersection
        }
        if (penetration < minPenetration) {
            minPenetration = penetration;
            bestAxis = axis;
        }
    }

    auto mtv = - bestAxis * minPenetration;
    auto contactPoints = computeCubeContactPoints(boxA, boxB, bestAxis);

    Debug::drawLine(boxA.shader(), contactPoints.first, contactPoints.second);
    return CollisionData(contactPoints.first, contactPoints.second, mtv);
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
    vec3 mtv = normal * ((c1->radius() + c2->radius()) - glm::length(centervector));
    return CollisionData(c1->center() + t1->position + normal * c1->radius(), c2->center() + t2->position - normal * c2->radius(), mtv);
}

std::vector<vec3> CollisionSolver::cubeSeparatingAxes(const Cube& boxA, const Cube& boxB) const
{
    std::vector<vec3> axes;

    auto orientationA = boxA.rotationMatrix();
    auto orientationB = boxB.rotationMatrix();

    // Face normals of boxA (also the edge vectors)
    axes.push_back(orientationA[0]);
    axes.push_back(orientationA[1]);
    axes.push_back(orientationA[2]);

    // Face normals of boxB (also the edge vectors)
    axes.push_back(orientationB[0]);
    axes.push_back(orientationB[1]);
    axes.push_back(orientationB[2]);

    // Cross products of edges from boxA and boxB
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vec3 edgeA = vec3(orientationA[i][0], orientationA[i][1], orientationA[i][2]);
            vec3 edgeB = vec3(orientationB[j][0], orientationB[j][1], orientationB[j][2]);

            vec3 crossProduct = glm::cross(edgeA, edgeB);
            if (crossProduct.x != 0 || crossProduct.y != 0 || crossProduct.z != 0) {
                axes.push_back(glm::normalize(crossProduct));
            }
        }
    }

    return axes;
}

std::pair<float, float> CollisionSolver::projectCube(const Cube& box, const vec3& axis) const
{
    vec3 normalizedAxis = glm::normalize(axis);
    auto vertices = box.verticesWorld();
    float minProjection = glm::dot(vertices[0], normalizedAxis);
    float maxProjection = minProjection;

    for (const auto& vertex : vertices) {
        float projection = glm::dot(vertex, normalizedAxis);
        minProjection = minProjection < projection ? minProjection : projection;
        maxProjection = maxProjection > projection ? maxProjection : projection;
    }

    return { minProjection, maxProjection };
}

float CollisionSolver::computeCubePenetration(const Cube& boxA, const Cube& boxB, const vec3& axis) const
{
    auto [minA, maxA] = projectCube(boxA, axis);
    auto [minB, maxB] = projectCube(boxB, axis);

    if (maxA < minB || maxB < minA) {
        return -1;
    }

    return (std::min)(maxA, maxB) - (std::max)(minA, minB);  // Calculate overlap
}

std::pair<vec3, vec3> CollisionSolver::computeCubeContactPoints(const Cube& boxA, const Cube& boxB, const vec3& bestAxis) const
{
    vec3 normalizedAxis = glm::normalize(bestAxis);
    float centerA_proj = glm::dot(boxA.transform()->position, normalizedAxis);
    float centerB_proj = glm::dot(boxB.transform()->position, normalizedAxis);

    vec3 contactPointA, contactPointB;
    if (centerA_proj < centerB_proj) {
        contactPointA = boxA.transform()->position + normalizedAxis * (centerB_proj - centerA_proj);
        contactPointB = boxB.transform()->position - normalizedAxis * (centerB_proj - centerA_proj);
    }
    else {
        contactPointA = boxA.transform()->position - normalizedAxis * (centerA_proj - centerB_proj);
        contactPointB = boxB.transform()->position + normalizedAxis * (centerA_proj - centerB_proj);
    }

    return { contactPointA, contactPointB };
}
