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

Actor& testScene1(World& world, std::vector<Shader*> shaders, Camera* cam) {
    static Sphere s1(shaders[0], 3, 0.5f);
    static Sphere s2(shaders[0], 3, 0.5f);

    static Cube c1(shaders[0], 2);
    static Cube c2(shaders[0], 2);
    static Cube c3(shaders[0], 2);

    cam->setPosition(vec3(3.0f, 3.0f, 5.0f));
    cam->setRotation(3.6f, -0.5f);

    c1.setColor(vec3(1, 0, 0));
    c2.setColor(vec3(1, 1, 0));
    c3.setColor(vec3(1, 0, 1));
    s1.setColor(vec3(1, 1, 1));
    s2.setColor(vec3(0, 1, 1));

    c1.setName("c1");
    c2.setName("c2");
    c3.setName("c3");
    s1.setName("s1");
    s2.setName("s2");

    c1.setPosition(vec3(-4.0f, 0.0f, 0.0f));
    c2.setPosition(vec3(-2.0f, 0.0f, 0.0f));
    c3.setPosition(vec3(4.0f, 0.0f, 0.0f));
    s1.setPosition(vec3(0.0f, 0.0f, 0.0f));
    s2.setPosition(vec3(2.0f, 0.0f, 0.0f));

    c1.setElasticity(0.0f); c1.setMass(0.5f);
    c2.setElasticity(0.2f); c2.setMass(1.0f);
    c3.setElasticity(0.4f); c3.setMass(1.2f);
    s1.setElasticity(0.7f); s1.setMass(2.0f);
    s2.setElasticity(1.0f); s2.setMass(2.0f);

    c3.addVelocity(vec3(-2.0f, 0.0f, 0.0f));

    world.addActor(&c1);
    world.addActor(&c2);
    world.addActor(&c3);
    world.addActor(&s1);
    world.addActor(&s2);

    return c2;
}

void testScene2(World& world, std::vector<Shader*> shaders, Camera* cam) {
    static Sphere s(shaders[0], 3, 0.5f);

    static Cube c1(shaders[1], 2);
    //static Cube c2(shaders[1], 2);

    cam->setPosition(vec3(-5.046048, 1.820663, 3.864308));
    cam->setRotation(-4.264772, -0.205169);

    c1.setName("floor");
    //c2.setName("ground");
    s.setName("sphere");

    s.setMass(1);

    c1.setKinematic(true);
    //c2.setKinematic(true);

    c1.rotate(vec3(0, 0, -0.15f));

    c1.scale(vec3(30.0f, 0.3f, 10.0f));
    //c2.scale(vec3(30.0f, 0.3f, 10.0f));

    c1.setPosition(vec3(2.0f, -2.0f, 0.0f));
    //c2.setPosition(vec3(0.0f, 2.0f, 0.0f));

    s.setPosition(vec3(0, -0.5f, 0));
    s.setRenderMode(Shader::RenderMode::Texture);
    s.addVelocity(vec3(0.0f, -3.0f, 0.0f));

    world.setGravity(vec3(0.0f, -9.8f, 0.0f));

    world.addActors(&c1, &s);
}

void testScene3(World& world, std::vector<Shader*> shaders, Camera* cam) {
    static Sphere s(shaders[0], 3, 0.5f);

    static Cube c1(shaders[1], 2);

    world.setGravity(vec3(0.0f, -9.8f, 0.0f));

    cam->setPosition(vec3(4.0f, 1.0f, 4.0f));
    cam->setRotation(-2.5, -0.1);

    c1.setName("ground");
    s.setName("sphere");

    c1.setKinematic(true);
    s.setElasticity(3.0f);

    c1.scale(vec3(30.0f, 0.3f, 10.0f));
    c1.setPosition(vec3(0, -2, 0));

    world.addActors(&c1, &s);
}

void testScene4(World& world, std::vector<Shader*> shaders, Camera* cam) {
    static Cube c1(shaders[0], 2);

    cam->setPosition(vec3(4.0f, 1.0f, 4.0f));
    cam->setRotation(-2.5, -0.1);

    c1.setName("Cube");

    world.addActors(&c1);

    c1.addTorque(vec3(0, 1, 0));    // 1 spin per second
}

void testScene5(World& world, std::vector<Shader*> shaders, Camera* cam){
    //static Sphere s1(shader, 3, 0.5f);
    //static Sphere s2(shader, 3, 0.5f);

    static Cube s1(shaders[0], 2);
    static Cube s2(shaders[0], 2);
    //static Cube c3(shader, 2);

    cam->setPosition(vec3(3.0f, 3.0f, 5.0f));
    cam->setRotation(3.6f, -0.5f);

    //c1.setColor(vec3(1, 0, 0));
    //c2.setColor(vec3(1, 1, 0));
    //c3.setColor(vec3(1, 0, 1));
    s1.setRenderMode(Shader::RenderMode::Texture);
    s2.setRenderMode(Shader::RenderMode::Texture);

    //c1.setPosition(vec3(-4.0f, 0.0f, 0.0f));
    //c2.setPosition(vec3(-2.0f, 0.0f, 0.0f));
    //c3.setPosition(vec3(4.0f, 0.0f, 0.0f));
    s1.setPosition(vec3(-3.0f, 0.0f, 1.0f));
    s2.setPosition(vec3(0.0f, 0.0f, 0.0f));

    s1.addVelocity(vec3(1, 0, 0));


    world.addActors(&s1, &s2);
}

int main()
{
    Window* window = new Window();
    World* world = new World(window);
    UI* ui = new UI(world, window);
    Camera* camera = new Camera(window, ui);
    Shader* gridShader = new Shader(camera, window, "./Shaders/grid.glsl");
    Shader* basicShader = new Shader(camera, window);

    double lastTime = glfwGetTime();
    
    bool paused = false;
    window->onKey([&paused, &lastTime](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action != GLFW_PRESS && key == GLFW_KEY_Z)
            paused = !paused;

        if(!paused)
            lastTime = glfwGetTime();
    });
    
    testScene2(*world, { basicShader, gridShader }, camera);
    while(!window->shouldClose())
    {
        if (!paused) {

            window->clear();
            double currentTime = glfwGetTime();
            float deltaTime = float(currentTime - lastTime);
            lastTime = currentTime;

            // Read input
            camera->tick(deltaTime);
            basicShader->updateViewMatrix(camera->viewMatrix());
            gridShader->updateViewMatrix(camera->viewMatrix());

            Debug::drawLine(basicShader, vec3(0.0f), vec3(100.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 5.0f);
            Debug::drawLine(basicShader, vec3(0.0f), vec3(0.0f, 100.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 5.0f);
            Debug::drawLine(basicShader, vec3(0.0f), vec3(0.0f, 0.0f, 100.0f), vec3(0.0f, 0.0f, 1.0f), 5.0f);
            
            world->tick(deltaTime);
            ui->tick();
            Debug::tick(deltaTime);
        }
        
        window->swap();
    }

    window->close();

    return 0;
}

