#pragma once
#include <string>

namespace TEngine {
	class FileSystem {
	public:
		static unsigned char * LoadTexture(const std::string& filename,  int *x, int *y, int *comp, int req_comp);
		static void FreeTextur(unsigned char * tex);
		static std::string ReadFile(const char* path);
	};

}