#version 330 core

in vec3 vertexColor;

uniform float ourRed;

out vec4 color;

void main()
{
	color = vec4(ourRed, vertexColor.y, vertexColor.z, 1);
}