#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 prev;
layout(location = 2) in vec3 next;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform float aspectRatio;
uniform float thickness;

void main() {
  vec2 aspectVec = vec2(aspectRatio, 1.0);

  // Get 3D clip space coordinates
  mat4 projViewModel = projection * view * model;
  vec4 currentProjected = projViewModel * vec4(position, 1.0);
  vec4 prevProjected = projViewModel * vec4(prev, 1.0);
  vec4 nextProjected = projViewModel * vec4(next, 1.0);

  // Get 2D screen space with W divide and aspect correction
  vec2 currentScreen = currentProjected.xy / abs(currentProjected.w) * aspectVec;
  vec2 prevScreen = prevProjected.xy / abs(prevProjected.w) * aspectVec;
  vec2 nextScreen = nextProjected.xy / abs(nextProjected.w) * aspectVec;

  // Now compute the miter join normal and length
  vec2 toCurr = normalize(currentScreen - prevScreen);
  vec2 toNext = normalize(nextScreen - currentScreen);
  vec2 tangent = normalize(toCurr + toNext);
  vec2 normalScreen = vec2(-tangent.y, tangent.x);
  normalScreen *= thickness / dot(tangent, toCurr) * currentProjected.w * 0.5f;
  normalScreen.x /= aspectRatio;

  // Do the extrusion
  float vertexID = float(gl_VertexID);
  float extrudeLeft = float(mod(vertexID, 2.0) < 1.0);
  vec4 offset = vec4((extrudeLeft * 2.0 - 1.0) * normalScreen, 0.0, 0.0);
  gl_Position = currentProjected + offset;
}
