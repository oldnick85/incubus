#include "objectphysics.h"
#include <cstring>

CObjectPhysics::CObjectPhysics(const ILoggerPtr log, const IDispatcherPtr disp,
                               sGlobalPhysicsParam *ph_global, IPhysicsOtherObjectsPtr other_objects,
                               const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat)
    : CObject(spec)
    , CLogged(log)
    , CMessageSender(disp, msg_stat)
    , m_other_objects(other_objects)
    , m_ph_global(ph_global)
{
    SetSelfActor(PhysicsActor());
}

void CObjectPhysics::SetNewPosition(const sCoord2 &pos)
{
    auto old_pos = Position();
    Position() = pos;
    onPositionChange(old_pos, Position());
    if (PhysicsParts().manipulator.obj_uses != OBJ_ID_NONE)
    {
        CObjectPhysics* o_u;
        if (not m_other_objects->GetPhysicsObject(PhysicsParts().manipulator.obj_uses, &o_u))
        {
            PhysicsParts().manipulator.obj_uses = OBJ_ID_NONE;
        }
        else
        {
            o_u->SetNewPosition(pos);
        }
    }
}

void CObjectPhysics::SetHealthDamage(const Real_t damage_points)
{
    if (damage_points == 0.0)
        return;
    if (Health().health_points_max == 0.0)
        return;
    Health().health_points -= damage_points;
    if (Health().health_points <= 0.0)
    {
        Health().health_points = 0.0;
        PhysicsState().SetDestroyed();
        onPhysicsStateChange();
    }
    onHealthChange();
}

void CObjectPhysics::SetNewHealth(const double health_points)
{
    Health().health_points = health_points;
    onHealthChange();
}

void CObjectPhysics::onPhysicsStateChange()
{
    Send_SetPhysicsState(ShowActor(), GetID(), PhysicsState());
    Send_SetPhysicsState(BehaviorActor(), GetID(), PhysicsState());
}

void CObjectPhysics::onPhysicsParamChange()
{
    Send_SetPhysicsParam(ShowActor(), GetID(), Physics());
    Send_SetPhysicsParam(BehaviorActor(), GetID(), Physics());
}

void CObjectPhysics::onHealthChange()
{
    Send_SetHealth(BehaviorActor(), GetID(), Health());
    Send_SetHealth(ShowActor(), GetID(), Health());
}

void CObjectPhysics::onPositionChange(const sCoord2 &old_pos, const sCoord2 &new_pos)
{
    if (old_pos == new_pos)
        return;
    SendNewKinematic();
}

void CObjectPhysics::Destroy()
{
    PhysicsState().SetDestroyed();
    onPhysicsStateChange();
    Send_DelObject(GameplayActor(), GetID());
}

void CObjectPhysics::CheckGoal()
{
    switch (m_goal.goal)
    {
    case pgGoToPoint:
    {
        if ((m_goal.destination - Position()).Length() <= (Velocity().Length()*m_ph_global->time_delta))
        {
            m_goal.DoNothing();
            Velocity().Nullify();
            Send_GoalEvent(BehaviorActor(), GetID(), Goal());
        }
        break;
    }
    case pgNone:
        break;
    default:
        break;
    }
}

void CObjectPhysics::SendNewKinematic()
{
    if (Kinematic().SignificantlyDifferent(m_kinematic_last_sent))
    {
        Send_SetKinematicParam(ShowActor(), GetID(), Kinematic());
        Send_SetKinematicParam(BehaviorActor(), GetID(), Kinematic());
        m_kinematic_last_sent = Kinematic();
    }
}

void CObjectPhysics::DoActionTake(const ObjID_t target_obj_id)
{
    CObjectPhysics* tar_obj;
    if (not m_other_objects->GetPhysicsObject(target_obj_id, &tar_obj))
        return;

    tar_obj->PhysicsContained().ContainBy(GetID());
    PhysicsParts().manipulator.obj_uses = target_obj_id;
    tar_obj->onPhysicsParamChange();
    onPhysicsParamChange();
}

void CObjectPhysics::DoAction(const sObjectAction &object_action)
{
    switch (object_action.type)
    {
    case oaAttack:
        DoActionAttack(object_action.pos);
        break;
    case oaTake:
        DoActionTake(object_action.targ_obj_id);
        break;
    default:
        assert(false);
        break;
    }
}

void CObjectPhysics::DoActionAttack(const sCoord2 &target_pos)
{
    log_debug3("%s %s;", __PRETTY_FUNCTION__, target_pos.print().c_str());
}
