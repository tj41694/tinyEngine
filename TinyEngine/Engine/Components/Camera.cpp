#pragma once
#include "Engine/Object.h"
#include "Camera.h"
#include "Engine/DataDef.h"
#include "DrawCmdFilter.h"
#include "Engine/Global.h"
#include "Render.h"
#include "glm/gtc/matrix_transform.hpp"

namespace TEngine {
	using namespace glm;

	Camera* Camera::main = nullptr;
	std::map<unsigned int, Camera*> Camera::cameras;

	Camera::Camera() : Component() {}

	void Camera::Start() {
		cameras.insert(std::pair<unsigned int, Camera*>(obj->GetInstanceID(), this));
		useScreenFrameBuffer = true;
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
		GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		if (useScreenFrameBuffer) {
			glBindFramebuffer(GL_FRAMEBUFFER, Global::ScreenFramebuffer); //绑定到预设屏幕帧缓冲
			glDrawBuffers(2, attachments); //显式告知OpenGL我们正在通过glDrawBuffers渲染到多个颜色缓冲，否则OpenGL只会渲染到帧缓冲的第一个颜色附件，而忽略所有其他的。
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, renderTarget);   //渲染到相机默认帧缓冲
		}
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);  //接下来绘制3D场景，需要保证开启深度测试
		drawFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (auto it = ((UserData*)esContext->userData)->allObjects->begin(); it != ((UserData*)esContext->userData)->allObjects->end(); it++) {
			DrawCmdFilter* filter = it._Ptr->_Myval.second->GetComponent<DrawCmdFilter>();
			if (filter != nullptr) {
				filter->DrawCmds(this);
			}
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (useScreenFrameBuffer) {
			glDisable(GL_DEPTH_TEST);
			glDrawBuffers(1, attachments);
			glBindFramebuffer(GL_FRAMEBUFFER, renderTarget); // 返回默认相机默认帧缓冲
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(Global::screenShader);
			glBindVertexArray(Global::screenVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Global::screenFBTexAttachs[rendTextIndex]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}

	glm::mat4 Camera::GetViewMatrix() {
		return glm::lookAt(obj->Trans()->Positon(), obj->Trans()->Positon() + obj->Trans()->Forwward(), obj->Trans()->Up());
	}

	static float _Zoom_ = 0;
	static float _nearPlan_ = 0;
	static float _farPlan_ = 0;
	static GLuint _width_ = 0;
	static GLuint _height_ = 0;
	glm::mat4& Camera::GetProjectionMatrix() {
		if (Zoom != _Zoom_ || nearPlan != _nearPlan_ || farPlan != _farPlan_ || width != _width_ || height != _height_) {
			_Zoom_ = Zoom; _nearPlan_ = nearPlan; _farPlan_ = farPlan; _width_ = width; _height_ = height;
			projectionMatrix = glm::perspective(glm::radians(Zoom), width / (float)height, nearPlan, farPlan);
		}
		return projectionMatrix;
		//return  glm::ortho(-2.0f, 2.0f, -1.8f, 1.8f, nearPlan, farPlan);
	}

	glm::vec3 Camera::ScreenToWorldPoint(const glm::vec2& screenPoint, bool readFromScreenBuffer)
	{
		glm::vec3 worldPoint(0);

		GLint viewport[4];
		GLfloat  winY;
		glGetIntegerv(GL_VIEWPORT, viewport);
		winY = (float)viewport[3] - screenPoint.y - 1.0f;
		//glReadBuffer(GL_BACK);
		if (useScreenFrameBuffer && readFromScreenBuffer)
		{
			//在屏幕FrameBuffer中直接渲染出一张记录各个像素世界坐标的图像
			//直接读取此图像值。
			glBindFramebuffer(GL_FRAMEBUFFER, Global::ScreenFramebuffer);
			glReadBuffer(GL_COLOR_ATTACHMENT1);
			glReadPixels((GLint)screenPoint.x, int(winY), 1, 1, GL_RGB, GL_FLOAT, &worldPoint);
		}
		else
		{
			//读取深度值
			useScreenFrameBuffer ?
				glBindFramebuffer(GL_FRAMEBUFFER, Global::ScreenFramebuffer) :
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			GLfloat depthValueZ;
			glReadPixels((GLint)screenPoint.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValueZ);
			//计算 * 逆矩阵
			mat4 matProjection = GetProjectionMatrix() * GetViewMatrix();
			dmat4 matInverse = inverse(matProjection);
			//屏幕坐标转换为标准设备坐标
			double in[4];
			in[0] = 2.0 * ((screenPoint.x - 0.0) / (width - 0)) - 1.0;
			in[1] = 1.0 - 2.0 * ((screenPoint.y - 0.0) / (height - 0));
			in[2] = 2.0 * depthValueZ - 1.0;
			in[3] = 1.0;
			dvec4 vIn = dvec4(in[0], in[1], in[2], in[3]);
			//转换标准化设备坐标至世界坐标
			dvec4 pos = matInverse * vIn;
			//逆透视除法
			pos.w = 1.0 / pos.w;
			pos.x = pos.x * pos.w;
			pos.y = pos.y * pos.w;
			pos.z = pos.z * pos.w;
			worldPoint = pos;
		}
		return worldPoint;
	}

	Camera::~Camera() {
		cameras.erase(obj->GetInstanceID());
		DEBUGLOG("~Camera");
	}
}