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
	double _generation_prob, meanTime;
	int _num_particles;
	Particle* _model = nullptr;
public:
	ParticleGenerator(std::string nombre, Vector3 velM, Vector3 posM, int nPart);
	~ParticleGenerator();
	inline void setParticle(Particle* model) {	_model = model;	};
	inline void setMeanPos(Vector3 pos) { _mean_pos = pos; };
	inline void setMeanVel(Vector3 vel) { _mean_vel = vel; };
	inline void setMeanTime(double T) { meanTime = T; };
	inline void setNParticles(int n) { _num_particles = n; };
	virtual std::list<Particle*> generateParticles() = 0;

	inline Vector3 getMeanPos() const { return _mean_pos; };
	inline Vector3 getMeanVel() const { return _mean_vel; };

	inline std::string getGeneratorName() { return _name; };
	inline void setGeneratorName(std::string s) { _name = s; };
};

