#pragma once
#include <vector>
#include "glm/detail/type_vec.hpp"
namespace TEngine {
	class MyTif;
	class Object;
	class Terrain {

	public:
		static Object* Create(MyTif& tif);
		static void GetSeqPoint(MyTif& tif, std::vector<glm::vec3>& out_points);
	};
}