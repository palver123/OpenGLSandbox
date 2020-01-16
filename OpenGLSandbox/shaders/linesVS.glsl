#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 prev;
layout(location = 2) in vec3 next;
layout(location = 3) in float orientation; 

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform float aspectRatio;
uniform float thickness;

void main() {
  vec2 aspectVec = vec2(aspectRatio, 1.0);
  mat4 projViewModel = projection * view * model;
  vec4 previousProjected = projViewModel * vec4(prev, 1.0);
  vec4 currentProjected = projViewModel * vec4(position, 1.0);
  vec4 nextProjected = projViewModel * vec4(next, 1.0);

  //get 2D screen space with W divide and aspect correction
  vec2 currentScreen = currentProjected.xy / currentProjected.w * aspectVec;
  vec2 previousScreen = previousProjected.xy / previousProjected.w * aspectVec;
  vec2 nextScreen = nextProjected.xy / nextProjected.w * aspectVec;
  
  //starting point uses (next - current)
  vec2 dir = vec2(0.0);
  if (currentScreen == previousScreen) {
    dir = normalize(nextScreen - currentScreen);
  } 
  //ending point uses (current - previous)
  else {
    //get directions from (C - B) and (B - A)
    dir = normalize((currentScreen - previousScreen));
  }
  vec2 normal = vec2(-dir.y, dir.x);
  normal *= thickness/2.0;
  normal.x /= aspectRatio;

  vec4 offset = vec4(normal * orientation, 0.0, 1.0);
  gl_Position = currentProjected + offset;
}
