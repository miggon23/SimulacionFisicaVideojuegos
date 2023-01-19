#include "WorldManager.h"
#include "UniformRBGenerator.h"
#include "GaussianRBGenerator.h"
#include "ExplosionForceGenerator.h"
#include "TorqueForceGenerator.h"
#include <time.h>
#include <iostream>

using namespace physx;

WorldManager::WorldManager(PxPhysics* gPhysics, PxScene* gScene) : _gPhysics(gPhysics), _gScene(gScene)
{
	_gen_delay = 600;
	_next_gen = clock() + _gen_delay; //Miliseconds
}

WorldManager::~WorldManager()
{
	delete tableroJuego;
	delete particleFR;
}

void WorldManager::initWorld()
{
	tableroJuego = new Tablero; 
	tableroJuego->width = 3.6;
	tableroJuego->height = 4.0;
	tableroJuego->rot = 0.6;
	particleFR = new ParticleForceRegistry();

	//Añadir tablero de pinball, rotado 20º
	PxRigidStatic* Suelo = _gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shape = CreateShape(PxBoxGeometry(tableroJuego->width, 0.3, tableroJuego->height));
	Suelo->attachShape(*shape);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape, Suelo, { 0.5, 0.25, 0.9, 1 });
	_gScene->addActor(*Suelo);

	PxRigidStatic* ParedBaja1 = _gPhysics->createRigidStatic(PxTransform({ -2, -1.4, 3.2 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shape1 = CreateShape(PxBoxGeometry(tableroJuego->width/2.5, 0.3, 0.3));
	ParedBaja1->attachShape(*shape1);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape1, ParedBaja1, { 0.5, 0.25, 0.9, 1 });
	_gScene->addActor(*ParedBaja1);

	PxRigidStatic* ParedBaja2 = _gPhysics->createRigidStatic(PxTransform({2, -1.4, 3.2 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shape2 = CreateShape(PxBoxGeometry(tableroJuego->width / 2.5, 0.3, 0.3));
	ParedBaja2->attachShape(*shape2);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape2, ParedBaja2, { 0.5, 0.25, 0.9, 1 });
	_gScene->addActor(*ParedBaja2);

	PxRigidStatic* ParedLateral1 = _gPhysics->createRigidStatic(PxTransform({ -3.5, 1.0, 0.0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shape3 = CreateShape(PxBoxGeometry(0.3, 0.3, tableroJuego->height));
	ParedLateral1->attachShape(*shape3);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape3, ParedLateral1, { 0.5, 0.25, 0.9, 1 });
	_gScene->addActor(*ParedLateral1);

	PxRigidStatic* ParedLateral2 = _gPhysics->createRigidStatic(PxTransform({ 3.5, 1.0, 0.0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shape4 = CreateShape(PxBoxGeometry(0.3, 0.3, tableroJuego->height));
	ParedLateral2->attachShape(*shape4);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape4, ParedLateral2, { 0.5, 0.25, 0.9, 1 });
	_gScene->addActor(*ParedLateral2);
	
	//Add wall
	//PxRigidStatic* Pared = _gPhysics->createRigidStatic(PxTransform({ 10, 10, -10 }));
	//PxShape* shape_suelo = CreateShape(PxBoxGeometry(40, 20, 5));
	//Pared->attachShape(*shape_suelo);
	////itemRenderList.push_back(new RenderItem(shape_suelo, Pared, { 0.8, 0.8, 0.8,1 }));
	//new RenderItem(shape_suelo, Pared, { 0.8, 0.8, 0.8,1 });
	//_gScene->addActor(*Pared);

	shared_ptr<ForceGenerator> gE(new ExplosionForceGenerator(100, 20000, 1.0, { 0.0, 0.0, 0.0 }, false));
	gE->setName("ExplosionGenerator");
	addForceGenerator(gE);

	shared_ptr<TorqueForceGenerator> gT(new TorqueForceGenerator(2.0, 80.0, {5.0, 0.0, 0.0}, false));
	gT->setName("TorqueGenerator");
	addForceGenerator(gT);



	// -------

	PxRigidDynamic* new_solid;
	Vector3 size = {0.0, 7.0, 7.0 };

	PxQuat q = { 45, PxVec3 {0, 1, 0} }; // 45 grados girado en Y
	PxQuat q2 = { 10, PxVec3 {1, 0, 0} }; // 10 grados girado en X
	new_solid = _gPhysics->createRigidDynamic(PxTransform({ 0, 1, 0 }, q*q2)); // añadir quaternion para generar con giro

	new_solid->setLinearVelocity({ 0.0, 0.0, 0.0 }); // velocidad inicial
	new_solid->setAngularVelocity({ 0.0, 0.0, 0.0 }); // velocidad de giro
	
	shape = CreateShape(PxSphereGeometry(0.1));
	//auto shape = CreateShape(PxSphereGeometry(2.0));
	new_solid->attachShape(*shape);

	new_solid->setMassSpaceInertiaTensor({ size.y * size.z, size.x * size.z, size.x * size.y }); // tensor de inercia, marca cómo gira el objeto al chocar

//    new_solid->setMass(1.0);

	auto item = new RenderItem(shape, new_solid, { 1.0, 0.0, 1.0, 1.0 });
//	_items.push_back(item);

	addToScene(new_solid);

	particleFR->addRegistry(gT.get(), new_solid);
	particleFR->addRegistry(gE.get(), new_solid);

	RigidDynamic* model = new RigidDynamic();
	model->rigidDynamic = new_solid;
	model->rItem = item;
	model->maxTime = MAXINT;
	model->alive = true;
	model->deathTime = MAXINT;
	model->forcesName.push_back(gT->getName());
	model->forcesName.push_back(gE->getName());

	shared_ptr<GaussianRBGenerator> g(new GaussianRBGenerator(this, { 2, 0.1, 5 }, { 3.0, 3.0, 3.0 }, 2.0, { -10.0, 2.0, 10.0 }, { 14.0 , 15.0, 0.0 }, 1, false));
	addParticleGenerator(g);
	g->setMeanTime(7);
	g->setGeneratorName("GaussianGenerator");
	g->setParticle(model);
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
	DeregisterRenderItem(rD->rItem);
	delete rD;
}

void WorldManager::update(float dt)
{
	particleFR->updateForces(dt);


	if (clock() >= _next_gen)
	{
		for (auto pG : _particle_generators)
		{
			if (pG->getActive())
			{
				auto l = pG->generateParticles();
				for (auto p : l) {
					itemList.push_back(p);
					_gScene->addActor(*p->rigidDynamic);
				}
			}
		}
		_next_gen = clock() + _gen_delay;
	}

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
	
	_gScene->addActor(*p);
}

void WorldManager::activateForceGenerator(std::string s)
{

	auto fG = getForceGenerator(s);
	if (fG != nullptr) {
		bool b = fG->getActive();
		fG->setActive(!b);
		if(b)
			std::cout << "Desactivado el generador de fuerzas " + fG->getName();
		else
			std::cout << "Activado el generador " + s;
		
	}
	else
		std::cout << "No existe el generador " + s;

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

	for (auto s : rD->forcesName) {
		particleFR->addRegistry(getForceGenerator(s).get(), rigD->rigidDynamic);
		rigD->forcesName.push_back(s);
	}

	return rigD;
}
