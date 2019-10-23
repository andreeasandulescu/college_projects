#include "Laborator6.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
using namespace std;

Laborator6::Laborator6()
{
	cameraType = CameraType::EngineCamera;
}

Laborator6::~Laborator6()
{
}

void Laborator6::Init()
{
	glm::vec3 lightGreen = glm::vec3(0.47f, 1, 0.188f);

	road = Road();
	car =  new Car();
	renderCar = 1;
	collisions = 0;
	score = 0;
	lastAvoidedObstacle = -1;
	polygonMode = GL_FILL;

	road.frags[10].addObstacle(glm::vec3(2.35f, 0.0f, 0.0f));
	road.frags[42].addObstacle(glm::vec3(0.16f, 0.0f, 0.0f));
	road.frags[82].addObstacle(glm::vec3(-2.0f, 0.0f, 0.0f));
	road.frags[187].addObstacle(glm::vec3(-2.0f, 0.0f, 0.0f));
	road.frags[188].addObstacle(glm::vec3(-1.0f, 0.0f, 0.0f));
	road.frags[189].addObstacle(glm::vec3(0.0f, 0.0f, 0.0f));
	road.frags[190].addObstacle(glm::vec3(1.0f, 0.0f, 0.0f));
	road.frags[191].addObstacle(glm::vec3(2.0f, 0.0f, 0.0f));

	car->scale(glm::vec3(0.2f));
	car->rotate(RADIANS(180),glm::vec3(0.0f, 1.0f, 0.0f));
	car->translate(glm::vec3(0.0f, 0.0f, -1.0f));
	
	renderCameraTarget = false;
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	projectionMatrixLab = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// grass
	{
		vector<VertexFormat> grassVertices
		{
			VertexFormat(glm::vec3(3, 2 , 0),lightGreen),
			VertexFormat(glm::vec3(-3 , 2, 0), lightGreen),
			VertexFormat(glm::vec3(-3, -2, 0),  lightGreen),
			VertexFormat(glm::vec3(3, -2, 0),  lightGreen),
		};

		vector<unsigned short> grassIndices =
		{
			0, 1, 2,		2, 3, 0,
		};
		CreateMesh("grass", grassVertices, grassIndices);
	}
	{
		Mesh* mesh = new Mesh("sky");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("car");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "Jeep.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("wheel");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "wheel.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("cone");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "cone.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Shader *shader = new Shader("ShaderLab6");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader *shader = new Shader("ShaderLabGrass");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShaderGrass.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShaderGrass.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader *shader = new Shader("ShaderLabSky");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShaderSky.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShaderSky.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader *shader = new Shader("ShaderLabRoad");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShaderRoad.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShaderRoad.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

}

Mesh* Laborator6::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	glBindVertexArray(0);

	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}


void Laborator6::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator6::Update(float deltaTimeSeconds)
{
	glm::vec3 move;
	float cosAngle, angle, obsC;
	bool check;
	int pavLeftFront, pavLeftBack, pavRightFront, pavRightBack;
	int obsLeftFront, obsLeftBack, obsRightFront, obsRightBack;
	int auxBreak = 0;
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	for (int i = 0; i < road.frags.size(); i++)
	{
		pavLeftBack = road.frags[i].checkCollisionPavement(car->wheels[0].centre);
		pavLeftFront = road.frags[i].checkCollisionPavement(car->wheels[1].centre);	//roata fata stanga
		pavRightBack = road.frags[i].checkCollisionPavement(car->wheels[2].centre);
		pavRightFront = road.frags[i].checkCollisionPavement(car->wheels[3].centre);	//roata fata dreapta

		obsLeftBack = road.frags[i].checkCollisionObstacle(car->wheels[0].centre);
		obsLeftFront = road.frags[i].checkCollisionObstacle(car->wheels[1].centre);
		obsRightBack = road.frags[i].checkCollisionObstacle(car->wheels[2].centre);
		obsRightFront = road.frags[i].checkCollisionObstacle(car->wheels[3].centre);
		
		obsC = road.frags[i].obstacleCentre.z;
		

		check = obsLeftFront == 1 || obsLeftBack == 1 || obsRightBack == 1 || obsRightFront == 1;
		if(check)
		{
			car->brake = 1;
			car->direction = glm::normalize(car->direction);

			cosAngle = glm::dot(car->direction, glm::vec3(0.0f, 0.0f, -1.0f));
			angle = acos(cosAngle);
			if (car->direction.x > 0.0f)
				angle = -angle;
			if (i > 5)
				move = road.frags[i - 5].centre;
			else
				move = road.frags[0].centre;

			car->translate(-car->centre);
			car->translate(move);
			car->rotate(-angle, glm::vec3(0.0f, 1.0f, 0.0f));
			car->direction = glm::vec3(car->direction.x, 0.0f, car->direction.z);
		
			score -= 1;																			
			collisions += 1;
			auxBreak = 1;
		}
		else if( (obsLeftBack == 2  && obsC > car->wheels[0].centre.z) || (obsRightBack == 2 && obsC > car->wheels[2].centre.z))
		{
			score += 1;
			road.frags[i].obstacleModelMatrix = glm::translate(road.frags[i].obstacleModelMatrix, glm::vec3(100.0f, 0.0f, 100.0f));
			road.frags[i].containsObtacle = 0;
		}

		if( pavLeftFront > 0 || pavLeftBack > 0 || pavRightBack > 0 || pavRightFront > 0)
		{
			car->brake = 1;
			car->direction = glm::normalize(car->direction);

			cosAngle = glm::dot(car->direction, glm::vec3(0.0f, 0.0f, -1.0f));
			angle = acos(cosAngle);
			if (car->direction.x > 0.0f)
				angle = -angle;
			if (i > 5)
				move = road.frags[i - 5].centre;
			else
				move = road.frags[0].centre;
			
			car->translate(-car->centre);
			car->translate(move);
			car->rotate(-angle, glm::vec3(0.0f, 1.0f, 0.0f));
			car->direction = glm::vec3(car->direction.x, 0.0f, car->direction.z);

			score -= 1;
			collisions += 1;
			auxBreak = 1;
		}
		if (auxBreak == 1)
			break;
	}
	if (collisions > 4)
	{
		Exit();
		FILE * file = fopen("Source\\Laboratoare\\Laborator6\\score.txt", "w");
		fprintf(file, "Scor: %d", score);
		fclose(file);
	}
		
	car->move(deltaTimeSeconds);
	if (cameraType == CameraType::LabCameraTPS)
	{
		glm::vec3 cameraPosition = car->centre - 7.0f * car->direction + 7.0f * glm::vec3(0, 1, 0);
		glm::vec3 lookAt = car->centre;
		camera->Set(cameraPosition, lookAt, glm::vec3(0, 1, 0));
	}
	if (cameraType == CameraType::LabCameraFPS)
	{
		glm::vec3 cameraPosition = car->centre + 2.0f * glm::vec3(0, 1, 0.0f);
		glm::vec3 lookAt = car->centre + 5.0f * car->direction;
		camera->Set(cameraPosition, lookAt, glm::vec3(0, 1, 0));
		renderCar = 0;
	}
	else
		renderCar = 1;
	{
		for (int i = 0; i < road.frags.size(); i++)
			RenderSimpleMesh(road.frags[i].mesh, shaders["ShaderLabRoad"], road.frags[i].modelMatrix);
		for (int i = 0; i < road.frags.size(); i++)
		{
			if(road.frags[i].containsObtacle == 1)
				RenderSimpleMesh(meshes["cone"], shaders["VertexNormal"], road.frags[i].obstacleModelMatrix);
		}
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(car->centre.x, 0.01f, car->centre.z - 20.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(200.0f, 1.0f, 200.0f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
		RenderSimpleMesh(meshes["grass"], shaders["ShaderLabGrass"], modelMatrix);
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(car->centre.x, 5.0f, car->centre.z));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(300.0f, 300.0f, 300.0f));
		RenderSimpleMesh(meshes["sky"], shaders["ShaderLabSky"], modelMatrix);
	}
	{
		if (renderCar == 1)
		{
			RenderSimpleMesh(meshes["car"], shaders["VertexNormal"], car->modelMatrix);
			for (int i = 0; i <4; i++)
				RenderSimpleMesh(meshes["wheel"], shaders["VertexNormal"], car->wheels[i].modelMatrix);
		}
		
	}
}

void Laborator6::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Laborator6::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
	glm::mat4 viewMatrix, projectionMatrix;
	if (!mesh || !shader || !shader->GetProgramID())
		return;
	int shaderID = shader->GetProgramID();
	int locationModel, locationView,locationProjection;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	locationModel = glGetUniformLocation(shaderID, "Model");
	glUniformMatrix4fv(locationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	locationView = glGetUniformLocation(shaderID, "View");
	if (cameraType == CameraType::EngineCamera)
		viewMatrix = GetSceneCamera()->GetViewMatrix();
	else
		viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	locationProjection = glGetUniformLocation(shaderID, "Projection");
	if (cameraType == CameraType::EngineCamera)
		projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	else
		projectionMatrix = projectionMatrixLab;
	glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int location = glGetUniformLocation(shaderID, "time");
	glUniform1f(location, Engine::GetElapsedTime());

	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(locationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator6::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_UP) || (window->KeyHold(GLFW_KEY_W) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)))
		car->up = 1;
	if (window->KeyHold(GLFW_KEY_DOWN) || (window->KeyHold(GLFW_KEY_S) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)))
		car->down = 1;
	if (window->KeyHold(GLFW_KEY_LEFT) || (window->KeyHold(GLFW_KEY_A) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)))
		car->left = 1;
	if (window->KeyHold(GLFW_KEY_RIGHT) || (window->KeyHold(GLFW_KEY_D) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)))
		car->right = 1;
	if (window->KeyHold(GLFW_KEY_SPACE))
		car->brake = 1;	
}

void Laborator6::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C)
	{
		cameraType = CameraType::EngineCamera;
	}
	if (key == GLFW_KEY_F)
	{
		cameraType = CameraType::LabCameraFPS;
	}
	if (key == GLFW_KEY_T)
	{
		cameraType = CameraType::LabCameraTPS;
	}
	if (key == GLFW_KEY_1)
	{
		polygonMode = GL_FILL;
	}
	if (key == GLFW_KEY_2)
	{
		polygonMode = GL_LINE;
	}
}

void Laborator6::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator6::OnWindowResize(int width, int height)
{
}
