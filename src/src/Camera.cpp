#include "Camera.h"


void Camera::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        _mouseMoving = true;

        //TryPickObject(glm::vec2(xpos, ypos));
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        _mouseMoving = false;
}

Camera::Camera(Window* window) :
    _position(0.0f, 0.0f, 5.0f),
    _horizontalAngle(3.14f),
    _verticalAngle(0.0f),
    _fov(45.0f),
    _mouseMoving(false)
    //_nearPlane(0.01f),
    //_farPlane(100.0f),
    //_viewportAspectRatio(4.0f / 3.0f)
{
    rotate(0.0f, 0.0f);
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

void Camera::tick(float delta, GLFWwindow& window)
{
    double xpos, ypos;
    static double xLastpos, yLastpos;
    glfwGetCursorPos(&window, &xpos, &ypos);
    glm::vec2 deltaPos = glm::vec2(xpos - xLastpos, ypos - yLastpos);

    if (_mouseMoving) {
        rotate(delta * deltaPos.x, delta * deltaPos.y);
    }

    xLastpos = xpos; yLastpos = ypos;

    if (glfwGetKey(&window, GLFW_KEY_UP) == GLFW_PRESS)
        moveForward(delta);

    if (glfwGetKey(&window, GLFW_KEY_DOWN) == GLFW_PRESS)
        moveForward(-delta);

    if (glfwGetKey(&window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        moveRight(delta);

    if (glfwGetKey(&window, GLFW_KEY_LEFT) == GLFW_PRESS)
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
