#include "Car.h"

#define SPEED 0.12

Car::Car()
{
	this->up = 0;
	this->down = 0;
	this->left = 0;
	this->right = 0;
	this->brake = 0;
	this->translX = 0.0f;
	this->translZ = 0.0f;
	this->rotAngle = 0.0f;
	this->wheelAngle = 0.0f;
	this->centre = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++)
		wheels.push_back(Wheel());
	this->modelMatrix = glm::mat4(1);
	this->direction = glm::vec3(0.0f,0.0f, 1.0f);

	this->wheelsPosition.push_back(glm::vec3(3.4, 1.7f, -4.6f));
	wheels[0].translate(wheelsPosition[0]);
	this->wheelsPosition.push_back(glm::vec3(3.4f, 1.7f, 5.0f));
	wheels[1].translate(wheelsPosition[1]);
	this->wheelsPosition.push_back(glm::vec3(-3.6f, 1.7f, -4.6f));
	wheels[2].translate(wheelsPosition[2]);
	this->wheelsPosition.push_back(glm::vec3(-3.6f, 1.7f, 5.0f));
	wheels[3].translate(wheelsPosition[3]);

	wheels.push_back(Wheel());
}

glm::vec3 Car::mulMatrixW3vec(glm::vec3 v, glm::mat4 m)
{
	glm::vec4 aux;
	aux = glm::vec4(v, 1);
	aux = m * aux;
	return glm::vec3(aux.x, aux.y, aux.z);
}


void Car::translate(glm::vec3 transl)
{
	glm::vec4 aux;
	glm::mat4 m = glm::translate(glm::mat4(1), transl);
	for (int i = 0; i < 4; i++)
	{
		this->wheels[i].translate(transl);
		this->wheelsPosition[i] = this->mulMatrixW3vec(this->wheelsPosition[i], m);
	}
	centre = this->mulMatrixW3vec(centre, m);

	modelMatrix = m * modelMatrix;
}


void Car::scale(glm::vec3 scale)
{
	glm::vec4 aux;
	glm::mat4 m = this->modelMatrix * glm::scale(glm::mat4(1), scale) * glm::inverse(this->modelMatrix);
	for (int i = 0; i < 4; i++)
	{
		this->wheels[i].translate(-wheelsPosition[i]);
		this->wheels[i].scale(scale);
		this->wheelsPosition[i] = this->mulMatrixW3vec(this->wheelsPosition[i], m);
		this->wheels[i].translate(wheelsPosition[i]);
	}
	centre = this->mulMatrixW3vec(centre, m);
	modelMatrix = m * modelMatrix;
}

void Car::rotate(float angle, glm::vec3 axis)
{
	glm::vec4 aux;
	glm::mat4 m = this->modelMatrix * glm::rotate(glm::mat4(1.0f), angle, axis) * glm::inverse(this->modelMatrix);
	glm::mat4 auxW;
	for (int i = 0; i < 4; i++)
	{
		this->wheels[i].modelMatrix = m * this->wheels[i].modelMatrix;
		this->wheels[i].centre = this->mulMatrixW3vec(this->wheels[i].centre, m);
	}
	centre = this->mulMatrixW3vec(centre, m);

	aux = glm::vec4(direction, 1);
	aux = glm::rotate(glm::mat4(1.0f), angle, axis) * aux;
	direction = glm::normalize(glm::vec3(aux.x, aux.y, aux.z));

	modelMatrix = m * modelMatrix;
}

void Car::move(float deltaTimeSeconds)
{
	float dist = 0.0f;
	float angle = 0.0f;
	float wheel = 0.0f;
	int aux = 0 ;
	glm::mat4 m;
	if (this->wheelsPosition[3].z > 0.0f && this->up == 0)		//pentru a nu putea iesi de pe drum
	{
			translZ = 0.0f;
			return;
	}
	//if (this->wheelsPosition[3].z > 0.0f)						
	//{
	//	this->rotate(RADIANS(180), glm::vec3(0.0f, 1.0f, 0.0f));
	//	this->translate(glm::vec3(0.0f, 0.0f, -5.0f));
	//}
	if (this->up == 1)
	{
		dist = deltaTimeSeconds * SPEED;
		this->up = 0;	
	}
	if (this->down == 1)
	{
		dist = -deltaTimeSeconds * SPEED;
		this->down = 0;
	}
	if (this->left == 1)
	{
		angle = RADIANS(0.9);
		this->left = 0;
		aux = 1;
	}
	if (this->right == 1)
	{
		angle = -RADIANS(0.9);
		this->right = 0;
		aux = 1;
	}
	
	this->translZ += dist;
	this->rotAngle += angle;

	if (this->brake == 0)					//rotatia rotilor independent de masina
	{
		for (int i = 0; i < 4; i++)
			this->wheels[i].rotate(translZ, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	
	if (this->brake == 1)
	{
		this->brake = 0;
		translZ = 0;
	}
	this->translate(direction * translZ);
	if (aux == 1)
		this->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
}
