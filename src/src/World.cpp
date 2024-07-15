#include "World.h"

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
    for (Actor* actor : _actors) {
        //actor->UpdateVelocity(actors, deltaTime);
        //actor->UpdatePosition(deltaTime);
        actor->tick(deltaTime);
    }
}
