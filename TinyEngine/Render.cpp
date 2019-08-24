#pragma once
#include "Render.h"
#include "Standard.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Texture.h"


namespace TEngine {

	Render::Render(Object* obj) : Component(obj) {
	}

	void Render::LoadModelMat(const aiScene * scene, std::string path) {
		if (scene == nullptr || !scene->HasMaterials()) return;
		for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
			Standard* mat = new Standard();
			unsigned int df = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
			unsigned int spec = scene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR);
			unsigned int norm = scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS);
			unsigned int opac = scene->mMaterials[i]->GetTextureCount(aiTextureType_OPACITY);
			if (df) {
				aiString str;
				scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &str);
				mat->diffuseMap = new Texture();
				for (unsigned int j = 0; j < str.length; j++) {
					mat->diffuseMap->path.push_back(str.C_Str()[j]);
				}
				mat->diffuseMap->path = path + mat->diffuseMap->path;
			} else if (opac) {
				aiString str;
				scene->mMaterials[i]->GetTexture(aiTextureType_OPACITY, 0, &str);
				mat->diffuseMap = new Texture();
				for (unsigned int j = 0; j < str.length; j++) {
					mat->diffuseMap->path.push_back(str.C_Str()[j]);
				}
				mat->diffuseMap->path = path + mat->diffuseMap->path;
			}
			if (norm) {
				aiString str;
				scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, 0, &str);
				mat->normalMap = new Texture();
				for (unsigned int j = 0; j < str.length; j++) {
					mat->normalMap->path.push_back(str.C_Str()[j]);
				}
				mat->normalMap->path = path + mat->normalMap->path;
			}
			materials.push_back(mat);
		}
	}

	Render::~Render() {
		//for (unsigned int i = 0; i < materials.size(); i++) 
		//	delete materials[i];
	}
}