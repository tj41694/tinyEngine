#pragma once
#include "Object.h"
#include "Camera.h"
#include "DataDef.h"
#include "MeshFilter.h"
#include "Global.h"
#include "Render.h"
#include "glm/gtc/matrix_transform.hpp"

namespace TEngine {
	using namespace glm;

	Camera* Camera::main = nullptr;
	std::map<unsigned int, Camera*> Camera::cameras;

	Camera::Camera(Object* obj_) : Component(obj_) {
		cameras.insert(std::pair<unsigned int, Camera*>(obj_->GetInstanceID(), this));
		Zoom = 45.0f;
		exposure = 1.0f;
		renderTarget = 0;
		nearPlan = 1.0f;
		farPlan = 1000000.0f;
		width = 512;
		height = 512;
	}

	void Camera::RenderAll(glContext* esContext) {
		for (auto& cam : cameras) {
			cam.second->Rend(esContext);
		}
	}

	void Camera::Rend(glContext* esContext) {
		UserData* userData = (UserData*)esContext->userData;
		static GLuint width_, height_;
		if (width_ != width && height_ != height) {
			glViewport(0, 0, width, height);
			width_ = width; height_ = height;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget);   //渲染到默认帧缓冲
		//glBindFramebuffer(GL_FRAMEBUFFER, Global::twoImageFramebuffer);   //渲染到预设屏幕帧缓冲
		//GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		//glDrawBuffers(2, attachments);
		for (auto it = userData->objs->begin(); it != userData->objs->end(); it++) {
			MeshFilter* meshFilter = it._Ptr->_Myval.second->GetComponent<MeshFilter>();
			if (meshFilter != nullptr) {
				meshFilter->DrawMeshes(this);
			}
		}

		//glDisable(GL_DEPTH_TEST);

		//GLboolean horizontal = true, first_iteration = true;
		//GLuint amount = 10;
		//glUseProgram(Global::shaderBlur);
		//for (GLuint i = 0; i < amount; i++) {
		//	glBindFramebuffer(GL_FRAMEBUFFER, Global::pingpongFBO[horizontal]);
		//	glUniform1i(glGetUniformLocation(Global::shaderBlur, "horizontal"), horizontal);
		//	glUniform1i(glGetUniformLocation(Global::shaderBlur, "image"), 0);
		//	glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活对应纹理单元
		//	glBindTexture(GL_TEXTURE_2D, first_iteration ? Global::texColorBuffer[1] : Global::pingpongBuffer[!horizontal]);
		//	glBindVertexArray(Global::quadVAO);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//	horizontal = !horizontal;
		//	if (first_iteration)
		//		first_iteration = false;
		//}

		//glBindFramebuffer(GL_FRAMEBUFFER, renderTarget); // 返回默认
		//glUseProgram(Global::screenShader);
		//GLuint screenTextureID = glGetUniformLocation(Global::screenShader, "screenTexture");
		//glUniform1i(screenTextureID, 0);
		//GLuint blurTextureID = glGetUniformLocation(Global::screenShader, "blurTexture");
		//glUniform1i(blurTextureID, 1);
		//glDrawBuffers(1, attachments);
		//glClear(GL_COLOR_BUFFER_BIT);
		//glUniform1f(glGetUniformLocation(Global::screenShader, "exposure"), exposure);
		//glBindVertexArray(Global::quadVAO);
		//glUniform1i(glGetUniformLocation(Global::screenShader, "screenTexture"), 0);
		//glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活对应纹理单元
		//glBindTexture(GL_TEXTURE_2D, Global::texColorBuffer[0]);
		//glUniform1i(glGetUniformLocation(Global::screenShader, "blurTexture"), 1);
		//glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活对应纹理单元
		//glBindTexture(GL_TEXTURE_2D, Global::pingpongBuffer[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glm::mat4 Camera::GetViewMatrix() {
		return glm::lookAt(obj->WorldPos(), obj->WorldPos() + obj->Forwward(), obj->Up());
	}

	glm::mat4 Camera::GetProjectionMatrix() {
		//return  glm::ortho(-2.0f, 2.0f, -1.8f, 1.8f, nearPlan, farPlan);
		return glm::perspective(glm::radians(Zoom), width / (float)height, nearPlan, farPlan);
	}

	Camera::~Camera() {
		cameras.erase(obj->GetInstanceID());
		Debug::Log("~Camera\n");
	}
}