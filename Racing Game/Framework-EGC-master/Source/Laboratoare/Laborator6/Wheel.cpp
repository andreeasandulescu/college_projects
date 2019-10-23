#include "Wheel.h"

Wheel::Wheel()
{
	this->modelMatrix = glm::scale(glm::mat4(1), glm::vec3(0.7f));
	this->centre = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Wheel::mulMatrixW3vec(glm::vec3 v, glm::mat4 m)
{
	glm::vec4 aux;
	aux = glm::vec4(v, 1);
	aux = m * aux;
	return glm::vec3(aux.x, aux.y, aux.z);
}

void Wheel::translate(glm::vec3 transl)
{
	glm::mat4 m = glm::translate(glm::mat4(1), transl);
	centre = mulMatrixW3vec(centre, m);
	this->modelMatrix = m * this->modelMatrix;
}

void Wheel::scale(glm::vec3 scale)
{
	glm::mat4 m = this->modelMatrix * glm::scale(glm::mat4(1), scale) * glm::inverse(this->modelMatrix);
	centre = mulMatrixW3vec(centre, m);
	modelMatrix = m * modelMatrix;
}

void Wheel::rotate(float angle, glm::vec3 axis)
{
	glm::mat4 m = this->modelMatrix * glm::rotate(glm::mat4(1.0f), angle, axis) * glm::inverse(this->modelMatrix);
	centre = mulMatrixW3vec(centre, m);
	modelMatrix = m * modelMatrix;
}
