#include "Object.h"
#include "Render.h"
#include "Mesh.h"
#include "MeshFilter.h"
#include "DataDef.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"
//#include "glm/gtc/quaternion.hpp"
//#include "glm/gtx/quaternion.hpp"

namespace TEngine {
	using namespace glm;
	using namespace std;
	unsigned int Object::instanceid = 1;
	glContext* Object::context;

	Object::Object() {
		((UserData*)context->userData)->objs->insert(std::pair<unsigned int, Object*>(GetInstanceID(), this));
		localPositon = vec3(0.0f);
		scale = vec3(1.0f);
		rotation = quat();
		eulerAngles = vec3(0.0f);
	}

	Object::Object(const char* name_) {
		id = 0;
		((UserData*)context->userData)->objs->insert(std::pair<int, Object*>(GetInstanceID(), this));
		localPositon = vec3(0.0f);
		scale = vec3(1.0f);
		rotation = quat();
		eulerAngles = vec3(0.0f);
		parent = 0;
		name.assign(name_);
		Debug::Log("Create Object:" + name);
	}

	mat4 Object::LocalToWorldMarix() {
		mat4 unit(1.0f);
		mat4 molel = translate(unit, localPositon) * mat4_cast(rotation) * glm::scale(unit, scale);
		if (parent == 0) {
			return molel;
		}
		else {
			return Parent()->LocalToWorldMarix() * molel;
		}
	}

	//世界坐标
	vec3 Object::WorldPos() {
		if (parent == 0)
			return localPositon;
		else
			return Parent()->WorldPos() + (Parent()->rotation * localPositon) * Parent()->scale;
	}

	vec3 Object::LocalPos() const {
		return localPositon;
	}

	quat Object::Rotation() const {
		if (parent == 0)
			return rotation;
		else
			return Parent()->Rotation() * rotation;
	}

	quat Object::LocalRotation() {
		return rotation;
	}

	vec3 Object::EulerAngles() {
		return eulerAngles;
	}

	vec3 Object::Forwward() const {
		return Rotation() * vec3(0, 0, 1.0f);
	}

	vec3 Object::Right() const {
		return Rotation() * vec3(1.0f, 0, 0);
	}

	vec3 Object::Up() const {
		return Rotation() * vec3(0, 1.0f, 0);
	}

	void Object::MoveTo(const float& x, const float& y, const float& z) {
		localPositon = vec3(x, y, z);
	}

	void Object::MoveTo(const vec3& vec) {
		localPositon.x = vec.x;
		localPositon.y = vec.y;
		localPositon.z = vec.z;
	}

	void Object::Move(const float& x, const float& y, const float& z) {
		localPositon += vec3(x, y, z);
	}

	void Object::Move(const vec3& vec) {
		localPositon += vec;
	}

	void Object::RotateTo(const vec3& eulerAngles_) {
		rotation = quat(eulerAngles_);
		eulerAngles = glm::eulerAngles(rotation);
	}

	void Object::RotateTo(const float& x, const float& y, const float& z) {
		rotation = quat(vec3(x, y, z));
		eulerAngles = glm::eulerAngles(rotation);
	}

	void Object::RotateTo(const quat& q) {
		rotation = q;
		eulerAngles = glm::eulerAngles(rotation);
	}

	void Object::Rotate(const vec3& eulerAngles_) {
		rotation = quat(eulerAngles_) * rotation;
		eulerAngles = glm::eulerAngles(rotation);
	}

	void Object::Rotate(const vec3& axis, const float value) {
		rotation = glm::angleAxis(value, axis) * rotation;
		eulerAngles = glm::eulerAngles(rotation);
	}

	Object* Object::Parent() const {
		if (parent == 0) return nullptr;
		return ((UserData*)context->userData)->objs->at(parent);
	}

	void Object::SetParent(Object* obj_, bool worldPositionStays) {
		if (worldPositionStays) {
			localPositon = localPositon - obj_->WorldPos();
		}
		parent = obj_->GetInstanceID();
		Parent()->childs.push_back(GetInstanceID());
	}

	unsigned int Object::ChildCount() {
		return (unsigned int)childs.size();
	}

	Object* Object::Child(unsigned int index) {
		if (childs.size() > index) {
			return ((UserData*)context->userData)->objs->at(childs[index]);
		}
		return nullptr;
	}

	unsigned int Object::GetInstanceID() {
		if (id == 0) id = instanceid++;
		return id;
	}

	Object* Object::CreateShape(Shape shape, float size, bool flip) {
		Object* obj = new Object();
		MeshFilter* meshFilter = obj->AddComponent<MeshFilter>();
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
		obj->AddComponent<MeshFilter>()->LoadModel(scene);
		return obj;
	}

	quat Object::RotationBetweenVectors(vec3 start, vec3 dest) {
		start = normalize(start);
		dest = normalize(dest);

		float cosTheta = dot(start, dest);
		vec3 rotationAxis;

		if (cosTheta < -1 + 0.000001f) {
			// special case when vectors in opposite directions :
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			// This implementation favors a rotation around the Up axis,
			// since it's often what you want to do.
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);
			if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
				rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);

			rotationAxis = normalize(rotationAxis);
			return angleAxis(glm::radians(180.0f), rotationAxis);
		}

		// Implementation from Stan Melax's Game Programming Gems 1 article
		rotationAxis = cross(start, dest);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return quat(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);

	}

	quat Object::LookAt(vec3 direction, vec3 desiredUp) {
		if (length2(direction) < 0.0001f)
			return rotation;
		//重新计算目标方向
		direction = Parent() == nullptr ? direction : glm::inverse(Parent()->Rotation()) * direction;

		quat rot1 = RotationBetweenVectors(vec3(0, 0, 1), direction);

		vec3 right = cross(direction, desiredUp);
		desiredUp = cross(right, direction);

		vec3 newUp = rot1 * vec3(0.0f, 1.0f, 0.0f);
		quat rot2 = RotationBetweenVectors(newUp, desiredUp);

		return  rot2 * rot1;
	}


	quat Object::RotateTowards(quat q1, quat q2, float maxAngle) {

		if (maxAngle < 0.001f) {
			// No rotation allowed. Prevent dividing by 0 later.
			return q1;
		}

		float cosTheta = dot(q1, q2);

		// q1 and q2 are already equal.
		// Force q2 just to be sure
		if (cosTheta > 0.9999f) {
			return q2;
		}

		// Avoid taking the long path around the sphere
		if (cosTheta < 0) {
			q1 = q1 * -1.0f;
			cosTheta *= -1.0f;
		}

		float angle = acos(cosTheta);

		// If there is only a 2° difference, and we are allowed 5°,
		// then we arrived.
		if (angle < maxAngle) {
			return q2;
		}

		// This is just like slerp(), but with a custom t
		float t = maxAngle / angle;
		angle = maxAngle;

		quat res = (sin((1.0f - t) * angle) * q1 + sin(t * angle) * q2) / sin(angle);
		res = normalize(res);
		return res;

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
		((UserData*)context->userData)->objs->erase(GetInstanceID());
		for (unsigned int i = 0; i < compenents.size(); i++)
			delete compenents[i];
		Debug::Log("~Object\n");
	}
}