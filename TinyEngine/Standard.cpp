#pragma once
#include "Standard.h"
#include "Object.h"
#include "FileSystem.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "Global.h"

namespace TEngine {
	Standard::Standard() : Material("./resources/shaders/Standard.vs", "./resources/shaders/Standard.fs") {
		skybox = Global::skyboxTexture;
		mat_view = glGetUniformLocation(shaderId, "view");
		mat_projection = glGetUniformLocation(shaderId, "projection");
		diffuseMap = nullptr;
		diffuseColor = vec3(0.9f);
		specularMap = nullptr;
		smoothness = 20.0f;
		specularColor = vec3(0.1f);
		normalMap = nullptr;
		selfLuminous = vec3(0.0f);
	}

	void Standard::Use(Camera* camera, Object* obj, Mesh* mesh) {
		glUseProgram(shaderId);
		SetFloat("smoothness", smoothness);
		SetMatrix(obj->LocalToWorldMarix(), Model);
		SetMatrix(camera->GetViewMatrix(), View);
		SetMatrix(camera->GetProjectionMatrix(), Projection);
		SetVector("sunPos", vec3(-3000, 20000, 6000));
		SetVector("sunColor", vec3(1.0f, 0.889f, 0.675f) * 0.5f);
		SetVector("diffuseColor", diffuseColor);
		SetVector("specularColor", specularColor);
		SetVector("ambientLighting", vec3(0.4191f, 0.6332f, 0.8980f) * 0.7f);
		SetVector("camPos", camera->obj->WorldPos());
		SetVector("selfLuminousColor", selfLuminous);
		GLuint diffuseTexLoc = glGetUniformLocation(shaderId, "diffuseTex");
		glUniform1i(diffuseTexLoc, 0);
		GLuint specularTexLoc = glGetUniformLocation(shaderId, "specularTex");
		glUniform1i(specularTexLoc, 1);
		GLuint normalTexLoc = glGetUniformLocation(shaderId, "normalTex");
		glUniform1i(normalTexLoc, 2);
		GLuint tskyboxTexLoc = glGetUniformLocation(shaderId, "skyboxTex");
		glUniform1i(tskyboxTexLoc, 3);
		if (diffuseMap) {
			if (diffuseMap->id == 0) {
				glGenTextures(1, &diffuseMap->id);
				int width, height, nrComponents;
				unsigned char* data = FileSystem::LoadTexture(diffuseMap->path.c_str(), &width, &height, &nrComponents, 0);
				if (data) {
					GLenum format = GL_RGBA;
					if (nrComponents == 1) {
						format = GL_RED;
					}
					else if (nrComponents == 3) {
						format = GL_RGB;
					}
					else if (nrComponents == 4) {
						format = GL_RGBA;
					}
					glBindTexture(GL_TEXTURE_2D, diffuseMap->id);
					glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					FileSystem::FreeTextur(data);
				}
			}
			glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活对应纹理单元
			glBindTexture(GL_TEXTURE_2D, diffuseMap->id);
		}
		else {
			glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活对应纹理单元
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (normalMap) {
			if (normalMap->id == 0) {
				glGenTextures(1, &normalMap->id);
				int width, height, nrComponents;
				unsigned char* data = FileSystem::LoadTexture(normalMap->path.c_str(), &width, &height, &nrComponents, 0);
				if (data) {
					GLenum format = GL_RGBA;
					if (nrComponents == 1) {
						format = GL_RED;
					}
					else if (nrComponents == 3) {
						format = GL_RGB;
					}
					else if (nrComponents == 4) {
						format = GL_RGBA;
					}
					glBindTexture(GL_TEXTURE_2D, normalMap->id);
					glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					FileSystem::FreeTextur(data);
				}
			}
			glActiveTexture(GL_TEXTURE2); // 在绑定纹理之前先激活对应纹理单元
			glBindTexture(GL_TEXTURE_2D, normalMap->id);
		}
		else {
			glActiveTexture(GL_TEXTURE2); // 在绑定纹理之前先激活对应纹理单元
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
		mesh->Draw();
	}

	Standard::~Standard() {
		glDeleteTextures(1, &diffuseMap->id);
		glDeleteTextures(1, &specularMap->id);
		glDeleteTextures(1, &normalMap->id);
		delete diffuseMap;
		delete specularMap;
		delete normalMap;
	}
}