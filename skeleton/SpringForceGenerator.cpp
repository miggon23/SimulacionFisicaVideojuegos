#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_length, Particle* other) : 
													_k(k), _resting_length(resting_length)	, _other(other)
{
}

inline void SpringForceGenerator::updateForce(Particle* particle, double duration)
{
	Vector3 force = _other->getPos() - particle->getPos();

	const float length = force.normalize();
	const float delta_x = length - _resting_length;

	force *= delta_x * _k;

	particle->addForce(force);
}

//-------------------------------------------------------------------------------------

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, Vector3 anchor_pos) :
							SpringForceGenerator(k, resting, nullptr)
{
	_other = new Particle(anchor_pos, { 0,0,0 }, { 0, 0, 0 }, { 0.0, 0.6, 0.0, 1.0 }, 0.9, 1000.0, 0.8);
	_other->setMass(1e6);
}	

AnchoredSpringFG::~AnchoredSpringFG()
{
	delete _other;
}

RubberForceGenerator::RubberForceGenerator(double k, double resting_length, Particle* other) : SpringForceGenerator(k, resting_length, other)
{
}

void RubberForceGenerator::updateForce(Particle* particle, double duration)
{
	Vector3 force = _other->getPos() - particle->getPos();

	if (force.magnitude() < _resting_length)
		return;

	const float length = force.normalize();
	const float delta_x = length - _resting_length;

	force *= delta_x * _k;

	particle->addForce(force);
}
