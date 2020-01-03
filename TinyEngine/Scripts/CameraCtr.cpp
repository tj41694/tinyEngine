#include "CameraCtr.h"
#include "Engine/Object.h"
#include "Engine/Components/Camera.h"
#include "Engine/Input.h"
#include "Tools/Debug.h"
#include <cmath>

namespace TEngine {
	using namespace std;
	CameraCtr::CameraCtr() :Script() {
	}

	void CameraCtr::Start() {
		camera_ = obj->GetComponent<Camera>();
		obj->Trans()->MoveTo(0, 15, 0);
	}

	void CameraCtr::Update() {
		static double moveSpeed = 5;
		if (Input::GetKey('W'))
			obj->Trans()->Move(obj->Trans()->Forwward() * (float)(Input::deltaTime * moveSpeed));
		if (Input::GetKey('S'))
			obj->Trans()->Move(-obj->Trans()->Forwward() * (float)(Input::deltaTime * moveSpeed));
		if (Input::GetKey('A'))
			obj->Trans()->Move(obj->Trans()->Right() * (float)(Input::deltaTime * moveSpeed));
		if (Input::GetKey('D'))
			obj->Trans()->Move(-obj->Trans()->Right() * (float)(Input::deltaTime * moveSpeed));

		if (Input::GetKey(GLFW_KEY_F1))
			obj->Trans()->MoveTo(0, 0, 0);

		//if (Input::GetKeyDown(GLFW_KEY_F2)) {
		//	ReflectProb* refectProb = spheremirror->GetComponent<ReflectProb>();
		//	if (!refectProb)
		//		spheremirror->AddComponent<ReflectProb>();
		//	else
		//		spheremirror->DestroyComponent<ReflectProb>();
		//}
		if (Input::GetMouseButton(1) == GLFW_PRESS) {
			obj->Trans()->Rotate(vec3(0, 1, 0), (float)(Input::GetAxisX() * Input::deltaTime / 2));
			obj->Trans()->Rotate(obj->Trans()->Right(), -(float)(Input::GetAxisY() * Input::deltaTime / 2));
		}
		if (Input::GetMouseButtonUp(0))
		{
			//DEBUGLOG(Input::mousePosition);
			vec3 worldPos = Camera::main->ScreenToWorldPoint(vec2(Input::mousePosition.x, Input::mousePosition.y));
			DEBUGLOG(worldPos);
		}
	}

	CameraCtr::~CameraCtr() {}
}