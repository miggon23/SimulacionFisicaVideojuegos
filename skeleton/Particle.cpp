#include "Particle.h"
#include <random>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 ac, float d = 1) : vel_(vel), pose(pos), acceleration(ac), dumping(d)
{
	setUpParticle(1);
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
	if (remainingTime <= 0)
		setAlive(false);

	if (!changingColor)
		return;

	//Cambio de color
	float a = renderItem->color.x + (0.0002 * colorDir);
	if (a >= 1 || a <= 0)
		colorDir *= -1;
	renderItem->color.x = a;
	renderItem->color.z = a;
}

Particle* Particle::clone() const
{
	return nullptr;
}

void Particle::setUpParticle(int radius)
{
	auto s = CreateShape(physx::PxSphereGeometry(radius));
	renderItem = new RenderItem(s, &pose, color);

	std::random_device rd{};
	std::mt19937 gen{ rd() };

	std::normal_distribution<> dNorm{ 6,2.5 };
	remainingTime = dNorm(gen); //El tiempo de vida sigue una distribución normal
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

