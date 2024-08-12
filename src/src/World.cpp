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

    _gravity = vec3(0.0f);

    // test
    //printf("TESTING\n");
    //_collisionSolver->solve(new BoxCollider(), new SphereCollider(), new Transform(), new Transform());
    //_collisionSolver->solve(new SphereCollider(), new SphereCollider(),new Transform(), new Transform());
    //_collisionSolver->solve(new SphereCollider(), new BoxCollider(), new Transform(), new Transform());
    //_collisionSolver->solve(new BoxCollider(), new BoxCollider(), new Transform(), new Transform());
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
        actor->addVelocity(_gravity / actor->mass() * delta);
        //if (dynamic_cast<Cube*>(actor) != nullptr)
        /*if(strcmp(actor->name(), "test2") == 0)
            actor->setPosition(vec3(pingPong(glfwGetTime() * _timeScale * 2, -2.0f, 2.0f), actor->position().y, actor->position().z));*/
        //if (strcmp(actor->name(), "test1") == 0)
            //actor->setPosition(vec3(actor->position().x, pingPong(glfwGetTime() * _timeScale * 1.5f, -4.0f, 4.0f), actor->position().z));
        //actor->setColor(vec3(sin(glfwGetTime())));
        //actor->rotate(20 * delta, glm::vec3(0, 1, 0));
        //actor->setScale(vec3(pingPong(glfwGetTime() * _timeScale * 0.2f, 0.5f, 1.5f)));
        //actor->setPosition(vec3(actor->position().x, pingPong(glfwGetTime() * _timeScale, 0.0f, 1.0f), actor->position().z));
    }
}

void World::solveCollisions()
{

    printf("a----------\n");
    for (Actor* a : _actors) {
        bool col = false;
        for (Actor* b : _actors)
        {
            if (a == b) break;
            if (!a->collider() || !b->collider())   continue;

            //CollisionData colData = _collisionSolver->solve(a, b);
            std::pair<vec3, float> colResponse = _collisionSolver->solve(a, b);

            a->addVelocity(-colResponse.first / a->mass());
            b->addVelocity(+colResponse.first / b->mass());            
        }

        //if (!col) a->setColor(vec3(1.0f, 1.0f, 1.0f));
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
