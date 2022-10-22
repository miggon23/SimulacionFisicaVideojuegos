#include "Particle.h"
#include "ParticleGenerator.h"
#include <random>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 ac,Vector4 col, float d = 0.999, float rTime = 5, float tam = 1.0) : 
																				_vel(vel), pose(pos), acceleration(ac), _radius(tam),
																				dumping(d), remainingTime(rTime), color(col), changingColor(false)
{
	setUpParticle(tam);
}

Particle::Particle(Vector3 pos, Vector3 dir, float radius): pose(pos), _vel(dir), _radius(radius), changingColor(false){
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
	float a = renderItem->color.x + (_factorColorChange * colorDir * t);
	if (a >= 0.8 || a <= 0.02)
		colorDir *= -1;
	renderItem->color.x = a;
	renderItem->color.y = a;
	/*if (a > )
	renderItem->color.z = a;*/
}

Particle* Particle::clone() const
{
	return new Particle(pose.p, _vel, acceleration, renderItem->color, dumping, remainingTime, _radius);
}

void Particle::setUpParticle(float radius)
{
	auto s = CreateShape(physx::PxSphereGeometry(radius));
	renderItem = new RenderItem(s, &pose, color);
	_factorColorChange = 0.0005;
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

Firework::Firework(Vector3 pos, Vector3 dir, list<shared_ptr<ParticleGenerator>> lG, float radius, int a) : Particle(pos, dir, radius), age(a), _gens(lG)
{
	
}

//void Firework::integrate(double t)
//{
//	if (!isAlive())
//		return;
//
//	_vel = _vel * pow(dumping, t) + acceleration * t;
//	pose.p += _vel * t + 0.5 * acceleration * t;
//
//	remainingTime -= t;
//}



Particle* Firework::clone() const
{
	Firework* f = new Firework(pose.p, _vel, _gens,  _radius, age - 1);
	f->setAcc(acceleration);
	f->setColor(renderItem->color);
	f->setRemainingTime(remainingTime);
	f->setChangingColor(changingColor, _factorColorChange);
	for (auto g : _gens)
		f->addGenerator(g);
	return f;
}

std::list<Particle*> Firework::explode()
{
	
	//Si no quedan más ciclos de explosión, devolvemos lista vacía
	/*if (age <= 0)
		return std::list<Particle*>();*/
	//Generar partículas
	std::list<Particle*> l = std::list<Particle*>();
	//Vector3 newDir{ 0.0, 0.0, 0.0 };
	//for (int i = 0; i < 5; i++) {
	//	newDir.x = rand() % 14;
	//	newDir.y = rand() % 14;
	//	newDir.z = rand() % 14;
	//	auto f = clone();
	//	f->setVel(f->getVel() + newDir);
	//	l.push_back(f);
	//	//RESET LIFE TIMER!!!
	//	f->setRemainingTime(2);
	//	
	//}
	
	for (auto g : _gens)
	{
		g->setMeanPos(pose.p);
		//Hay que añadirle la velocidad que llevaba la partícula que se destruyó
		g->setMeanVel(_vel);
		for (auto c : g.get()->generateParticles()) {
			l.push_back(c);
		}
	}

	return l;
}
