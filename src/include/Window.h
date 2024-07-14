#ifndef __WINDOW__H_
#define __WINDOW__H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>     // GLFW library
#include <iostream>         // cout, cerr
#include <functional>

typedef void (*resize_callback)(GLFWwindow*, int, int);
typedef void (*mouse_button_callback)(GLFWwindow*, int, int, int);
typedef void (*scroll_callback)(GLFWwindow*, double, double);
typedef void (*key_callback)(GLFWwindow*, int, int, int, int);

class Window {

private:
    GLFWwindow* _window;
    int _width = 1024, _height = 768;

    std::vector<mouse_button_callback> _mouse_button_callbacks;
    std::vector<scroll_callback> _scroll_callbacks;
    std::vector<key_callback>    _key_callbacks;
    std::vector<resize_callback> _resize_callbacks;

    void resizeCallback(GLFWwindow* window, int new_width, int new_height);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
public:
    Window();
    void tick(float deltaTime);
    void close();
    GLFWwindow* window() { return _window; }
    typedef void* (*loadProc)(const char*);
    loadProc getLoadProc() { return (loadProc)glfwGetProcAddress; }
    bool shouldClose() { return glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(_window); }
    //GLADloadproc gladProcAddress() { return (GLADloadproc)glfwGetProcAddress; }

    void onMouseButton(mouse_button_callback lambda);
    void onResize(resize_callback lambda);
    void onScroll(scroll_callback lambda);
    void onKey(key_callback lambda);

    int width() const { return _width; };
    int height() const { return _height; };
};

#endif