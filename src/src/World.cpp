#include "World.h"
#include <Actors/Cube.h>
#include <Physics/Collider.h>  // delete this after test
#include <Debug.h>

World::World(Window* window) {
    _actors = std::vector<Actor*>{};
    
    _window = window;
    _window->onKey([this](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action != GLFW_PRESS)
            return;

        else if (key == GLFW_KEY_L)  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else if (key == GLFW_KEY_F)  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else if (key == GLFW_KEY_P)  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

        else if (key == GLFW_KEY_N)
            for (Actor* obj : actors())
                obj->setRenderMode(Shader::RenderMode::Normal);

        else if (key == GLFW_KEY_C)
            for (Actor* obj : actors())
                obj->setRenderMode(Shader::RenderMode::Color);

        else if (key == GLFW_KEY_T)
            for (Actor* obj : actors())
                obj->setRenderMode(Shader::RenderMode::Texture);
    });

    _collisionSolver = new CollisionSolver();

    // test
    printf("TESTING\n");
    _collisionSolver->solve(new PlaneCollider(), new SphereCollider(), new Transform(), new Transform());
    _collisionSolver->solve(new SphereCollider(), new SphereCollider(),new Transform(), new Transform());
    _collisionSolver->solve(new SphereCollider(), new PlaneCollider(), new Transform(), new Transform());
    _collisionSolver->solve(new PlaneCollider(), new PlaneCollider(), new Transform(), new Transform());
}

void World::addActor(Actor* actor)
{
    _actors.push_back(actor);
}

void World::solveDynamics(float delta)
{
    // Test
    auto pingPong = [](float t, float a, float b) -> float {
        float length = b - a;
        t = fmod(t, length * 2);
        if (t < length)
            return a + t;
        return b - (t - length);
        };

    for (Actor*& actor : _actors) {
        //if (dynamic_cast<Cube*>(actor) != nullptr)
        if(strcmp(actor->name(), "test") == 0)
            actor->setPosition(vec3(pingPong(glfwGetTime() * _timeScale * 2, -2.0f, 2.0f), actor->position().y, actor->position().z));

        //actor->setColor(vec3(sin(glfwGetTime())));
        //actor->rotate(20 * delta, glm::vec3(0, 1, 0));
        //actor->setScale(vec3(pingPong(glfwGetTime() * _timeScale * 0.2f, 0.5f, 1.5f)));
        //actor->setPosition(vec3(actor->position().x, pingPong(glfwGetTime() * _timeScale, 0.0f, 1.0f), actor->position().z));
    }
}

void World::solveCollisions()
{
    std::vector<CollisionData> collisions;

    printf("----------\n");
    for (Actor* a : _actors)
        for (Actor* b : _actors)
        {
            
            if (a == b) break;
            if (!a->collider() || !b->collider())   continue;

            printf("Testing %s vs %s\n", a->name(), b->name());
            CollisionData colData = _collisionSolver->solve(
                a->collider(), b->collider(), a->transform(), b->transform()
            );

            if (colData.hit) {
                a->setColor(vec3(1.0f, 0.0f, 0.0f));
                b->setColor(vec3(0.0f, 0.0f, 1.0f));
                printf("Normal: (%f, %f, %f)\n", colData.normal.x, colData.normal.y, colData.normal.z);
                Debug::drawLine(a->shader(), a->position(), a->position() + colData.normal, vec3(0.0f, 1.0f, 0.0f), 50.0f);
                collisions.push_back(colData);
            }
            else {
                a->setColor(vec3(1.0f, 1.0f, 1.0f));
                b->setColor(vec3(1.0f, 1.0f, 1.0f));
            }
        }
    printf("----------\n");
}

void World::tick(float deltaTime) {
    deltaTime *= _timeScale;

    if (_timeScale > 0) {
        solveDynamics(deltaTime);
        solveCollisions();
    }

    for (Actor* actor : _actors) {
        actor->tick(deltaTime);
    }
}
