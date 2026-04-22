#version 330 core
layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inColor;
layout (location=2) in vec2 inTexPos;
out vec3 vertexColor;
out vec2 texPos;
void main()
{
gl_Position = vec4(inPos, 1.0f);
vertexColor = inColor;
texPos = vec2(inTexPos);
}