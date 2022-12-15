#pragma once
#include "ParticleDragGenerator.h"
class UniformWindGenerator :
    public ParticleDragGenerator
{
private:
    Vector3 _wind;
    Vector3 _center;
    Vector3 _limits;
public:

    UniformWindGenerator(Vector3 center, Vector3 cubeLimits, float k1, float k2, Vector3 w);

    virtual void updateForce(physx::PxRigidDynamic* particle, double t) override;
};

