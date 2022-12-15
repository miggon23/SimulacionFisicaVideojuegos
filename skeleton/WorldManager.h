#pragma once

#include <PxScene.h>
#include <PxPhysics.h>
#include "RenderUtils.hpp"
#include "RigidBodyGenerator.h"
#include "ParticleForceRegistry.h"
#include "ForceGenerator.h"
#include <list>

using namespace physx;

struct RigidDynamic {
	PxRigidDynamic* rigidDynamic;
	RenderItem* rItem;
	unsigned int actualTime;
	unsigned int deathTime;
	unsigned int maxTime;
	bool alive;

	~RigidDynamic() { DeregisterRenderItem(rItem); };
};

class WorldManager
{
protected:
	const unsigned int NUM_DYN_OBJ_LIMIT = 100;

	std::list<RigidDynamic*> itemList;
	std::list<shared_ptr<RigidBodyGenerator>> _particle_generators;
	std::list<shared_ptr<ForceGenerator>> _force_generators;
	std::shared_ptr<ForceGenerator> _activeForceGenerator = nullptr;

	ParticleForceRegistry* particleFR;

	PxPhysics* _gPhysics;
	PxScene* _gScene;

public:
	WorldManager(PxPhysics* gPhysics, PxScene* gScene);
	
	void initWorld();
	void addDynamicRigidBody(Vector3 size, Vector3 p, Vector3 vel, Vector4 color, unsigned int time);
	void deleteDynamicRigidBody(RigidDynamic* rD);
	
	void update(float dt);

	shared_ptr<RigidBodyGenerator> getParticleGenerator(std::string name);
	shared_ptr<ForceGenerator> getForceGenerator(std::string name);
	inline void addParticleGenerator(shared_ptr<RigidBodyGenerator> pG) { _particle_generators.push_back(pG); };
	inline void addForceGenerator(shared_ptr<ForceGenerator> fG) { _force_generators.push_back(fG); };
	//inline void addToScene(PxActor* actor);
	inline void addToScene(PxRigidDynamic* actor);
	void activateForceGenerator(std::string s);

	RigidDynamic* clone(RigidDynamic* rD);
};

