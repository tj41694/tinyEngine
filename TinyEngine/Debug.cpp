#pragma once
#include "Debug.h"
#include <codecvt>

namespace TEngine {
	using namespace std;
	void Debug::Log(const string& obj) {
		cout << obj << endl;
	}

	void Debug::Log(const wstring & obj) {
		wstring_convert<codecvt_utf8<wchar_t>> converter;
		string str = converter.to_bytes(obj);
		cout << str << endl;
	}
	
	void Debug::Log(const char* chars) {
		cout << chars;
	}

	void Debug::Log(short obj) {
		cout << obj << endl;
	}

	void Debug::Log(int obj) {
		cout << obj << endl;
	}

	void Debug::Log(unsigned int obj) {
		cout << obj << endl;
	}

	void Debug::Log(unsigned long obj)
	{
		cout << obj << endl;
	}

	void Debug::Log(unsigned long long obj)
	{
		cout << obj << endl;
	}

	void Debug::Log(float obj) {
		cout << obj << endl;
	}

	void Debug::Log(double obj) {
		cout << obj << endl;
	}

	void Debug::Log(bool obj)
	{
#ifdef _DEBUG
		if (obj) {
			cout << "true" << endl;
		} else {
			cout << "false" << endl;
		}
#endif // DEBUG
	}

	void Debug::Log(const type_info& type) {
#ifdef _DEBUG
		cout << type.name() << endl;
#endif // DEBUG
	}
	void Debug::Log(const glm::vec3 & vec3) {
		cout << "x:" << vec3.x << " y:" << vec3.y << " z:" << vec3.z << endl;
	}
	void Debug::Log(const void *& ptr) {
		cout << ptr << endl;
	}
}