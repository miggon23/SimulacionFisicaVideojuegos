#pragma once
#include "ParticleDragGenerator.h"
class WhirlwindGenerator :
    public ParticleDragGenerator
{
private:
    Vector3 _center;
    float k;
public:
    WhirlwindGenerator(float k1, float k2, float wForce, Vector3 wCenter);
    virtual void updateForce(Particle* particle, double t) override;

};

