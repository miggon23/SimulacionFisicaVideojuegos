#pragma once
#include "ForceGenerator.h"
class SinkForceGenerator :
    public ForceGenerator
{
protected:
    float _height, _liquid_density;
    Vector3 pos;

public:
    SinkForceGenerator(float h, float lD, Vector3 p);

    virtual void updateForce(Particle* particle, double duration) override;
    inline float getLiquidDensity() { return _liquid_density; };

    inline void setLiquidDensity(float ld) { _liquid_density = ld; };
};

