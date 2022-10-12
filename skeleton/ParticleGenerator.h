#pragma once
#include <string>
#include "Particle.h"
#include <vector>
#include <list>

class ParticleGenerator
{
protected:
	std::string _name;
	Vector3 _mean_pos{ 0.0, 50.0, 0.0 }, _mean_vel{20, 10, 20};
	double _generation_prob;
	int _num_particles = 6;
	Particle* _model;
public:
	ParticleGenerator(std::string nombre);
	void setParticle(Particle* model);
	virtual std::list<Particle*> generateParticles() = 0;

	inline std::string getGeneratorName() { return _name; };
};

