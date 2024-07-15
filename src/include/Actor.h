#ifndef __SCENECOMPONENT__H_
#define __SCENECOMPONENT__H_

#include "Shader.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

using vec3 = glm::vec3;
using mat4 = glm::mat4;
using std::vector;

class Shader;

class Actor
{
    private:
        vec3 _position;
        vec3 _rotation;
        vec3 _scale;
        
    public:
        // Constructors
        Actor(Shader* shader);
        Actor(Shader* shader, std::string name);
        Actor(Shader* shader, const vector<GLfloat> &vertices, const vector<GLuint> &indices);
        Actor(Shader* shader, const vector<GLfloat> &vertices, const vector<GLuint> &indices, vector<GLfloat> colors);
        Actor(Shader* shader, const vector<GLfloat> &vertices, const vector<GLuint> &indices, vector<GLfloat> texCoords, const char *texturePath);

        // Getters
        inline vector<GLfloat>  vertices()       { return _vertices; }
        inline vector<GLuint>   indices()        { return _indices; }
        inline vector<GLfloat>  colors()         { return _colors; }
        inline vector<GLfloat>  uv()             { return _texCoords; }
        inline Shader*          shader()         { return _shader; }

        inline vec3             position()       { return _position; }
        inline vec3             rotation()       { return _rotation; }
        inline vec3             scale()          { return _scale; }
        inline mat4             modelMatrix()    { return _modelMatrix; }
        inline const char*      name()           { return _name.c_str(); }
        
        // Setters
        void setPosition(vec3 position);
        void setRotation(vec3 eulerAngles);
        void setScale(vec3 scale);
        void setColor(vec3 color);
        void setRenderMode(Shader::RenderMode renderMode);
        void setName(std::string name);

        // Transform
        const Actor* translate(vec3 translation);
        const Actor* rotate(float angle_degrees, vec3 axis);
        const Actor* scale(vec3 scale);

        // Lifecycle
        void bindResources();
        virtual void tick(float deltaTime);

        void printMesh();
        void printTransform();

    protected:
        std::string _name;

        GLuint _vao;
        GLuint _vertexBuffer, _colorBuffer, _indiceBuffer, _normalBuffer;

        vector<GLfloat> _vertices;
        vector<GLuint> _indices;
        vector<GLfloat> _colors;
        vector<GLfloat> _texCoords;
        vector<GLfloat> _normals;
        Shader::RenderMode _renderMode;

        Shader* _shader;
        mat4 _modelMatrix;

        void loadImage(const char* filename);
};

#endif