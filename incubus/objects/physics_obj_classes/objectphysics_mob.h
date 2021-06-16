#ifndef OBJECTPHYSICS_MOB_H
#define OBJECTPHYSICS_MOB_H

#include "objects/objectphysics.h"

class CObjectPhysics_Mob
        : public CObjectPhysics
{
public:
    CObjectPhysics_Mob(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global,
                       IPhysicsOtherObjectsPtr other_objects, const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat);
    CObjectPhysics_Mob(const CObjectPhysics_Mob &other) = delete;
private:
    virtual void DoActionAttack(const sCoord2 &target_pos) override;
};

#endif // OBJECTPHYSICS_MOB_H
