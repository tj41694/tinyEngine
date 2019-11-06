#pragma once
#include "DrawCmd.h"
#include <vector>
#include "glm/glm.hpp"

namespace TEngine {
	class Mesh : public DrawCmd {
	private:
		GLuint eboID = 0;
		GLuint vboIDs[3] = { 0,0,0 };
		GLuint shaderID = 0;
		GLuint defaultShader = 0;
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