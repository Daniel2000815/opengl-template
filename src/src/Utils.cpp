#include "Utils.h"
#include <algorithm>
#include <iterator>

template <typename T, typename U>
void Utils::copyVecToArray(const std::vector<T>& vecArray, std::vector<U>& array) {
    array.clear();
    array.reserve(vecArray.size() * T::length());

    for (const auto& vec : vecArray) {
        std::copy(&vec[0], &vec[0] + T::length(), std::back_inserter(array));
    }
}

template <typename T>
std::vector<T> combineArrays(const std::vector<T>& array1, const std::vector<T>& array2) {
    std::vector<T> result;
    result.reserve(array1.size() + array2.size());  // Reservar espacio para evitar múltiples asignaciones de memoria
    result.insert(result.end(), array1.begin(), array1.end());
    result.insert(result.end(), array2.begin(), array2.end());
    return result;
}

template void Utils::copyVecToArray<glm::vec2, GLfloat>(const std::vector<glm::vec2>&, std::vector<GLfloat>&);
template void Utils::copyVecToArray<glm::vec3, GLfloat>(const std::vector<glm::vec3>&, std::vector<GLfloat>&);
template void Utils::copyVecToArray<glm::vec2, GLuint>(const std::vector<glm::vec2>&, std::vector<GLuint>&);
template void Utils::copyVecToArray<glm::vec3, GLuint>(const std::vector<glm::vec3>&, std::vector<GLuint>&);
//template std::vector<GLfloat> combineArrays(const std::vector<GLfloat>& array1, const std::vector<GLfloat>& array2);