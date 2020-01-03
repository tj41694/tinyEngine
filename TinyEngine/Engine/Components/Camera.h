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
		float nearPlan			= 0.1f;
		float farPlan			= 200.0f;
		GLuint width			= 512;
		GLuint height			= 512;

	private:
		static std::map<unsigned int, Camera*> cameras;
		glm::mat4 projectionMatrix;

	public:

		Camera();
		void Rend(glContext* esContext);
		glm::mat4 GetViewMatrix();
		glm::mat4& GetProjectionMatrix();
		glm::vec3 ScreenToWorldPoint(const glm::vec2 &);

		virtual ~Camera();
		virtual void Start() override;

		static void RenderAll(glContext* esContext);
	};
}