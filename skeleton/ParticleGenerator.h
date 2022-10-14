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
	int _num_particles = 6;
	Particle* _model;
public:
	ParticleGenerator(std::string nombre, Vector3 velM, Vector3 posM, int nPart);
	~ParticleGenerator();
	inline void setParticle(Particle* model) {	_model = model;	};
	inline void setMeanPos(Vector3 pos) { _mean_pos = pos; };
	inline void setMeanVel(Vector3 vel) { _mean_vel = vel; };
	virtual std::list<Particle*> generateParticles() = 0;

	inline std::string getGeneratorName() { return _name; };
};

