#include "Shader.h"
#include "loadShaders.hpp"
#include <iostream>

Shader::Shader(Camera* camera, Window* window){
    _programID = loadShaders( "./Shaders/vertex_basic.glsl", "./Shaders/frag_basic.glsl" );
    _deltaTimeID = glGetUniformLocation(_programID, "deltaTime");
    _modelMatrixID = glGetUniformLocation(_programID, "modelMatrix");
    _viewMatrixID = glGetUniformLocation(_programID, "viewMatrix");
    _projectionMatrixID = glGetUniformLocation(_programID, "projectionMatrix");
    _renderModeID = glGetUniformLocation(_programID, "renderMode");

    glUseProgram(_programID);

    _modelMatrix = glm::mat4(1.0f);
    _viewMatrix = glm::lookAt(glm::vec3(1.f), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
    _projectionMatrix = glm::perspective(glm::radians(camera->fov()), (float)window->width() / (float)window->height(), 0.1f, 100.0f);
    _renderMode = Color;

    _window = window;
    _camera = camera;

    /*_window->onResize([this](GLFWwindow* window, int w, int h) {
        updateWindowDimensions(_camera->fov(), w, h);
    });*/
}

Shader::~Shader()
{
    glDeleteProgram(_programID);
}

void Shader::tick(float deltaTime){
    glUniformMatrix4fv(_modelMatrixID, 1, GL_FALSE, &_modelMatrix[0][0]);
    glUniformMatrix4fv(_viewMatrixID, 1, GL_FALSE, &_viewMatrix[0][0]);
    glUniformMatrix4fv(_projectionMatrixID, 1, GL_FALSE, &_projectionMatrix[0][0]);
    glUniform1i(_renderModeID, _renderMode);
    glUniform1f(_deltaTimeID, deltaTime);
}

void Shader::updateViewMatrix(glm::vec3 cameraPos, glm::vec3 lookAt, glm::vec3 up){
    _viewMatrix = glm::lookAt(cameraPos, lookAt, up);
}

void Shader::setModelMatrix(glm::mat4 &newMatrix){
    _modelMatrix = newMatrix;
}

void Shader::setRenderMode(RenderMode renderMode){
    this->_renderMode = renderMode;
}

void Shader::updateWindowDimensions(float fov, int width, int height)
{
    _projectionMatrix = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
}

