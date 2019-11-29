#version 330 core

layout(location = 0) in vec3 position;

out vec3 vertexColor;

void main()
{
	gl_Position = vec4(position.xyz, 1.0);
	vertexColor = vec3((position.x + 1.0f) / 2.0f, (position.y + 1.0f) / 2.0f, (position.z + 1.0f) / 2.0f);
}