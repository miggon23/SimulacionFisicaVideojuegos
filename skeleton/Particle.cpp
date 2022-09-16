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
	float nColor;
	modff(color.x + 0.1, &nColor);
	color.x = nColor;
}
