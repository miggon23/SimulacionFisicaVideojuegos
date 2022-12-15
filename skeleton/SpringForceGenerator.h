#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(double k, double resting_length, physx::PxRigidDynamic* other);

	virtual void updateForce(physx::PxRigidDynamic* particle, double duration) override;

	inline void setK(double k) { if(k > 0.1) _k = k; };
	inline float getK() { return _k; };

	virtual ~SpringForceGenerator() {};
protected:
	double _k;
	double _resting_length;
	physx::PxRigidDynamic* _other;
};

class AnchoredSpringFG : public SpringForceGenerator {
public:
	AnchoredSpringFG(double k, double resting, Vector3 anchor_pos);

	~AnchoredSpringFG();
};

class RubberForceGenerator : public SpringForceGenerator
{
public:
	RubberForceGenerator(double k, double resting_length, physx::PxRigidDynamic* other);
	virtual void updateForce(physx::PxRigidDynamic* particle, double duration) override;
};

