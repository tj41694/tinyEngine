#pragma once
struct GLContext;
namespace TEngine {
	class Text;
	class Global {
	public:
		static unsigned int skyboxTexture;
		static unsigned int twoImageFramebuffer;
		static unsigned int screenShader, shaderBlur;
		static unsigned int screenVAO;
		static unsigned int texColorBuffer[2];
		static unsigned int pingpongFBO[2];
		static unsigned int pingpongBuffer[2];
		static Text* FPSText;
		static Text* FPSText1;
		static void Initial(glContext* glContext);
		static void Update(double deltaTime);

	private:
		static void InitialSkybox();
		static void InitialScence(glContext* glContext);
		static void InitialUI();
		static void GenTerrain();
	};
}