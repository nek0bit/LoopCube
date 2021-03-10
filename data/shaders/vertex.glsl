#version 330 core

in vec3 position;
in vec2 texCoord;
in vec3 instanceTrans;
    
out vec2 inTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    inTexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position + instanceTrans, 1.0);
}
