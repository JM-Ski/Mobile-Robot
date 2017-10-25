#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform vec3 colour;
uniform sampler2D tex;

void main()
{
    color =  texture(tex, TexCoord) * vec4(colour.x, colour.y, colour.z, 1.0f);
} 