#include "physics.h"

CPhysics::CPhysics(const ILoggerPtr log, const IDispatcherPtr disp)
    : CMessageCallbacks(log, "PHYSICS")
    , CLogged(log)
    , CMessageSender(disp, &GetMessagesStatistic())
{
    log_debug1("CPhysics::CGameplay");
    m_id.type = enActor::actPhysics;
    Dispatcher()->RegisterActor(this);
    SetSelfActor(GetActorID());
}

CPhysics::~CPhysics()
{
    log_debug1("CPhysics::~CGameplay");
    Dispatcher()->UnregisterActor(this);
}

bool CPhysics::onNewObjectPhysics(const ObjID_t obj_id, const sObjSpec &spec, const sPhysicsParamMsg &ph_param)
{
    sPhysicsParam ph_p;
    ph_p.FromMsg(ph_param);
    AddObject(obj_id, spec, ph_p);
    return true;
}

bool CPhysics::onDelObjectPhysics(const ObjID_t obj_id)
{
    DelObject(obj_id);
    return true;
}

bool CPhysics::onSetVelocity(const ObjID_t obj_id, const sVect2 &velocity, const sVect2 &direction)
{
    ObjectSetVelocity(obj_id, velocity);
    ObjectSetDirection(obj_id, direction);
    return true;
}

bool CPhysics::onRequestPhysicsParamAll(const uint_8 flags)
{
    if (flags & sMsgRequestPhysicsParamAll::enRequestFlags::rfKinematic)
        SendKinematicParamAll();
    return true;
}

CObjectPhysics* CPhysics::FindObject(const ObjID_t obj_id)
{
    auto obj_it = m_objects.find(obj_id);
    if (obj_it == m_objects.end()) return nullptr;
    return obj_it->second;
}

bool CPhysics::onSetPhysicsGoal(const ObjID_t obj_id, const sPhysicsGoal &ph_goal)
{
    CObjectPhysics* obj = FindObject(obj_id);
    if (not obj)
    {
        log_warning("cant find object id=%d;", obj_id);
        return true;
    }
    obj->Goal() = ph_goal;
    return true;
}

bool CPhysics::onObjectAction(const sObjectAction &object_action)
{
    CObjectPhysics* obj = FindObject(object_action.obj_id);
    if (not obj) return true;
    obj->DoAction(object_action);
    return true;
}

void CPhysics::onMessage()
{
    log_debug3("CPhysics::onMessage %s", m_message->print().c_str());
    bool callback_defined = ActorCallbacks_onMessage();
    assert(callback_defined);
}

void CPhysics::SendKinematicParamAll()
{
    for (auto &o : m_objects)
    {
        CObjectPhysics* obj = o.second;
        obj->SendNewKinematic();
    }
}

void CPhysics::onTimer(const TimerID_t timer_id)
{
    //log_debug3("CPhysics::onTimer %u; start", timer_id);
    if (timer_id == m_tm_1sec)
    {
        m_statistic.common.objects_count = static_cast<uint_32>(m_objects.size());
        m_statistic.common.msg_stat.FromStat(GetMessagesStatistic());
        Send_StatisticPh(ShowActor(), m_statistic);
        auto s = PrintSenderStatistic();
        log_warning("CPhysics sender statistic\n%s", s.c_str());
    }
    if (timer_id == m_tm_move)
    {
        auto t_d = m_ph_global.time_delta*m_ph_global.time_speed;
        ProcGoals();
        ProcCollisionsProjectiles(t_d);
        ProcMove(t_d);
        ProcCollisions();
        AddToGlobalTime(t_d);
    }
    log_debug3("CPhysics::onTimer %u; end", timer_id);
}

void CPhysics::AddToGlobalTime(const double t_d)
{
    m_ph_global.physics_time += t_d;
    Send_GlobalPhysicsParam(ShowActor(), m_ph_global);
}

void CPhysics::ProcMove(const double time_delta)
{
    for (auto &o : m_objects)
    {
        CObjectPhysics* obj = o.second;
        //if (obj->Physics().ph_class == pcObstacle) continue;
        if (obj->PhysicsState().IsDestroyed()) continue;
        sVect2 pos_delta = obj->Velocity();
        pos_delta.Scale(time_delta);
        auto old_pos = obj->Position();
        auto new_pos = old_pos + pos_delta;
        if (new_pos.x > m_ph_global.max_coord.x)
            new_pos.x -= m_ph_global.max_coord.x*2.0;
        if (new_pos.x < -m_ph_global.max_coord.x)
            new_pos.x += m_ph_global.max_coord.x*2.0;
        if (new_pos.y > m_ph_global.max_coord.y)
            new_pos.y -= m_ph_global.max_coord.y*2.0;
        if (new_pos.y < -m_ph_global.max_coord.y)
            new_pos.y += m_ph_global.max_coord.y*2.0;
        obj->SetNewPosition(new_pos);
    }
}

void CPhysics::ProcCollisions()
{
    ProcCollisionsMobs();
}

void CPhysics::ProcCollisionsMobs()
{
    auto o1_it = m_objects.begin();
    while (o1_it != m_objects.end())
    {
        CObjectPhysics* o1 = o1_it->second;
        o1_it++;
        if (not o1->ObjSpec().IsMob()) continue;
        if (not o1->Physics().CollisionsInvolved()) continue;

        auto o2_it = m_objects.begin();
        while (o2_it != m_objects.end())
        {
            CObjectPhysics* o2 = o2_it->second;
            o2_it++;
            if ((not o2->ObjSpec().IsMob()) and (not o2->ObjSpec().IsConstruction())) continue;
            if (not o2->Physics().CollisionsInvolved()) continue;
            ProcCollisionMob(o1, o2);
        }
    }
}

void CPhysics::ProcCollisionsProjectiles(const double time_delta)
{
    auto obj_projectile_it = m_objects.begin();
    while (obj_projectile_it != m_objects.end())
    {
        CObjectPhysics* obj_projectile = obj_projectile_it->second;
        obj_projectile_it++;
        if (not obj_projectile->ObjSpec().IsProjectile()) continue;
        if (not obj_projectile->Physics().CollisionsInvolved()) continue;

        std::list<CObjectPhysics*> possible_objects;

        auto obj_another_it = m_objects.begin();
        while (obj_another_it != m_objects.end())
        {
            CObjectPhysics* obj_another = obj_another_it->second;
            if ((obj_another->Physics().CollisionsInvolved()) and (not obj_another->ObjSpec().IsLoot()) and (not obj_another->ObjSpec().IsProjectile()))
            {
                bool clsn = obj_another->Shape().PotentiallyCollideMovingPoint(obj_another->Position(), obj_projectile->Position(), obj_projectile->Velocity()*time_delta, nullptr);
                if (clsn) possible_objects.push_back(obj_another);
            }
            obj_another_it++;
        }

        CObjectPhysics* obj_cldd = nullptr;
        double min_distance = 0.0;
        sCollision collision_match;
        for (CObjectPhysics* o2 : possible_objects)
        {
            sCollision collision;
            bool clsn = o2->Shape().CollisionMovingPoint(o2->Position(), obj_projectile->Position(), obj_projectile->Velocity()*time_delta, &collision);
            if (clsn)
            {
                auto l = (collision.point - obj_projectile->Position()).Length2();
                if ((obj_cldd == nullptr) or (l < min_distance))
                {
                    min_distance = l;
                    obj_cldd = o2;
                    collision_match = collision;
                }
            }
        }

        if (obj_cldd != nullptr)
        {
            ProcCollisionProjectile(obj_projectile, obj_cldd, collision_match);
        }
    }
}

void CPhysics::ProcCollisionProjectile(CObjectPhysics *obj_projectile, CObjectPhysics *obj_another, const sCollision &collision)
{
    log_warning("ProcCollisionProjectile %s and %s",
                obj_projectile->ObjSpec().Print().c_str(), obj_another->ObjSpec().Print().c_str());
    obj_projectile->SetNewPosition(collision.point);
    Real_t collision_energy = obj_projectile->Physics().material.mass*obj_projectile->Velocity().Length2()/2.0;
    log_warning("collision: obj1=%i(%s); obj2=%i(%s); energy=%.3f;",
                obj_projectile->GetID(), obj_projectile->ObjSpec().Print().c_str(),
                obj_another->GetID(), obj_another->ObjSpec().Print().c_str(), collision_energy.Val());
    log_warning("mass=%.4f", obj_projectile->Physics().material.mass.Val());
    log_warning("%s is %s", obj_projectile->ObjSpec().Print().c_str(), obj_projectile->PhysicsState().print().c_str());
    log_warning("%s is %s", obj_another->ObjSpec().Print().c_str(), obj_another->PhysicsState().print().c_str());
    obj_projectile->SetHealthDamage(collision_energy);
    obj_another->SetHealthDamage(collision_energy);
    log_warning("%s is %s", obj_projectile->ObjSpec().Print().c_str(), obj_projectile->PhysicsState().print().c_str());
    log_warning("%s is %s", obj_another->ObjSpec().Print().c_str(), obj_another->PhysicsState().print().c_str());

    sSoundParam sound_param;
    sound_param.type = stCollisionBullet;
    sound_param.snd_pos = collision.point;
    Send_SoundEvent(ShowActor(), sound_param);
}

void CPhysics::ProcCollisionMob(CObjectPhysics* obj1, CObjectPhysics* obj2)
{
    if (obj1 == obj2) return;
    sCollision collision;
    bool clsn;

    if (not obj1->Shape().PotentiallyCollide(&obj2->Shape(), obj1->Position(), obj2->Position()))
        return;

    clsn = obj1->Shape().Collision(&obj2->Shape(), obj1->Position(), obj2->Position(), &collision);
    if (clsn)
    {
        auto m1 = obj1->Physics().material.mass;
        auto m2 = obj2->Physics().material.mass;
        auto m = m1 + m2;

        if (collision.direction.IsNul())
        {
            double vx = (static_cast<double>(rand() % 1000))/1000.0;
            double vy = (static_cast<double>(rand() % 1000))/1000.0;
            collision.direction = {vx, vy};
        }

        sVect2 v1 = collision.direction;
        v1.Scale(-collision.depth*m2/m);
        auto pos1 = obj1->Position() + v1;
        obj1->SetNewPosition(pos1);

        sVect2 v2 = collision.direction;
        v2.Scale(collision.depth*m1/m);
        auto pos2 = obj2->Position() + v2;
        obj2->SetNewPosition(pos2);
    }
}


void CPhysics::onStart()
{
    log_debug1("CPhysics::onStart");
    m_tm_move = Dispatcher()->GetTimer(GetActorID().id, IDispatcher::enTimerType::ttContinuing);
    Dispatcher()->StartTimerPeriodical(GetActorID().id, m_tm_move, static_cast<uint>((m_ph_global.time_delta*1000.0).Val()));
    m_tm_1sec = Dispatcher()->GetTimer(GetActorID().id, IDispatcher::enTimerType::ttContinuing);
    Dispatcher()->StartTimerPeriodical(GetActorID().id, m_tm_1sec, 1000);
}

void CPhysics::onStop()
{
    log_debug1("CPhysics::onStop");
}

bool CPhysics::AddObject(const ObjID_t &id, const sObjSpec &spec, const sPhysicsParam &param)
{
    log_warning("CPhysics::AddObject id=%u; %s;", id, param.print().c_str());
    CObjectPhysics* obj = nullptr;
    if (spec.IsMob())
        obj = new CObjectPhysics_Mob(m_log, Dispatcher(), &m_ph_global, IPhysicsOtherObjectsPtr(this), spec, &GetMessagesStatistic());
    else if (spec.IsProjectile())
        obj = new CObjectPhysics_Projectile(m_log, Dispatcher(), &m_ph_global, IPhysicsOtherObjectsPtr(this), spec, &GetMessagesStatistic());
    else if (spec.IsLoot())
        obj = new CObjectPhysics_Loot(m_log, Dispatcher(), &m_ph_global, IPhysicsOtherObjectsPtr(this), spec, &GetMessagesStatistic());
    else if (spec.IsConstruction())
        obj = new CObjectPhysics_Construction(m_log, Dispatcher(), &m_ph_global, IPhysicsOtherObjectsPtr(this), spec, &GetMessagesStatistic());
    else
        assert(false);
    obj->SetID(id);
    obj->SetCores(GameplayActor(), GetActorID(), BehaviorActor(), ShowActor());
    obj->Physics() = param;
    m_objects[id] = obj;
    return true;
}

bool CPhysics::DelObject(const ObjID_t &id)
{
    log_warning("CPhysics::DelObject id=%u;", id);
    m_objects.erase(id);
    return true;
}

bool CPhysics::ObjectSetVelocity(const ObjID_t &id, const sVect2 &velocity)
{
    log_debug3("CPhysics::ObjectSetVelocity id=%u; %s;", id, velocity.print().c_str());
    CObjectPhysics* obj = FindObject(id);
    if (not obj) return false;
    obj->Velocity() = velocity;
    return true;
}

bool CPhysics::ObjectSetDirection(const ObjID_t &id, const sVect2 &direction)
{
    log_debug3("CPhysics::ObjectSetDirection id=%u; %s;", id, direction.print().c_str());
    CObjectPhysics* obj = FindObject(id);
    if (not obj) return false;
    obj->Direction() = direction;
    return true;
}


void CPhysics::ProcGoals()
{
    for (auto &o : m_objects)
    {
        CObjectPhysics* obj = o.second;
        ProcGoal(obj);
    }
}

void CPhysics::ProcGoal(CObjectPhysics* ph_obj)
{
    ph_obj->CheckGoal();
    switch (ph_obj->Goal().goal)
    {
    case pgGoToPoint:
    {
        sVect2 v = ph_obj->Goal().destination - ph_obj->Position();
        v.Normalize(2.0);
        ph_obj->Velocity() = v;
        break;
    }
    case pgNone:
        break;
    default:
        break;
    }
}

bool CPhysics::GetPhysicsObject(const ObjID_t obj_id, CObjectPhysics** ph_obj)
{
    CObjectPhysics* obj = FindObject(obj_id);
    if (not obj) return false;
    if (ph_obj)
        *ph_obj = obj;
    return true;
}

bool CPhysics::GetPhysicsParam(const ObjID_t obj_id, sPhysicsParam &ph_param)
{
    CObjectPhysics* obj = FindObject(obj_id);
    if (not obj) return false;
    ph_param = obj->Physics();
    return true;
}

bool CPhysics::GetPhysicsMaterialParam(const ObjID_t obj_id, sMaterialParam &mt_param)
{
    CObjectPhysics* obj = FindObject(obj_id);
    if (not obj) return false;
    mt_param = obj->Material();
    return true;
}
