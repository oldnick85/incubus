#ifndef OBJECTGAMEPLAY_H
#define OBJECTGAMEPLAY_H
#include "../logger/i_logger.h"
#include "cores/messages/messagesender.h"
#include "object.h"

class CObjectGameplay
        : public CObject
        , public CCoresParam
        , private CLogged
        , public CMessageSender
{
public:
    CObjectGameplay(const ILoggerPtr &log, const IDispatcherPtr disp, const sObjSpec &spec, sDispatcherMsgStatistic *msg_stat);
    CObjectGameplay(const CObjectGameplay &other) = delete;

    void CreateSubobjects();
    void DeleteSubobjects();

    sObjGpParam     m_param;
    sPhysicsParam   m_ph_param;
    sShowParam      m_sh_param;
    sBehaviorParam  m_bh_param;

    bool            m_wait_for_delete = false;
private:
};
using CObjectGameplayPtr = std::shared_ptr<CObjectGameplay>;

#endif // OBJECTGAMEPLAY_H
