#ifndef __DEBUG__H_
#define __DEBUG__H_

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <glm/glm.hpp>
#include <Actors/Actor.h>

class Shader;

class Debug {
public:
    static void drawLine(Shader* shader, const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f), float width = 1.f);
    static void drawSphere(Shader* shader, const glm::vec3& position, float radius = 1.f, const glm::vec3& color = glm::vec3(1.0f));

    static void tick(float deltaTime);
    static void clear();

private:
    static std::unordered_map<std::type_index, std::vector<Actor*>> _pool;    // Pool de objetos reutilizables
    static std::unordered_map<std::type_index, std::vector<Actor*>> _active;  // Objetos actualmente en uso

    template <typename T>
    static T* getObjectFromPool(Shader* shader);

    static void renderDebugObjects(float deltaTime);
};

template <typename T>
T* Debug::getObjectFromPool(Shader* shader) {
    auto& pool = _pool[typeid(T)];
    for (auto it = pool.begin(); it != pool.end(); ++it) {
        T* obj = dynamic_cast<T*>(*it);
        if (obj) {
            pool.erase(it);
            printf("not creating\n");
            return obj;
        }
    }

    printf("creating\n");
    return new T(shader);
}

#endif
