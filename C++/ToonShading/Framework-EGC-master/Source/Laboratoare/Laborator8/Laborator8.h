#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

enum class CameraType { LabCameraFPS, LabCameraTPS };


class Laborator8 : public SimpleScene
{
	public:
		Laborator8();
		~Laborator8();

		glm::vec3 fstLightPosition;
		glm::vec3 sndLightPosition;
		glm::vec3 thrdLightPosition;
		glm::vec3 fthLightPosition;
		glm::vec3 lightDirection;
		unsigned int materialShininess;
		float materialKa;
		float materialKs;

		int numLevels;
		int stopLights;
		int stopToonShading;
		float rotate;
		double time;

		std::unordered_map<std::string, Texture2D*> mapTextures;

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, glm::vec3 color = glm::vec3(1, 1, 1));

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
};
