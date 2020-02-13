#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 a_normal;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform float aspectRatio;
uniform float thickness;

void main() {
  vec2 aspectVec = vec2(aspectRatio, 1.0);
  mat4 projViewModel = projection * view * model;
  vec4 polylinePointProjected = projViewModel * vec4(position, 1.0);
  vec4 extrudedPointProjected = projViewModel * vec4(position + a_normal, 1.0);

  //get 2D screen space with W divide and aspect correction
  vec2 polylinePointScreen = polylinePointProjected.xy / polylinePointProjected.w * aspectVec;
  vec2 extrudedPointScreen = extrudedPointProjected.xy / extrudedPointProjected.w * aspectVec;

  vec2 normalScreen = normalize(extrudedPointScreen - polylinePointScreen);
  normalScreen *= thickness * polylinePointProjected.w * 0.5f;
  normalScreen.x /= aspectRatio;

  vec4 offset = vec4(normalScreen, 0.0, 0.0);
  gl_Position = polylinePointProjected + offset;
}
