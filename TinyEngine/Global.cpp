#pragma once
#include "Object.h"
#include "Global.h"
#include "FileSystem.h"
#include "Render.h"
#include "MatSkyBox.h"

namespace TEngine {
	using namespace std;

	unsigned int Global::skyboxTexture = 0;
	unsigned int Global::twoImageFramebuffer = 0;
	unsigned int Global::screenShader = 0;
	unsigned int Global::shaderBlur = 0;
	unsigned int Global::quadVAO = 0;
	unsigned int Global::texColorBuffer[2] = { 0,0 };
	unsigned int Global::pingpongFBO[2] = { 0,0 };
	unsigned int Global::pingpongBuffer[2] = { 0,0 };

	unsigned int Global::loadCubemap(vector<std::string> faces) {
		glGenTextures(1, &skyboxTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++) {
			unsigned char *data = FileSystem::LoadTexture(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				FileSystem::FreeTextur(data);
			} else {
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				FileSystem::FreeTextur(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return skyboxTexture;
	}

	void Global::Initial() {
#pragma region skybox
		vector<std::string> space
		{
			"skybox/spaceblue/right.png",
			"skybox/spaceblue/left.png",
			"skybox/spaceblue/top.png",
			"skybox/spaceblue/bottom.png",
			"skybox/spaceblue/front.png",
			"skybox/spaceblue/back.png"
		};
		vector<std::string> lake
		{
			"skybox/lake/right.jpg",
			"skybox/lake/left.jpg",
			"skybox/lake/top.jpg",
			"skybox/lake/bottom.jpg",
			"skybox/lake/front.jpg",
			"skybox/lake/back.jpg"
		};
		vector<std::string> gray
		{
			"skybox/gray/right.jpg",
			"skybox/gray/left.jpg",
			"skybox/gray/top.jpg",
			"skybox/gray/bottom.jpg",
			"skybox/gray/front.jpg",
			"skybox/gray/back.jpg"
		};
		loadCubemap(gray);
		Object* obj = Object::CreateShape(Shape::cube, 1.0f, true);
		Render* render = obj->GetComponent<Render>();
		Material* skyboxMat = new MatSkyBox();
		render->materials.push_back(skyboxMat);
#pragma endregion

#pragma region QuadVAO
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		//positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};

		unsigned int quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);

#pragma endregion

#pragma region FrameBuffer
		//screenShader = esLoadProgram(FileSystem::ReadFile("./resources/shaders/screenShader/screenShader.vs").c_str(),
		//	FileSystem::ReadFile("./resources/shaders/screenShader/screenShader.fs").c_str());
		//shaderBlur = esLoadProgram(FileSystem::ReadFile("./resources/shaders/shaderBlur/shaderBlur.vs").c_str(),
		//	FileSystem::ReadFile("./resources/shaders/shaderBlur/shaderBlur.fs").c_str());

		//glGenFramebuffers(1, &twoImageFramebuffer);
		//glBindFramebuffer(GL_FRAMEBUFFER, twoImageFramebuffer);

		//glGenTextures(2, texColorBuffer);    //创建纹理并附加到帧缓冲对象
		//glBindTexture(GL_TEXTURE_2D, texColorBuffer[0]);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Object::context->width, Object::context->height, 0, GL_RGB, GL_FLOAT, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer[0], 0); //附加
		//glBindTexture(GL_TEXTURE_2D, texColorBuffer[1]);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Object::context->width, Object::context->height, 0, GL_RGB, GL_FLOAT, NULL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, texColorBuffer[1], 0); //附加

		//unsigned int rbo;            //创建渲染缓冲对象并附加在帧缓冲对象
		//glGenRenderbuffers(1, &rbo);
		//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Object::context->width, Object::context->height);
		//glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);   //附加

		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		//
		//glGenFramebuffers(2, pingpongFBO);  //再创建两个帧缓冲用于高斯模糊
		//glGenTextures(2, pingpongBuffer);
		//for (GLuint i = 0; i < 2; i++) {
		//	glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		//	glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Object::context->width, Object::context->height, 0, GL_RGB, GL_FLOAT, NULL);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);
		//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		//}
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion
	}

}