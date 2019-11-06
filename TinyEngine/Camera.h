#pragma once
#include "OpenglUtil.h"
#include "Component.h"
#include "glm/glm.hpp"
#include <map>

namespace TEngine {
	using namespace glm;
	class Camera : public Component {
	private:
		static std::map<unsigned int, Camera*> cameras;
		void Rend(glContext*esContext);
	public:
		static Camera* main;
		GLuint renderTarget;
		float exposure;
		float Zoom;
		float nearPlan;
		float farPlan;
		unsigned int width;
		unsigned int height;
		Camera(Object* obj_);
		static void RenderAll(glContext*esContext);
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
		virtual ~Camera();
	};
}