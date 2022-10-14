#pragma once
#include "ParticleGenerator.h"
class UniformParticleGenerator :
    public ParticleGenerator
{
private:
    Vector3 _vel_width{ 0.0, 0.0, 0.0 }, _pos_width{0.0, 0.0, 0.0};
public:
    UniformParticleGenerator(Vector3 velW, Vector3 posW, Vector3 posM, Vector3 velM, int nPart);
    virtual std::list<Particle*> generateParticles() override;
};  

