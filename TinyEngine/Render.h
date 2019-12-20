#pragma once
#include "Component.h"
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

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