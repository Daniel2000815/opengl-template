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

template <typename T>
T* Debug::getObjectFromPool(Shader* shader) {
    auto& pool = _pool[typeid(T)];
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        T* obj = dynamic_cast<T*>(*it);
        if (obj) {
            pool.erase(it);
            return obj;
        }
    }

    return new T(shader);
}

template <>
Line* Debug::getObjectFromPool<Line>(Shader* shader) {
    auto& pool = _pool[typeid(Line)];
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        Line* obj = dynamic_cast<Line*>(*it);
        if (obj) {
            pool.erase(it);
            return obj;
        }
    }

    printf("creating Line\n");
    return new Line(shader, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), 50.0f);
}

template <>
Sphere* Debug::getObjectFromPool<Sphere>(Shader* shader) {
    auto& pool = _pool[typeid(Sphere)];
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        Sphere* obj = dynamic_cast<Sphere*>(*it);
        if (obj) {
            pool.erase(it);
            return obj;
        }
    }

    return new Sphere(shader, 2, 1.0f);
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