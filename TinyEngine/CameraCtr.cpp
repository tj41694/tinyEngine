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

CameraCtr::CameraCtr(Object* obj) :Script(obj) {
	camera_ = nullptr;
	spheremirror = nullptr;
}

void CameraCtr::Start() {
	camera_ = obj->GetComponent<Camera>();
	obj->MoveTo(0, 800, 0);

	MyTif myTif("resources\\textures\\out2.tif");
	Object* terrain = Terrain::Create(myTif);
	Standard* matTerrain = new Standard();
	matTerrain->diffuseMap = new Texture();
	matTerrain->diffuseMap->path.assign("textures\\www.jpg");
	matTerrain->diffuseColor = vec3(0.8f);
	terrain->GetComponent<Render>()->materials.push_back(matTerrain);
	terrain->MoveTo(-15000.0f, 0, -1000.0f);
#pragma region old
	//Object* terrain = Object::LoadModel("./resources/models/terrain/", "terrain.obj");
	//terrain->scale = vec3(0.05f);
	//terrain->MoveTo(0, -20, 100);

	//Object* sphere = Object::CreateShape(Shape::sphere);
	////sphere->scale = vec3(4.0f);
	//sphere->AddComponent<SelfRot>();
	//sphere->MoveTo(-30, 0, 0);
	//auto render = sphere->GetComponent<Render>();
	//Standard* mat = new Standard();
	//mat->selfLuminous = vec3(1.0f, 0.889f, 0.675f);
	//mat->selfLuminous *= 1.2f;
	//render->materials.push_back(mat);

	Object* cube1 = Object::CreateShape(Shape::sphere, 600);
	cube1->MoveTo(1000, 600.0f, 2500.0f);
	cube1->Rotate(vec3(20, -10, 50));
	cube1->AddComponent<SelfRot>()->speed = 0.1f;
	//cube1->SetParent(sphere, false);
	Standard* mat2 = new Standard();
	mat2->diffuseMap = new Texture();
	mat2->diffuseMap->path.assign("textures\\www.jpg");
	auto render2 = cube1->GetComponent<Render>();
	render2->materials.push_back(mat2);
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

	//Object* nanosuit = Object::LoadModel("./resources/models/nanosuit/", "nanosuit.obj");
	//nanosuit->AddComponent<SelfRot>()->speed = 0.5f;
	//nanosuit->MoveTo(glm::vec3(15.0f, -8.0f, 0.0f));

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

	//创建地面
	//Standard* matGround = new Standard();
	//matGround->diffuseMap = new Texture();
	//matGround->diffuseMap->path.assign("textures\\container.jpg");
	//matGround->diffuseColor = vec3(0.8f);
	//Object* ground = Object::CreateShape(Shape::plane, 5000);
	//ground->GetComponent<Render>()->materials.push_back(matGround);
	////创建反射球
	//Standard* mirrormat = new Standard();
	//mirrormat->specularColor = vec3(0.9f);
	//spheremirror = Object::CreateShape(Shape::sphere, 1.0f);
	//auto mirrorrender = spheremirror->GetComponent<Render>();
	//mirrorrender->materials.push_back(mirrormat);
	//spheremirror->MoveTo(0, 1, 0);
#pragma endregion old
}

void CameraCtr::Update() {
	static double moveSpeed = 500;
	if (Input::GetKeyDown(VK_ESCAPE)) SendMessage(Object::context->eglNativeWindow, WM_CLOSE, 0, 0); //press ESC Exit Program
	if (Input::GetKey('W'))
		obj->Move(obj->Forwward() * (float)(Input::deltaTime * moveSpeed));
	if (Input::GetKey('S'))
		obj->Move(-obj->Forwward() * (float)(Input::deltaTime * moveSpeed));
	if (Input::GetKey('A'))
		obj->Move(obj->Right() * (float)(Input::deltaTime * moveSpeed));
	if (Input::GetKey('D'))
		obj->Move(-obj->Right() * (float)(Input::deltaTime * moveSpeed));

	if (Input::GetKey(VK_F1))
		obj->MoveTo(0, 0, 0);

	if (Input::GetKeyDown(VK_F2)) {
		ReflectProb* refectProb = spheremirror->GetComponent<ReflectProb>();
		if (!refectProb)
			spheremirror->AddComponent<ReflectProb>();
		else
			spheremirror->DestroyComponent<ReflectProb>();
	}

	obj->Rotate(vec3(0, 1, 0), Input::GetAxisX() * 0.01f);
	obj->Rotate(obj->Right(), Input::GetAxisY() * -0.01f);
}

CameraCtr::~CameraCtr() {}