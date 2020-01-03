#include "DrawCmdBelt.h"
#include <glad/glad.h>

namespace TEngine {

	DrawCmdBelt::DrawCmdBelt() {}

	bool DrawCmdBelt::TransToGL() {
		if (vertices.data == nullptr) { return false; }
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &vbo);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.count * sizeof(GLfloat), vertices.data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		FreeDatas();
		return true;
	}

	void DrawCmdBelt::FreeDatas() {
		if (vertices.data) {
			delete[] vertices.data;
			vertices.data = nullptr;
		}
	}

	void DrawCmdBelt::Draw() {
		if (VAO == 0) {
			TransToGL();
		}
		else {
			glBindVertexArray(VAO);
			glDrawArrays(GL_LINE_STRIP, 0, vertices.count / 3);
			glBindVertexArray(0);
		}
	}

	void DrawCmdBelt::Clear() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &vbo);
		VAO = 0;
		vbo = 0;
		FreeDatas();
	}

	DrawCmdBelt::~DrawCmdBelt() {
		Clear();
	}
}