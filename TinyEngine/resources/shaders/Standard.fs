#version 300 es
precision highp float;

in vec3 ourNormal;
in vec2 TexCoord;
in vec3 FragPos;

layout (location = 0) out vec4 fragColor;
//layout (location = 1) out vec4 BrightColor;

uniform vec3 camPos;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 selfLuminousColor;
uniform float smoothness;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;
uniform samplerCube skyboxTex;


uniform vec3 ambientLighting;
struct Light {
	vec3 pos;
	vec3 color;
	uint type;
};
uniform uint lightNum;
uniform Light lights[32];

uniform vec3 sunPos;
uniform vec3 sunColor;


float specularStrength = 4.5;

void main()
{
	//漫反射
	vec3 norm = normalize(ourNormal); //标准化法向量
	vec3 lightDir = normalize(sunPos - FragPos); //物体表面至太阳标准化向量
	float diff = max(dot(norm, lightDir), 0.0) * 3.0f; //cos值
	vec3 diffuse = diff * sunColor * diffuseColor; //太阳漫反射强度
	
	//高光
	vec3 viewDir = normalize(camPos - FragPos);//物体表面至摄像机标准化向量
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), smoothness);
	vec3 specular = specularStrength * spec * sunColor * specularColor;
	
	//反射
	vec3 R = reflect(-viewDir, norm);
	vec4 reflectColor = texture(skyboxTex, R) * vec4(specularColor, 1.0f);
	
	vec3 result = ambientLighting + diffuse + specular ;
	
	vec4 texColor = texture(diffuseTex, TexCoord) * vec4(result, 1.0f);
	//if(texColor.a < 0.05f)
    //    discard;
	fragColor = texColor + reflectColor + vec4(selfLuminousColor, 1.0f);
	
	//float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	//if(brightness > 0.5)
	//BrightColor = vec4(fragColor.rgb, 1.0);
	
	
}