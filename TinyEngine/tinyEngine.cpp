#pragma once
#include "Engine/Object.h"
#include "Engine/DataDef.h"
#include "Engine/Global.h"
#include "Engine/Input.h"
#include "Engine/Components/Camera.h"
#include "Scripts/CameraCtr.h"
#include <Engine/Components/Script.h>
#include "Engine/UI/UI.h"

namespace TEngine {
	int Init(glContext* glContext) {
		Object::context = glContext;
		((UserData*)glContext->userData)->allObjects = new std::unordered_map<unsigned int, Object*>();

		//ShowCursor(FALSE);
		Input::Initial(glContext);
		Global::Initial(glContext);

		return TRUE;
	}

	void Draw(glContext* esContext) {
		Camera::RenderAll(esContext); //绘制场景
		UI::DrawAll();                //绘制UI
	}

	void Shutdown(glContext* esContext) {
		Global::Exit();
	}

	void Update(glContext* esContext, double deltaTime) {
		Input::Update(deltaTime);
		Global::Update(deltaTime);
		for (auto& it : *((UserData*)esContext->userData)->allObjects) {
			auto components = it.second->Components();
			for (auto& cp : components) {
				Script* script = dynamic_cast<Script*>(cp.second);
				if (script) {
					script->Update();
				}
			}
		}
	}

	void ResizeWindowCallback(GLContext* context, int width, int height) {
		if (Camera::main)
		{
			context->width = Camera::main->width = width;
			context->height = Camera::main->height = height;
		}
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