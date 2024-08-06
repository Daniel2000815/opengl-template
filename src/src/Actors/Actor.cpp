#include "Actors/Actor.h"
#include "Shader/Shader.h"
#include "stb_image.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <Actors/Line.h>
#include <Utils.h>
#include <Debug.h>

void Actor::loadImage(const char* filename) {
    // Load texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 3);
    
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
}

void Actor::bindResources()
{
    // Create VAO
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    if (_colors.size() > 0) {
        glGenBuffers(1, &_colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(GLfloat), _colors.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(1);
    }

    if (_indices.size() > 0) {
        glGenBuffers(1, &_indiceBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);
    }

    if (_texCoords.size() > 0) {
        //printf("%s\n", _texturePath);
        if(_texturePath.length() == 0)
            loadImage("../resources/textures/smiley.png");
        else
            loadImage(_texturePath.c_str());

        GLuint uvBuffer;
        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, _texCoords.size() * sizeof(GLfloat), _texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(2);
    }

    if (_normals.size() > 0) {
        glGenBuffers(1, &_normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat), _normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(3);
    }

    //printf("Error? %s\n", glGetError());
}

Actor::Actor(Shader *shader)
{
    _transform = new Transform();
    _modelMatrix = glm::mat4(1.f);

    this->_shader = shader;
    this->_vertices = std::vector<GLfloat>{};
    this->_indices = std::vector<GLuint>{};
    this->_colors = std::vector<GLfloat>{};
    this->_texCoords = std::vector<GLfloat>{};
    this->_normals = std::vector<GLfloat>{};
    this->_renderMode = Shader::RenderMode::Color;
    this->_name = "New actor";
    this->_texturePath = "";
    //BindResources();
}

Actor::Actor(Shader* shader, std::string name) : Actor(shader)
{
    _name = name;
}

Actor::Actor(Shader* shader, std::string name, std::string texturePath) : Actor(shader, name)
{
    _texturePath = texturePath;
}

void Actor::tick(float deltaTime)
{
    _shader->setModelMatrix(_modelMatrix);
    _shader->setRenderMode(_renderMode);
    _shader->tick(deltaTime);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);

    if(_renderMode == Shader::RenderMode::Normal && (dynamic_cast<Line*>(this) == nullptr)){
        for (int i = 0; i < _vertices.size() / 3; i++)
            Debug::drawLine(_shader, vertexWorld(i), vertexWorld(i) + 0.1f * normalWorld(i));
    }
}


vec3 Actor::vertexWorld(int vertexIdx) const
{
    glm::vec4 p = modelMatrix() * glm::vec4(vertex(vertexIdx), 1.0f);
    return vec3(p.x, p.y, p.z) / p.w;    
}

vec3 Actor::normalWorld(int vertexIdx) const
{
    return rotationMatrix() * normal(vertexIdx);
}

void Actor::setPosition(glm::vec3 position){
    _transform->position = position;
    _modelMatrix[3][0] = position[0];
    _modelMatrix[3][1] = position[1];
    _modelMatrix[3][2] = position[2];
}

void Actor::setRotation(vec3 angle_radians)
{
    glm::mat4 m = glm::mat4(1.0f);
    m = glm::rotate(m, angle_radians.x, glm::vec3(1.0f, 0.0f, 0.0f));
    m = glm::rotate(m, angle_radians.y, glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::rotate(m, angle_radians.z, glm::vec3(0.0f, 0.0f, 1.0f));
    
    
    _modelMatrix = m;
    setPosition(_transform->position);
    setScale(_transform->scale);
    _transform->rotation = angle_radians;
}

void Actor::setScale(vec3 newScale)
{
    if (glm::epsilonEqual(newScale.x, 0.0f, 0.001f) || glm::epsilonEqual(newScale.y, 0.0f, 0.001f) || glm::epsilonEqual(newScale.z, 0.0f, 0.001f)) {
        return;
    }

    vec3 factor = 1.0f / _transform->scale * newScale;
    scale(1.0f / _transform->scale * newScale);
}

void Actor::setColor(vec3 color)
{
    for (uint16_t i = 0; i < _colors.size()/3; i++) {
        _colors[3 * i] = color.x;
        _colors[3 * i + 1] = color.y;
        _colors[3 * i + 2] = color.z;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(GLfloat), _colors.data(), GL_STATIC_DRAW);
}

void Actor::setVertex(size_t idx, vec3 localPosition)
{
    assert(idx >= 0 && idx < _vertices.size() / 3);

    _vertices[3 * idx] = localPosition.x;
    _vertices[3 * idx + 1] = localPosition.y;
    _vertices[3 * idx + 2] = localPosition.z;

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), _vertices.data(), GL_STATIC_DRAW);
}

const Actor* Actor::translate(glm::vec3 translation){
    _transform->position += translation;
    _modelMatrix = glm::translate(_modelMatrix, translation);

    return this;
}

const Actor* Actor::scale(glm::vec3 scale){
    _transform->scale *= scale;
    _modelMatrix = glm::scale(_modelMatrix, scale);

    return this;
}

glm::mat3 Actor::rotationMatrix() const {
    glm::mat3 rotationMatrix;

    // Copy the upper-left 3x3 part of the 4x4 matrix
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            rotationMatrix[i][j] = _modelMatrix[i][j];
        }
    }

    // Normalize the rows to remove scaling
    for (int i = 0; i < 3; ++i) {
        float length = std::sqrt(rotationMatrix[i][0] * rotationMatrix[i][0] +
            rotationMatrix[i][1] * rotationMatrix[i][1] +
            rotationMatrix[i][2] * rotationMatrix[i][2]);
        for (int j = 0; j < 3; ++j) {
            rotationMatrix[i][j] /= length;
        }
    }

    return rotationMatrix;
}

std::vector<vec3> Actor::verticesWorld() const {
    auto orientation = rotationMatrix();
    std::vector<vec3> vertices;

    for (int i = 0; i < _vertices.size() / 3; i++) {
        glm::vec4 p = modelMatrix() * glm::vec4(vec3(
            _vertices[3*i], _vertices[3 * i + 1], _vertices[3 * i + 2]
        ), 1.0f);
        vec3 norm_p = vec3(p.x, p.y, p.z) / p.w;
        vertices.push_back(norm_p);
    }

    return vertices;
}

std::vector<vec3> Actor::normalsWorld() const
{
    auto orientation = rotationMatrix();
    std::vector<vec3> normals;

    for (int i = 0; i < _normals.size() / 3; i++) {
        glm::vec4 p = modelMatrix() * glm::vec4(vec3(
            _normals[3 * i], _normals[3 * i + 1], _normals[3 * i + 2]
        ), 1.0f);
        vec3 norm_p = vec3(p.x, p.y, p.z) / p.w;
        normals.push_back(norm_p);
    }

    return normals;
}

const Actor* Actor::rotate(float angle_radians, glm::vec3 axis){
    _modelMatrix = glm::rotate(_modelMatrix, angle_radians, axis);

    glm::vec3 _scale;
    glm::quat _rotation;
    glm::vec3 _translation;
    glm::vec3 _skew;
    glm::vec4 _perspective;
    glm::decompose(_modelMatrix, _scale, _rotation, _translation, _skew, _perspective);

    _rotation = glm::conjugate(_rotation);
    this->_transform->rotation = glm::eulerAngles(_rotation);
    return this;
}

void Actor::printMesh(){
    
    int k=0;
    printf("%s mesh:\n", _name.c_str());
    printf("\t====== VERTICES =======\n");
    for(auto v : _vertices){
        printf("%f, ", v);
        k+=1;
        if(k==3){
            printf("\n");
            k=0;
        }        
    }
    printf("====================\n");

    printf("\t====== INDICES =======\n");
    k=0;
    for(auto v : _indices){
        printf("%u, ", v);
        k+=1;
        if(k==3){
            printf("\n");
            k=0;
        }        
    }
    printf("====================");
}

void Actor::printTransform() {
    printf("%s transform:\n\tPosition: (%f, %f, %f)\n\tRotation: (%f, %f, %f)\n\tScale: (%f, %f, %f)\n", 
        _name.c_str(), 
        _transform->position.x, _transform->position.y, _transform->position.z,
        _transform->rotation.x, _transform->rotation.y, _transform->rotation.z,
        _transform->scale.x,    _transform->scale.y,    _transform->scale.z
    );
}

void Actor::setRenderMode(Shader::RenderMode renderMode){
    this->_renderMode = renderMode;
}

void Actor::setName(std::string name)
{
    this->_name = name;
}
