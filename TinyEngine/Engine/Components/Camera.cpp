#pragma once
#include "Camera.h"
#include "Engine/DataDef.h"
#include "MeshFilter.h"
#include "Engine/Global.h"
#include "Render.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Transform.h"
#include "Engine/Object.h"

namespace TEngine {
	using namespace glm;

	Camera* Camera::main = nullptr;
	std::map<unsigned int, Camera*> Camera::cameras;

	Camera::Camera() : Component() {}

	void Camera::Start() {
		cameras.insert(std::pair<unsigned int, Camera*>(obj->GetInstanceID(), this));
	}

	void Camera::RenderAll(glContext* esContext) {
		for (auto& cam : cameras) {
			cam.second->Rend(esContext);
		}
	}

	static void CameraSetGLViewPort(GLuint w, GLuint h) {
		static GLuint width_, height_;
		if (width_ != w || height_ != h) {
			width_ = w; height_ = h;
			glViewport(0, 0, w, h);
		}
	}

	void Camera::Rend(glContext* esContext) {
		CameraSetGLViewPort(width, height);
		std::unordered_map<unsigned int, Object*>* objs = ((UserData*)esContext->userData)->allObjects;
		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget);   //渲染到默认帧缓冲
		//glBindFramebuffer(GL_FRAMEBUFFER, Global::twoImageFramebuffer);   //渲染到预设屏幕帧缓冲
		//GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		//glDrawBuffers(2, attachments);
		for (auto it = objs->begin(); it != objs->end(); it++) {
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
		return glm::lookAt(obj->Trans()->Positon(), obj->Trans()->Positon() + obj->Trans()->Forwward(), obj->Trans()->Up());
	}

	glm::mat4 Camera::GetProjectionMatrix() {
		//return  glm::ortho(-2.0f, 2.0f, -1.8f, 1.8f, nearPlan, farPlan);
		return glm::perspective(glm::radians(Zoom), width / (float)height, nearPlan, farPlan);
	}

	glm::vec3 Camera::ScreenToWorldPoint(const glm::vec3& screenPoint)
	{
		GLint viewport[4];
		GLfloat  winY, winZ;
		glGetIntegerv(GL_VIEWPORT, viewport);
		winY = (float)viewport[3] - (float)screenPoint.y - 1.0f;
		glReadBuffer(GL_BACK);
		glReadPixels((GLint)screenPoint.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

		mat4 matProjection = GetViewMatrix() * GetProjectionMatrix();
		mat4 matInverse = inverse(matProjection);
		float in[4];
		in[0] = (2.0f * ((float)(screenPoint.x - 0) / (width - 0))) - 1.0f;
		in[1] = 1.0f - (2.0f * ((float)(screenPoint.y - 0) / (height - 0)));
		in[2] = 2.0f * winZ - 1.0f;
		in[3] = 1.0f;
		vec4 vIn = vec4(in[0], in[1], in[2], in[3]);
		vec4 pos = matInverse * vIn;
		pos.w = 1.0 / pos.w;

		pos.x *= pos.w;
		pos.y *= pos.w;
		pos.z *= pos.w;
		return glm::vec3(pos.x, pos.y, pos.z);
	}

	Camera::~Camera() {
		cameras.erase(obj->GetInstanceID());
		DEBUGLOG("~Camera");
	}
}