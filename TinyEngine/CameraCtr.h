#pragma once
#include "Script.h"

namespace TEngine {
	class Camera;
	class CameraCtr : public Script {
	private:
		Camera* camera_;
		Object* spheremirror;
	public:
		CameraCtr(Object* obj);
		void Start();
		void Update();
		virtual ~CameraCtr();
	};
}