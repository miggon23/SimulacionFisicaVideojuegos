#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel) : vel_(vel), pose(pos)
{
	auto s = CreateShape(physx::PxSphereGeometry(1));
	renderItem = new RenderItem(s, &pose, color);
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	pose.p += vel_ * t;

	//Cambio de color
	float nColor;
	float a = renderItem->color.x + (0.0002 * colorDir);
	if (a >= 1 || a <= 0)
		colorDir *= -1;
	renderItem->color.x = a;
	renderItem->color.z = a;
}
