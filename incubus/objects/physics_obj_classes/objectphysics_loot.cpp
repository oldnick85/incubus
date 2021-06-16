#include "objectphysics_loot.h"

CObjectPhysics_Loot::CObjectPhysics_Loot(const ILoggerPtr log, const IDispatcherPtr disp,
                                         sGlobalPhysicsParam* ph_global, IPhysicsOtherObjectsPtr other_objects,
                                         const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat)
    : CObjectPhysics (log, disp, ph_global, other_objects, spec, msg_stat)
{
    assert(spec.IsLoot());
}

void CObjectPhysics_Loot::DoActionAttack(const sCoord2 &target_pos)
{
    sCoord2 o_pos = Position();
    sVect2 v = target_pos - o_pos;
    v.Normalize(40.0);
    sVect2 v_d = v;

    sMaterialParam mt;
    double sz = 0.0;
    if (m_other_objects->GetPhysicsMaterialParam(PhysicsContained().contain_me, mt))
        sz = mt.size;

    v_d.Normalize(sz*1.0000001);
    o_pos += v_d;
    Send_CreateProjectile(GameplayActor(), o_pos, v);
}
