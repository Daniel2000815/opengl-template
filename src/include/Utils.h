#ifndef __UTILS__H
#define __UTILS__H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>

class Utils {
public:
    // Funciones para imprimir vectores y matrices
    template <typename T>
    static void printVec(const std::string& name, const T& vec);

    template <typename T>
    static void printMat(const std::string& name, const T& mat);

    // Funciones existentes en la clase Utils
    template <typename T, typename U>
    static void copyVecToArray(const std::vector<T>& vecArray, std::vector<U>& array);

    template <typename T>
    static std::vector<T> combineArrays(const std::vector<T>& v1, std::vector<T>& v2);
};

#endif
