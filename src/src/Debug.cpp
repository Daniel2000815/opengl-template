#include "Debug.h"
#include <cstdio>

// Definición de los miembros estáticos
std::vector<Line*> Debug::_lines;
std::vector<Sphere*> Debug::_spheres;
size_t Debug::_activeLineCount = 0;
size_t Debug::_activeSphereCount = 0;

void Debug::drawLine(Shader* shader, const glm::vec3& start, const glm::vec3& end, const glm::vec3& color, float width) {
    Line* line = getNextLine(shader);

    line->setVertex(0, start);
    line->setVertex(1, end);
    line->setColor(color);
    line->setWidth(width);
}

void Debug::drawSphere(Shader* shader, const glm::vec3& position, float radius, const glm::vec3& color) {
    Sphere* sphere = getNextSphere(shader);
    sphere->setPosition(position);
    sphere->setScale(glm::vec3(radius));
    sphere->setColor(color);
}

Line* Debug::getNextLine(Shader* shader) {
    if (_activeLineCount < _lines.size()) {
        return _lines[_activeLineCount++];
    }

    Line* newLine = new Line(shader, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 50.0f);
    _lines.push_back(newLine);
    _activeLineCount++;

    return newLine;
}

Sphere* Debug::getNextSphere(Shader* shader) {
    if (_activeSphereCount < _spheres.size()) {
        return _spheres[_activeSphereCount++];
    }

    Sphere* newSphere = new Sphere(shader, 2, 1.0f);
    _spheres.push_back(newSphere);

    _activeSphereCount++;

    return newSphere;
}

void Debug::tick(float deltaTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    renderDebugObjects(deltaTime);
}

void Debug::renderDebugObjects(float deltaTime) {
    for (size_t i = 0; i < _activeLineCount; ++i) {
        _lines[i]->tick(deltaTime);
    }

    for (size_t i = 0; i < _activeSphereCount; ++i) {
        _spheres[i]->tick(deltaTime);
    }

    clear();
}

void Debug::clear() {
    _activeLineCount = 0;
    _activeSphereCount = 0;
}
