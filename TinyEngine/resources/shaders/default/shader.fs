#version 300 es

precision highp float;

in vec3 ourNormal;
in vec2 TexCoord;

out vec4 fragColor;

void main()
{
   fragColor = vec4(ourNormal, 1.0f);
}