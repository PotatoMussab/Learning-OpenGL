#version 330 core
in vec2 texPos;
out vec4 fColor;

uniform sampler2D texSampler;
void main()
{
    fColor = texture(texSampler, texPos);
}