#ifndef __SHADER__H_
#define __SHADER__H_
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "loadShaders.hpp"
#include <Window.h>
#include <Camera.h>

class Shader
{
public:
    enum RenderMode {Color, Texture, Normal};

    Shader(Camera* camera, Window* window);
    ~Shader();
    void tick(float deltaTime);

    void updateViewMatrix(glm::vec3 cameraPos, glm::vec3 lookAt = glm::vec3(0,0,0), glm::vec3 up = glm::vec3(0,1,0));
    void setModelMatrix(glm::mat4 &newMatrix);
    void setRenderMode(RenderMode newMode);
    void updateWindowDimensions(float fov, int width, int height);
    inline GLuint id(){return _programID;}

    inline glm::mat4 viewMatrix() { return _viewMatrix; }
    inline glm::mat4 projectionMatrix() { return _projectionMatrix; }
    inline glm::mat4 modelMatrix() { return _modelMatrix; }

    
private:
    
   GLuint _deltaTimeID, _programID, _modelMatrixID, _viewMatrixID, _projectionMatrixID, _renderModeID;
   glm::mat4 _modelMatrix = glm::mat4(1.0f);
   glm::mat4 _viewMatrix;
   
   glm::mat4 _projectionMatrix;
   RenderMode _renderMode = Color;

   Camera* _camera;
   Window* _window;
};

#endif