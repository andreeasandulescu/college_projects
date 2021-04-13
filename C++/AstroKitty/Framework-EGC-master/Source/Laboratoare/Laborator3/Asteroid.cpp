#include <Laboratoare\Laborator3\Laborator3_Vis2D.h>
int Asteroid::index = 0;

Asteroid::Asteroid(){
	this->aux1 = 0;
	this->aux2 = 0;
	this->h = hVal;
	this->i = iVal;
	this->j = jVal;
	this->k = kVal;
	this->l = lVal;
	this->m = mVal;
	this->n = nVal;
	this->p = pVal;
	this->g = gVal;
	this->modelMatrix = glm::mat3(1);
	this->radius = abs(sqrt((p.x - g.x) *(p.x - g.x) + (p.y - g.y)*(p.y - g.y)));
	sprintf(name, "oct%d", index);
	if (index == 0)
		this->mesh = Object2D::CreateOctagon(name, g, h, i, j, k, l, m, n, p, glm::vec3(0, 0.8, 0.8), true);
	else {
		if (index == 1)
			this->mesh = Object2D::CreateOctagon(name, g, h, i, j, k, l, m, n, p, glm::vec3(0.31f, 0.58f, 0.9f), true);
		else
			this->mesh = Object2D::CreateOctagon(name, g, h, i, j, k, l, m, n, p, glm::vec3(0, 0, 1), true);
	}
	index = index + 1;
}

Asteroid::Asteroid(glm::vec3 h, glm::vec3 i, glm::vec3 j, glm::vec3 k, glm::vec3 l, glm::vec3 m, glm::vec3 n, glm::vec3 p, Mesh* octagon) {
	this->aux1 = 0;
	this->aux2 = 0;
	this->h = h;
	this->i = i;
	this->j = j;
	this->k = k;
	this->l = l;
	this->m = m;
	this->n = n;
	this->p = p;
	this->g = glm::vec3((h.x + i.x + j.x + k.x + l.x + m.x + n.x + p.x) / 8, (h.y + i.y + j.y + k.y + l.y + m.y + n.y + p.y) / 8, 1);
	this->modelMatrix = glm::mat3(1);
	this->radius = abs(sqrt((p.x - g.x) *(p.x - g.x) + (p.y - g.y)*(p.y - g.y)));
	this->mesh = octagon;
}

void Asteroid::setVertices(glm::vec3 h, glm::vec3 i, glm::vec3 j, glm::vec3 k, glm::vec3 l, glm::vec3 m, glm::vec3 n, glm::vec3 p){
	this->h = h;
	this->i = i;
	this->j = j;
	this->k = k;
	this->l = l;
	this->m = m;
	this->n = n;
	this->p = p;
	g = glm::vec3((h.x + i.x + j.x + k.x + l.x + m.x + n.x + p.x) / 8, (h.y + i.y + j.y + k.y + l.y + m.y + n.y + p.y) / 8, 1);
	radius = abs(sqrt((p.x - g.x) *(p.x - g.x) + (p.y - g.y)*(p.y - g.y)));
}

void Asteroid::Transform(glm::mat3 transform) {
	modelMatrix = transform * modelMatrix;
	this->setVertices(transform * h, transform * i, transform * j, transform *k, transform *l, transform *m, transform *n, transform *p);
}

void Asteroid::RotatingAsteroid(float deltaTime) {
	glm::mat3 rot = Transform2D::Translate(g.x, g.y) * Transform2D::Rotate(-0.78 * deltaTime * 2) * Transform2D::Translate(-g.x, -g.y);
	this->Transform(rot);
}

void Asteroid::ScallingAsteroid(float deltaTime) {
	glm::mat3 transf;
	float x = g.x;
	float y = g.y;
	this->Transform(glm::inverse(this->modelMatrix));		//incep de fiecare data de la asteroidul initial
	if (aux1 == 0)											//pt initializarea valorilor scaleFactorsi scale
	{
		scale = 2;
		scaleFactor = deltaTime / 10;
		aux1 = 1;
	}
	if (scale > 10)											//daca factorul de scalare(scale) depaseste limita superioara
		scaleFactor = (-1) * scaleFactor;					//trebuie sa incep sa scad scaleFactor il loc sa il adun
	if (scale < 2)											//daca factorul depaseste limita inferioara
		scaleFactor = (-1) * scaleFactor;					//trebuie adunat cu scaleFactor
	scale = scale + scaleFactor;
	transf = Transform2D::Translate(x, y) * Transform2D::Scale(scale,scale) * Transform2D::Translate(-g.x, -g.y);
	this->Transform(transf);
}

void Asteroid::TranslatingAsteroid(float deltaTime) {
	glm::mat3 transf;
	if (aux2 == 0)						//pentru initializarea factorilor de scalare pe Ox si Oy
	{
		translX = 0;
		translY = deltaTime / 5;
		translFactor = deltaTime / 20;
		aux2 = 1; 
	}

	if (translX > 1)
		translFactor = (-1) * translFactor;
	if (translX < -1)
		translFactor = (-1) * translFactor;
	translX = translX + translFactor;

	if (g.y > 80)								//din nou, daca ating limita superioara, trebuie sa scada valoarea
		translY = (-1) * translY;				//factorului de scalare
	if (g.y < 50)
		translY = (-1) * translY;
	transf = Transform2D::Translate(translX, translY);
	this->Transform(transf);
}