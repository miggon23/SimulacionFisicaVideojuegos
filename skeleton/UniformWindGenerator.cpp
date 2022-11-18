#include "UniformWindGenerator.h"
#include <iostream>

UniformWindGenerator::UniformWindGenerator(Vector3 center, Vector3 cubeLimits, float k1, float k2, Vector3 w) : 
										ParticleDragGenerator(k1, k2), _wind(w), _center(center), _limits(cubeLimits)
									
{
}

void UniformWindGenerator::updateForce(Particle* particle, double t)
{
	if (fabs(particle->getInverseMass() < 1e-10))
		return;
	auto pos = particle->getPos();
	auto positiveLimits = _center + _limits;
	auto negativeLimits = -positiveLimits;
	if (pos.x > positiveLimits.x || pos.x < negativeLimits.x || pos.y > positiveLimits.y
		|| pos.y < negativeLimits.y || pos.z > positiveLimits.z || pos.z < negativeLimits.z)
		return;

	//APLICAR EN AREA DE EFECTO

	//Compute the drag force
	Vector3 v = particle->getVel() - _wind;
	float drag_coef = v.normalize();
	Vector3 windF;
	drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
	windF = -v * drag_coef;
	//Apply the drag force
	//std::cout << windF.x << "\t" << windF.y << "\t" << windF.z << "\n";
	particle->addForce(windF);
}

