#pragma once
#include <unordered_map>

namespace TEngine {
	class Object;
	struct UserData {
		std::unordered_map<unsigned int, Object*>* allObjects;
	};
}