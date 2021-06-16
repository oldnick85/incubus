#ifndef CDISPATCERTIMERS_H
#define CDISPATCERTIMERS_H

#include "dispatchertimer.h"
#include <list>

class CDispatcherTimers
{
public:
    CDispatcherTimers();

    TimerID_t AddTmr(const IDispatcher::enTimerType type, const ActID_t a_id);
    bool DelTmr(const TimerID_t timer_id);
    bool StartTmr(const TimerID_t tm_id, const uint_32 time_ms, const int_32 count);
    bool StopTmr(const TimerID_t tm_id);
    void TimersToWork(const TimePoint_t &now, TimePoint_t &tm_goal, std::list<TimerID_t> &tm_list);
protected:

private:
    CDispatcherTimerPtr FindTimer(const TimerID_t tm_id);
    std::unordered_map<TimerID_t, CDispatcherTimerPtr> m_timers;
    TimerID_t       m_max_timer_id = 0;
};

#endif // CDISPATCERTIMERS_H
