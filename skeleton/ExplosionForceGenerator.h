#pragma once
#include "ForceGenerator.h"
class ExplosionForceGenerator :
    public ForceGenerator
{
private:
	float R;
	float k;	
	float _tConstant;
	Vector3 _center;
public:
	ExplosionForceGenerator(const float explosionRadius, const float explosionForce, const float timeConstant, Vector3 center);

	virtual void updateForce(Particle* particle, double duration) override;

};

