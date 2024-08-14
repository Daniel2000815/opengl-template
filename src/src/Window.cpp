#include "Window.h"

void Window::resizeCallback(GLFWwindow* window, int new_width, int new_height)
{
    _width = new_width;
    _height = new_height;
    glViewport(0, 0, _width, _height);
    
    for (auto& f : _resize_callbacks)
        f(window, new_width, new_height);
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    for (auto& f : _scroll_callbacks)
        f(window, xoffset, yoffset);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    for (auto& f : _key_callbacks)
        f(window, key, scancode, action, mods);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    for (auto& f : _mouse_button_callbacks)
        f(window, button, action, mods);
}

Window::Window()
{
    _mouse_button_callbacks = std::vector<mouse_button_callback>{};
    _key_callbacks          = std::vector<key_callback>{};
    _scroll_callbacks       = std::vector<scroll_callback>{};
    _resize_callbacks       = std::vector<resize_callback>{};

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _window = glfwCreateWindow(_width, _height, "New Window", NULL, NULL);
    if (_window == NULL)
    {
        std::cout << "Failed to open GLFW window" << std::endl;
        close();
    }
    glfwMakeContextCurrent(_window);

    glfwSetWindowUserPointer(_window, this);
    auto resize_callback = [](GLFWwindow* window, int w, int h)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->resizeCallback(window, w, h);
    };

    auto mouse_button_callback = [](GLFWwindow* w, int b, int a, int m)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(w))->mouseButtonCallback(w, b, a, m);
    };

    auto scroll_callback = [](GLFWwindow* w, double x, double y)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(w))->scrollCallback(w, x, y);
    };

    auto key_callback = [](GLFWwindow* w, int k, int s, int a, int m)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(w))->keyCallback(w, k, s, a, m);
    };

    glfwSetScrollCallback(_window, scroll_callback);
    glfwSetKeyCallback(_window, key_callback);
    glfwSetFramebufferSizeCallback(_window, resize_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);

    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }
    gladLoadGL();

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    // For transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);   //  Accept fragment if it closer to the camera than the former one

}

void Window::swap()
{
    // Clean up
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::close()
{
    glfwTerminate();
}

void Window::onMouseButton(mouse_button_callback lambda)
{
    _mouse_button_callbacks.push_back(lambda);
}

void Window::onResize(resize_callback lambda)
{
    _resize_callbacks.push_back(lambda);
}

void Window::onScroll(scroll_callback lambda)
{
    _scroll_callbacks.push_back(lambda);
}

void Window::onKey(key_callback lambda)
{
    _key_callbacks.push_back(lambda);
}
