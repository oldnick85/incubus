#include "dispatcheractors.h"

CDispatcherActors::CDispatcherActors()
{

}

CDispatcherActorPtr CDispatcherActors::AddActor(IActor *actor)
{
    std::lock_guard<std::mutex> locker(m_mtx_actors);
    actor->GetActorID().id = m_cur_max_actor_id;

    CDispatcherActorPtr dsp_act(new CDispatcherActor(actor));
    m_actors[m_cur_max_actor_id] = dsp_act;

    m_cur_max_actor_id++;
    m_new_actors_count++;
    return dsp_act;
}

bool CDispatcherActors::DelActor(const ActID_t actor_id)
{
    std::lock_guard<std::mutex> locker(m_mtx_actors);
    auto act_it = m_actors.find(actor_id);
    if (act_it == m_actors.end()) return false;
    auto act_p = (*act_it).second;
    m_actors.erase(act_it);
    return true;
}

bool CDispatcherActors::SendMsg(const sMsg &message)
{
    auto act = FindActor(message.destination.id);
    if (not act) return false;
    return act->SendMsg(message);
}

TimerID_t CDispatcherActors::AddTmr(const ActID_t a_id, const IDispatcher::enTimerType type)
{
    auto act = FindActor(a_id);
    if (not act) return TIMER_ID_NONE;
    return act->AddTmr(type, a_id);
}

bool CDispatcherActors::DelTimer(const ActID_t a_id, const TimerID_t timer_id)
{
    auto act = FindActor(a_id);
    if (not act) return false;
    return act->DelTmr(timer_id);
}

bool CDispatcherActors::StartTmr(const ActID_t a_id, const TimerID_t timer_id, const uint_32 time_ms, const int_32 count)
{
    auto act = FindActor(a_id);
    if (not act) return false;
    return act->StartTmr(timer_id, time_ms, count);
}

bool CDispatcherActors::StopTmr(const ActID_t a_id, const TimerID_t timer_id)
{
    auto act = FindActor(a_id);
    if (not act) return false;
    return act->StopTimer(timer_id);
}

CDispatcherActorPtr CDispatcherActors::FindActor(const ActID_t a_id)
{
    auto a_it = m_actors.find(a_id);
    if (a_it == m_actors.end()) return nullptr;
    return (*a_it).second;
}

void CDispatcherActors::ProcTimers(TimePoint_t &tm_goal, std::unordered_set<ActID_t> &actors_timer_event)
{
    tm_goal = TimePoint_t::max();
    TimePoint_t now = std::chrono::system_clock::now();
    for (auto &aact : m_actors)
    {
        auto act = aact.second;
        std::list<TimerID_t> tm_l;
        TimePoint_t tm_g;
        act->TimersToWork(now, tm_g, tm_l);
#ifdef INCUBUS_LOG_DEBUG_DISPATCHER
        auto tm_ms = std::chrono::duration_cast<std::chrono::microseconds>(tm_g - now).count();
        printf("CDispatcherActors::ProcTimers act_id=%d; tm_l_size=%lu; %ld;\n",
               act->Actor()->GetActorID().id, tm_l.size(), tm_ms);
#endif
        if (tm_g < tm_goal) tm_goal = tm_g;
        if (not tm_l.empty())
            actors_timer_event.insert(act->Actor()->GetActorID().id);
        for (auto &tm_id : tm_l)
            act->PushTimerToWork(tm_id);
    }
}
