#version 330 core

in vec2 uv; // Interpolated values from the vertex shaders
in vec3 fragmentColor;
in vec3 normalVector;
out vec4 color;

uniform sampler2D textureSampler; // constant for the whole mesh.
uniform int renderMode = 0;

void main(){
  if(renderMode == 0) color.xyz = fragmentColor;
  else if(renderMode == 1) color.xyz = texture( textureSampler, uv ).rgb;
  else if(renderMode == 2) color.xyz = normalVector;
  color.a = 1.0;
}