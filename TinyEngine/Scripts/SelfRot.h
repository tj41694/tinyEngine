#pragma once
#include "Engine/Components/Script.h"

using namespace TEngine;
class SelfRot : public Script {
public:
	float speed;

public:
	SelfRot();
	void Start();
	void Update();
	virtual ~SelfRot() override;
};