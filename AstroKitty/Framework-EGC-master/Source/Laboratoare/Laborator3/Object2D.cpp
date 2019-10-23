#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), glm::vec3(0,0,1)),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftUCorner, glm::vec3 leftBCorner, glm::vec3 rightBCorner, glm::vec3 rightUCorner,glm::vec3 color, bool fill){
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(leftUCorner, color),
		VertexFormat(leftBCorner, color),
		VertexFormat(rightBCorner, color),
		VertexFormat(rightUCorner, color),
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 2,3, 0};

		rectangle->SetDrawMode(GL_TRIANGLES);

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}


Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 upperCorner, glm::vec3 leftCorner, glm::vec3 rightCorner, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, bool fill)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(upperCorner, color1),
		VertexFormat(leftCorner, color2),
		VertexFormat(rightCorner, color3),
	};
	
	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };

	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateOctagon(std::string name, glm::vec3 c0, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4, glm::vec3 c5, glm::vec3 c6, glm::vec3 c7, glm::vec3 c8, glm::vec3 color, bool fill) {
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(c0, color),	//G
		VertexFormat(c1, color),	//H
		VertexFormat(c2, color),	//I
		VertexFormat(c3, color),	//J
		VertexFormat(c4, color),	//K
		VertexFormat(c5, color),	//L
		VertexFormat(c6, color),	//M
		VertexFormat(c7, color),	//N
		VertexFormat(c8, color),	//P
	};
	Mesh* octagon = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2,
					0, 2, 3,
					0, 3 ,4,
					0, 4, 5,
					0, 5, 6,
					0, 6, 7,
					0, 7, 8,
					0, 8, 1
	};
	
	octagon->SetDrawMode(GL_TRIANGLES);
	octagon->InitFromData(vertices, indices);
	return octagon;
}
