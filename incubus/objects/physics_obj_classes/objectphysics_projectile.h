#ifndef OBJECTPHYSICS_PROJECTILE_H
#define OBJECTPHYSICS_PROJECTILE_H

#include "objects/objectphysics.h"

class CObjectPhysics_Projectile
        : public CObjectPhysics
{
public:
    CObjectPhysics_Projectile(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global,
                              IPhysicsOtherObjectsPtr other_objects, const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat);
    CObjectPhysics_Projectile(const CObjectPhysics_Projectile &other) = delete;

    virtual void onPositionChange(const sCoord2 &old_pos, const sCoord2 &new_pos) override;
private:

    Real_t          m_distance_moved = 0.0;
};

#endif // OBJECTPHYSICS_PROJECTILE_H
