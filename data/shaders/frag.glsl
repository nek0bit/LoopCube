#version 330 core

in vec3 inColor;

out vec4 outColor;

void main()
{
    outColor = vec4(inColor, 1.0);
}
