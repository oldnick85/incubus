#include "dispatchertimer.h"

CDispatcherTimer::CDispatcherTimer(const TimerID_t t_id, const ActID_t a_id, const IDispatcher::enTimerType type)
{
    m_timer_id = t_id;
    m_actor_id = a_id;
    m_type = type;
}

bool CDispatcherTimer::Start(const uint_32 time_ms, const int_32 count)
{
    m_goal_count = count;
    m_duration_ms = time_ms;
    m_start_time = std::chrono::high_resolution_clock::now();
    m_goal_time = m_start_time + std::chrono::milliseconds(m_duration_ms);
    m_is_started = true;
    return true;
}

bool CDispatcherTimer::Stop()
{
    if (not m_is_started) return false;
    m_goal_count = 0;
    m_is_started = false;
    return true;
}

bool CDispatcherTimer::IsEvent(const TimePoint_t &now, TimePoint_t &tm_goal)
{
#ifdef INCUBUS_LOG_DEBUG_DISPATCHER
        printf("CDispatcherTimer::IsEvent tm=%d; act=%d; %s;\n", m_timer_id, m_actor_id, m_is_started ? "started" : "stopped");
#endif
    if (not m_is_started) {tm_goal = TimePoint_t::max(); return false;}
    if (m_goal_time > now) {tm_goal = m_goal_time; return false;}
    switch (m_type)
    {
    case IDispatcher::enTimerType::ttStrict:
        m_goal_time += std::chrono::milliseconds(m_duration_ms);
        break;
    case IDispatcher::enTimerType::ttSkipping:
        while (m_goal_time < now)
            m_goal_time += std::chrono::milliseconds(m_duration_ms);
        break;
    case IDispatcher::enTimerType::ttContinuing:
        m_goal_time = now + std::chrono::milliseconds(m_duration_ms);
        break;
    }
    tm_goal = m_goal_time;
    if (m_goal_count > 0) m_goal_count--;
    if (m_goal_count == 0) Stop();
    return true;
}
