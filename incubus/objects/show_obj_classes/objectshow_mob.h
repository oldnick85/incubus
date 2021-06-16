#ifndef COBJECTSHOW_MOB_H
#define COBJECTSHOW_MOB_H

#include "objects/objectshow.h"

class CObjectShow_Mob
        : public CObjectShow
{
public:
    CObjectShow_Mob(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global, sGlobalShowParam* sh_global,
                    CMediaGraphicsPtr graphics, sCoreShowParam* core_show_param, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat);

private:
    virtual void DrawGraphics() override;
};

#endif // COBJECTSHOW_MOB_H
