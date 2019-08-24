#version 300 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourNormal;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourNormal = mat3(transpose(inverse(model))) * aNormal;
	TexCoord = aTexCoord;
}