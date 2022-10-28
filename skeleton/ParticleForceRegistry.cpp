#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry()
{
}

void ParticleForceRegistry::addRegistry(ForceGenerator* fG, Particle* p)
{
	this->insert(FRPair{fG, p});
}

void ParticleForceRegistry::deleteParticleRegistry(Particle* p)
{
	for (auto it = begin(); it != end();) {
		if (it->second == p) {
			it = this->erase(it);
		}
		else
			it++;
	}
}
