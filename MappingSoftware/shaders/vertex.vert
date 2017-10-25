#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoord;

void main()
{
	TexCoord = uv;
    gl_Position = projection * model * vec4(position.xy, 0,  1.0);
}