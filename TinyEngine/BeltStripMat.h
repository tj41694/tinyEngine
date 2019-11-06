#pragma once
#include "Material.h"
namespace TEngine {
	class Texture;
	class BeltStripMat : public Material {
	public:
		vec3          diffuseColor;
		Texture*      diffuseMap;
		float         beltWidth;
	public:
		BeltStripMat();
		void Use(Camera* camera, Object* obj, DrawCmd* mesh);
		~BeltStripMat();
	};
}