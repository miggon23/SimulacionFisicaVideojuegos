#include "RigidBodyGenerator.h"

RigidBodyGenerator::RigidBodyGenerator(WorldManager* wM, std::string nombre, Vector3 posM, Vector3 velM, int nPart, bool active)
										: _wM(wM), _name(nombre), _mean_pos(posM), _mean_vel(velM), _num_particles(nPart), limitOfParticles(3000), _active(active)
{
}

RigidBodyGenerator::~RigidBodyGenerator()
{
}
