#pragma once

#ifndef _ASTRO_H_
#define _ASTRO_H_

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
#include <Laboratoare/Laborator3/Asteroid.h>

class Astro {

public:
	glm::vec3 aVal = glm::vec3(5, 8.66, 1.0);
	glm::vec3 bVal = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 cVal = glm::vec3(10, 0.0, 1.0);
	glm::vec3 orange = glm::vec3(1, 0.5, 0);
	glm::vec3 red = glm::vec3(1, 0, 0);
	glm::vec3 a, b, c, g;
	glm::mat3 modelMatrix;
	Mesh* mesh;
	float radius;
	int move;
	Astro();
	Astro(glm::vec3 a, glm::vec3 b, glm::vec3 c, Mesh* triangle);
	static float distanceBetween(float x1, float x2, float y1, float y2);
	static double angleBetween(float fstX1, float fstY1, float fstX2, float fstY2, float sndX1, float sndY1, float sndX2, float sndY2);
	void Transform(glm::mat3 transform);
	void setVertices(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	int asteroidCollision(Asteroid& s);
	void mouseMove(glm::vec3 mouse);
	void Astro::projectOnDirection(glm::vec3 point1, glm::vec3 point2, float deltaTime);
};

#endif

