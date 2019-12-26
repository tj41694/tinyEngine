#pragma once
#include "FileSystem.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Tools/Debug.h"
#include <fstream>
#include <sstream>

namespace TEngine {
	unsigned char* FileSystem::LoadTexture(const char* filename, int* x, int* y, int* comp, int req_comp) {
		static char path[128] = { 0 };
		strcpy(path, "resources/");
		strcat(path, filename);
		return stbi_load(path, x, y, comp, req_comp);
	}
	void FileSystem::FreeTextur(unsigned char* tex) {
		stbi_image_free(tex);
	}
	std::string FileSystem::ReadFile(const char* path) {
		try {
			std::string shaderCode;
			std::ifstream shaderFile;
			shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // 保证ifstream对象可以抛出异常：
			shaderFile.open(path);
			std::stringstream sStream;
			sStream << shaderFile.rdbuf();
			shaderFile.close();
			shaderCode = sStream.str();
			return shaderCode;
		}
		catch (std::ifstream::failure e) {
			DEBUGLOG("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
		}
		return nullptr;
	}
}