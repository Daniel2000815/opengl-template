#include "Debug.h"

// Definición de los miembros estáticos
std::vector<Line*> Debug::_pool;
std::vector<Line*> Debug::_activeLines;

void Debug::drawLine(Shader* shader, const glm::vec3& start, const glm::vec3& end, const glm::vec3& color, float width) {
    Line* line = getLineFromPool(shader);
    line->setPoint(0, start, color);
    line->setPoint(1, end, color);
    line->setWidth(width);

    _activeLines.push_back(line);
}

void Debug::tick(float deltaTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    renderDebugLines(deltaTime);
}

void Debug::renderDebugLines(float deltaTime) {
    for (auto line : _activeLines) {
        line->tick();
    }

    _activeLines.clear();
}

void Debug::clear() {
    for (auto line : _activeLines) {
        _pool.push_back(line);
    }
    _activeLines.clear();
}

Line* Debug::getLineFromPool(Shader* shader) {
    if (!_pool.empty()) {
        Line* line = _pool.back();
        _pool.pop_back();
        return line;
    }
    return new Line(shader, glm::vec3(0.0f), glm::vec3(0.0f)); // Crear una nueva línea si el pool está vacío
}
