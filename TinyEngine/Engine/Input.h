#pragma once
#include "glm/glm.hpp"
#include <unordered_map>

struct GLContext;
struct GLFWwindow;
namespace TEngine {
	class Input {
	private:
		static glm::vec2 lasPos;
		static float Vertical;
		static float Horizontal;

		static std::unordered_map<int, bool> keyPressing;
		static std::unordered_map<int, bool> keyPressed;
	public:
		static glm::vec2 mousePosition;
		static bool mouseMoving;
		static double deltaTime;
	public:
		static void Initial(glContext* window);
		static void OnCursorEnterCallback(GLFWwindow* window, int val);
		static void Update(double deltaTime);
		static void OnMouseMove(GLFWwindow*, double x, double y);
		static bool GetKey(int vKey);
		static bool GetKeyUp(int vKey);
		static bool GetKeyDown(int vKey);
		static int  GetMouseButton(int button);
		static bool  GetMouseButtonUp(int button);
		static bool  GetMouseButtonDown(int button);
		static double GetAxisX();
		static double GetAxisY();
	};
}