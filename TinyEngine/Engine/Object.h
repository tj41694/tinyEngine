#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "OpenglUtil.h"
#include "Tools/Debug.h"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtx/quaternion.hpp"

namespace TEngine {
	class Component;
	class Script;
	class Transform;
	enum class Shape {
		cube,
		plane,
		sphere
	};

	class Object {
	public:
		static glContext* context;

		std::string name;
		//glm::vec3 scale;

	private:
		static unsigned int instanceid;

		unsigned int id = 0;
		//unsigned int parentId = 0;
		std::unordered_map<size_t, Component*> compenents;
		std::vector<unsigned int> childs;
	protected:
		Transform* transform;
		//glm::vec3 localPositon;
		//glm::quat rotation;
		//glm::vec3 eulerAngles;

	public:
		Object();
		Object(const char* name_);

		Transform* Trans() const;
		//const glm::vec3 WorldPos() const;
		//const glm::vec3& LocalPos() const;
		//const glm::quat Rotation() const;
		//const glm::quat& LocalRotation() const;
		//const glm::vec3& EulerAngles() const;
		//const glm::vec3 Forwward() const;
		//const glm::vec3 Right() const;
		//const glm::vec3 Up() const;
		//const glm::mat4 LocalToWorldMarix();
		//const glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
		//const glm::quat LookAt(glm::vec3 direction, glm::vec3 desiredUp);
		//const glm::quat RotateTowards(glm::quat q1, glm::quat q2, float maxAngle);

		//void MoveTo(const float& x, const float& y, const float& z);
		//void MoveTo(const glm::vec3& vec);
		//void Move(const float& x, const float& y, const float& z);
		//void Move(const glm::vec3& vec);
		//void RotateTo(const glm::vec3& eulerAngles);
		//void RotateTo(const float& x, const float& y, const float& z);
		//void RotateTo(const glm::quat& q);
		//void Rotate(const glm::vec3& eulerAngles);
		//void Rotate(const glm::vec3& axis, const float value);


		//Object* Parent() const;
		//void SetParent(Object* obj_, bool worldPositionStays = true);
		//unsigned int ChildCount();
		//Object* Child(unsigned int index);

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
					DEBUGLOG("存在相同组件 添加失败");
				}
			}
			else {
				DEBUGLOG("非Component类型 添加组件失败");
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