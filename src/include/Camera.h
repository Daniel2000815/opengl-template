#ifndef __CAMERA__H_
#define __CAMERA__H_

#include <Window.h>
#include <UI.h>
#include <glm/gtc/matrix_transform.hpp>

using vec3 = glm::vec3;

class Camera
{
public:
    Camera(Window* window, UI* ui);

    const vec3& position() const { return _position; }
    void setPosition(const vec3& position);
    void moveForward(float deltaDistance);
    void moveRight(float deltaDistance);
    void moveUp(float deltaDistance);

    float fov() const { return _fov; }
    void setFov(float fov);

    const vec3& forward() const { return _forward; }
    const vec3& right() const { return _right; }
    const vec3& up() const { return _up; }

    const glm::mat4 viewMatrix() const;

    void rotate(double horizontalDelta, double verticalDelta);

    void tick(float delta);

private:
    Window* _window;
    UI* _ui;

    vec3 _position;
    vec3 _forward, _right, _up;
    
    float _horizontalAngle = 0.0f, _verticalAngle = 0.0f;
    float _fov = 45.0f;
    
    float _moveSpeed = 2.0f;
    float _rotateSpeed = 0.1f;
    
    bool _mouseMoving;
};

#endif