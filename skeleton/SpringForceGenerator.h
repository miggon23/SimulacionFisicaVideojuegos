#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(double k, double resting_length, Particle* other);

	inline void updateForce(Particle* particle);

	inline void setK(double k) { _k = k; };

	virtual ~SpringForceGenerator() {};
protected:
	double _k;
	double _resting_length;
	Particle* _other;
};

