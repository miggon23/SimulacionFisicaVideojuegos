#include "ParticleSystem.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"
#include "Particle.h"
#include "GravityForceGenerator.h"
#include "ParticleDragGenerator.h"
#include "UniformWindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "SinkForceGenerator.h"
#include <iostream>

ParticleSystem::ParticleSystem() : listP(0), activeGeneratorFollowCamera(false)
{


	generateFireworkSystem();

	////Generador que dispara el primer firework
	shared_ptr<GaussianParticleGenerator> g3(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 2.0, 1.0, 1.0 }, 2.0, { 0.1, -10, -14 }, {0.0 , 4.0, -8.0}, 1));
	addParticleGenerator(g3);
	g3->setMeanTime(4);
	g3->setGeneratorName("FireworkShooterGenerator");

	

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
		p1 = _particle_generators.erase(p1); //Al quedarse sin referencias el shared pointer, este se eliminará
	}

	/*while (!_force_generators.empty())
		_force_generators.pop_front();
	delete particleFR;*/
}

void ParticleSystem::update(double t)
{
	//particleFR->updateForces(t);

	for (auto p : listP) {
		p->integrate(t);
		//Intentamos cast dinamico para saber si es un firework

		/*if (p->getRemainingTime() <= 0 || p->getPos().y <= 0) {
			p->setAlive(false);		
		}*/
		if (p->getRemainingTime() <= 0) {
			p->setAlive(false);
		}
	}

	auto p = listP.begin();
	while (p != listP.end()) {
		if (!(*p)->isAlive()) {
			onParticleDeath(*p);
			delete *p;
			p = listP.erase(p);		
		}
		else
			p++;
	}

	////GENERADORES DE PARTÍCULA ACTIVOS
	//if (activeGenerator != nullptr) {
	//	auto lP = activeGenerator->generateParticles();
	//	for (auto particle : lP) {
	//		addParticle(particle);
	//		particleFR->addRegistry(gravityForceGenerator.get(), particle);
	//	}
	//	/*if (activeGeneratorFollowCamera) {
	//		activeGenerator->setMeanPos(GetCamera()->getEye() + GetCamera()->getDir() * 3);
	//		activeGenerator->setMeanVel(GetCamera()->getDir() * 40);
	//	}*/
	//}
}

void ParticleSystem::addParticle(Vector3 pos, Vector3 dir)
{
	addParticle(new Proyectil(pos, dir, pType, 1));
}

void ParticleSystem::addParticle(Particle* model)
{
	
	if (listP.size() < LIMIT_NUM_PARTICLE)
		listP.push_back(model);
	else
		delete model;
	
}

void ParticleSystem::activateGenerator(std::string s)
{
	/*if (activeGenerator == nullptr)
		activeGenerator = getParticleGenerator(s);
	else
		activeGenerator = nullptr;*/
}

void ParticleSystem::activateForceGenerator(std::string s)
{
	////Si no hay generador activo o el nombre que llega es de un nuevo generador
	//if (activeForceGenerator == nullptr || activeForceGenerator->getName() != s) {
	//	activeForceGenerator = getForceGenerator(s);
	//	if (activeForceGenerator == nullptr)
	//		std::cout << "No existe el generador " + s;
	//	else 
	//		std::cout << "Activado el generador " + s;
	//}
	//else {
	//	std::cout << "Desactivado el generador de fuerzas " + activeForceGenerator->getName();
	//	activeForceGenerator = nullptr;
	//}
	//std::cout << '\n';
}


void ParticleSystem::desactivateGenerator()
{
	//activeGenerator = nullptr;
}

void ParticleSystem::shootFirework(int type)
{
	auto gen = getParticleGenerator("FireworkShooterGenerator").get();
	if (gen == nullptr || type >= _firework_pool.size())
		return;
	//modelo
	auto model = _firework_pool[type]->clone();
	//model->setVel({ 0.0, 3.0, 0.0 });
	gen->setParticle(model);

	//asumimos que solo genera una partícula
	auto p = gen->generateParticles().front();
	//explota después de 5 segundos
	p->setRemainingTime(3);
	addParticle(p);
	
}

void ParticleSystem::onParticleDeath(Particle * p)
{
	//particleFR->deleteParticleRegistry(p);
	Firework* f;
	f = dynamic_cast<Firework*>(p);
	if (f != nullptr) {
		listP;
		for (auto firework : f->explode())
			addParticle(firework);
	}
}

void ParticleSystem::addParticleGenerator(shared_ptr<ParticleGenerator> pG)
{

	//AÑADIMOS EL NUEVO GENERADOR
	_particle_generators.push_back(pG);
}

void ParticleSystem::addForceGenerator(shared_ptr<ForceGenerator> pG)
{
	_force_generators.push_back(pG);
}


shared_ptr<ParticleGenerator> ParticleSystem::getParticleGenerator(std::string name)
{
	for(auto p : _particle_generators)
		if(p->getGeneratorName() == name)
			return p;
	return nullptr;
}

shared_ptr<ForceGenerator> ParticleSystem::getForceGenerator(std::string name)
{
	for (auto p : _force_generators)
		if (p->getName() == name)
			return p;
	return nullptr;
}

void ParticleSystem::generateFireworkSystem()
{
	//Particulas que simulan humo, no generan nada al morir
	auto pHumo = new Particle({ -10000.0, -10000.0, 0.0 }, { 0.0, 0.0, 0.0 }, _gravity, { 0.3, 0.2, 0.2, 0.8 }, 0.999, 1, 0.1);


	//Firewrok cero, solo genera humillo
	shared_ptr<ParticleGenerator> g1(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 1.0, 2.0, 1.0 }, 0.1, { 0.1, 0.1, 0.1 }, { 0.0, 18.0, 0.0 }, 4));
	g1->setParticle(pHumo);
	g1->setGeneratorName("BaseFireworkGenerator");
	g1->setMeanTime(0.3);
	addParticleGenerator(g1);
	auto fBase = new Firework({ -10000.0, -10000.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g1 }, 0.08, 2);
	fBase->setAcc({ 0.0, 0.0, 0.0 });
	fBase->setColor({ 0.7, 0.3, 0.5, 1.0 });
	_firework_pool.push_back(fBase);

	//Firework 1 --> genera fireworks 0
	shared_ptr<ParticleGenerator> g2(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 6.0, 6.0, 6.0 }, 0.1, { 0.1, 0.1, 0.1 }, {8.0, 8.0, 8.0}, 8));
	g2->setParticle(_firework_pool[0]);
	g2->setGeneratorName("FireworkGenerator1");
	g2->setMeanTime(0.4);
	addParticleGenerator(g2);
	auto p = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g2, g1 }, 0.15, 2);
	//p->setChangingColor(true);
	p->setColor({ 0.4, 0.2, 0.8, 1.0 });
	p->setAcc({ 0.0, 0.0, 0.0 });
	_firework_pool.push_back(p);

	//Genera Firework 2 y humo
	shared_ptr<ParticleGenerator> g3(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 8.0, 8.0, 8.0 }, 0.1, { 0.1, 0.1, 0.1 }, {0.0, 12.0, 0.0}, 6));
	g3->setParticle(_firework_pool[1]);
	g3->setGeneratorName("FireworkGenerator2");
	g3->setMeanTime(0.5);
	auto p1 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g3 }, 0.2, 2);
	p1->setColor({ 0.2, 0.5, 0.2, 1.0 });
	p1->setAcc({ 0.0, 0.0, 0.0 });
	_firework_pool.push_back(p1);

	//Bola arcoíris, va cambiando de color
	shared_ptr<ParticleGenerator> g4(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 8.0, 8.0, 8.0 }, 0.12, { 0.1, 0.1, 0.1 }, { 0.0, 12.0, 0.0 }, 4));
	g4->setParticle(_firework_pool[2]);
	g4->setGeneratorName("FireworkGenerator3");
	g4->setMeanTime(0.6);
	auto p2 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g4, g2 }, 0.3, 2);
	p2->setChangingColor(true, 0.4);
	p2->setAcc({ 0.0, 0.0, 0.0 });
	p2->setColor({ 0.2, 0.5, 0.3, 1.0 });
	_firework_pool.push_back(p2);

	//Ultima bola
	shared_ptr<ParticleGenerator> g5(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 2.0, 2.0, 2.0 }, 0.12, { 0.1, 0.1, 0.1 }, { 0.0, 3.0, 0.0 }, 3));
	g5->setParticle(_firework_pool[3]);
	g5->setGeneratorName("FireworkGenerator4");
	g5->setMeanTime(0.6);
	auto p3 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g5, g3 }, 0.45, 2);
	//p3->setChangingColor(true, 0.55);
	p3->setColor({ 0.2, 0.6, 0.5, 1.0 });
	p3->setAcc({ 0.0, 0.0, 0.0 });
	_firework_pool.push_back(p3);

	
}

void ParticleSystem::generateForcesGenerators()
{
	//shared_ptr<ForceGenerator> gF(new GravityForceGenerator({ 0.0, -9.8, 0.0 }));
	//gF->setName("GravityGenerator");
	//addForceGenerator(gF);
	//gravityForceGenerator = gF;

	//shared_ptr<ForceGenerator> gD(new ParticleDragGenerator(0.2, 0.4));
	//gD->setName("DragGenerator");
	//addForceGenerator(gD);

	//shared_ptr<ForceGenerator> gW(new UniformWindGenerator({ 0.0, 20.0, 0.0 }, { 20.0, 20.0, 20.0 }, 0.2, 0.4, {0.0, 0.0, 4.0}));
	//gW->setName("WindGenerator");
	//addForceGenerator(gW);

	//shared_ptr<ForceGenerator> gwW(new WhirlwindGenerator(0.1, 0.2, 4.0, { 0.0, 0.0, 0.0 }));
	//gwW->setName("WhirlwindGenerator");
	//addForceGenerator(gwW);

	//shared_ptr<ForceGenerator> gE(new ExplosionForceGenerator(100, 20000, 1.0, { 0.0, 0.0, 0.0 }));
	//gE->setName("ExplosionGenerator");
	//addForceGenerator(gE);

	///*shared_ptr<ForceGenerator> gAnch(new AnchoredSpringFG(20.0, 10.0, {0.0, 50.0, 0.0}));
	//gAnch->setName("AnchoredSpringFG");
	//addForceGenerator(gAnch);*/

	//shared_ptr<ForceGenerator> gSink(new SinkForceGenerator(1, 1000.0, { 0.0, 10.0, 0.0 }));
	//gSink->setName("SinkFG");
	//addForceGenerator(gSink);

}


