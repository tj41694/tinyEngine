#pragma once
#include "Mesh.h"
#include "Debug.h"
#include "OpenglUtil.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace TEngine {
	Mesh::Mesh() {}

	void Mesh::RecalculateNormals() {
		using namespace glm;
		void* ptr = nullptr;
		if (normals.data) {
			ptr = realloc(normals.data, vertices.count * sizeof(GLfloat));
			if (ptr == nullptr) {
				free(normals.data);
				ptr = malloc(vertices.count * sizeof(GLfloat));
			}
		}
		else {
			ptr = malloc(vertices.count * sizeof(GLfloat));
		}
		if (ptr == nullptr || triangles.count % 3 != 0) { return; }
		normals.data = (GLfloat*)ptr;
		normals.count = vertices.count;
		GLuint triangleCount = triangles.count / 3;
		for (GLuint i = 0; i < triangleCount; i++) {
			GLuint first = triangles.data[i * 3];
			GLuint secend = triangles.data[first + 1];
			GLuint third = triangles.data[secend + 1];
			vec3 v0(vertices.data[(first * 3)], vertices.data[(first * 3) + 1], vertices.data[(first * 3) + 2]);
			vec3 v1(vertices.data[(secend * 3)], vertices.data[(secend * 3) + 1], vertices.data[(secend * 3) + 2]);
			vec3 v2(vertices.data[(third * 3)], vertices.data[(third * 3) + 1], vertices.data[(third * 3) + 2]);
			vec3 v20 = v0 - v2;
			vec3 v01 = v1 - v0;
			vec3 v12 = v2 - v1;
			vec3 n0 = cross(v20, v01);
			n0 = normalize(n0);
			normals.data[first * 3] = n0.x; normals.data[(first * 3) + 1] = n0.y; normals.data[(first * 3) + 2] = n0.z;
			vec3 n1 = cross(v01, v12);
			n1 = normalize(n1);
			normals.data[secend * 3] = n1.x; normals.data[(secend * 3) + 1] = n1.y; normals.data[(secend * 3) + 2] = n1.z;
			vec3 n2 = cross(v12, v20);
			n2 = normalize(n2);
			normals.data[third * 3] = n2.x; normals.data[(third * 3) + 1] = n2.y; normals.data[(third * 3) + 2] = n2.z;
		}
	}

	bool Mesh::TransToGL() {
		if (vertices.data == nullptr) { return false; }
		glGenVertexArrays(1, &VAO);
		glGenBuffers(3, vboIDs);
		glGenBuffers(1, &eboID);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.count * sizeof(GLfloat), vertices.data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
		glBufferData(GL_ARRAY_BUFFER, normals.count * sizeof(GLfloat), normals.data, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
		glBufferData(GL_ARRAY_BUFFER, uv0.count * sizeof(GLfloat), uv0.data, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.count * sizeof(GLuint), triangles.data, GL_STATIC_DRAW);
		glBindVertexArray(0);
		FreeDatas();
		return true;
	}

	void Mesh::Clear() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(3, vboIDs);
		glDeleteBuffers(1, &eboID);
		VAO = 0;
		memset(vboIDs, 0, sizeof(vboIDs));
		eboID = 0;
		FreeDatas();
	}

	void Mesh::FreeDatas() {
		if (vertices.data) {
			delete[] vertices.data;
			vertices.data = nullptr;
		}
		if (normals.data) {
			delete[] normals.data;
			normals.data = nullptr;
		}
		if (uv0.data) {
			delete[]uv0.data;
			uv0.data = nullptr;
		}
		if (triangles.data) {
			delete[] triangles.data;
			triangles.data = nullptr;
		}
	}

	void Mesh::Draw() {
		if (VAO == 0) {
			TransToGL();
		}
		else {
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, triangles.count, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	Mesh::~Mesh() {
		Clear();
	}
}