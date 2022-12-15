#include "WorldManager.h"
#include "UniformRBGenerator.h"
#include "GaussianRBGenerator.h"
#include "ExplosionForceGenerator.h"
#include <time.h>
#include <iostream>

using namespace physx;

WorldManager::WorldManager(PxPhysics* gPhysics, PxScene* gScene) : _gPhysics(gPhysics), _gScene(gScene)
{
}

void WorldManager::initWorld()
{
	particleFR = new ParticleForceRegistry();

	PxRigidStatic* Suelo = _gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	Suelo->attachShape(*shape);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 });
	_gScene->addActor(*Suelo);

	//Add wall
	PxRigidStatic* Pared = _gPhysics->createRigidStatic(PxTransform({ 10, 10, -10 }));
	PxShape* shape_suelo = CreateShape(PxBoxGeometry(40, 20, 5));
	Pared->attachShape(*shape_suelo);
	//itemRenderList.push_back(new RenderItem(shape_suelo, Pared, { 0.8, 0.8, 0.8,1 }));
	new RenderItem(shape_suelo, Pared, { 0.8, 0.8, 0.8,1 });
	_gScene->addActor(*Pared);

	shared_ptr<ForceGenerator> gE(new ExplosionForceGenerator(100, 20000, 1.0, { 0.0, 0.0, 0.0 }));
	gE->setName("ExplosionGenerator");
	addForceGenerator(gE);

	shared_ptr<GaussianRBGenerator> g(new GaussianRBGenerator(this, { 0.1, 0.1, 0.1 }, { 3.0, 2.0, 3.0 }, 2.0, { 0.0, 10.0, 0.0 }, {0.0 , 50, 0.0}, 1));
	addParticleGenerator(g);
	g->setMeanTime(4);
	g->setGeneratorName("FireworkShooterGenerator");
}

void WorldManager::addDynamicRigidBody(Vector3 size, Vector3 p, Vector3 vel, Vector4 color, unsigned int time)
{
	if (_gScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC) >= NUM_DYN_OBJ_LIMIT)
		return;

	auto rD = new RigidDynamic();
	PxRigidDynamic* new_solid;
	new_solid = _gPhysics->createRigidDynamic(PxTransform(p));
	new_solid->setLinearVelocity(vel);
	new_solid->setAngularVelocity({ 0, 0, 0 });
	auto shape = CreateShape(PxBoxGeometry(size));
	new_solid->attachShape(*shape);
	new_solid->setMassSpaceInertiaTensor({ size.y * size.z, size.x * size.z, size.x * size.y });
	rD->rigidDynamic = new_solid;
	rD->rItem = new RenderItem(shape, new_solid, color);
	rD->alive = true;
	rD->deathTime = time;
	rD->maxTime = clock() + time;
	itemList.push_back(rD);
	//_gScene->addActor(*new_solid);
	addToScene(new_solid);

	//new_solid->putToSleep();
}

void WorldManager::deleteDynamicRigidBody(RigidDynamic* rD)
{
	delete rD;
}

void WorldManager::update(float dt)
{
	particleFR->updateForces(dt);

	for (auto p : itemList) {
		//Intentamos cast dinamico para saber si es un firework

		/*if (p->getRemainingTime() <= 0 || p->getPos().y <= 0) {
			p->setAlive(false);		
		}*/
		if (clock() >= p->maxTime) {
			p->alive = false;
		}
	}

	auto p = itemList.begin();
	while (p != itemList.end()) {
		if (!(*p)->alive) {
			deleteDynamicRigidBody(*p);
			p = itemList.erase(p);
		}
		else
			p++;
	}
}

shared_ptr<RigidBodyGenerator> WorldManager::getParticleGenerator(std::string name)
{
	for (auto p : _particle_generators)
		if (p->getGeneratorName() == name)
			return p;
	return nullptr;
}

shared_ptr<ForceGenerator> WorldManager::getForceGenerator(std::string name)
{
	for (auto p : _force_generators)
		if (p->getName() == name)
			return p;
	return nullptr;
}

inline void WorldManager::addToScene(PxRigidDynamic* p)
{
	if (_activeForceGenerator != nullptr) {
		particleFR->addRegistry(_activeForceGenerator.get(), p);
	}
	_gScene->addActor(*p);
}

void WorldManager::activateForceGenerator(std::string s)
{
	//Si no hay generador activo o el nombre que llega es de un nuevo generador
	if (_activeForceGenerator == nullptr || _activeForceGenerator->getName() != s) {
		_activeForceGenerator = getForceGenerator(s);
		if (_activeForceGenerator == nullptr)
			std::cout << "No existe el generador " + s;
		else 
			std::cout << "Activado el generador " + s;
	}
	else {
		std::cout << "Desactivado el generador de fuerzas " + _activeForceGenerator->getName();
		_activeForceGenerator = nullptr;
	}
	std::cout << '\n';
}

RigidDynamic* WorldManager::clone(RigidDynamic* rD)
{
	auto rigD = new RigidDynamic();
	rigD->deathTime = rD->deathTime;
	rigD->maxTime = clock() + rD->deathTime;
	rigD->alive = true;

	auto pos = rD->rItem->actor->getGlobalPose();
	auto new_solid = _gPhysics->createRigidDynamic(pos);
	rigD->rigidDynamic = new_solid;
	auto g = rD->rItem->shape->getGeometryType();

	Vector3 size = { 2.0, 2.0, 2.0 };
	new_solid->setMassSpaceInertiaTensor({ size.y * size.z, size.x * size.z, size.x * size.y });

	PxShape* shape;
	switch (g)
	{
	case physx::PxGeometryType::eSPHERE:
		shape = CreateShape(PxSphereGeometry(size.x / 2));
		break;
	case physx::PxGeometryType::eBOX:
		shape = CreateShape(PxBoxGeometry(size));
		break;
	default:
		break;
	}
	new_solid->attachShape(*shape);
	Vector4 color = rD->rItem->color;
	rigD->rItem = new RenderItem(shape, new_solid, color);
	return rigD;
}
