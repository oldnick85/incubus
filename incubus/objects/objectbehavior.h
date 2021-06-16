#ifndef OBJECTBEHAVIOR_H
#define OBJECTBEHAVIOR_H
#include "../logger/i_logger.h"
#include "object.h"
#include "cores/messages/messages.h"
#include "cores/messages/messagesender.h"
#include "objects/behavior_obj_classes/ibhobjclass.h"

class CObjectBehavior
        : public CObject
        , public CCoresParam
        , public CPhysicsParam
        , private CLogged
        , public CMessageSender
{
public:
    CObjectBehavior(const ILoggerPtr &log, const IDispatcherPtr &disp, ObjID_t id, const sObjSpec &spec, sDispatcherMsgStatistic* msg_stat);
    CObjectBehavior(const CObjectBehavior &other) = delete;

    bool ActionAttack(const sCoord2 &target_pos);

    sObjBhParam     m_param;
    void onTimer100ms(const uint_32 timer_count);
private:
    std::shared_ptr<IBhObjClass>    m_class;
};
using CObjectBehaviorPtr = std::shared_ptr<CObjectBehavior>;


#endif // OBJECTBEHAVIOR_H
