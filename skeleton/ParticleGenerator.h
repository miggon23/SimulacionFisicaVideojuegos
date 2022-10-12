#pragma once
#include <string>
#include "Particle.h"
#include <vector>
#include <list>

class ParticleGenerator
{
protected:
	std::string _name;
	Vector3 _mean_pos, _mean_vel;
	double _generation_prob;
	int _num_particles;
	Particle* _model;
public:
	void setParticle(Particle* model);
	virtual std::list<Particle*> generateParticles() = 0;

	inline std::string getGeneratorName() { return _name; };
};

