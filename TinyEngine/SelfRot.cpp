#include "SelfRot.h"
#include "Object.h"
#include "Input.h"
#include "Debug.h"

SelfRot::SelfRot(Object* obj) :Script(obj)
{
	speed = 1.0f;
}

void SelfRot::Start()
{
}


void SelfRot::Update()
{
	obj->Rotate(vec3(0, speed * Input::deltaTime, 0));
}

SelfRot::~SelfRot()
{
}