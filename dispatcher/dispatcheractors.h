#ifndef CDISPATCHERACTORS_H
#define CDISPATCHERACTORS_H
#include <unordered_set>

#include "i_dispatcher.h"
#include "dispatcheractor.h"

class CDispatcherActors
{
public:
    CDispatcherActors();

    uint ActorsCount() const {return static_cast<uint>(m_actors.size());}
    CDispatcherActorPtr AddActor(IActor* actor);
    bool DelActor(const ActID_t actor_id);
    bool SendMsg(const sMsg &message);

    TimerID_t AddTmr(const ActID_t a_id, const IDispatcher::enTimerType type);
    bool DelTimer(const ActID_t a_id, const TimerID_t timer_id);
    bool StartTmr(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms, const int_32 count);
    bool StopTmr(const ActID_t a_id, const TimerID_t timer_id);

    void ProcTimers(TimePoint_t &tm_goal, std::unordered_set<ActID_t> &actors_timer_event);

private:

    CDispatcherActorPtr FindActor(const ActID_t a_id);

    typedef std::unordered_map<ActID_t, CDispatcherActorPtr> DispatcherActors_t;

    std::mutex          m_mtx_actors;
    std::mutex          m_mtx_timers;
    DispatcherActors_t  m_actors;
    uint                m_new_actors_count = 0;
    ActID_t             m_cur_max_actor_id = 0;
    TimerID_t           m_cur_max_timer_id = 0;
};

typedef std::shared_ptr<CDispatcherActors>  CDispatcherActorsPtr;

#endif // CDISPATCHERACTORS_H
