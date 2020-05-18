#pragma once
#include "Object.h"
#include "Input.h"

namespace TEngine {
	using namespace glm;
	vec2							Input::mousePosition(0, 0);
	vec2							Input::lasPos(0, 0);
	bool							Input::mouseMoving = false;
	float							Input::Vertical = 0;
	float							Input::Horizontal = 0;
	double							Input::deltaTime = 0;
	std::unordered_map<int, bool>	Input::keyPressing;
	std::unordered_map<int, bool>	Input::keyPressed;

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

		//static bool pressing;
		//pressing = glfwGetKey(Object::context->glNativeWindow, GLFW_KEY_F2) == GLFW_PRESS;
		//DEBUGLOG(pressing);
		if (GetKeyDown(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(Object::context->glNativeWindow, true);
		double x, y;
		glfwGetCursorPos(Object::context->glNativeWindow, &x, &y);

		if (first) {
			mousePosition.x = (float)x;
			mousePosition.y = (float)y;
			lasPos.x = mousePosition.x;
			lasPos.y = mousePosition.y;
			first = false;
		}
		Vertical = lasPos.y - mousePosition.y;
		Horizontal = lasPos.x - mousePosition.x;
		lasPos.x = mousePosition.x;
		lasPos.y = mousePosition.y;
		mousePosition.x = (float)x;
		mousePosition.y = (float)y;
		mouseMoving = (lasPos != mousePosition);
	}

	void Input::OnMouseMove(GLFWwindow* window, double x, double y) {}

	bool Input::GetKey(int vKey) {
		return glfwGetKey(Object::context->glNativeWindow, vKey) == GLFW_PRESS;
	}

	bool Input::GetKeyUp(int vKey)
	{
		if (keyPressing.find(vKey) == keyPressing.end()) { keyPressing[vKey] = false; }
		if (keyPressed.find(vKey) == keyPressed.end()) { keyPressed[vKey] = false; }
		bool pressing = keyPressing[vKey] = glfwGetKey(Object::context->glNativeWindow, vKey) == GLFW_PRESS;
		if (!pressing && keyPressed[vKey])
		{
			keyPressed[vKey] = pressing;
			return true;
		}
		keyPressed[vKey] = pressing;
		return false;
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