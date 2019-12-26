#pragma once
#include "MeshFilter.h"
#include "Engine/Object.h"
#include "Engine/Materials/Material.h"
#include "Render.h"
#include "Engine/DrawCommand/Mesh.h"


namespace TEngine {
	MeshFilter::MeshFilter() : Component() {}

	void MeshFilter::LoadModel(const aiScene* scene) {
		if (scene == nullptr) return;
		processNode(scene->mRootNode, scene);
	}

	void MeshFilter::processNode(aiNode* node, const aiScene* scene) {
		// 处理节点所有的网格（如果有的话）
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* ms = processMesh(aimesh, scene);
			drawCmds.push_back(ms);
		}
		// 接下来对它的子节点重复这一过程
		for (unsigned int i = 0; i < node->mNumChildren; i++)
			processNode(node->mChildren[i], scene);
	}

	Mesh* MeshFilter::processMesh(aiMesh* mesh_, const aiScene* scene) {
		Mesh* mesh = new Mesh();
		mesh->vertices.data = new float[mesh_->mNumVertices * 3];
		mesh->vertices.count = mesh_->mNumVertices * 3;
		mesh->normals.data = new float[mesh_->mNumVertices * 3];
		mesh->normals.count = mesh_->mNumVertices * 3;
		mesh->uv0.data = new float[mesh_->mNumVertices * 2];
		mesh->uv0.count = mesh_->mNumVertices * 2;
		for (unsigned int i = 0; i < mesh_->mNumFaces; i++) { mesh->triangles.count += mesh_->mFaces[i].mNumIndices; }
		mesh->triangles.data = new GLuint[mesh->triangles.count];

		for (unsigned int i = 0; i < mesh_->mNumVertices; i++) {
			mesh->vertices.data[i * 3] = mesh_->mVertices[i].x;
			mesh->vertices.data[i * 3 + 1] = mesh_->mVertices[i].y;
			mesh->vertices.data[i * 3 + 2] = mesh_->mVertices[i].z;

			mesh->normals.data[i * 3] = mesh_->mNormals[i].x;
			mesh->normals.data[i * 3 + 1] = mesh_->mNormals[i].y;
			mesh->normals.data[i * 3 + 2] = mesh_->mNormals[i].z;
			if (mesh_->mTextureCoords[0]) {
				mesh->uv0.data[i * 2] = mesh_->mTextureCoords[0][i].x;
				mesh->uv0.data[i * 2 + 1] = mesh_->mTextureCoords[0][i].y;
			}
		}
		unsigned int tmpIndex = 0;
		for (unsigned int i = 0; i < mesh_->mNumFaces; i++) {
			for (unsigned int j = 0; j < mesh_->mFaces[i].mNumIndices; j++) {
				mesh->triangles.data[tmpIndex++] = mesh_->mFaces[i].mIndices[j];
			}
		}
		mesh->materialIndex = mesh_->mMaterialIndex;
		return mesh;
	}

	void MeshFilter::DrawMeshes(Camera* camera) {
		if (render == nullptr) {
			render = obj->GetComponent<Render>();
		}
		else {
			for (GLuint i = 0; i < drawCmds.size(); i++) {
				DrawCmd* mesh = drawCmds[i];
				Material* mat;
				if (render->materials.size() > mesh->materialIndex)
					mat = render->materials[mesh->materialIndex];
				else
					mat = Material::GetDefaultMaterial();
				mat->Use(camera, obj, mesh);
			}
		}
	}

	MeshFilter::~MeshFilter() {
		for (GLuint i = 0; i < drawCmds.size(); i++)
			delete drawCmds[i];
	}
}