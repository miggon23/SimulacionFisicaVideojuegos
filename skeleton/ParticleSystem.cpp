#include "ParticleSystem.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"

ParticleSystem::ParticleSystem() : listP(0)
{
	//Generador uniforme con velocidades dispersas pero con la posicion muy homogenes en el origen
	_particle_generators.push_back(new UniformParticleGenerator({ 2.0, 3.0, 3.0 }, { 1.0, 1.0, 1.0 }));
	_particle_generators.push_back(new GaussianParticleGenerator());
}

ParticleSystem::~ParticleSystem()
{
	auto p = listP.begin();
	while (!listP.empty()) {	
		delete* p;
		p = listP.erase(p);				
	}

	auto p1 = _particle_generators.begin();
	while (!_particle_generators.empty()) {
		delete* p1;
		p1 = _particle_generators.erase(p1);
	}
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

void ParticleSystem::addParticle(Particle* model)
{
	listP.push_back(model); //Model o Model.copy() --> Implementar Model.copy()
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
