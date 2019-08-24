#pragma once
#include "Material.h"

namespace TEngine {
	class Texture;
	class Standard : public Material {
	private:

	public:
		unsigned int skybox;
		vec3 diffuseColor;
		Texture* diffuseMap;
		vec3 specularColor;
		Texture* specularMap;
		Texture* normalMap;
		vec3 selfLuminous;
		float smoothness;
	public:
		Standard();
		void Use(Camera* camera, Object* obj, Mesh* mesh);
		virtual ~Standard();
	};

}