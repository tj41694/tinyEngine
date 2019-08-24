#pragma once
#include "Component.h"
#include "Debug.h"

namespace TEngine {
	using namespace std;

	Component::Component(Object* obj) : obj(obj) {
	}

	void Component::CreateVtable() {
	}

	Component::~Component() {
	}
}