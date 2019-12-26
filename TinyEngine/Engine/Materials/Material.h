#pragma once
#include <string>
#include "Engine/OpenglUtil.h"
#include "glm/glm.hpp"
#include <unordered_map>

namespace TEngine {
	using namespace glm;
	class Object;
	class DrawCmd;
	class Camera;
	class Material {
	private:
		static Material* defaultMaterial;
	protected:
		std::unordered_map<std::string, GLuint> uniformLocationsMap;
		const GLuint shaderId;
	public:
		static Material* GetDefaultMaterial();

		Material(const char* vShader, const char* fShader);
		Material(const char* vShader, const char* gShader, const char* fShader);
		virtual void Use(Camera* camera, Object* obj, DrawCmd* mesh);
		void SetFloat(const char * name, const float & value);
		void SetVector(const char* name, const vec3 & vec3);
		void SetMatrix(const char* name, const mat4& mat4);
		virtual ~Material();
	};
}