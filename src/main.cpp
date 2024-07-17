#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

#include <Window.h>
#include <Shader/Shader.h>
#include <Camera.h>
#include <World.h>
#include <UI.h>
#include "Actors/Actor.h"
#include "Actors/Cube.h"
#include <Actors/Sphere.h>
#include <Actors/Line.h>
#include <Actors/Plane.h>

using namespace std;

int main()
{
    Window* window = new Window();
    World* world = new World(window);
    UI* ui = new UI(world, window);
    Camera* camera = new Camera(window, ui);
    Shader* gridShader = new Shader(camera, window, "./Shaders/grid.glsl");
    Shader* basicShader = new Shader(camera, window);

    int timeScale = 1;
    double lastTime = glfwGetTime();
    
    //Grid grid(&shader, 3);
    Sphere s(basicShader, 16, 0.5f);
    Plane p(gridShader, vec3(0,0,0), vec3(1,0,0), vec3(0,0,-1));
    Cube c(basicShader, 16);
    Line l(basicShader, vec3(.0f, -.5f, .0f), vec3(.0f, .5f, .0f),  vec3(1, 0, 0), 10.0f);

    s.setPosition(glm::vec3(-2, 0, 0));
    l.setPosition(glm::vec3(2, 0, 0));
    p.setScale(vec3(10));
    p.setPosition(vec3(-5, -1, 5));
    world->addActor(&c);
    world->addActor(&s);
    world->addActor(&p);
    world->addActor(&l);
    
    while(!window->shouldClose())
    {
        window->clear();
        double currentTime = glfwGetTime();
        float deltaTime = timeScale * float(currentTime - lastTime);
        lastTime = currentTime;

        // Read input
        camera->tick(deltaTime);
        basicShader->updateViewMatrix(camera->viewMatrix());
        gridShader->updateViewMatrix(camera->viewMatrix());

        world->tick(deltaTime);
        ui->tick();
        window->swap();
    }

    window->close();

    return 0;
}

