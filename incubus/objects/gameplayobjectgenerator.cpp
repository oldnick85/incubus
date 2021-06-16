#include "gameplayobjectgenerator.h"

CGameplayObjectGenerator::CGameplayObjectGenerator(const sAllCores &cores, const IDispatcherPtr disp, ILoggerPtr log, sDispatcherMsgStatistic *msg_stat)
    : CLogged (log)
    , m_cores(cores)
    , m_disp(disp)
    , m_msg_stat(msg_stat)
{

}

CObjectGameplayPtr CGameplayObjectGenerator::MakeObject(const sObjSpec &spec)
{
    auto obj = CObjectGameplayPtr(new CObjectGameplay(Log(), m_disp, spec, m_msg_stat));
    obj->SetCores(m_cores);

    sObjGpParam     gp_param;
    sPhysicsParam   ph_param;
    sShowParam      sh_param;

    switch (spec.Class())
    {
    case ocMob:             MakeParam_Mob(gp_param, sh_param, ph_param);            break;
    case ocConstruction:    MakeParam_Construction(gp_param, sh_param, ph_param);   break;
    case ocProjectile:      MakeParam_Projectile(gp_param, sh_param, ph_param);     break;
    case ocLoot:            MakeParam_Loot(gp_param, sh_param, ph_param);           break;
    default:                assert(false);                                          break;
    }

    switch (spec.Spec())
    {
    case opHuman:           MakeHumanParam(gp_param, sh_param, ph_param);           break;
    case opZombie:          MakeZombieParam(gp_param, sh_param, ph_param);          break;
    case opWall:            MakeWallParam(gp_param, sh_param, ph_param);            break;
    case opBullet:          MakeParam_Bullet(gp_param, sh_param, ph_param);         break;
    case opPistol:          MakeParam_Pistol(gp_param, sh_param, ph_param);         break;
    default:                assert(false);                                          break;
    }

    obj->m_param = gp_param;
    obj->m_ph_param = ph_param;
    obj->m_sh_param = sh_param;

    return obj;
}

void CGameplayObjectGenerator::MakeParam_Mob(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    if (not ph_param.kinematic.position.placed)
    {
        ph_param.kinematic.position.x = (static_cast<double>(rand() % 1000) - 500.0)/1000.0;
        ph_param.kinematic.position.y = (static_cast<double>(rand() % 1000) - 500.0)/1000.0;
    }
    ph_param.kinematic.direction.vx = (static_cast<double>(rand() % 1000) - 500.0)/1000.0;
    ph_param.kinematic.direction.vy = (static_cast<double>(rand() % 1000) - 500.0)/1000.0;
    ph_param.kinematic.direction.Normalize();

    ph_param.parts.body.type = ppHumanTorso;
    ph_param.parts.chassis.type = ppHumanLegs;
    ph_param.parts.ganglion.type = ppHumanHead;
    ph_param.parts.manipulator.type = ppHumanArms;

    sh_param.sh_height = sShowParam::enShowHeight::shStay;
}

void CGameplayObjectGenerator::MakeParam_Construction(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    ph_param.perseption.sight_distance = 0;
    ph_param.material.mass = 1.0E+100;
    ph_param.health.health_points_max = 0;

    sh_param.sh_height = sShowParam::enShowHeight::shStay;
}

void CGameplayObjectGenerator::MakeParam_Projectile(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    ph_param.perseption.sight_distance = 0.0;
    sh_param.sh_height = sShowParam::enShowHeight::shFly;
    sh_param.trace_ttl = 0.4;
}

void CGameplayObjectGenerator::MakeParam_Loot(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    ph_param.health.health_points_max = 0.0;
    ph_param.contained.can_be_contained = true;
    sh_param.sh_height = sShowParam::enShowHeight::shOnGround;
}

void CGameplayObjectGenerator::MakeZombieParam(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    ph_param.material.size = 0.3;
    ph_param.shape.Clear();
    ph_param.shape.AddCircle({0,0}, ph_param.material.size);
}

void CGameplayObjectGenerator::MakeHumanParam(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    ph_param.material.size = 0.3;
    ph_param.shape.Clear();
    ph_param.shape.AddCircle({0,0}, ph_param.material.size);
}

void CGameplayObjectGenerator::MakeWallParam(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    ph_param.shape.Clear();
    ph_param.shape.AddLine({0.0, 0.0}, {5.0, 0.0});
}

void CGameplayObjectGenerator::MakeParam_Bullet(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    ph_param.shape.Clear();
    ph_param.shape.AddPoint({0,0});
    ph_param.health.health_points = 1.0;
    ph_param.health.health_points_max = 1.0;
    ph_param.health.max_distance_move = 20.0;
    ph_param.material.mass = 0.051;
}

void CGameplayObjectGenerator::MakeParam_Pistol(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param)
{
    log_debug1("%s %s; %s; %s;", __PRETTY_FUNCTION__, param.print().c_str(), sh_param.print().c_str(), ph_param.print().c_str());
    ph_param.shape.Clear();
    ph_param.shape.AddLine({0.0, 0.0}, {0.15, 0.0});
    ph_param.material.mass = 0.75;
}
