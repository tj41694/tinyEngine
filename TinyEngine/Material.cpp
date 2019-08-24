#pragma once
#include "Material.h"
#include "Camera.h"
#include "Object.h"
#include "FileSystem.h"
#include "glm/gtc/type_ptr.hpp"
#include "Mesh.h"


namespace TEngine {
	using namespace glm;

	Material::Material(const  std::string& vShader, const std::string& fShader) :
		shaderId(esLoadProgram(FileSystem::ReadFile(vShader.c_str()).c_str(), FileSystem::ReadFile(fShader.c_str()).c_str())) {
		mat_model = glGetUniformLocation(shaderId, "model");
		mat_view = glGetUniformLocation(shaderId, "view");
		mat_projection = glGetUniformLocation(shaderId, "projection");
	}

	//Ðéº¯Êý
	void Material::Use(Camera* camera, Object* obj, Mesh* mesh) {
		glUseProgram(shaderId);
		SetMatrix(obj->LocalToWorldMarix(), Model);
		SetMatrix(camera->GetViewMatrix(), View);
		SetMatrix(camera->GetProjectionMatrix(), Projection);
		mesh->Draw();
	}

	Material* Material::defaultMaterial = nullptr;
	Material* Material::GetDefaultMaterial() {
		if (!defaultMaterial)
			defaultMaterial = new Material("./resources/shaders/default/shader.vs", "./resources/shaders/default/shader.fs");
		return defaultMaterial;
	}

	void Material::SetFloat(const char* name, const float& value) {
		static char contrast[64] = { 0 };
		static GLuint id = -1;
		if (strcmp(name, contrast) != 0) {

			strcpy(contrast, name);
			id = glGetUniformLocation(shaderId, name);
		}
		glUniform1f(id, value);
	}

	void Material::SetMatrix(const mat4& mat4, MatrixType type) {
		switch (type) {
		case MatrixType::Model:
			glUniformMatrix4fv(mat_model, 1, GL_FALSE, value_ptr(mat4));
			break;
		case MatrixType::View:
			glUniformMatrix4fv(mat_view, 1, GL_FALSE, value_ptr(mat4));
			break;
		case MatrixType::Projection:
			glUniformMatrix4fv(mat_projection, 1, GL_FALSE, value_ptr(mat4));
			break;
		}
	}

	void Material::SetVector(const char* name, const vec3& vec3) {
		static char contrast[64] = { 0 };
		static GLuint id = -1;
		if (strcmp(name, contrast) != 0) {

			strcpy(contrast, name);
			id = glGetUniformLocation(shaderId, name);
		}
		glUniform3fv(id, 1, &vec3[0]);
	}

	void Material::SetMatrix(const char* name, const mat4& mat4) {
		static char contrast[64] = { 0 };
		static GLuint id = -1;
		if (strcmp(name, contrast) != 0) {

			strcpy(contrast, name);
			id = glGetUniformLocation(shaderId, name);
		}
		glUniformMatrix4fv(id, 1, GL_FALSE, value_ptr(mat4));
	}

	Material::~Material() {}
}