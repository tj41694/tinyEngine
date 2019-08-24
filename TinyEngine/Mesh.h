#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "esUtil.h"

namespace TEngine {
	using namespace std;

	struct FloatArr {
		float* data;
		GLuint count;
	};

	struct UintArr {
		GLuint* data;
		GLuint count;
	};

	class Mesh {
	private:
		GLuint VAO = 0;
		GLuint eboID = 0;
		GLuint vboIDs[3] = { 0,0,0 };
		GLuint shaderID = 0;
		GLuint defaultShader = 0;
	public:
		GLuint materialIndex = 0;
		FloatArr vertices	{ 0 };
		FloatArr normals	{ 0 };
		FloatArr colors		{ 0 };
		FloatArr uv0		{ 0 };
		UintArr  triangles	{ 0 };

	private:
		bool TransToGL();
		void FreeDatas();
	public:
		void RecalculateNormals();
		Mesh();
		void Draw();
		~Mesh();
	};
}