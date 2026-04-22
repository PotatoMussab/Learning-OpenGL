#version 330
in vec3 vertexColor;
in vec2 texPos;
out vec4 fragColor;

uniform sampler2D texSampler;
void main()
{
    fragColor = texture(texSampler, texPos);
}