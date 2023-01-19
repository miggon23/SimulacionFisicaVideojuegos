#include "Player.h"

Player::Player(PxRigidDynamic* Px) : Px_(Px), initPos_(Px->getGlobalPose())
{
	nextJump = 0;
}

void Player::resetPosition()
{
	PxQuat p = { 0.0, Vector3(0, 1, 0) };
	Px_->setGlobalPose({ 4.0, 2, 0.0 });
	Px_->setLinearVelocity({0.0, 0.0, 0.0});
	Px_->clearForce();
}

bool Player::addForceToPlayer(float ft)
{
	if (clock() >= nextJump) {
		Vector3 force = { -0.2, 0.55, -1.0 };
		Px_->setLinearVelocity(force * ft);
		nextJump = clock() + delay;
		return true;
	}
	else
		return false;
}

void Player::update(float dt)
{
	/*if()*/
}


