#version 330 core
out vec4 FragColor;
in vec2 uv0;
uniform sampler2D mytexture;
void main()
{
	FragColor = texture2D(mytexture, uv0);
};