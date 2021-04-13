#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <include\math.h>
#include <Core/Managers/ResourcePath.h>

class Wheel
{
public:
	Wheel();
	glm::mat4 modelMatrix;
	glm::vec3 centre;
	glm::vec3 mulMatrixW3vec(glm::vec3 v, glm::mat4 m);
	void translate(glm::vec3 transl);
	void scale(glm::vec3 scale);
	void rotate(float angle, glm::vec3 axis);
};