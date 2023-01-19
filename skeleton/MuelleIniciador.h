#pragma once

#include <PxPhysics.h>
#include <PxScene.h>
#include "WorldManager.h"
#include "SpringForceGenerator.h" 

class MuelleIniciador
{
protected:
	RigidDynamic* obj_;
	PinballSpringGenerator* spring_;

public:
	MuelleIniciador(RigidDynamic* rD);
	void update(float dt);
};

