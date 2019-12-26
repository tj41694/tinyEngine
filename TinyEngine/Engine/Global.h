#pragma once
#include <string>
#include <vector>

namespace TEngine {
	using namespace std;
	class Global {
	private:
		static unsigned int loadCubemap(vector<string> faces);
	public:
		static unsigned int skyboxTexture;
		static unsigned int twoImageFramebuffer;
		static unsigned int screenShader, shaderBlur;
		static unsigned int quadVAO;
		static unsigned int texColorBuffer[2];
		static unsigned int pingpongFBO[2];
		static unsigned int pingpongBuffer[2];
		static void Initial();
	};
}