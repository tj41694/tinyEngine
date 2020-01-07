#include "SelfRot.h"
#include "Engine/Object.h"
#include "Engine/Input.h"
#include "Tools/Debug.h"
#include "Engine/Components/Transform.h"

SelfRot::SelfRot() :Script()
{
	speed = 1.0f;
}

void SelfRot::Start()
{
}


void SelfRot::Update()
{
	//obj->Trans()->Rotate(glm::vec3(0, speed * Input::deltaTime, 0));
}

SelfRot::~SelfRot()
{
}