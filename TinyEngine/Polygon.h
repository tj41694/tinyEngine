#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace TPolygon{
	double Area(std::vector<glm::vec2>& _polygon);
	void Offset(const std::vector<glm::vec3>& polygon, float offsetValue, std::vector<glm::vec3>& out_polygon);
}