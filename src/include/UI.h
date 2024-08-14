#ifndef __UI__H_
#define __UI__H_

#include <format> 
#include <iostream> 

#include <imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

using namespace std;

class UI {

private:
    class World* _world;
    class Window* _window;
    ImGuiIO* _io;

    double _lastTimeFPS;
    int _nbFrames;

    double _frameTime;

public:
    UI(World* world, Window* window);
    ~UI();

    void tick();
    bool mouseHovering() const;
};

#endif