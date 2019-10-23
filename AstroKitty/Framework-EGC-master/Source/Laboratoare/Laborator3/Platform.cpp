#include <Laboratoare\Laborator3\Platform.h>
int Platform::index = 0;

Platform::Platform() {
	this->a = aVal;
	this->b = bVal;
	this->c = cVal;
	this->d = dVal;
	this->g = glm::vec3((a.x + b.x + c.x + d.x) / 4, (a.y + b.y + c.y + d.x) / 4, 1);
	this->modelMatrix = glm::mat3(1);
	sprintf(name, "rec%d", index);
	if (index == 0)
		this->mesh = Object2D::CreateRectangle(name, a, b, c, d, glm::vec3(0.5,1, 0), true);
	else {
		if (index == 1)
			this->mesh = Object2D::CreateRectangle(name, a, b, c, d, glm::vec3(1, 0.76, 0), true);
		else
			this->mesh = Object2D::CreateRectangle(name, a, b, c, d, glm::vec3( 1, 0.1, 0.1), true);
	}
	index = index + 1;
}

void Platform::setVertices(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	g = glm::vec3((a.x + b.x + c.x + d.x) / 4, (a.y + b.y + c.y + d.y) / 4, 1);
}

void Platform::Transform(glm::mat3 transform) {
	modelMatrix = transform * modelMatrix;
	this->setVertices(transform * a, transform * b, transform * c, transform * d);
}

int Platform::platformCollision(Astro& astro) {

	return 5;
}