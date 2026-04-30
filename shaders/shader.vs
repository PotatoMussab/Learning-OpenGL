#version 330 core
layout (location=0) in vec3 inPos;
layout (location=1) in vec2 inTexPos;
out vec2 texPos;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 local;

void main()
{
gl_Position = proj * view * local * vec4(inPos, 1.0f);
texPos = vec2(inTexPos);
}