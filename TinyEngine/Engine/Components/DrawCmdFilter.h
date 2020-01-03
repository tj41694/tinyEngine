#pragma once
#include <vector>
#include "Component.h"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace TEngine {
	class Mesh;
	class Render;
	class Camera;
	class DrawCmd;
	class DrawCmdFilter : public Component {
	public:
		std::vector<DrawCmd*> drawCmds;
	public:
		DrawCmdFilter();
		void LoadModel(const aiScene* scene);
		void DrawCmds(Camera* camera);
		virtual ~DrawCmdFilter();
	private:
		Render* render = nullptr;
	private:
		void processNode(aiNode* node, const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh_, const aiScene* scene);
	};
}