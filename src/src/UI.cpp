#include "UI.h"
#include <World.h>
#include <Window.h>

UI::UI(World* world, Window* window)
{
    _world = world;
    _window = window;

    // FPS
    _lastTimeFPS = glfwGetTime();
    _nbFrames = 0;
    _frameTime = 0;

    // GUI setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    _io = &ImGui::GetIO();
    _io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    _io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(_window->window(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::tick()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static int location = 0;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    ImVec2 initial_window_size(400, 300); // Ajusta el tamaño inicial según sea necesario
    ImGui::SetNextWindowSize(initial_window_size, ImGuiCond_FirstUseEver);

    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos;
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

    _nbFrames++;
    if (glfwGetTime() - _lastTimeFPS >= 1.0) {
        _frameTime = std::round( double(_nbFrames) ) / 1000.0;
        _nbFrames = 0;
        _lastTimeFPS += 1.0;
    }
 
    // Aquí estableces el título de la ventana
    ImGui::Begin(std::format("OpenGL template | {} fps | {} ms/fr", std::round(_io->Framerate), _frameTime).c_str(), nullptr, window_flags);

    for (auto& object : _world->actors()) {
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

    // Finaliza la ventana ImGui
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UI::mouseHovering()
{
    return _io->WantCaptureMouse;
}
