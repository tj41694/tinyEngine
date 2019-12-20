#pragma once
#include "OpenglUtil.h"
#include <vector>
#include <string>
#include <unordered_set>
#include "Debug.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include <unordered_map>
#include "Script.h"

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
		std::vector<Script*> scripts;
		std::unordered_map<size_t, Component*> compenents;
		std::vector<unsigned int> childs;
	protected:
		vec3 localPositon;
		quat rotation;
		vec3 eulerAngles;

	public:
		Object();
		Object(const char* name_);
		const vec3 WorldPos() const;
		const vec3& LocalPos() const;
		const quat Rotation() const;
		const quat& LocalRotation() const;
		const vec3& EulerAngles() const;
		const vec3 Forwward() const;
		const vec3 Right() const;
		const vec3 Up() const;
		const mat4 LocalToWorldMarix();
		const quat RotationBetweenVectors(vec3 start, vec3 dest);
		const quat LookAt(vec3 direction, vec3 desiredUp);
		const quat RotateTowards(quat q1, quat q2, float maxAngle);

		void MoveTo(const float& x, const float& y, const float& z);
		void MoveTo(const vec3& vec);
		void Move(const float& x, const float& y, const float& z);
		void Move(const vec3& vec);
		void RotateTo(const vec3& eulerAngles);
		void RotateTo(const float& x, const float& y, const float& z);
		void RotateTo(const quat& q);
		void Rotate(const vec3& eulerAngles);
		void Rotate(const vec3& axis, const float value);


		Object* Parent() const;
		void SetParent(Object* obj_, bool worldPositionStays = true);
		unsigned int ChildCount();
		Object* Child(unsigned int index);

		const std::unordered_map<size_t, Component*>& Components() const;
		unsigned int GetInstanceID();
		virtual ~Object();

	public:
		template <typename T>
		inline T* const AddComponent() {
			T* instance = new T();
			Component* component = dynamic_cast<Component*>(instance);
			if (component) {
				if (compenents.end() == compenents.find(typeid(T).hash_code())) {
					component->obj = this;
					component->Start();
					compenents.insert(std::pair<size_t, Component*>(typeid(T).hash_code(), component));
					return instance;
				}
				else {
					Debug::Log("存在相同组件 添加失败\n");
				}
			}
			else {
				Debug::Log("非Component类型 添加组件失败\n");
			}
			delete instance;
			return nullptr;
		}

		template <typename T>
		inline T* const GetComponent() {
			if (compenents.end() != compenents.find(typeid(T).hash_code())) {
				T* result = dynamic_cast<T*>(compenents[typeid(T).hash_code()]);
				return result;
			}
			else {
				return nullptr;
			}
		}

		template <typename T>
		inline void const DestroyComponent() {
			if (compenents.end() != compenents.find(typeid(T).hash_code())) {
				delete compenents[typeid(T).hash_code()];
				compenents.erase(typeid(T).hash_code());
			}
		}

	public:
		static Object* CreateShape(Shape shape, float size = 1.0f, bool flip = false);
		static Object* LoadModel(std::string path, std::string name);
		static int GenSphere(int numSlices, float radius, GLfloat** vertices, GLfloat** normals, GLfloat** texCoords, GLuint** indices);
		static int GenCube(float scale, GLfloat** vertices, GLfloat** normals, GLfloat** texCoords, GLuint** indices);
		static int GenSquareGrid(int size, GLfloat** vertices, GLuint** indices);
	};
}