#pragma once
#include <string>
#include "glm/glm.hpp"
#include <unordered_map>

namespace TEngine {
	class Object;
	class DrawCmd;
	class Camera;
	class Material {
	private:
		static Material* defaultMaterial;
	protected:
		std::unordered_map<std::string, unsigned int> uniformLocationsMap;
		const unsigned int shaderId;
	public:
		static Material* GetDefaultMaterial();

		Material(const char* vShader, const char* fShader);
		Material(const char* vShader, const char* fShader, int flag);
		Material(const char* vShader, const char* gShader, const char* fShader);
		virtual void Use(Camera* camera, Object* obj, DrawCmd* mesh);
		void SetFloat(const char * name, const float & value);
		void SetVector(const char* name, const glm::vec3 & vec3);
		void SetMatrix(const char* name, const glm::mat4& mat4);
		virtual ~Material();
	};
}