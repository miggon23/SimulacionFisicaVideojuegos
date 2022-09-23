#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 ac, float d) : vel_(vel), pose(pos), acceleration(ac), dumping(d)
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
	vel_ = vel_ * pow(dumping, t) + acceleration * t;

	pose.p += vel_ * t + 0.5 * acceleration * t;

	//Cambio de color
	float nColor;
	float a = renderItem->color.x + (0.0002 * colorDir);
	if (a >= 1 || a <= 0)
		colorDir *= -1;
	renderItem->color.x = a;
	renderItem->color.z = a;
}
