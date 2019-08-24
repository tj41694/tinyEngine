#pragma once
#include "Object.h"
#include "esUtil.h"
#include "DataDef.h"
#include "Global.h"
#include "Input.h"
#include "Camera.h"
#include "CameraCtr.h"
#include "Text.h"

namespace TEngine {
	static Text* FPSText;
	static Text* FPSText1;
	int Init(ESContext* esContext)
	{
		Object::context = esContext;
		UserData* userData = (UserData*)esContext->userData;
		userData->objs = new std::unordered_map<unsigned int, Object*>();
		Global::Initial();
		//Text* text = new Text("你好GL。!>?#@_");
		//text->color = vec3(0.3, 0.3, 0.8);
		//text->Move(300, 500, 0);
		FPSText = new Text();
		FPSText->scale = vec3(0.5f);
		FPSText->Move(80, esContext->height - 30.0f, 0);
		FPSText->color = vec3(0.1, 0.9, 0.5);
		FPSText1 = new Text("FPS:");
		FPSText1->scale = vec3(0.5f);
		FPSText1->Move(10, esContext->height - 30.0f, 0);
		FPSText1->color = vec3(0.1, 0.9, 0.5);
		Object* camObj = new Object("Camera");  //创建摄像机物体（载体）
		Camera::main = camObj->AddComponent<Camera>(); //添加摄像机组件
		camObj->AddComponent<CameraCtr>();   //添加摄像机控制脚本
		Camera::main->width = esContext->width;
		Camera::main->height = esContext->height;
		glEnable(GL_DEPTH_TEST);   //深度缓冲测试
		glEnable(GL_CULL_FACE);  //面剔除
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //混合方式
		return TRUE;
	}

	//
	// Draw a triangle using the shader pair created in Init()
	//
	void Draw(ESContext* esContext)
	{
		Camera::RenderAll(esContext); //绘制场景
		UI::DrawAll();                //绘制UI
	}

	void Shutdown(ESContext* esContext)
	{

	}

	void Update(ESContext* esContext, float deltaTime)
	{
		Input::Update(esContext, deltaTime);
		static unsigned int times = 0;
		if (deltaTime != 0.0 && times++ % 60 == 0)
		{
			FPSText->SetText(1.0 / deltaTime);
		}
		UserData* userData = (UserData*)esContext->userData;
		for (auto it = userData->objs->begin(); it != userData->objs->end(); it++)
		{
			Script* script = it._Ptr->_Myval.second->GetComponent<Script>();
			if (script)
			{
				if (script->notStarted)
				{
					script->notStarted = false;
					script->Start();
				}
				script->Update();
			}
		}
	}

	extern "C" int esMain(ESContext* esContext)
	{
		esContext->userData = malloc(sizeof(UserData));
		esCreateWindow(esContext, "你好GL", 0, 0, 1080, 768, ES_WINDOW_RGB | ES_WINDOW_DEPTH | EGL_SAMPLE_BUFFERS);
		//ShowCursor(FALSE);
		if (!Init(esContext)) { return GL_FALSE; }
		esRegisterShutdownFunc(esContext, Shutdown);
		esRegisterUpdateFunc(esContext, Update);
		esRegisterDrawFunc(esContext, Draw);
		return GL_TRUE;
	}
}