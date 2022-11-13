#include "Particle.h"
#include "ParticleGenerator.h"
#include <random>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 ac,Vector4 col, float d = 0.999, float rTime = 5, float tam = 1.0) : 
																				_vel(vel), pose(pos), acceleration(ac), _radius(tam),
																				damping(d), remainingTime(rTime), color(col), changingColor(false),
																				force({0.0, 0.0, 0.0})
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
	remainingTime -= t;	//	SEGUNDOS	
	if (!isAlive())
		return;

	if (inverse_mass > 0.0f) {
		_vel = _vel * pow(damping, t) + acceleration * t;
		//pose.p += _vel * t + 0.5 * acceleration * t;
		pose.p += _vel * t;
		Vector3 totalAcceleration = acceleration;
		totalAcceleration += force * inverse_mass;

		//Update linear velocity
		_vel += totalAcceleration * t;

		//Impose drag(damping)
		_vel *= powf(damping, t);

		clearForce(); //Limpiamos a fuerza una vez integrada
	}

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
	return new Particle(pose.p, _vel, acceleration, renderItem->color, damping, remainingTime, _radius);
}

//El radio es uno de los elementos que no se pueden cambiar a psteriori en una partícula
Particle* Particle::cloneWithNewRadius(float rad) const
{
	//evitamos valores por debajo de 0
	if (rad <= 0.001)
		rad = 0.05;
	return new Particle(pose.p, _vel, acceleration, renderItem->color, damping, remainingTime, rad);
}

void Particle::clearForce()
{
	force = { 0.0, 0.0, 0.0 };
}

void Particle::addForce(const Vector3& f)
{
	force += f;
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

Particle* Firework::clone() const
{
	Firework* f = new Firework(pose.p, _vel, _gens,  _radius, age - 1);
	f->setAcc(acceleration);
	f->setColor(renderItem->color);
	f->setRemainingTime(remainingTime);
	f->setChangingColor(changingColor, _factorColorChange);
	//for (auto g : _gens)
	//	f->addGenerator(g);
	return f;
}

Particle* Firework::cloneWithNewRadius(float rad) const
{
	//evitamos valores por debajo de 0
	if (rad <= 0.001)
		rad = 0.05;
	Firework* f = new Firework(pose.p, _vel, _gens, rad, age - 1);
	f->setAcc(acceleration);
	f->setColor(renderItem->color);
	f->setRemainingTime(remainingTime);
	f->setChangingColor(changingColor, _factorColorChange);
	return f;
}

std::list<Particle*> Firework::explode()
{
	//Generar partículas
	std::list<Particle*> l = std::list<Particle*>();
	
	for (auto g : _gens)
	{
		//Si choca contra el suelo
		if (pose.p.y <= 0) {
			Vector3 nMPos = Vector3(pose.p.x, 0.5, pose.p.z);
			g->setMeanPos(nMPos);
			//Hacemos que rebote con una velocidad comtraria en el eje y
			Vector3 nMVel = Vector3(_vel.x, -_vel.y * 0.35, _vel.z);
			g->setMeanVel(nMVel);
		}
		else {
			g->setMeanPos(pose.p);
			//Hay que añadirle la velocidad que llevaba la partícula que se destruyó
			g->setMeanVel(_vel);
		}
		for (auto c : g.get()->generateParticles()) {
			l.push_back(c);
		}
	}

	return l;
}
