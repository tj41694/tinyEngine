#pragma once
#include "OpenglUtil.h"
#include <vector>
#include <string>
#include <unordered_set>
#include "Debug.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace TEngine {
	using namespace glm;

	class Component;
	enum class Shape {
		cube,
		plane,
		sphere
	};

	class Object {
	public:
		static glContext* context;

		std::string name;
		vec3 scale;

	private:
		static unsigned int instanceid;

		unsigned int id = 0;
		unsigned int parent = 0;
		std::vector<Component*> compenents;
		std::vector<unsigned int> childs;
	protected:
		vec3 localPositon;
		quat rotation;
		vec3 eulerAngles;

	public:
		Object();
		Object(const char* name_);
		vec3 WorldPos();
		vec3 LocalPos() const;
		quat Rotation() const;
		quat LocalRotation();
		vec3 EulerAngles();
		vec3 Forwward() const;
		vec3 Right() const;
		vec3 Up() const;
		void MoveTo(const float &x, const float &y, const float &z);
		void MoveTo(const vec3 & vec);
		void Move(const float & x, const float & y, const float & z);
		void Move(const vec3 &vec);
		void RotateTo(const vec3 & eulerAngles);
		void RotateTo(const float & x, const float & y, const float & z);
		void RotateTo(const quat & q);
		void Rotate(const vec3 & eulerAngles);
		void Rotate(const vec3 & axis, const float value);

		Object* Parent() const;
		void SetParent(Object* obj_, bool worldPositionStays = true);
		unsigned int ChildCount();
		Object* Child(unsigned int index);

		template <typename T>
		inline T* const AddComponent() {
			for (unsigned int i = 0; i < compenents.size(); i++) {
				T* result = dynamic_cast<T*>(compenents[i]);
				if (result != nullptr) {
					Debug::Log("添加组件失败(存在相同组件):");
					Debug::Log(typeid(T));
					return nullptr;
				}
			}
			T* component = new T(this);
			compenents.push_back(component);
			return component;
		}

		template <typename T>
		inline T* const GetComponent() {
			T* result = nullptr;
			for (auto it = compenents.begin(); it != compenents.end(); it++) {
				result = dynamic_cast<T*>(*it);
				if (result != nullptr) {
					break;
				}
			}
			return result;
		}

		template <typename T>
		inline void const DestroyComponent() {
			T* result = nullptr;
			for (auto it = compenents.begin(); it != compenents.end();) {
				result = dynamic_cast<T*>(*it);
				if (result != nullptr) {
					compenents.erase(it);
					delete result;
					break;
				} else {
					it++;
				}
			}
		}

		unsigned int GetInstanceID();
		static Object* CreateShape(Shape shape, float size = 1.0f, bool flip = false);
		static Object* LoadModel(std::string path, std::string name);
		static int GenSphere(int numSlices, float radius, GLfloat** vertices, GLfloat** normals, GLfloat** texCoords, GLuint** indices);
		static int GenCube(float scale, GLfloat** vertices, GLfloat** normals, GLfloat** texCoords, GLuint** indices);
		static int GenSquareGrid(int size, GLfloat** vertices, GLuint** indices);

		mat4 LocalToWorldMarix();
		quat RotationBetweenVectors(vec3 start, vec3 dest);
		quat LookAt(vec3 direction, vec3 desiredUp);
		quat RotateTowards(quat q1, quat q2, float maxAngle);
		virtual ~Object();
	};
}
using namespace TEngine;