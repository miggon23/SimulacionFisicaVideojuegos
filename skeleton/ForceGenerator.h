#pragma once

#include "Particle.h"
#include <list>
#include <random>
class ForceGenerator{
public:
	virtual void updateForce(physx::PxRigidDynamic* particle, double duration) = 0;
	double t = -1e10; //If starting negative, eternal

	inline void setName(std::string n) { _name = n; };
	inline std::string getName() { return _name; };	
	inline void setActive(bool b) { _active = b; };
	inline bool getActive() { return _active; };
protected:
	std::string _name;

	bool _active;
};	

