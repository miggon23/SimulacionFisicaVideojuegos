#include "SinkForceGenerator.h"

SinkForceGenerator::SinkForceGenerator(float h, float lD, Vector3 p) : _height(h), _liquid_density(lD), pos(p)
{

}

void SinkForceGenerator::updateForce(physx::PxRigidDynamic* particle, double duration)
{
	float h = particle->getGlobalPose().p.y;
	float h0 = pos.y;

	Vector3 F(0, 0, 0);
	float immersed = 0.0;
	if (h - h0 > _height * 0.5) {
		//Fuera del agua
		immersed = 0.0;
	}
	else if (h0 - h > _height * 0.5) {
		//Totalmente hundido
		immersed = 1.0;
	}
	else {
		immersed = (h0 - h) / _height + 0.5;
	}

	//WARNING descomentar:
	//F.y = _liquid_density * particle->getVolume() * immersed * 9.8;

	particle->addForce(F);
}
