#pragma once
namespace TEngine {
	class Object;
	class Component {
	public:
		Object* obj;
	public:
		Component();
		virtual ~Component();
		virtual void Start();
		virtual void Update();
	private:
	};
}