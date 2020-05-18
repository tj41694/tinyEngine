#pragma once

static const char* stdvs = "#version 440 core \n\
		layout(location = 0) in vec3 aPos;\
		layout(location = 1) in vec3 aNormal;\
		layout(location = 2) in vec2 aTexCoord;\
		uniform mat4 model;\
		uniform mat4 view;\
		uniform mat4 projection;\
		out vec3 ourNormal;\
		out vec2 TexCoord;\
		out vec3 FragPos;\
		out vec3 locationPos;\
		void main()\
		{\
			vec4 fragPos = model * vec4(aPos, 1.0);\
			gl_Position = projection * view * fragPos;\
			FragPos = vec3(fragPos);\
			locationPos.x = FragPos.x * 10.0f / 111.31f;\
			locationPos.y = FragPos.y * 10.0f;\
			locationPos.z = -(FragPos.z * 10.0f / 111.31f);\
			ourNormal = mat3(transpose(inverse(model))) * aNormal;;\
			TexCoord = aTexCoord;\
		}";
static const char* stdfs = "#version 440 core \n\
		precision highp float;\
		in vec3 ourNormal;\
		in vec2 TexCoord;\
		in vec3 FragPos;\
		in vec3 locationPos;\
		layout(location = 0) out vec4 fragColor;\
		layout(location = 1) out vec4 lacationColor;\
		uniform vec3 camPos;\
		uniform vec3 diffuseColor;\
		uniform vec3 specularColor;\
		uniform vec3 selfLuminousColor;\
		uniform float smoothness;\
		uniform sampler2D diffuseTex;\
		uniform sampler2D specularTex;\
		uniform sampler2D normalTex;\
		uniform samplerCube skyboxTex;\
		uniform vec3 ambientLighting;\
		struct Light {\
			vec3 pos;\
			vec3 color;\
			uint type;\
		};\
		uniform uint lightNum;\
		uniform Light lights[32];\
		uniform vec3 sunPos;\
		uniform vec3 sunColor;\
		void main()\
		{\
			fragColor = texture(diffuseTex, TexCoord);\
			lacationColor = vec4(locationPos.rgb, 1.0);\
		}";

static const char* textVs = "#version 300 es\n\
		layout(location = 0) in vec4 vertex;\
		out vec2 TexCoords;\
		uniform mat4 projection;\
		void main()\
		{\
			gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\
			TexCoords = vertex.zw;\
		}";

static const char* textFs = "#version 300 es\n\
		precision mediump float;\
		in vec2 TexCoords;\
		out vec4 color;\
		uniform sampler2D text;\
		uniform vec3 textColor;\
		void main()\
		{\
			vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\
			color = vec4(textColor, 1.0) * sampled;\
		}";
static const char* screenShaderVs = "#version 300 es\n\
		layout(location = 0) in vec2 aPos;\
		layout(location = 1) in vec2 aTexCoords;\
		out vec2 TexCoords;\
		void main()\
		{\
			gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\
			TexCoords = aTexCoords;\
		}";

static const char* screenShaderFs = "#version 300 es\n\
		precision highp float;\
		out vec4 FragColor;\
		in vec2 TexCoords;\
		uniform sampler2D screenTexture;\
		uniform sampler2D blurTexture;\
		uniform float exposure;\
		void main()\
		{\
			FragColor = texture(screenTexture, TexCoords);\
		}";