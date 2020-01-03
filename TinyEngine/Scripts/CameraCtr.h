#pragma once
#include "Engine/Components/Script.h"

namespace TEngine {
	class Camera;
	class CameraCtr : public Script {
	public:
		CameraCtr();
		void Start();
		void Update();
		virtual ~CameraCtr();
	private:
		Camera* camera_		 = nullptr;
		Object* spheremirror = nullptr;
	};
}