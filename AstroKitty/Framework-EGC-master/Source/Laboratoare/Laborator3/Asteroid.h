#pragma once


#ifndef _ASTEROID_H_
#define _ASTEROID_H_

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

class Asteroid {

public:
	glm::vec3 g, h, i, j, k, l, m, n, p;
	glm::vec3 gVal = glm::vec3(0.5, 1.205, 1.0);  //G
	glm::vec3 hVal = glm::vec3(0, 2.41, 1.0);		//H
	glm::vec3 iVal = glm::vec3(-0.71, 1.71, 1.0); //I
	glm::vec3 jVal = glm::vec3(-0.71, 0.71, 1.0); //J
	glm::vec3 kVal = glm::vec3(0, 0, 1.0);		//K
	glm::vec3 lVal = glm::vec3(1, 0, 1.0);		//L
	glm::vec3 mVal = glm::vec3(1.71, 0.71, 1.0);  //M
	glm::vec3 nVal = glm::vec3(1.71, 1.71, 1.0);  //N
	glm::vec3 pVal = glm::vec3(1, 2.41, 1.0);   //P

	glm::mat3 modelMatrix;
	char name[10];
	static int index;
	float radius;
	float aux1, aux2;
	float translY;
	float scale,scaleFactor,translX,translFactor;
	Mesh* mesh;
	Asteroid();
	Asteroid(glm::vec3 h, glm::vec3 i, glm::vec3 j, glm::vec3 k, glm::vec3 l, glm::vec3 m, glm::vec3 n, glm::vec3 p, Mesh* octagon);

	void Asteroid::setVertices(glm::vec3 h, glm::vec3 i, glm::vec3 j, glm::vec3 k, glm::vec3 l, glm::vec3 m, glm::vec3 n, glm::vec3 p);
	void Transform(glm::mat3 transform);
	void Asteroid::RotatingAsteroid(float deltaTime);
	void Asteroid::ScallingAsteroid(float deltaTime);
	void Asteroid::TranslatingAsteroid(float deltaTime);

};

#endif

