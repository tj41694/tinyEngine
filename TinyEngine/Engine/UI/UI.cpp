#include "UI.h"
#include "Engine/FileSystem.h"


namespace TEngine {
	std::unordered_map<unsigned int, UI*> UI::UIs;
	GLuint UI::textShader = 0;
	GLuint UI::UIVBO = 0;
	GLuint UI::UIVAO = 0;

	UI::UI() {
		UI::LoadUIVAO();
		UIs.insert(std::pair<unsigned int, UI*>(GetInstanceID(), this));
	}

	void UI::DrawAll() {
		for (auto &ui : UIs) {
			ui.second->Draw();
		}
	}

	void UI::Draw() {
	}

	void UI::LoadUIVAO() {
		if (UIVAO == 0) {
			textShader = esLoadProgram(FileSystem::ReadFile("resources/shaders/text/text.vs").c_str(), FileSystem::ReadFile("resources/shaders/text/text.fs").c_str());
			// Configure VAO/VBO for texture quads
			glGenVertexArrays(1, &UI::UIVAO);
			glGenBuffers(1, &UI::UIVBO);
			glBindVertexArray(UI::UIVAO);
			glBindBuffer(GL_ARRAY_BUFFER, UI::UIVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW); //´´½¨¿ÕVBO
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}


	UI::~UI() {
		UIs.erase(GetInstanceID());
	}
}