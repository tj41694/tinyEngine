#pragma once
#include "Material.h"

namespace TEngine {
	class Texture;
	class Standard : public Material {
	public:
		bool          depTest;
		unsigned int  skybox;
		vec3          diffuseColor;
		Texture*      diffuseMap;
		vec3          specularColor;
		Texture*      specularMap;
		Texture*      normalMap;
		vec3          selfLuminous;
		float         smoothness;
	public:
		Standard();
		void Use(Camera* camera, Object* obj, DrawCmd* mesh);
		virtual ~Standard();

	protected:
		GLuint specularTexLoc = 0;
		GLuint normalTexLoc = 0;
		GLuint tskyboxTexLoc = 0;
		GLuint diffuseTexLoc = 0;
	};

}