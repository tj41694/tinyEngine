#pragma once
#include "Script.h"

namespace TEngine {
	class Camera;
	class CameraCtr : public Script {
	public:
		CameraCtr();
		void Start();
		void Update();
		virtual ~CameraCtr();
	private:
		Camera* camera_;
		Object* spheremirror;
	private:
		void GenTerrain();
	};
}