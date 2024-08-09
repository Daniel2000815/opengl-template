#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float deltaTime;

// Interpolated for each fragment
out vec3 fragmentColor;
out vec3 normalVector;
out vec2 uv;

void main(){
    vec3 pos = vertexPosition_modelspace + vec3(0.0, 0.0, sin(deltaTime + vertexPosition_modelspace.x));
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1.0);
    fragmentColor = vertexColor;
    uv = texCoord;

    mat3 norm_matrix = transpose(inverse(mat3(mvp)));
    // normalVector = normalize(norm_matrix * normal);
    normalVector = normal;
}