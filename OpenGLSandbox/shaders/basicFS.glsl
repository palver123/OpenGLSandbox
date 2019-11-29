#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;

uniform float texBlendFactor;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

out vec4 color;

void main()
{
	vec4 wood = texture(ourTexture1, TexCoord);
	vec4 smiley = texture(ourTexture2, TexCoord);
	color = mix(wood, smiley, texBlendFactor);
}