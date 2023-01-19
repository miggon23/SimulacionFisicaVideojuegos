#include "WhirlwindGenerator.h"
#include <iostream>


WhirlwindGenerator::WhirlwindGenerator(float k1, float k2, float wForce, float rango, Vector3 wCenter) : ParticleDragGenerator(k1, k2), 
																						k(wForce), _center(wCenter), rango(rango)
{
}

void WhirlwindGenerator::updateForce(physx::PxRigidDynamic* p, double t)
{
	float distance = abs((p->getGlobalPose().p - _center).normalize());

	if (distance > rango)
		return;

	//Velocidad del viento proporcional a la distancia en el centro del torbellino
	auto pos = p->getGlobalPose().p;
	Vector3 v = p->getLinearVelocity() - k * Vector3(
		-(pos.z - _center.z) -0.9* (pos.x - _center.x),
		0,
		(pos.x - _center.x) -0.9*(pos.z - _center.z)
	);

	float drag_coef = v.normalize();
	Vector3 windF;
	drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
	windF = -v * drag_coef;
	//Apply the drag force
	//std::cout << windF.x << "\t" << windF.y << "\t" << windF.z << "\n";
	p->addForce(windF);
}
