#pragma once
#include "Material.h"
#include <glm/detail/type_vec.hpp>
namespace TEngine {
	class Texture;
	class BeltStripMat : public Material {
	public:
		glm::vec3     diffuseColor;
		Texture*      diffuseMap;
		float         beltWidth;
	public:
		BeltStripMat();
		void Use(Camera* camera, Object* obj, DrawCmd* mesh);
		~BeltStripMat();
	};
}