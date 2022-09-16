#pragma once
#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <ctype.h>

class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel);
	~Particle();

	void integrate(double t);

private:
	Vector3 vel_;
	physx::PxTransform pose; //A render item le pasaremos la direcci�n de esta pose, para que se actualice autom�ticamente
	RenderItem* renderItem;
	Vector4 color = Vector4(0.5f, 0.5f, 0.5f, 1);
	int colorDir = 1;
};

