#pragma once
#include "Script.h"

class SelfRot : public Script {
public:
	float speed;
	SelfRot(Object* obj);
	void Start();
	void Update();
	virtual ~SelfRot();
};