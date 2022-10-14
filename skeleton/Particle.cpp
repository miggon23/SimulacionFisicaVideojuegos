#include "Particle.h"
#include <random>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 ac,Vector4 col, float d = 1, float rTime = 5) : 
																				vel_(vel), pose(pos), acceleration(ac), 
																				dumping(d), remainingTime(rTime), color(col)
{
	setUpParticle(0.4);
}

Particle::Particle(Vector3 pos, Vector3 dir,float radius): pose(pos){
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

	vel_ = vel_ * pow(dumping, t) + acceleration * t;

	pose.p += vel_ * t + 0.5 * acceleration * t;

	remainingTime -= t;			//	SEGUNDOS	

	if (!changingColor)
		return;

	//Cambio de color
	float a = renderItem->color.x + (0.0004 * colorDir);
	if (a >= 1 || a <= 0)
		colorDir *= -1;
	renderItem->color.x = a;
	renderItem->color.y = a;
	/*if (a > )
	renderItem->color.z = a;*/
}

Particle* Particle::clone() const
{
	return new Particle(pose.p, vel_, acceleration, renderItem->color, dumping, remainingTime);
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

