#include "BeltStrip.h"
#include "Engine/Components/DrawCmdFilter.h"
#include "Engine/DrawCommand/Mesh.h"
#include "Engine/Components/Render.h"
#include "Engine/Texture.h"
#include "Engine/Object.h"
#include "Tools/Polygon.h"
#include "Engine/Materials/BeltStripMat.h"
#include "Engine/DrawCommand/DrawCmdBelt.h"

using namespace std;
using namespace TEngine;
BeltStrip::BeltStrip() {
	obj = new Object("beltStrip_0");
	DrawCmdFilter* meshFilter = obj->AddComponent<DrawCmdFilter>();
	Render* render = obj->AddComponent<Render>();
	BeltStripMat* material = new BeltStripMat();
	render->materials.push_back(material);
	material->diffuseMap = new Texture("textures\\HmsLog.jpg");
	material->diffuseColor = glm::vec3(0.6f, 0.9f, 0.9f);
}

void BeltStrip::DrawBeltStrip(const std::vector<glm::vec3>& pointSrrip, float width, glm::vec2 texSize) {
	DrawCmdFilter* meshFilter = obj->GetComponent<DrawCmdFilter>();
	DrawCmdBelt* cmdBelt = new DrawCmdBelt();
	meshFilter->drawCmds.push_back(cmdBelt);
	cmdBelt->vertices.count = pointSrrip.size() * 3;
	cmdBelt->vertices.data = new float[cmdBelt->vertices.count];
	memcpy(cmdBelt->vertices.data, pointSrrip.data(), cmdBelt->vertices.count * sizeof(float));
}

void BeltStrip::DrawBeltStrip_Mesh(const std::vector<glm::vec3>& pointSrrip, float width, glm::vec2 texSize) {
	DrawCmdFilter* meshFilter = obj->GetComponent<DrawCmdFilter>();
	Mesh* mesh = new Mesh();
	meshFilter->drawCmds.push_back(mesh);
	if (mesh == nullptr) { return; }
	mesh->Clear();

	vector<glm::vec3> offsetPolygon;
	TPolygon::Offset(pointSrrip, width, offsetPolygon);
	size_t vertexCount = pointSrrip.size();
	float yTile = width / texSize.x;

	mesh->vertices.count = (vertexCount - 1) * 4 * 3;
	mesh->vertices.data = new float[mesh->vertices.count];
	glm::vec3* veteces = (glm::vec3*)mesh->vertices.data;

	mesh->normals.count = mesh->vertices.count;
	mesh->normals.data = new float[mesh->normals.count];
	for (size_t i = 0; i < mesh->normals.count; i += 3) {
		memcpy(mesh->normals.data + i, &glm::vec3(0, 1, 0), sizeof(glm::vec3));
	}

	mesh->triangles.count = (vertexCount - 1) * 6;
	mesh->triangles.data = new GLuint[mesh->triangles.count];
	GLuint* indexs = (GLuint*)mesh->triangles.data;

	mesh->uv0.count = (vertexCount - 1) * 4 * 2;
	mesh->uv0.data = new float[mesh->uv0.count];
	glm::vec2* uv = (glm::vec2*)mesh->uv0.data;

	float* lens = new float[vertexCount];
	float* dotVal = new float[vertexCount];
	for (size_t i = 0; i < vertexCount; i++) {
		if (i == 0) {
			lens[0] = 0;
			dotVal[0] = glm::dot(offsetPolygon[i] - pointSrrip[i], pointSrrip[i + 1] - pointSrrip[i]) / glm::length(pointSrrip[i + 1] - pointSrrip[i]);
		}
		else {
			lens[i] = lens[i - 1] + glm::length(pointSrrip[i] - pointSrrip[i - 1]);
			dotVal[i] = glm::dot(offsetPolygon[i] - pointSrrip[i], pointSrrip[i - 1] - pointSrrip[i]) / glm::length(pointSrrip[i - 1] - pointSrrip[i]);
		}
	}

	size_t count = 0;
	size_t index = 0;
	for (size_t i = 0; i < vertexCount - 1; i++) {
		veteces[i * 4] = pointSrrip[i];
		veteces[i * 4 + 1] = pointSrrip[i + 1];
		veteces[i * 4 + 2] = offsetPolygon[i];
		veteces[i * 4 + 3] = offsetPolygon[i + 1];

		uv[i * 4 + 0] = glm::vec2(lens[i] / texSize.y, 0);
		uv[i * 4 + 1] = glm::vec2(lens[i + 1] / texSize.y, 0);
		uv[i * 4 + 2] = glm::vec2((lens[i] + dotVal[i]) / texSize.y, yTile);
		uv[i * 4 + 3] = glm::vec2((lens[i + 1] - dotVal[i + 1]) / texSize.y, yTile);

		indexs[index++] = 0 + count;
		indexs[index++] = 2 + count;
		indexs[index++] = 3 + count;
		indexs[index++] = 0 + count;
		indexs[index++] = 3 + count;
		indexs[index++] = 1 + count;

		count += 4;
	}

	delete[] lens;
	delete[] dotVal;
}


BeltStrip::~BeltStrip() {
	delete obj;
}

