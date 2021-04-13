#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <vector>

class Fragment
{
	public:
	int containsObtacle;
	float obstacleRadius, pavementRadius;
	Mesh* mesh;
	glm::mat4 modelMatrix, obstacleModelMatrix;
	glm::vec3 centre;
	glm::vec3 lftPavC, rgtPavC, obstacleCentre;
	std::vector<glm::vec3> coords;
	Fragment(int lightOrDark);
	Fragment();
	void translate(glm::vec3 transl);
	void addObstacle(glm::vec3 obstacleCentre);
	int checkCollisionObstacle(glm::vec3 p);
	int checkCollisionPavement(glm::vec3 p);
	static Mesh* CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);
	
	static glm::vec3 white;
	static glm::vec3 gray;
	static glm::vec3 black;
	static glm::vec3 yellow;
	static glm::vec3 red;

	static glm::vec3 a;
	static glm::vec3 b;
	static glm::vec3 c;
	static glm::vec3 d;

	static glm::vec3 app;
	static glm::vec3 bpp;
	static glm::vec3 cpp;
	static glm::vec3 dpp;
};