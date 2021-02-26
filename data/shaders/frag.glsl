#version 330 core

in vec2 inTexCoord;

out vec4 outColor;

uniform sampler2D tex;

void main()
{
    outColor = texture(tex, inTexCoord);
}
