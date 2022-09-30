#pragma once
#include "Particle.h"
#include <list>
class ParticleSystem
{
private:
	std::list<Particle*> listP;
	ProyType pType = PAINT_BALL;
public:
	ParticleSystem();
	~ParticleSystem();

	void update(double t);
	void addParticle(Vector3 pos, Vector3 dir);
	inline void changeParticleType(ProyType pT) { pType = pT; };
};

