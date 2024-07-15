#include <Actors/Plane.h>
#include <iostream>

Plane::Plane(Shader *shader, glm::vec3 startPoint, glm::vec3 right, glm::vec3 up, glm::vec2 resolution) : Actor(shader, "Plane")
{
    // Make sure that plane is 1x1
    right = glm::normalize(right);
    up = glm::normalize(up);
    glm::vec3 normal = glm::cross(right,up);
    right /=  resolution[0];
    up /= resolution[1];
    
    for(int i=0; i<=resolution[1]; i++){
        for(int j=0; j<=resolution[0]; j++){
            _vertices.push_back(startPoint[0] + j*right[0] + i*up[0]);
            _vertices.push_back(startPoint[1] + j*right[1] + i*up[1]);
            _vertices.push_back(startPoint[2] + j*right[2] + i*up[2]);

            _normals.push_back(normal[0]);
            _normals.push_back(normal[1]);
            _normals.push_back(normal[2]);
            
            _texCoords.push_back((float)j/resolution[0]);
            _texCoords.push_back(1.f-(float)i/resolution[1]);
            
            if(j<resolution[0] && i<resolution[1]){
                _indices.push_back(i*(resolution[0]+1) + j);
                _indices.push_back((i+1)*(resolution[0]+1) + j);
                _indices.push_back(i*(resolution[0]+1) + (j+1));
                
                _indices.push_back(i*(resolution[0]+1) + (j+1));
                _indices.push_back((i+1)*(resolution[0]+1) + j);
                _indices.push_back((i+1)*(resolution[0]+1) + (j+1));
            }

        }
    }

    _colors.assign(_vertices.size(), 1.f);
    _renderMode = Shader::RenderMode::Normal;

    bindResources();
}
