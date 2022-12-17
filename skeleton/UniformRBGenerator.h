#pragma once
#include "RigidBodyGenerator.h"

class WorldManager;
class UniformRBGenerator :
    public RigidBodyGenerator
{
private:
    Vector3 _vel_width{ 0.0, 0.0, 0.0 }, _pos_width{ 0.0, 0.0, 0.0 };
public:
    UniformRBGenerator(WorldManager* _gM, Vector3 velW, Vector3 posW, Vector3 posM, Vector3 velM, int nPart, bool active);
    virtual std::list<RigidDynamic*> generateParticles() override;
};

