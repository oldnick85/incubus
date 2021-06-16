#include "objectphysics_projectile.h"

CObjectPhysics_Projectile::CObjectPhysics_Projectile(const ILoggerPtr log, const IDispatcherPtr disp,
                                                     sGlobalPhysicsParam* ph_global, IPhysicsOtherObjectsPtr other_objects,
                                                     const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat)
    : CObjectPhysics (log, disp, ph_global, other_objects, spec, msg_stat)
{
    assert(spec.IsProjectile());
}

void CObjectPhysics_Projectile::onPositionChange(const sCoord2 &old_pos, const sCoord2 &new_pos)
{
    CObjectPhysics::onPositionChange(old_pos, new_pos);
    m_distance_moved += (new_pos - old_pos).Length();
    if (m_distance_moved >= Physics().health.max_distance_move)
        Destroy();
}
