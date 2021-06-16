#ifndef COBJECTPHYSICS_CONSTRUCTION_H
#define COBJECTPHYSICS_CONSTRUCTION_H

#include "objects/objectphysics.h"

class CObjectPhysics_Construction
        : public CObjectPhysics
{
public:
    CObjectPhysics_Construction(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global,
                                IPhysicsOtherObjectsPtr other_objects, const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat);
    CObjectPhysics_Construction(const CObjectPhysics_Construction &other) = delete;

};

#endif // COBJECTPHYSICS_CONSTRUCTION_H
