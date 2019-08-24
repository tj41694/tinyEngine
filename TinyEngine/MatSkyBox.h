#pragma once
#include "Material.h"

namespace TEngine {
	class MatSkyBox : public Material {
	public:
		MatSkyBox();
		void Use(Camera* camera, Object* obj, Mesh* mesh);
		virtual ~MatSkyBox();
	};
}