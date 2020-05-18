#include "CameraCtr.h"
#include "Engine/Object.h"
#include "Engine/Components/Camera.h"
#include "Engine/Input.h"
#include "Tools/Debug.h"
#include <cmath>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <Engine/Global.h>

namespace TEngine {
	using namespace std;
	using namespace glm;
	CameraCtr::CameraCtr() :Script() {
	}

	void CameraCtr::Start() {
		camera_ = obj->GetComponent<Camera>();
		obj->Trans()->MoveTo(1135.362f + 17.0f, 1.0f, -333.93f - 3.0f);
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

		if (Input::GetKeyUp(GLFW_KEY_F2))
		{
			camera_->rendTextIndex == 0 ? camera_->rendTextIndex = 1 : camera_->rendTextIndex = 0;
		}

		if (Input::GetKeyUp(GLFW_KEY_F3))
		{
			camera_->drawFrame = !camera_->drawFrame;
		}

		static float pixels[2000 * 1500];
		static unsigned char ipixels[2000 * 1500];
		static int index = 0;
		char savePath[64] = { 0 };
		if (Input::GetKeyUp(GLFW_KEY_F5))
		{
			sprintf(savePath, "lacationPics/image%d.jpg", index);
			glBindFramebuffer(GL_FRAMEBUFFER, Global::ScreenFramebuffer);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
			glReadPixels(0, 0, camera_->width, camera_->height, GL_RGB, GL_UNSIGNED_BYTE, ipixels);
			stbi_write_jpg(savePath, camera_->width, camera_->height, 3, (void*)ipixels, 99);

			sprintf(savePath, "lacationPics/lacationimage%d.hdr", index);
			glBindFramebuffer(GL_FRAMEBUFFER, Global::ScreenFramebuffer);
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			glReadPixels(0, 0, camera_->width, camera_->height, GL_RGB, GL_FLOAT, pixels);
			stbi_write_hdr(savePath, camera_->width, camera_->height, 3, pixels);
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
			static bool flag = true;
			//flag = !flag;
			vec3 worldPos = Camera::main->ScreenToWorldPoint(Input::mousePosition, flag);
			DEBUGLOG(worldPos);
			LOG(worldPos);
		}
	}

	CameraCtr::~CameraCtr() {}
}