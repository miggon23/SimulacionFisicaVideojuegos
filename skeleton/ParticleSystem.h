#pragma once
#include "Particle.h"
#include <list>
#include "ParticleGenerator.h"
class ParticleSystem
{
private:
	ProyType pType = PAINT_BALL;
	std::list<Particle*> listP;

	std::vector<Firework*> _firework_pool;
	std::list<shared_ptr<ParticleGenerator>> _particle_generators;

	//std::list<ParticleGenerator*> _active_particle_generators;
	shared_ptr<ParticleGenerator> activeGenerator = nullptr;
	Vector3 _gravity{0.0, -10.0, 0.0};
public:
	ParticleSystem();
	~ParticleSystem();

	void update(double t);
	void addParticle(Vector3 pos, Vector3 dir);
	void addParticle(Particle* model);
	inline void changeParticleType(ProyType pT) { pType = pT; };
	inline void addParticlesFromList(std::list<Particle*> l) { listP.merge(l); };
	void activateGenerator(std::string s);
	void desactivateGenerator();

	void shootFirework(int type = 0);
	void onParticleDeath(Particle* p);

	void addParticleGenerator(shared_ptr<ParticleGenerator> pG);
	shared_ptr<ParticleGenerator> getParticleGenerator(std::string name);

	inline void setGeneratorToFollowCamera(bool b) { activeGeneratorFollowCamera = b; };
	inline bool getGeneratorFollowCamera() { return activeGeneratorFollowCamera; };

	void generateFireworkSystem();
protected:
	bool activeGeneratorFollowCamera;
};

