#pragma once

#ifndef _PLATFORM_H
#define _PLATFORM_H_

#include <Component/SimpleScene.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <Laboratoare/Laborator3/Object2D.h>
#include <Laboratoare/Laborator3/Transform2D.h>
#include <Laboratoare\Laborator3\Astro.h>

class Platform {

public:
	glm::vec3 aVal = glm::vec3(0, 0, 1);
	glm::vec3 bVal = glm::vec3(30, 0, 1);
	glm::vec3 cVal = glm::vec3(30, 5, 1);
	glm::vec3 dVal = glm::vec3(0, 5, 1);
	glm::vec3 a, b, c, d, g;
	glm::mat3 modelMatrix;
	Mesh* mesh;
	static int index;
	char name[10];
	Platform();
	void setVertices(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	void Transform(glm::mat3 transform);
	virtual int platformCollision(Astro& astro);
};

#endif

