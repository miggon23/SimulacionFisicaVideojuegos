#include "ExplosionForceGenerator.h"

ExplosionForceGenerator::ExplosionForceGenerator(const float explosionRadius, const float explosionForce, const float timeConstant, Vector3 center) : R(explosionRadius),
																											k(explosionForce), _center(center), _tConstant(timeConstant)
{
}

void ExplosionForceGenerator::updateForce(Particle* p, double duration)
{
	auto pos = p->getPos();
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
