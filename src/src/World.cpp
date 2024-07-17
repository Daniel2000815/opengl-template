#include "World.h"
#include <Actors/Plane.h>

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

}

void World::addActor(Actor* actor)
{
    _actors.push_back(actor);
}

void World::tick(float deltaTime) {
    deltaTime *= _timeScale;

    // Test
    auto pingPong = [](float t, float a, float b) -> float {
        float length = b - a;
        t = fmod(t, length * 2);
        if (t < length)
            return a + t;
        return b - (t - length);
    };

    if (_timeScale > 0) {
        for (Actor*& actor : _actors) {
            if (dynamic_cast<Plane*>(actor) != nullptr)
                continue;

            actor->rotate(20 * deltaTime, glm::vec3(0, 1, 0));
            actor->setScale(vec3(pingPong(glfwGetTime() * _timeScale * 0.2f, 0.5f, 1.5f)));
            actor->setPosition(vec3(actor->position().x, pingPong(glfwGetTime() * _timeScale, 0.0f, 1.0f), actor->position().z));
        }
    }

    for (Actor* actor : _actors) {
        actor->tick(deltaTime);
    }
}
