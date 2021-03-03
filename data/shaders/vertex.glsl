#version 330 core

in vec3 position;
in vec2 texCoord;

out vec2 inTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    inTexCoord = texCoord;
    //gl_Position = projection * view * model * vec4(position, 1.0);
    gl_Position = projection * view * model * vec4(position, 1.0);
}
