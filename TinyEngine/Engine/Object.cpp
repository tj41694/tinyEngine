#include "Object.h"
#include "Engine/Components/Render.h"
#include "Engine/DrawCommand/Mesh.h"
#include "Engine/Components/DrawCmdFilter.h"
#include "DataDef.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace TEngine {
	using namespace glm;
	using namespace std;
	unsigned int Object::instanceid = 1;
	glContext* Object::context;

	Object::Object() :id(instanceid++) {
		((UserData*)context->userData)->allObjects->insert(std::pair<unsigned int, Object*>(GetInstanceID(), this));
		transform = AddComponent<Transform>();
	}

	Object::Object(const char* name_) :id(instanceid++) {
		((UserData*)context->userData)->allObjects->insert(std::pair<int, Object*>(GetInstanceID(), this));
		transform = AddComponent<Transform>();
		name.assign(name_);
		//DEBUGLOG("Create Object:" + name);
	}

	Transform* Object::Trans() const {
		return transform;
	}

	const std::unordered_map<size_t, Component*>& Object::Components() const {
		return compenents;
	}

	unsigned int Object::GetInstanceID() {
		return id;
	}

	Object* Object::CreateShape(Shape shape, float size, bool flip) {
		Object* obj = new Object();
		obj->name = "Skybox";
		DrawCmdFilter* meshFilter = obj->AddComponent<DrawCmdFilter>();
		Render* render = obj->AddComponent<Render>();
		Mesh* mesh = new Mesh();
		meshFilter->drawCmds.push_back(mesh);
		switch (shape) {
		case Shape::cube:
		{
			mesh->vertices.count = mesh->normals.count = 24 * 3;
			mesh->uv0.count = 24 * 2;
			mesh->triangles.count = GenCube(size, &mesh->vertices.data, &mesh->normals.data, &mesh->uv0.data, &mesh->triangles.data);
			break;
		}
		case Shape::sphere:
		{
			int slice = 48;
			int numVertices = (slice / 2 + 1) * (slice + 1);
			mesh->vertices.count = mesh->normals.count = numVertices * 3;
			mesh->uv0.count = numVertices * 2;
			mesh->triangles.count = GenSphere(slice, size, &mesh->vertices.data, &mesh->normals.data, &mesh->uv0.data, &mesh->triangles.data);
			break;
		}
		case Shape::plane:
		{
			float value = size / 2.0f;
			mesh->vertices.count = mesh->normals.count = 4 * 3;
			mesh->uv0.count = 4 * 2;
			mesh->triangles.count = 6;
			mesh->vertices.data = (GLfloat*)malloc(mesh->vertices.count * sizeof(GLfloat));
			if (mesh->vertices.data) {
				float temp[] = { -value, 0, -value, -value, 0, value, value, 0, value, value, 0, -value };
				memcpy(mesh->vertices.data, temp, sizeof(temp));
			}
			mesh->normals.data = (GLfloat*)malloc(mesh->normals.count * sizeof(GLfloat));
			if (mesh->normals.data) {
				float temp[] = { 0,1,0, 0,1,0, 0,1,0, 0,1,0 };
				memcpy(mesh->normals.data, temp, sizeof(temp));
			}
			mesh->uv0.data = (GLfloat*)malloc(mesh->uv0.count * sizeof(GLfloat));
			if (mesh->uv0.data) {
				float temp[] = { 0,0,1,0,1,1,0,1 };
				memcpy(mesh->uv0.data, temp, sizeof(temp));
			}
			mesh->triangles.data = (GLuint*)malloc(mesh->triangles.count * sizeof(GLuint));
			if (mesh->triangles.data) {
				GLuint temp[] = { 0,1,2,0,2,3 };
				memcpy(mesh->triangles.data, temp, sizeof(temp));
			}
		}
		default:
			break;
		}
		if (flip) {
			for (unsigned int i = 0; i < mesh->triangles.count / 3; i++) {
				unsigned int tmp = 0;
				if (mesh->triangles.data) {
					tmp = mesh->triangles.data[i * 3];
					mesh->triangles.data[i * 3] = mesh->triangles.data[i * 3 + 2];
					mesh->triangles.data[i * 3 + 2] = tmp;
				}
			}
		}
		return obj;
	}

	Object* Object::LoadModel(std::string path, std::string name) {
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile("resources/" + path + name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
			return nullptr;
		}
		Object* obj = new Object();
		obj->AddComponent<Render>()->LoadModelMat(scene, path);
		obj->AddComponent<DrawCmdFilter>()->LoadModel(scene);
		return obj;
	}

	///
	// Defines
	//
#define GL_PI  (3.14159265f)

	/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores
	///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
	/// \param numSlices The number of slices in the sphere
	/// \param vertices If not NULL, will contain array of float3 positions
	/// \param normals If not NULL, will contain array of float3 normals
	/// \param texCoords If not NULL, will contain array of float2 texCoords
	/// \param indices If not NULL, will contain the array of indices for the triangle strip
	/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
	///         if it is not NULL ) as a GL_TRIANGLE_STRIP
	//
	int Object::GenSphere(int numSlices, float radius, GLfloat** vertices, GLfloat** normals,
		GLfloat** texCoords, GLuint** indices) {
		int i;
		int j;
		int numParallels = numSlices / 2;
		int numVertices = (numParallels + 1) * (numSlices + 1);
		int numIndices = numParallels * numSlices * 6;
		float angleStep = (2.0f * GL_PI) / ((float)numSlices);

		// Allocate memory for buffers
		if (vertices != NULL) {
			//*vertices = malloc(sizeof(GLfloat) * 3 * numVertices);
			*vertices = new GLfloat[3 * numVertices];
		}

		if (normals != NULL) {
			//*normals = malloc(sizeof(GLfloat) * 3 * numVertices);
			*normals = new GLfloat[3 * numVertices];
		}

		if (texCoords != NULL) {
			//*texCoords = malloc(sizeof(GLfloat) * 2 * numVertices);
			*texCoords = new GLfloat[2 * numVertices];
		}

		if (indices != NULL) {
			//*indices = malloc(sizeof(GLuint) * numIndices);
			*indices = new GLuint[numIndices];
		}

		for (i = 0; i < numParallels + 1; i++) {
			for (j = 0; j < numSlices + 1; j++) {
				int vertex = (i * (numSlices + 1) + j) * 3;

				if (vertices) {
					(*vertices)[vertex + 0] = radius * sinf(angleStep * (float)i) *
						sinf(angleStep * (float)j);
					(*vertices)[vertex + 1] = radius * cosf(angleStep * (float)i);
					(*vertices)[vertex + 2] = radius * sinf(angleStep * (float)i) *
						cosf(angleStep * (float)j);
				}

				if (normals) {
					(*normals)[vertex + 0] = (*vertices)[vertex + 0] / radius;
					(*normals)[vertex + 1] = (*vertices)[vertex + 1] / radius;
					(*normals)[vertex + 2] = (*vertices)[vertex + 2] / radius;
				}

				if (texCoords) {
					int texIndex = (i * (numSlices + 1) + j) * 2;
					(*texCoords)[texIndex + 0] = (float)j / (float)numSlices;
					(*texCoords)[texIndex + 1] = (1.0f - (float)i) / (float)(numParallels - 1);
				}
			}
		}

		// Generate the indices
		if (indices != NULL) {
			GLuint* indexBuf = (*indices);

			for (i = 0; i < numParallels; i++) {
				for (j = 0; j < numSlices; j++) {
					*indexBuf++ = i * (numSlices + 1) + j;
					*indexBuf++ = (i + 1) * (numSlices + 1) + j;
					*indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);

					*indexBuf++ = i * (numSlices + 1) + j;
					*indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);
					*indexBuf++ = i * (numSlices + 1) + (j + 1);
				}
			}
		}

		return numIndices;
	}

	//
	/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores
	///        the results in the arrays.  Generate index list for a TRIANGLES
	/// \param scale The size of the cube, use 1.0 for a unit cube.
	/// \param vertices If not NULL, will contain array of float3 positions
	/// \param normals If not NULL, will contain array of float3 normals
	/// \param texCoords If not NULL, will contain array of float2 texCoords
	/// \param indices If not NULL, will contain the array of indices for the triangle strip
	/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
	///         if it is not NULL ) as a GL_TRIANGLE_STRIP
	//
	int Object::GenCube(float scale, GLfloat** vertices, GLfloat** normals,
		GLfloat** texCoords, GLuint** indices) {
		int i;
		int numVertices = 24;
		int numIndices = 36;

		GLfloat cubeVerts[] =
		{
		   -0.5f, -0.5f, -0.5f,
		   -0.5f, -0.5f,  0.5f,
		   0.5f, -0.5f,  0.5f,
		   0.5f, -0.5f, -0.5f,
		   -0.5f,  0.5f, -0.5f,
		   -0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,
		   -0.5f, -0.5f, -0.5f,
		   -0.5f,  0.5f, -0.5f,
		   0.5f,  0.5f, -0.5f,
		   0.5f, -0.5f, -0.5f,
		   -0.5f, -0.5f, 0.5f,
		   -0.5f,  0.5f, 0.5f,
		   0.5f,  0.5f, 0.5f,
		   0.5f, -0.5f, 0.5f,
		   -0.5f, -0.5f, -0.5f,
		   -0.5f, -0.5f,  0.5f,
		   -0.5f,  0.5f,  0.5f,
		   -0.5f,  0.5f, -0.5f,
		   0.5f, -0.5f, -0.5f,
		   0.5f, -0.5f,  0.5f,
		   0.5f,  0.5f,  0.5f,
		   0.5f,  0.5f, -0.5f,
		};

		GLfloat cubeNormals[] =
		{
		   0.0f, -1.0f, 0.0f,
		   0.0f, -1.0f, 0.0f,
		   0.0f, -1.0f, 0.0f,
		   0.0f, -1.0f, 0.0f,
		   0.0f, 1.0f, 0.0f,
		   0.0f, 1.0f, 0.0f,
		   0.0f, 1.0f, 0.0f,
		   0.0f, 1.0f, 0.0f,
		   0.0f, 0.0f, -1.0f,
		   0.0f, 0.0f, -1.0f,
		   0.0f, 0.0f, -1.0f,
		   0.0f, 0.0f, -1.0f,
		   0.0f, 0.0f, 1.0f,
		   0.0f, 0.0f, 1.0f,
		   0.0f, 0.0f, 1.0f,
		   0.0f, 0.0f, 1.0f,
		   -1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,
		   -1.0f, 0.0f, 0.0f,
		   1.0f, 0.0f, 0.0f,
		   1.0f, 0.0f, 0.0f,
		   1.0f, 0.0f, 0.0f,
		   1.0f, 0.0f, 0.0f,
		};

		GLfloat cubeTex[] =
		{
		   0.0f, 0.0f,
		   0.0f, 1.0f,
		   1.0f, 1.0f,
		   1.0f, 0.0f,
		   1.0f, 0.0f,
		   1.0f, 1.0f,
		   0.0f, 1.0f,
		   0.0f, 0.0f,
		   0.0f, 0.0f,
		   0.0f, 1.0f,
		   1.0f, 1.0f,
		   1.0f, 0.0f,
		   0.0f, 0.0f,
		   0.0f, 1.0f,
		   1.0f, 1.0f,
		   1.0f, 0.0f,
		   0.0f, 0.0f,
		   0.0f, 1.0f,
		   1.0f, 1.0f,
		   1.0f, 0.0f,
		   0.0f, 0.0f,
		   0.0f, 1.0f,
		   1.0f, 1.0f,
		   1.0f, 0.0f,
		};

		// Allocate memory for buffers
		if (vertices != NULL) {
			//*vertices = malloc(sizeof(GLfloat) * 3 * numVertices);
			*vertices = new GLfloat[3 * numVertices];
			memcpy(*vertices, cubeVerts, sizeof(cubeVerts));

			for (i = 0; i < numVertices * 3; i++) {
				(*vertices)[i] *= scale;
			}
		}

		if (normals != NULL) {
			//*normals = malloc(sizeof(GLfloat) * 3 * numVertices);
			*normals = new GLfloat[3 * numVertices];
			memcpy(*normals, cubeNormals, sizeof(cubeNormals));
		}

		if (texCoords != NULL) {
			//*texCoords = malloc(sizeof(GLfloat) * 2 * numVertices);
			*texCoords = new GLfloat[2 * numVertices];
			memcpy(*texCoords, cubeTex, sizeof(cubeTex));
		}


		// Generate the indices
		if (indices != NULL) {
			GLuint cubeIndices[] =
			{
			   0, 2, 1,
			   0, 3, 2,
			   4, 5, 6,
			   4, 6, 7,
			   8, 9, 10,
			   8, 10, 11,
			   12, 15, 14,
			   12, 14, 13,
			   16, 17, 18,
			   16, 18, 19,
			   20, 23, 22,
			   20, 22, 21
			};

			//*indices = malloc(sizeof(GLuint) * numIndices);
			*indices = new GLuint[numIndices];
			memcpy(*indices, cubeIndices, sizeof(cubeIndices));
		}

		return numIndices;
	}

	//
	/// \brief Generates a square grid consisting of triangles.  Allocates memory for the vertex data and stores
	///        the results in the arrays.  Generate index list as TRIANGLES.
	/// \param size create a grid of size by size (number of triangles = (size-1)*(size-1)*2)
	/// \param vertices If not NULL, will contain array of float3 positions
	/// \param indices If not NULL, will contain the array of indices for the triangle strip
	/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
	///         if it is not NULL ) as a GL_TRIANGLES
	//
	int Object::GenSquareGrid(int size, GLfloat** vertices, GLuint** indices) {
		int i, j;
		int numIndices = (size - 1) * (size - 1) * 2 * 3;

		// Allocate memory for buffers
		if (vertices != NULL) {
			int numVertices = size * size;
			float stepSize = (float)size - 1;
			//*vertices = malloc(sizeof(GLfloat) * 3 * numVertices);
			*vertices = new GLfloat[3 * numVertices];

			for (i = 0; i < size; ++i) // row
			{
				for (j = 0; j < size; ++j) // column
				{
					(*vertices)[3 * (j + i * size)] = i / stepSize;
					(*vertices)[3 * (j + i * size) + 1] = j / stepSize;
					(*vertices)[3 * (j + i * size) + 2] = 0.0f;
				}
			}
		}

		// Generate the indices
		if (indices != NULL) {
			//*indices = malloc(sizeof(GLuint) * numIndices);
			*indices = new GLuint[numIndices];

			for (i = 0; i < size - 1; ++i) {
				for (j = 0; j < size - 1; ++j) {
					// two triangles per quad
					(*indices)[6 * (j + i * (size - 1))] = j + (i) * (size);
					(*indices)[6 * (j + i * (size - 1)) + 1] = j + (i) * (size)+1;
					(*indices)[6 * (j + i * (size - 1)) + 2] = j + (i + 1) * (size)+1;

					(*indices)[6 * (j + i * (size - 1)) + 3] = j + (i) * (size);
					(*indices)[6 * (j + i * (size - 1)) + 4] = j + (i + 1) * (size)+1;
					(*indices)[6 * (j + i * (size - 1)) + 5] = j + (i + 1) * (size);
				}
			}
		}

		return numIndices;
	}

	Object::~Object() {
		((UserData*)context->userData)->allObjects->erase(GetInstanceID());
		for (auto& it : compenents) {
			delete it.second;
		}
		//for (unsigned int i = 0; i < compenents.size(); i++)
		//	delete compenents[i];
		DEBUGLOG("~Object");
	}
}