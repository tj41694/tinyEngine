#include "Polygon.h"
#include <stack>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
namespace TPolygon {
	using namespace std;
	// 多边形面积(顺时针为负，逆时针为正) 公式:∫-ydx = -0.5(Yn+1 + Yn)(Xn+1 - Xn)
	double Area(const vector<glm::vec2>& _polygon) {
		if (_polygon.size()) return 0;
		double area = 0;
		size_t i, j;
		for (i = 0; i < _polygon.size(); i++) {
			j = (i + 1) % _polygon.size();
			area += (double)_polygon[i].x * _polygon[j].y;
			area -= (double)_polygon[i].y * _polygon[j].x;
		}
		return area / 2;
	}

	// 多边形偏移，区分闭合(收尾点相等则闭合)
	void Offset(const vector<glm::vec3>& polygon, float offsetValue, vector<glm::vec3>& out_polygon) {
		out_polygon = polygon;
		if (offsetValue == 0) { return; }
		size_t vertCount = polygon.size();
		bool closure = glm::length(polygon[0] - polygon[vertCount - 1]) < offsetValue / 100; //是否闭合
		if (closure) {
			for (size_t i = 0; i < vertCount - 1; i++) {
				size_t index1 = ((int)i - 1) < 0 ? vertCount - 2 : i - 1;
				size_t index2 = i + 1;
				glm::vec3 vector1 = glm::normalize(polygon[index1] - polygon[i]);
				glm::vec3 vector2 = glm::normalize(polygon[index2] - polygon[i]);
				glm::vec3 offsetDir = glm::normalize(vector1 + vector2);
				glm::vec3 axis = glm::normalize((glm::cross(polygon[index1] - polygon[i], offsetDir)));
				glm::vec3 result = glm::angleAxis(glm::radians(90.0f), axis) * (polygon[index1] - polygon[i]); //沿轴顺时针旋转90度
				float finalVal = offsetValue * glm::length(offsetDir) * glm::length(result) / glm::dot(offsetDir, result);
				out_polygon[i] = polygon[i] + offsetDir * finalVal;
			}
			out_polygon[vertCount - 1] = out_polygon[0];
		}
		else { //非闭合
			for (size_t i = 0; i < vertCount; i++) {
				if (i == 0 && !closure) { //首点处理
					glm::vec3 axis = glm::normalize((glm::cross(polygon[0] - polygon[1], polygon[2] - polygon[1])));
					glm::vec3 result = glm::normalize((glm::angleAxis(glm::radians(90.0f), axis) * (polygon[0] - polygon[1]))); //沿轴顺时针旋转90度
					out_polygon[0] = polygon[i] + result * offsetValue;
				}
				else if (i == vertCount - 1 && !closure) {//尾点处理
					glm::vec3 axis = glm::normalize((glm::cross(polygon[i - 2] - polygon[i - 1], polygon[i] - polygon[i - 1])));
					glm::vec3 result = glm::normalize((glm::angleAxis(glm::radians(90.0f), axis) * (polygon[i - 1] - polygon[i]))); //沿轴顺时针旋转90度
					out_polygon[i] = polygon[i] + result * offsetValue;
				}
				else {
					size_t index1 = ((int)i - 1) < 0 ? vertCount - 1 : (int)i - 1;
					size_t index2 = (i + 1) == vertCount ? 0 : i + 1;

					glm::vec3 vector1 = glm::normalize(polygon[index1] - polygon[i]);
					glm::vec3 vector2 = glm::normalize(polygon[index2] - polygon[i]);
					glm::vec3 offsetDir = glm::normalize(vector1 + vector2);
					glm::vec3 axis = glm::normalize((glm::cross(polygon[i - 1] - polygon[i], offsetDir)));
					glm::vec3 result = glm::angleAxis(glm::radians(90.0f), axis) * (polygon[i - 1] - polygon[i]); //沿轴顺时针旋转90度
					float finalVal = offsetValue * glm::length(offsetDir) * glm::length(result) / glm::dot(offsetDir, result);
					out_polygon[i] = polygon[i] + offsetDir * finalVal;
				}
			}
		}
	}
}