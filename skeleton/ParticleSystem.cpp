#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : listP(0)
{
}

ParticleSystem::~ParticleSystem()
{
	listP.erase(listP.begin(), listP.end());
}

void ParticleSystem::update(double t)
{
	for (auto p : listP) {
		p->integrate(t);
		if (p->getPos().y <= -0)
			p->setAlive(false);		
	}

	auto p = listP.begin();
	while (p != listP.end()) {
		if (!(*p)->isAlive()) {
			delete *p;
			p = listP.erase(p);
		}
		else
			p++;
	}
}

void ParticleSystem::addParticle(Vector3 pos, Vector3 dir)
{
	listP.push_back(new Proyectil(pos, dir, pType, 1));
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
	for(auto p : _particle_generators)
		if(p->getGeneratorName() == name)
			return p;
	return nullptr;
}

void ParticleSystem::generateFireworkSystem()
{
}
