#include "objectphysics_mob.h"

CObjectPhysics_Mob::CObjectPhysics_Mob(const ILoggerPtr log, const IDispatcherPtr disp,
                                       sGlobalPhysicsParam* ph_global, IPhysicsOtherObjectsPtr other_objects,
                                       const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat)
    : CObjectPhysics (log, disp, ph_global, other_objects, spec, msg_stat)
{
    assert(spec.IsMob());
}

void CObjectPhysics_Mob::DoActionAttack(const sCoord2 &target_pos)
{
    if (PhysicsParts().manipulator.type != ppNone)
    {
        auto o_id = PhysicsParts().manipulator.obj_uses;
        if (o_id != OBJ_ID_NONE)
        {
            sObjectAction obj_action;
            obj_action.SetAttack(o_id, target_pos);
            Send_ObjectAction(BehaviorActor(), obj_action);
        }
    }
}
