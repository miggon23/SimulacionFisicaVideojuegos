#include "Particle.h"
#include "ParticleGenerator.h"
#include <random>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 ac,Vector4 col, float d = 1, float rTime = 5) : 
																				_vel(vel), pose(pos), acceleration(ac), 
																				dumping(d), remainingTime(rTime), color(col), changingColor(false)
{
	setUpParticle(0.4);
}

Particle::Particle(Vector3 pos, Vector3 dir,float radius): pose(pos), _vel(dir), _radius(radius), changingColor(false){
	setUpParticle(radius);
}


Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

void Particle::integrate(double t)
{
	if (!isAlive())
		return;

	_vel = _vel * pow(dumping, t) + acceleration * t;
	pose.p += _vel * t + 0.5 * acceleration * t;

	remainingTime -= t;			//	SEGUNDOS	

	if (!changingColor)
		return;

	//Cambio de color
	float a = renderItem->color.x + (0.0005 * colorDir);
	if (a >= 0.8 || a <= 0.02)
		colorDir *= -1;
	renderItem->color.x = a;
	renderItem->color.y = a;
	/*if (a > )
	renderItem->color.z = a;*/
}

Particle* Particle::clone() const
{
	return new Particle(pose.p, _vel, acceleration, renderItem->color, dumping, remainingTime);
}

void Particle::setUpParticle(float radius)
{
	auto s = CreateShape(physx::PxSphereGeometry(radius));
	renderItem = new RenderItem(s, &pose, color);
}

//------------------------------------------

Proyectil::Proyectil(Vector3 pos, Vector3 dir, ProyType tipo, float r) : Particle(pos, dir, r)
{
	Vector3 camDir = GetCamera()->getDir();

	switch (tipo)
	{
	case PAINT_BALL:
		setVel(camDir * PAINT_VEL);
		setAcc({ 0, -1.8, 0 });
		setDumping(0.98);
		changingColor = true;		

		break;
	case SNOW_BALL:
		setVel(camDir * PAPER_VEL);
		setAcc({ 0, -1.8, 0 });
		setDumping(0.9);
		setColor({ 0.9, 0.9, 0.9, 1.0 });
		changingColor = false;

		break;
	default:
		setVel(camDir * PAINT_VEL);
		setAcc({ 0, -1.8, 0 });
		setDumping(0.9);
		break;
	}
	
}

Firework::Firework(Vector3 pos, Vector3 dir, float radius, int a) : Particle(pos, dir, radius), age(a)
{
	
}

void Firework::integrate(double t)
{
	if (!isAlive())
		return;

	_vel = _vel * pow(dumping, t) + acceleration * t;
	pose.p += _vel * t + 0.5 * acceleration * t;

	remainingTime -= t;
}



Particle* Firework::clone() const
{
	Firework* f = new Firework(pose.p, _vel, _radius, age - 1);
	f->setAcc(acceleration);
	f->setColor(renderItem->color);
	return f;
}

std::list<Particle*> Firework::explode()
{
	
	//Si no quedan más ciclos de explosión, devolvemos lista vacía
	if (age <= 0)
		return std::list<Particle*>();
	//Generar partículas
	std::list<Particle*> l = std::list<Particle*>();
	Vector3 newDir{ 0.0, 0.0, 0.0 };
	for (int i = 0; i < 5; i++) {
		newDir.x = rand() % 14;
		newDir.y = rand() % 14;
		newDir.z = rand() % 14;
		auto f = clone();
		f->setVel(f->getVel() + newDir);
		l.push_back(f);
		//RESET LIFE TIMER!!!
		f->setRemainingTime(2);
		
	}
	/*for (auto g : _gens)
	{
		(*g)->generateParticles();
	}*/

	return l;
}
