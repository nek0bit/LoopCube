#version 330 core

in vec3 position;
in vec2 texCoord;

out vec2 inTexCoord;

void main()
{
    inTexCoord = texCoord;
    gl_Position = vec4(position, 1.0);
}
