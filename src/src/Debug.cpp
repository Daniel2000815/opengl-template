#include "Debug.h"
#include <Actors/Line.h>
#include <Actors/Sphere.h>

// Definición de los miembros estáticos
std::unordered_map<std::type_index, std::vector<Actor*>> Debug::_pool;
std::unordered_map<std::type_index, std::vector<Actor*>> Debug::_active;

void Debug::drawLine(Shader* shader, const glm::vec3& start, const glm::vec3& end, const glm::vec3& color, float width) {
    Line* line = getObjectFromPool<Line>(shader);

    if (line->nPoints() == 0) {
        line->addPoint(start);
        line->addPoint(end);
    }
    else {
        line->setPoint(0, start, color);
        line->setPoint(1, end, color);
    }
    
    line->setWidth(width);

    _active[typeid(Line)].push_back(line);
}

void Debug::drawSphere(Shader* shader, const glm::vec3& position, float radius, const glm::vec3& color)
{
    Sphere* s = getObjectFromPool<Sphere>(shader);
    s->setPosition(position);
    s->setScale(glm::vec3(radius));
    s->setColor(color);

    _active[typeid(Sphere)].push_back(s);
}

void Debug::tick(float deltaTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    renderDebugObjects(deltaTime);
}

void Debug::renderDebugObjects(float deltaTime) {
    for (auto& [type, objects] : _active) {
        for (Actor* obj : objects) {
            obj->tick(deltaTime);
        }
    }

    clear();
}

void Debug::clear() {
    for (auto& [type, objects] : _active) {
        for (Actor* obj : objects) {
            _pool[type].push_back(obj);
        }
        objects.clear();
    }
}