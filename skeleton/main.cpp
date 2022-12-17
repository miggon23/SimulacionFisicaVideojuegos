#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Particle.h"
#include "ParticleSystem.h"
#include <vector>
#include "GaussianParticleGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "SinkForceGenerator.h"
#include "WorldManager.h"

#include <iostream>



using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

//std::vector<Particle*> vParticle;
ParticleSystem* pSys;
RenderItem* renderItemPlano;
RenderItem* renderItemBoxFirework;

WorldManager* wM = NULL;

float radiud;
float mass;

// Initialize physics engine
void initPhysics(bool interactive)
{
	srand(time(NULL));
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc); 


	pSys = new ParticleSystem();
	wM = new WorldManager(gPhysics, gScene);
	wM->initWorld();
	//auto s = CreateShape(physx::PxPlaneGeometry());
	//PxTransform pose;
	/*renderItemPlano = new RenderItem(CreateShape(PxBoxGeometry(300, 1, 300)), new PxTransform(-100, -2, -100), { 1,1,1,1 });
	renderItemPlano->color = {0.4, 0.2, 0.1, 1.0};*/

	/*renderItemBoxFirework = new RenderItem(CreateShape(PxBoxGeometry(2, 4, 2)), new PxTransform(0, 2, 0), { 1,1,1,1 });
	renderItemBoxFirework->color = {0.7, 0.7, 0.1, 1.0};*/
	mass = 10.0;
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	wM->update(t);
	/*for (auto p : vParticle) {
		p->integrate(t);
	}*/
	/*pSys->update(t);*/

	/*auto pG = pSys->getParticleGenerator("UNIFORM_GENERATOR");
	for (auto p : pG->generateParticles())
		pSys->addParticle(p);*/
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	delete wM;
	gFoundation->release();
	//DeregisterRenderItem(renderItemPlano);
	//DeregisterRenderItem(renderItemBoxFirework);
	delete pSys;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	
	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case 'B':
	{
		auto cam = GetCamera(); 
		pSys->addParticle(cam->getEye(), cam->getDir());
		break;
	}
	case 'O':
		wM->addDynamicRigidBody({ 3.0, 3.0, 3.0 }, { 0.0, 0.0, 0.0 }, { 8.0, 20.0, 0.0 }, { 0.6, 0.0, 0.7, 1.0 }, 5000);
	break;
	case 'M':
		wM->activateForceGenerator("ExplosionGenerator");
	break;
	case 'T':
		wM->activateForceGenerator("TorqueGenerator");
	break;
	case 'G':
	{
		auto pG = wM->getParticleGenerator("Gaussian");
		pG->setActive(!pG->getActive());
	}
	break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}