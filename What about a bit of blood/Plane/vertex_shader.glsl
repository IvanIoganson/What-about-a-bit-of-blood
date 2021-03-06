#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec4 OurColor;
out vec2 TexCoord;

uniform float time;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
	TexCoord = vec2(texCoord.x, texCoord.y);
	OurColor = vec4(1.0, 1.0, 1.0, 1.0);
}
