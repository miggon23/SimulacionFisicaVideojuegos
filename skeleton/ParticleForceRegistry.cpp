#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry()
{
}

void ParticleForceRegistry::addRegistry(ForceGenerator* fG, physx::PxRigidDynamic* p)
{
	this->insert(FRPair{fG, p});
}

void ParticleForceRegistry::deleteParticleRegistry(physx::PxRigidDynamic* p)
{
	for (auto it = begin(); it != end();) {
		if (it->second == p) {
			it = this->erase(it);
		}
		else
			it++;
	}
}
