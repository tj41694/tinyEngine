#pragma once
#include "Component.h"
#include "glm/glm.hpp"
#include <map>

struct GLContext;
namespace TEngine {
	class Camera : public Component {
	public:
		static Camera* main;

		unsigned int	renderTarget = 0;
		float			exposure = 1.0f;
		float			Zoom = 45.0f;
		float			nearPlan = 0.1f;
		float			farPlan = 1000.0f;
		unsigned int	width = 512;
		unsigned int	height = 512;
		bool			useScreenFrameBuffer = false;
		int             rendTextIndex = 0;
		bool			drawFrame = false;

	private:
		static std::map<unsigned int, Camera*> cameras;
		glm::mat4 projectionMatrix;

	public:

		Camera();
		void Rend(glContext* esContext);
		glm::mat4 GetViewMatrix();
		glm::mat4& GetProjectionMatrix();
		glm::vec3 ScreenToWorldPoint(const glm::vec2&, bool readFromScreenBuffer = true);

		virtual ~Camera();
		virtual void Start() override;

		static void RenderAll(glContext* esContext);
	};
}