#version 330 core
in vec3 vNormal;

out vec4 fColor;

uniform vec3 color;

void main()
{
    fColor = vec4(color, 1.0f);
}