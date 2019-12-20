#pragma once
#include <vector>
#include "glm/detail/type_vec.hpp"

namespace TEngine {
	class Object;
	class BeltStrip {
	public:
		Object* obj;
	public:
		BeltStrip();
		void DrawBeltStrip(const std::vector<glm::vec3>& pointSrrip, float width, glm::vec2 texSize);
		void DrawBeltStrip_Mesh(const std::vector<glm::vec3>& pointSrrip, float width, glm::vec2 texSize);
		~BeltStrip();
	};
}