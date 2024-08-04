#ifndef __DEBUG__H_
#define __DEBUG__H_

#include <vector>
#include <glm/glm.hpp>
#include <Actors/Line.h>

class Debug {
public:
    static void drawLine(Shader* shader, const glm::vec3& start, const glm::vec3& end, const glm::vec3& color = glm::vec3(1.0f), float width = 1.f);
    static void tick(float deltaTime);
    static void clear();

private:
    static std::vector<Line*> _pool;        // Pool de líneas reutilizables
    static std::vector<Line*> _activeLines; // Líneas actualmente en uso

    static Line* getLineFromPool(Shader* shader);
    static void renderDebugLines(float deltaTime);
};

#endif
