#ifndef __SCENECOMPONENT__H_
#define __SCENECOMPONENT__H_

#include <Shader/Shader.h>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <Physics/Collider.h>


using vec3 = glm::vec3;
using mat3 = glm::mat3;
using mat4 = glm::mat4;
using std::vector;

class Shader;
class Line;

struct Transform {
    vec3 position;
    vec3 scale;
    vec3 rotation;
    vec3 velocity;
    vec3 angularVelocity;

    Transform() : position(0.0f), scale(1.0f), rotation(0.0f), velocity(0.0f), angularVelocity(0.0f) {}
};

class Actor
{
    private:
        std::string _texturePath;
        std::vector<Line*> debugNormals;

        bool _modelMatrixDirty = false;
        
    public:
        // Constructors
        Actor(Shader* shader);
        Actor(Shader* shader, std::string name);
        Actor(Shader* shader, std::string name, std::string texturePath);
        Actor(Shader* shader, const vector<GLfloat> &vertices, const vector<GLuint> &indices);
        Actor(Shader* shader, const vector<GLfloat> &vertices, const vector<GLuint> &indices, vector<GLfloat> colors);

        // Getters
        inline const std::vector<GLfloat>& vertices() const { return _vertices; }
        inline const std::vector<GLfloat>& normals() const { return _normals; }
        std::vector<vec3> verticesWorld() const;
        std::vector<vec3> normalsWorld() const;
        std::vector<vec3> packedVertices() const;

        inline vector<GLuint>   indices()        { return _indices; }
        inline vector<GLfloat>  colors()         { return _colors; }
        inline vector<GLfloat>  uv()             { return _texCoords; }
        inline Shader* shader() const { return _shader; };

        inline const Transform*        transform() const     { return _transform; }
        inline float mass() const { return _mass; }
        inline float elasticity() const { return _elasticity; }
        inline float kinematic() const { return _kinematic; }
        inline const Collider* collider() const { return _collider; }
        inline vec3             position()       { return _transform->position; }
        inline vec3             rotation()       { return _transform->rotation; }
        inline vec3             scale()          { return _transform->scale; }
        inline mat4             modelMatrix() const   { return _modelMatrix; }
        inline const char*      name()  const         { return _name.c_str(); }
        
        inline vec3 vertex(int vertexIdx) const {
            return vec3(_vertices[3 * vertexIdx], _vertices[3 * vertexIdx + 1], _vertices[3 * vertexIdx + 2]);
        }

        inline vec3 normal(int vertexIdx) const {
            return vec3(_normals[3 * vertexIdx], _normals[3 * vertexIdx + 1], _normals[3 * vertexIdx + 2]);
        }

        vec3 vertexWorld(int vertexIdx) const;
        vec3 normalWorld(int vertexIdx) const;

        // Setters
        void setPosition(vec3 position);
        void setRotation(vec3 eulerAngles);
        void setScale(vec3 scale);
        void setColor(vec3 color);
        void setVertex(size_t idx, vec3 localPosition);
        void setRenderMode(Shader::RenderMode renderMode);
        void setModelMatrix(mat4 m) { _modelMatrix = m; }
        void setName(std::string name);
        void setKinematic(bool kinematic) { _kinematic = kinematic; }
        void setMass(float m) { _mass = m; }
        void setElasticity(float e) { _elasticity = e; }

        // Transform
        void updateModelMatrix();
        const Actor* translate(vec3 translation);
        const Actor* rotate(vec3 angle_radians);
        const Actor* scale(vec3 scale);

        virtual mat3 inertiaTensor() const = 0;

        void addVelocity(vec3 v);
        void setVelocity(vec3 v) { _transform->velocity = v; }
        void addTorque(vec3 t);
        void setAngularVelocity(vec3 v) { _transform->angularVelocity = v; }

        glm::mat3 rotationMatrix() const;
        

        // Lifecycle
        void bindResources();
        virtual void tick(float deltaTime);

        void printMesh();
        void printTransform();

    protected:
        std::string _name;
        Collider* _collider;
        Transform* _transform;
        bool _kinematic = false;
        float _mass = 1.0f;
        float _elasticity = 1.0f;   // [0,1]


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