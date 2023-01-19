#pragma once

#include <PxScene.h>
#include <PxPhysics.h>
#include <vector>
#include "core.hpp"
#include <time.h>

using namespace std;
using namespace physx;

class Player
{
private:
	PxRigidDynamic* Px_;
	PxTransform initPos_;
	float forceCount;
	
	unsigned int nextJump;
	const unsigned int delay = 1000;
public:
	Player(PxRigidDynamic* Px);

	void resetPosition();
	inline Vector3 getPosition() { return Px_->getGlobalPose().p; };
	
	bool addForceToPlayer(float ft);
	void update(float dt);
};

