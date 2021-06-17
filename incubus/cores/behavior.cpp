#include "behavior.h"

#include "gameplay.h"

CBehavior::CBehavior(const ILoggerPtr log, const IDispatcherPtr disp)
    : CMessageCallbacks(log, "BEHAVIOR")
    , CLogged(log)
    , CMessageSender(disp, &GetMessagesStatistic())
{
    log_debug1("CBehavior::CBehavior");
    m_id.type = enActor::actBehavior;
    Dispatcher()->RegisterActor(this);
}

CBehavior::~CBehavior()
{
    log_debug1("CBehavior::~CBehavior");
    Dispatcher()->KillTimer(GetActorID().id, m_tm_100ms);
    Dispatcher()->UnregisterActor(this);
}

bool CBehavior::onNewObjectBehavior(const ObjID_t obj_id, const sObjSpec &spec, const sObjBhParam &bh_param, const sPhysicsParamMsg &ph_param)
{
    auto obj = CObjectBehaviorPtr(new CObjectBehavior(m_log, Dispatcher(), obj_id, spec, &GetMessagesStatistic()));
    obj->SetCores(GameplayActor(), PhysicsActor(), GetActorID(), ShowActor());
    obj->m_param = bh_param;
    obj->Physics().FromMsg(ph_param);
    m_objects[obj_id] = obj;
    return true;
}

bool CBehavior::onDelObjectBehavior(const ObjID_t obj_id)
{
    DelObject(obj_id);
    return true;
}

bool CBehavior::onSetKinematicParam(const ObjID_t obj_id, const sKinematicParam &ph_kn_param)
{
    ObjectSetKinematic(obj_id, ph_kn_param);
    return true;
}

bool CBehavior::onGameActionAttack(const ObjID_t obj_id, const sCoord2 &obj_pos, const sCoord2 &target_pos)
{
    log_debug1("%s %u; %s; %s;", __PRETTY_FUNCTION__, obj_id, obj_pos.print().c_str(), target_pos.print().c_str());
    auto o_it = m_objects.find(obj_id);
    if (o_it == m_objects.end())
        return true;

    auto o = o_it->second;

    o->ActionAttack(target_pos);

    return true;
}

bool CBehavior::onSetHealth(const ObjID_t obj_id, const sHealthParam &health)
{
    if (health.health_points == 0.0)
        Send_DelObject(GameplayActor(), obj_id);
    return true;
}

bool CBehavior::onSetPhysicsState(const ObjID_t obj_id, const sPhysicsState &state)
{
    //PhysicsState() = state;
    if (state.IsDestroyed())
        Send_DelObject(GameplayActor(), obj_id);
    return true;
}

void CBehavior::onMessage()
{
    log_debug1("CBehavior::onMessage %s", m_message->print().c_str());
    bool callback_defined = ActorCallbacks_onMessage();
    assert(callback_defined);
}

void CBehavior::onTimer(const TimerID_t timer_id)
{
    log_debug3("CBehavior::onTimer %u; start", timer_id);
    if (timer_id == m_tm_1sec)
    {
        m_statistic.common.objects_count = static_cast<uint_32>(m_objects.size());
        m_statistic.common.msg_stat.FromStat(GetMessagesStatistic());
        Send_StatisticBh(ShowActor(), m_statistic);
    }
    if (timer_id == m_tm_100ms)
    {
        for (auto &o : m_objects)
        {
            auto obj = o.second;
            obj->onTimer100ms(m_timer_count);
        }
        m_timer_count++;
    }
    log_debug3("CBehavior::onTimer %u; end", timer_id);
}

void CBehavior::onStart()
{
    log_debug1("CBehavior::onStart");
    m_tm_100ms = Dispatcher()->GetTimer(GetActorID().id);
    Dispatcher()->StartTimerPeriodical(GetActorID().id, m_tm_100ms, 250);
    m_tm_1sec = Dispatcher()->GetTimer(GetActorID().id);
    Dispatcher()->StartTimerPeriodical(GetActorID().id, m_tm_1sec, 1000);
}

void CBehavior::onStop()
{
    log_debug1("CBehavior::onStop");
}

bool CBehavior::DelObject(const ObjID_t &id)
{
    log_debug3("CPhysics::DelObject id=%u;", id);
    auto obj_it = m_objects.find(id);
    if (obj_it == m_objects.end()) return false;
    m_objects.erase(obj_it);
    return true;
}

bool CBehavior::ObjectSetKinematic(const ObjID_t &id, const sKinematicParam &ph_param_kinematic)
{
    auto obj = FindObject(id);
    if (not obj) return false;
    obj->Kinematic() = ph_param_kinematic;
    return true;
}

CObjectBehaviorPtr CBehavior::FindObject(const ObjID_t &id)
{
    auto obj_it = m_objects.find(id);
    if (obj_it == m_objects.end())
        return nullptr;
    return (*obj_it).second;
}

bool CBehavior::onObjectAction(const sObjectAction &object_action)
{
    log_debug1("%s %s", __PRETTY_FUNCTION__, object_action.Print().c_str());
    switch (object_action.type)
    {
    case enObjectActionType::oaNone:
        break;
    case enObjectActionType::oaAttack:
        Send_ObjectAction(PhysicsActor(), object_action);
        break;
    case enObjectActionType::oaTake:
        break;
    }
    return true;
}

bool CBehavior::onSetPhysicsParam(const ObjID_t obj_id, const sPhysicsParamMsg &ph_param)
{
    auto obj = FindObject(obj_id);
    if (not obj) return false;
    obj->Physics().FromMsg(ph_param);
    return true;
}

bool CBehavior::onGoalEvent(const ObjID_t obj_id, const sPhysicsGoal &ph_goal)
{
    log_debug1("%s %u; %s;", __FUNCTION__, obj_id, ph_goal.Print().c_str());
    return true;
}
