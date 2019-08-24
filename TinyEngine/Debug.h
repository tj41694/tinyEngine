#pragma once
#include <iostream>
#include <string>
#include "glm/gtc/matrix_transform.hpp"

namespace TEngine {
	class Debug {
	public:
		static void Log(const std::string& obj);
		static void Log(const std::wstring& obj);
		static void Log(const char * chars);
		static void Log(short obj);
		static void Log(int obj);
		static void Log(unsigned int obj);
		static void Log(unsigned long obj);
		static void Log(unsigned long long obj);
		static void Log(float obj);
		static void Log(double obj);
		static void Log(bool obj);
		static void Log(const type_info & type);
		static void Log(const glm::vec3 & vec3);
		static void Log(const void* & ptr);
	};
}