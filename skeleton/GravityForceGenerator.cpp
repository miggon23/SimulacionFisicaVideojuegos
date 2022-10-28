#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g) : _gravity(g)
{
}

void GravityForceGenerator::updateForce(Particle* p, double t)
{
	if (fabs(p->getInverseMass()) < 1e-10)
		return;

	//Aply the mass scaled gravity	
	p->addForce(_gravity * p->getMass());
}
