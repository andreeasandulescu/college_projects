#include "Fragment.h"

glm::vec3 Fragment::white = glm::vec3(1, 1, 1);
glm::vec3 Fragment::gray = glm::vec3(0.54f, 0.54f, 0.54f);
glm::vec3 Fragment::black = glm::vec3(0, 0, 0);
glm::vec3 Fragment::yellow = glm::vec3(1, 1, 0);
glm::vec3 Fragment::red = glm::vec3(1, 0, 0);

glm::vec3 Fragment::a = glm::vec3(0.25f, 0.05f, -0.5f);
glm::vec3 Fragment::b = glm::vec3(-0.25f, 0.05f, -0.5f);
glm::vec3 Fragment::c = glm::vec3(-0.25f, 0.05f, 0.5f);
glm::vec3 Fragment::d = glm::vec3(0.25f, 0.05f, 0.5f);

glm::vec3 Fragment::app = glm::vec3(3.0f, 0.05f, -0.5f);
glm::vec3 Fragment::bpp = glm::vec3(-3.0f, 0.05f, -0.5f);
glm::vec3 Fragment::cpp = glm::vec3(-3.0f, 0.05f, 0.5f);
glm::vec3 Fragment::dpp = glm::vec3(3.0f, 0.05f, 0.5f);

Fragment::Fragment(int lightOrDark)	//lightOrDark colors(white, yelow) or (gray, black)
{
	glm::vec3 middleColor, borderColor;
	this->containsObtacle = 0;
	if (lightOrDark == 1)
	{
		middleColor = white;
		borderColor = yellow;
	}
	else
	{
		middleColor = gray;
		borderColor = black;
	}

	
	std::vector<VertexFormat> vertices = 
	{
			VertexFormat(a, middleColor),				//A = 0
			VertexFormat(b, middleColor),				//B = 1
			VertexFormat(c, middleColor),				//C = 2
			VertexFormat(d, middleColor),				//D = 3

			VertexFormat(a, gray),				//A' = 4
			VertexFormat(d, gray),				//D' = 5
			VertexFormat(app, gray),			//A'' = 6
			VertexFormat(dpp, gray),			//D'' = 7
			VertexFormat(b, gray),				//B' = 8
			VertexFormat(c, gray),				//C' = 9
			VertexFormat(bpp, gray),			//B'' = 10
			VertexFormat(cpp, gray),			//C'' = 11

			VertexFormat(app, borderColor),										//G cub = 12			cub dreapta
			VertexFormat(dpp, borderColor),										//C cub = 13
			VertexFormat(glm::vec3(4.0f, 0.05f, 0.5f), borderColor),				//D cub = 14
			VertexFormat(glm::vec3(4.0f, 0.05f, -0.5f), borderColor),				//H cub = 15
			VertexFormat(glm::vec3(4.0f, 1.0f, 0.5f), borderColor),				//A cub = 16
			VertexFormat(glm::vec3(3.0f, 1.0f, 0.5f), borderColor),				//B cub = 17
			VertexFormat(glm::vec3(3.0f, 1.0f, -0.5f), borderColor),			//F cub = 18
			VertexFormat(glm::vec3(4.0f, 1.0f, -0.5f), borderColor),			//E cub = 19

			VertexFormat(bpp, borderColor),										//H cub = 20			//cub stanga
			VertexFormat(cpp, borderColor),										//D cub = 21
			VertexFormat(glm::vec3(-4.0f, 0.05f, 0.5f), borderColor),				//C cub = 22
			VertexFormat(glm::vec3(-4.0f, 0.05f, -0.5f), borderColor),				//G cub = 23
			VertexFormat(glm::vec3(-3.0f, 1.0f, 0.5f), borderColor),			//A cub = 24
			VertexFormat(glm::vec3(-3.0f, 1.0f, -0.5f), borderColor),			//E cub = 25		
			VertexFormat(glm::vec3(-4.0f, 1.0f, 0.5f), borderColor),			//B cub = 26
			VertexFormat(glm::vec3(-4.0f, 1.0f, -0.5f), borderColor),			//F cub = 27
	};

		std::vector<unsigned short> indices =
		{
			0, 1, 2,		0, 2, 3,
			6, 4, 5,		6, 5, 7,
			8, 10, 11,		8, 11, 9,
			15, 12, 13,		15, 13, 14,				//cub dreapta
			18, 17, 13,		18, 13, 12, 			
			19, 18, 17,		19, 17, 16,				
			19, 18, 12,		19, 12, 15,
			19, 16, 14,		19, 14, 15,
			16, 17, 13,		16, 13, 14,
			20, 23, 22,		20, 22, 21,
			25, 24, 21,		25, 21, 20,
			25, 27, 26,		25, 26, 24,
			27, 26, 22,		27, 22, 23,
			25, 27, 23,		25, 23, 20,
			24, 26, 22,		24, 22, 21,
		};

		for (int i = 0; i < vertices.size(); i++) {
			coords.push_back(glm::vec4(vertices[i].position,1));
		}
		mesh = Fragment::CreateMesh("pavement", vertices, indices);
		centre = glm::vec3(0,0.05f,0);
		pavementRadius = 1.0f;
		lftPavC = glm::vec3(-3.5f, 0.525f, 0.0f);
		rgtPavC = glm::vec3(3.5f, 0.525f, 0.0f);
}


Fragment::Fragment()	
{
	this->containsObtacle = 0;
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(a, gray),				//A = 0
		VertexFormat(b, gray),				//B = 1
		VertexFormat(c, gray),				//C = 2
		VertexFormat(d, gray),				//D = 3

		VertexFormat(a, gray),				//A' = 4
		VertexFormat(d, gray),				//D' = 5
		VertexFormat(app, gray),			//A'' = 6
		VertexFormat(dpp, gray),			//D'' = 7
		VertexFormat(b, gray),				//B' = 8
		VertexFormat(c, gray),				//C' = 9
		VertexFormat(bpp, gray),			//B'' = 10
		VertexFormat(cpp, gray),			//C'' = 11

		VertexFormat(app, red),										//G cub = 12			cub dreapta
		VertexFormat(dpp, red),										//C cub = 13
		VertexFormat(glm::vec3(4.0f, 0.05f, 0.5f), red),				//D cub = 14
		VertexFormat(glm::vec3(4.0f, 0.05f, -0.5f), red),				//H cub = 15
		VertexFormat(glm::vec3(4.0f, 1.0f, 0.5f), red),				//A cub = 16
		VertexFormat(glm::vec3(3.0f, 1.0f, 0.5f), red),				//B cub = 17
		VertexFormat(glm::vec3(3.0f, 1.0f, -0.5f), red),			//F cub = 18
		VertexFormat(glm::vec3(4.0f, 1.0f, -0.5f), red),			//E cub = 19

		VertexFormat(bpp, red),										//H cub = 20			//cub stanga
		VertexFormat(cpp, red),										//D cub = 21
		VertexFormat(glm::vec3(-4.0f, 0.05f, 0.5f), red),				//C cub = 22
		VertexFormat(glm::vec3(-4.0f, 0.05f, -0.5f), red),				//G cub = 23
		VertexFormat(glm::vec3(-3.0f, 1.0f, 0.5f), red),			//A cub = 24
		VertexFormat(glm::vec3(-3.0f, 1.0f, -0.5f), red),			//E cub = 25		
		VertexFormat(glm::vec3(-4.0f, 1.0f, 0.5f), red),			//B cub = 26
		VertexFormat(glm::vec3(-4.0f, 1.0f, -0.5f), red),			//F cub = 27
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,		0, 2, 3,
		6, 4, 5,		6, 5, 7,
		8, 10, 11,		8, 11, 9,
		15, 12, 13,		15, 13, 14,				//cub dreapta
		18, 17, 13,		18, 13, 12,
		19, 18, 17,		19, 17, 16,
		19, 18, 12,		19, 12, 15,
		19, 16, 14,		19, 14, 15,
		16, 17, 13,		16, 13, 14,
		20, 23, 22,		20, 22, 21,
		25, 24, 21,		25, 21, 20,
		25, 27, 26,		25, 26, 24,
		27, 26, 22,		27, 22, 23,
		25, 27, 23,		25, 23, 20,
		24, 26, 22,		24, 22, 21,
	};

	for (int i = 0; i < vertices.size(); i++) {
		coords.push_back(glm::vec4(vertices[i].position, 1));
	}
	mesh = Fragment::CreateMesh("pavement", vertices, indices);
	centre = glm::vec3(0, 0.05f, 0);
}


void Fragment::translate(glm::vec3 transl)
{
	glm::vec4 aux;
	glm::mat4 m = glm::mat4(1);
	m = glm::translate(m, transl);
	for (int i = 0; i < coords.size(); i++)
	{
		aux = glm::vec4(coords[i], 1);
		aux = m * aux;
		coords[i] = glm::vec3(aux.x, aux.y, aux.z);
	}
	aux = glm::vec4(centre, 1);
	aux = m * aux;
	centre = glm::vec3(aux.x, aux.y, aux.z);

	aux = glm::vec4(lftPavC, 1);
	aux = m * aux;
	lftPavC = glm::vec3(aux.x, aux.y, aux.z);

	aux = glm::vec4(rgtPavC, 1);
	aux = m * aux;
	rgtPavC = glm::vec3(aux.x, aux.y, aux.z);

	modelMatrix = glm::translate(modelMatrix, transl);
}

void Fragment::addObstacle(glm::vec3 obstacleCentre)
{
	glm::vec4 aux;
	this->containsObtacle = 1;
	obstacleRadius = 4 * 0.2f;

	obstacleModelMatrix = glm::mat4(1);
	this->obstacleCentre = glm::vec3(-0.05f, 3.475f, -0.04f);
	
	glm::vec3 transl = glm::vec3(obstacleCentre.x + this->centre.x, obstacleCentre.y + this->centre.y, obstacleCentre.z + this->centre.z);
	obstacleModelMatrix = glm::translate(obstacleModelMatrix, transl);
	obstacleModelMatrix = glm::translate(obstacleModelMatrix, glm::vec3(-0.01f, 0.695, -0.008f));
	obstacleModelMatrix = glm::scale(obstacleModelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
	obstacleModelMatrix = glm::translate(obstacleModelMatrix, glm::vec3(0.05f, -3.475f, 0.04f));

	aux = glm::vec4(this->obstacleCentre, 1.0f);
	aux = obstacleModelMatrix * aux;
	this->obstacleCentre = glm::vec3(aux.x, aux.y, aux.z);

}

int Fragment::checkCollisionPavement(glm::vec3 p) {
	int collisions = 0;
	float distLeft, distRight;
	distLeft = glm::distance(lftPavC, p);
	distRight = glm::distance(rgtPavC, p);

	if (pavementRadius >= distLeft)
		collisions++;
	if (pavementRadius >= distRight)
		collisions++;
	return collisions;
}

int Fragment::checkCollisionObstacle(glm::vec3 p) {
	float dist;
	if (containsObtacle == 0)
		return 0;

	dist = glm::distance(obstacleCentre, p);
	if (obstacleRadius >= dist)
	{
		obstacleModelMatrix = glm::translate(obstacleModelMatrix, glm::vec3(100.0f, 0.0f, 100.0f));
		containsObtacle = 0;
		return 1;					//are loc coliziune
	}
	else 
		return 2;
}


Mesh* Fragment::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	Mesh* auxMesh;
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	auxMesh = new Mesh(name);
	auxMesh->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	auxMesh->vertices = vertices;
	auxMesh->indices = indices;
	return auxMesh;
}