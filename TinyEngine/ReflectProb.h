#pragma once
#include "Component.h"
namespace TEngine {
	class Camera;
	class ReflectProb : public Component {
	public:
		unsigned int texSize;
		unsigned int cubeMap;
	public:
		ReflectProb();
		void Start() override;
		virtual ~ReflectProb();
	private:
		Camera* cameras[6];
		unsigned int framebuffer[6];
	};
}

