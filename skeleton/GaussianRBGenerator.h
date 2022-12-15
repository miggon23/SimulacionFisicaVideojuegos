#pragma once
#include "RigidBodyGenerator.h"
class GaussianRBGenerator :
    public RigidBodyGenerator
{
protected:
    Vector3 std_dev_pos, std_dev_vel; //Desviaciones estandar de pos y vel
    double std_dev_t; //desviacion estandar del tiempo de vida de la partícula
public:
    GaussianRBGenerator(WorldManager* wM, Vector3 dvPos, Vector3 dvVel, double dvT, Vector3 posM, Vector3 velM, int nPart);
    virtual std::list<RigidDynamic*> generateParticles() override;
    inline void setDevPos(Vector3 dPos) { std_dev_pos = dPos; };
    inline void setDevVel(Vector3 dVel) {
        if (dVel.x <= 0.01 || dVel.y <= 0.01 || dVel.z <= 0.01)
            return;
        std_dev_vel = dVel;
    };

    inline Vector3 getDevPos() { return std_dev_pos; };
    inline Vector3 getDevVel() { return std_dev_vel; };
};



