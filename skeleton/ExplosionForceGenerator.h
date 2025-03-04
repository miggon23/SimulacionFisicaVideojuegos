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
	ExplosionForceGenerator(const float explosionRadius, const float explosionForce, const float timeConstant, Vector3 center, bool active);

	virtual void updateForce(physx::PxRigidDynamic* p, double duration) override;

	inline void setK(float constant) { 
		if(constant > 0.2)
			k = constant; 
	};
	inline float getK() { return k; };

	inline void setR(float constant) {
		if (constant > 10)
			R = constant;
	}
	inline float getR() { return R; };
};

