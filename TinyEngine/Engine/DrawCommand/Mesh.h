#pragma once
#include "DrawCmd.h"

namespace TEngine {
	class Mesh : public DrawCmd {
	private:
		unsigned int eboID = 0;
		unsigned int vboIDs[3] = { 0,0,0 };
		unsigned int shaderID = 0;
		unsigned int defaultShader = 0;
	public:
		FloatArr vertices{ 0 };
		FloatArr normals{ 0 };
		FloatArr uv0{ 0 };
		UintArr  triangles{ 0 };

	private:
		bool TransToGL();
		void FreeDatas();
	public:
		void RecalculateNormals();
		Mesh();
		void Draw();
		void Clear();
		~Mesh();
	};
}