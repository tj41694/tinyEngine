#pragma once
#include "Component.h"
	class Script : public Component {
	private:
	public:
		bool notStarted;

		Script(Object* obj);
		virtual void Start();
		virtual void Update();
		virtual ~Script();
	};