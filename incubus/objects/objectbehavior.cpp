#include "objectbehavior.h"

CObjectBehavior::CObjectBehavior(const ILoggerPtr &log, const IDispatcherPtr &disp, ObjID_t id, const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat)
    : CObject(spec)
    , CLogged(log)
    , CMessageSender(disp, msg_stat)
{
    SetID(id);
    SetSelfObjId(GetID());
}

void CObjectBehavior::onTimer100ms(const uint_32 timer_count)
{
    return;
    if (ObjSpec().Spec() != enObjSpec::opZombie) return;
    if ((timer_count % 10) != (static_cast<uint>(rand()) % 10)) return;
    sVect2 v;
    if ((abs(Position().x.Val()) < 0.5) and (abs(Position().y.Val()) < 0.5))
    {
        v.vx = ((rand() % 200) - 100);
        v.vx /= 1000;
        v.vy = ((rand() % 200) - 100);
        v.vy /= 1000;
    }
    else
    {
        double k = ((rand() % 10) + 10);
        v.vx = -Position().x/k;
        v.vy = -Position().y/k;
        if (v.Length() > 0.2)
        {
            v.Normalize(0.2);
        }
    }
    auto d = v;
    d.Normalize();
    Send_SetVelocity(BehaviorActor(), PhysicsActor(), v, d);
}

bool CObjectBehavior::ActionAttack(const sCoord2 &target_pos)
{
    sObjectAction obj_action;
    obj_action.SetAttack(GetID(), target_pos);
    Send_ObjectAction(PhysicsActor(), obj_action);
    return true;
}
