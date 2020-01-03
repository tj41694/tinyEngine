#pragma once
#include "Object.h"
#include "Global.h"
#include "FileSystem.h"
#include "Engine/Components/Render.h"
#include "Engine/Materials/MatSkyBox.h"
#include "External/MyTif.h"
#include <Tools/Terrain.h>
#include "Materials/Standard.h"
#include "Texture.h"
#include <Scripts/BeltStrip.h>
#include <Scripts/SelfRot.h>
#include "Engine/UI/Text.h"
#include "Components/Camera.h"
#include <glUtil.h>
#include <Scripts/CameraCtr.h>

namespace TEngine {
	using namespace std;

	unsigned int Global::skyboxTexture = 0;
	unsigned int Global::twoImageFramebuffer = 0;
	unsigned int Global::screenShader = 0;
	unsigned int Global::shaderBlur = 0;
	unsigned int Global::screenVAO = 0;
	unsigned int Global::texColorBuffer[2] = { 0,0 };
	unsigned int Global::pingpongFBO[2] = { 0,0 };
	unsigned int Global::pingpongBuffer[2] = { 0,0 };
	Text* Global::FPSText = nullptr;
	Text* Global::FPSText1 = nullptr;

	void Global::Initial(glContext* glContext) {
		InitialSkybox();
		InitialScence(glContext);
		InitialUI();
#pragma region screenVAO
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
		glGenVertexArrays(1, &screenVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(screenVAO);
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

	void Global::Update(double deltaTime) {
		static unsigned int ct = 0;
		if (deltaTime != 0.0 && ct++ % 10 == 0) {
			FPSText->SetText(1.0 / deltaTime);
		}
	}

	void Global::InitialSkybox()
	{
		//vector<std::string> space {
		//	"skybox/spaceblue/right.png",
		//	"skybox/spaceblue/left.png",
		//	"skybox/spaceblue/top.png",
		//	"skybox/spaceblue/bottom.png",
		//	"skybox/spaceblue/front.png",
		//	"skybox/spaceblue/back.png"
		//};
		//vector<std::string> lake
		//{
		//	"skybox/lake/right.jpg",
		//	"skybox/lake/left.jpg",
		//	"skybox/lake/top.jpg",
		//	"skybox/lake/bottom.jpg",
		//	"skybox/lake/front.jpg",
		//	"skybox/lake/back.jpg"
		//};
		//vector<std::string> gray
		//{
		//	"skybox/gray/right.jpg",
		//	"skybox/gray/left.jpg",
		//	"skybox/gray/top.jpg",
		//	"skybox/gray/bottom.jpg",
		//	"skybox/gray/front.jpg",
		//	"skybox/gray/back.jpg"
		//};
		vector<std::string> spaceGray
		{
			"skybox/spaceGray/right.png",
			"skybox/spaceGray/left.png",
			"skybox/spaceGray/top.png",
			"skybox/spaceGray/bottom.png",
			"skybox/spaceGray/front.png",
			"skybox/spaceGray/back.png"
		};

		glGenTextures(1, &skyboxTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		int width, height, nrChannels;
		for (unsigned int i = 0; i < spaceGray.size(); i++) {
			unsigned char* data = FileSystem::LoadTexture(spaceGray[i].c_str(), &width, &height, &nrChannels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				FileSystem::FreeTextur(data);
			}
			else {
				std::cout << "Cubemap texture failed to load at path: " << spaceGray[i] << std::endl;
				FileSystem::FreeTextur(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		Object* obj = Object::CreateShape(Shape::cube, 1.0f, true);
		Render* render = obj->GetComponent<Render>();
		Material* skyboxMat = new MatSkyBox();
		render->materials.push_back(skyboxMat);
	}

	void Global::InitialScence(glContext* glContext)
	{
		Object* camObj = new Object("Camera");  //创建摄像机物体（载体）
		Camera::main = camObj->AddComponent<Camera>(); //添加摄像机组件
		Camera::main->width = glContext->width;
		Camera::main->height = glContext->height;
		camObj->AddComponent<CameraCtr>();   //添加摄像机控制脚本
		if (0) {
			//地面
			Standard* matGround = new Standard();
			matGround->diffuseMap = new Texture();
			matGround->diffuseMap->path.assign("textures\\terrian.jpg");
			matGround->diffuseColor = vec3(0.8f);
			Object* ground = Object::CreateShape(Shape::plane, 100);
			ground->GetComponent<Render>()->materials.push_back(matGround);
		}
		else {
			//地形
			GenTerrain();
		}
		BeltStrip* strip = new BeltStrip();
		float x = 10.0f;
		float z = 15.0f;
		vector<glm::vec3> polygon2
		{
			vec3(-x, 0, -z),
			vec3(0, 0, -1.5 * z),
			vec3(x, 0, -z),
			vec3(1.5 * x, 0, 0),
			vec3(x, 0, z),
			vec3(0, 0, 1.5 * z),
			vec3(-x, 0, z),
			vec3(-x, 0, -z)
		};
		strip->DrawBeltStrip_Mesh(polygon2, 0.2f, vec2(100, 360));
		strip->obj->Trans()->Move(150, 4, 16);


#pragma region 画各种物体

		//Object* cube1 = Object::CreateShape(Shape::sphere, 600);
		//cube1->MoveTo(1000, 600.0f, 2500.0f);
		//cube1->Rotate(vec3(20, -10, 50));
		//cube1->AddComponent<SelfRot>()->speed = 0.1f;
		////cube1->SetParent(sphere, false);
		//Standard* mat2 = new Standard();
		//mat2->diffuseMap = new Texture();
		//mat2->specularColor = vec3(0.5);
		//mat2->diffuseMap->path.assign("textures\\www.jpg");
		//auto render2 = cube1->GetComponent<Render>();
		//render2->materials.push_back(mat2);

		//Object* sphere = Object::CreateShape(Shape::sphere);
		////sphere->scale = vec3(4.0f);
		//sphere->AddComponent<SelfRot>();
		//sphere->MoveTo(-30, 0, 0);
		//auto render = sphere->GetComponent<Render>();
		//Standard* mat = new Standard();
		//mat->selfLuminous = vec3(1.0f, 0.889f, 0.675f);
		//mat->selfLuminous *= 1.2f;
		//render->materials.push_back(mat);

		//cube1->GetComponent<MeshFilter>()->meshes[0]->RecalculateNormals();

		//Object* cube2 = Object::CreateShape(Shape::cube);
		//cube2->MoveTo(2, -3, 5);
		//cube1->Rotate(vec3(45, -45, 50));
		//cube2->AddComponent<SelfRot>();
		//cube2->SetParent(sphere, false);
		//auto render3 = cube2->GetComponent<Render>();
		//render3->materials.push_back(mat2);

		//Object* cube3 = Object::CreateShape(Shape::cube);
		//cube3->AddComponent<SelfRot>();
		//cube3->MoveTo(-5.0f, 0, 0);
		//cube1->Rotate(vec3(50, -10, 0));
		//cube3->SetParent(sphere, false);
		//auto render4 = cube3->GetComponent<Render>();
		//render4->materials.push_back(mat2);

		Object* nanosuit = Object::LoadModel("models/bakeTest/", "bakeTest.obj");
		nanosuit->AddComponent<SelfRot>()->speed = 0.2f;
		nanosuit->Trans()->MoveTo(glm::vec3(5, 1, 5));
		nanosuit->Trans()->SetLocalScale(glm::vec3(0.1, 0.1, 0.1));

		//Object* board1 = Object::LoadModel("./resources/models/board/", "board.obj");
		//board1->MoveTo(glm::vec3(-15.0f, 18.0f, 0.0f));
		//board1->SetParent(nanosuit, false);
		//board1->AddComponent<BoardCtr>();

		//Object* as350 = Object::LoadModel("./resources/models/as350/", "AS350.obj");
		//as350->AddComponent<SelfRot>()->speed = 2.0f;
		//as350->MoveTo(-15, 0, 0);
		//as350->SetParent(nanosuit);

		//Object* board2 = Object::LoadModel("./resources/models/board/", "board.obj");
		//board2->MoveTo(glm::vec3(10.0f, 3.0f, 0.0f));
		//board2->SetParent(nanosuit, false);
		//board2->AddComponent<BoardCtr>();

		//Object* board3 = Object::LoadModel("./resources/models/board/", "board.obj");
		//board3->MoveTo(glm::vec3(0.0f, 3.0f, 15.0f));
		//board3->AddComponent<BoardCtr>();

		//Object* board4 = Object::LoadModel("./resources/models/board/", "board.obj");
		//board4->MoveTo(glm::vec3(-6.0f, 10.0f, 0.0));
		//board4->AddComponent<BoardCtr>();
		//Object* board5 = Object::LoadModel("./resources/models/board/", "board.obj");
		//board5->MoveTo(glm::vec3(-6.0f, 10.0f, 8.0));
		//board5->AddComponent<BoardCtr>();
		//Object* board6 = Object::LoadModel("./resources/models/board/", "board.obj");
		//board6->MoveTo(glm::vec3(6.0f, 10.0f, 0.0));
		//board6->AddComponent<BoardCtr>();
		//Object* board7 = Object::LoadModel("./resources/models/board/", "board.obj");
		//board7->MoveTo(glm::vec3(6.0f, 10.0f, 8.0));
		//board7->AddComponent<BoardCtr>();

		////创建反射球
		//Standard* mirrormat = new Standard();
		//mirrormat->specularColor = vec3(0.9f);
		//spheremirror = Object::CreateShape(Shape::sphere, 1.0f);
		//auto mirrorrender = spheremirror->GetComponent<Render>();
		//mirrorrender->materials.push_back(mirrormat);
		//spheremirror->MoveTo(0, 1, 0);
#pragma endregion 
	}

	void Global::InitialUI()
	{
		//Text* text = new Text("你好GL。!>?#@_");
		//text->color = vec3(0.3, 0.3, 0.8);
		//text->Move(300, 500, 0);
		FPSText = new Text();
		FPSText->Trans()->SetLocalScale(vec3(0.5f));
		FPSText->Trans()->Move(80, Camera::main->height - 30.0f, 0);
		FPSText->color = vec3(0.1, 0.9, 0.5);
		FPSText1 = new Text("FPS:");
		FPSText1->Trans()->SetLocalScale(vec3(0.5f));
		FPSText1->Trans()->Move(10, Camera::main->height - 30.0f, 0);
		FPSText1->color = vec3(0.1, 0.9, 0.5);
	}

	void Global::GenTerrain()
	{
		printf("加载地形\n");
		MyTif myTif("resources\\textures\\out2.tif");
		Object* terrain = Terrain::Create(myTif);
		Standard* matTerrain = new Standard();
		matTerrain->diffuseMap = new Texture();
		matTerrain->diffuseMap->path.assign("textures\\terrian.jpg");
		matTerrain->diffuseColor = vec3(0.8f);
		terrain->GetComponent<Render>()->materials.push_back(matTerrain);
		terrain->Trans()->MoveTo(-200, 0, -50);
	}

}