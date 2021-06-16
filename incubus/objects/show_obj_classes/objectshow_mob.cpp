#include "objectshow_mob.h"

CObjectShow_Mob::CObjectShow_Mob(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam *ph_global, sGlobalShowParam* sh_global,
                                 CMediaGraphicsPtr graphics, sCoreShowParam *core_show_param, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat)
    : CObjectShow(log, disp, ph_global, sh_global, graphics, core_show_param, spec, msg_stat)
{
    if (not spec.IsMob())
        assert(false);
}

void CObjectShow_Mob::DrawGraphics()
{
    sGraphicsDrawParam draw_param;
    draw_param.size = Size();
    draw_param.kinematic = Kinematic();
    draw_param.time_state = m_ph_global->physics_time;
    m_graphics->DrawPhysicsPart(PhysicsParts().chassis.type, draw_param);
    m_graphics->DrawPhysicsPart(PhysicsParts().manipulator.type, draw_param);
    m_graphics->DrawPhysicsPart(PhysicsParts().body.type, draw_param);
    m_graphics->DrawPhysicsPart(PhysicsParts().ganglion.type, draw_param);
}
