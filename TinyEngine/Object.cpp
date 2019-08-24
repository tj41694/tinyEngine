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
	unsigned int Object::instanceid = 1;
	ESContext* Object::context;

	Object::Object() {
		id = 0;
		((UserData*)context->userData)->objs->insert(std::pair<unsigned int, Object*>(GetInstanceID(), this));
		localPositon = vec3(0.0f);
		scale = vec3(1.0f);
		rotation = quat();
		eulerAngles = vec3(0.0f);
		parent = 0;
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
		return (int)childs.size();
	}

	Object* Object::Child(unsigned int index) {
		if (childs.size() > index)
			((UserData*)context->userData)->objs->at(childs[index]);
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
		meshFilter->meshes.push_back(mesh);
		switch (shape) {
		case Shape::cube:
		{
			mesh->vertices.count = mesh->normals.count = 24 * 3;
			mesh->uv0.count = 24 * 2;
			mesh->triangles.count = esGenCube(size, &mesh->vertices.data, &mesh->normals.data, &mesh->uv0.data, &mesh->triangles.data);
			break;
		}
		case Shape::sphere:
		{
			int slice = 48;
			int numVertices = (slice / 2 + 1) * (slice + 1);
			mesh->vertices.count = mesh->normals.count = numVertices * 3;
			mesh->uv0.count = numVertices * 2;
			mesh->triangles.count = esGenSphere(slice, size, &mesh->vertices.data, &mesh->normals.data, &mesh->uv0.data, &mesh->triangles.data);
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
				float temp[] = { 0,1,0,0,1,0, 0,1,0, 0,1,0 };
				memcpy(mesh->normals.data, temp, sizeof(temp));
			}
			mesh->uv0.data = (GLfloat*)malloc(mesh->uv0.count * sizeof(GLfloat));
			if (mesh->uv0.data) {
				float temp[] = { 0,0,0,1,1,1,1,0 };
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
		const aiScene* scene = import.ReadFile(path + name, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
			return nullptr;
		}
		Object* obj = new Object();
		Render* render = obj->AddComponent<Render>();
		render->LoadModelMat(scene, path);
		MeshFilter* meshFilter = obj->AddComponent<MeshFilter>();
		meshFilter->LoadModel(scene);
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


	Object::~Object() {
		((UserData*)context->userData)->objs->erase(GetInstanceID());
		for (unsigned int i = 0; i < compenents.size(); i++)
			delete compenents[i];
		Debug::Log("~Object\n");
	}
}