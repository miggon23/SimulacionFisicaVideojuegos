#include "GaussianParticleGenerator.h"
#include <random>

GaussianParticleGenerator::GaussianParticleGenerator(Vector3 dvPos, Vector3 dvVel, double dvT, Vector3 posM, Vector3 velM, int nPart) :
                                                                                                 ParticleGenerator("NORMAL_GENERATOR", posM, velM, nPart),
                                                                                                 std_dev_pos(dvPos), std_dev_vel(dvVel), std_dev_t(dvT)
{

}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
    std::random_device rd{};
    std::mt19937 gen{ rd() };

    //POSICION INICIAL
    std::normal_distribution<> dNormXpos{ _mean_pos.x, std_dev_pos.x };
    std::normal_distribution<> dNormYpos{ _mean_pos.y, std_dev_pos.y };
    std::normal_distribution<> dNormZpos{ _mean_pos.z, std_dev_pos.z };
    //VELOCIDAD INICIAL
    std::normal_distribution<> dNormXvel{ _mean_vel.x, std_dev_vel.x };
    std::normal_distribution<> dNormYvel{ _mean_vel.y, std_dev_vel.y };
    std::normal_distribution<> dNormZvel{ _mean_vel.z, std_dev_vel.z };
    //TIEMPO DE VIDA
    std::normal_distribution<> dNormTime{ meanTime, std_dev_t };

    Vector3 pos, vel;
    std::list<Particle*> list = std::list<Particle*>();
    for (int i = 0; i < _num_particles; i++)
    {
        //if ((rand() % 100) < 90) //Probabilidad del 90% de generarse
        //{
        //Generar partícula
        pos.x = dNormXpos(gen);	pos.y = dNormYpos(gen);	pos.z = dNormZpos(gen);
        vel.x = dNormXvel(gen);	vel.y = dNormYvel(gen);	vel.z = dNormZvel(gen);
        //list.push_back(new Particle(pos, vel, { 0.0, -10.0, 0.0 }, { 0.0, 0.4, 0.6, 1.0 }, 0.999, dNormTime(gen)));
        auto p = _model->clone();
        p->setPos(pos);
        p->setVel(vel);
        p->setRemainingTime(dNormTime(gen));
        list.push_back(p);
        //}
    }

    return list;
}

//------------------------------------------------------------

RainGenerator::RainGenerator(Vector3 dvPos, Vector3 dvVel, double dvT, Vector3 posM, Vector3 velM, int nPart, float radiusM, float dvRad): 
                                                                       GaussianParticleGenerator(dvPos, dvVel, dvT, posM, velM, nPart), std_dev_rad(dvRad), mean_rad(radiusM)
{
}

std::list<Particle*> RainGenerator::generateParticles()
{
    std::random_device rd{};
    std::mt19937 gen{ rd() };

    //POSICION INICIAL
    std::normal_distribution<> dNormXpos{ _mean_pos.x, std_dev_pos.x };
    std::normal_distribution<> dNormYpos{ _mean_pos.y, std_dev_pos.y };
    std::normal_distribution<> dNormZpos{ _mean_pos.z, std_dev_pos.z };
    //VELOCIDAD INICIAL
    std::normal_distribution<> dNormXvel{ _mean_vel.x, std_dev_vel.x };
    std::normal_distribution<> dNormYvel{ _mean_vel.y, std_dev_vel.y };
    std::normal_distribution<> dNormZvel{ _mean_vel.z, std_dev_vel.z };
    //TIEMPO DE VIDA
    std::normal_distribution<> dNormTime{ meanTime, std_dev_t };
    //Radio de la particula
    std::normal_distribution<> dNormRad{ mean_rad, std_dev_rad };
    Vector3 pos, vel;
    std::list<Particle*> list = std::list<Particle*>();
    for (int i = 0; i < _num_particles; i++)
    {
        //if ((rand() % 100) < 90) //Probabilidad del 90% de generarse
        //{
        //Generar partícula
        pos.x = dNormXpos(gen);	pos.y = dNormYpos(gen);	pos.z = dNormZpos(gen);
        vel.x = dNormXvel(gen);	vel.y = dNormYvel(gen);	vel.z = dNormZvel(gen);
        //list.push_back(new Particle(pos, vel, { 0.0, -10.0, 0.0 }, { 0.0, 0.4, 0.6, 1.0 }, 0.999, dNormTime(gen)));
        auto p = _model->cloneWithNewRadius(dNormRad(gen));
        p->setPos(pos);
        p->setVel(vel);
        p->setRemainingTime(dNormTime(gen));
        list.push_back(p);
        //}
    }

    return list;
}
