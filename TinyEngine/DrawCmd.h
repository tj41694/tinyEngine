#pragma once
#include "OpenglUtil.h"
namespace TEngine {

	struct FloatArr {
		float* data;
		GLuint count;
	};

	struct UintArr {
		GLuint* data;
		GLuint count;
	};

	class DrawCmd {
	protected:
		GLuint VAO = 0;
	public:
		GLuint materialIndex = 0;
	public:
		virtual void Draw() = 0;
	};
}