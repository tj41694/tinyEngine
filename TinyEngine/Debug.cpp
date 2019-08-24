#pragma once
#include "Debug.h"
#include <codecvt>

namespace TEngine {
	void Debug::Log(const std::string& obj) {
		std::cout << obj << std::endl;
	}

	void Debug::Log(const std::wstring & obj) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string str = converter.to_bytes(obj);
		std::cout << str << std::endl;
	}
	
	void Debug::Log(const char* chars) {
		std::cout << chars;
	}

	void Debug::Log(short obj) {
		std::cout << obj << std::endl;
	}

	void Debug::Log(int obj) {
		std::cout << obj << std::endl;
	}

	void Debug::Log(unsigned int obj) {
		std::cout << obj << std::endl;
	}

	void Debug::Log(unsigned long obj)
	{
		std::cout << obj << std::endl;
	}

	void Debug::Log(unsigned long long obj)
	{
		std::cout << obj << std::endl;
	}

	void Debug::Log(float obj) {
		std::cout << obj << std::endl;
	}

	void Debug::Log(double obj) {
		std::cout << obj << std::endl;
	}

	void Debug::Log(bool obj)
	{
#ifdef _DEBUG
		if (obj) {
			std::cout << "true" << std::endl;
		} else {
			std::cout << "false" << std::endl;
		}
#endif // DEBUG
	}

	void Debug::Log(const type_info& type) {
#ifdef _DEBUG
		std::cout << type.name() << std::endl;
#endif // DEBUG
	}
	void Debug::Log(const glm::vec3 & vec3) {
		std::cout << "x:" << vec3.x << " y:" << vec3.y << " z:" << vec3.z << std::endl;
	}
	void Debug::Log(const void *& ptr) {
		std::cout << ptr << std::endl;
	}
}