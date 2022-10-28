#pragma once
#include "core.hpp"
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include <ctype.h>
#include <list>
#include <memory>

class ParticleGenerator;

using namespace std;
class Particle
{
public:
	Particle(Vector3 pos, Vector3 vel, Vector3 ac, Vector4 col, float d, float rTime, float tam);
	Particle(Vector3 pos, Vector3 dir, float radius);
	~Particle();

	virtual void integrate(double t);

	inline void setVel(Vector3 v) { _vel = v; };
	inline Vector3 getVel() { return _vel; };
	inline void setDumping(float d) { damping = d; };
	inline void setAcc(Vector3 ac) { acceleration = ac; };
	inline void setPos(Vector3 pos) { pose.p = pos; };
	inline void setColor(Vector4 col) { color = col; renderItem->color = color; };
	inline void setAlive(bool b) { alive = b; };
	inline void setRemainingTime(float t) { remainingTime = t; };
	inline void setChangingColor(bool b, float timeChange) { changingColor = b; _factorColorChange = timeChange; };
	inline Vector3 getPos() { return pose.p; };

	inline bool isAlive() { return alive; };

	virtual Particle* clone() const;
	virtual Particle* cloneWithNewRadius(float rad) const;

	inline float getRemainingTime() { return remainingTime; };

	void clearForce();
	void addForce(const Vector3& f);

	inline float getMass() { return mass; };
	inline float getInverseMass() { return inverse_mass; };
	inline void setMass(float m = 0.01) { mass = m; inverse_mass = 1 / m; };

protected:
	Vector3 _vel, acceleration, force;
	RenderItem* renderItem = nullptr;
	Vector4 color = Vector4(0.5f, 0.5f, 0.5f, 1);
	int colorDir = 1;

	float damping = 1;
	bool changingColor = false;
	bool alive = true;
	double remainingTime;	
	float mass, inverse_mass;
	float _radius;
	//CUanto va a cambiar de color en cada frame
	float _factorColorChange;
	physx::PxTransform pose; //A render item le pasaremos la dirección de esta pose, para que se actualice automáticamente
private:
	void setUpParticle(float radius);
};

//--------------------------------------------------------------------------------------------

class Firework : public Particle {
	//unsigned type;
	int age; //edad del firework, si es 0 no se generan más
	list <shared_ptr <ParticleGenerator>> _gens;
public:
	Firework(Vector3 pos, Vector3 dir, list<shared_ptr<ParticleGenerator>> lG, float radius, int a);
	~Firework() = default;
	//virtual void integrate(double t) override;
	virtual Particle* clone() const;
	virtual Particle* cloneWithNewRadius(float rad) const;

	inline void addGenerator(shared_ptr<ParticleGenerator> pG) { _gens.push_back(pG); };

	std::list<Particle* >explode();
};

//struct Payload {
//	//Type of firework to create
//	unsigned type;
//
//	unsigned count;
//
//	void set(unsigned _type, unsigned _count)
//	{
//		type = _type;
//		count = _count;
//	}
//};
//
//struct FireworkRule {
//	//Type of firework configured
//	unsigned type;
//
//	float minAge;
//	float maxAge;
//
//	//Minimun relative velocity
//	Vector3 minVelocity;
//
//	float dumping;
//
//	std::vector<Payload> payloads;
//};

//--------------------------------------

enum ProyType {
	PAINT_BALL = 0,
	SNOW_BALL,
};
class Proyectil : public Particle {
public:
	Proyectil(Vector3 pos, Vector3 dir, ProyType tipo, float radius);

private:

	const float PAINT_VEL = 45;     //90 m/s en la realidad, 2.5 grm
	const float PAPER_VEL = 12.0;
};

