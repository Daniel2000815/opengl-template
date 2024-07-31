#ifndef __UTILS__H
#define __UTILS__H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Utils {
public:
    template <typename T, typename U>
    static void copyVecToArray(const std::vector<T>& vecArray, std::vector<U>& array);
    
    template <typename T>
    static std::vector<T> combineArrays(const std::vector<T>& v1, std::vector<T>& v2);

};

#endif
