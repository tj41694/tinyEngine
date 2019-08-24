#version 300 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourNormal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
	vec4 fragPos = model * vec4(aPos, 1.0);
	gl_Position = projection * view * fragPos;
    FragPos = vec3(fragPos);
	ourNormal = mat3(transpose(inverse(model))) * aNormal;;
	TexCoord = aTexCoord;
}