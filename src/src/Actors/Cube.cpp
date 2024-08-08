#include <Actors/Cube.h>
#include <Actors/Plane.h>
#include <iostream>
#include <Utils.h>
#include "stb_image.h"

void Cube::drawFace(glm::vec3 startPoint, glm::vec3 v1, glm::vec3 v2, glm::vec2 resolution)
{
	int startVertexIndex = _vertices.size() / 3;
	v1 = glm::normalize(v1);
    v2 = glm::normalize(v2);
    glm::vec3 normal = glm::cross(v1,v2);
    v1 /=  resolution[0];
    v2 /= resolution[1];

    for(int i=0; i<=resolution[1]; i++){
        for(int j=0; j<=resolution[0]; j++){
            _vertices.push_back(startPoint[0] + j*v1[0] + i*v2[0]);
            _vertices.push_back(startPoint[1] + j*v1[1] + i*v2[1]);
            _vertices.push_back(startPoint[2] + j*v1[2] + i*v2[2]);
            
            _normals.push_back(normal[0]);
            _normals.push_back(normal[1]);
            _normals.push_back(normal[2]);

            _texCoords.push_back((float)j/resolution[0]);
            _texCoords.push_back(1.f-(float)i/resolution[1]);
            
            if(j<resolution[0] && i<resolution[1]){
                _indices.push_back(startVertexIndex + i*(resolution[0]+1) + j);
                _indices.push_back(startVertexIndex + (i+1)*(resolution[0]+1) + j);
                _indices.push_back(startVertexIndex + i*(resolution[0]+1) + (j+1));
                
                _indices.push_back(startVertexIndex + i*(resolution[0]+1) + (j+1));
                _indices.push_back(startVertexIndex + (i+1)*(resolution[0]+1) + j);
                _indices.push_back(startVertexIndex + (i+1)*(resolution[0]+1) + (j+1));
            }

        }
    }
}

Cube::Cube(Shader* shader, uint16_t resolution) : Actor(shader, "Cube") {

	// front
	drawFace(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,1.f,0.f), glm::vec2(resolution, resolution));
	// rigth
	drawFace(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec3(0.f,0.f,-1.f), glm::vec3(0.f,1.f,0.f), glm::vec2(resolution, resolution));
	// top
	drawFace(glm::vec3(-0.5f, +0.5f, +0.5f), glm::vec3(1.f,0.f,0.f), glm::vec3(0.f,0.f,-1.f), glm::vec2(resolution, resolution));
	//left
	drawFace(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f,0.f,+1.f), glm::vec3(0.f,1.f,0.f), glm::vec2(resolution, resolution));
	// back
	drawFace(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec3(-1.f,0.f,0.f), glm::vec3(0.f,1.f,0.f), glm::vec2(resolution, resolution));
	// bottom
	drawFace(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(+1.f,0.f,0.f), glm::vec2(resolution, resolution));

    _colors.assign(_vertices.size(), 1.f);
    _renderMode = Shader::RenderMode::Normal;
    _collider = new BoxCollider(vec3(0.0f), _transform->scale);

    bindResources();
}

std::vector<glm::vec3> Cube::corners()
{
    auto orientation = rotationMatrix();
    std::vector<vec3> vertices;
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            for (int k = -1; k <= 1; k += 2) {
                vec3 corner = _transform->position + glm::vec3(
                    i * _transform->scale.x / 2,
                    j * _transform->scale.y / 2,
                    k * _transform->scale.z / 2);

                glm::vec4 p = modelMatrix() * glm::vec4(corner, 1.0f);
                vec3 norm_p = vec3(p.x, p.y, p.z) / p.w;
                vertices.push_back(norm_p);
            }
        }
    }


    return vertices;
}

