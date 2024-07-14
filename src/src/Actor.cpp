#include "Actor.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/gtx/matrix_decompose.hpp>

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
        loadImage("../resources/textures/smiley.png");
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

    printf("Error? %s\n", glGetError());
}

Actor::Actor(Shader *shader)
{
    
    _position = glm::vec3(0.f);
    _rotation = glm::vec3(0.f);
    _scale = glm::vec3(1.f);

    _modelMatrix = glm::mat4(1.f);

    this->_shader = shader;
    this->_vertices = std::vector<GLfloat>{};
    this->_indices = std::vector<GLuint>{};
    this->_colors = std::vector<GLfloat>{};
    this->_texCoords = std::vector<GLfloat>{};
    this->_normals = std::vector<GLfloat>{};
    this->_renderMode = Shader::RenderMode::Color;
    this->_name = "New actor";

    //BindResources();
}

Actor::Actor(Shader* shader, std::string name) : Actor(shader)
{
    _name = name;
}

void Actor::tick(float deltaTime)
{
    _shader->setModelMatrix(_modelMatrix);
    _shader->setRenderMode(_renderMode);
    _shader->tick(deltaTime);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}


void Actor::setPosition(glm::vec3 position){
    this->_position = position;
    _modelMatrix[3][0] = position[0];
    _modelMatrix[3][1] = position[1];
    _modelMatrix[3][2] = position[2];
}

void Actor::setRotation(vec3 eulerAngles)
{
    rotate(eulerAngles.x, vec3(1.0f, 0.0, 0.0f));
    rotate(eulerAngles.y, vec3(0.0f, 1.0, 0.0f));
    rotate(eulerAngles.z, vec3(0.0f, 0.0, 1.0f));
}

void Actor::setScale(vec3 newScale)
{
    vec3 factor = 1.0f / this->_scale * newScale;
    scale(1.0f / this->_scale * newScale);
}

void Actor::setColor(vec3 color)
{
    for (uint16_t i = 0; i < _colors.size()/3; i++) {
        _colors[3 * i] = color.x;
        _colors[3 * i + 1] = color.y;
        _colors[3 * i + 2] = color.z;
    }
}

const Actor* Actor::translate(glm::vec3 translation){
    this->_position += translation;
    _modelMatrix = glm::translate(_modelMatrix, translation);

    return this;
}

const Actor* Actor::scale(glm::vec3 scale){
    this->_scale *= scale;
    _modelMatrix = glm::scale(_modelMatrix, scale);

    return this;
}


const Actor* Actor::rotate(float angle_degrees, glm::vec3 axis){
    _modelMatrix = glm::rotate(_modelMatrix, glm::radians(angle_degrees), axis);

    glm::vec3 _scale;
    glm::quat _rotation;
    glm::vec3 _translation;
    glm::vec3 _skew;
    glm::vec4 _perspective;
    glm::decompose(_modelMatrix, _scale, _rotation, _translation, _skew, _perspective);

    _rotation = glm::eulerAngles(glm::conjugate(_rotation)) * 180.f / 3.14159f;

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
        _position.x, _position.y, _position.z, 
        _rotation.x, _rotation.y, _rotation.z,
        _scale.x,    _scale.y,    _scale.z
    );
}

void Actor::setRenderMode(Shader::RenderMode renderMode){
    this->_renderMode = renderMode;
}

void Actor::setName(std::string name)
{
    this->_name = name;
}
