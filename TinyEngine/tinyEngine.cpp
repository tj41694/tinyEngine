#pragma once
#include "Engine/Object.h"
#include "Engine/DataDef.h"
#include "Engine/Global.h"
#include "Engine/Input.h"
#include "Engine/Components/Camera.h"
#include "Scripts/CameraCtr.h"
#include "Engine/UI/Text.h"
#include "Engine/OpenglUtil.h"
#include "Engine/Components/Transform.h"
#include <Engine/Components/Script.h>

namespace TEngine {
	static Text* FPSText;
	static Text* FPSText1;
	int Init(glContext* glContext) {

		//ShowCursor(FALSE);

		Object::context = glContext;
		((UserData*)glContext->userData)->allObjects = new std::unordered_map<unsigned int, Object*>();
		Input::Initial(glContext);
		Global::Initial();
		//Text* text = new Text("你好GL。!>?#@_");
		//text->color = vec3(0.3, 0.3, 0.8);
		//text->Move(300, 500, 0);
		FPSText = new Text();
		FPSText->Trans()->LocalScale() = vec3(0.5f);
		FPSText->Trans()->Move(80, glContext->height - 30.0f, 0);
		FPSText->color = vec3(0.1, 0.9, 0.5);
		FPSText1 = new Text("FPS:");
		FPSText1->Trans()->LocalScale() = vec3(0.5f);
		FPSText1->Trans()->Move(10, glContext->height - 30.0f, 0);
		FPSText1->color = vec3(0.1, 0.9, 0.5);
		Object* camObj = new Object("Camera");  //创建摄像机物体（载体）
		Camera::main = camObj->AddComponent<Camera>(); //添加摄像机组件
		camObj->AddComponent<CameraCtr>();   //添加摄像机控制脚本
		Camera::main->width = glContext->width;
		Camera::main->height = glContext->height;

		return TRUE;
	}

	//
	// Draw a triangle using the shader pair created in Init()
	//
	void Draw(glContext* esContext) {
		Camera::RenderAll(esContext); //绘制场景
		UI::DrawAll();                //绘制UI
	}

	void Shutdown(glContext* esContext) {

	}

	void Update(glContext* esContext, double deltaTime) {
		static unsigned int ct = 0;
		Input::Update(deltaTime);
		if (deltaTime != 0.0 && ct++ % 10 == 0) {
			FPSText->SetText(1.0 / deltaTime);
		}
		UserData* userData = (UserData*)esContext->userData;
		for (auto it = userData->allObjects->begin(); it != userData->allObjects->end(); it++) {
			auto components = it->second->Components();
			for (auto& cp : components) {
				Script* script = dynamic_cast<Script*>(cp.second);
				if (script) {
					script->Update();
				}
			}
		}
	}

	void ResizeWindowCallback(GLContext* context, int width, int height) {
		context->width = Camera::main->width = width;
		context->height = Camera::main->height = height;
	}

#ifdef __GLES__
	extern "C" int esMain(ESContext * esContext) {
		esContext->userData = malloc(sizeof(UserData));
		esCreateWindow(esContext, "你好GL", 1912, -31, 1920, 1080, ES_WINDOW_RGB | ES_WINDOW_DEPTH | EGL_SAMPLE_BUFFERS);
		//ShowCursor(FALSE);
		if (!Init(esContext)) { return GL_FALSE; }
		esRegisterShutdownFunc(esContext, Shutdown);
		esRegisterUpdateFunc(esContext, Update);
		esRegisterDrawFunc(esContext, Draw);
		return GL_TRUE;
	}
#else

	extern "C" int glMain(GLContext * glContext) {
		glContext->userData = malloc(sizeof(UserData));

		if (!glCreateWindow(glContext, "Hello GL", 1920, 0, 768, 1024)) {
			return -1;
		}
		if (!Init(glContext)) { return GL_FALSE; }
		glRegisterShutdownFunc(glContext, Shutdown);
		glRegisterUpdateFunc(glContext, Update);
		glRegisterDrawFunc(glContext, Draw);
		glRegisterReSizeWindowFunc(glContext, ResizeWindowCallback);
		return GL_TRUE;
	}
#endif
}