#version 330 core

uniform vec3 matDiffColor;

out vec4 color;

void main() {
  color = vec4(matDiffColor.xyz, 1.0);
}