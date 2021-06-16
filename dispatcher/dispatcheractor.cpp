#include "dispatcheractor.h"

CDispatcherActor::CDispatcherActor(IActor *actor)
{
    m_actor = actor;
    m_actor_id = actor->GetActorID();

    m_working = false;
    m_state = stNew;
}

CDispatcherActor& CDispatcherActor::operator= (const CDispatcherActor &da)
{
    m_actor = da.m_actor;
    m_actor_id = da.m_actor_id;
    m_working.store(da.m_working);
    return *this;
}

bool CDispatcherActor::SendMsg(const sMsg &msg)
{
    std::lock_guard<std::mutex> locker(m_mtx_messages);
    sMsgPtr m(new sMsg(msg));
    m_messages.push_back(m);
    return true;
}

sMsgPtr CDispatcherActor::PopMsg()
{
    std::lock_guard<std::mutex> locker(m_mtx_messages);
    if (m_messages.empty()) return nullptr;
    auto m = m_messages.front();
    m_messages.pop_front();
    return m;
}

bool CDispatcherActor::StopTimer(const TimerID_t timer_id)
{
    return StopTmr(timer_id);
}

IActor* CDispatcherActor::Actor()
{
    return m_actor;
}

void CDispatcherActor::PushTimerToWork(const TimerID_t tm_id)
{
    std::lock_guard<std::mutex> locker(m_mtx_timers_to_work);
    m_timers_to_work.push_back(tm_id);
}

TimerID_t CDispatcherActor::PopTimerToWork()
{
    std::lock_guard<std::mutex> locker(m_mtx_timers_to_work);
    if (m_timers_to_work.empty()) return TIMER_ID_NONE;
    auto tm_id = m_timers_to_work.front();
    m_timers_to_work.pop_front();
    return tm_id;
}
