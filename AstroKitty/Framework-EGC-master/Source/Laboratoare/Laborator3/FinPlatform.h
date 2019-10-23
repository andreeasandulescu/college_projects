#pragma once

#ifndef _FINPLATFORM_H_
#define _FINPLATFORM_H_

#include <Component/SimpleScene.h>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>
#include <Laboratoare/Laborator3/Object2D.h>
#include <Laboratoare/Laborator3/Transform2D.h>
#include <Laboratoare/Laborator3/Platform.h>

class FinPlatform : public Platform {
public:
	FinPlatform();
	int platformCollision(Astro& astro);
};

#endif

