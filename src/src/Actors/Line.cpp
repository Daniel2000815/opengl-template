#include <iostream>
#include <Actors/Line.h>

Line::Line(Shader* shader, glm::vec3 color, float width) : Actor(shader, "Line") {
    this->_color = color;
    this->_width = width;
    this->_startPoint = glm::vec3(0.f);
    this->_endPoint = glm::vec3(0.f);
}

Line::Line(Shader* shader, glm::vec3 start, glm::vec3 end, glm::vec3 color, float width) : Line(shader, color, width) {
    _startPoint = start;
    _endPoint = end;

    addPoint(start);
    addPoint(end);
}

// TODO: check if only buffering data agai works
void Line::addPoint(glm::vec3 p) {

    _vertices.push_back(p[0]);
    _vertices.push_back(p[1]);
    _vertices.push_back(p[2]);

    _colors.push_back(_color[0]);
    _colors.push_back(_color[1]);
    _colors.push_back(_color[2]);

    _renderMode = Shader::RenderMode::Color;
    bindResources();
}

void Line::tick(float deltaTime) {
    glEnable(GL_DEPTH_TEST);
    
    _shader->setModelMatrix(_modelMatrix);
    _shader->setRenderMode(_renderMode);
    _shader->tick(deltaTime);

    glBindVertexArray(_vao);
    float oldWidth;
    glGetFloatv(GL_LINE_WIDTH, &oldWidth);
    glLineWidth(_width);
    glDrawArrays(GL_LINE_STRIP, 0, _vertices.size() / 3);
    glLineWidth(oldWidth);
    glBindVertexArray(0);
}
