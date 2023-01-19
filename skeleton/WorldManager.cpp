#include "WorldManager.h"
#include "UniformRBGenerator.h"
#include "GaussianRBGenerator.h"
#include "ExplosionForceGenerator.h"
#include "UniformWindGenerator.h"
#include "TorqueForceGenerator.h"
#include <time.h>
#include <iostream>
#include "MuelleIniciador.h"
#include "Player.h"

using namespace physx;	

void WorldManager::setUpTablero()
{

	tableroJuego = new Tablero;
	tableroJuego->width = 3.6;
	tableroJuego->height = 4.6;
	tableroJuego->rot = 0.6;
	tableroJuego->color = { 0.5, 0.1, 0.1, 1 };
	tableroJuego->colorsaque = { 0.1, 0.1, 0.4, 1 };
	particleFR = new ParticleForceRegistry();

	//Añadir tablero de pinball, rotado 20º
	PxRigidStatic* Suelo = _gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shape = CreateShape(PxBoxGeometry(tableroJuego->width, 0.4, tableroJuego->height));
	Suelo->attachShape(*shape);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape, Suelo, tableroJuego->color);
	_gScene->addActor(*Suelo);

	PxRigidStatic* techo = _gPhysics->createRigidStatic(PxTransform({ 0, 2, 0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapeTecho = CreateShape(PxBoxGeometry(tableroJuego->width, 0.2, tableroJuego->height*1.5));
	Suelo->attachShape(*shapeTecho);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	//new RenderItem(shapeTecho, techo, tableroJuego->color);
	_gScene->addActor(*techo);

	PxQuat pQuatParedBaja1 = { 0.7, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedBaja1 = _gPhysics->createRigidStatic(PxTransform({ -2, -1.4, 3.2 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} } *pQuatParedBaja1));
	PxShape* shape1 = CreateShape(PxBoxGeometry(tableroJuego->width / 6, 0.3, 0.3));
	ParedBaja1->attachShape(*shape1);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape1, ParedBaja1, tableroJuego->color * 0.4);
	_gScene->addActor(*ParedBaja1);

	PxQuat pQuatParedBaja2 = { 0.7, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedBaja2 = _gPhysics->createRigidStatic(PxTransform({ 2, -1.4, 3.2 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} } *pQuatParedBaja2));
	PxShape* shape2 = CreateShape(PxBoxGeometry(tableroJuego->width / 6, 0.3, 0.3));
	ParedBaja2->attachShape(*shape2);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape2, ParedBaja2, tableroJuego->color*0.4);
	_gScene->addActor(*ParedBaja2);

	PxQuat pQuatParedBaja3 = { 0.7, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedBaja3 = _gPhysics->createRigidStatic(PxTransform({ 0, -0.8, 1.6 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} } *pQuatParedBaja3));
	PxShape* shapePB = CreateShape(PxBoxGeometry(tableroJuego->width / 10, 0.3, 0.3));
	ParedBaja3->attachShape(*shape1);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapePB, ParedBaja3, tableroJuego->color * 0.4);
	_gScene->addActor(*ParedBaja3);

	PxRigidStatic* ParedAlta = _gPhysics->createRigidStatic(PxTransform({ 0, 3.26, -3.3 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapePAlta = CreateShape(PxBoxGeometry(tableroJuego->width, 0.5, 0.3));
	ParedAlta->attachShape(*shapePAlta);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapePAlta, ParedAlta, tableroJuego->color * 0.4);
	_gScene->addActor(*ParedAlta);

	PxRigidStatic* ParedLateral1 = _gPhysics->createRigidStatic(PxTransform({ -3.5, 1.0, 0.0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shape3 = CreateShape(PxBoxGeometry(0.3, 0.5, tableroJuego->height*0.94));
	ParedLateral1->attachShape(*shape3);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape3, ParedLateral1, tableroJuego->color * 0.4);
	_gScene->addActor(*ParedLateral1);

	PxRigidStatic* ParedLateral2 = _gPhysics->createRigidStatic(PxTransform({ 3.5, 1.0, 0.0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shape4 = CreateShape(PxBoxGeometry(0.3, 0.3, tableroJuego->height*0.94));
	ParedLateral2->attachShape(*shape4);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shape4, ParedLateral2, tableroJuego->color * 0.4);
	_gScene->addActor(*ParedLateral2);

	PxRigidStatic* Obstaculo1 = _gPhysics->createRigidStatic(PxTransform({ -1.5, 0.6, 0.0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapeObs1 = CreateShape(PxBoxGeometry(0.2, 0.5, 0.2));
	Obstaculo1->attachShape(*shapeObs1); Obstaculo1->setName("ObsExplosion");
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapeObs1, Obstaculo1, tableroJuego->color * 0.4);
	_gScene->addActor(*Obstaculo1);

	PxRigidStatic* Obstaculo2 = _gPhysics->createRigidStatic(PxTransform({ 1.5, 0.6, 0.0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapeObs2 = CreateShape(PxBoxGeometry(0.2, 0.5, 0.2));
	Obstaculo2->attachShape(*shapeObs2); Obstaculo2->setName("ObsExplosion");
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapeObs2, Obstaculo2, tableroJuego->color * 0.4);
	_gScene->addActor(*Obstaculo2);

	//-----------------Zona de Saque -----------------
	

	PxRigidStatic* SueloSaque = _gPhysics->createRigidStatic(PxTransform({ 4.2, 1, 0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapeSaque = CreateShape(PxBoxGeometry(0.4, 0.3, tableroJuego->height*0.8));
	SueloSaque->attachShape(*shapeSaque);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapeSaque, SueloSaque, tableroJuego->colorsaque);
	_gScene->addActor(*SueloSaque);

	PxRigidStatic* cubridorSaque = _gPhysics->createRigidStatic(PxTransform({ 4.2, 1.7, 0.5 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapecubriSaque = CreateShape(PxBoxGeometry(0.4, 0.05, tableroJuego->height));
	cubridorSaque->attachShape(*shapecubriSaque);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	//auto rICubridorSaque = new RenderItem(shapecubriSaque, cubridorSaque, {0.0, 0.0, 0.2, 0.0});
	_gScene->addActor(*cubridorSaque);

	PxRigidStatic* ParedSaque = _gPhysics->createRigidStatic(PxTransform({ 4.2, -1.0, 3.4 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapesaquebaja = CreateShape(PxBoxGeometry(0.6, 0.3, 0.3));
	ParedSaque->attachShape(*shapesaquebaja);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquebaja, ParedSaque, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaque);

	PxRigidStatic* ParedSaqueLD = _gPhysics->createRigidStatic(PxTransform({ 4.5, 0.62, 1.0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapesaquel = CreateShape(PxBoxGeometry(0.2, 0.3, tableroJuego->height * 0.68));
	ParedSaqueLD->attachShape(*shapesaquel);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquel, ParedSaqueLD, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueLD);

	PxRigidStatic* ParedSaqueLI = _gPhysics->createRigidStatic(PxTransform({ 3.5, 0.09, 1.8 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapesaquelI = CreateShape(PxBoxGeometry(0.3, 0.3, tableroJuego->height * 0.50));
	ParedSaqueLI->attachShape(*shapesaquelI);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquelI, ParedSaqueLI, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueLI);

	auto quat1 = PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} };
	auto quat2 = PxQuat{ 0.1, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedSaqueL1 = _gPhysics->createRigidStatic(PxTransform({ 4.46, 2.1, -1.0 }, quat1 * quat2));
	PxShape* shapesaquel1 = CreateShape(PxBoxGeometry(0.2, 0.3, 0.3));
	ParedSaqueL1->attachShape(*shapesaquel1);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquel1, ParedSaqueL1, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueL1);

	quat1 = PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} };
	quat2 = PxQuat{ 0.2, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedSaqueL2 = _gPhysics->createRigidStatic(PxTransform({ 4.42, 2.2, -1.2 }, quat1 * quat2));
	PxShape* shapesaquel2 = CreateShape(PxBoxGeometry(0.2, 0.3, 0.3));
	ParedSaqueL2->attachShape(*shapesaquel2);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquel2, ParedSaqueL2, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueL2);

	quat1 = PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} };
	quat2 = PxQuat{ 0.4, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedSaqueL3 = _gPhysics->createRigidStatic(PxTransform({ 4.34, 2.4, -1.5 }, quat1 * quat2));
	PxShape* shapesaquel3 = CreateShape(PxBoxGeometry(0.2, 0.3, 0.3));
	ParedSaqueL3->attachShape(*shapesaquel3);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquel3, ParedSaqueL3, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueL3);

	quat1 = PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} };
	quat2 = PxQuat{ 0.6, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedSaqueL4 = _gPhysics->createRigidStatic(PxTransform({ 4.18, 2.6, -1.8 }, quat1 * quat2));
	PxShape* shapesaquel4 = CreateShape(PxBoxGeometry(0.2, 0.3, 0.3));
	ParedSaqueL4->attachShape(*shapesaquel4);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquel4, ParedSaqueL4, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueL4);

	quat1 = PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} };
	quat2 = PxQuat{ 0.8, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedSaqueL5 = _gPhysics->createRigidStatic(PxTransform({ 3.9, 2.8, -2.1 }, quat1 * quat2));
	PxShape* shapesaquel5 = CreateShape(PxBoxGeometry(0.2, 0.3, 0.3));
	ParedSaqueL5->attachShape(*shapesaquel5);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquel5, ParedSaqueL5, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueL5);

	quat1 = PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} };
	quat2 = PxQuat{ 1.0, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedSaqueL6 = _gPhysics->createRigidStatic(PxTransform({ 3.6, 2.92, -2.3 }, quat1 * quat2));
	PxShape* shapesaquel6 = CreateShape(PxBoxGeometry(0.2, 0.3, 0.3));
	ParedSaqueL6->attachShape(*shapesaquel6);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquel6, ParedSaqueL6, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueL6);

	quat1 = PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} };
	quat2 = PxQuat{ 1.4, PxVec3 {0, 1, 0} };
	PxRigidStatic* ParedSaqueL7 = _gPhysics->createRigidStatic(PxTransform({ 3.5, 2.97, -2.35 }, quat1 * quat2));
	PxShape* shapesaquel7 = CreateShape(PxBoxGeometry(0.2, 0.3, 0.3));
	ParedSaqueL7->attachShape(*shapesaquel7);
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapesaquel7, ParedSaqueL7, { 0.0, 0.0, 0.2, 0.0 });
	_gScene->addActor(*ParedSaqueL7);

	//Add wall
	//PxRigidStatic* Pared = _gPhysics->createRigidStatic(PxTransform({ 10, 10, -10 }));
	//PxShape* shape_suelo = CreateShape(PxBoxGeometry(40, 20, 5));
	//Pared->attachShape(*shape_suelo);
	////itemRenderList.push_back(new RenderItem(shape_suelo, Pared, { 0.8, 0.8, 0.8,1 }));
	//new RenderItem(shape_suelo, Pared, { 0.8, 0.8, 0.8,1 });
	//_gScene->addActor(*Pared);

	//Geneeradores de fuerzas

	shared_ptr<ForceGenerator> gE(new ExplosionForceGenerator(3, 100, 1.0, Obstaculo1->getGlobalPose().p, false));
	gE->setName("ExplosionGenerator");
	addForceGenerator(gE);

	shared_ptr<TorqueForceGenerator> gT(new TorqueForceGenerator(2.0, 80.0, { 5.0, 0.0, 0.0 }, false));
	gT->setName("TorqueGenerator");
	addForceGenerator(gT);

	shared_ptr<UniformWindGenerator> gW(new UniformWindGenerator({ 0.0, 0.0, 0.0 }, { 1.5, 1, 2 }, 0.1, 0.2, {5, 0, 0}));
	gW->setName("WGenerator");
	addForceGenerator(gW);

	// ------- Player --------

	PxRigidDynamic* new_solid;
	Vector3 size = { 0.1, 0.1, 0.1 };

	//PxQuat q = { 45, PxVec3 {0, 1, 0} }; // 45 grados girado en Y
	//PxQuat q2 = { 10, PxVec3 {1, 0, 0} }; // 10 grados girado en X
	//new_solid = _gPhysics->createRigidDynamic(PxTransform({ 4.0, -0.6, 3.2 }, q * q2)); // añadir quaternion para generar con giro
	PxQuat qPrueba =  { 0, PxVec3 {0, 1, 0} };
	new_solid = _gPhysics->createRigidDynamic(PxTransform({ 4.0, 0.3, 0.0 }, qPrueba)); // añadir quaternion para generar con giro
	player_ = new Player(new_solid);
	new_solid->setName("Player");

	new_solid->setLinearVelocity({ 0.0, 8.0, 0.0 }); // velocidad inicial
	new_solid->setAngularVelocity({ 0.0, 0.0, 0.0 }); // velocidad de giro

	auto shapePlayer = CreateShape(PxSphereGeometry(0.1));
	//auto shape = CreateShape(PxSphereGeometry(2.0));
	new_solid->attachShape(*shapePlayer);
	new_solid->setMass(1);
	new_solid->setMassSpaceInertiaTensor({ size.y * size.y + size.z * size.z, size.x * size.x + size.z * size.z, size.y * size.y + size.x * size.x }); // tensor de inercia, marca cómo gira el objeto al chocar

//    new_solid->setMass(1.0);

	auto item = new RenderItem(shapePlayer, new_solid, { 0.2, 0.2, 0.2, 1.0 });
	//	_items.push_back(item);

	addToScene(new_solid);

	particleFR->addRegistry(gT.get(), new_solid);
	particleFR->addRegistry(gE.get(), new_solid);
	particleFR->addRegistry(gW.get(), new_solid);

	// -----Muelle Jugador------
	size = { 0.2, 0.2, 0.2 };
	PxRigidDynamic* muelleInit = _gPhysics->createRigidDynamic(PxTransform({ 4.2, -0.4, 3.0 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapeMuelle = CreateShape(PxBoxGeometry(size));
	muelleInit->setMass(6);
	muelleInit->attachShape(*shapeMuelle);
	muelleInit->setMassSpaceInertiaTensor({ size.y * size.y + size.z * size.z, size.x * size.x + size.z * size.z, size.y * size.y + size.x * size.x }); // tensor de inercia, marca cómo gira el objeto al chocar
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapeMuelle, muelleInit, { 0.0, 0.0, 0.6, 0.0 });
	_gScene->addActor(*muelleInit);

	size = { 0.2, 0.2, 0.2 };
	PxRigidDynamic* muelleInit2 = _gPhysics->createRigidDynamic(PxTransform({ 4.2, -0.2, 2.7 }, PxQuat{ tableroJuego->rot, PxVec3 {1, 0, 0} }));
	PxShape* shapeMuelle2 = CreateShape(PxBoxGeometry(size));
	muelleInit2->setMass(10);
	muelleInit2->attachShape(*shapeMuelle2);
	muelleInit2->setMassSpaceInertiaTensor({ size.y * size.y + size.z * size.z, size.x * size.x + size.z * size.z, size.y * size.y + size.x * size.x }); // tensor de inercia, marca cómo gira el objeto al chocar
	//itemRenderList.push_back(new RenderItem(shape, Suelo, { 0.8, 0.8, 0.8, 1 }));
	new RenderItem(shapeMuelle2, muelleInit2, { 0.0, 0.6, 0.0, 0.0 });
	_gScene->addActor(*muelleInit2);

	auto nItem = new RenderItem(shapePlayer, new_solid, { 0.2, 0.6, 0.2, 1.0 });

	RigidDynamic* model = new RigidDynamic();
	model->rigidDynamic = new_solid;  
	model->rItem = nItem;
	model->maxTime = 0;
	model->alive = true;
	model->deathTime = 4000;
	model->forcesName.push_back(gT->getName());
	model->forcesName.push_back(gE->getName());

	shared_ptr<GaussianRBGenerator> g(new GaussianRBGenerator(this, { 2, 0.1, 0.1 }, { 0.1, 0.1, 3.0 }, 2.0, { 0, 3.0, -2.8 }, { 1.0 , 0.1, 4.0 }, 1, false));
	addParticleGenerator(g);
	g->setMeanTime(7);
	g->setGeneratorName("GaussianGenerator");
	g->setParticle(model);

	shared_ptr<ForceGenerator> gAnch(new SpringForceGenerator(12.0, 0.5, muelleInit));
	gAnch->setName("AnchoredSpringFG");
	addForceGenerator(gAnch);

	particleFR->addRegistry(gAnch.get(), muelleInit2);
}

WorldManager::WorldManager(PxPhysics* gPhysics, PxScene* gScene) : _gPhysics(gPhysics), _gScene(gScene)
{
	puntos = 0;
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
	setUpTablero();
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

	auto a = player_->getPosition().y;
	bool b = a < -10.0f;
	if (a < -10.0f) {
		player_->resetPosition();
		puntos = 0;
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

	Vector3 size = { 0.4, 0.4, 0.4 };
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
