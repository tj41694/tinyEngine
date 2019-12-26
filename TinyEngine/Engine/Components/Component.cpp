#pragma once
#include "Component.h"

namespace TEngine {
	Component::Component() : obj(nullptr) {}
	Component::~Component() {}
	void Component::Start() {}
	void Component::Update() {}
}