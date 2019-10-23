#pragma once

#ifndef _REFLPLATFORM_H_
#define _REFLPLATFORM_H_

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
#include <Laboratoare/Laborator3/Astro.h>

class ReflPlatform : public Platform {
public:
	ReflPlatform();
	int platformCollision(Astro& astro);

};

#endif

