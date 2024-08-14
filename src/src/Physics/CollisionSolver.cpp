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
    if (!data.hit) {
        data.response1 = { vec3(0.0f), vec3(0.0f) };
        data.response2 = { vec3(0.0f), vec3(0.0f) };
        return data;
    }

    if (swap)
    {
        std::swap(data.p1, data.p2);
        data.normal = -data.normal;
        data.mtv = -data.mtv;
    }

    float velocityAlongNormal = (swap ? -1 : 1) * glm::dot(col2->transform()->velocity - col1->transform()->velocity, data.normal);
    float elasticity = (col2->elasticity() + col1->elasticity()) / 2.0f;
    float impulseMagnitude = (-(1.0f + elasticity) * velocityAlongNormal) / ((1 / col1->mass() + 1 / col2->mass()));

    vec3 linearForce2 = impulseMagnitude * data.normal;
    vec3 linearForce1 = -linearForce2;
    vec3 angularForce1 = glm::cross(linearForce1, data.p1 - col1->transform()->position );
    vec3 angularForce2 = glm::cross(linearForce2, data.p2 - col2->transform()->position);

    data.response1 = { linearForce1, angularForce1 };
    data.response2 = { linearForce2, angularForce2 };

    return data;
}

CollisionData CollisionSolver::testCubeSphere(const Cube& c, const Sphere& s) const
{
    glm::vec3 closestPoint = Utils::closestPointToCube(s.transform()->position, c);

    glm::vec3 vectorToCenter =  -closestPoint + s.transform()->position;
    float distanceSquared = glm::dot(vectorToCenter, vectorToCenter);

    if (distanceSquared > s.radius() * s.radius()) {
        return CollisionData(); // No hay colisión
    }

    float distance = glm::sqrt(distanceSquared);
    glm::vec3 normal = vectorToCenter / distance;
    float penetrationDepth = s.radius() - distance;
    glm::vec3 mtv = normal * penetrationDepth;

    glm::vec3 contactPointSphere = s.transform()->position - normal * s.radius();
    glm::vec3 contactPointCube = closestPoint;

    return CollisionData(contactPointCube, contactPointSphere, mtv);
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

    // Invertir el eje para obtener el Movimiento de Translación Mínimo (MTV)
    auto mtv = -bestAxis * minPenetration;

    // Usar el bestAxis para calcular los puntos de contacto
    auto contactPoints = computeCubeContactPoints(boxA, boxB, bestAxis);

    // Dibujar los puntos de contacto para depuración (opcional)
    Debug::drawLine(boxA.shader(), contactPoints.first, contactPoints.second);

    // Crear y devolver los datos de colisión con los puntos de contacto y el MTV
    return CollisionData(contactPoints.first, contactPoints.second, mtv);
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

    //vec3 normalizedAxis = glm::normalize(bestAxis);

    // Transformar los centros de las cajas al espacio del mundo
    //auto centerA = boxA.transform()->position;
    //auto centerB = boxB.transform()->position;

    //// Proyectar los centros en el eje de separación
    //float centerA_proj = glm::dot(centerA, normalizedAxis);
    //float centerB_proj = glm::dot(centerB, normalizedAxis);

    //vec3 contactPointA, contactPointB;
    //if (centerA_proj < centerB_proj) {
    //    contactPointA = centerA + normalizedAxis * (centerB_proj - centerA_proj) * 0.5f;
    //    contactPointB = centerB - normalizedAxis * (centerB_proj - centerA_proj) * 0.5f;
    //}
    //else {
    //    contactPointA = centerA - normalizedAxis * (centerA_proj - centerB_proj) * 0.5f;
    //    contactPointB = centerB + normalizedAxis * (centerA_proj - centerB_proj) * 0.5f;
    //}

    //return { contactPointA, contactPointB };
}
