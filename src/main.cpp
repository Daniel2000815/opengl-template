#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include <Window.h>
#include <Shader.h>
#include <Camera.h>
#include <World.h>
#include <UI.h>
#include "Actor.h"
#include "Cube.h"
#include <Sphere.h>
#include <Plane.h>

using namespace std;

int main()
{
    Window* window = new Window();
    World* world = new World(window);
    UI* ui = new UI(world, window);
    Camera* camera = new Camera(window, ui);
    Shader* shader = new Shader(camera, window);

    int timeScale = 1;
    double lastTime = glfwGetTime();
    
    //Grid grid(&shader, 3);
    Sphere s(shader, 16, 0.5f);
    Cube c(shader, 16);
    Plane p(shader);

    s.setPosition(glm::vec3(-2, 0, 0));
    world->addActor(&c);
    world->addActor(&s);
    world->addActor(&p);
    
    while(!window->shouldClose())
    {
        window->clear();
        double currentTime = glfwGetTime();
        float deltaTime = timeScale * float(currentTime - lastTime);
        lastTime = currentTime;

        // Read input
        camera->tick(currentTime);
        shader->updateViewMatrix(camera->viewMatrix());

        // Update objects
        for (auto& actor : world->actors()) {
            if (dynamic_cast<Plane*>(actor) != nullptr)
                continue;

            actor->rotate(20 * deltaTime, glm::vec3(0, 1, 0));
            actor->setPosition(vec3(actor->position().x, sin(currentTime), actor->position().z));
            actor->setScale(vec3(0.5f + (1 + cos(0.5f * currentTime)) / 2.0f));
        }

        world->tick(deltaTime);
        ui->tick();
        window->swap();
    }

    window->close();

    return 0;
}

