#ifndef OBJECTPHYSICS_LOOT_H
#define OBJECTPHYSICS_LOOT_H

#include "objects/objectphysics.h"

class CObjectPhysics_Loot
        : public CObjectPhysics
{
public:
    CObjectPhysics_Loot(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global,
                        IPhysicsOtherObjectsPtr other_objects, const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat);
    CObjectPhysics_Loot(const CObjectPhysics_Loot &other) = delete;
private:
    virtual void DoActionAttack(const sCoord2 &target_pos) override;
};

#endif // OBJECTPHYSICS_LOOT_H
