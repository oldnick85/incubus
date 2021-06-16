#ifndef COBJECTSHOW_PROJECTILE_H
#define COBJECTSHOW_PROJECTILE_H

#include "objects/objectshow.h"

class CObjectShow_Projectile
        : public CObjectShow
{
public:
    CObjectShow_Projectile(const ILoggerPtr log, const IDispatcherPtr disp, sGlobalPhysicsParam* ph_global, sGlobalShowParam* sh_global,
                           CMediaGraphicsPtr graphics, sCoreShowParam* core_show_param, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat);
    virtual ~CObjectShow_Projectile() override = default;
private:
    virtual void DrawPhysics() override;
};
using CObjectShow_ProjectilePtr = std::shared_ptr<CObjectShow_Projectile>;

#endif // COBJECTSHOW_PROJECTILE_H
