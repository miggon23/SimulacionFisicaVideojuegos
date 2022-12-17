#pragma once
#include "ForceGenerator.h"
class TorqueForceGenerator :
    public ForceGenerator
{
public:
    TorqueForceGenerator(float k, float r, Vector3 p, bool b);
    virtual void updateForce(physx::PxRigidDynamic* p, double duration) override;

protected:
    float _k; //Intensidad
    float _r; //Radio de la zona
    Vector3 _p; //Centro del radio
};

