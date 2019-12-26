#pragma once
#include "Engine/OpenglUtil.h"
#include "Component.h"
#include "glm/glm.hpp"
#include <map>

namespace TEngine {
	class Camera : public Component {
	public:

		static Camera* main;

		GLuint renderTarget		= 0;
		float exposure			= 1.0f;
		float Zoom				= 45.0f;
		float nearPlan			= 1.0f;
		float farPlan			= 1000000.0f;
		unsigned int width		= 512;
		unsigned int height		= 512;

	private:
		static std::map<unsigned int, Camera*> cameras;

		void Rend(glContext* esContext);

	public:
		static void RenderAll(glContext* esContext);

		Camera();
		void Start() override;
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
		virtual ~Camera();
	};
}