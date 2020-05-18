#pragma once
#include "Debug.h"
#include <codecvt>
#include <stdio.h>
#include <iostream>
#include <stdio.h>

//#define DEBUG_FILE_LINE
namespace TEngine {

	using namespace std;
	void Log(const char* file, int line, const string& obj) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << obj << endl;
#else
		cout << obj << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, const wstring& obj) {
		wstring_convert<codecvt_utf8<wchar_t>> converter;
		string str = converter.to_bytes(obj);
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << str << endl;
#else
		cout << str << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, const char* chars) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << chars << endl;
#else
		cout << chars << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, short obj) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << obj << endl;
#else
		cout << obj << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, int obj) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << obj << endl;
#else
		cout << obj << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, unsigned int obj) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << obj << endl;
#else
		cout << obj << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, unsigned long obj) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << obj << endl;
#else
		cout << obj << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, unsigned long long obj) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << obj << endl;
#else
		cout << obj << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, float obj) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << obj << endl;
#else
		cout << obj << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, double obj) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << obj << endl;
#else
		cout << obj << endl;
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, bool obj) {
#ifdef DEBUG_FILE_LINE
		if (obj) {
			cout << file << " Line: " << line << "\t" << "true" << endl;
		}
		else {
			cout << file << " Line: " << line << "\t" << "false" << endl;
		}
#else
		if (obj) {
			cout << "true" << endl;
		}
		else {
			cout << "false" << endl;
		}
#endif // DEBUG_FILE_LINE
	}

	void Log(const char* file, int line, const type_info& type) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << type.name() << endl;
#else
		cout << type.name() << endl;
#endif // DEBUG_FILE_LINE
	}
	void Log(const char* file, int line, const glm::vec2& vec2)
	{
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << "x:" << vec2.x << " y:" << vec2.y << endl;
#else
		cout << "x:" << vec2.x << " y:" << vec2.y << endl;
#endif // DEBUG_FILE_LINE
	}
	void Log(const char* file, int line, const glm::vec3& vec3) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << "x:" << vec3.x << " y:" << vec3.y << " z:" << vec3.z << endl;
#else
		cout << "x:" << vec3.x << " y:" << vec3.y << " z:" << vec3.z << endl;
#endif // DEBUG_FILE_LINE
	}
	void Log(const char* file, int line, const void*& ptr) {
#ifdef DEBUG_FILE_LINE
		cout << file << " Line: " << line << "\t" << ptr << endl;
#else
		cout << ptr << endl;
#endif // DEBUG_FILE_LINE
	}
}