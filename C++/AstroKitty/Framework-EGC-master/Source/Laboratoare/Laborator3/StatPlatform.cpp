#include <Laboratoare/Laborator3/StatPlatform.h>

StatPlatform::StatPlatform() : Platform() {

}

int StatPlatform::platformCollision(Astro& astro) {
	glm::mat3 collision;
	float x = astro.a.x;
	float y = astro.a.y;
	float lenX = abs(astro.c.x - astro.b.x);
	if (astro.a.x < a.x || astro.a.x > b.x || astro.a.y < a.y || astro.a.y > d.y)
		return 0;
	astro.Transform(glm::inverse(astro.modelMatrix));
	if (y <= g.y)
	{
		astro.Transform(Transform2D::Rotate(3.14));
		astro.Transform(Transform2D::Translate(x + lenX / 2, y));
	}
	else
		astro.Transform(Transform2D::Translate(x - lenX / 2, y + (abs(a.y - b.y) / 2)));
	astro.move = 0;
	return 1;
}