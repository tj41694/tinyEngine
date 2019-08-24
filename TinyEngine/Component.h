#pragma once
namespace TEngine {
	class Object;
	class Component {
	private:
	protected:
		virtual void CreateVtable();
	public:
		Object* obj;
		Component(Object* obj);
		virtual ~Component();
	};
}
using namespace TEngine;