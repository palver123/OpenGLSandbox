#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vData; 

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float aspectRatio;
uniform float thickness;

void main() {
  vec2 aspectVec = vec2(aspectRatio, 1.0);
  mat4 projViewModel = projection * view * model;
  vec4 previousProjected = projViewModel * vec4(vData.xyz, 1.0);
  vec4 currentProjected = projViewModel * vec4(position, 1.0);

  //get 2D screen space with W divide and aspect correction
  vec2 currentScreen = currentProjected.xy / currentProjected.w * aspectVec;
  vec2 previousScreen = previousProjected.xy / previousProjected.w * aspectVec;

  vec2 dir = normalize(currentScreen - previousScreen);
  vec2 normal = vec2(-dir.y, dir.x);
  normal *= thickness/2.0;
  normal.x /= aspectRatio;

  vec4 offset = vec4(normal * vData.w, 0.0, 1.0);
  gl_Position = currentProjected + offset;
//  gl_PointSize = 1.0;
}

