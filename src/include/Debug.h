#ifndef __DEBUG__H_
#define __DEBUG__H_

#include <vector>
#include <glm/glm.hpp>
#include <Actors/Actor.h>
#include <Actors/Sphere.h>
#include <Actors/Line.h>

class Shader;

class Debug {
public:
    static void drawLine(Shader* shader, const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f), float width = 1.f);
    static void drawSphere(Shader* shader, const glm::vec3& position, float radius = 1.f, const glm::vec3& color = glm::vec3(1.0f));

    static void tick(float deltaTime);
    static void clear();

private:
    static std::vector<Line*> _lines;
    static std::vector<Sphere*> _spheres;
    static size_t _activeLineCount;
    static size_t _activeSphereCount;

    static Line* getNextLine(Shader* shader);
    static Sphere* getNextSphere(Shader* shader);

    static void renderDebugObjects(float deltaTime);
};

#endif // __DEBUG__H_
