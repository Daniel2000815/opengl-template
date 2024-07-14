#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
//#include <GL/glew.h>        // GLEW library
#include <glad/glad.h>
#include <GLFW/glfw3.h>     // GLFW library
#include <glm/glm.hpp>
#include <vector>

#include <imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <Window.h>
#include <Shader.h>
#include <Camera.h>
#include <World.h>
#include "Actor.h"
#include "Cube.h"
#include <Sphere.h>
#include <Plane.h>

using namespace std;

// Window
//GLFWwindow* window;
int window_width = 1024;
int window_height = 768;

// FPS
double lastTime = 0.0;
double lastTimeFPS = glfwGetTime();
int nbFrames = 0;



//#pragma region Callbacks
//
//void framebuffer_size_callback(GLFWwindow* window, int new_width, int new_height)
//{
//    window_width = new_width;
//    window_height = new_height;
//    glViewport(0, 0, new_width, new_height);
//    shader->updateWindowDimensions(camera->fov(), new_width, new_height);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera->setFov(camera->fov() + yoffset);
//    shader->updateWindowDimensions(camera->fov(), window_width, window_height);
//}
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (action != GLFW_PRESS)
//        return;
//
//    if (key == GLFW_KEY_L)  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    if (key == GLFW_KEY_F)  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    if (key == GLFW_KEY_P)  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
//
//    if (key == GLFW_KEY_N)
//        for (Actor* obj : world->getActors())
//            obj->setRenderMode(Shader::RenderMode::Normal);
//
//    if (key == GLFW_KEY_C)
//        for (Actor* obj : world->getActors())
//            obj->setRenderMode(Shader::RenderMode::Color);
//
//    if (key == GLFW_KEY_T)
//        for (Actor* obj : world->getActors())
//            obj->setRenderMode(Shader::RenderMode::Texture);
//}
//
//#pragma endregion

void UpdateUI(World* world) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }

    for (auto& object : world->getActors()) {
        if (ImGui::TreeNode(object->name()))
        {
            float position[4] = { object->position().x, object->position().y, object->position().z, 0.0f };
            float rotation[4] = { object->rotation().x, object->rotation().y, object->rotation().z, 0.0f };
            float scale[4] = { object->scale().x, object->scale().y, object->scale().z, 1.0f };

            if (ImGui::InputFloat3("position", position))
                object->setPosition(glm::vec3(position[0], position[1], position[2]));
            if (ImGui::InputFloat3("rotation", rotation))
                object->setRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));
            if (ImGui::InputFloat3("scale", scale))
                object->setScale(glm::vec3(scale[0], scale[1], scale[2]));
            ImGui::TreePop();
        }
    }
}

int main()
{
    //if (!glfwInit())
    //{
    //    cout << "Failed to initialize GLFW" << endl;
    //    return -1;
    //}

    /*glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, "Test", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowUserPointer(window, camera);
    auto func = [](GLFWwindow* w, int b, int a, int m)
    {
        static_cast<Camera*>(glfwGetWindowUserPointer(w))->mouseButtonCallback(w, b, a, m);
    };

    glfwSetMouseButtonCallback(window, func);*/

    // Init glad
    

    Window* window = new Window();
    Camera* camera = new Camera(window);
    Shader* shader = new Shader(camera, window);
    World* world = new World();

    //window->onMouseButton([](GLFWwindow* w, int a, int b, int c) {
    //    std::cout << "Hola, boton pulsado " << a << std::endl;
    //    });

    //window->onMouseButton([](GLFWwindow* w, int a, int b, int c) {
    //    std::cout << "Hola de nuevo, boton pulsado " << b << std::endl;
    //    });



    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //// Ensure we can capture the escape key being pressed below
    //glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // GUI setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window->window(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Z-buffer
    //  Enable depth test
    glEnable(GL_DEPTH_TEST);

    // For transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //  Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    //Grid grid(&shader, 3);


    Sphere s1(shader, 16, 0.5f);
    Cube c1(shader, 16);
    Plane p(shader);


    s1.setPosition(glm::vec3(-2, 0, 0));
    world->AddActor(&c1);
    world->AddActor(&s1);
    world->AddActor(&p);
    

    int timeScale = 1;
    auto waitForDelta = [&](int t_id) { return true; };

    while(!window->shouldClose())
    //while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UpdateUI(world);

        double currentTime = glfwGetTime();
        float deltaTime = timeScale * float(currentTime - lastTime);
        lastTime = currentTime;
        // FPS
        nbFrames++;
        if (currentTime - lastTimeFPS >= 1.0) {
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            nbFrames = 0;
            lastTimeFPS += 1.0;
        }

        // Read input
        //camera->tick(currentTime, *window);
        //shader->updateViewMatrix(camera->position(), camera->position() + camera->forward(), camera->up());

        // Update objects
        for (auto& actor : world->getActors()) {
            if (dynamic_cast<Plane*>(actor) != nullptr)
                continue;

            actor->rotate(20 * deltaTime, glm::vec3(0, 1, 0));
            actor->setPosition(vec3(actor->position().x, sin(currentTime), actor->position().z));
            actor->setScale(vec3(0.5f + (1 + cos(0.5f * currentTime)) / 2.0f));
        }

        world->Tick(deltaTime);

        //c1.PrintTransform();
        //c2.Tick(deltaTime);
        //s1.Tick(deltaTime);
        //stable1.Tick(deltaTime);
        //stable1.Tick(deltaTime);
        //grid.Tick(deltaTime);
        //axis.Tick(deltaTime);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Clean up
        /*glfwSwapBuffers(window);
        glfwPollEvents();*/
        window->tick(deltaTime);
    }

    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

