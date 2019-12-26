#pragma once
#include <string>

namespace TEngine {
	class Texture {
	public:
		unsigned int id;
		std::string path;
		Texture();
		Texture(const char* path_);
		Texture(std::string path);
		virtual ~Texture();
	};
}