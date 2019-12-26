#include "Terrain.h"
#include "Engine/External/MyTif.h"
#include "Engine/Object.h"
#include "Engine/Components/MeshFilter.h"
#include "Engine/Components/Render.h"
#include "Engine/DrawCommand/Mesh.h"

namespace TEngine {
	static const float mapUnit = 92.66f;
	static const float displayScale = 0.1f;
	Object* Terrain::Create(MyTif& tif) {
		Object* obj = new Object();
		MeshFilter* meshFilter = obj->AddComponent<MeshFilter>();
		Render* render = obj->AddComponent<Render>();
		Mesh* mesh = new Mesh();
		mesh->vertices.count = mesh->normals.count = tif.height * tif.width * 3;
		mesh->uv0.count = tif.height * tif.width * 2;
		mesh->triangles.count = (tif.height - 1) * (tif.width - 1) * 6;
		mesh->vertices.data = (GLfloat*)malloc(mesh->vertices.count * sizeof(GLfloat));
		mesh->normals.data = (GLfloat*)malloc(mesh->normals.count * sizeof(GLfloat));
		mesh->uv0.data = (GLfloat*)malloc(mesh->uv0.count * sizeof(GLfloat));
		mesh->triangles.data = (GLuint*)malloc(mesh->triangles.count * sizeof(GLuint));

		const unsigned short* raster = tif.GetRasterData();
		const unsigned short* valPtr = raster;

		GLuint vIndex = 0;
		GLuint uvIndex = 0;
		GLuint indexIndex = 0;
		for (int i = 0; i < tif.height; i++) {
			unsigned short value = 0;
			float zPos = mapUnit * (tif.height - i - 1);
			for (int j = 0; j < tif.width; j++) {
				float xPos = mapUnit * j;
				value = *(valPtr++);
				mesh->vertices.data[vIndex] = xPos * displayScale;
				mesh->normals.data[vIndex++] = 0;

				mesh->vertices.data[vIndex] = value * displayScale;
				mesh->normals.data[vIndex++] = 1;

				mesh->vertices.data[vIndex] = zPos * displayScale;
				mesh->normals.data[vIndex++] = 0;

				mesh->uv0.data[uvIndex++] = j / (float)(tif.width - 1.0f);
				mesh->uv0.data[uvIndex++] = (tif.height - i - 1) / (float)(tif.height - 1.0f);

				if (j != tif.width - 1 && i != tif.height - 1) {

					mesh->triangles.data[indexIndex++] = (i * tif.width) + j;
					mesh->triangles.data[indexIndex++] = (i * tif.width) + j + 1;
					mesh->triangles.data[indexIndex++] = (i * tif.width) + j + tif.width;
					mesh->triangles.data[indexIndex++] = (i * tif.width) + j + 1;
					mesh->triangles.data[indexIndex++] = (i * tif.width) + j + 1 + tif.width;
					mesh->triangles.data[indexIndex++] = (i * tif.width) + j + tif.width;
				}
			}
		}
		//mesh->RecalculateNormals();
		meshFilter->drawCmds.push_back(mesh);
		tif.ClearRasterData();
		return obj;
	}

	void Terrain::GetSeqPoint(MyTif& tif, std::vector<glm::vec3>& out_points) {
		const unsigned short* raster = tif.GetRasterData();
		const unsigned short* valPtr = raster;
		for (int i = 0; i < tif.height; i++) {
			unsigned short value = 0;
			float zPos = mapUnit * (tif.height - i - 1);
			for (int j = 0; j < tif.width; j++) {
				float xPos = mapUnit * j;
				value = *(valPtr++);
				out_points.emplace_back(glm::vec3(xPos * displayScale, value * displayScale, zPos * displayScale));
			}
		}
	}
}