#include "TorqueForceGenerator.h"

TorqueForceGenerator::TorqueForceGenerator(float k, float r, Vector3 p, bool b) : _k(k), _r(r), _p(p)
{
	_active = b;
}

void TorqueForceGenerator::updateForce(physx::PxRigidDynamic* p, double duration)
{
	if (!_active)
		return;

	auto pos = p->getGlobalPose().p;

	auto difX = pos.x - _p.x;
	auto difY = pos.y - _p.y;
	auto difZ = pos.z - _p.z;

	auto r2 = pow(difX, 2) + pow(difY, 2) + pow(difZ, 2);

	if (r2 > _r * _r)
		return;

	p->addTorque(Vector3(difX, difY, difX) * _k);
}
