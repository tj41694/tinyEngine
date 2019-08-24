#pragma once
#include <string>
#include "esUtil.h"
#include "glm/glm.hpp"

namespace TEngine {
	using namespace glm;
	class Object;
	class Mesh;
	class Camera;
	enum MatrixType { Model, View, Projection };
	class Material {
	private:
		static Material* defaultMaterial;
	protected:
		GLuint mat_model;
		GLuint mat_view;
		GLuint mat_projection;
		const GLuint shaderId;
	public:
		static Material* GetDefaultMaterial();

		Material(const std::string& vShader, const std::string& fShader);
		virtual void Use(Camera* camera, Object* obj, Mesh* mesh);
		void SetFloat(const char * name, const float & value);
		void SetVector(const char* name, const vec3 & vec3);
		void SetMatrix(const char * name, const mat4& mat4);
		void SetMatrix(const mat4& mat4, MatrixType type);
		virtual ~Material();
	};
}