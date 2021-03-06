#pragma once
#include "Engine/Object.h"
#include "Material.h"
#include "Engine/Components/Camera.h"
#include "Engine/FileSystem.h"
#include "glm/gtc/type_ptr.hpp"
#include "Engine/DrawCommand/Mesh.h"

namespace TEngine {
	using namespace glm;

	Material::Material(const char* vShader, const char* fShader) :
		shaderId(esLoadProgram(FileSystem::ReadFile(vShader).c_str(), FileSystem::ReadFile(fShader).c_str())) {}

	Material::Material(const char* vShader, const char* gShader, const char* fShader) :
		shaderId(esLoadProgram(FileSystem::ReadFile(vShader).c_str(), FileSystem::ReadFile(gShader).c_str(), FileSystem::ReadFile(fShader).c_str())) {}

	Material::Material(const char* vShader, const char* fShader, int flag):
		shaderId(esLoadProgram(vShader, fShader)){ }

	void Material::Use(Camera* camera, Object* obj, DrawCmd* cmd) {
		glUseProgram(shaderId);
		SetMatrix("model", obj->Trans()->LocalToWorldMarix());
		SetMatrix("view", camera->GetViewMatrix());
		SetMatrix("projection", camera->GetProjectionMatrix());
		cmd->Draw();
	}

	Material* Material::defaultMaterial = nullptr;
	Material* Material::GetDefaultMaterial() {
		if (!defaultMaterial)
			defaultMaterial = new Material("./resources/shaders/default/shader.vs", "./resources/shaders/default/shader.fs");
		return defaultMaterial;
	}

	void Material::SetFloat(const char* name, const float& value) {
		auto it = uniformLocationsMap.find(name);
		if (it != uniformLocationsMap.end()) {
			glUniform1f((*it).second, value);
		}
		else {
			uniformLocationsMap[name] = glGetUniformLocation(shaderId, name);
		}
	}

	void Material::SetMatrix(const char* name, const mat4& mat4) {
		auto it = uniformLocationsMap.find(name);
		if (it != uniformLocationsMap.end()) {
			glUniformMatrix4fv((*it).second, 1, GL_FALSE, value_ptr(mat4));
		}
		else {
			uniformLocationsMap[name] = glGetUniformLocation(shaderId, name);
		}
	}

	void Material::SetVector(const char* name, const vec3& vec3) {
		auto it = uniformLocationsMap.find(name);
		if (it != uniformLocationsMap.end()) {
			glUniform3fv((*it).second, 1, &vec3[0]);
		}
		else {
			uniformLocationsMap[name] = glGetUniformLocation(shaderId, name);
		}
	}

	Material::~Material() {}
}