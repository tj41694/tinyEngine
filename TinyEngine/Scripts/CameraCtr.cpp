#include "CameraCtr.h"
#include "Engine/Object.h"
#include "Engine/Components/Camera.h"
#include "Engine/Input.h"
#include "Tools/Debug.h"
#include <cmath>

namespace TEngine {
	using namespace std;
	using namespace glm;
	CameraCtr::CameraCtr() :Script() {
	}

	void CameraCtr::Start() {
		camera_ = obj->GetComponent<Camera>();
		obj->Trans()->MoveTo(1135.362, 3, -333.93);
	}

	void CameraCtr::Update() {
		static double moveSpeed = 1;
		if (Input::GetKey('W'))
			obj->Trans()->Move(obj->Trans()->Forwward() * (float)(Input::deltaTime * moveSpeed));
		if (Input::GetKey('S'))
			obj->Trans()->Move(-obj->Trans()->Forwward() * (float)(Input::deltaTime * moveSpeed));
		if (Input::GetKey('A'))
		{
			obj->Trans()->Move(-obj->Trans()->Right() * (float)(Input::deltaTime * moveSpeed));
			//DEBUGLOG(obj->Trans()->Positon());
		}
		if (Input::GetKey('D'))
		{
			obj->Trans()->Move(obj->Trans()->Right() * (float)(Input::deltaTime * moveSpeed));
			//DEBUGLOG(obj->Trans()->Positon());
		}

		//if (Input::GetKey(GLFW_KEY_F1))
		//	obj->Trans()->MoveTo(0, 0, 0);

		if (Input::GetKey(GLFW_KEY_F2))
		{
			camera_->rendTextIndex == 0 ? camera_->rendTextIndex = 1 : camera_->rendTextIndex = 0;
		}

		if (Input::GetKey(GLFW_KEY_F3))
		{
			camera_->drawFrame = !camera_->drawFrame;
		}

		//if (Input::GetKeyDown(GLFW_KEY_F2)) {
		//	ReflectProb* refectProb = spheremirror->GetComponent<ReflectProb>();
		//	if (!refectProb)
		//		spheremirror->AddComponent<ReflectProb>();
		//	else
		//		spheremirror->DestroyComponent<ReflectProb>();
		//}
		if (Input::GetMouseButton(1) == GLFW_PRESS) {
			obj->Trans()->Rotate(vec3(0, 1, 0), (float)(Input::GetAxisX() * Input::deltaTime / 2));
			obj->Trans()->Rotate(obj->Trans()->Right(), (float)(Input::GetAxisY() * Input::deltaTime / 2));
		}
		if (Input::GetMouseButtonUp(0))
		{
			static bool flag = false;
			flag = !flag;
			vec3 worldPos = Camera::main->ScreenToWorldPoint(Input::mousePosition, flag);
			DEBUGLOG(worldPos);
		}
	}

	CameraCtr::~CameraCtr() {}
}