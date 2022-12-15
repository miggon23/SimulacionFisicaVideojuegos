#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_length, physx::PxRigidDynamic* other) :
													_k(k), _resting_length(resting_length)	, _other(other)
{
}

inline void SpringForceGenerator::updateForce(physx::PxRigidDynamic* particle, double duration)
{
	Vector3 force = _other->getGlobalPose().p - particle->getGlobalPose().p;

	const float length = force.normalize();
	const float delta_x = length - _resting_length;

	force *= delta_x * _k;

	particle->addForce(force);
}

//-------------------------------------------------------------------------------------

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, Vector3 anchor_pos) :
							SpringForceGenerator(k, resting, nullptr)
{
	//WARNING Descomentar:
	//_other = new physx::PxRigidDynamic(anchor_pos, { 0,0,0 }, { 0, 0, 0 }, { 0.0, 0.6, 0.0, 1.0 }, 0.9, 1000.0, 0.8);
	_other->setMass(1e6);
}	

AnchoredSpringFG::~AnchoredSpringFG()
{
}

RubberForceGenerator::RubberForceGenerator(double k, double resting_length, physx::PxRigidDynamic* other) : SpringForceGenerator(k, resting_length, other)
{
}

void RubberForceGenerator::updateForce(physx::PxRigidDynamic* particle, double duration)
{
	Vector3 force = _other->getGlobalPose().p - particle->getGlobalPose().p;

	if (force.magnitude() < _resting_length)
		return;

	const float length = force.normalize();
	const float delta_x = length - _resting_length;

	force *= delta_x * _k;

	particle->addForce(force);
}
