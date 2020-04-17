#pragma once
#include "../Object.h"
#include "Standard.h"
#include "../FileSystem.h"
#include "Engine/Components/Camera.h"
#include "../Texture.h"
#include "Engine/DrawCommand/Mesh.h"
#include "Engine/Global.h"

namespace TEngine {
	using namespace glm;
	Standard::Standard() : Material("./resources/shaders/Standard.vs", "./resources/shaders/Standard.fs") {
		depTest = true;
		skybox = Global::skyboxTexture;
		specularTexLoc = glGetUniformLocation(shaderId, "specularTex");
		normalTexLoc = glGetUniformLocation(shaderId, "normalTex");
		tskyboxTexLoc = glGetUniformLocation(shaderId, "skyboxTex");
		diffuseTexLoc = glGetUniformLocation(shaderId, "diffuseTex");
		diffuseMap = nullptr;
		diffuseColor = glm::vec3(0.9f);
		specularMap = nullptr;
		smoothness = 20.0f;
		specularColor = glm::vec3(0.1f);
		normalMap = nullptr;
		selfLuminous = glm::vec3(0.0f);
	}

	void Standard::Use(Camera* camera, Object* obj, DrawCmd* mesh) {
		glUseProgram(shaderId);
		SetFloat("smoothness", smoothness);
		SetMatrix("model", obj->Trans()->LocalToWorldMarix());
		SetMatrix("view", camera->GetViewMatrix());
		SetMatrix("projection", camera->GetProjectionMatrix());
		SetVector("sunPos", glm::vec3(-3000, 20000, 6000));
		SetVector("sunColor", glm::vec3(1.0f, 0.889f, 0.675f) * 0.4f);
		SetVector("diffuseColor", diffuseColor);
		SetVector("specularColor", specularColor);
		SetVector("ambientLighting", glm::vec3(0.4191f, 0.6332f, 0.8980f) * 0.7f);
		SetVector("camPos", camera->obj->Trans()->Positon());
		SetVector("selfLuminousColor", selfLuminous);
		glUniform1i(specularTexLoc, 1);
		if (diffuseMap) {
			glUniform1i(diffuseTexLoc, 0);
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
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (normalMap) {
			glUniform1i(normalTexLoc, 2);
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
		glUniform1i(tskyboxTexLoc, 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
		if (depTest) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}

		mesh->Draw();
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, 0);
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