#pragma once
#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <ctype.h>

class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 ac, Vector4 col, float d, float rTime);
	Particle(Vector3 pos, Vector3 dir, float radius);
	~Particle();

	void integrate(double t);

	inline void setVel(Vector3 v) { vel_ = v; };
	inline void setDumping(float d) { dumping = d; };
	inline void setAcc(Vector3 ac) { acceleration = ac; };
	inline void setPos(Vector3 pos) { pose.p = pos; };
	inline void setColor(Vector4 col) { color = col; renderItem->color = color; };
	inline void setAlive(bool b) { alive = b; };
	inline void setRemainingTime(float t) { remainingTime = t; };
	inline void setChangingColor(bool b) { changingColor = b; };
	inline Vector3 getPos() { return pose.p; };

	inline bool isAlive() { return alive; };

	virtual Particle* clone() const;

	inline float getRemainingTime() { return remainingTime; };

protected:
	Vector3 vel_, acceleration;
	RenderItem* renderItem = nullptr;
	Vector4 color = Vector4(0.5f, 0.5f, 0.5f, 1);
	int colorDir = 1;

	float dumping = 1;
	bool changingColor = false;
	bool alive = true;
	double remainingTime;	

	physx::PxTransform pose; //A render item le pasaremos la dirección de esta pose, para que se actualice automáticamente

private:
	void setUpParticle(float radius);
};

//--------------------------------------------------------------------------------------------

class Firework : public Particle {
	unsigned type;
	float age;

	Firework();
public:
	void explode();
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

	float dumping;

	std::vector<Payload> payloads;
};

//--------------------------------------

enum ProyType {
	PAINT_BALL = 0,
	SNOW_BALL,
};
class Proyectil : public Particle {
public:
	Proyectil(Vector3 pos, Vector3 dir, ProyType tipo, float radius);

private:
	float masa;
	const float PAINT_VEL = 45;     //90 m/s en la realidad, 2.5 grm
	const float PAPER_VEL = 12.0;
};

