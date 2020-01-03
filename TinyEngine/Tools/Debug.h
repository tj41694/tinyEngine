#pragma once
#include <string>
#include "glm/gtc/matrix_transform.hpp"
namespace TEngine {
	void Log(const char* file, int line, const std::string& obj);
	void Log(const char* file, int line, const std::wstring& obj);
	void Log(const char* file, int line, const char* chars);
	void Log(const char* file, int line, short obj);
	void Log(const char* file, int line, int obj);
	void Log(const char* file, int line, unsigned int obj);
	void Log(const char* file, int line, unsigned long obj);
	void Log(const char* file, int line, unsigned long long obj);
	void Log(const char* file, int line, float obj);
	void Log(const char* file, int line, double obj);
	void Log(const char* file, int line, bool obj);
	void Log(const char* file, int line, const type_info& type);
	void Log(const char* file, int line, const glm::vec2& vec2);
	void Log(const char* file, int line, const glm::vec3& vec3);
	void Log(const char* file, int line, const void*& ptr);

#ifdef _DEBUG
#define DEBUGLOG(info) Log(__FILE__, __LINE__, info)
#else
#define DEBUGLOG(info)
#endif
}