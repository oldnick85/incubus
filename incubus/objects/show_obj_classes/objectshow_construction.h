#ifndef COBJECTSHOW_CONSTRUCTION_H
#define COBJECTSHOW_CONSTRUCTION_H

#include "objects/objectshow.h"

class CObjectShow_Construction
        : public CObjectShow
{
public:
    CObjectShow_Construction(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global, sGlobalShowParam* sh_global,
                             CMediaGraphicsPtr graphics, sCoreShowParam* core_show_param, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat);
};

#endif // COBJECTSHOW_CONSTRUCTION_H
