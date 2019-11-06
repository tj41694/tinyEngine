#pragma once
#include "OpenglUtil.h"
#include "glm/glm.hpp"

namespace TEngine {
	using namespace glm;
	class Input {
	private:
		static dvec2 lasPos;
		static double Vertical;
		static double Horizontal;

	public:
		static dvec2 mousePosition;
		static bool mouseMoving;
		static double deltaTime;
	public:
		static void Initial(glContext* window);
		static void OnCursorEnterCallback(GLFWwindow* window, int val);
		static void Update(double deltaTime);
		static void OnMouseMove(GLFWwindow*, double x, double y);
		static bool GetKey(int vKey);
		static bool GetKeyDown(int vKey);
		static int  GetMouseButton(int button);
		static double GetAxisX();
		static double GetAxisY();
	};
}