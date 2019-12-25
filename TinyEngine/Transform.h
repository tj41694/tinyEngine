#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <vector>

namespace TEngine {
	class Transform : public Component {
	public:

	public:
		Transform();

		Transform* Parent() const;
		void SetParent(Transform* parent_, bool worldPositionStays = true);
		unsigned int ChildCount() const;
		Transform* Child(unsigned int index) const;

		glm::vec3& LocalPos();
		glm::quat& LocalRotation();
		glm::vec3& LocalScale();
		glm::vec3 Positon() const;              //世界坐标
		glm::quat Rotation() const;				//世界旋转四元数
		const glm::vec3& EulerAngles() const;	//世界欧拉角

		const glm::vec3 Forwward() const;		//物体前向量
		const glm::vec3 Right() const;			//物体右向量
		const glm::vec3 Up() const;				//物体上向量

		const glm::mat4 LocalToWorldMarix();
		const glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
		const glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp);
		const glm::quat RotateTowards(glm::quat q1, glm::quat q2, float maxAngle);

		void MoveTo(const float& x, const float& y, const float& z);
		void MoveTo(const glm::vec3& vec);
		void Move(const float& x, const float& y, const float& z);
		void Move(const glm::vec3& vec);
		void RotateTo(const glm::vec3& eulerAngles_);
		void RotateTo(const float& x, const float& y, const float& z);
		void RotateTo(const glm::quat& q);
		void Rotate(const glm::vec3& eulerAngles_);
		void Rotate(const glm::vec3& axis, const float value);

	private:
		glm::vec3 localPositon;
		glm::quat localRotation;
		glm::vec3 localScale;
		glm::vec3 eulerAngles;
		Transform* parent;
		std::vector<Transform*> childs;
	};
}