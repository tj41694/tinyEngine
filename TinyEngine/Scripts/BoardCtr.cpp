#include "BoardCtr.h"
#include "Engine/Object.h"
#include "Engine/Components/Render.h"
#include "Engine/Materials/Standard.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Transform.h"

BoardCtr::BoardCtr() : Script()
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
	obj->Trans()->RotateTo(obj->Trans()->LookAt(Camera::main->obj->Trans()->Positon() - obj->Trans()->Positon(), vec3(0, 1, 0)));
}

BoardCtr::~BoardCtr()
{
}
