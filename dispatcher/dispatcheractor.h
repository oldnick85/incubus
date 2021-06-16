#ifndef CDISPATCHERACTOR_H
#define CDISPATCHERACTOR_H

#include <list>
#include <atomic>
#include <mutex>
#include "i_actor.h"
#include "dispatchertimers.h"
#include "dispatcher_common.h"

/**
 * @brief CDispatcherActor - структура актора внутри диспетчера
 */

class CDispatcherActor
        : public CDispatcherTimers
{
public:
    CDispatcherActor(IActor *actor);

    enum enState
    {
        stNew,
        stNormal,
        stDeleted,
    };

    CDispatcherActor& operator= (const CDispatcherActor &da);

    bool SendMsg(const sMsg &msg);
    bool StopTimer(const TimerID_t timer_id);
    IActor* Actor();

    sMsgPtr PopMsg();

    void PushTimerToWork(const TimerID_t tm_id);
    TimerID_t PopTimerToWork();

    void SetNew() {m_state = stNew;}
    void SetNormal() {m_state = stNormal;}
    void SetDeleted() {m_state = stDeleted;}
    bool IsNew() const {return (m_state == stNew);}
    bool IsNormal() const {return (m_state == stNormal);}
    bool IsDeleted() const {return (m_state == stDeleted);}

private:
    IActor*                 m_actor;
    sActorID                m_actor_id;
    enState                 m_state;
    std::atomic<bool>       m_working;
    std::mutex              m_mtx_messages;
    std::list<sMsgPtr>      m_messages;
    std::mutex              m_mtx_timers_to_work;
    std::list<TimerID_t>    m_timers_to_work;
};

typedef std::shared_ptr<CDispatcherActor>  CDispatcherActorPtr;

#endif // CDISPATCHERACTOR_H
