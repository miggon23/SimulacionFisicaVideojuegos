#pragma once
#include "ParticleGenerator.h"
class UniformParticleGenerator :
    public ParticleGenerator
{
private:
    Vector3 _vel_width{ 0.0, 0.0, 0.0 }, _pos_width{0.0, 0.0, 0.0};
public:
    UniformParticleGenerator();
    UniformParticleGenerator(Vector3 velW, Vector3 posW);
    virtual std::list<Particle*> generateParticles() override;
};  

