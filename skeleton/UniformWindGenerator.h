#pragma once
#include "ParticleDragGenerator.h"
class UniformWindGenerator :
    public ParticleDragGenerator
{
private:
    Vector3 _wind;
public:

    UniformWindGenerator(float k1, float k2, Vector3 w);

    virtual void updateForce(Particle* particle, double t) override;
};

