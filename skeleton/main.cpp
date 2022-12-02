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

	auto s = CreateShape(physx::PxPlaneGeometry());
	PxTransform pose;
	renderItemPlano = new RenderItem(CreateShape(PxBoxGeometry(300, 1, 300)), new PxTransform(-100, -2, -100), { 1,1,1,1 });
	auto renderItemPlano2 = new RenderItem(CreateShape(PxBoxGeometry(30, 0.2, 30)), new PxTransform( 10, 10, 10), { 1,1,1,1 });
	renderItemPlano->color = {0.4, 0.2, 0.1, 1.0};
	renderItemPlano2->color = { 0.15, 0.15, 0.7, 1.0 };

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
	/*for (auto p : vParticle) {
		p->integrate(t);
	}*/
	pSys->update(t);
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
	
	gFoundation->release();
	DeregisterRenderItem(renderItemPlano);
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
	case '0':
		pSys->shootFirework(0);
		break;
	case '1':
		//pSys->changeParticleType(PAINT_BALL);
		pSys->shootFirework(1);
		break;
	case '2':
		//pSys->changeParticleType(SNOW_BALL);
		pSys->shootFirework(2);
		break;
	case '3':
		pSys->shootFirework(3);
		break;
	case '4':
		pSys->shootFirework(4);
		break;
	case 'Z':
		glClearColor(0.1f, 0.1f, 0.1f, 1.0); //BackgroundColor
		break;
	case 'X':
		glClearColor(0.4f, 0.5f, 0.6f, 1.0); //BackgroundColor
		break;
	/*case 'U':
	{
		auto pG = pSys->getParticleGenerator("UNIFORM_GENERATOR");
		for (auto p : pG->generateParticles()) {
			p->setChangingColor(true, 0.2);
			pSys->addParticle(p);
		}
		break;
	}*/
	case 'G':
		/*pG = pSys->getParticleGenerator("NORMAL_GENERATOR");
		for (auto p : pG->generateParticles())
			pSys->addParticle(p);*/
		pSys->activateGenerator("NORMAL_GENERATOR");
		break;
	case 'F':
		pSys->setGeneratorToFollowCamera(!pSys->getGeneratorFollowCamera());
		break;
	case 'O':
	{		
		/*auto f = static_cast<GaussianParticleGenerator* >(pSys->getParticleGenerator("NORMAL_GENERATOR").get());
		f->setDevVel(f->getDevVel() + Vector3{0.15, 0.0, 0.15});*/
		/*auto f = pSys->getParticleGenerator("RainGenerator");
		f->setMeanVel(f->getMeanVel() - Vector3{ 0.0, 1.5, 0.0 });
		f->setNParticles(f->getNParticles() + 1);*/
		//------------------------------------------------------
		/*auto explFG = dynamic_cast<WhirlwindGenerator*>(pSys->getForceGenerator("WhirlwindGenerator").get());
		explFG->setK(explFG->getK() + 0.1);*/

		auto anchFG = dynamic_cast<SpringForceGenerator*>(pSys->getForceGenerator("SpringUno").get());
		anchFG->setK(anchFG->getK() + 2.0);
		cout << "Nueva k para el muelle: " << anchFG->getK() << "\n";

		break;
	}
	case 'P':
	{
		/*auto pGG = static_cast<GaussianParticleGenerator* >(pSys->getParticleGenerator("NORMAL_GENERATOR").get());
		pGG->setDevVel(pGG->getDevVel() - Vector3{ 0.15, 0.0, 0.15 });*/
		/*auto f = pSys->getParticleGenerator("RainGenerator");
		f->setMeanVel(f->getMeanVel() + Vector3{ 0.0, 1.5, 0.0 });
		f->setNParticles(f->getNParticles() - 1);*/
		//-----------------------------------------------
		/*auto explFG = dynamic_cast<WhirlwindGenerator*>(pSys->getForceGenerator("WhirlwindGenerator").get());
		explFG->setK(explFG->getK() - 0.1);*/

		auto anchFG = dynamic_cast<SpringForceGenerator*>(pSys->getForceGenerator("SpringUno").get());
		anchFG->setK(anchFG->getK() - 2.0);
		cout << "Nueva k para el muelle: " << anchFG->getK() << "\n";
		break;
	}
	case 'R':
	{
		pSys->activateGenerator("RainGenerator");
		break;
	}
	case 'T':
	{
		auto dragFG = pSys->getForceGenerator("WhirlwindGenerator");
		for (int j = 0; j < 4; j++){
			for (int i = 0; i < 7; i++) {
				float x = -15.0 + 5.0 * i;
				float y = 0.0 + j * 10.0;
				auto f = new Particle({ x, y, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.2, 0.2, 0.8, 1.0 }, 0.999, 20, 1.0);
				f->setMass(30.0);
				pSys->getParticleForceRegistry()->addRegistry(dragFG.get(), f);
				pSys->addParticle(f);
			}
		}
	}
	break;
	case 'E':
	{
		auto explFG = pSys->getForceGenerator("ExplosionGenerator");
		for (int k = 0; k < 7; k++){
			for (int j = 0; j < 4; j++) {
				for (int i = 0; i < 7; i++) {
					float x = -15.0 + 5.0 * i;
					float y = 0.0 + j * 5.0;
					float z = -20 + k * 5.0;
					auto f = new Particle({ x, y, z }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.8, 0.2, 0.2, 1.0 }, 0.999, 12, 1.0);
					f->setChangingColor(true, 0.002);
					f->setMass(15.0);
					pSys->getParticleForceRegistry()->addRegistry(explFG.get(), f);
					pSys->addParticle(f);
				}
			}
		}
	}
	break;
	case 'Y':
	{
		auto dragFG = pSys->getForceGenerator("WindGenerator");
		for (int j = 0; j < 5; j++) {		
			float x = -30.0 + 15.0 * j;
	
			auto f = new Particle({ x, 30.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.4, 0.4, 0.7, 1.0 }, 0.999, 5, 0.8);
			f->setMass(10.0);
			pSys->getParticleForceRegistry()->addRegistry(dragFG.get(), f);
			pSys->addParticle(f);		
		}
	}
	break;
	case 'M':
		pSys->activateForceGenerator("GravityGenerator");
	break;
	case 'N':
		pSys->activateForceGenerator("ExplosionGenerator");
		break;
	case 'V':
		pSys->activateForceGenerator("WhirlwindGenerator");
		break;
	case 'U':
	{
		/*auto explFG = dynamic_cast<WhirlwindGenerator*>(pSys->getForceGenerator("WhirlwindGenerator").get());
		explFG->setCenter(explFG->getCenter() + Vector3(0.0, 0.5, 0.0));*/
		mass += 2;
		cout << "Masa para las particulas flotantes: " << mass << '\n';
	}
	break;
	case 'J':
	{
		/*auto explFG = dynamic_cast<WhirlwindGenerator*>(pSys->getForceGenerator("WhirlwindGenerator").get());
		explFG->setCenter(explFG->getCenter() - Vector3(0.0, 0.5, 0.0));*/
		if (mass > 2)
			mass -= 2;
		cout << "Masa para las particulas flotantes: " << mass << '\n';
	}
	break;
	case 'K':
	{
		/*auto explFG = dynamic_cast<ExplosionForceGenerator*>(pSys->getForceGenerator("ExplosionGenerator").get());
		explFG->setK(explFG->getK() + 500);*/
		auto anchFG = dynamic_cast<SinkForceGenerator*>(pSys->getForceGenerator("SinkFG").get());
		anchFG->setLiquidDensity(anchFG->getLiquidDensity() + 10.0);
		cout << "Nueva densidad para el liquido: " << anchFG->getLiquidDensity() << "\n";
	}
	break;
	case 'L':
	{
		/*auto explFG = dynamic_cast<ExplosionForceGenerator*>(pSys->getForceGenerator("ExplosionGenerator").get());
		explFG->setK(explFG->getK() - 500);*/
		auto anchFG = dynamic_cast<SinkForceGenerator*>(pSys->getForceGenerator("SinkFG").get());
		anchFG->setLiquidDensity(anchFG->getLiquidDensity() - 10.0);
		cout << "Nueva densidad para el liquido: " << anchFG->getLiquidDensity() << "\n";
	}
	break;
	case '5':
	{
		auto sinkFG = dynamic_cast<SinkForceGenerator*>(pSys->getForceGenerator("SinkFG").get());
		auto gFG = pSys->getForceGenerator("GravityGenerator").get();
		float x = rand() % 30;
		float y = 10 + rand() % 10;
		float z = rand() % 30;
		auto p = new Particle({ x, y, z }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.8, 0.2, 0.4, 1.0 }, 0.9, 30, 1);
		p->setMass(mass);
		pSys->getParticleForceRegistry()->addRegistry(sinkFG, p);
		pSys->getParticleForceRegistry()->addRegistry(gFG, p);

		pSys->addParticle(p);
	}
	break;
	case '6':
	{
		pSys->testSlinky();
	}
	break;
	case '7':
	{
		pSys->testRubber();
	}
	break;
	case '8':
	{
		pSys->testSprings();
	}
	break;
	case '9':
	{
		/*auto explFG = dynamic_cast<ExplosionForceGenerator*>(pSys->getForceGenerator("ExplosionGenerator").get());
		explFG->setR(explFG->getR() - 1);*/
		auto anchFG = dynamic_cast<AnchoredSpringFG*>(pSys->getForceGenerator("AnchoredSpringFG").get());

		auto f = new Particle({ 0, 49, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.8, 0.2, 0.2, 1.0 }, 0.999, 120, 0.5);
		f->setMass(1.0);
		pSys->getParticleForceRegistry()->addRegistry(anchFG, f);
		f->setSemiImplicit(true);
		pSys->addParticle(f);
	}
	break;
	case '+':
	{
		auto anchFG = dynamic_cast<AnchoredSpringFG*>(pSys->getForceGenerator("AnchoredSpringFG").get());
		anchFG->setK(anchFG->getK() + 2.0);
		cout << "Nueva k para el muelle: " << anchFG->getK() << "\n";
	}
	break;
	case '-':
	{
		auto anchFG = dynamic_cast<AnchoredSpringFG*>(pSys->getForceGenerator("AnchoredSpringFG").get());
		anchFG->setK(anchFG->getK() - 2.0);
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