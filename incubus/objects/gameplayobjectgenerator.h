#ifndef GAMEPLAYOBJECTGENERATOR_H
#define GAMEPLAYOBJECTGENERATOR_H
#include "objectgameplay.h"

class CGameplayObjectGenerator
        : private CLogged
{
public:
    CGameplayObjectGenerator(const sAllCores &cores, const IDispatcherPtr disp, ILoggerPtr log, sDispatcherMsgStatistic* msg_stat);

    void MakeParam_Mob(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);
    void MakeParam_Construction(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);
    void MakeParam_Projectile(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);
    void MakeParam_Loot(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);


    void MakeParam_Bullet(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);
    void MakeParam_Pistol(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);

    void MakeHumanParam(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);
    void MakeWallParam(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);
    void MakeZombieParam(sObjGpParam &param, sShowParam &sh_param, sPhysicsParam &ph_param);

    CObjectGameplayPtr MakeObject(const sObjSpec &spec);
private:
    sAllCores                m_cores;
    IDispatcherPtr           m_disp;
    sDispatcherMsgStatistic* m_msg_stat = nullptr;
};

using CGameplayObjectGeneratorPtr = std::shared_ptr<CGameplayObjectGenerator>;

#endif // GAMEPLAYOBJECTGENERATOR_H
