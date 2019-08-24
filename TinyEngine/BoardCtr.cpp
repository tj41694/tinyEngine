#include "BoardCtr.h"
#include "Object.h"
#include "Render.h"
#include "Standard.h"
#include "Camera.h"

BoardCtr::BoardCtr(Object* obj) : Script(obj)
{
}

void BoardCtr::Start()
{
	Render* render = obj->GetComponent<Render>();
	for (unsigned int i = 0; i < render->materials.size(); i++)
	{
		Standard* mat = dynamic_cast<Standard*>(render->materials.at(i));
		if (mat) mat->diffuseColor = vec3(0.9f);
	}
}

void BoardCtr::Update()
{
	obj->RotateTo(obj->LookAt(Camera::main->obj->WorldPos() - obj->WorldPos(), vec3(0, 1, 0)));
}

BoardCtr::~BoardCtr()
{
}
