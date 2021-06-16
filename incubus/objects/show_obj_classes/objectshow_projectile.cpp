#include "objectshow_projectile.h"

CObjectShow_Projectile::CObjectShow_Projectile(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam *ph_global, sGlobalShowParam* sh_global,
                                               CMediaGraphicsPtr graphics, sCoreShowParam *core_show_param, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat)
    : CObjectShow(log, disp, ph_global, sh_global, graphics, core_show_param, spec, msg_stat)
{
    if (not spec.IsProjectile())
        assert(false);
}

void CObjectShow_Projectile::DrawPhysics()
{
    bool draw_trace = (m_sh_global->HeightFly());
    if (draw_trace) DrawTrace();
    CObjectShow::DrawPhysics();
}
