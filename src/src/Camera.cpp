#include "Camera.h"

Camera::Camera(Window* window, UI* ui) :
    _position(3.0f, 3.0f, 5.0f),
    _horizontalAngle(3.6f),
    _verticalAngle(-0.5f),
    _fov(45.0f),
    _mouseMoving(false)
{
    _window = window;
    _ui = ui;

    _window->onScroll([this](GLFWwindow* window, double x, double y) {
        setFov(fov() + y);
    });

    _window->onMouseButton([this](GLFWwindow* window, int button, int action, int mods) {
        _mouseMoving = !_ui->mouseHovering() && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
    });

    rotate(0.0f, 0.0f);
}


const glm::mat4 Camera::viewMatrix() const
{
    return glm::lookAt(_position, _position+_forward, _up);
}

void Camera::rotate(double horizontalDelta, double verticalDelta)
{
    _horizontalAngle += _rotateSpeed * horizontalDelta;
    _verticalAngle += _rotateSpeed * verticalDelta;
    
    _forward = glm::vec3(
        cos(_verticalAngle) * sin(_horizontalAngle),
        sin(_verticalAngle),
        cos(_verticalAngle) * cos(_horizontalAngle)
    );
    
    _right = glm::vec3(
        sin(_horizontalAngle - 3.14f / 2.0f),
        0,
        cos(_horizontalAngle - 3.14f / 2.0f)
    );
    
    _up = glm::cross(_right, _forward);
}

void Camera::tick(float delta)
{
    double xpos, ypos;
    static double xLastpos, yLastpos;
    glfwGetCursorPos(_window->window(), &xpos, &ypos);
    glm::vec2 deltaPos = glm::vec2(xpos - xLastpos, ypos - yLastpos);

    if (_mouseMoving) {
        rotate(delta * deltaPos.x, delta * deltaPos.y);
    }

    xLastpos = xpos; yLastpos = ypos;

    if ((glfwGetKey(_window->window(), GLFW_KEY_UP) | glfwGetKey(_window->window(), GLFW_KEY_W)) == GLFW_PRESS)
        moveForward(delta);

    if ((glfwGetKey(_window->window(), GLFW_KEY_DOWN) | glfwGetKey(_window->window(), GLFW_KEY_S)) == GLFW_PRESS)
        moveForward(-delta);

    if ((glfwGetKey(_window->window(), GLFW_KEY_RIGHT) | glfwGetKey(_window->window(), GLFW_KEY_D)) == GLFW_PRESS)
        moveRight(delta);

    if ((glfwGetKey(_window->window(), GLFW_KEY_LEFT) | glfwGetKey(_window->window(), GLFW_KEY_A)) == GLFW_PRESS)
        moveRight(-delta);
}


void Camera::setPosition(const vec3& position)
{
    _position = position;
}

void Camera::moveForward(float deltaDistance)
{
    _position += _forward * _moveSpeed * deltaDistance;
}

void Camera::moveRight(float deltaDistance)
{
    _position += _right * _moveSpeed * deltaDistance;
}

void Camera::moveUp(float deltaDistance)
{
    _position += _up * _moveSpeed * deltaDistance;
}

void Camera::setFov(float fov)
{
    _fov = fov;
}
