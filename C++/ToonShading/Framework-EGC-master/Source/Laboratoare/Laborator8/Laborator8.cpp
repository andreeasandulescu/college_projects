#include "Laborator8.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator8::Laborator8()
{
}

Laborator8::~Laborator8()
{
}

void Laborator8::Init()
{
	numLevels = 3;
	stopLights = 0;
	rotate = 0.0f;
	const string textureLoc = "Source/Laboratoare/Laborator8/Textures/";

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "santa_diffuse.png").c_str(), GL_REPEAT);
		mapTextures["diffuse"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "santa_specular.png").c_str(), GL_REPEAT);
		mapTextures["specular"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "snow.jpg").c_str(), GL_REPEAT);
		mapTextures["snow"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sky.jpg").c_str(), GL_REPEAT);
		mapTextures["sky"] = texture;
	}
	{
		Mesh* mesh = new Mesh("santa");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "santa.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab8");
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader *shader = new Shader("Outline");
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/OutlineVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/OutlineFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader *shader = new Shader("Sphere");
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/SphereFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader *shader = new Shader("Sky");
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator8/Shaders/SkyFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		fstLightPosition = glm::vec3(0, 0, 0);
		sndLightPosition = glm::vec3(0, 0, 0);
		thrdLightPosition = glm::vec3(0, 0, 0);
		fthLightPosition = glm::vec3(0, 0, 0);

		lightDirection = glm::vec3(0, -1, 0);
		materialShininess = 10;
		materialKa = 0.4;
		materialKs = 0.4;
	}
}

void Laborator8::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void Laborator8::Update(float deltaTimeSeconds)
{
	if (stopLights == 0)
	{
		if (time == 6.2f)
			time = 0.0f;
		else
			time += 0.02;
	}
	if (stopToonShading == 1)
		numLevels = 1;
	if (stopToonShading == 0 && numLevels < 3)
		numLevels = 3;
	if(stopToonShading == 0)
	{															//Toon outline
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.8f, 0));
		RenderSimpleMesh(meshes["santa"], shaders["Outline"], modelMatrix, mapTextures["diffuse"], mapTextures["specular"]);
		glDisable(GL_CULL_FACE);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.8f, 0));
		RenderSimpleMesh(meshes["santa"], shaders["ShaderLab8"], modelMatrix, mapTextures["diffuse"], mapTextures["specular"]);
	}
	

	// Render ground
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.015f, 0));
		RenderSimpleMesh(meshes["plane"], shaders["ShaderLab8"], modelMatrix, mapTextures["snow"]);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(200.0f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(200), glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes["sphere"], shaders["Sky"], modelMatrix, mapTextures["sky"]);
	}


	// Render lights
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		modelMatrix = glm::rotate(modelMatrix, -rotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(14.0f, -sin(time) * 10 + 12.5f, 12.5f));
		fstLightPosition = (modelMatrix * glm::vec4( 1.0f));

		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5f));
		RenderSimpleMesh(meshes["sphere"], shaders["Sphere"], modelMatrix, mapTextures["snow"], mapTextures["snow"], glm::vec3(0, 1, 0));
	}
		
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-14.0f,  sin(time) * 10 + 12.5f, 12.5f));
		sndLightPosition = (modelMatrix * glm::vec4( 1.0f));

		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5f));
		RenderSimpleMesh(meshes["sphere"], shaders["Sphere"], modelMatrix, mapTextures["snow"], mapTextures["snow"], glm::vec3(1, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		modelMatrix = glm::rotate(modelMatrix, rotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(14.0f, sin(time) * 10 + 12.5f, -12.5f));
		thrdLightPosition = (modelMatrix * glm::vec4(1.0f));

		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5f));
		RenderSimpleMesh(meshes["sphere"], shaders["Sphere"], modelMatrix, mapTextures["snow"], mapTextures["snow"], glm::vec3(0, 0, 1));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.09f));
		modelMatrix = glm::rotate(modelMatrix, -rotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-14.0f, -sin(time) * 10 + 12.5f, -12.5f));
		fthLightPosition = (modelMatrix * glm::vec4(1.0f));

		modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5f));
		RenderSimpleMesh(meshes["sphere"], shaders["Sphere"], modelMatrix, mapTextures["snow"], mapTextures["snow"], glm::vec3(1, 1, 1));
	}

}

void Laborator8::FrameEnd()
{
//	DrawCoordinatSystem();
}

void Laborator8::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2, glm::vec3 color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	glUniform1f(glGetUniformLocation(shader->program, "time"), Engine::GetElapsedTime());
	glUniform1i(glGetUniformLocation(shader->program, "num_levels"), numLevels );

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform

	int fstLight = glGetUniformLocation(shader->program, "fst_light_position");
	glUniform3f(fstLight, fstLightPosition.x, fstLightPosition.y, fstLightPosition.z);

	int sndLight = glGetUniformLocation(shader->program, "snd_light_position");
	glUniform3f(sndLight, sndLightPosition.x, sndLightPosition.y, sndLightPosition.z);

	int thrdLight = glGetUniformLocation(shader->program, "thrd_light_position");
	glUniform3f(thrdLight, thrdLightPosition.x, thrdLightPosition.y, thrdLightPosition.z);

	int fthLight = glGetUniformLocation(shader->program, "fth_light_position");
	glUniform3f(fthLight, fthLightPosition.x, fthLightPosition.y, fthLightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_ka = glGetUniformLocation(shader->program, "material_ka");
	glUniform1f(material_ka, materialKa);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int colorobj = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(colorobj, 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
	}

	if (texture2)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
	}
	

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator8::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;
	if (stopLights == 0)
		rotate += deltaTime;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

	}
	
	
}

void Laborator8::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE)
		stopLights = (stopLights + 1) % 2;
	if (key == GLFW_KEY_T)
		stopToonShading = (stopToonShading + 1) % 2;
	if (key == GLFW_KEY_UP)
	{
		if (numLevels < 255)
			numLevels++;
	}
	if (key == GLFW_KEY_DOWN)
	{
		if (numLevels > 3)
			numLevels--;
	}
}

void Laborator8::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator8::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator8::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator8::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator8::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator8::OnWindowResize(int width, int height)
{
}
