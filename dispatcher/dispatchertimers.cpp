#include "dispatchertimers.h"

CDispatcherTimers::CDispatcherTimers()
{

}

TimerID_t CDispatcherTimers::AddTmr(const IDispatcher::enTimerType type, const ActID_t a_id)
{
    m_max_timer_id++;
    CDispatcherTimerPtr tm(new CDispatcherTimer(m_max_timer_id, a_id, type));
    m_timers[m_max_timer_id] = tm;
    return m_max_timer_id;
}

bool CDispatcherTimers::DelTmr(const TimerID_t timer_id)
{
    auto tm_it = m_timers.find(timer_id);
    if (tm_it == m_timers.end()) return false;
    m_timers.erase(tm_it);
    return true;
}

bool CDispatcherTimers::StartTmr(const TimerID_t tm_id, const uint_32 time_ms, const int_32 count)
{
    auto tm = FindTimer(tm_id);
    if (not tm) return false;
    return tm->Start(time_ms, count);
}

bool CDispatcherTimers::StopTmr(const TimerID_t tm_id)
{
    auto tm = FindTimer(tm_id);
    if (not tm) return false;
    return tm->Stop();
}

CDispatcherTimerPtr CDispatcherTimers::FindTimer(const TimerID_t tm_id)
{
    auto tm_it = m_timers.find(tm_id);
    if (tm_it == m_timers.end()) return nullptr;
    return (*tm_it).second;
}

void CDispatcherTimers::TimersToWork(const TimePoint_t &now, TimePoint_t &tm_goal, std::list<TimerID_t> &tm_list)
{
    tm_goal = TimePoint_t::max();
    for (auto &ttm : m_timers)
    {
        auto tm = ttm.second;
        TimePoint_t tm_g;
        if (tm->IsEvent(now, tm_g))
        {
            tm_list.push_front(tm->GetId());
        }
        if (tm_g < tm_goal) tm_goal = tm_g;
    }
}
