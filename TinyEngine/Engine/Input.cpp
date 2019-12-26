#pragma once
#include "Object.h"
#include "Input.h"

namespace TEngine {
	dvec2		Input::mousePosition(0, 0);
	dvec2		Input::lasPos(0, 0);
	bool		Input::mouseMoving = false;
	double	    Input::Vertical = 0;
	double		Input::Horizontal = 0;
	double	    Input::deltaTime = 0;

	void Input::Initial(glContext* glContext) {
		if (glContext == nullptr || glContext->glNativeWindow == nullptr) { return; }
		glfwSetCursorPosCallback(glContext->glNativeWindow, OnMouseMove);
		glfwSetCursorEnterCallback(glContext->glNativeWindow, OnCursorEnterCallback);
	}

	bool first = true;
	void Input::OnCursorEnterCallback(GLFWwindow* window, int val) {
		first = true;
	}

	void Input::Update(double deltaTime_) {
		deltaTime = deltaTime_;
		if (GetKeyDown(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(Object::context->glNativeWindow, true);
		double x, y;
		glfwGetCursorPos(Object::context->glNativeWindow, &x, &y);

		if (first) {
			mousePosition.x = x;
			mousePosition.y = y;
			lasPos.x = mousePosition.x;
			lasPos.y = mousePosition.y;
			first = false;
		}
		Vertical = lasPos.y - mousePosition.y;
		Horizontal = lasPos.x - mousePosition.x;
		lasPos.x = mousePosition.x;
		lasPos.y = mousePosition.y;
		mousePosition.x = x;
		mousePosition.y = y;
		mouseMoving = (lasPos != mousePosition);
	}

	void Input::OnMouseMove(GLFWwindow* window, double x, double y) {}

	bool Input::GetKey(int vKey) {
		return glfwGetKey(Object::context->glNativeWindow, vKey) == GLFW_PRESS;
	}

	bool Input::GetKeyDown(int vKey) {
		return glfwGetKey(Object::context->glNativeWindow, vKey) == GLFW_PRESS;
	}

	int Input::GetMouseButton(int button) {
		return glfwGetMouseButton(Object::context->glNativeWindow, button);
	}

	bool Input::GetMouseButtonUp(int button) {
		static bool pressing;
		static bool pressed;
		pressing = glfwGetMouseButton(Object::context->glNativeWindow, button) == GLFW_PRESS;
		if(!pressing && pressed)
		{
			pressed = pressing;
			return true;
		}
		pressed = pressing;
		return false;
	}

	bool Input::GetMouseButtonDown(int button) {
		static bool pressing;
		static bool pressed;
		pressing = glfwGetMouseButton(Object::context->glNativeWindow, button) == GLFW_PRESS;
		if(pressing && pressed != pressing){
			pressed = pressing;
			return true;
		}
		pressed = pressing;
		return false;
	}

	double Input::GetAxisX() {
		return Horizontal;
	}

	double Input::GetAxisY() {
		return Vertical;
	}
}