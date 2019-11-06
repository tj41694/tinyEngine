#pragma once
#include "DrawCmd.h"

namespace TEngine {
	class DrawCmdBelt : public DrawCmd {
	public:
		FloatArr vertices{ 0 };
	private:
		GLuint vbo = 0;
	public:
		virtual void Draw() override;
		void Clear();
		DrawCmdBelt();
		~DrawCmdBelt();
	private:
		bool TransToGL();
		void FreeDatas();
	};

}