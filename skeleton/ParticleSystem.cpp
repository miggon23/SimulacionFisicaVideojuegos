#include "ParticleSystem.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"

ParticleSystem::ParticleSystem() : listP(0)
{
	//Generador uniforme con velocidades dispersas pero con la posicion muy homogenes en el origen
	//_particle_generators.push_back(new UniformParticleGenerator({ 6.0, 3.0, 6.0 }, { 1.0, 1.0, 1.0 }));
	auto p = addParticleGenerator(new UniformParticleGenerator({ 300.0, 150.0, 300.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 80.0, 0.0 }, {0.0, 0.0, 0.0}, 16));
	p->setParticle(new Particle({ 0.0, 20000.0, 0.0 }, {0.0, 0.0, 0.0}, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0, 1.0 }, 0.999, 0));
	//Generador con los mismos parámetros per con distribucion uniforme
	p = addParticleGenerator(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 3.0, 2.0, 3.0 }, 2.0, GetCamera()->getEye(), GetCamera()->getDir() * 20, 2));
	p->setParticle(new Particle({ 0.0, 20000.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, -10.0, 0.0 }, { 0.0, 0.4, 0.6, 1.0 }, 0.999, 0));

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
		if (p->getRemainingTime() <= 0 || p->getPos().y <= -0)
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

	//GENERADORES DE PARTÍCULA ACTIVOS
	if (activeGenerator != nullptr) {
		auto lP = activeGenerator->generateParticles();
		for (auto particle : lP)
			listP.push_back(particle);
		activeGenerator->setMeanPos(GetCamera()->getEye() + GetCamera()->getDir() * 3);
		activeGenerator->setMeanVel(GetCamera()->getDir() * 30);
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

void ParticleSystem::activateGenerator(std::string s)
{
	if (activeGenerator == nullptr)
		activeGenerator = getParticleGenerator(s);
	else
		activeGenerator = nullptr;
}

void ParticleSystem::desactivateGenerator()
{
	//activeGenerator = nullptr;
}

ParticleGenerator* ParticleSystem::addParticleGenerator(ParticleGenerator* pG)
{
	//BORRADO DEL GENERADOR SI YA ESTABA CREADO
	auto p = _particle_generators.begin();
	while (p != _particle_generators.end() && (*p)->getGeneratorName() != pG->getGeneratorName())
		p++;
	if (p != _particle_generators.end())//Ha encontrado ese generador con el mismo nombre
	{
		delete *p;
		_particle_generators.erase(p);
	}

	//AÑADIMOS EL NUEVO GENERADOR
	_particle_generators.push_back(pG);
	return pG;
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
