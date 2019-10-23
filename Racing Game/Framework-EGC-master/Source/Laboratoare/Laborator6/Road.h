#pragma once
#include "Fragment.h"

class Road
{
public:
	Road();
	std::vector<Fragment> frags;
	void straightRoad(int nrFragments, glm::vec3 centre);
	void leftCurve(int nrFragments, glm::vec3 centre);
	void rightCurve(int nrFragments, glm::vec3 centre);

};