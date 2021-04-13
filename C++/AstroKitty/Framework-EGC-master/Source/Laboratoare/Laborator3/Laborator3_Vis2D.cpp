#include <Laboratoare\Laborator3\Laborator3_Vis2D.h>

using namespace std;

Laborator3_Vis2D::Laborator3_Vis2D()
{
}

Laborator3_Vis2D::~Laborator3_Vis2D()
{
}

void Laborator3_Vis2D::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	glm::ivec2 resolution = window->GetResolution();

	logicSpace.x = 0;		// logic x
	logicSpace.y = 0;		// logic y
	logicSpace.width = 192;	// logic width
	logicSpace.height = 108;	// logic height
	mouseBegin = 0;
	fin = 0;
	
	astro = Astro();
	AddMeshToList(astro.mesh);
	
	asteroizi = std::vector<Asteroid>{
		Asteroid(), Asteroid(),	Asteroid()
	};

	for(int i = 0 ; i < asteroizi.size();i++)
	AddMeshToList(asteroizi[i].mesh);

	platforme = std::vector<Platform*>{
		new ReflPlatform(), new StatPlatform(), new FinPlatform()
	};
	for (int i = 0; i < asteroizi.size(); i++)
		AddMeshToList((*platforme[i]).mesh);


	for (int i = 0; i < asteroizi.size(); i++)
		asteroizi[i].Transform(Transform2D::Scale(8, 8));
	asteroizi[0].Transform(Transform2D::Translate(80, 55));
	asteroizi[1].Transform(Transform2D::Translate(40, 25));
	asteroizi[2].Transform(Transform2D::Translate(110, 55));
	(*platforme[0]).Transform(Transform2D::Translate(75, 20));
	(*platforme[1]).Transform(Transform2D::Translate(25, 80));
	(*platforme[2]).Transform(Transform2D::Translate(140, 90));
}

// 2D visualization matrix
glm::mat3 Laborator3_Vis2D::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Laborator3_Vis2D::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void Laborator3_Vis2D::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void Laborator3_Vis2D::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Laborator3_Vis2D::Update(float deltaTimeSeconds)
{
	deltaTime = deltaTimeSeconds;
	glm::ivec2 resolution = window->GetResolution();

	viewSpace = ViewportSpace(0, 0, resolution.x , resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0), true);
	
	// Compute the 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);

	asteroizi[1].RotatingAsteroid(deltaTime);
	asteroizi[0].ScallingAsteroid(deltaTime);
	asteroizi[2].TranslatingAsteroid(deltaTime);
	DrawScene(visMatrix);

	if (fin == 1)
		this->Exit();
}

void Laborator3_Vis2D::FrameEnd()
{

}

void Laborator3_Vis2D::DrawScene(glm::mat3 visMatrix)
{
	glm::vec3 viewPortMouse = glm::vec3(mouseX, viewSpace.height - mouseY, 1);
	glm::vec3 windowMouse = viewPortMouse * glm::inverse(visMatrix);
	glm::mat3 transf;
	float x, y, lenX, lenY;

	for (int i = 0; i < asteroizi.size(); i++)				//verific coliziunea cu fiecare asteroid
		astro.asteroidCollision(asteroizi[i]);
		
	(*platforme[0]).platformCollision(astro);				//verific coliziunea cu fiecare platforma
	(*platforme[1]).platformCollision(astro);
	if ((*platforme[2]).platformCollision(astro) == 1)		//in cazul platformei finale, jocul trebuie sa se termine
		fin = 1;
	if (mouseBegin == 1)
	{
		astro.mouseMove(glm::vec3(windowMouse.x, windowMouse.y, 1));
		mouseBegin = 0;
	}
									//marginile ecranului sunt platforme de stationare
	x = astro.a.x;	
	y = astro.a.y;
	lenX = abs(astro.c.x - astro.b.x);
	if (astro.a.y <= 0)			
	{										
		astro.Transform(glm::inverse(astro.modelMatrix));
		astro.Transform(Transform2D::Translate(x - lenX / 2, 0));
		astro.move = 0;

	}
	if (astro.a.y >= 108)
	{
		astro.Transform(glm::inverse(astro.modelMatrix));
		astro.Transform(Transform2D::Rotate(3.14));
		astro.Transform(Transform2D::Translate(x + lenX / 2, y));
		astro.move = 0;
	}
	if (astro.a.x <= 0 )
	{
		astro.Transform(glm::inverse(astro.modelMatrix));
		transf = Transform2D::Translate(astro.g.x, astro.g.y)* Transform2D::Rotate(-1.57) * Transform2D::Translate(-astro.g.x, -astro.g.y);
		astro.Transform(transf);
		astro.Transform(Transform2D::Translate(-2, y + lenX / 2));
		astro.move = 0;

	}
	if (astro.a.x >= 192)
	{
		astro.Transform(glm::inverse(astro.modelMatrix));
		transf = Transform2D::Translate(astro.g.x, astro.g.y)* Transform2D::Rotate(1.57) * Transform2D::Translate(-astro.g.x, -astro.g.y);
		astro.Transform(transf);
		astro.Transform(Transform2D::Translate(x - 8, y - lenX/2));
		astro.move = 0;
	}

	if(astro.move == 1)
		astro.projectOnDirection(astro.a, astro.g, deltaTime);

	RenderMesh2D(meshes["astronaut"], shaders["VertexColor"], visMatrix * astro.modelMatrix);
	RenderMesh2D(meshes["oct0"], shaders["VertexColor"], visMatrix * asteroizi[0].modelMatrix);
	RenderMesh2D(meshes["oct1"], shaders["VertexColor"], visMatrix * asteroizi[1].modelMatrix);
	RenderMesh2D(meshes["oct2"], shaders["VertexColor"], visMatrix * asteroizi[2].modelMatrix);
	RenderMesh2D(meshes["rec0"], shaders["VertexColor"], visMatrix * (*platforme[0]).modelMatrix);
	RenderMesh2D(meshes["rec1"], shaders["VertexColor"], visMatrix * (*platforme[1]).modelMatrix);
	RenderMesh2D(meshes["rec2"], shaders["VertexColor"], visMatrix * (*platforme[2]).modelMatrix);
}

void Laborator3_Vis2D::OnInputUpdate(float deltaTime, int mods)
{
	//TODO move the logic window with W, A, S, D (up, left, down, right)
	//TODO zoom in and zoom out logic window with Z and X
}

void Laborator3_Vis2D::OnKeyPress(int key, int mods)
{

}

void Laborator3_Vis2D::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3_Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3_Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		this->mouseX = mouseX;
		this->mouseY = mouseY;
		mouseBegin = 1;
	}
	
}

void Laborator3_Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3_Vis2D::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}