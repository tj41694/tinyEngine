#pragma once
#include <vector>
#include "Component.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace TEngine {
	class Mesh;
	class Render;
	class Camera;
	class DrawCmd;
	class MeshFilter : public Component {
		Render * render = nullptr;
		void processNode(aiNode *node, const aiScene *scene);
		Mesh* processMesh(aiMesh *mesh_, const aiScene *scene);
	public:
		std::vector<DrawCmd*> drawCmds;
		MeshFilter(Object* obj);
		void LoadModel(const aiScene *scene);
		void DrawMeshes(Camera* camera);
		virtual ~MeshFilter();
	};
}