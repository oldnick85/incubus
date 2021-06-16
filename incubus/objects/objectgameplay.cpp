#include "objectgameplay.h"
#include <cstring>

CObjectGameplay::CObjectGameplay(const ILoggerPtr &log, const IDispatcherPtr disp, const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat)
    : CObject(spec)
    , CLogged(log)
    , CMessageSender(disp, msg_stat)
{

}

void CObjectGameplay::CreateSubobjects()
{
    Send_NewObjectPhysics(PhysicsActor(), GetID(), ObjSpec(), m_ph_param);
    Send_NewObjectBehavior(BehaviorActor(), GetID(), ObjSpec(), m_bh_param, m_ph_param);
    Send_NewObjectShow(ShowActor(), GetID(), ObjSpec(), m_sh_param, m_ph_param);
}

void CObjectGameplay::DeleteSubobjects()
{
    Send_DelObjectPhysics(PhysicsActor(), GetID());
    Send_DelObjectBehavior(BehaviorActor(), GetID());
    Send_DelObjectShow(ShowActor(), GetID());
}
