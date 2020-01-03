#include "Transform.h"
#include "Engine/Object.h"
namespace TEngine {
	using namespace glm;
	using namespace std;
	Transform::Transform() {
		localScale = vec3(1.0f);
		localPositon = vec3(0.0f);
		localRotation = quat();
		eulerAngles = vec3(0.0f);
		parent = nullptr;
	}
	Transform* Transform::Parent() const {
		return parent;
	}
	void Transform::SetParent(Transform* parent_, bool worldPositionStays) {
		if (worldPositionStays) {
			localPositon = localPositon - parent_->Positon();
		}
		parent = parent_;
		parent_->childs.push_back(this);
	}

	unsigned int Transform::ChildCount() const {
		return childs.size();
	}

	Transform* Transform::Child(unsigned int index) const {
		return childs[index];
	}

	glm::vec3 Transform::Positon() const {
		if (parent == nullptr)
			return localPositon;
		else
			return parent->Positon() + (parent->localRotation * localPositon) * parent->localScale;
	}
	const glm::vec3& Transform::LocalPos() const {
		return localPositon;
	}
	glm::quat Transform::Rotation() const {
		if (parent == nullptr)
			return localRotation;
		else
			return parent->Rotation() * localRotation;
	}
	const glm::quat& Transform::LocalRotation() const {
		return localRotation;
	}
	const glm::vec3& Transform::LocalScale() const {
		return localScale;
	}
	const glm::vec3& Transform::EulerAngles() const {
		return eulerAngles;
	}
	const glm::vec3 Transform::Forwward() const {
		return Rotation() * vec3(0, 0, 1.0f);
	}
	const glm::vec3 Transform::Right() const {
		return Rotation() * vec3(1.0f, 0, 0);
	}
	const glm::vec3 Transform::Up() const {
		return Rotation() * vec3(0, 1.0f, 0);
	}
	const glm::mat4& Transform::LocalToWorldMarix() {
		if (modelMatrixDirt) {
			modelMatrixDirt = false;
			mat4 unit(1.0f);
			modelMatrix = translate(unit, localPositon) * mat4_cast(localRotation) * glm::scale(unit, localScale);
		}
		if (parent == nullptr) {
			return modelMatrix;
		}
		else {
			modelMatrix = parent->LocalToWorldMarix() * modelMatrix;
			return modelMatrix;
		}
	}
	const glm::quat Transform::RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
		start = normalize(start);
		dest = normalize(dest);

		float cosTheta = dot(start, dest);
		vec3 rotationAxis;

		if (cosTheta < -1 + 0.000001f) {
			// special case when vectors in opposite directions :
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			// This implementation favors a rotation around the Up axis,
			// since it's often what you want to do.
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);
			if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
				rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);

			rotationAxis = normalize(rotationAxis);
			return angleAxis(glm::radians(180.0f), rotationAxis);
		}

		// Implementation from Stan Melax's Game Programming Gems 1 article
		rotationAxis = cross(start, dest);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);
	}
	const glm::quat Transform::LookAt(glm::vec3 direction, glm::vec3 desiredUp) {
		if (length2(direction) < 0.0001f)
			return localRotation;
		//重新计算目标方向
		direction = parent == nullptr ? direction : glm::inverse(parent->Rotation()) * direction;

		quat rot1 = RotationBetweenVectors(vec3(0, 0, 1), direction);

		vec3 right = cross(direction, desiredUp);
		desiredUp = cross(right, direction);

		vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
		quat rot2 = RotationBetweenVectors(newUp, desiredUp);

		return  rot2 * rot1;
	}
	const glm::quat Transform::RotateTowards(glm::quat q1, glm::quat q2, float maxAngle) {
		if (maxAngle < 0.001f) { return q1; } // No rotation allowed. Prevent dividing by 0 later.

		float cosTheta = dot(q1, q2);

		// q1 and q2 are already equal.
		// Force q2 just to be sure
		if (cosTheta > 0.9999f) { return q2; }

		// Avoid taking the long path around the sphere
		if (cosTheta < 0) {
			q1 = q1 * -1.0f;
			cosTheta *= -1.0f;
		}

		float angle = acos(cosTheta);

		// If there is only a 2° difference, and we are allowed 5°,
		// then we arrived.
		if (angle < maxAngle) {
			return q2;
		}

		// This is just like slerp(), but with a custom t
		float t = maxAngle / angle;
		angle = maxAngle;

		quat res = (sin((1.0f - t) * angle) * q1 + sin(t * angle) * q2) / sin(angle);
		res = normalize(res);
		return res;
	}
	void Transform::MoveTo(const float& x, const float& y, const float& z) {
		localPositon = vec3(x, y, z);
		SetLocalToWorldMarixDirt();
	}

	void Transform::MoveTo(const glm::vec3& vec) {
		localPositon = vec;
		SetLocalToWorldMarixDirt();
	}

	void Transform::Move(const float& x, const float& y, const float& z) {
		localPositon += vec3(x, y, z);
		SetLocalToWorldMarixDirt();
	}

	void Transform::Move(const glm::vec3& vec) {
		localPositon += vec;
		SetLocalToWorldMarixDirt();
	}

	void Transform::RotateTo(const glm::vec3& eulerAngles_) {
		localRotation = quat(eulerAngles_);
		eulerAngles = glm::eulerAngles(localRotation);
		SetLocalToWorldMarixDirt();
	}

	void Transform::RotateTo(const float& x, const float& y, const float& z) {
		localRotation = quat(vec3(x, y, z));
		eulerAngles = glm::eulerAngles(localRotation);
		SetLocalToWorldMarixDirt();
	}

	void Transform::RotateTo(const glm::quat& q) {
		localRotation = q;
		eulerAngles = glm::eulerAngles(localRotation);
		SetLocalToWorldMarixDirt();
	}

	void Transform::Rotate(const glm::vec3& eulerAngles_) {
		localRotation = quat(eulerAngles_) * localRotation;
		eulerAngles = glm::eulerAngles(localRotation);
		SetLocalToWorldMarixDirt();
	}

	void Transform::Rotate(const glm::vec3& axis, const float value) {
		localRotation = glm::angleAxis(value, axis) * localRotation;
		eulerAngles = glm::eulerAngles(localRotation);
		SetLocalToWorldMarixDirt();
	}
	void Transform::SetLocalScale(const glm::vec3& scale_) {
		localScale = scale_;
		SetLocalToWorldMarixDirt();
	}
	void Transform::SetLocalToWorldMarixDirt() {
		modelMatrixDirt = true;
		for (auto child : childs)
		{
			child->SetLocalToWorldMarixDirt();
		}
	}
}