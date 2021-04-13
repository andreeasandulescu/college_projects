#include <Laboratoare/Laborator3/ReflPlatform.h>

ReflPlatform::ReflPlatform() : Platform() {

}

int ReflPlatform::platformCollision(Astro& astro) {
	glm::mat3 collision;
	float rads;
	if (astro.a.x < a.x || astro.a.x > b.x || astro.a.y < a.y || astro.a.y > d.y)
		 return 0;
	rads = astro.angleBetween(0, 0, astro.a.x, 0, astro.g.x, astro.g.y, astro.a.x, astro.a.y);
	rads = 3.14 - 2 * rads;
	collision = Transform2D::Translate(astro.a.x, astro.a.y) * Transform2D::Rotate(-rads) * Transform2D::Translate(-astro.a.x, -astro.a.y);
	astro.Transform(collision);
	collision = Transform2D::Translate(astro.g.x, astro.g.y) * Transform2D::Rotate(3.14) * Transform2D::Translate(-astro.g.x, -astro.g.y);
	astro.Transform(collision);
	astro.move = 1;
	return 1;
}