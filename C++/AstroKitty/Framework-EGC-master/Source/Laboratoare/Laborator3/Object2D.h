#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	Mesh* CreateRectangle(std::string name, glm::vec3 leftUCorner, glm::vec3 leftBCorner, glm::vec3 rightBCorner, glm::vec3 rightUCorner,  glm::vec3 color1, bool fill);

	// Create triangle with given corners and color
	Mesh* CreateTriangle(std::string name, glm::vec3 upperCorner, glm::vec3 leftCorner, glm::vec3 rightCorner, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, bool fill);

	// Create octagon with given corners and color
	Mesh* CreateOctagon(std::string name, glm::vec3 c0, glm::vec3 c1, glm::vec3 c2, glm::vec3 c3, glm::vec3 c4, glm::vec3 c5, glm::vec3 c6, glm::vec3 c7, glm::vec3 c8, glm::vec3 color, bool fill);
}