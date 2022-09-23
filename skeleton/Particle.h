#pragma once
#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <ctype.h>

class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 ac, float d);
	~Particle();

	void integrate(double t);

private:
	Vector3 vel_;
	physx::PxTransform pose; //A render item le pasaremos la dirección de esta pose, para que se actualice automáticamente
	RenderItem* renderItem;
	Vector4 color = Vector4(0.5f, 0.5f, 0.5f, 1);
	int colorDir = 1;

	Vector3 acceleration;
	float dumping;
};

//--------------------------------------------------------------------------------------------

class Firework : public Particle {
	unsigned type;
	float age;

	Firework();
};

struct Payload {
	//Type of firework to create
	unsigned type;

	unsigned count;

	void set(unsigned _type, unsigned _count)
	{
		type = _type;
		count = _count;
	}
};

struct FireworkRule {
	//Type of firework configured
	unsigned type;

	float minAge;
	float maxAge;

	//Minimun relative velocity
	Vector3 minVelocity;

	float damping;

	std::vector<Payload> payloads;
};

