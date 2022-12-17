#include "UniformRBGenerator.h"
#include "WorldManager.h"
#include <random>

UniformRBGenerator::UniformRBGenerator(WorldManager* _gM, Vector3 velW, Vector3 posW, Vector3 posM, Vector3 velM, int nPart, bool active)
                                        : RigidBodyGenerator(_gM, "UNIFORM_GENERATOR", posM, velM, nPart, active), 
                                          _pos_width(posW), _vel_width(velW)
{
}

std::list<RigidDynamic*> UniformRBGenerator::generateParticles()
{
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

	//POSICION
	std::uniform_real_distribution<> std_dis_posX(_mean_pos.x - _pos_width.x / 2, _mean_pos.x + _pos_width.x / 2);
	std::uniform_real_distribution<> std_dis_posY(_mean_pos.y - _pos_width.y / 2, _mean_pos.y + _pos_width.y / 2);
	std::uniform_real_distribution<> std_dis_posZ(_mean_pos.z - _pos_width.z / 2, _mean_pos.z + _pos_width.z / 2);
	//VELOCIDAD
	std::uniform_real_distribution<> std_dis_velX(_mean_vel.x - _vel_width.x / 2, _mean_vel.x + _vel_width.x / 2);
	std::uniform_real_distribution<> std_dis_velY(_mean_vel.y - _vel_width.y / 2, _mean_vel.y + _vel_width.y / 2);
	std::uniform_real_distribution<> std_dis_velZ(_mean_vel.z - _vel_width.z / 2, _mean_vel.z + _vel_width.z / 2);
	//TIEMPO DE VIDA
	std::uniform_real_distribution<> std_dis_time(8, 20);

	Vector3 pos, vel;
	std::list<RigidDynamic*> list = std::list<RigidDynamic*>();
	for (int i = 0; i < _num_particles; i++)
	{
		if ((rand() % 100) < 90) //Probabilidad del 90% de generarse
		{
			//Generar partícula
			pos.x = std_dis_posX(gen);	pos.y = std_dis_posY(gen);	pos.z = std_dis_posZ(gen);
			vel.x = std_dis_velX(gen);	vel.y = std_dis_velY(gen);	vel.z = std_dis_velZ(gen);
			//list.push_back(new Particle(pos, vel, { 0.0, -10.0, 0.0 }, { 0.0, 0.0, 0.6, 1.0 }, 0.999, std_dis_time(gen)));
			auto p = _wM->clone(_model);
			physx::PxTransform t;
			t.p = pos;
			p->rigidDynamic->setGlobalPose(t);
			p->rigidDynamic->setLinearVelocity(vel);
			//p->maxTime = std_dis_time(gen);
			//p->setChangingColor(true);
			list.push_back(p);

		}
	}
	return list;
}
