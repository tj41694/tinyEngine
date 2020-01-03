#include "ReflectProb.h"
#include "Engine/Object.h"
#include "Camera.h"
#include "Engine/Materials/Standard.h"
#include "Render.h"
#include "Engine/Global.h"
#include <iostream>

namespace TEngine {
	ReflectProb::ReflectProb() : Component() {
		texSize = 1024;
		/*创建一个天空盒，并将其6个面的纹理附加到6个帧缓冲*/
		glGenTextures(1, &cubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap); //绑定对象到cubeMap
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //配置cubeMap属性
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
		for (GLuint i = 0; i < 6; i++)    //对cubeMap每一个面分配内存
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, texSize, texSize, 0, GL_RGB, GL_FLOAT, NULL);


		glGenFramebuffers(6, framebuffer); //创建6个帧缓冲
		for (int i = 0; i < 6; i++) {
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[i]); //绑定帧缓冲

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMap, 0); //附加
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			unsigned int rbo;            //创建渲染缓冲对象并附加在帧缓冲对象
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, texSize, texSize);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);   //附加

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void ReflectProb::Start() {

		float fov = 90.0f;

		Object* camObj0 = new Object("right");
		camObj0->Trans()->SetParent(obj->Trans(), false);
		camObj0->Trans()->RotateTo(0, -glm::pi<float>() / 2, glm::pi<float>());
		cameras[0] = camObj0->AddComponent<Camera>();
		cameras[0]->renderTarget = framebuffer[0];
		cameras[0]->Zoom = fov;
		cameras[0]->width = texSize;
		cameras[0]->height = texSize;

		Object* camObj1 = new Object("left");
		camObj1->Trans()->SetParent(obj->Trans(), false);
		camObj1->Trans()->RotateTo(0, glm::pi<float>() / 2, glm::pi<float>());
		cameras[1] = camObj1->AddComponent<Camera>();
		cameras[1]->renderTarget = framebuffer[1];
		cameras[1]->Zoom = fov;
		cameras[1]->width = texSize;
		cameras[1]->height = texSize;

		Object* camObj2 = new Object("top");
		camObj2->Trans()->RotateTo(glm::pi<float>() / 2, 0, glm::pi<float>());
		camObj2->Trans()->SetParent(obj->Trans(), false);
		cameras[2] = camObj2->AddComponent<Camera>();
		cameras[2]->renderTarget = framebuffer[2];
		cameras[2]->Zoom = fov;
		cameras[2]->width = texSize;
		cameras[2]->height = texSize;

		Object* camObj3 = new Object("bottom");
		camObj3->Trans()->RotateTo(-glm::pi<float>() / 2, 0, glm::pi<float>());
		camObj3->Trans()->SetParent(obj->Trans(), false);
		cameras[3] = camObj3->AddComponent<Camera>();
		cameras[3]->renderTarget = framebuffer[3];
		cameras[3]->Zoom = fov;
		cameras[3]->width = texSize;
		cameras[3]->height = texSize;

		Object* camObj4 = new Object("front");
		camObj4->Trans()->SetParent(obj->Trans(), false);
		camObj4->Trans()->RotateTo(0, 0, glm::pi<float>());
		cameras[4] = camObj4->AddComponent<Camera>();
		cameras[4]->renderTarget = framebuffer[4];
		cameras[4]->Zoom = fov;
		cameras[4]->width = texSize;
		cameras[4]->height = texSize;

		Object* camObj5 = new Object("back");
		camObj5->Trans()->SetParent(obj->Trans(), false);
		camObj5->Trans()->RotateTo(0, glm::pi<float>(), glm::pi<float>());
		cameras[5] = camObj5->AddComponent<Camera>();
		cameras[5]->renderTarget = framebuffer[5];
		cameras[5]->Zoom = fov;
		cameras[5]->width = texSize;
		cameras[5]->height = texSize;

		DEBUGLOG("intial ReflectProb.. \n");

		Render* render = obj->GetComponent<Render>();
		for (unsigned int i = 0; i < render->materials.size(); i++) {
			Standard* mat = dynamic_cast<Standard*>(render->materials.at(i));
			if (mat) {
				mat->diffuseColor = vec3(0.0f);
				mat->specularColor = vec3(0.8f);
				mat->skybox = cubeMap;
			}
			else {
				DEBUGLOG("null mat \n");
			}
		}
	}

	ReflectProb::~ReflectProb() {
		glDeleteFramebuffers(6, framebuffer);
		glDeleteTextures(1, &cubeMap);
		for (int i = 0; i < 6; i++) {
			delete cameras[i]->obj;
		}
		Render* render = obj->GetComponent<Render>();
		for (unsigned int i = 0; i < render->materials.size(); i++) {
			Standard* mat = dynamic_cast<Standard*>(render->materials.at(i));
			if (mat) {
				mat->skybox = Global::skyboxTexture;
			}
		}
	}
}