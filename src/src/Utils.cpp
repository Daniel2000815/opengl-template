#include "Utils.h"
#include <algorithm>
#include <iterator>
#include <Actors/Cube.h>

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
    result.reserve(array1.size() + array2.size());  // Reservar espacio para evitar m�ltiples asignaciones de memoria
    result.insert(result.end(), array1.begin(), array1.end());
    result.insert(result.end(), array2.begin(), array2.end());
    return result;
}

template <>
void Utils::printVec(const std::string& name, const glm::vec3& vec) {
    printf("%s(%f, %f, %f)\n", name.c_str(), vec.x, vec.y, vec.z);
}

// Especializaci�n para glm::vec4
template <>
void Utils::printVec(const std::string& name, const glm::vec4& vec) {
    printf("%s(%f, %f, %f, %f)\n", name.c_str(), vec.x, vec.y, vec.z, vec.w);
}

// Especializaci�n para glm::mat3
template <>
void Utils::printMat(const std::string& name, const glm::mat3& mat) {
    printf("%s(\n", name.c_str());
    for (int i = 0; i < 3; ++i) {
        printf("  %f %f %f\n", mat[i][0], mat[i][1], mat[i][2]);
    }
    printf(")\n");
}

// Especializaci�n para glm::mat4
template <>
void Utils::printMat(const std::string& name, const glm::mat4& mat) {
    printf("%s(\n", name.c_str());
    for (int i = 0; i < 4; ++i) {
        printf("  %f %f %f %f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
    }
    printf(")\n");
}

template void Utils::copyVecToArray<glm::vec2, GLfloat>(const std::vector<glm::vec2>&, std::vector<GLfloat>&);
template void Utils::copyVecToArray<glm::vec3, GLfloat>(const std::vector<glm::vec3>&, std::vector<GLfloat>&);
template void Utils::copyVecToArray<glm::vec2, GLuint>(const std::vector<glm::vec2>&, std::vector<GLuint>&);
template void Utils::copyVecToArray<glm::vec3, GLuint>(const std::vector<glm::vec3>&, std::vector<GLuint>&);
//template std::vector<GLfloat> combineArrays(const std::vector<GLfloat>& array1, const std::vector<GLfloat>& array2);

vec3 Utils::closestPointToCube(vec3 p, const Cube& c)
{
    vec3 d = p - c.transform()->position;
    vec3 result = c.transform()->position;  // local coordinates to box
    glm::mat3 rot = c.rotationMatrix();

    for(int i=0; i<3; i++) {
        // ...project d onto that axis to get the distance
        // along the axis of d from the box center
        vec3 axis = glm::normalize(rot[i]);
        float dist = glm::dot(d, axis);
        float currDist = c.transform()->scale[i] * 0.5f;

        if (dist > currDist)     dist  = currDist;
        else if (dist < -currDist) dist = -currDist;
        
        // Step that distance along the axis to get world coordinate
        result += dist * axis;
    }

    return result;
}
