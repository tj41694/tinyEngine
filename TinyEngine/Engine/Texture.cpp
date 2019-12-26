#include "Texture.h"


namespace TEngine {

	Texture::Texture() : id(0) {
	}

	Texture::Texture(const char* path_) : id(0) {
		path.assign(path_);
	}

	Texture::Texture(std::string path_) : id(0) {
		path.assign(path_);
	}

	Texture::~Texture() {
	}
}