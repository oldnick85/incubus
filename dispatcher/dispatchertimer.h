#ifndef CDISPATCHERTIMER_H
#define CDISPATCHERTIMER_H

#include "dispatcher_common.h"
#include "i_dispatcher.h"

/**
 * @brief sDispTimer - класс таймера в диспетчере
 */
class CDispatcherTimer
{
public:

    CDispatcherTimer() = delete;
    CDispatcherTimer(const TimerID_t t_id, const ActID_t a_id, const IDispatcher::enTimerType type);

    bool Start(const uint_32 time_ms, const int_32 count);
    bool Stop();

    bool IsStarted() const {return m_is_started;}
    TimePoint_t GoalTime() const {return  m_goal_time;}

    bool IsEvent(const TimePoint_t &now, TimePoint_t &tm_goal);

    TimerID_t GetId() const {return m_timer_id;}

private:
    TimePoint_t                 m_start_time;                                   ///< время запуска таймера
    TimePoint_t                 m_goal_time;                                    ///< время следующего срабатывания
    uint_32                     m_duration_ms;                                  ///< время срабатывания таймера
    TimerID_t                   m_timer_id;                                     ///< идентификатор таймера
    ActID_t                     m_actor_id;                                     ///< актор, зарегистрировавший таймер
    int_32                      m_goal_count = 0;                               ///< сколько осталось раз сработать
    IDispatcher::enTimerType    m_type = IDispatcher::enTimerType::ttStrict;    ///< тип таймера
    bool                        m_is_started = false;                           ///< запущен ли таймер
};

typedef std::shared_ptr<CDispatcherTimer>  CDispatcherTimerPtr;

#endif // CDISPATCHERTIMER_H
