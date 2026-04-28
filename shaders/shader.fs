#version 330 core
in vec2 texPos;
out vec4 fragColor;

uniform sampler2D texSampler;
void main()
{
    fragColor = texture(texSampler, texPos);
}