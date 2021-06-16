#include "objectphysics_construction.h"

CObjectPhysics_Construction::CObjectPhysics_Construction(const ILoggerPtr log, const IDispatcherPtr disp,
                                                         sGlobalPhysicsParam* ph_global, IPhysicsOtherObjectsPtr other_objects,
                                                         const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat)
    : CObjectPhysics (log, disp, ph_global, other_objects, spec, msg_stat)
{
    assert(spec.IsConstruction());
}
