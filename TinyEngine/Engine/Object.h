#pragma once
#include <vector>
#include <string>
#include <vcruntime_typeinfo.h>
#include <unordered_map>
#include "OpenglUtil.h"
#include "Tools/Debug.h"
#include "Components/Transform.h"

namespace TEngine {
	class Script;
	enum class Shape {
		cube,
		plane,
		sphere
	};

	class Object {
	public:
		static glContext* context;
		std::string name;

	private:
		static unsigned int instanceid;
		unsigned int id = 0;
		std::unordered_map<size_t, Component*> compenents;
		std::vector<unsigned int> childs;
	protected:
		Transform* transform = nullptr;

	public:
		Object();
		Object(const char* name_);
		Transform* Trans() const;
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