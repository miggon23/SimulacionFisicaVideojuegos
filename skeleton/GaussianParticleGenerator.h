#pragma once
#include "ParticleGenerator.h"
class GaussianParticleGenerator :
    public ParticleGenerator
{
private:
    Vector3 std_dev_pos, std_dev_vel;
    double std_dev_t;
public:

    virtual std::list<Particle*> generateParticles();
};

