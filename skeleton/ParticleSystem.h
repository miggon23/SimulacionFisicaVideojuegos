#pragma once
#include "Particle.h"
#include <list>
#include "ParticleGenerator.h"
class ParticleSystem
{
private:
	ProyType pType = PAINT_BALL;
	std::list<Particle*> listP;
	std::list<ParticleGenerator*> _particle_generators;
public:
	ParticleSystem();
	~ParticleSystem();

	void update(double t);
	void addParticle(Vector3 pos, Vector3 dir);
	inline void changeParticleType(ProyType pT) { pType = pT; };

	ParticleGenerator* getParticleGenerator(std::string name);

	void generateFireworkSystem();
};

