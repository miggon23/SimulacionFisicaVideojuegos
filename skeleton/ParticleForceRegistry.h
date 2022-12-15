#pragma once

#include <map>
#include "ForceGenerator.h"

typedef std::pair<ForceGenerator*, physx::PxRigidDynamic*> FRPair;

class ParticleForceRegistry : public std::multimap<ForceGenerator*, physx::PxRigidDynamic*> {
public:
	ParticleForceRegistry();
	inline void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++) {
			it->first->updateForce(it->second, duration);
		}
	}
	void addRegistry(ForceGenerator* fG, physx::PxRigidDynamic* p);

	void deleteParticleRegistry(physx::PxRigidDynamic* p);
};


