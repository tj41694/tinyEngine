#pragma once
#include "Component.h"
namespace TEngine {
	class Camera;
	class ReflectProb : public Component {
	private:
		Camera* cameras[6];
		unsigned int framebuffer[6];
	public:
		unsigned int texSize;
		unsigned int cubeMap;
		ReflectProb(Object* obj_);
		virtual ~ReflectProb();
	};
}

