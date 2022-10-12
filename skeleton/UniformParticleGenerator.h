#pragma once
#include "ParticleGenerator.h"
class UniformParticleGenerator :
    public ParticleGenerator
{
private:
    Vector3 _vel_width, _pos_width;
public:
    virtual std::list<Particle*> generateParticles();
};  

