#include "ParticleSystem.h"
#include "UniformParticleGenerator.h"
#include "GaussianParticleGenerator.h"

ParticleSystem::ParticleSystem() : listP(0), activeGeneratorFollowCamera(false)
{
	//Generador uniforme con velocidades dispersas pero con la posicion muy homogenes en el origen
	//_particle_generators.push_back(new UniformParticleGenerator({ 6.0, 3.0, 6.0 }, { 1.0, 1.0, 1.0 }));
	shared_ptr<UniformParticleGenerator> g1(new UniformParticleGenerator({ 300.0, 150.0, 300.0 }, { 1.0, 1.0, 1.0 }, { 0.0, 80.0, 0.0 }, { 0.0, 0.0, 0.0 }, 20));
	addParticleGenerator(g1);
	g1->setMeanTime(4);
	g1->setParticle(new Particle({ 0.0, 20000.0, 0.0 }, {0.0, 0.0, 0.0}, { 0.0, 0.0, 0.0 }, { 0.5, 0.05, 0.0, 1.0 }, 0.999, 0, 0.2));
	//Generador con los mismos parámetros per con distribucion uniforme

	shared_ptr<GaussianParticleGenerator> g2(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 3.0, 2.0, 3.0 }, 2.0, GetCamera()->getEye(), GetCamera()->getDir() * 20, 2));
	addParticleGenerator(g2);
	g2->setMeanTime(4);
	g2->setParticle(new Particle({ 0.0, 20000.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, -10.0, 0.0 }, { 0.0, 0.4, 0.6, 1.0 }, 0.999, 0, 0.5));

	generateFireworkSystem();

	shared_ptr<GaussianParticleGenerator> g3(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 3.0, 2.0, 3.0 }, 2.0, { 0.0, 10.0, 0.0 }, {0.0 , 50, 0.0}, 1));
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
}

void ParticleSystem::update(double t)
{
	for (auto p : listP) {
		p->integrate(t);
		//Intentamos cast dinamico para saber si es un firework

		if (p->getRemainingTime() <= 0 || p->getPos().y <= 0) {
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

	//GENERADORES DE PARTÍCULA ACTIVOS
	if (activeGenerator != nullptr) {
		auto lP = activeGenerator->generateParticles();
		for (auto particle : lP)
			listP.push_back(particle);
		if (activeGeneratorFollowCamera) {
			activeGenerator->setMeanPos(GetCamera()->getEye() + GetCamera()->getDir() * 3);
			activeGenerator->setMeanVel(GetCamera()->getDir() * 40);
		}
	}
}

void ParticleSystem::addParticle(Vector3 pos, Vector3 dir)
{
	listP.push_back(new Proyectil(pos, dir, pType, 1));
}

void ParticleSystem::addParticle(Particle* model)
{
	listP.push_back(model);
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

void ParticleSystem::shootFirework(int type)
{
	auto gen = getParticleGenerator("FireworkShooterGenerator").get();
	if (gen == nullptr || type >= _firework_pool.size())
		return;
	//MODELO
	auto model = _firework_pool[type]->clone();
	gen->setParticle(model);
	/*if(gen != nullptr)
		model->addGenerator(gen);
	gen.get()->setParticle(model);*/

	//model->setColor({0.2, 0.2, 0.8, 1.0});
	//model->setAcc({ 0.0, -10.0, 0.0 });
	/*Firework* f = dynamic_cast<Firework*>(model->clone());
	f->setPos({ 0.0, 10.0, 0.0 });
	f->setVel({ 0.0, 50.0, 0.0 });*/
	//Asumimos que solo genera una partícula
	auto p = gen->generateParticles().front();
	//Aumentamos un segundo su tiempo base de explosión para que dure más en el disparo
	//p->setRemainingTime(p->getRemainingTime() + 0.8);
	p->setRemainingTime(5);
	addParticle(p);
	
}

void ParticleSystem::onParticleDeath(Particle * p)
{
	Firework* f;
	f = dynamic_cast<Firework*>(p);
	if (f != nullptr) {
		listP;
		auto l  = f->explode();
		for (auto firework : l)
			addParticle(firework);
	}
}

void ParticleSystem::addParticleGenerator(shared_ptr<ParticleGenerator> pG)
{

	//AÑADIMOS EL NUEVO GENERADOR
	_particle_generators.push_back(pG);
}

shared_ptr<ParticleGenerator> ParticleSystem::getParticleGenerator(std::string name)
{
	for(auto p : _particle_generators)
		if(p->getGeneratorName() == name)
			return p;
	return nullptr;
}

void ParticleSystem::generateFireworkSystem()
{
	//Particulas que simulan humo, no generan nada al morir
	auto pHumo = new Particle({ -10000.0, -10000.0, 0.0 }, { 0.0, 0.0, 0.0 }, _gravity, { 0.3, 0.2, 0.2, 0.8 }, 0.999, 1, 0.1);


	//Firewrok cero, solo genera humillo
	shared_ptr<ParticleGenerator> g1(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 1.0, 2.0, 1.0 }, 0.12, { 0.1, 0.1, 0.1 }, { 0.0, 18.0, 0.0 }, 4));
	g1->setParticle(pHumo);
	g1->setGeneratorName("BaseFireworkGenerator");
	g1->setMeanTime(0.4);
	addParticleGenerator(g1);
	auto fBase = new Firework({ -10000.0, -10000.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g1 }, 0.2, 2);
	fBase->setAcc(_gravity);
	fBase->setColor({ 0.7, 0.3, 0.5, 1.0 });
	_firework_pool.push_back(fBase);

	//Firework 1 --> genera fireworks 0
	shared_ptr<ParticleGenerator> g2(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 6.0, 6.0, 6.0 }, 0.12, { 0.1, 0.1, 0.1 }, {8.0, 8.0, 8.0}, 7));
	g2->setParticle(_firework_pool[0]);
	g2->setGeneratorName("FireworkGenerator1");
	g2->setMeanTime(0.8);
	addParticleGenerator(g2);
	auto p = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g2, g1 }, 0.3, 2);
	//p->setChangingColor(true);
	p->setColor({ 0.4, 0.2, 0.8, 1.0 });
	p->setAcc(_gravity);
	_firework_pool.push_back(p);

	//Genera Firework 2 y humo
	shared_ptr<ParticleGenerator> g3(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 8.0, 8.0, 8.0 }, 0.12, { 0.1, 0.1, 0.1 }, {0.0, 12.0, 0.0}, 5));
	g3->setParticle(_firework_pool[1]);
	g3->setGeneratorName("FireworkGenerator2");
	g3->setMeanTime(1.0);
	auto p1 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g3 }, 0.4, 2);
	p1->setColor({ 0.2, 0.5, 0.2, 1.0 });
	p1->setAcc(_gravity);
	_firework_pool.push_back(p1);

	//Bola arcoíris Genea verde, morado, rosa (uva, piña, pera)
	shared_ptr<ParticleGenerator> g4(new GaussianParticleGenerator({ 0.1, 0.1, 0.1 }, { 8.0, 8.0, 8.0 }, 0.12, { 0.1, 0.1, 0.1 }, { 0.0, 12.0, 0.0 }, 3));
	g4->setParticle(_firework_pool[2]);
	g4->setGeneratorName("FireworkGenerator3");
	g4->setMeanTime(1.2);
	auto p2 = new Firework({ -10000.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { g4, g2 }, 0.6, 2);
	p2->setChangingColor(true, 0.4);
	p2->setColor({ 0.2, 0.5, 0.3, 1.0 });
	p2->setAcc(_gravity);
	_firework_pool.push_back(p2);

}
