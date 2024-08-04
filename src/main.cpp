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
#include <Actors/Cylinder.h>
#include <Debug.h>

using namespace std;

int main()
{
    Window* window = new Window();
    World* world = new World(window);
    UI* ui = new UI(world, window);
    Camera* camera = new Camera(window, ui);
    Shader* gridShader = new Shader(camera, window, "./Shaders/grid.glsl");
    Shader* basicShader = new Shader(camera, window);

    double lastTime = glfwGetTime();
    
    Sphere s(basicShader, 16, 0.5f);
    Sphere test(basicShader, 16, 0.5f);
    Cylinder cyl(basicShader, 1.0f, 0.1f, 1.3f, 10);
    Cube c(basicShader, 16);
    //Cylinder test1(basicShader, 1.0f, 0.4f, 1.3f, 10);
    //Cylinder test2(basicShader, 1.0f, 1.0f, 1.3f, 10);
    //Cylinder test3(basicShader, 1.0f, 1.4f, 1.3f, 10);

    test.setName("test");
    cyl.setPosition(vec3(2.0f, 0.0f, 0.0f));
    c.setPosition(glm::vec3(-2, 0, 0));
    /*test2.setPosition(vec3(2.0f, 0.0f, 0.0f));
    test3.setPosition(vec3(4.0f, 0.0f, 0.0f));*/

    world->addActor(&test);
    world->addActor(&s);
    world->addActor(&c);


    //Plane p(gridShader, vec3(0,0,0), vec3(1,0,0), vec3(0,0,-1));
    
    //Line l(basicShader, vec3(.0f, -.5f, .0f), vec3(.0f, .5f, .0f),  vec3(1, 0, 0), 10.0f);

    //s.setPosition(glm::vec3(-2, 0, 0));
    //l.setPosition(glm::vec3(2, 0, 0));
    /*c.setPosition(glm::vec3(2, 0, 0));
    p.setScale(vec3(10));
    p.setPosition(vec3(-5, -1, 5));*/
    //world->addActor(&c);
    //world->addActor(&s);
    //world->addActor(&p);
    
    
    while(!window->shouldClose())
    {
        window->clear();
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        // Read input
        camera->tick(deltaTime);
        basicShader->updateViewMatrix(camera->viewMatrix());
        gridShader->updateViewMatrix(camera->viewMatrix());

        
        ui->tick();
        world->tick(deltaTime);
        Debug::tick(deltaTime);

        window->swap();
    }

    window->close();

    return 0;
}

