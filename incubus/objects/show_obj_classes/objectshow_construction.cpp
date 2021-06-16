#include "objectshow_construction.h"

CObjectShow_Construction::CObjectShow_Construction(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam *ph_global, sGlobalShowParam* sh_global,
                                                   CMediaGraphicsPtr graphics, sCoreShowParam *core_show_param, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat)
    : CObjectShow(log, disp, ph_global, sh_global, graphics, core_show_param, spec, msg_stat)
{
    if (not spec.IsConstruction())
        assert(false);
}
