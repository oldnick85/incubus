#ifndef COBJECTSHOW_LOOT_H
#define COBJECTSHOW_LOOT_H

#include "objects/objectshow.h"

class CObjectShow_Loot
        : public CObjectShow
{
public:
    CObjectShow_Loot(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global, sGlobalShowParam* sh_global,
                     CMediaGraphicsPtr graphics, sCoreShowParam* core_show_param, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat);
};

#endif // COBJECTSHOW_LOOT_H
