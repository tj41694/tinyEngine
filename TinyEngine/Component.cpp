#pragma once
#include "Component.h"
#include "Debug.h"

namespace TEngine {
	using namespace std;

	Component::Component() : obj(nullptr) {
	}

	Component::~Component() {
	}
	void Component::Start() {}
	void Component::Update() {}
}