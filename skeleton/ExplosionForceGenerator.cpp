#include "ExplosionForceGenerator.h"

ExplosionForceGenerator::ExplosionForceGenerator(const float explosionRadius, const float explosionForce, const float timeConstant, Vector3 center, bool active) : R(explosionRadius),
																						k(explosionForce), _center(center), _tConstant(timeConstant)
{
	_active = active;
}

void ExplosionForceGenerator::updateForce(physx::PxRigidDynamic* p, double duration)
{
	if (!_active)
		return;

	//auto pos = p->getPos();
	auto pos = p->getGlobalPose().p;
	//float distance = sqrtf(pow(pos.x - _center.x, 2) + pow(pos.y - _center.y, 2) + pow(pos.z - _center.z, 2));
	//Calculamos la distancia al cuadrado
	float distance = pow(pos.x - _center.x, 2) + pow(pos.y - _center.y, 2) + pow(pos.z - _center.z, 2);

	//Si se sale del radio de la explosión, salimos
	if (distance >= (R * R))
		return;
	
	float kR = k / distance;
	float eConstant = pow(2.71828182846, -(duration/_tConstant));//e number
	Vector3 fVec = kR * eConstant * Vector3(pos - _center);

	p->addForce(fVec);
}
