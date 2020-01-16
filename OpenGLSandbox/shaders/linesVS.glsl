#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 auxPos;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform float aspectRatio;
uniform float thickness;

void main() {
  vec2 aspectVec = vec2(aspectRatio, 1.0);
  mat4 projViewModel = projection * view * model;
  vec4 currentProjected = projViewModel * vec4(position, 1.0);
  vec4 auxProjected = projViewModel * vec4(auxPos, 1.0);

  //get 2D screen space with W divide and aspect correction
  vec2 currentScreen = currentProjected.xy / currentProjected.w * aspectVec;
  vec2 auxScreen = auxProjected.xy / auxProjected.w * aspectVec;
  
  vec2 dir = normalize(
      gl_VertexID % 4 < 2 ?
      (auxScreen - currentScreen) :
      (currentScreen - auxScreen));
  vec2 normal = vec2(-dir.y, dir.x);
  normal *= thickness/2.0;
  normal.x /= aspectRatio;

  vec4 offset = vec4(gl_VertexID % 2 == 0 ? normal : -normal, 0.0, 1.0);
  gl_Position = currentProjected + offset;
}
