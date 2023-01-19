#pragma once
#include "ParticleDragGenerator.h"
class WhirlwindGenerator :
    public ParticleDragGenerator
{
private:
    Vector3 _center;
    float k;
    float rango;
public:
    WhirlwindGenerator(float k1, float k2, float wForce, float rango, Vector3 wCenter);
    virtual void updateForce(physx::PxRigidDynamic* p, double t) override;

    inline void setK(float constant) {
        if (constant > 0.2)
            k = constant;
    };
    inline float getK() { return k; };


    inline Vector3 getCenter() { return _center; };
    inline void setCenter(Vector3 v) {if(v.y > 0.1) _center = v; };
};

