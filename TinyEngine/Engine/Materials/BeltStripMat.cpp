#include "../Object.h"
#include "BeltStripMat.h"
#include "../Texture.h"
#include "../FileSystem.h"
#include "Engine/DrawCommand/Mesh.h"
#include "Engine/Components/Camera.h"

namespace TEngine {
	BeltStripMat::BeltStripMat() :Material("./resources/shaders/beltStrip/beltStrip.vs", "./resources/shaders/beltStrip/beltStrip.fs") {
		beltWidth = 1;
		diffuseColor = glm::vec3(1);
		diffuseMap = nullptr;
	}

	void BeltStripMat::Use(Camera* camera, Object* obj, DrawCmd* mesh) {
		glUseProgram(shaderId);
		SetFloat("beltWidth", beltWidth);
		SetVector("diffuseColor", diffuseColor);
		SetMatrix("MVP", camera->GetProjectionMatrix() * camera->GetViewMatrix() * obj->Trans()->LocalToWorldMarix());

		if (diffuseMap) {
			GLuint diffuseTexLoc = glGetUniformLocation(shaderId, "diffuseTex");
			glUniform1i(diffuseTexLoc, 0);
			if (diffuseMap->id == 0) {
				glGenTextures(1, &diffuseMap->id);
				int width, height, nrComponents;
				unsigned char* data = FileSystem::LoadTexture(diffuseMap->path.c_str(), &width, &height, &nrComponents, 0);
				if (data) {
					GLenum format = GL_RGB;
					switch (nrComponents) {
					case 1: format = GL_RED; break;
					case 3: format = GL_RGB; break;
					case 4: format = GL_RGBA; break;
					}
					glBindTexture(GL_TEXTURE_2D, diffuseMap->id);
					glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					FileSystem::FreeTextur(data);
				}
			}
			glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活对应纹理单元
			glBindTexture(GL_TEXTURE_2D, diffuseMap->id);
		}
		else {
			glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活对应纹理单元
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glDisable(GL_DEPTH_TEST);
		mesh->Draw();
	}

	BeltStripMat::~BeltStripMat() {
		glDeleteTextures(1, &diffuseMap->id);
	}
}