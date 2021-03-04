#version 330 core

in vec2 inTexCoord;

out vec4 outColor;

uniform sampler2D tex;
uniform vec2 texPos;

void main()
{
    outColor = texture(tex, inTexCoord * texPos);
}
