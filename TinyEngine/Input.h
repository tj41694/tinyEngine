#pragma once
#include "esUtil.h"
#include "glm/glm.hpp"

namespace TEngine {
	using namespace glm;
	class Input {
	private:
		static ivec2 lasPos;
		static long Vertical;
		static long Horizontal;

	public:
		static ivec2 mousePosition;
		static bool mouseMoving;
		static double deltaTime;
		static void Update(ESContext *esContext, const float deltaTime);
		static bool GetKey(int vKey);
		static bool GetKeyDown(int vKey);
		static long GetAxisX();
		static long GetAxisY();
	};
}