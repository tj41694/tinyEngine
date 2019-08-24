#pragma once
#include <string>

namespace TEngine {
	class Texture {
	public:
		unsigned int id;
		std::string path;
		Texture();
		virtual ~Texture();
	};
}