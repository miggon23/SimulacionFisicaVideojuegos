#pragma once
#include <string>
#include "Particle.h"
#include <vector>
#include <list>

class WorldManager;
class RigidDynamic;

class RigidBodyGenerator
{
protected:
	std::string _name;
	Vector3 _mean_pos, _mean_vel;
	double _generation_prob, meanTime;
	int _num_particles;
	RigidDynamic* _model = nullptr;
	int limitOfParticles;

	WorldManager* _wM;
public:
	RigidBodyGenerator(WorldManager* wM, std::string nombre, Vector3 velM, Vector3 posM, int nPart);
	~RigidBodyGenerator();
	inline void setParticle(RigidDynamic* model) { _model = model; };
	inline void setMeanPos(Vector3 pos) { _mean_pos = pos; };
	inline void setMeanVel(Vector3 vel) { _mean_vel = vel; };
	inline void setMeanTime(double T) { meanTime = T; };
	inline void setNParticles(int n) {
		if (n < 0)
			n = 0;
		else if (n > limitOfParticles)
			n = limitOfParticles;
		_num_particles = n;
	};
	inline int getNParticles() { return _num_particles; };
	virtual std::list<RigidDynamic*> generateParticles() = 0;
	inline void setLimitOfParticlesPerFrame(int n) { limitOfParticles = n; };

	inline Vector3 getMeanPos() const { return _mean_pos; };
	inline Vector3 getMeanVel() const { return _mean_vel; };

	inline std::string getGeneratorName() { return _name; };
	inline void setGeneratorName(std::string s) { _name = s; };
};