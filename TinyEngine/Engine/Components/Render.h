#pragma once
#include "Component.h"
#include <vector>
#include <string>

struct aiScene;

namespace TEngine {
	class Material;
	class Render : public Component {
	public:
		std::vector<Material*> materials;

	public:
		Render();
		virtual ~Render();
		void LoadModelMat(const aiScene* scene, std::string path);
	};
}