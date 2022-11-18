#include "WhirlwindGenerator.h"
#include <iostream>


WhirlwindGenerator::WhirlwindGenerator(float k1, float k2, float wForce,Vector3 wCenter) : ParticleDragGenerator(k1, k2), 
																						k(wForce), _center(wCenter)
{
}

void WhirlwindGenerator::updateForce(Particle* p, double t)
{
	//Velocidad del viento proporcional a la distancia en el centro del torbellino
	auto pos = p->getPos();
	Vector3 v = p->getVel() - k * Vector3(
		-(pos.z - _center.z) -0.9* (pos.x - _center.x),
		20 - (pos.y - _center.y),
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
