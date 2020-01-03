#pragma once
#include "Material.h"
#include <glm/detail/type_vec.hpp>

namespace TEngine {
	class Texture;
	class Standard : public Material {
	public:
		bool          depTest;
		unsigned int  skybox;
		glm::vec3     diffuseColor;
		Texture*      diffuseMap;
		glm::vec3     specularColor;
		Texture*      specularMap;
		Texture*      normalMap;
		glm::vec3     selfLuminous;
		float         smoothness;
	public:
		Standard();
		void Use(Camera* camera, Object* obj, DrawCmd* mesh);
		virtual ~Standard();

	protected:
		unsigned int specularTexLoc = 0;
		unsigned int normalTexLoc = 0;
		unsigned int tskyboxTexLoc = 0;
		unsigned int diffuseTexLoc = 0;
	};

}