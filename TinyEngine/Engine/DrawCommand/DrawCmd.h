#pragma once
namespace TEngine {

	struct FloatArr {
		float* data;
		unsigned int count;
	};

	struct UintArr {
		unsigned int* data;
		unsigned int count;
	};

	class DrawCmd {
	protected:
		unsigned int VAO = 0;
	public:
		unsigned int materialIndex = 0;
	public:
		virtual void Draw() = 0;
	};
}