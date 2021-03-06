#pragma once
#include <string>

namespace TEngine {
	class FileSystem {
	public:
		static unsigned char * LoadTexture(const char* filename,  int *x, int *y, int *comp, int req_comp);
		static void FreeTextur(unsigned char * tex);
		static void SaveTexture();
		static std::string ReadFile(const char* path);
	};

}