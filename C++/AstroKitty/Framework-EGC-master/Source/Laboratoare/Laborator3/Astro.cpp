#include <Laboratoare\Laborator3\Laborator3_Vis2D.h>

Astro::Astro(){
	this->move = 0;
	this->a = aVal;
	this->b = bVal;
	this->c = cVal;
	this->g = glm::vec3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, 1);
	this->radius = abs(sqrt((a.x - g.x) *(a.x - g.x) + (a.y - g.y)*(a.y - g.y)));
	this->modelMatrix = glm::mat3(1);
	this->mesh = Object2D::CreateTriangle("astronaut", a, b, c, red, orange, orange, true);
}

Astro::Astro(glm::vec3 a, glm::vec3 b, glm::vec3 c, Mesh* triangle){
	this->move = 0;
	this->a = a;
	this->b = b;
	this->c = c;
	this->g = glm::vec3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, 1);
	this->radius = abs(sqrt((a.x - g.x) *(a.x - g.x) + (a.y - g.y)*(a.y - g.y)));
	this->modelMatrix = glm::mat3(1);
	this->mesh = triangle;
}                             

float Astro::distanceBetween(float x1, float x2, float y1, float y2) {
	return abs(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

double Astro::angleBetween(float fstX1, float fstY1, float fstX2, float fstY2, float sndX1, float sndY1, float sndX2, float sndY2) {
	double fstAngle = atan2(fstY1-fstY2, fstX1-fstX2);
	double sndAngle = atan2(sndY1-sndY2,sndX1-sndX2);
	return fstAngle - sndAngle;
}

void Astro::Transform(glm::mat3 transform) {
	modelMatrix = transform * modelMatrix;
	this->setVertices(transform * a, transform * b, transform * c);
}

void Astro::setVertices(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	this->a = a;
	this->b = b;
	this->c = c;
	g = glm::vec3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, 1);
	radius = this->distanceBetween(a.x, g.x, a.y, g.y);					//daca scalez astronautul, raza se modifica
}

int Astro::asteroidCollision(Asteroid& s) {
	glm::mat3 collision;
	float distCenters, rads;
	distCenters = this->distanceBetween(this->g.x, s.g.x, this->g.y, s.g.y);
	if (distCenters > this->radius + s.radius)
		return 0;
	rads = 2 * this->angleBetween(s.g.x, s.g.y, a.x, a.y, g.x, g.y, a.x, a.y);
	collision = Transform2D::Translate(a.x, a.y) * Transform2D::Rotate(rads) * Transform2D::Translate(-a.x, -a.y);
	this->Transform(collision);
	collision = Transform2D::Translate(g.x, g.y) * Transform2D::Rotate(3.14) * Transform2D::Translate(-g.x, -g.y);
	this->Transform(collision);
	s.Transform(Transform2D::Translate(3000, 3000));			//distrugere asteroid la coliziune
	move = 1;
	return 1;
}

void Astro::projectOnDirection(glm::vec3 point1, glm::vec3 point2, float deltaTime) {
	int v = 35;
	glm::vec3 direction = glm::vec3(point1.x - point2.x, point1.y - point2.y, 1);
	direction = normalize(direction);
	this->Transform(Transform2D::Translate(direction.x * deltaTime * v, direction.y * deltaTime * v));
}

void Astro::mouseMove(glm::vec3 mouse) {
	float rotationAngle;
	glm::mat3 rot;
	rotationAngle = this->angleBetween(mouse.x, mouse.y, g.x, g.y, a.x, a.y, g.x, g.y);
	rot = Transform2D::Translate(g.x, g.y) * Transform2D::Rotate(rotationAngle) * Transform2D::Translate(-g.x, -g.y);
	this->Transform(rot);
	this->move = 1;
}
