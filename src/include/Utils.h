#ifndef __UTILS__H
#define __UTILS__H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>

using vec3 = glm::vec3;
class Cube;

class Utils {
public:
    template <typename T>
    static void printVec(const std::string& name, const T& vec);

    template <typename T>
    static void printMat(const std::string& name, const T& mat);

    // Funciones existentes en la clase Utils
    template <typename T, typename U>
    static void copyVecToArray(const std::vector<T>& vecArray, std::vector<U>& array);

    static vec3 closestPointToCube(vec3 p, const Cube& c);
};

#endif
