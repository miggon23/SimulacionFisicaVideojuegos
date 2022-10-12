#pragma once
#include "ParticleGenerator.h"
class GaussianParticleGenerator :
    public ParticleGenerator
{
private:
    Vector3 std_dev_pos, std_dev_vel; //Desviaciones estandar de pos y vel
    double std_dev_t; //desviacion estandar del tiempo de vida de la partícula
public:
    GaussianParticleGenerator(Vector3 dvPos, Vector3 dvVel, double dvT);
    virtual std::list<Particle*> generateParticles() override;
};

