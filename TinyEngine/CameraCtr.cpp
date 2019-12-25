#include "CameraCtr.h"
#include "Object.h"
#include "Camera.h"
#include "Input.h"
#include "Debug.h"
#include "SelfRot.h"
#include "Render.h"
#include "Standard.h"
#include "BoardCtr.h"
#include "FileSystem.h"
#include "ReflectProb.h"
#include <cmath>
#include "Mesh.h"
#include "Texture.h"
#include "MyTif.h"
#include "Terrain.h"
#include "MeshFilter.h"
#include "BeltStrip.h"
#include "Transform.h"

using namespace std;
CameraCtr::CameraCtr() :Script() {
	camera_ = nullptr;
	spheremirror = nullptr;
}

void CameraCtr::GenTerrain() {
	printf("加载地形\n");
	MyTif myTif("resources\\textures\\out2.tif");
	Object* terrain = Terrain::Create(myTif);
	Standard* matTerrain = new Standard();
	matTerrain->diffuseMap = new Texture();
	matTerrain->diffuseMap->path.assign("textures\\terrian.jpg");
	matTerrain->diffuseColor = vec3(0.8f);
	terrain->GetComponent<Render>()->materials.push_back(matTerrain);
	terrain->Trans()->MoveTo(-15000.0f, 0, -1000.0f);
}

void CameraCtr::Start() {
	camera_ = obj->GetComponent<Camera>();
	obj->Trans()->MoveTo(0, 500, 2000);
#pragma region 画各种物体
	if (0) {
		//地面
		Standard* matGround = new Standard();
		matGround->diffuseMap = new Texture();
		matGround->diffuseMap->path.assign("textures\\terrian.jpg");
		matGround->diffuseColor = vec3(0.8f);
		Object* ground = Object::CreateShape(Shape::plane, 20000);
		ground->GetComponent<Render>()->materials.push_back(matGround);
	}
	else {
		//地形
		GenTerrain();
	}
	//画带
	//BeltStrip* strip1 = new BeltStrip();
	//float x = 1000.0f;
	//float z = 1500.0f;
	//vector<glm::vec3> polygon;
	//{
	//	vec3(-x, 0, -z),
	//	vec3(x,  0, -z),
	//	vec3(x,  0,  z),
	//	vec3(-x, 0,  z)
	//};
	//MyTif myTif("resources\\textures\\out4.tif");
	//Terrain::GetSeqPoint(myTif, polygon);
	//strip1->Set(polygon, 2.5, vec2(2.5, 9));


	BeltStrip* strip2 = new BeltStrip();
	float x = 1000.0f;
	float z = 1500.0f;
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
	strip2->DrawBeltStrip_Mesh(polygon2, 10, vec2(100, 360));
	strip2->obj->Trans()->Move(0, 0, 6000);


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
	nanosuit->Trans()->MoveTo(glm::vec3(0, 80, 4000));
	nanosuit->Trans()->LocalScale() = glm::vec3(3, 3, 3);

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

void CameraCtr::Update() {
	static double moveSpeed = 500;
	if (Input::GetKey('W'))
		obj->Trans()->Move(obj->Trans()->Forwward() * (float)(Input::deltaTime * moveSpeed));
	if (Input::GetKey('S'))
		obj->Trans()->Move(-obj->Trans()->Forwward() * (float)(Input::deltaTime * moveSpeed));
	if (Input::GetKey('A'))
		obj->Trans()->Move(obj->Trans()->Right() * (float)(Input::deltaTime * moveSpeed));
	if (Input::GetKey('D'))
		obj->Trans()->Move(-obj->Trans()->Right() * (float)(Input::deltaTime * moveSpeed));

	//if (Input::GetKey(VK_F1))
	//	obj->MoveTo(0, 0, 0);

	//if (Input::GetKeyDown(VK_F2)) {
	//	ReflectProb* refectProb = spheremirror->GetComponent<ReflectProb>();
	//	if (!refectProb)
	//		spheremirror->AddComponent<ReflectProb>();
	//	else
	//		spheremirror->DestroyComponent<ReflectProb>();
	//}
	if (Input::GetMouseButton(1) == GLFW_PRESS ||
		Input::GetMouseButton(0) == GLFW_PRESS
		) {
		obj->Trans()->Rotate(vec3(0, 1, 0), (float)(Input::GetAxisX() * Input::deltaTime / 2));
		obj->Trans()->Rotate(obj->Trans()->Right(), -(float)(Input::GetAxisY() * Input::deltaTime / 2));
	}
}

CameraCtr::~CameraCtr() {}