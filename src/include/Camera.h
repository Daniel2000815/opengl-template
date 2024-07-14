#ifndef __CAMERA__H_
#define __CAMERA__H_

#include <Window.h>
#include <glm/gtc/matrix_transform.hpp>

using vec3 = glm::vec3;

class Camera
{
public:
    Camera(Window* window);

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

    void tick(float delta, GLFWwindow& window);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:
    vec3 _position;
    vec3 _forward, _right, _up;
    float _horizontalAngle = 0.0f, _verticalAngle = 0.0f;
    float _fov = 45.0f;
    
    float _moveSpeed = 0.003f;
    float _rotateSpeed = 0.001f;
    double lastTime = 0.0;
    

    //vec3 _position;
    //float _horizontalAngle;
    //float _verticalAngle;
    //float _fov;
    //float _nearPlane;
    //float _farPlane;
    //float _viewportAspectRatio;

    //void normalizeAngles();

    bool _mouseMoving;
    
};

#endif