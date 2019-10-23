#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <vector>
#include <include\math.h>
#include <Core/Managers/ResourcePath.h>
#include "Wheel.h"

class Car
{
public:
	Car();
	int up, down, left, right, brake;
	float translX, translZ, rotAngle, wheelAngle;
	glm::mat4 modelMatrix;
	glm::vec3 centre;
	glm::vec3 direction;
	std::vector<Wheel> wheels;
	std::vector<glm::vec3> wheelsPosition;

	glm::vec3 mulMatrixW3vec(glm::vec3 v, glm::mat4 m);
	void translate(glm::vec3 transl);
	void scale(glm::vec3 scale);
	void rotate(float angle, glm::vec3 axis);
	void move(float deltaTimeSeconds);
};